#include "StageMain.h"
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
#include "EffectManager.h"


// 初期化
void SceneTitle::Initialize()
{
	// 次のシーンの名前
	SetSceneName("SceneTitle");

    // スプライト初期化
	InitializeComponent();

	// カメラ初期設定 見える位置追いかけるものなど
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		cameraPos,
		cameraFocus,
		cameraUp
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
		mainDirectionalLight->SetDirection({ 1,-1,-1 });
		LightManager::Instanes().Register(mainDirectionalLight);
	}

	// 点光源を追加
	{
		//Light* light = new Light(LightType::Spot);
		//light->SetPosition(DirectX::XMFLOAT3(-30, 0, 0));
		//light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		//light->SetDirection(DirectX::XMFLOAT3(+1, -0.2f, 0));
		//light->SetRange(1000.0f);
		//LightManager::Instanes().Register(light);

		//Light* light = new Light(LightType::Point);
		//light->SetPosition(DirectX::XMFLOAT3(0.000, -30, -42));
		//light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		//light->SetRange(lightRange);
		//LightManager::Instanes().Register(light);


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
		bloomData.luminanceExtractionData.threshold = 0.18f;
		bloomData.luminanceExtractionData.intensity = 0.128f;

		bloomData.gaussianFilterData.kernelSize = 8;
		bloomData.gaussianFilterData.deviation = 10.3f;
		postprocessingRenderer.SetBloomData(bloomData);

		colorGradingData.hueShift = 3;
		colorGradingData.saturation = 1;
		colorGradingData.brigthness = 1.0f;
		postprocessingRenderer.SetColorGradingData(colorGradingData);
		postprocessingRenderer.SetColorGradingMinData(colorGradingData);

		// 周辺減光
		vignetteData.color = { 1.0f, 0.0f, 0.0f, 1.0f };
		vignetteData.center = { 0.5f, 0.5f };
		vignetteData.intensity = 0.0f;
		vignetteData.smoothness = 0.0f;
		vignetteData.rounded = false;
		vignetteData.roundness = 0.0f;
		
		postprocessingRenderer.SetVignetteData(vignetteData);
		postprocessingRenderer.SetVignetteMinData(vignetteData);
	}

	// カメラ初期化
	cameraControlle = new CameraController();

	StartMusic();
}
void SceneTitle::Start()
{
}
// 終了化
void SceneTitle::Finalize()
{
	StageManager::Instance().Clear();
	LightManager::Instanes().Clear();
	PlayerManager::Instance().Clear();
	UiManager::Instance().Clear();
	ActorManager::Instance().Clear();

	//Audio::Instance().AllStop();

	//Audio::Instance().AllClear();

	if (cameraControlle != nullptr)
	{
		delete cameraControlle;
		cameraControlle = nullptr;
	}

}
// 更新処理
void SceneTitle::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	ActorManager::Instance().Update(elapsedTime);

	// 音更新
	Audio::Instance().Update();

	// カメラコントローラー更新処理
	cameraControlle->Update(elapsedTime);

	// エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);

	int uiManagerMax = UiManager::Instance().GetUiesCount();
	// 何かボタンを押したらローディングをはさんでゲームシーンへ切り替え
	const GamePadButton anyButton =
		GamePad::BTN_B;

	int playerCount = PlayerManager::Instance().GetPlayerCount();

	if (playerCount < 0) return;

	auto playerId = PlayerManager::Instance().GetPlayer((int)PlayerManager::PlayerType::Main);
	std::shared_ptr<Player> playerIdMain = playerId->GetComponent<Player>();

	// 演出開始
	if (playerIdMain->InputAttack())
	{
		// 決定後操作不能
		isSelectCommand = true;

		// 曲再生中止
		StopMusic();

		// 描画許可
		isDrawButton = true;
		// ボタンを押す描画許可
		UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Select)->
			GetComponent<Ui>()->SetDrawCheck(isDrawButton);
	}
	// 演出停止
	if (playerIdMain->GetEndState())
	{
		//　シーン変更
		playerIdMain->SetEndState(false);
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
		return;
	}
	// UIコマンド操作
	SelectScene(elapsedTime);
}
// 描画処理
void SceneTitle::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	// ポストプロセスシェーダー
	PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
	//// シャドウマップの描画
	RenderShadowmap();
	// model描画
	Render3DScene();
	//postprocessingRenderer.SetBloomData(bloomData);

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
		rc.radialBlurData = radialBlurData;
		rc.colorGradingData = colorGradingData;
		rc.bloomData = bloomData;
		// 周辺減光
		rc.vignetteData.color = vignetteData.color;
		rc.vignetteData.center = vignetteData.center;
		rc.vignetteData.intensity = vignetteData.intensity;
		rc.vignetteData.smoothness = vignetteData.smoothness;
		rc.vignetteData.rounded = vignetteData.rounded;
		rc.vignetteData.roundness = vignetteData.roundness;
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

	// デバッグ
	{
		//2DデバッグGUI描画
		{
			if (ImGui::TreeNode("Shake"))
			{
				if (ImGui::Button("Shake"))
				{
					MessageData::CAMERASHAKEDATA p;
					p.shakePower = debugShakePower;
					p.shakeTimer = debugShakeTimer;

					Messenger::Instance().SendData(MessageData::CAMERASHAKE, &p);
				}

				ImGui::SliderFloat("shakePower", &debugShakePower, 0.0f, 5.0f);
				ImGui::SliderFloat("shakeTimer", &debugShakeTimer, 0.0f, 5.0f);

				ImGui::TreePop();
			}
			ImGui::Separator();
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

			if (ImGui::TreeNode("vignette"))
			{
				ImGui::SliderFloat2("threshold", &vignetteData.center.x, 0.0f, 10.0f);
				ImGui::ColorEdit4("color", &vignetteData.color.x);
				ImGui::SliderFloat("intensity", &vignetteData.intensity, -10, 10);
				ImGui::SliderFloat("roundness", &vignetteData.roundness, -10, 10);
				ImGui::SliderFloat("smoothness", &vignetteData.smoothness, -10, 10);
				ImGui::TreePop();
			}

			ImGui::Separator();
			// カメラパラメータ設定
			//cameraControlle->OnGUI();
			ImGui::Separator();
		}
		LightManager::Instanes().DrawDebugGUI();
		postprocessingRenderer.DrawDebugGUI();
	}
}
void SceneTitle::StartMusic()
{
	Audio& bgm = Audio::Instance();
	AudioParam audioParam;
	audioParam.filename = "Data/Audio/BGM/maou_bgm_healing17.wav";
	audioParam.loop = true;
	audioParam.volume = bgmVolume;
	bgm.Play(audioParam);
}
void SceneTitle::StopMusic()
{
	Audio& bgm = Audio::Instance();
	std::string filename = "Data/Audio/SE/maou_bgm_healing17.wav";
	bgm.Stop(filename);
}
// model描画
void SceneTitle::Render3DScene()
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

	rc.shadowMapData.shadowBias;

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
// 影描画
void SceneTitle::RenderShadowmap()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11DepthStencilView* dsv = shadowmapDepthStencil->GetDepthStencilView().Get();

	// 画面クリア
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (!mainDirectionalLight)
		return;

	// 描画する場所
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

void SceneTitle::InitializeComponent()
{
	Graphics& graphics = Graphics::Instance();
	// 画面の比率
	scaleScreen = { graphics.GetScreenWidth() / screenWidth,graphics.GetScreenHeight() / screenHeight };

	// 画面最大値
	screenWidth = Graphics::Instance().GetScreenWidth();
	screenHeight = Graphics::Instance().GetScreenHeight();

	// ボタンの位置差
	buttonPos = { (startPos.x + commandScale.x) - buttonOffset, startPos.y };

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
		auto actor = ActorManager::Instance().Create();
		// 安全チェック
		if (!actor) return;
		actor->AddComponent<ModelControll>();
		actor->GetComponent<ModelControll>()->LoadModel(filename);
		actor->SetName("StageMain");
		actor->AddComponent<Transform>();
		// トランスフォーム（位置、角度、スケール値）
		auto transform = actor->GetComponent<Transform>();
		// 安全チェック
		if (!transform) return;

		transform->SetPosition(DirectX::XMFLOAT3(0, -25, 0));
		transform->SetAngle(DirectX::XMFLOAT3(0, 0, 0));
		transform->SetScale(DirectX::XMFLOAT3(1, 1, 1));

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
			SetAngle(DirectX::XMFLOAT3(0, 1.5f, 0));
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

	////player
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

		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerTitleIdleState(actor));
		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerTitlePushState(actor));

		// ステートセット
		actor->GetComponent<Player>()->GetStateMachine()->SetState(static_cast<int>(Player::StateTitle::Idle));

		actor->AddComponent<Collision>();

		// これが２Dかの確認
		bool check2d = false;
		actor->SetCheck2d(check2d);

		PlayerManager::Instance().Register(actor);
	}

	// UI タイトル名前
	{
		const char* filename = "Data/Sprite/TitleName.png";
		auto actor = ActorManager::Instance().Create();
		// 安全チェック
		if (!actor) return;
		actor->SetName("UITitle");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// トランスフォーム（位置、角度、スケール値）
		auto transform2D = actor->GetComponent<TransForm2D>();
		// 安全チェック
		if (!transform2D) return;

		transform2D->SetPosition(titlePos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 676,329 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		auto ui = actor->GetComponent<Ui>();
		// 安全チェック
		if (!ui) return;
		ui->SetDrawCheck(true);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI タイトル名前
	{
		const char* filename = "Data/Sprite/Start Button.png";
		auto actor = ActorManager::Instance().Create();
		// 安全チェック
		if (!actor) return;
		actor->SetName("UIPush");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// トランスフォーム（位置、角度、スケール値）
		auto transform2D = actor->GetComponent<TransForm2D>();
		// 安全チェック
		if (!transform2D) return;

		transform2D->SetPosition(startPos);
		// 元の位置
		transform2D->SetTexPosition(commandUnSelectTexPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		transform2D->SetScale(commandScale);
		// 元の大きさ
		transform2D->SetTexScale(commandSUnelectTexScale);


		actor->AddComponent<Ui>();
		// 描画チェック
		auto ui = actor->GetComponent<Ui>();
		// 安全チェック
		if (!ui) return;
		ui->SetDrawCheck(true);

		// 非選択状態透明度
		ui->SetAlpha(commandAlphaUnSelect);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI タイトル名前
	{
		const char* filename = "Data/Sprite/EXIT.png";
		auto actor = ActorManager::Instance().Create();
		// 安全チェック
		if (!actor) return;
		actor->SetName("UIExit");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// トランスフォーム（位置、角度、スケール値）
		auto transform2D = actor->GetComponent<TransForm2D>();
		// 安全チェック
		if (!transform2D) return;

		transform2D->SetPosition(exitPos);
		// 元の位置
		transform2D->SetTexPosition(commandUnSelectTexPos);

		float angle = 0;
		transform2D->SetAngle(angle);

		transform2D->SetScale(commandScale);
		// 元の大きさ
		transform2D->SetTexScale(commandSUnelectTexScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		auto ui = actor->GetComponent<Ui>();
		// 安全チェック
		if (!ui) return;
		ui->SetDrawCheck(true);

		// 非選択状態透明度
		ui->SetAlpha(commandAlphaUnSelect);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI ボタン
	{
		const char* filename = "Data/Sprite/Select button.png";
		auto actor = ActorManager::Instance().Create();
		// 安全チェック
		if (!actor) return;
		actor->SetName("UI Button");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// トランスフォーム（位置、角度、スケール値）
		auto transform2D = actor->GetComponent<TransForm2D>();
		// 安全チェック
		if (!transform2D) return;
	
		transform2D->SetPosition(buttonPos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		transform2D->SetScale(buttonScale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		auto ui = actor->GetComponent<Ui>();
		// 安全チェック
		if (!ui) return;
		ui->SetDrawCheck(true);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI 選択
	{
		const char* filename = "Data/Sprite/choice.png";
		auto actor = ActorManager::Instance().Create();
		// 安全チェック
		if (!actor) return;
		actor->SetName("UI Select");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// トランスフォーム（位置、角度、スケール値）
		auto transform2D = actor->GetComponent<TransForm2D>();
		// 安全チェック
		if (!transform2D) return;
		transform2D->SetPosition(startPos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 181,104 };
		transform2D->SetScale(commandScale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		auto ui = actor->GetComponent<Ui>();
		// 安全チェック
		if (!ui) return;
		ui->SetDrawCheck(false);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}
}

void SceneTitle::CameraInitialize()
{
	for (int i = 0; i < PlayerManager::Instance().GetPlayerCount(); ++i)
	{
		std::weak_ptr<Transform> pTransformId = PlayerManager::Instance().GetPlayer(i)->GetComponent<Transform>();

		// 位置
		DirectX::XMFLOAT3 position = pTransformId.lock()->GetPosition();
		// 角度
		DirectX::XMFLOAT3 angle = pTransformId.lock()->GetAngle();
		// 構造体
		MessageData::CAMERACHANGEMOTIONMODEDATA	p;

		// 角度
		float vx = sinf(angle.y) * 6;
		float vz = cosf(angle.y) * 6;

		float vx2 = sinf(angle.y) - 10;
		float vz2 = cosf(angle.y) * 7;
		float vx3 = sinf(angle.y);

		// 時間の設定
		p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
		p.data.push_back({ 50, {position.x + vx, position.y + 3, position.z + vz }, position });
		p.data.push_back({ 100, {position.x - vx2, position.y + 5, position.z - vz2 }, position });
		p.data.push_back({ 140, {position.x - vx3 , position.y + 1, (position.z + 0.1f) - vz2 }, position });

		// 設定
		Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);
	}
}
// スロー状態変更
void SceneTitle::SetSlowState(float elapsedTime)
{
	Graphics& graphics = Graphics::Instance();
	shaderPlayStateTimer -= elapsedTime;
	// エフェクト
	if (shaderPlayStateTimer > 0)
	{
		// 画面白ボケ開始
		colorGradingData.brigthness =
			(colorGradingData.brigthness + FLT_EPSILON) > (colorGradingDataBrigthnessMax - FLT_EPSILON) ?
			colorGradingData.brigthness : colorGradingData.brigthness + (0.01f + elapsedTime);
		// 画面真ん中
		radialBlurData.center = { 0.5f ,0.5f };
		// 画面ブラー
		float radislBlurRadius = 200;
		radialBlurData.radius =
			radialBlurData.radius + FLT_EPSILON > radialBlurDataRadislBlurRadiusMax - FLT_EPSILON ?
			radialBlurData.radius : radialBlurData.radius + (5 + elapsedTime);
	// 歪み具合
		int radislBlurSamplingCount = 10;
		radialBlurData.samplingCount = radislBlurSamplingCount;
		// 自分が見える範囲
		float radislBlurMaskRadius = 300;
		radialBlurData.mask_radius = radislBlurMaskRadius;
		if (!radialBlurData.radius + FLT_EPSILON > radialBlurDataRadislBlurRadiusMax - FLT_EPSILON)
		{
			radialBlurData.radius;
		}

		if (radialBlurData.radius + FLT_EPSILON > radialBlurDataRadislBlurRadiusMax - FLT_EPSILON)
		{
			radialBlurData.radius;
		}

		// ブルーム関係
		{
			bloomData.luminanceExtractionData.threshold;
			bloomData.luminanceExtractionData.intensity;

			bloomData.gaussianFilterData.kernelSize;
			bloomData.gaussianFilterData.deviation;
		}

	}
	else
	{
		// 画面白ボケ
		float colorGradingBrigthness = 0.8f;
		colorGradingData.brigthness = colorGradingBrigthness + FLT_EPSILON > colorGradingData.brigthness - FLT_EPSILON ? colorGradingBrigthness : colorGradingData.brigthness - (0.01f + elapsedTime);
		// ブラー範囲
		float radislBlurRadius = 0;
		radialBlurData.radius = radislBlurRadius - FLT_EPSILON < radialBlurData.radius + FLT_EPSILON ? radialBlurData.radius - (5 + elapsedTime) : radislBlurRadius - FLT_EPSILON;
		// ブラーのかからない範囲
		float radislBlurMaskRadiusNormal = 600;
		float radislBlurMaskRadiusEffectOn = 300;
		radialBlurData.mask_radius = radislBlurRadius - FLT_EPSILON < radialBlurData.radius + FLT_EPSILON ? radislBlurMaskRadiusEffectOn : radislBlurMaskRadiusNormal;
	}

	// 画面スロー
	bool hitCheck = PlayerManager::Instance().GetPlayer(0)->GetComponent<Player>()->GetHitCheck();
	if (hitCheck)
	{
		dlayStateTimer = dlayStateTimerMax;
		PlayerManager::Instance().GetPlayer(0)->GetComponent<Player>()->SetHitCheck(false);
	}
	if (dlayStateTimer - FLT_EPSILON > 0.0f + FLT_EPSILON)
	{
		dlayStateTimer -= 0.1f;
		dlayTimeCheck = true;
		float saturationGageMin = 0.0f;
		colorGradingData.saturation = saturationGageMin - FLT_EPSILON > colorGradingData.saturation + FLT_EPSILON ? colorGradingData.saturation : colorGradingData.saturation - (0.01f + elapsedTime);
	}
	else
	{
		dlayTimeCheck = false;
		float saturationGageMax = 1;
		colorGradingData.saturation = saturationGageMax + FLT_EPSILON < colorGradingData.saturation - FLT_EPSILON ? colorGradingData.saturation : colorGradingData.saturation + (0.01f + elapsedTime);
	}

	// ブラーエフェクト
	if (shaderBlurStateTimer > 0)
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
	else if (shaderPlayStateTimer < 0)
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
// UIコマンド操作
void SceneTitle::SelectScene(float elapsedTime)
{
	// 決定後操作不能
	if (isSelectCommand) return;

	int uiManagerMax = UiManager::Instance().GetUiesCount();
	const GamePadButton anyButton =
		GamePad::BTN_B;
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ay = gamePad.GetAxisLY();
	switch (selectPush)
	{
	case (int)Select::Game:
	{
		// 位置選択
		{

				// UI ボタンを押す
			UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Select)->
				GetComponent<TransForm2D>()->SetPosition({ startPos.x ,startPos.y  });

			UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Push)->
				GetComponent<TransForm2D>()->SetPositionY((startPos.y + buttonOffset));

			// 選択透明度
			UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Start)->GetComponent<Ui>()
				->SetAlpha(commandAlphaSelect);

			// 選択
			UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Start)->GetComponent<TransForm2D>()
				->SetTexPosition(commandSelectTexPos);

			UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Start)->GetComponent<TransForm2D>()
				->SetTexScale(commandSelectTexScale);


			// 非選択透明度
			UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Exit)->GetComponent<Ui>()
				->SetAlpha(commandAlphaUnSelect);
			// 非選択
			UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Exit)->GetComponent<TransForm2D>()
				->SetTexPosition(commandUnSelectTexPos);

			UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Exit)->GetComponent<TransForm2D>()
				->SetTexScale(commandSUnelectTexScale);

		}
		break;
	}
	case (int)Select::Exit:
	{
		// 大きさ変わる選択
		{
			UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Select)->
				GetComponent<TransForm2D>()->SetPosition({ exitPos.x  ,exitPos.y });


			UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Push)->
				GetComponent<TransForm2D>()->SetPositionY((exitPos.y  + buttonOffset));


			// 選択透明度
			UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Exit)->GetComponent<Ui>()
				->SetAlpha(commandAlphaSelect);

			// 選択
			UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Exit)->GetComponent<TransForm2D>()
				->SetTexPosition(commandSelectTexPos);

			UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Exit)->GetComponent<TransForm2D>()
				->SetTexScale(commandSelectTexScale);


			// 非選択透明度
			UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Start)->GetComponent<Ui>()
				->SetAlpha(commandAlphaUnSelect);
			// 非選択
			UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Start)->GetComponent<TransForm2D>()
				->SetTexPosition(commandUnSelectTexPos);

			UiManager::Instance().GetUies((int)UiManager::UiCountTitle::Start)->GetComponent<TransForm2D>()
				->SetTexScale(commandSUnelectTexScale);

		}

		if (gamePad.GetButtonDown() & anyButton)// ロードの次ゲームという書き方
		{
			//　シーン変更
			PostQuitMessage(0);
		}

		break;
	}
	}
	if (isPush) return;

	// コマンド操作スティック操作用
	if (gamePad.GetButtonDown() & GamePad::BTN_UP || ay < 0)
	{
		stickHoldTimerY += elapsedTime;

		if (gamePad.GetButtonDown() & GamePad::BTN_UP || stickHoldTimerY >= stickHoldTime)
		{
			stickHoldTimerY = stickHoldTimerYStart;
			selectPush = selectPush <= (int)Select::Game ? (int)Select::Exit : (int)Select::Game;
		}
	}


	if (gamePad.GetButtonDown() & GamePad::BTN_DOWN || ay > 0)
	{
		stickHoldTimerY += elapsedTime;

		if (gamePad.GetButtonDown() & GamePad::BTN_DOWN || stickHoldTimerY >= stickHoldTime)
		{
			stickHoldTimerY = stickHoldTimerYStart;
			selectPush = selectPush >= (int)Select::Exit ? (int)Select::Game : (int)Select::Exit;
		}
	}
	// 入力が無かったら初期化
	if (ay == 0.0f)
	{

		stickHoldTimerY = stickHoldTimerYStart;

	}
}

