#include "SceneGameClear.h"
#include "Graphics/Graphics.h"
#include "StageMain.h"
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

void SceneGameClear::Initialize()
{
    // スプライト初期化
    //sprite = new Sprite("Data/Sprite/仮Clear.png");

    InitializeComponent();

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
		//mainDirectionalLight = std::make_unique<Light>(LightType::Directional);
		mainDirectionalLight->SetDirection({ 1,-3,-1 });
		//ambientLightColor = { 0.2f,0.2f,0.2f,0.2f };
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
		//Graphics& graphics = Graphics::Instance();
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
	{
		postprocessingRenderer = std::make_unique<PostprocessingRenderer>();
		// シーンテクスチャを設定しておく
		ShaderResourceViewData srvData;
		srvData.srv = renderTarget->GetShaderResourceView();
		srvData.width = renderTarget->GetWidth();
		srvData.height = renderTarget->GetHeight();
		//srvData.width = 100;
		//srvData.height = 100;
		postprocessingRenderer->SetSceneData(srvData);
	}


	bloomData.luminanceExtractionData.threshold = 0.41f;
	bloomData.luminanceExtractionData.intensity = 1.6f;

	bloomData.gaussianFilterData.kernelSize = 15;
	bloomData.gaussianFilterData.deviation = 8.3f;


	// カメラ初期化
	//CameraInitialize();

}



void SceneGameClear::Finalize()
{
    //// スプライト終了化
    //if (this->sprite)
    //{
    //    delete sprite;
    //    sprite = nullptr;
    //}

	StageManager::Instance().Clear();

	LightManager::Instanes().Clear();

	PlayerManager::Instance().Clear();

	EnemyManager::Instance().Clear();

	UiManager::Instance().Clear();

	ActorManager::Instance().Clear();

	if (cameraControlle != nullptr)
	{
		delete cameraControlle;
		cameraControlle = nullptr;
	}

}

void SceneGameClear::Update(float elapsedTime)
{


    //GamePad& gamePad = Input::Instance().GetGamePad();

    //// 何かボタンを押したらローディングをはさんでゲームシーンへ切り替え
    //const GamePadButton anyButton =
    //    GamePad::BTN_A |
    //    GamePad::BTN_B |
    //    GamePad::BTN_X |
    //    GamePad::BTN_Y;

    //if (gamePad.GetButtonUp() & anyButton)// ロードの次ゲームという書き方
    //    SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));


	GamePad& gamePad = Input::Instance().GetGamePad();

	// カメラコントローラー更新処理
	//cameraControlle->Update(elapsedTime);

	ActorManager::Instance().Update(elapsedTime);

	// カメラコントローラー更新処理
	cameraControlle->Update(elapsedTime);

	// エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);

	if(cameraControlle->GetCameraMortionDataTime())
	PlayEffectsShaders(elapsedTime);

	int uiManagerMax = UiManager::Instance().GetUiesCount();

	SelectScene();

	//for (int i = 0; i < EnemyManager::Instance().GetEnemyCount(); ++i)
	//{
	//	std::weak_ptr<Actor> enemyid = EnemyManager::Instance().GetEnemy(i);

	//	//if (enemyid.lock()->GetComponent<EnemyBoss>()->GetFlashOn())
	//	//{
	//	//	shaderPlayStateTimer = shaderPlayStateTimerMax;
	//	//	shaderBlurStateTimer = shaderBlurStateTimerMax;
	//	//}

	//	//if (enemyid.lock()->GetComponent<EnemyBoss>()->GetClearCheck())
	//	//{
	//	//	//　シーン変更
	//	//	SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	//	//}


	//	// 何かボタンを押したらローディングをはさんでゲームシーンへ切り替え
	//	const GamePadButton anyButton =
	//		GamePad::BTN_B;

	//	if (!enemyid.lock()->GetComponent<EnemyBoss>()->GetClearCheck()) return;

	//	if (gamePad.GetButtonDown() & anyButton)// ロードの次ゲームという書き方
	//	{
	//		UiManager::Instance().GetUies(uiManagerMax - 2)->GetComponent<Ui>()->SetDrawCheck(false);
	//		UiManager::Instance().GetUies(uiManagerMax - 1)->GetComponent<Ui>()->SetDrawCheck(true);
	//		//　シーン変更
	//		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	//	}
	//	else
	//	{
	//		UiManager::Instance().GetUies(uiManagerMax - 2)->GetComponent<Ui>()->SetDrawCheck(true);
	//		UiManager::Instance().GetUies(uiManagerMax - 1)->GetComponent<Ui>()->SetDrawCheck(false);
	//	}

	//}

}

void SceneGameClear::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	//ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	//ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	//// 画面クリア＆レンダーターゲット設定
	//FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };   // RGBA(0.0~1.0)
	//dc->ClearRenderTargetView(rtv, color);
	//dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//dc->OMSetRenderTargets(1, &rtv, dsv);

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



		//postprocessingRenderer->SetBloomData();

		// ポストプロセスを処理を行う
		postprocessingRenderer->Render(rc);


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

    //Graphics& graphics = Graphics::Instance();
    //ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    //ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    //ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //// 画面クリア＆レンダーターゲット設定
    //FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };   // RGBA(0.0~1.0)
    //dc->ClearRenderTargetView(rtv, color);
    //dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    //dc->OMSetRenderTargets(1, &rtv, dsv);

    //// 2Dスプライト描画
    //{
    //    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    //    float screenHeight = static_cast<float>(graphics.GetScreenHeight());
    //    float textureWidth = static_cast<float>(sprite->GetTextureWidth());
    //    float textureHeight = static_cast<float>(sprite->GetTextureHeight());
    //    // 描画
    //    sprite->Render(dc,
    //        0, 0, screenWidth, screenHeight,
    //        0, 0, textureWidth, textureHeight,
    //        0,
    //        1, 1, 1, 1);
    //    // {位置}{サイズ}{画像どこから}{画像何処まで}
    //    // dc , ｛範囲｝｛｝
    //}
}

void SceneGameClear::Render3DScene()
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

void SceneGameClear::RenderShadowmap()
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
		//LightPosition = DirectX::XMVectorScale(LightPosition, lightPositionScale);
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

void SceneGameClear::InitializeComponent()
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
		//const char* filename = "Data/Model/Jammo/Jammo.mdl";
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

		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerClearIdleState(actor));

		// ステートセット
		actor->GetComponent<Player>()->GetStateMachine()->SetState(static_cast<int>(Player::StateClear::Idle));

		actor->AddComponent<Collision>();

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
		//actor->GetComponent<EnemyBoss>()->GetStateMachine()->RegisterState(new DeathState(actor));
		actor->GetComponent<EnemyBoss>()->GetStateMachine()->RegisterState(new ClearState(actor));

		// ステートセット
		actor->GetComponent<EnemyBoss>()->GetStateMachine()->SetState(static_cast<int>(EnemyBoss::ClearState::Death));


		// これが２Dかの確認
		bool check2d = false;
		actor->SetCheck2d(check2d);

		EnemyManager::Instance().Register(actor);

		//

	}

	// UI タイトル名前
	{
		const char* filename = "Data/Sprite/Clear.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UIClear");
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
		const char* filename = "Data/Sprite/タイトル戻る.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UITitle");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 543, 515 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);

		transform2D->SetScale(titleUiScaleSelected);
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
		const char* filename = "Data/Sprite/ゲーム戻る.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UIGame");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 543, 515 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);

		transform2D->SetScale(gameUiScaleSelected);
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

	//// UI タイトル名前
	//{
	//	const char* filename = "Data/Sprite/コマンドPUSH.png";
	//	std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
	//	actor->SetName("UIPush");
	//	actor->AddComponent<SpriteControll>();
	//	actor->GetComponent<SpriteControll>()->LoadSprite(filename);
	//	actor->AddComponent<TransForm2D>();
	//	// 位置　角度　スケール情報
	//	std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
	//	DirectX::XMFLOAT2 pos = { 543, 515 };
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

	//	actor->AddComponent<Ui>();
	//	// 描画チェック
	//	std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
	//	ui->SetDrawCheck(false);

	//	// これが２Dかの確認
	//	bool check2d = true;
	//	actor->SetCheck2d(check2d);

	//	UiManager::Instance().Register(actor);
	//}


	//// UI タイトル名前
	//{
	//	const char* filename = "Data/Sprite/コマンドPUSH押し込み.png";
	//	std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
	//	actor->SetName("UIPushOn");
	//	actor->AddComponent<SpriteControll>();
	//	actor->GetComponent<SpriteControll>()->LoadSprite(filename);
	//	actor->AddComponent<TransForm2D>();
	//	// 位置　角度　スケール情報
	//	std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
	//	DirectX::XMFLOAT2 pos = { 543, 515 };
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

void SceneGameClear::CameraInitialize()
{
}

void SceneGameClear::CameraUpdate(float elapsedTime)
{
}

void SceneGameClear::PlayEffectsShaders(float elapsedTime)
{
}

void SceneGameClear::SelectScene()
{
	int uiManagerMax = UiManager::Instance().GetUiesCount();

	const GamePadButton anyButton =
		GamePad::BTN_B;

	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_UP)
	{
		selectPush = selectPush <= (int)Select::Title ? (int)Select::Game : (int)Select::Title;
	}
	if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)
	{
		selectPush = selectPush >= (int)Select::Game ? (int)Select::Title : (int)Select::Game;
	}

	switch (selectPush)
	{
	case (int)Select::Title:
	{
		// ゲーム
		UiManager::Instance().GetUies(uiManagerMax - 1)->GetComponent<TransForm2D>()->
			SetPositionY(gameUiPositionSelected);
		// タイトル
		UiManager::Instance().GetUies(uiManagerMax - 2)->GetComponent<TransForm2D>()->
			SetPositionY(titleUiPositionSelected);

		UiManager::Instance().GetUies(uiManagerMax - 2)->GetComponent<TransForm2D>()->
			SetScale(titleUiScaleSelected);
		UiManager::Instance().GetUies(uiManagerMax - 1)->GetComponent<TransForm2D>()->
			SetScale(gameUiScaleUnselected);


		if (gamePad.GetButtonDown() & anyButton)// ロードの次ゲームという書き方
		{
			//　シーン変更
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
		}

		break;
	}
	case (int)Select::Game:
	{
		// タイトル
		UiManager::Instance().GetUies(uiManagerMax - 2)->GetComponent<TransForm2D>()->
			SetPositionY(titleUiPositionUnselected);


		UiManager::Instance().GetUies(uiManagerMax - 2)->GetComponent<TransForm2D>()->
			SetScale(titleUiScaleUnselected);
		UiManager::Instance().GetUies(uiManagerMax - 1)->GetComponent<TransForm2D>()->
			SetScale(gameUiScaleSelected);

		int playerCount = PlayerManager::Instance().GetPlayerCount();
		for (int i = 0; i < playerCount; ++i)
		{
			std::weak_ptr<Player> playerId = PlayerManager::Instance().GetPlayer(i)->GetComponent<Player>();
			//if (gamePad.GetButtonDown() & anyButton)// ロードの次ゲームという書き方
			//{
			//	// ステート変換
			//	playerId.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::StateOver::Revive));

			//	////　シーン変更
			//	//SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
			//}


			if (playerId.lock()->GetEndState())
			{
				//　シーン変更
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
			}
		}

		break;
	}
	}
}
