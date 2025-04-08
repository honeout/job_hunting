#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "EnemyBoss.h"
#include "StageMain.h"
#include "SceneLoading.h"
#include "Camera.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "SceneGame.h"

// 初期化
void SceneLoading::Initialize()
{
    // コンポーネント指向
    InitializeComponent();

    currentScene = std::make_unique<SceneGame>();

    // ゲームをロード中に消すか
    isFinalizeGame = true;

    //// スプライト初期化
    //spriteLoading = std::make_unique<Sprite>("Data/Sprite/LoadingIcon.png");
    //spriteControlManual = std::make_unique<Sprite>("Data/Sprite/xbx操作方法.png");
    //// スレッド開始      (関数名、引数の中身 自分自信)スレッド立ち上げる
    // 引数を何個でも入れられる。関数の引数の長さによる
    this->thread = new std::thread(LoadingThread,this);
}

void SceneLoading::Start()
{
}

// 終了化
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
    // スレッド終了化
    if (this->thread)
    {
        // 処理している最中にデリートするとまずいだからまつ
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
// 更新処理
void SceneLoading::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    // 回転する
    RotateLoadingIcon(elapsedTime);

    ActorSceneLoadManager::Instance().Update(elapsedTime);

    // ゲームスタート準備完了UI表示
    if (nextScene->IsReady())
    {
        // ロードの描画チェック
        UiLoadingManager::Instance().GetUies(
            (int)UiLoadingManager::UiCountLoading::LodingIcon)->
            GetComponent<Ui>()->SetDrawCheck(false);

        // ロードの描画チェック
        UiLoadingManager::Instance().GetUies(
            (int)UiLoadingManager::UiCountLoading::NowLoading)->
            GetComponent<Ui>()->SetDrawCheck(false);

        // ゲームスタートの描画チェック
        UiLoadingManager::Instance().GetUies(
            (int)UiLoadingManager::UiCountLoading::GameStart)->
            GetComponent<Ui>()->SetDrawCheck(true);

        // ゲームスタートのスタート描画チェック
        UiLoadingManager::Instance().GetUies(
            (int)UiLoadingManager::UiCountLoading::Start)->
            GetComponent<Ui>()->SetDrawCheck(true);


        // ゲームスタートのボタン描画チェック
        UiLoadingManager::Instance().GetUies(
            (int)UiLoadingManager::UiCountLoading::Button)->
            GetComponent<Ui>()->SetDrawCheck(true);
    }

    // 次のシーンの準備が完了したらシーンを切り替える
    if (nextScene->IsReady() && 
        gamePad.GetButtonDown() & GamePad::BTN_B)
    {
        isFinalizeGame = false;
        SceneManager::Instance().ChangeScene(nextScene);
        nextScene = nullptr;
    }
}
// 描画処理
void SceneLoading::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // GUI
    ActorSceneLoadManager::Instance().RenderGui();

    // 画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // RGBA(0.0～1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    //// ２Dスプライト描画
    //{
    //    // 画面右下にローディングアイコンを描画
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

    //// 画面クリア＆レンダーターゲット設定
    //FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // RGBA(0.0～1.0)
    //dc->ClearRenderTargetView(rtv, color);
    //dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    //dc->OMSetRenderTargets(1, &rtv, dsv);

    //// ２Dスプライト描画
    //{
    //    // 画面右下にローディングアイコンを描画
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
        // 位置　角度　スケール情報
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = { 900, 350 };
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
        // 位置　角度　スケール情報
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = { 600, 500 };
        transform2D->SetPosition(pos);
        // 元の位置
        DirectX::XMFLOAT2 texPos = { 0, 0 };
        transform2D->SetTexPosition(texPos);

        float angle = 0;
        transform2D->SetAngle(angle);
        DirectX::XMFLOAT2 scale = { 300,200 };
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
        // 位置　角度　スケール情報
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = { 600, 500 };
        transform2D->SetPosition(pos);
        // 元の位置
        DirectX::XMFLOAT2 texPos = { 0, 0 };
        transform2D->SetTexPosition(texPos);

        float angle = 0;
        transform2D->SetAngle(angle);
        DirectX::XMFLOAT2 scale = { 300,200 };
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
        UiLoadingManager::Instance().Register(actor);
    }

    // UI Control
    {
        const char* filename = "Data/Sprite/xbx操作方法.png";
        std::shared_ptr<Actor> actor = ActorSceneLoadManager::Instance().Create();
        actor->SetName("Control");
        actor->AddComponent<SpriteControll>();
        actor->GetComponent<SpriteControll>()->LoadSprite(filename);
        actor->AddComponent<TransForm2D>();
        // 位置　角度　スケール情報
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = { 100, 100 };
        transform2D->SetPosition(pos);
        // 元の位置
        DirectX::XMFLOAT2 texPos = { 0, 0 };
        transform2D->SetTexPosition(texPos);

        float angle = 0;
        transform2D->SetAngle(angle);
        DirectX::XMFLOAT2 scale = { 400,300 };
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
        UiLoadingManager::Instance().Register(actor);
    }

    // UI ControlPC
    {
        const char* filename = "Data/Sprite/キーボード操作説明.png";
        std::shared_ptr<Actor> actor = ActorSceneLoadManager::Instance().Create();
        actor->SetName("ControlPC");
        actor->AddComponent<SpriteControll>();
        actor->GetComponent<SpriteControll>()->LoadSprite(filename);
        actor->AddComponent<TransForm2D>();
        // 位置　角度　スケール情報
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = { 700, 100 };
        transform2D->SetPosition(pos);
        // 元の位置
        DirectX::XMFLOAT2 texPos = { 0, 0 };
        transform2D->SetTexPosition(texPos);

        float angle = 0;
        transform2D->SetAngle(angle);
        DirectX::XMFLOAT2 scale = { 400,300 };
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
        UiLoadingManager::Instance().Register(actor);
    }

    // UI StartUI
    {
        const char* filename = "Data/Sprite/スタートボタン.png";
        std::shared_ptr<Actor> actor = ActorSceneLoadManager::Instance().Create();
        actor->SetName("UITitle");
        actor->AddComponent<SpriteControll>();
        actor->GetComponent<SpriteControll>()->LoadSprite(filename);
        actor->AddComponent<TransForm2D>();
        // 位置　角度　スケール情報
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = { 989, 530 };
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

        UiLoadingManager::Instance().Register(actor);
    }

    // UI ボタン
    {
        const char* filename = "Data/Sprite/選択 ボタン.png";
        std::shared_ptr<Actor> actor = ActorSceneLoadManager::Instance().Create();
        actor->SetName("UI Button");
        actor->AddComponent<SpriteControll>();
        actor->GetComponent<SpriteControll>()->LoadSprite(filename);
        actor->AddComponent<TransForm2D>();
        // 位置　角度　スケール情報
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = { 930, 545 };
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
        ui->SetDrawCheck(false);

        // これが２Dかの確認
        bool check2d = true;
        actor->SetCheck2d(check2d);

        UiLoadingManager::Instance().Register(actor);
    }
}

void SceneLoading::RotateLoadingIcon(float elapsedTime)
{
    // 回転する
    constexpr float speed = 180;
    angleLoading += speed * elapsedTime;
    // 実際の回転
    {
        UiLoadingManager::Instance().GetUies(
            (int)UiLoadingManager::UiCountLoading::LodingIcon)->GetComponent<TransForm2D>()->SetAngle(angleLoading);
    }
}

// ローディングスレッド
void SceneLoading::LoadingThread(SceneLoading* scene)
{
    // COM関連の初期化でスレッド毎に呼ぶ必要がある
    // サウンド、テクスチャ等の処理はCOMを使ってやっている。
    // スレッドごとの初期化しないといけない
    // 毎回やってる
    HRESULT hr = CoInitialize(nullptr);

    // 次のシーンの初期化を行う
    scene->nextScene->Initialize();

    // スレッドが終わる前にCOM関連の終了化
    // 必要ないなら終了化
    CoUninitialize();

    // 次のシーンの準備完了設定
    scene->nextScene->SetReady();
}
