#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
#include "EnemyBoss.h"
#include "EffectManager.h"
#include "Input/Input.h"
#include <DirectXMath.h>
#include "StageMain.h"
#include "Actor.h"
#include "Collision.h"
#include "Movement.h"
#include "HP.h"
#include "ModelControll.h"
#include "ProjectileStraight.h"
#include "ProjectileImpact.h"
#include "Transform.h"
#include "Ui.h"
#include "TransForm2D.h"
#include "UiManager.h"
#include "LightManager.h"
#include "StateDerived.h"
#include "SceneLoading.h"
#include "SceneManager.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"
#include "Audio\Audio.h"
#include "UiTime.h"
#include "Mp.h"

// シャドウマップのサイズ
static const UINT SHADOWMAP_SIZE = 2048;
// 初期化
void SceneGame::Initialize()
{
	// 行動範囲
	{
		minPos.x = -30;
		minPos.y = -3.525f;
		minPos.z = -30;

		maxPos.x = 30;
		maxPos.y = 3.625f;
		maxPos.z = 30;
	}

		// カメラ初期化
	//cameraControlle = nullptr;
	//cameraControlle = new CameraController();
	cameraControlle = std::make_unique<CameraController>();

	// コンポネント登録
	InitializeComponent();

	// カメラ初期設定 見える位置追いかけるものなど
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)

	);
	// どの範囲をどれだけ見るか奥行含め
	camera.SetPerspedtiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	// 平行光源を追加
	{
		mainDirectionalLight = new Light(LightType::Directional);
		mainDirectionalLight->SetDirection({ 1,-3,-1 });
		LightManager::Instanes().Register(mainDirectionalLight);
	}

	// 点光源を追加
	{
		Light* light = new Light(LightType::Point);
		light->SetPosition(DirectX::XMFLOAT3(-40.000, -3, 1));
		light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		light->SetRange(lightRange);
		LightManager::Instanes().Register(light);

		Light* lightOne = new Light(LightType::Point);
		lightOne->SetPosition(DirectX::XMFLOAT3(40.000, -3, 1));
		lightOne->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		lightOne->SetRange(lightRange);
		LightManager::Instanes().Register(lightOne);


		Light* lightSeconde = new Light(LightType::Point);
		lightSeconde->SetPosition(DirectX::XMFLOAT3(1, -3, -40.000));
		lightSeconde->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		lightSeconde->SetRange(lightRange);
		LightManager::Instanes().Register(lightSeconde);


		Light* lightTherd = new Light(LightType::Point);
		lightTherd->SetPosition(DirectX::XMFLOAT3(1, -3, 40.000));
		lightTherd->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		lightTherd->SetRange(lightRange);
		LightManager::Instanes().Register(lightTherd);
	}

	// 新しい描画ターゲットの生成
	{
		Graphics& graphics = Graphics::Instance();
		//renderTarget = new RenderTarget(static_cast<UINT>(graphics.GetScreenWidth())
		renderTarget = std::make_unique<RenderTarget>(static_cast<UINT>(graphics.GetScreenWidth())
			, static_cast<UINT>(graphics.GetScreenHeight())
			, DXGI_FORMAT_R8G8B8A8_UNORM);
	}

	// シャドウマップ用に深度ステンシルの生成
	{
		shadowmapDepthStencil = std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	}

	// ポストプロセス描画クラス生成
	PostProcessingRendererInitialize();
}

void SceneGame::Start()
{
	// bgm開始
	StartBgm();
}


// 終了化
void SceneGame::Finalize()
{
	ActorManager::Instance().Clear();
	LightManager::Instanes().Clear();
	EnemyManager::Instance().Clear();
	PlayerManager::Instance().Clear();
	StageManager::Instance().Clear();
	UiManager::Instance().Clear();
	ProjectileManager::Instance().Clear();
	Audio::Instance().AllStop();
	Audio::Instance().AllClear();

	//if (cameraControlle != nullptr)
	//{
	//	delete cameraControlle;
	//	cameraControlle = nullptr;
	//}
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	// メニューを選ぶ
	if (InputMenue())
	{
		isMenue = isMenue ? isMenueOf : isMenueOn;
	}

	// メニュー停止
	//if (isMenue)return;

	float dlayTime = dlayTimeCheck ?  elapsedTime / 2 : elapsedTime;

	ActorManager::Instance().Update(dlayTime);

	Audio::Instance().Update();

	// カメラ更新
	cameraControlle->Update(elapsedTime);

	// エフェクト更新処理
	EffectManager::Instance().Update(dlayTime);

	// ポストエフェクト
	PlayEffectsShaders(elapsedTime);

	// シーン切り替え
	{

		for (int i = 0; i < PlayerManager::Instance().GetPlayerCount(); ++i)
		{
			// 死んだ瞬間
			if (PlayerManager::Instance().GetPlayer(i)->GetComponent<HP>()->GetDead() && !sceneChengeCheckDead)
			{
				PlayerManager::Instance().GetPlayer(i)->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Death));
				vignette_smoothness = 0.0f;
				vignette_intensity = 0.0f;
				sceneChengeCheckDead = true;
				colorGradingData.brigthness = 2.0f;
			}
			// 演出終了
			if (!PlayerManager::Instance().GetPlayer(i)->GetComponent<HP>()->GetDead() && sceneChengeCheckDead)
				// ゲームオーバー
				SceneManager::Instance().ChangeScene(new SceneGameOver);
		}

		for (int i = 0; i < EnemyManager::Instance().GetEnemyCount(); ++i)
		{
			std::shared_ptr<HP> hp = EnemyManager::Instance().GetEnemy(i)->GetComponent<HP>();
			if (hp->GetDead())
			{
				vignette_smoothness = 0.0f;
				vignette_intensity = 0.0f;
				colorGradingData.brigthness = 3.0f;
				hp->SetDead(false);
				//ActorManager::Instance().Clear();
				EnemyManager::Instance().GetEnemy(i)->GetComponent<EnemyBoss>()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Death));
			}
			if (hp->GetHealth() <= 0 && hp->GetLife() >= 0)
			{
				// hpを回復
				hp->SetHealth(hp->GetMaxHealth());
			}
			// クリア
			if (EnemyManager::Instance().GetEnemy(i)->GetComponent<EnemyBoss>()->GetClearCheck())
			{
				SceneManager::Instance().ChangeScene(new SceneGameClear);
			}
		}
		// ゲームオーバーに行かないように
		if (EnemyManager::Instance().GetEnemyCount() <= 0 ||
			UiManager::Instance().GetUiesCount() <= 5)return;

		// ゲームオーバー時間制限
		std::weak_ptr<UiTime> uiTime = UiManager::Instance().GetUies((int)UiManager::UiCount::Time)->GetComponent<UiTime>();
		if (uiTime.lock()->GetTimeUp())
		{
			colorGradingData.brigthness = 3.0f;
			// ゲームオーバー
			SceneManager::Instance().ChangeScene(new SceneGameOver);
		}

	}
}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
	//// シャドウマップの描画
	RenderShadowmap();
	Render3DScene();
	// GUI
	ActorManager::Instance().RenderGui();
	// 書き込み先をバックバッファに変えてオフスクリーンレンダリングの結果を描画する
	{
		ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
		// 画面クリア＆レンダーターゲット設定
		FLOAT color[] = { 0.0f,0.0f,0.0f,1.0f }; // RGBA(0.0~1.0)
		dc->ClearRenderTargetView(rtv, color);
		dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		dc->OMSetRenderTargets(1, &rtv, dsv);
		// UINT11
        // ビューポートの設定
		D3D11_VIEWPORT vp = {};
		vp.Width = graphics.GetScreenWidth();
		vp.Height = graphics.GetScreenHeight();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		dc->RSSetViewports(1, &vp);
		RenderContext rc;
		
		rc.deviceContext = dc;
		postprocessingRenderer.Render(rc);
	}


	// 2Dスプライト描画
	{
		// 描画処理
		RenderContext rc;
		rc.deviceContext = dc;

		// カメラパラメータ設定
		Camera& camera = Camera::Instance();
		rc.viewPosition.x = camera.GetEye().x;
		rc.viewPosition.y = camera.GetEye().y;
		rc.viewPosition.z = camera.GetEye().z;
		rc.viewPosition.w = 1;
		rc.view = camera.GetView();
		rc.projection = camera.GetProjection();
		
		SpriteShader* shaderUi = graphics.GetShader(SpriteShaderId::Default);

		ActorManager::Instance().Render(rc, shaderUi);
	}

#ifdef _DEBUG
	
	cameraControlle->OnGUI();
		 //2DデバッグGUI描画
	{
		if (ImGui::TreeNode("2DUI"))
		{
			ImGui::SliderFloat2("debugPush2Pos", &debugPush2Pos.x, 0.0f, 1200.0f);
			ImGui::SliderFloat2("debugShortCutPos", &debugShortCutPos.x, 0.0f, 1200.0f);
			ImGui::SliderFloat2("debugShortPushPos", &debugShortPushPos.x, 0.0f, 1200.0f);

			ImGui::SliderFloat2("debugPush2Scale", &debugPush2Scale.x, 0.0f, 200.1f);
			ImGui::SliderFloat2("debugShortCutScale", &debugShortCutScale.x, 0.0f, 200.1f);
			ImGui::SliderFloat2("debugShortPushScale", &debugShortPushScale.x, 0.0f, 200.1f);

			UiManager::Instance().GetUies((int)UiManager::UiCount::Push2)->GetComponent<TransForm2D>()->SetPosition(debugPush2Pos);
			UiManager::Instance().GetUies((int)UiManager::UiCount::ShortCut)->GetComponent<TransForm2D>()->SetPosition(debugShortCutPos);
			UiManager::Instance().GetUies((int)UiManager::UiCount::PushShort)->GetComponent<TransForm2D>()->SetPosition(debugShortPushPos);

			UiManager::Instance().GetUies((int)UiManager::UiCount::Push2)->GetComponent<TransForm2D>()->SetScale(debugPush2Scale);
			UiManager::Instance().GetUies((int)UiManager::UiCount::ShortCut)->GetComponent<TransForm2D>()->SetScale(debugShortCutScale);
			UiManager::Instance().GetUies((int)UiManager::UiCount::PushShort)->GetComponent<TransForm2D>()->SetScale(debugShortPushScale);

			ImGui::TreePop();
		}
		ImGui::Separator();

		if (ImGui::TreeNode("shadowmap"))
		{
			ImGui::SliderFloat("DrawRect", &shadowDrawRect, 1.0f, 2048.0f);
			ImGui::ColorEdit3("Color", &shadowColor.x);
			ImGui::SliderFloat("Bias", &shadowBias, 0.0f, 0.1f);
			ImGui::SliderFloat("lightPosition", &lightPositionScale, -13.0f, 13.0f);
			ImGui::Text("texture");
			ImGui::Image(shadowmapDepthStencil->GetShaderResourceView().Get(), { 256,256 }, { 0,0 }, { 1,1 },
				{ 1,1,1,1 });
			ImGui::TreePop();
		}
		ImGui::Separator();
		if (ImGui::TreeNode("ColorGrading"))
		{
			ImGui::SliderFloat("hueShift", &colorGradingData.hueShift, 0.0f, +360.0f);
			ImGui::SliderFloat("saturation", &colorGradingData.saturation, 0.0f, +2.0f);
			ImGui::SliderFloat("brigtness", &colorGradingData.brigthness, 0.0f, +10.0f);

			ImGui::TreePop();
		}
		ImGui::Separator();
		if (ImGui::TreeNode("RadialBlur"))
		{
			ImGui::SliderFloat("radius", &radialBlurData.radius, 0.0f, 200.0f);
			ImGui::SliderInt("samplingCount", &radialBlurData.samplingCount, 0, 10);
			ImGui::SliderFloat2("center", &radialBlurData.center.x, 0.0f, 1.0f);
			ImGui::SliderFloat("mask radius", &radialBlurData.mask_radius, 0.0f, 600.0f);
			ImGui::TreePop();
		}
		ImGui::Separator();
		if (ImGui::TreeNode("BloomData"))
		{
			ImGui::SliderFloat("threshold", &bloomData.luminanceExtractionData.threshold, 0.0f, 1.0f);
			ImGui::SliderFloat("intensity", &bloomData.luminanceExtractionData.intensity, 0.0f, 10.0f);
			ImGui::SliderInt("kernelSize", &bloomData.gaussianFilterData.kernelSize, 1, MaxkernelSize - 1);
			ImGui::SliderFloat("deviation", &bloomData.gaussianFilterData.deviation, 1.0f, 10.0f);
			ImGui::TreePop();
		}
		ImGui::Separator();
	}

	if (ImGui::TreeNode("audio"))
	{
		if (ImGui::Button("AudioStop"))
		{
			StopBgm();
		}
	}

	LightManager::Instanes().DrawDebugGUI();

	postprocessingRenderer.DrawDebugGUI();

	Audio::Instance().DebugDrawGUI();
#endif // _DEBUG
}

void SceneGame::Render3DScene()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = renderTarget->GetRenderTargetView().Get();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// ビューポートの設定
	D3D11_VIEWPORT vp = {};
	vp.Width = graphics.GetScreenWidth();
	vp.Height = graphics.GetScreenHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// 描画処理
	RenderContext rc;
	rc.deviceContext = dc;

	// ライトの情報を詰め込む
	LightManager::Instanes().PushRenderContext(rc);

	// UNIT11
	// ライトの情報をつめこむ 
	rc.shadowMapData.shadowMap = shadowmapDepthStencil->GetShaderResourceView().Get();
	rc.shadowMapData.lightViewProjection = lightViewProjeciton;
	rc.shadowMapData.shadowColor = shadowColor;
	rc.shadowMapData.shadowBias = shadowBias;

	// カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// ソードトレイル
	{
		// ポリゴン描画
		primitiveRenderer->Render(dc, rc.view, rc.projection, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//// グリッド描画
		primitiveRenderer->Render(dc, rc.view, rc.projection, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	}

	// 3Dモデル描画
	{
		ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);

		ActorManager::Instance().Render(rc, shader);
	}

	// 3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}
#ifdef _DEBUG

	// デバッグプリミティブの表示
	{
		// グリッド描画
		//DrawGrid(dc, 20, 10.0f);

		for (int i = 0; i < EnemyManager::Instance().GetEnemyCount(); i++)
		{
			EnemyManager::Instance().GetEnemy(i)->GetComponent<EnemyBoss>()->DrawDebugPrimitive();
		}

		//for (int i = 0; i < ProjectileManager::Instance().GetProjectileCount(); i++)
		//{
		//	if(ProjectileManager::Instance().GetProjectile(i)->GetComponent<ProjectileImpact>())
		//	ProjectileManager::Instance().GetProjectile(i)->GetComponent<ProjectileImpact>()->DrawDebugPrimitive();
		//}

		// ライトのデバッグプリミティブの描画
		LightManager::Instanes().DrawDebugPrimitive();

		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, camera.GetView(), camera.GetProjection());

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, camera.GetView(), camera.GetProjection());
	}
#endif // _DEBUG
}

void SceneGame::RenderShadowmap()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11DepthStencilView* dsv = shadowmapDepthStencil->GetDepthStencilView().Get();

	// 画面クリア
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (!mainDirectionalLight)
		return;

	dc->OMSetRenderTargets(0, &rtv, dsv);

	// ビューポートの設定
	D3D11_VIEWPORT vp = {};
	vp.Width = static_cast<float>(shadowmapDepthStencil->GetWidth());
	vp.Height = static_cast<float> (shadowmapDepthStencil->GetHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// 描画処理
	RenderContext rc;
	rc.deviceContext = dc;

	// カメラパラメーター設定
	{
		// 平行光源からカメラ位置を作成し、そこから原点の位置を見るように視線行列を生成
		DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat3(&mainDirectionalLight->GetDirection());
		LightPosition = DirectX::XMVectorScale(LightPosition, -3.0f);
		DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(LightPosition,
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

		// シャドウマップに描画したい範囲の射影行列を生成
		DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(shadowDrawRect, shadowDrawRect, 0.1f,
			1000.0f);
		DirectX::XMStoreFloat4x4(&rc.view, V);
		DirectX::XMStoreFloat4x4(&rc.projection, P);
		DirectX::XMStoreFloat4x4(&lightViewProjeciton, V * P);
	}

	// シェーダー
	{
		ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);

		ActorManager::Instance().Render(rc, shader);
	}
}

void SceneGame::PostProcessingRendererInitialize()
{
	PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
	// シーンテクスチャを設定しておく
	ShaderResourceViewData srvData;
	srvData.srv = renderTarget->GetShaderResourceView();
	srvData.width = renderTarget->GetWidth();
	srvData.height = renderTarget->GetHeight();
	postprocessingRenderer.SetSceneData(srvData);

	bloomData.luminanceExtractionData.threshold = 0.41f;
	bloomData.luminanceExtractionData.intensity = 1.6f;

	bloomData.gaussianFilterData.kernelSize = 15;
	bloomData.gaussianFilterData.deviation = 8.3f;

	postprocessingRenderer.SetBloomMaxData(bloomData);

	// ポストエフェクト
	// 画面白ボケ
	// カラーグラディエンス
	float colorGradingBrigthness = 1.0f;
	float colorGradingHueShift = 3.0f;
	colorGradingDataMin.brigthness = colorGradingBrigthness;
	colorGradingDataMin.hueShift = colorGradingHueShift;

	postprocessingRenderer.SetColorGradingMinData(colorGradingDataMin);

	// ブラー
	// ブラー範囲
	float radislBlurRadius = 0;
	radialBlurData.radius = radislBlurRadius;

	// ブラーのかからない範囲
	float radislBlurMaskRadiusNormal = 600;
	radialBlurData.mask_radius = radislBlurMaskRadiusNormal;

	postprocessingRenderer.SetRadialBlurMinData(radialBlurData);

	// 周辺減光
	vignetteData.color = { 1.0f, 0.0f, 0.0f, 1.0f };
	vignetteData.center = { 0.5f, 0.5f };
	vignetteData.intensity = 0.0f;
	vignetteData.smoothness = 0.0f;
	vignetteData.rounded = false;
	vignetteData.roundness = 1.0f;

	postprocessingRenderer.SetVignetteMinData(vignetteData);
	postprocessingRenderer.SetVignetteData(vignetteData);
}

void SceneGame::PostProcessingRendererFinalize()
{
	PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();

	// シーンテクスチャを設定しておく
	ShaderResourceViewData srvData;
	srvData.srv = renderTarget->GetShaderResourceView();
	srvData.width = renderTarget->GetWidth();
	srvData.height = renderTarget->GetHeight();

	postprocessingRenderer.SetSceneData(srvData);

	bloomData.luminanceExtractionData.threshold = 0.41f;
	bloomData.luminanceExtractionData.intensity = 1.6f;

	bloomData.gaussianFilterData.kernelSize = 15;
	bloomData.gaussianFilterData.deviation = 8.3f;

	postprocessingRenderer.SetBloomMinData(bloomData);
	postprocessingRenderer.SetBloomData(bloomData);

	// ポストエフェクト
	// 画面白ボケ
	// カラーグラディエンス
	float colorGradingBrigthness = 1.0f;
	float colorGradingHueShift = 3.0f;
	colorGradingDataMin.brigthness = colorGradingBrigthness;
	colorGradingDataMin.hueShift = colorGradingHueShift;


	postprocessingRenderer.SetColorGradingMinData(colorGradingDataMin);
	postprocessingRenderer.SetColorGradingData(colorGradingDataMin);

	// ブラー
	// ブラー範囲
	float radislBlurRadius = 0;
	radialBlurData.radius = radislBlurRadius;


	// ブラーのかからない範囲
	float radislBlurMaskRadiusNormal = 600;
	radialBlurData.mask_radius = radislBlurMaskRadiusNormal;

	postprocessingRenderer.SetRadialBlurMinData(radialBlurData);
	postprocessingRenderer.SetRadialBlurData(radialBlurData);

	// 周辺減光
	vignetteData.color = { 1.0f, 0.0f, 0.0f, 1.0f };
	vignetteData.center = { 0.5f, 0.5f };
	vignetteData.intensity = 0.0f;
	vignetteData.smoothness = 0.0f;
	vignetteData.rounded = false;
	vignetteData.roundness = 1.0f;

	postprocessingRenderer.SetVignetteMinData(vignetteData);
	postprocessingRenderer.SetVignetteData(vignetteData);
}

void SceneGame::PlayEffectsShaders(float elapsedTime)
{
	Graphics& graphics = Graphics::Instance();

	// 画面スロー
	bool hitCheck = PlayerManager::Instance().GetPlayer(0)->GetComponent<Player>()->GetHitCheck();

	if (hitCheck)
	{
		dlayStateTimer = dlayStateTimerMax;
		PlayerManager::Instance().GetPlayer(0)->GetComponent<Player>()->SetHitCheck(false);
	}
	if (dlayStateTimer - FLT_EPSILON > 0.0f + FLT_EPSILON)
	{
		dlayStateTimer -= elapsedTime;

		dlayTimeCheck = true;
	}

	else
	{
		dlayTimeCheck = false;
	}

	// ブラーエフェクト
	if (shaderBlurStateTimer > 0 )
	{
		shaderBlurStateTimer -= elapsedTime;

		// 画面真ん中
		radialBlurData.center = { 0.5f ,0.5f };
		// 画面ブラー
		float radislBlurRadius = 200;
		radialBlurData.radius =
			radialBlurData.radius > radialBlurDataRadislBlurRadiusMax ?
			radialBlurData.radius : radialBlurData.radius + (5 + elapsedTime);
		// 歪み具合
		int radislBlurSamplingCount = 10;
		radialBlurData.samplingCount = radislBlurSamplingCount;
		// 自分が見える範囲
		float radislBlurMaskRadius = 300;
		radialBlurData.mask_radius = radislBlurMaskRadius;

	}
	else if(shaderPlayStateTimer < 0)
	{
		// ブラー範囲
		float radislBlurRadius = 0;
		radialBlurData.radius = radislBlurRadius - FLT_EPSILON < radialBlurData.radius + FLT_EPSILON ? radialBlurData.radius - (5 + elapsedTime) : radislBlurRadius - FLT_EPSILON;
		// ブラーのかからない範囲
		float radislBlurMaskRadiusNormal = 600;
		float radislBlurMaskRadiusEffectOn = 300;
		radialBlurData.mask_radius = radislBlurRadius - FLT_EPSILON < radialBlurData.radius + FLT_EPSILON ? radislBlurMaskRadiusEffectOn : radislBlurMaskRadiusNormal;
	}
}

bool SceneGame::InputMenue()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	if (gamePad.GetButtonDown() & GamePad::BTN_START)
	{
		return true;
	}
	return false;
}

void SceneGame::InitializeComponent()
{
	// ステージ初期化
	{
		const char* filename = "Data/Model/ExampleStage/stageNotRuby.mdl";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->AddComponent<ModelControll>();
		actor->GetComponent<ModelControll>()->LoadModel(filename);
		actor->SetName("StageMain");
		actor->AddComponent<Transform>();

		actor->GetComponent<Transform>()->
			SetPosition(DirectX::XMFLOAT3(0, -25, 0));

		actor->GetComponent<Transform>()->
			SetAngle(DirectX::XMFLOAT3(0, 0, 0));

		actor->GetComponent<Transform>()->
			SetScale(DirectX::XMFLOAT3(1, 1, 1));

		actor->AddComponent<Collision>();

		actor->AddComponent<StageMain>();

		// 影シェーダー
		actor->GetComponent<StageMain>()->SetIsRimRightning(0);

		// 解像度
		actor->GetComponent<StageMain>()->SetTexcoordMult(20);

		// スペキュラー
		actor->GetComponent<StageMain>()->SetIsSpecular(0);

		// これが２Dかの確認
		bool check2d = false;
		actor->SetCheck2d(check2d);

		StageManager::Instance().Register(actor);
	}

	// ステージルビー初期化
	{
		const char* filename = "Data/Model/ExampleStage/RubyStage.mdl";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->AddComponent<ModelControll>();
		actor->GetComponent<ModelControll>()->LoadModel(filename);
		actor->SetName("StageRuby");
		actor->AddComponent<Transform>();

		actor->GetComponent<Transform>()->
			SetPosition(DirectX::XMFLOAT3(0, -17.85f, 0));

		actor->GetComponent<Transform>()->
			SetAngle(DirectX::XMFLOAT3(0, 0, 0));

		actor->GetComponent<Transform>()->
			SetScale(DirectX::XMFLOAT3(1, 1, 1));

		actor->AddComponent<Collision>();

		actor->AddComponent<StageMain>();

		// 影シェーダー
		actor->GetComponent<StageMain>()->SetIsRimRightning(1);

		// 解像度
		actor->GetComponent<StageMain>()->SetTexcoordMult(1);

		// スペキュラー
		actor->GetComponent<StageMain>()->SetIsSpecular(1);

		// これが２Dかの確認
		bool check2d = false;
		actor->SetCheck2d(check2d);

		StageManager::Instance().Register(actor);
	}

	////player = new Player;
	{
		// プレイヤー初期化
		const char* filename = "Data/Model/Player/Maria.mdl";

		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->AddComponent<ModelControll>();
		actor->GetComponent<ModelControll>()->LoadModel(filename);
		actor->SetName("Player");
		actor->AddComponent<Transform>();

		actor->GetComponent<Transform>()->
			SetPosition(DirectX::XMFLOAT3(0, -3.6f, -10));

		actor->GetComponent<Transform>()->
			SetAngle(DirectX::XMFLOAT3(0, 0, 0));

		actor->GetComponent<Transform>()->
			SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));

		actor->AddComponent<Collision>();

		actor->AddComponent<Movement>();

		// 移動範囲
		actor->GetComponent<Movement>()->SetArea(minPos, maxPos);

		actor->AddComponent<HP>();
		std::shared_ptr<HP> hp = actor->GetComponent<HP>();
		int life = 0;
		hp->SetLife(life);

		actor->AddComponent<Mp>();
		std::shared_ptr<Mp> mp = actor->GetComponent<Mp>();
		int mpMax = 50;
		mp->SetMaxMagic(mpMax);

		actor->AddComponent<Player>();

		// uiの有無で処理があるかを変える
		actor->GetComponent<Player>()->SetUiControlleCheck(true);

		// ステート設定
		actor->GetComponent<Player>()->StateMachineCreate();

		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerIdleState(actor));
		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerMovestate(actor));
		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerJumpState(actor));
		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerLandState(actor));
		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerJumpFlipState(actor));
		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerQuickJabState(actor));
		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerSideCutState(actor));
		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerCycloneStrikeState(actor));
		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerSpecialAttackState(actor));
		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerMagicState(actor));
		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerSpecialMagicState(actor));
		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerDamageState(actor));
		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerDeathState(actor));
		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerReviveState(actor));
		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerAvoidanceState(actor));
		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerReflectionState(actor));

		// ステートセット
		actor->GetComponent<Player>()->GetStateMachine()->SetState(static_cast<int>(Player::State::Idle));

		// これが２Dかの確認
		bool check2d = false;
		actor->SetCheck2d(check2d);

		PlayerManager::Instance().Register(actor);
	}

	// 敵
	{
		
		const char* filename = "Data/Model/Boss/BossAnim8.mdl";
		//const char* filename = "Data/Model/Slime/Slime.mdl";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->AddComponent<ModelControll>();
		actor->GetComponent<ModelControll>()->LoadModel(filename);
		actor->SetName("ClestaleBoss");
		actor->AddComponent<Transform>();

		actor->GetComponent<Transform>()->
			SetPosition(DirectX::XMFLOAT3(0, 0, 1));

		actor->GetComponent<Transform>()->
			SetAngle(DirectX::XMFLOAT3(0, 3, 0));

		actor->GetComponent<Transform>()->
			SetScale(DirectX::XMFLOAT3(0.06f, 0.06f, 0.06f));

		actor->AddComponent<Collision>();

		actor->AddComponent<Movement>();

		// 行動範囲設定
		actor->GetComponent<Movement>()->SetArea(minPos, maxPos);
		actor->AddComponent<HP>();
		std::shared_ptr<HP> hp = actor->GetComponent<HP>();
		int life = 2;
		hp->SetLife(life);
		actor->AddComponent<Collision>();
		actor->AddComponent<EnemyBoss>();

		// クリエイト
		actor->GetComponent<EnemyBoss>()->StateMachineCreate();

		// ステートマシンにステート登録
		actor->GetComponent<EnemyBoss>()->GetStateMachine()->RegisterState(new WanderState(actor));
		actor->GetComponent<EnemyBoss>()->GetStateMachine()->RegisterState(new IdleState(actor));
		actor->GetComponent<EnemyBoss>()->GetStateMachine()->RegisterState(new PursuitState(actor));
		actor->GetComponent<EnemyBoss>()->GetStateMachine()->RegisterState(new JumpState(actor));
		actor->GetComponent<EnemyBoss>()->GetStateMachine()->RegisterState(new AttackState(actor));
		actor->GetComponent<EnemyBoss>()->GetStateMachine()->RegisterState(new ConfusionState(actor));
		actor->GetComponent<EnemyBoss>()->GetStateMachine()->RegisterState(new DamageState(actor));
		actor->GetComponent<EnemyBoss>()->GetStateMachine()->RegisterState(new DeathState(actor));
		actor->GetComponent<EnemyBoss>()->GetStateMachine()->RegisterState(new AwakeStartState(actor));

		// ステートセット
		actor->GetComponent<EnemyBoss>()->GetStateMachine()->SetState(static_cast<int>(EnemyBoss::State::Idle));

		// これが２Dかの確認
		bool check2d = false;
		actor->SetCheck2d(check2d);
		EnemyManager::Instance().Register(actor);
	}

	// UI PlayerHP
	{
		const char* filename = "Data/Sprite/HP.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerHP");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 902, 336 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 358,310 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI EnemyHP
	{
		const char* filename = "Data/Sprite/enemyHPber.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("EnemyHPSet");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 900, -100 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 376,329 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}
	//// UI SpecialBox
	//{
	//	const char* filename = "Data/Sprite/必殺技入れ物.png";
	//	std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
	//	actor->SetName("SpecialBox");
	//	actor->AddComponent<SpriteControll>();
	//	actor->GetComponent<SpriteControll>()->LoadSprite(filename);
	//	actor->AddComponent<TransForm2D>();
	//	// 位置　角度　スケール情報
	//	std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
	//	//DirectX::XMFLOAT2 pos = { 490, 604 };
	//	//DirectX::XMFLOAT2 pos = { 740, 604 };
	//	DirectX::XMFLOAT2 pos = { 0, 0 };
	//	transform2D->SetPosition(pos);
	//	// 元の位置
	//	DirectX::XMFLOAT2 texPos = { 0, 0 };
	//	transform2D->SetTexPosition(texPos);

	//	float angle = 0;
	//	transform2D->SetAngle(angle);
	//	DirectX::XMFLOAT2 scale = { 400,230 };
	//	transform2D->SetScale(scale);
	//	// 元の大きさ
	//	DirectX::XMFLOAT2 texScale = { 0,0 };
	//	transform2D->SetTexScale(texScale);

	//	actor->AddComponent<Ui>();
	//	// 描画チェック
	//	std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
	//	ui->SetDrawCheck(DrawUi);

	//	// これが２Dかの確認
	//	bool check2d = true;
	//	actor->SetCheck2d(check2d);

	//	UiManager::Instance().Register(actor);
	//}

	// UI PlayerCommandAttack
	{
		const char* filename = "Data/Sprite/コマンド　非選択攻撃.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandAttack");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 0, 310 };
		//DirectX::XMFLOAT2 pos = { 0, 455 };
		DirectX::XMFLOAT2 pos = { 0, 375 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 358,310 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandMagick
	{
		const char* filename = "Data/Sprite/コマンド　非選択魔法.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandMagick");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 0, 390 };
		//DirectX::XMFLOAT2 pos = { 0, 535 };
		DirectX::XMFLOAT2 pos = { 0, 455 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 358,310 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}


	// UI PlayerCommandAttackCheck
	{
		const char* filename = "Data/Sprite/コマンド　選択攻撃.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandAttackCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 0, 375 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 358,310 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandMagickCheck
	{
		const char* filename = "Data/Sprite/コマンド　選択魔法.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandMagickCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 0, 455 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 358,310 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandSpecial
	{
		const char* filename = "Data/Sprite/必殺技　非選択.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpecial");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 0, 390 };

		DirectX::XMFLOAT2 pos = { 28, 620 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 280,89 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}
	// UI PlayerCommandSpecialUnCheck
	{
		const char* filename = "Data/Sprite/必殺技　点滅.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpecialUnCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 0, 390 };

		DirectX::XMFLOAT2 pos = { 28, 620 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 280,89 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}
	// UI PlayerCommandSpecialCheck
	{
		const char* filename = "Data/Sprite/必殺技　選択.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpecialCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 28, 620 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 308,89 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandFire
	{
		const char* filename = "Data/Sprite/コマンド　非選択ファイアー.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandFire");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 179, 290 };
		DirectX::XMFLOAT2 pos = { 179, 345 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		//DirectX::XMFLOAT2 scale = { 358,310 };
		DirectX::XMFLOAT2 scale = { 181,104 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandFireCheck
	{
		const char* filename = "Data/Sprite/コマンド　選択ファイアー.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandFireCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 179, 290 };
		DirectX::XMFLOAT2 pos = { 179, 345 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		//DirectX::XMFLOAT2 scale = { 358,310 };
		DirectX::XMFLOAT2 scale = { 206,104 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}
	// UI PlayerCommandRigtning
	{
		const char* filename = "Data/Sprite/サンダラ　押し込み.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandRigtning");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 179, 375 };
		DirectX::XMFLOAT2 pos = { 179, 430 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		//DirectX::XMFLOAT2 scale = { 358,310 };
		DirectX::XMFLOAT2 scale = { 181,104 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandRigtningCheck
	{
		const char* filename = "Data/Sprite/サンダラ.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandRigtningCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 179, 375 };
		DirectX::XMFLOAT2 pos = { 179, 430 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		//DirectX::XMFLOAT2 scale = { 358,310 };
		DirectX::XMFLOAT2 scale = { 201,104 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandIce
	{
		const char* filename = "Data/Sprite/ブリザラ　押し込み.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandIce");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 179, 460 };
		DirectX::XMFLOAT2 pos = { 179, 520 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		//DirectX::XMFLOAT2 scale = { 358,310 };
		DirectX::XMFLOAT2 scale = { 181,104 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandIceCheck
	{
		const char* filename = "Data/Sprite/ブリザラ.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandIceCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 179, 460 };
		DirectX::XMFLOAT2 pos = { 179, 520 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		//DirectX::XMFLOAT2 scale = { 358,310 };
		DirectX::XMFLOAT2 scale = { 201,104 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandHeale
	{
		const char* filename = "Data/Sprite/コマンド　非選択ケアル.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandHeale");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 200, 525 };
		//DirectX::XMFLOAT2 pos = { 200, 615 };
		DirectX::XMFLOAT2 pos = { 179, 611 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		//DirectX::XMFLOAT2 scale = { 312,111 };
		DirectX::XMFLOAT2 scale = { 181,104 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandHealeCheck
	{
		const char* filename = "Data/Sprite/コマンド　選択ケアル.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandHealeCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 179, 611 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 201,104 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerComandSpeciulChargeBox
	{
		const char* filename = "Data/Sprite/ゲージ.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerComandSpeciulChargeBox");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 10, 280 };
		DirectX::XMFLOAT2 pos = { 10, 235 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		//DirectX::XMFLOAT2 scale = { 382,161 };
		DirectX::XMFLOAT2 scale = { 410,111 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandSpeciulCharge01
	{
		const char* filename = "Data/Sprite/矢印.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpeciulCharge");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 158, 286 };
		DirectX::XMFLOAT2 pos = { 158, 241 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 100,110 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandSpeciulCharge02
	{
		const char* filename = "Data/Sprite/矢印.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpeciulCharge");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 232, 241 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 100,110 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandSpeciulCharge03
	{
		const char* filename = "Data/Sprite/矢印.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpeciulCharge");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 310, 241 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 100,110 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandSpeciulShurashu
	{
		const char* filename = "Data/Sprite/特殊技スラッシュ.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpeciulShurashu");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 38, 25 };
		//DirectX::XMFLOAT2 pos = { 38, 57 };
		DirectX::XMFLOAT2 pos = { 179, 520 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 160,89 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandSpeciulFrame
	{
		const char* filename = "Data/Sprite/特殊技フレイム.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpeciulFrame");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 38, 98 };
		//DirectX::XMFLOAT2 pos = { 38, 130 };
		DirectX::XMFLOAT2 pos = { 179, 611 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		//DirectX::XMFLOAT2 scale = { 160,170 };
		DirectX::XMFLOAT2 scale = { 160,89 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandSpeciulIce
	{
		const char* filename = "Data/Sprite/特殊技ブリザラ.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpeciulIce");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 0, 200 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		//DirectX::XMFLOAT2 scale = { 160,170 };
		DirectX::XMFLOAT2 scale = { 150,70 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandSpeciulThander
	{
		const char* filename = "Data/Sprite/特殊技サンダラ.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpeciulFrame");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 0, 200 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		//DirectX::XMFLOAT2 scale = { 160,170 };
		DirectX::XMFLOAT2 scale = { 0.1f,0.1f };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerHP
	{
		const char* filename = "Data/Sprite/player_status.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerHP");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 979, 508 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 243,30 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);


		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerMPBar
	{
		const char* filename = "Data/Sprite/MP.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerMPBar");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 930, 554 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 310,52 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);


		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}
	// UI EnemyHP
	{
		const char* filename = "Data/Sprite/enemy_status.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("EnemyHP");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();

		DirectX::XMFLOAT2 pos = { 916, 31 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);

		DirectX::XMFLOAT2 scale = { 303,30 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);


		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI EnemyHP 残機１
	{
		const char* filename = "Data/Sprite/enemy_status.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("EnemyHPLife1");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();

		DirectX::XMFLOAT2 pos = { 1142, 79 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);

		DirectX::XMFLOAT2 scale = { 27,17 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);


		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI EnemyHP 残機2
	{
		const char* filename = "Data/Sprite/enemy_status.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("EnemyHPLife2");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();

		DirectX::XMFLOAT2 pos = { 1185, 79 };
		transform2D->SetPosition(pos);

		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);

		DirectX::XMFLOAT2 scale = { 27,17 };
		transform2D->SetScale(scale);

		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI UiTime 制限時間１桁
	{
		const char* filename = "Data\\Font\\fonts\\font4.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UiTime 1digit");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();

		// 位置
		DirectX::XMFLOAT2 pos = { 100, 31 };
		//DirectX::XMFLOAT2 pos = { 32, 31 };
		transform2D->SetPosition(pos);

		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 60 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);

		DirectX::XMFLOAT2 scale = { 43,49 };
		transform2D->SetScale(scale);

		DirectX::XMFLOAT2 texScale = { 20,20 };
		transform2D->SetTexScale(texScale);

		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<UiTime>();
		// 描画チェック
		std::shared_ptr<UiTime> ui = actor->GetComponent<UiTime>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		// 制限時間
		//int timeMax = 10;
		int timeMax = 180;
		//int timeMax = 10;
		ui->SetTimeMax(timeMax);

		int countDown = ui->GetTime() * ui->GetTimeMax();
		ui->SetCountDown(countDown);

		// 桁
		int digit = 3;
		ui->SetDigit(digit);

		UiManager::Instance().Register(actor);
	}

	// UI sight
	{
		const char* filename = "Data/Sprite/Loading.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("sight");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();

		DirectX::XMFLOAT2 pos = { 640, 360 };
		transform2D->SetPosition(pos);

		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);

		DirectX::XMFLOAT2 scale = { 50,50 };
		transform2D->SetScale(scale);

		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI sightCheck
	{
		const char* filename = "Data/Sprite/Loading_move.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("sightCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();

		DirectX::XMFLOAT2 pos = { 616, 335 };
		transform2D->SetPosition(pos);

		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);

		DirectX::XMFLOAT2 scale = { 120,120 };
		transform2D->SetScale(scale);

		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI MP
	{
		const char* filename = "Data/Sprite/enemyHPberGreen.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("MP");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 989, 415 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 376,329 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandShortCutFireCheck
	{
		const char* filename = "Data/Sprite/コマンド　非選択　ショートカットファイアー.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandShortCutFireCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 179, 340 };
		//DirectX::XMFLOAT2 pos = { 179, 265 };
		DirectX::XMFLOAT2 pos = { 179, 345 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 181,104 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandShortCutSunderCheck
	{
		const char* filename = "Data/Sprite/コマンド　非選択　ショートカットサンダラ.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandShortCutSunderCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 179, 440 };
		//DirectX::XMFLOAT2 pos = { 179, 350 };
		DirectX::XMFLOAT2 pos = { 179, 430 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 181,104 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandShortCutBrezerdCheck
	{
		const char* filename = "Data/Sprite/コマンド　非選択　ショートカットブリザラ.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandBrezerdCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 179, 540 };
		//DirectX::XMFLOAT2 pos = { 179, 440 };
		DirectX::XMFLOAT2 pos = { 179, 520 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 181,104 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandShortCutKeuleCheck
	{
		const char* filename = "Data/Sprite/コマンド　非選択　ショートカットケアル.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandShortCutKeuleCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 179, 640};
		//DirectX::XMFLOAT2 pos = { 179, 530 };
		DirectX::XMFLOAT2 pos = { 179, 611 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 181,104 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI Push
	{
		const char* filename = "Data/Sprite/選択.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("Push");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 179, 285 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 181,104 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI Push2
	{
		const char* filename = "Data/Sprite/選択.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("Push2");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 179, 285 };
		transform2D->SetPosition(debugPush2Pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 181,104 };
		transform2D->SetScale(debugPush2Scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandPush
	{
		const char* filename = "Data/Sprite/長押し中.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandPush");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 179, 640};
		//DirectX::XMFLOAT2 pos = { 179, 530 };
		DirectX::XMFLOAT2 pos = { 179, 611 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 181,104 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandPushNow
	{
		const char* filename = "Data/Sprite/チャージ用.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandPushNow");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 179, 640};
		//DirectX::XMFLOAT2 pos = { 179, 530 };
		DirectX::XMFLOAT2 pos = { 179, 611 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 181,104 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandCharge
	{
		const char* filename = "Data/Sprite/長押し文字.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandCharge");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 179, 640};
		//DirectX::XMFLOAT2 pos = { 179, 530 };
		DirectX::XMFLOAT2 pos = { 179, 611 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 181,104 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}


	//// UI PlayerCommandChargeFire
	//{
	//	const char* filename = "Data/Sprite/長押し文字.png";
	//	std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
	//	actor->SetName("PlayerCommandChargeFire");
	//	actor->AddComponent<SpriteControll>();
	//	actor->GetComponent<SpriteControll>()->LoadSprite(filename);
	//	actor->AddComponent<TransForm2D>();
	//	// 位置　角度　スケール情報
	//	std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
	//	//DirectX::XMFLOAT2 pos = { 179, 640};
	//	//DirectX::XMFLOAT2 pos = { 179, 530 };
	//	DirectX::XMFLOAT2 pos = { 179, 611 };
	//	transform2D->SetPosition(pos);
	//	// 元の位置
	//	DirectX::XMFLOAT2 texPos = { 0, 0 };
	//	transform2D->SetTexPosition(texPos);

	//	float angle = 0;
	//	transform2D->SetAngle(angle);
	//	DirectX::XMFLOAT2 scale = { 181,104 };
	//	transform2D->SetScale(scale);
	//	// 元の大きさ
	//	DirectX::XMFLOAT2 texScale = { 0,0 };
	//	transform2D->SetTexScale(texScale);

	//	// UI揺らす範囲を指定揺らす場合
	//	int max = (int)pos.y + 3;
	//	int min = (int)pos.y - 3;

	//	transform2D->SetUiMax(max);
	//	transform2D->SetUiMin(min);
	//	// UI揺らす時間
	//	int MaxTime = 30;

	//	transform2D->SetShakeTimeMax(MaxTime);

	//	actor->AddComponent<Ui>();
	//	// 描画チェック
	//	std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
	//	ui->SetDrawCheck(DrawUiEmpth);

	//	// これが２Dかの確認
	//	bool check2d = true;
	//	actor->SetCheck2d(check2d);

	//	UiManager::Instance().Register(actor);
	//}
	

	// UI ShortCut
	{
		const char* filename = "Data/Sprite/ショートカット.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("ShortCut");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 179, 285 };
		DirectX::XMFLOAT2 pos = { 179, 285 };
		transform2D->SetPosition(debugShortCutPos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 181,104 };
		transform2D->SetScale(debugShortCutScale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PushShort
	{
		const char* filename = "Data/Sprite/選択.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PushShort");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 179, 285 };
		transform2D->SetPosition(debugShortPushPos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 181,104 };
		transform2D->SetScale(debugShortPushScale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI OperationInstructionsSelect 
	{
		const char* filename = "Data/Sprite/操作説明.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("OperationInstructionsSelect");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 160, 604 };
		DirectX::XMFLOAT2 pos = { 494, 604 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 360,126 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI OperationInstructionsButton 
	{
		const char* filename = "Data/Sprite/操作説明ボタン.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("OperationInstructionsButton");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 490, 604 };
		//DirectX::XMFLOAT2 pos = { 740, 604 };
		DirectX::XMFLOAT2 pos = { 894, 604 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 360,126 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI ボタン
	{
		const char* filename = "Data/Sprite/選択 ボタン.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UI ButtonY");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 150, 76 };

		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 50,54 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUiEmpth);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI CommandDisabled01
	{
		const char* filename = "Data/Sprite/選択.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("CommandDisabled01");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 179, 285 };
		DirectX::XMFLOAT2 pos = { 179, 520 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 181,104 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI CommandDisabled02
	{
		const char* filename = "Data/Sprite/選択.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("CommandDisabled02");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		//DirectX::XMFLOAT2 pos = { 179, 285 };
		DirectX::XMFLOAT2 pos = { 179, 611 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 181,104 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(DrawUi);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}
	//// UI OperationInstructionsRBLB 
	//{
	//	const char* filename = "Data/Sprite/RBLB.png";
	//	std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
	//	actor->SetName("OperationInstructionsRBLB");
	//	actor->AddComponent<SpriteControll>();
	//	actor->GetComponent<Sprite>()->LoadSprite(filename);
	//	actor->AddComponent<TransForm2D>();
	//	// 位置　角度　スケール情報
	//	std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
	//	DirectX::XMFLOAT2 pos = { 835, 604 };
	//	transform2D->SetPosition(pos);
	//	// 元の位置
	//	DirectX::XMFLOAT2 texPos = { 0, 0 };
	//	transform2D->SetTexPosition(texPos);

	//	float angle = 0;
	//	transform2D->SetAngle(angle);
	//	DirectX::XMFLOAT2 scale = { 360,126 };
	//	transform2D->SetScale(scale);
	//	// 元の大きさ
	//	DirectX::XMFLOAT2 texScale = { 0,0 };
	//	transform2D->SetTexScale(texScale);

	//	actor->AddComponent<Ui>();
	//	// 描画チェック
	//	std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
	//	ui->SetDrawCheck(false);

	//	// これが２Dかの確認
	//	bool check2d = true;
	//	actor->SetCheck2d(check2d);

	//	UiManager::Instance().Register(actor);
	//}
}

void SceneGame::StartBgm()
{
	Audio& bgm = Audio::Instance();
	AudioParam audioParam;
	audioParam.filename = "Data/Audio/BGM/戦闘中 (online-audio-converter.com).wav";
	audioParam.loop = true;
	audioParam.volume = bgmVolume;
	bgm.Play(audioParam);
}

void SceneGame::StopBgm()
{
	Audio& bgm = Audio::Instance();
	AudioParam audioParam;
	audioParam.filename = "Data/Audio/BGM/戦闘中 (online-audio-converter.com).wav";
	bgm.Stop(audioParam);
}


