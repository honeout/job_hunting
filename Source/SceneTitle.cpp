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
    // スプライト初期化

    //sprite = std::make_unique<Sprite>("Data/Sprite/Title.png");
    //sprite = std::make_unique<Sprite>("Data/Sprite/タイトル.png");
    //spritePush = std::make_unique<Sprite>("Data/Sprite/コマンドPUSH.png");

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
	cameraControlle = nullptr;
	cameraControlle = new CameraController();

	// カメラ初期化
	//CameraInitialize();


	// カメラ初期化
	//cameraControlle = new CameraController();

}
// 終了化
void SceneTitle::Finalize()
{
	//if (cameraControlle != nullptr)
	//{
	//	delete cameraControlle;
	//	cameraControlle = nullptr;
	//}
	

	StageManager::Instance().Clear();

	LightManager::Instanes().Clear();

	PlayerManager::Instance().Clear();

	UiManager::Instance().Clear();

	ActorManager::Instance().Clear();

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

	// カメラコントローラー更新処理
	//cameraControlle->Update(elapsedTime);

	ActorManager::Instance().Update(elapsedTime);

	// カメラコントローラー更新処理
	cameraControlle->Update(elapsedTime);

	// エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);

	PlayEffectsShaders(elapsedTime);

	int uiManagerMax = UiManager::Instance().GetUiesCount();
	// 何かボタンを押したらローディングをはさんでゲームシーンへ切り替え
	const GamePadButton anyButton =
		GamePad::BTN_B;

	if (gamePad.GetButtonDown() & anyButton)// ロードの次ゲームという書き方
	{
		UiManager::Instance().GetUies(uiManagerMax - 2)->GetComponent<Ui>()->SetDrawCheck(false);
		UiManager::Instance().GetUies(uiManagerMax - 1)->GetComponent<Ui>()->SetDrawCheck(true);
	}
	else
	{
		UiManager::Instance().GetUies(uiManagerMax - 2)->GetComponent<Ui>()->SetDrawCheck(true);
		UiManager::Instance().GetUies(uiManagerMax - 1)->GetComponent<Ui>()->SetDrawCheck(false);
	}

	for (int i = 0; i < PlayerManager::Instance().GetPlayerCount(); ++i)
	{
		std::weak_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(i);

		if (playerid.lock()->GetComponent<Player>()->GetFlashOn())
		{
			shaderPlayStateTimer = shaderPlayStateTimerMax;
			shaderBlurStateTimer = shaderBlurStateTimerMax;
		}

		if (playerid.lock()->GetComponent<Player>()->GetEndState())
		{
			//　シーン変更
			playerid.lock()->GetComponent<Player>()->SetEndState(false);
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
		}

	}


	// カメラ更新処理
	//CameraUpdate(elapsedTime);




}
// 描画処理
void SceneTitle::Render()
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

    //// 2Dスプライト描画
    //{
    //    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    //    float screenHeight = static_cast<float>(graphics.GetScreenHeight());
    //    float textureWidth = static_cast<float>(sprite->GetTextureWidth());
    //    float textureHeight = static_cast<float>(sprite->GetTextureHeight());

    //    float texturePushWidth = static_cast<float>(spritePush->GetTextureWidth());
    //    float texturePushHeight = static_cast<float>(spritePush->GetTextureHeight());
    //    // 描画
    //    sprite->Render(dc,
    //        0, 0, screenWidth, screenHeight,
    //        0, 0, textureWidth, textureHeight,
    //        0,
    //        1, 1, 1, 1);
    //    // {位置}{サイズ}{画像どこから}{画像何処まで}
    //    // dc , ｛範囲｝｛｝

    //    // Push
    //    spritePush->Render(dc,
    //        position.x, position.y, texturePushWidth, texturePushHeight,
    //        0, 0, texturePushWidth, texturePushHeight,
    //        0,
    //        1, 1, 1, 1);
    //    // {位置}{サイズ}{画像どこ
    //}
}
//void SceneTitle::DebugGui()
//{
//}
// model描画
void SceneTitle::Render3DScene()
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

void SceneTitle::InitializeComponent()
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
		const char* filename = "Data/Sprite/タイトル画像.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UITitle");
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
		const char* filename = "Data/Sprite/コマンドPUSH.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UIPush");
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
		const char* filename = "Data/Sprite/コマンドPUSH押し込み.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UIPushOn");
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

void SceneTitle::CameraUpdate(float elapsedTime)
{
	

}


void SceneTitle::PlayEffectsShaders(float elapsedTime)
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
		//radialBlurData.radius + FLT_EPSILON > radislBlurRadius - FLT_EPSILON ?
		//radialBlurData.radius + (5 + elapsedTime) : radialBlurData.radius;
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

		//shaderPlayStateTimer = shaderPlayStateTimerMax;

		// ブラー範囲
		float radislBlurRadius = 0;
		radialBlurData.radius = radislBlurRadius - FLT_EPSILON < radialBlurData.radius + FLT_EPSILON ? radialBlurData.radius - (5 + elapsedTime) : radislBlurRadius - FLT_EPSILON;

		//float radislBlurSamplingCount = 10;
		//radialBlurData.samplingCount = radislBlurSamplingCount;
		//float radislBlurMaskRadius = 30;
		//radialBlurData.mask_radius = radislBlurMaskRadius;
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


	//if (hitCheck)
	//{
	//	dlayStateTimer = dlayStateTimerMax;

	//	dlayTimeCheck = true;

	//	float saturationGageMin = 0.0f;

	//	colorGradingData.saturation = saturationGageMin - FLT_EPSILON > colorGradingData.saturation + FLT_EPSILON ? colorGradingData.saturation : colorGradingData.saturation - (0.01f + elapsedTime);
	//}

	//else
	//{
	//	dlayTimeCheck = false;

	//	float saturationGageMax = 1;

	//	colorGradingData.saturation = saturationGageMax + FLT_EPSILON < colorGradingData.saturation - FLT_EPSILON ? colorGradingData.saturation : colorGradingData.saturation + (0.01f + elapsedTime);
	//}
}

