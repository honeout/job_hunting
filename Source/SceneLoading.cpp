#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "EnemyBoss.h"
#include "StageMain.h"
#include "SceneLoading.h"
#include "Camera.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "SceneGame.h"

// ������
void SceneLoading::Initialize()
{
    // �R���|�[�l���g�w��
    InitializeComponent();

    currentScene = std::make_unique<SceneGame>();

    // �Q�[�������[�h���ɏ�����
    isFinalizeGame = true;

    //// �X�v���C�g������
    //spriteLoading = std::make_unique<Sprite>("Data/Sprite/LoadingIcon.png");
    //spriteControlManual = std::make_unique<Sprite>("Data/Sprite/xbx������@.png");
    //// �X���b�h�J�n      (�֐����A�����̒��g �������M)�X���b�h�����グ��
    // ���������ł��������B�֐��̈����̒����ɂ��
    this->thread = new std::thread(LoadingThread,this);
}

void SceneLoading::Start()
{
}

// �I����
void SceneLoading::Finalize()
{
    ActorSceneLoadManager::Instance().Clear();
    UiLoadingManager::Instance().Clear();
    if (!nextScene->IsReady())
    {
        ActorManager::Instance().Clear();
        LightManager::Instanes().Clear();
        EnemyManager::Instance().Clear();
        PlayerManager::Instance().Clear();
        StageManager::Instance().Clear();
        UiManager::Instance().Clear();
        ProjectileManager::Instance().Clear();
        
    }
    // �X���b�h�I����
    if (this->thread)
    {
        // �������Ă���Œ��Ƀf���[�g����Ƃ܂���������܂�
        thread->join();
        delete thread;
        thread = nullptr;
    }

    //if (this->nextScene)
    if (isFinalizeGame)
    {
        delete nextScene;
        nextScene = nullptr;
    }
    //if (!nextScene->IsReady())
    //    nextScene = nullptr;
}
// �X�V����
void SceneLoading::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    // ��]����
    RotateLoadingIcon(elapsedTime);

    ActorSceneLoadManager::Instance().Update(elapsedTime);

    // �Q�[���X�^�[�g��������UI�\��
    if (nextScene->IsReady())
    {
        // ���[�h�̕`��`�F�b�N
        UiLoadingManager::Instance().GetUies(
            (int)UiLoadingManager::UiCountLoading::LodingIcon)->
            GetComponent<Ui>()->SetDrawCheck(false);

        // ���[�h�̕`��`�F�b�N
        UiLoadingManager::Instance().GetUies(
            (int)UiLoadingManager::UiCountLoading::NowLoading)->
            GetComponent<Ui>()->SetDrawCheck(false);

        // �Q�[���X�^�[�g�̕`��`�F�b�N
        UiLoadingManager::Instance().GetUies(
            (int)UiLoadingManager::UiCountLoading::GameStart)->
            GetComponent<Ui>()->SetDrawCheck(true);

        // �Q�[���X�^�[�g�̃X�^�[�g�`��`�F�b�N
        UiLoadingManager::Instance().GetUies(
            (int)UiLoadingManager::UiCountLoading::Start)->
            GetComponent<Ui>()->SetDrawCheck(true);


        // �Q�[���X�^�[�g�̃{�^���`��`�F�b�N
        UiLoadingManager::Instance().GetUies(
            (int)UiLoadingManager::UiCountLoading::Button)->
            GetComponent<Ui>()->SetDrawCheck(true);
    }

    // ���̃V�[���̏���������������V�[����؂�ւ���
    if (nextScene->IsReady() && 
        gamePad.GetButtonDown() & GamePad::BTN_B)
    {
        isFinalizeGame = false;
        SceneManager::Instance().ChangeScene(nextScene);
        nextScene = nullptr;
    }
}
// �`�揈��
void SceneLoading::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // GUI
    ActorSceneLoadManager::Instance().RenderGui();

    // ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // RGBA(0.0�`1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    //// �QD�X�v���C�g�`��
    //{
    //    // ��ʉE���Ƀ��[�f�B���O�A�C�R����`��
    //    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    //    float screenHeight = static_cast<float>(graphics.GetScreenHeight());

    //    float textureWidth = static_cast<float>(spriteLoading->GetTextureWidth());
    //    float textureHeight = static_cast<float>(spriteLoading->GetTextureHeight());
    //    float positionX = screenWidth - textureWidth;
    //    float positionY = screenHeight - textureHeight;


    //    Render2D(spriteLoading, angleLoading, dc, { positionX ,positionY });
    //}
    //Render2D(spriteControlManual, angle, dc, {200, 300});

    //Graphics& graphics = Graphics::Instance();
    //ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    //ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    //ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    //// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    //FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // RGBA(0.0�`1.0)
    //dc->ClearRenderTargetView(rtv, color);
    //dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    //dc->OMSetRenderTargets(1, &rtv, dsv);

    //// �QD�X�v���C�g�`��
    //{
    //    // ��ʉE���Ƀ��[�f�B���O�A�C�R����`��
    //    float screenWidth = static_cast<float>(graphics.GetScreenWidth());
    //    float screenHeight = static_cast<float>(graphics.GetScreenHeight());
    //    float textureWidth = static_cast<float>(spriteLoading->GetTextureWidth());
    //    float textureHeight = static_cast<float>(spriteLoading->GetTextureHeight());
    //    float positionX = screenWidth - textureWidth;
    //    float positionY = screenHeight - textureHeight;

    //    spriteLoading->Render(dc,
    //        positionX, positionY, textureWidth, textureHeight,
    //        0, 0, textureWidth, textureHeight,
    //        angle,
    //        1, 1, 1, 1);
    //}


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

        ActorSceneLoadManager::Instance().Render(rc, shaderUi);
    }
}

void SceneLoading::Render2D(std::shared_ptr<Sprite> sprite,
    float angle, ID3D11DeviceContext* dc, DirectX::XMFLOAT2 pos)
{
    float textureWidth = static_cast<float>(sprite->GetTextureWidth());
    float textureHeight = static_cast<float>(sprite->GetTextureHeight());

        sprite->Render(dc,
            pos.x, pos.y, textureWidth, textureHeight,
            0, 0, textureWidth, textureHeight,
            angle,
            1, 1, 1, 1);
    
}

void SceneLoading::InitializeComponent()
{
    // UI LoadingIcon
    {
        const char* filename = "Data/Sprite/LoadingIcon.png";
        std::shared_ptr<Actor> actor = ActorSceneLoadManager::Instance().Create();
        actor->SetName("LoadingIcon");
        actor->AddComponent<SpriteControll>();
        actor->GetComponent<SpriteControll>()->LoadSprite(filename);
        actor->AddComponent<TransForm2D>();
        // �ʒu�@�p�x�@�X�P�[�����
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = { 900, 350 };
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

        UiLoadingManager::Instance().Register(actor);
    }

    // UI NowLoading
    {
        const char* filename = "Data/Sprite/NOW LOADING.png";
        std::shared_ptr<Actor> actor = ActorSceneLoadManager::Instance().Create();
        actor->SetName("NowLoading");
        actor->AddComponent<SpriteControll>();
        actor->GetComponent<SpriteControll>()->LoadSprite(filename);
        actor->AddComponent<TransForm2D>();
        // �ʒu�@�p�x�@�X�P�[�����
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = { 600, 500 };
        transform2D->SetPosition(pos);
        // ���̈ʒu
        DirectX::XMFLOAT2 texPos = { 0, 0 };
        transform2D->SetTexPosition(texPos);

        float angle = 0;
        transform2D->SetAngle(angle);
        DirectX::XMFLOAT2 scale = { 300,200 };
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
        UiLoadingManager::Instance().Register(actor);
    }

    // UI GameStart
    {
        const char* filename = "Data/Sprite/START GAME.png";
        std::shared_ptr<Actor> actor = ActorSceneLoadManager::Instance().Create();
        actor->SetName("START GAME");
        actor->AddComponent<SpriteControll>();
        actor->GetComponent<SpriteControll>()->LoadSprite(filename);
        actor->AddComponent<TransForm2D>();
        // �ʒu�@�p�x�@�X�P�[�����
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = { 600, 500 };
        transform2D->SetPosition(pos);
        // ���̈ʒu
        DirectX::XMFLOAT2 texPos = { 0, 0 };
        transform2D->SetTexPosition(texPos);

        float angle = 0;
        transform2D->SetAngle(angle);
        DirectX::XMFLOAT2 scale = { 300,200 };
        transform2D->SetScale(scale);
        // ���̑傫��
        DirectX::XMFLOAT2 texScale = { 0,0 };
        transform2D->SetTexScale(texScale);

        actor->AddComponent<Ui>();
        // �`��`�F�b�N
        std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
        ui->SetDrawCheck(false);

        // ���ꂪ�QD���̊m�F
        bool check2d = true;
        actor->SetCheck2d(check2d);
        UiLoadingManager::Instance().Register(actor);
    }

    // UI Control
    {
        const char* filename = "Data/Sprite/xbx������@.png";
        std::shared_ptr<Actor> actor = ActorSceneLoadManager::Instance().Create();
        actor->SetName("Control");
        actor->AddComponent<SpriteControll>();
        actor->GetComponent<SpriteControll>()->LoadSprite(filename);
        actor->AddComponent<TransForm2D>();
        // �ʒu�@�p�x�@�X�P�[�����
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = { 100, 100 };
        transform2D->SetPosition(pos);
        // ���̈ʒu
        DirectX::XMFLOAT2 texPos = { 0, 0 };
        transform2D->SetTexPosition(texPos);

        float angle = 0;
        transform2D->SetAngle(angle);
        DirectX::XMFLOAT2 scale = { 400,300 };
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
        UiLoadingManager::Instance().Register(actor);
    }

    // UI ControlPC
    {
        const char* filename = "Data/Sprite/�L�[�{�[�h�������.png";
        std::shared_ptr<Actor> actor = ActorSceneLoadManager::Instance().Create();
        actor->SetName("ControlPC");
        actor->AddComponent<SpriteControll>();
        actor->GetComponent<SpriteControll>()->LoadSprite(filename);
        actor->AddComponent<TransForm2D>();
        // �ʒu�@�p�x�@�X�P�[�����
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = { 700, 100 };
        transform2D->SetPosition(pos);
        // ���̈ʒu
        DirectX::XMFLOAT2 texPos = { 0, 0 };
        transform2D->SetTexPosition(texPos);

        float angle = 0;
        transform2D->SetAngle(angle);
        DirectX::XMFLOAT2 scale = { 400,300 };
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
        UiLoadingManager::Instance().Register(actor);
    }

    // UI StartUI
    {
        const char* filename = "Data/Sprite/�X�^�[�g�{�^��.png";
        std::shared_ptr<Actor> actor = ActorSceneLoadManager::Instance().Create();
        actor->SetName("UITitle");
        actor->AddComponent<SpriteControll>();
        actor->GetComponent<SpriteControll>()->LoadSprite(filename);
        actor->AddComponent<TransForm2D>();
        // �ʒu�@�p�x�@�X�P�[�����
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = { 989, 530 };
        transform2D->SetPosition(pos);
        // ���̈ʒu
        DirectX::XMFLOAT2 texPos = { 0, 0 };
        transform2D->SetTexPosition(texPos);

        float angle = 0;
        transform2D->SetAngle(angle);
        DirectX::XMFLOAT2 scale = { 181,104 };
        transform2D->SetScale(scale);
        // ���̑傫��
        DirectX::XMFLOAT2 texScale = { 0,0 };
        transform2D->SetTexScale(texScale);

        actor->AddComponent<Ui>();
        // �`��`�F�b�N
        std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
        ui->SetDrawCheck(false);

        // ���ꂪ�QD���̊m�F
        bool check2d = true;
        actor->SetCheck2d(check2d);

        UiLoadingManager::Instance().Register(actor);
    }

    // UI �{�^��
    {
        const char* filename = "Data/Sprite/�I�� �{�^��.png";
        std::shared_ptr<Actor> actor = ActorSceneLoadManager::Instance().Create();
        actor->SetName("UI Button");
        actor->AddComponent<SpriteControll>();
        actor->GetComponent<SpriteControll>()->LoadSprite(filename);
        actor->AddComponent<TransForm2D>();
        // �ʒu�@�p�x�@�X�P�[�����
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = { 930, 545 };
        transform2D->SetPosition(pos);
        // ���̈ʒu
        DirectX::XMFLOAT2 texPos = { 0, 0 };
        transform2D->SetTexPosition(texPos);

        float angle = 0;
        transform2D->SetAngle(angle);
        DirectX::XMFLOAT2 scale = { 60,64 };
        transform2D->SetScale(scale);
        // ���̑傫��
        DirectX::XMFLOAT2 texScale = { 0,0 };
        transform2D->SetTexScale(texScale);

        actor->AddComponent<Ui>();
        // �`��`�F�b�N
        std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
        ui->SetDrawCheck(false);

        // ���ꂪ�QD���̊m�F
        bool check2d = true;
        actor->SetCheck2d(check2d);

        UiLoadingManager::Instance().Register(actor);
    }
}

void SceneLoading::RotateLoadingIcon(float elapsedTime)
{
    // ��]����
    constexpr float speed = 180;
    angleLoading += speed * elapsedTime;
    // ���ۂ̉�]
    {
        UiLoadingManager::Instance().GetUies(
            (int)UiLoadingManager::UiCountLoading::LodingIcon)->GetComponent<TransForm2D>()->SetAngle(angleLoading);
    }
}

// ���[�f�B���O�X���b�h
void SceneLoading::LoadingThread(SceneLoading* scene)
{
    // COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
    // �T�E���h�A�e�N�X�`�����̏�����COM���g���Ă���Ă���B
    // �X���b�h���Ƃ̏��������Ȃ��Ƃ����Ȃ�
    // �������Ă�
    HRESULT hr = CoInitialize(nullptr);

    // ���̃V�[���̏��������s��
    scene->nextScene->Initialize();

    // �X���b�h���I���O��COM�֘A�̏I����
    // �K�v�Ȃ��Ȃ�I����
    CoUninitialize();

    // ���̃V�[���̏��������ݒ�
    scene->nextScene->SetReady();
}
