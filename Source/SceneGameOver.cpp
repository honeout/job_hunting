#include "SceneGameOver.h"
#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "Actor.h"
#include "Camera.h"
#include "LightManager.h"
#include "StateDerived.h"
#include "Player.h"
#include "EnemyBoss.h"
#include "EffectManager.h"
void SceneGameOver::Initialize()
{
	// bgm
	StartBgm();
    // コンポーネント初期化
    InitializeComponent();

	// 選択の種類
	selectPush = (int)Select::Game;

    // カメラ初期化
    cameraControlle = nullptr;
    cameraControlle = new CameraController();

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
		renderTarget = std::make_unique<RenderTarget>(static_cast<UINT>(graphics.GetScreenWidth())
			, static_cast<UINT>(graphics.GetScreenHeight())
			, DXGI_FORMAT_R8G8B8A8_UNORM);
	}

	// シャドウマップ用に深度ステンシルの生成
	{
		shadowmapDepthStencil = std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	}

	// ポストプロセス描画クラス生成
	{
		PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
		// シーンテクスチャを設定しておく
		ShaderResourceViewData srvData;
		srvData.srv = renderTarget->GetShaderResourceView();
		srvData.width = renderTarget->GetWidth();
		srvData.height = renderTarget->GetHeight();
		postprocessingRenderer.SetSceneData(srvData);

		// シェーダー値
		bloomData.luminanceExtractionData.threshold = 0.41f;
		bloomData.luminanceExtractionData.intensity = 1.6f;

		bloomData.gaussianFilterData.kernelSize = 15;
		bloomData.gaussianFilterData.deviation = 8.3f;

		postprocessingRenderer.SetBloomData(bloomData);
		vignetteData.color = { 1,1,1,1 };

		vignetteData.intensity = 1.0f;
		vignetteData.roundness = 1.0f;
		vignetteData.smoothness = 0.2f;
		postprocessingRenderer.SetVignetteData(vignetteData);

		vignetteData.smoothness = 1.0f;
		postprocessingRenderer.SetVignetteMinData(vignetteData);


		colorGradingData.saturation = 1.0f;
		colorGradingData.hueShift = 0.2f;
		colorGradingData.brigthness = 0.3f;
		postprocessingRenderer.SetColorGradingData(colorGradingData);
		postprocessingRenderer.SetColorGradingMinData(colorGradingData);
	}
}

void SceneGameOver::Start()
{
	// 音BGM
	//StartBgm();
}

void SceneGameOver::Finalize()
{
	StageManager::Instance().Clear();
	LightManager::Instanes().Clear();
	PlayerManager::Instance().Clear();
	EnemyManager::Instance().Clear();
	UiManager::Instance().Clear();
	ActorManager::Instance().Clear();
	Audio::Instance().AllStop();
	Audio::Instance().AllClear();
    if (cameraControlle != nullptr)
    {
        delete cameraControlle;
        cameraControlle = nullptr;
    }
}

void SceneGameOver::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	ActorManager::Instance().Update(elapsedTime);

	// カメラコントローラー更新処理
	cameraControlle->Update(elapsedTime);
	// エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);
	if (cameraControlle->GetCameraMortionDataTime())
		PlayEffectsShaders(elapsedTime);
	int uiManagerMax = UiManager::Instance().GetUiesCount();

	std::shared_ptr<Ui> uiId = UiManager::Instance().GetUies(0)->GetComponent<Ui>();
	if (!uiId) return;
	uiId->IncrementToAlpha(0.01f);
	
	if (gamePad.GetButtonDown() & GamePad::BTN_B)
	{
		// 描画許可
		isDrawButton = true;
		// ボタンを押す描画許可
		UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Select)->
			GetComponent<Ui>()->SetDrawCheck(isDrawButton);

		PushPos = selectPush == (int) Select::Title ? titlePos : reStartPos;

		// UI ボタンを押す
		UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Select)->
			GetComponent<TransForm2D>()->SetPosition(PushPos);
	}
	SelectScene();
}

void SceneGameOver::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
	//// シャドウマップの描画
	RenderShadowmap();
	// model描画
	Render3DScene();

	// GUI
	ActorManager::Instance().RenderGui();

	// 書き込み先をバックバッファに変えてオフスクリーンレンダリングの結果を描画する
	{
		ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
		// 画面クリア＆レンダーターゲット設定
		FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f }; // RGBA(0.0~1.0)
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

		// ポストプロセスを処理を行う
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
	//2DデバッグGUI描画
	{
		//ImGui::Separator();
		 //UNIT11
		if (ImGui::TreeNode("shadowmap"))
		{
			ImGui::SliderFloat("DrawRect", &shadowDrawRect, 1.0f, 2048.0f);
			ImGui::ColorEdit3("Color", &shadowColor.x);
			ImGui::SliderFloat("Bias", &shadowBias, 0.0f, 0.1f);
			ImGui::Text("texture");
			ImGui::Image(shadowmapDepthStencil->GetShaderResourceView().Get(), { 256,256 }, { 0,0 }, { 1,1 },
				{ 1,1,1,1 });
			ImGui::TreePop();
		}
		ImGui::Separator();
		//UNIT09
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


		ImGui::Separator();

		if (ImGui::TreeNode("vignette"))
		{
			ImGui::SliderFloat2("threshold", &vignetteData.center.x, 0.0f, 10.0f);
			ImGui::ColorEdit4("color", &vignetteData.color.x);
			ImGui::SliderFloat("intensity", &vignetteData.intensity, -10, 10);
			ImGui::SliderFloat("roundness", &vignetteData.roundness, -10, 10);
			ImGui::SliderFloat("smoothness", &vignetteData.smoothness, -10, 10);

			if (ImGui::Button("colorPushu"))
			{
				PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance().Instance();

				postprocessingRenderer.SetVignetteData(vignetteData);
			}

			if (ImGui::Button("colorPushuMin"))
			{
				PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance().Instance();

				postprocessingRenderer.SetVignetteMinData(vignetteData);
			}

			if (ImGui::Button("colorPushuMax"))
			{
				PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance().Instance();

				postprocessingRenderer.SetVignetteMaxData(vignetteData);
			}
			
			ImGui::TreePop();

		}
		ImGui::Separator();
	}

	LightManager::Instanes().DrawDebugGUI();

	postprocessingRenderer.DrawDebugGUI();
	
#endif // _DEBUG
}

void SceneGameOver::Render3DScene()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = renderTarget->GetRenderTargetView().Get();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
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

	// 3Dモデル描画
	{
		ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);
		ActorManager::Instance().Render(rc, shader);
	}

	// 3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}
}

void SceneGameOver::RenderShadowmap()
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

void SceneGameOver::InitializeComponent()
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
	// 行動範囲
	{
		minPos.x = -30;
		minPos.y = -3.525f;
		minPos.z = -30;

		maxPos.x = 30;
		maxPos.y = 3.625f;
		maxPos.z = 30;
	}

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
			SetAngle(DirectX::XMFLOAT3(0, 3, 0));

		actor->GetComponent<Transform>()->
			SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
		actor->AddComponent<Movement>();


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
		actor->GetComponent<Player>()->SetUiControlleCheck(false);

		// ステート設定
		actor->GetComponent<Player>()->StateMachineCreate();

		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerOverIdleState(actor));
		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerOverReviveState(actor));

		// ステートセット
		actor->GetComponent<Player>()->GetStateMachine()->SetState(static_cast<int>(Player::StateOver::Idle));

		actor->AddComponent<Collision>();

		// これが２Dかの確認
		bool check2d = false;
		actor->SetCheck2d(check2d);
		PlayerManager::Instance().Register(actor);
	}

	// UI タイトル名前
	{
		const char* filename = "Data/Sprite/GAME OVER (3).png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UIGAME OVER");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 785, 35 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 376,329 };
		//DirectX::XMFLOAT2 scale = { 820,770 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(true);

		DirectX::XMFLOAT4 color = { 1,1,1,0 };
		ui->SetColor(color);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI タイトル名前
	{
		const char* filename = "Data/Sprite/タイトル戻る.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UITitle");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 543, 515 };
		transform2D->SetPosition(titlePos);
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
		ui->SetDrawCheck(true);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI タイトル名前
	{
		const char* filename = "Data/Sprite/スタートボタン.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UIGame");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 543, 515 };
		transform2D->SetPosition(reStartPos);
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
		ui->SetDrawCheck(true);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI ボタン
	{
		const char* filename = "Data/Sprite/選択 ボタン.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UI Button");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 717, 100 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 60,64 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(true);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI 選択
	{
		const char* filename = "Data/Sprite/選択.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UI Select");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 543, 477 };
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
		ui->SetDrawCheck(false);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}
}

void SceneGameOver::PlayEffectsShaders(float elapsedTime)
{
}

void SceneGameOver::SelectScene()
{
	int uiManagerMax = UiManager::Instance().GetUiesCount();
	const GamePadButton anyButton =
		GamePad::BTN_B;

	GamePad& gamePad = Input::Instance().GetGamePad();

	// ボタン確認
	if (gamePad.GetButtonDown() & anyButton)isPush = true;

	switch (selectPush)
	{
	case (int)Select::Title:
	{
		// 位置
		{
			UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Select)->
				GetComponent<TransForm2D>()->SetPosition(titlePos);


			UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Push)->
				GetComponent<TransForm2D>()->SetPositionY((titlePos.y + buttonPosYAdd));
		}
		if (gamePad.GetButtonDown() & anyButton)// ロードの次ゲームという書き方
		{
			//　シーン変更
			SceneManager::Instance().ChangeScene(new SceneTitle);
		}

		break;
	}
	case (int)Select::Game:
	{
		// 位置
		{
			// UI ボタンを押す
			UiManager::Instance().GetUies((int)UiManager::UiCountGameOver::Select)->
				GetComponent<TransForm2D>()->SetPosition(startPos);

			UiManager::Instance().GetUies((int)UiManager::UiCountGameOver::Push)->
				GetComponent<TransForm2D>()->SetPositionY((startPos.y + buttonPosYAdd));
		}
		int playerCount = PlayerManager::Instance().GetPlayerCount();
		for (int i = 0; i < playerCount; ++i)
		{
			std::weak_ptr<Player> playerId = PlayerManager::Instance().GetPlayer(i)->GetComponent<Player>();
			if (playerId.lock()->GetEndState())
			{
				//　シーン変更
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
			}
		}

		break;
	}
	}

	// ボタン確認
	if (isPush) return;

	if (gamePad.GetButtonDown() & GamePad::BTN_UP)
	{
		selectPush = selectPush <= (int)Select::Title ? (int)Select::Game : (int)Select::Title;
	}
	if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)
	{
		selectPush = selectPush >= (int)Select::Game ? (int)Select::Title : (int)Select::Game;
	}
}

void SceneGameOver::StartBgm()
{
	Audio& Se = Audio::Instance();
	AudioParam audioParam;
	audioParam.filename = "Data/Audio/BGM/maou_bgm_fantasy06.wav";
	audioParam.loop = false;
	audioParam.volume = bgmVolume;
	Se.Play(audioParam);
}

