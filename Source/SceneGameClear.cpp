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
    // �X�v���C�g������
    //sprite = new Sprite("Data/Sprite/��Clear.png");

    InitializeComponent();

	// �J����������
	cameraControlle = nullptr;
	cameraControlle = new CameraController();

	// �J���������ݒ� ������ʒu�ǂ���������̂Ȃ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)

	);
	// �ǂ͈̔͂��ǂꂾ�����邩���s�܂�
	camera.SetPerspedtiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);



	// ���s������ǉ�
	{
		mainDirectionalLight = new Light(LightType::Directional);
		//mainDirectionalLight = std::make_unique<Light>(LightType::Directional);
		mainDirectionalLight->SetDirection({ 1,-3,-1 });
		//ambientLightColor = { 0.2f,0.2f,0.2f,0.2f };
		LightManager::Instanes().Register(mainDirectionalLight);
	}

	// �_������ǉ�
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


	// �V�����`��^�[�Q�b�g�̐���
	{
		//Graphics& graphics = Graphics::Instance();
		//renderTarget = new RenderTarget(static_cast<UINT>(graphics.GetScreenWidth())
		renderTarget = std::make_unique<RenderTarget>(static_cast<UINT>(graphics.GetScreenWidth())
			, static_cast<UINT>(graphics.GetScreenHeight())
			, DXGI_FORMAT_R8G8B8A8_UNORM);
	}


	// �V���h�E�}�b�v�p�ɐ[�x�X�e���V���̐���
	{
		shadowmapDepthStencil = std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	}

	// �|�X�g�v���Z�X�`��N���X����
	{
		postprocessingRenderer = std::make_unique<PostprocessingRenderer>();
		// �V�[���e�N�X�`����ݒ肵�Ă���
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


	// �J����������
	//CameraInitialize();

}



void SceneGameClear::Finalize()
{
    //// �X�v���C�g�I����
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

    //// �����{�^�����������烍�[�f�B���O���͂���ŃQ�[���V�[���֐؂�ւ�
    //const GamePadButton anyButton =
    //    GamePad::BTN_A |
    //    GamePad::BTN_B |
    //    GamePad::BTN_X |
    //    GamePad::BTN_Y;

    //if (gamePad.GetButtonUp() & anyButton)// ���[�h�̎��Q�[���Ƃ���������
    //    SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));


	GamePad& gamePad = Input::Instance().GetGamePad();

	// �J�����R���g���[���[�X�V����
	//cameraControlle->Update(elapsedTime);

	ActorManager::Instance().Update(elapsedTime);

	// �J�����R���g���[���[�X�V����
	cameraControlle->Update(elapsedTime);

	// �G�t�F�N�g�X�V����
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
	//	//	//�@�V�[���ύX
	//	//	SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
	//	//}


	//	// �����{�^�����������烍�[�f�B���O���͂���ŃQ�[���V�[���֐؂�ւ�
	//	const GamePadButton anyButton =
	//		GamePad::BTN_B;

	//	if (!enemyid.lock()->GetComponent<EnemyBoss>()->GetClearCheck()) return;

	//	if (gamePad.GetButtonDown() & anyButton)// ���[�h�̎��Q�[���Ƃ���������
	//	{
	//		UiManager::Instance().GetUies(uiManagerMax - 2)->GetComponent<Ui>()->SetDrawCheck(false);
	//		UiManager::Instance().GetUies(uiManagerMax - 1)->GetComponent<Ui>()->SetDrawCheck(true);
	//		//�@�V�[���ύX
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

	//// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	//FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };   // RGBA(0.0~1.0)
	//dc->ClearRenderTargetView(rtv, color);
	//dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//dc->OMSetRenderTargets(1, &rtv, dsv);

	//// �V���h�E�}�b�v�̕`��
	RenderShadowmap();
	// model�`��
	Render3DScene();

	// GUI
	ActorManager::Instance().RenderGui();

	// �������ݐ���o�b�N�o�b�t�@�ɕς��ăI�t�X�N���[�������_�����O�̌��ʂ�`�悷��
	{
		ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();


		// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
		FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f }; // RGBA(0.0~1.0)
		dc->ClearRenderTargetView(rtv, color);
		dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		dc->OMSetRenderTargets(1, &rtv, dsv);


		// UINT11
		// �r���[�|�[�g�̐ݒ�
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


		// ���ӌ���
		rc.vignetteData.color = vignetteData.color;
		rc.vignetteData.center = vignetteData.center;
		rc.vignetteData.intensity = vignetteData.intensity;
		rc.vignetteData.smoothness = vignetteData.smoothness;
		rc.vignetteData.rounded = vignetteData.rounded;
		rc.vignetteData.roundness = vignetteData.roundness;



		//postprocessingRenderer->SetBloomData();

		// �|�X�g�v���Z�X���������s��
		postprocessingRenderer->Render(rc);


	}

	// 2D�X�v���C�g�`��
	{
		// �`�揈��
		RenderContext rc;
		rc.deviceContext = dc;

		// �J�����p�����[�^�ݒ�
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

    //// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    //FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };   // RGBA(0.0~1.0)
    //dc->ClearRenderTargetView(rtv, color);
    //dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    //dc->OMSetRenderTargets(1, &rtv, dsv);

    //// 2D�X�v���C�g�`��
    //{
    //    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    //    float screenHeight = static_cast<float>(graphics.GetScreenHeight());
    //    float textureWidth = static_cast<float>(sprite->GetTextureWidth());
    //    float textureHeight = static_cast<float>(sprite->GetTextureHeight());
    //    // �`��
    //    sprite->Render(dc,
    //        0, 0, screenWidth, screenHeight,
    //        0, 0, textureWidth, textureHeight,
    //        0,
    //        1, 1, 1, 1);
    //    // {�ʒu}{�T�C�Y}{�摜�ǂ�����}{�摜�����܂�}
    //    // dc , �o�͈́p�o�p
    //}
}

void SceneGameClear::Render3DScene()
{

	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = renderTarget->GetRenderTargetView().Get();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
	PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp = {};
	vp.Width = graphics.GetScreenWidth();
	vp.Height = graphics.GetScreenHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// �`�揈��
	RenderContext rc;
	rc.deviceContext = dc;


	// ���C�g�̏����l�ߍ���
	LightManager::Instanes().PushRenderContext(rc);

	// UNIT11
	// ���C�g�̏����߂��� 
	rc.shadowMapData.shadowMap = shadowmapDepthStencil->GetShaderResourceView().Get();
	rc.shadowMapData.lightViewProjection = lightViewProjeciton;
	rc.shadowMapData.shadowColor = shadowColor;
	rc.shadowMapData.shadowBias = shadowBias;



	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();



	// 3D���f���`��
	{

		ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);

		ActorManager::Instance().Render(rc, shader);

	}

	// 3D�G�t�F�N�g�`��
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

	// ��ʃN���A
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (!mainDirectionalLight)
		return;

	dc->OMSetRenderTargets(0, &rtv, dsv);

	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp = {};
	vp.Width = static_cast<float>(shadowmapDepthStencil->GetWidth());
	vp.Height = static_cast<float> (shadowmapDepthStencil->GetHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// �`�揈��
	RenderContext rc;
	rc.deviceContext = dc;

	// �J�����p�����[�^�[�ݒ�
	{
		// ���s��������J�����ʒu���쐬���A�������猴�_�̈ʒu������悤�Ɏ����s��𐶐�
		DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat3(&mainDirectionalLight->GetDirection());
		LightPosition = DirectX::XMVectorScale(LightPosition, -3.0f);
		//LightPosition = DirectX::XMVectorScale(LightPosition, lightPositionScale);
		DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(LightPosition,
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

		// �V���h�E�}�b�v�ɕ`�悵�����͈͂̎ˉe�s��𐶐�
		DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(shadowDrawRect, shadowDrawRect, 0.1f,
			1000.0f);
		DirectX::XMStoreFloat4x4(&rc.view, V);
		DirectX::XMStoreFloat4x4(&rc.projection, P);
		DirectX::XMStoreFloat4x4(&lightViewProjeciton, V * P);
	}

	// �V�F�[�_�[
	{
		ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);

		ActorManager::Instance().Render(rc, shader);
	}
}

void SceneGameClear::InitializeComponent()
{
	// �s���͈�
	{

		minPos.x = -30;
		minPos.y = -3.525f;
		minPos.z = -30;

		maxPos.x = 30;
		maxPos.y = 3.625f;
		maxPos.z = 30;
	}


	// �X�e�[�W������
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

		// �e�V�F�[�_�[
		actor->GetComponent<StageMain>()->SetIsRimRightning(0);

		// �𑜓x
		actor->GetComponent<StageMain>()->SetTexcoordMult(20);

		// �X�y�L�����[
		actor->GetComponent<StageMain>()->SetIsSpecular(0);


		// ���ꂪ�QD���̊m�F
		bool check2d = false;
		actor->SetCheck2d(check2d);

		StageManager::Instance().Register(actor);




	}



	// �X�e�[�W���r�[������
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

		// �e�V�F�[�_�[
		actor->GetComponent<StageMain>()->SetIsRimRightning(1);

		// �𑜓x
		actor->GetComponent<StageMain>()->SetTexcoordMult(1);

		// �X�y�L�����[
		actor->GetComponent<StageMain>()->SetIsSpecular(1);


		// ���ꂪ�QD���̊m�F
		bool check2d = false;
		actor->SetCheck2d(check2d);

		StageManager::Instance().Register(actor);



	}



	////player = new Player;
	{
		// �v���C���[������
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

		// ui�̗L���ŏ��������邩��ς���
		actor->GetComponent<Player>()->SetUiControlleCheck(false);

		// �X�e�[�g�ݒ�
		actor->GetComponent<Player>()->StateMachineCreate();

		actor->GetComponent<Player>()->GetStateMachine()->RegisterState(new PlayerClearIdleState(actor));

		// �X�e�[�g�Z�b�g
		actor->GetComponent<Player>()->GetStateMachine()->SetState(static_cast<int>(Player::StateClear::Idle));

		actor->AddComponent<Collision>();

		// ���ꂪ�QD���̊m�F
		bool check2d = false;
		actor->SetCheck2d(check2d);

		PlayerManager::Instance().Register(actor);





	}


	// �G
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


		// �s���͈͐ݒ�
		actor->GetComponent<Movement>()->SetArea(minPos, maxPos);
		actor->AddComponent<HP>();
		std::shared_ptr<HP> hp = actor->GetComponent<HP>();
		int life = 2;
		hp->SetLife(life);
		actor->AddComponent<Collision>();
		actor->AddComponent<EnemyBoss>();

		// �N���G�C�g
		actor->GetComponent<EnemyBoss>()->StateMachineCreate();

		// �X�e�[�g�}�V���ɃX�e�[�g�o�^
		//actor->GetComponent<EnemyBoss>()->GetStateMachine()->RegisterState(new DeathState(actor));
		actor->GetComponent<EnemyBoss>()->GetStateMachine()->RegisterState(new ClearState(actor));

		// �X�e�[�g�Z�b�g
		actor->GetComponent<EnemyBoss>()->GetStateMachine()->SetState(static_cast<int>(EnemyBoss::ClearState::Death));


		// ���ꂪ�QD���̊m�F
		bool check2d = false;
		actor->SetCheck2d(check2d);

		EnemyManager::Instance().Register(actor);

		//

	}

	// UI �^�C�g�����O
	{
		const char* filename = "Data/Sprite/Clear.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UIClear");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// �ʒu�@�p�x�@�X�P�[�����
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 785, 35 };
		transform2D->SetPosition(pos);
		// ���̈ʒu
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 376,329 };
		transform2D->SetScale(scale);
		// ���̑傫��
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// �`��`�F�b�N
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(true);

		// ���ꂪ�QD���̊m�F
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}



	// UI �^�C�g�����O
	{
		const char* filename = "Data/Sprite/�^�C�g���߂�.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UITitle");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// �ʒu�@�p�x�@�X�P�[�����
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 543, 515 };
		transform2D->SetPosition(pos);
		// ���̈ʒu
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);

		transform2D->SetScale(titleUiScaleSelected);
		// ���̑傫��
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// �`��`�F�b�N
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(true);

		// ���ꂪ�QD���̊m�F
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}


	// UI �^�C�g�����O
	{
		const char* filename = "Data/Sprite/�Q�[���߂�.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UIGame");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// �ʒu�@�p�x�@�X�P�[�����
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 543, 515 };
		transform2D->SetPosition(pos);
		// ���̈ʒu
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);

		transform2D->SetScale(gameUiScaleSelected);
		// ���̑傫��
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// �`��`�F�b�N
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(true);

		// ���ꂪ�QD���̊m�F
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	//// UI �^�C�g�����O
	//{
	//	const char* filename = "Data/Sprite/�R�}���hPUSH.png";
	//	std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
	//	actor->SetName("UIPush");
	//	actor->AddComponent<SpriteControll>();
	//	actor->GetComponent<SpriteControll>()->LoadSprite(filename);
	//	actor->AddComponent<TransForm2D>();
	//	// �ʒu�@�p�x�@�X�P�[�����
	//	std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
	//	DirectX::XMFLOAT2 pos = { 543, 515 };
	//	transform2D->SetPosition(pos);
	//	// ���̈ʒu
	//	DirectX::XMFLOAT2 texPos = { 0, 0 };
	//	transform2D->SetTexPosition(texPos);

	//	float angle = 0;
	//	transform2D->SetAngle(angle);
	//	DirectX::XMFLOAT2 scale = { 181,104 };
	//	transform2D->SetScale(scale);
	//	// ���̑傫��
	//	DirectX::XMFLOAT2 texScale = { 0,0 };
	//	transform2D->SetTexScale(texScale);

	//	actor->AddComponent<Ui>();
	//	// �`��`�F�b�N
	//	std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
	//	ui->SetDrawCheck(false);

	//	// ���ꂪ�QD���̊m�F
	//	bool check2d = true;
	//	actor->SetCheck2d(check2d);

	//	UiManager::Instance().Register(actor);
	//}


	//// UI �^�C�g�����O
	//{
	//	const char* filename = "Data/Sprite/�R�}���hPUSH��������.png";
	//	std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
	//	actor->SetName("UIPushOn");
	//	actor->AddComponent<SpriteControll>();
	//	actor->GetComponent<SpriteControll>()->LoadSprite(filename);
	//	actor->AddComponent<TransForm2D>();
	//	// �ʒu�@�p�x�@�X�P�[�����
	//	std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
	//	DirectX::XMFLOAT2 pos = { 543, 515 };
	//	transform2D->SetPosition(pos);
	//	// ���̈ʒu
	//	DirectX::XMFLOAT2 texPos = { 0, 0 };
	//	transform2D->SetTexPosition(texPos);

	//	float angle = 0;
	//	transform2D->SetAngle(angle);
	//	DirectX::XMFLOAT2 scale = { 181,104 };
	//	transform2D->SetScale(scale);
	//	// ���̑傫��
	//	DirectX::XMFLOAT2 texScale = { 0,0 };
	//	transform2D->SetTexScale(texScale);

	//	actor->AddComponent<Ui>();
	//	// �`��`�F�b�N
	//	std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
	//	ui->SetDrawCheck(false);

	//	// ���ꂪ�QD���̊m�F
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
		// �Q�[��
		UiManager::Instance().GetUies(uiManagerMax - 1)->GetComponent<TransForm2D>()->
			SetPositionY(gameUiPositionSelected);
		// �^�C�g��
		UiManager::Instance().GetUies(uiManagerMax - 2)->GetComponent<TransForm2D>()->
			SetPositionY(titleUiPositionSelected);

		UiManager::Instance().GetUies(uiManagerMax - 2)->GetComponent<TransForm2D>()->
			SetScale(titleUiScaleSelected);
		UiManager::Instance().GetUies(uiManagerMax - 1)->GetComponent<TransForm2D>()->
			SetScale(gameUiScaleUnselected);


		if (gamePad.GetButtonDown() & anyButton)// ���[�h�̎��Q�[���Ƃ���������
		{
			//�@�V�[���ύX
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
		}

		break;
	}
	case (int)Select::Game:
	{
		// �^�C�g��
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
			//if (gamePad.GetButtonDown() & anyButton)// ���[�h�̎��Q�[���Ƃ���������
			//{
			//	// �X�e�[�g�ϊ�
			//	playerId.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::StateOver::Revive));

			//	////�@�V�[���ύX
			//	//SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
			//}


			if (playerId.lock()->GetEndState())
			{
				//�@�V�[���ύX
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
			}
		}

		break;
	}
	}
}
