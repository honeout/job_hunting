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
    // 次のシーンの名前
    SetSceneName("SceneLoading");

    // コンポーネント指向
    InitializeComponent();

    // 次のシーン
    currentScene = std::make_unique<SceneGame>();

    // 選択範囲最大値
    selectPushMax = (int)Select::KeyBord;

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
        UiLoadingManager::Instance().Clear();
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

    Audio::Instance().AllStop();
    Audio::Instance().AllClear();
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

    // 音楽アップデート
    Audio::Instance().Update();

    // 操作説明選択用
    Select(elapsedTime);

    // 不透明度点滅
    FadeAlphaPulse(elapsedTime);

    //UiLoadingManager::Instance().GetUies(
    //    (int)UiLoadingManager::UiCountLoading::Debug)->GetComponent<SpriteControll>()->GetSprite()
    //    ->UpdateAnimation(elapsedTime);
    

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

        // ロードの描画チェック
        UiLoadingManager::Instance().GetUies(
            (int)UiLoadingManager::UiCountLoading::NowLoading1)->
            GetComponent<Ui>()->SetDrawCheck(false);

        //// ゲームスタートの描画チェック
        //UiLoadingManager::Instance().GetUies(
        //    (int)UiLoadingManager::UiCountLoading::GameStart)->
        //    GetComponent<Ui>()->SetDrawCheck(true);

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
        gamePad.GetButtonDown() & GamePad::BTN_B && 
        isFinalizeGame)
    {
        isFinalizeGame = false;

        AudioParam param;
        // フェードアウト開始
        Audio::Instance().PlayFadeOut(param);
    }

    // 次のシーンへ
    if (isFinalizeGame) return;

    // フェードアウト
    if (!Audio::Instance().AllUpdateFadeOut(elapsedTime)) return;
        SceneManager::Instance().ChangeScene(nextScene);
        nextScene = nullptr;
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
    Graphics& graphics = Graphics::Instance();
    // 画面の比率
    scaleScreen = { graphics.GetScreenWidth() / screenWidth,graphics.GetScreenHeight() / screenHeight };

    // 画面最大値
    screenWidth = Graphics::Instance().GetScreenWidth();
    screenHeight = Graphics::Instance().GetScreenHeight();

    // ボタンの位置差
    buttonPos = { (startCommandPos.x + commandScale.x) - buttonOffset, startCommandPos.y + buttonOffset };

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
        //DirectX::XMFLOAT2 pos = { screenWidth, 700 };

        transform2D->SetPosition(loadingIconPos);
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
        ui->SetDrawCheck(false);

        // これが２Dかの確認
        bool check2d = true;
        actor->SetCheck2d(check2d);

        UiLoadingManager::Instance().Register(actor);
    }

    // UI NowLoading1
    {
        const char* filename = "Data/Sprite/NowLoading.png";
        std::shared_ptr<Actor> actor = ActorSceneLoadManager::Instance().Create();
        actor->SetName("NowLoading1");
        actor->AddComponent<SpriteControll>();
        actor->GetComponent<SpriteControll>()->LoadSprite(filename);
        actor->AddComponent<TransForm2D>();
        // 位置　角度　スケール情報
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        //DirectX::XMFLOAT2 pos = { 600, 700 };

        transform2D->SetPosition(nomLoadingIconPos);
        // 元の位置
        DirectX::XMFLOAT2 texPos = { 0, 0 };
        transform2D->SetTexPosition(texPos);

        float angle = 0;
        transform2D->SetAngle(angle);
        DirectX::XMFLOAT2 scale = { 300,200 };
        transform2D->SetScale(scale);
        // 元の大きさ
        transform2D->SetTexScale(LoadingSelectTexScale);

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
        //DirectX::XMFLOAT2 pos = { 600, 700 };

        transform2D->SetPosition(nowLoadingIconPos);
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
        DirectX::XMFLOAT2 pos = { 400, 500 };

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
        const char* filename = "Data/Sprite/xbx operation method.png";
        std::shared_ptr<Actor> actor = ActorSceneLoadManager::Instance().Create();
        actor->SetName("Control");
        actor->AddComponent<SpriteControll>();
        actor->GetComponent<SpriteControll>()->LoadSprite(filename);
        actor->AddComponent<TransForm2D>();
        // 位置　角度　スケール情報
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        //DirectX::XMFLOAT2 pos = { 100, 100 };

        transform2D->SetPosition(operationPos);
        // 元の位置
        DirectX::XMFLOAT2 texPos = { 0, 0 };
        transform2D->SetTexPosition(texPos);

        float angle = 0;
        transform2D->SetAngle(angle);
        DirectX::XMFLOAT2 scale = { 1200,900 };
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

    // UI ControlKeyBoard
    {
        const char* filename = "Data/Sprite/Keyboard operation explanation.png";
        std::shared_ptr<Actor> actor = ActorSceneLoadManager::Instance().Create();
        actor->SetName("ControlKeyBoard");
        actor->AddComponent<SpriteControll>();
        actor->GetComponent<SpriteControll>()->LoadSprite(filename);
        actor->AddComponent<TransForm2D>();
        // 位置　角度　スケール情報
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        //DirectX::XMFLOAT2 pos = { screenWidth, 100 };

        transform2D->SetPosition(operationPos);
        // 元の位置
        DirectX::XMFLOAT2 texPos = { 0, 0 };
        transform2D->SetTexPosition(texPos);

        float angle = 0;
        transform2D->SetAngle(angle);
        DirectX::XMFLOAT2 scale = { 1200,900 };
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

    // UI SelectNumbar
    {
        const char* filename = "Data\\Font\\fonts\\font4.png";
        std::shared_ptr<Actor> actor = ActorSceneLoadManager::Instance().Create();
        actor->SetName("SelectNumbar");
        actor->AddComponent<SpriteControll>();
        actor->GetComponent<SpriteControll>()->LoadSprite(filename);
        actor->AddComponent<TransForm2D>();
        // 位置　角度　スケール情報
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        //DirectX::XMFLOAT2 pos = { screenWidth, 100 };

        transform2D->SetPosition(selectNumbar);
        // 元の位置
        transform2D->SetTexPosition(selectNumbarTexPosition);

        float angle = 0;
        transform2D->SetAngle(angle);
        DirectX::XMFLOAT2 scale = { 80,80 };
        transform2D->SetScale(scale);
        // 元の大きさ
        transform2D->SetTexScale(selectNumbarTexScale);

        actor->AddComponent<Ui>();
        // 描画チェック
        std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
        ui->SetDrawCheck(true);

        // アルファ値
        ui->SetAlpha(numbarAlpha);

        // これが２Dかの確認
        bool check2d = true;
        actor->SetCheck2d(check2d);
        UiLoadingManager::Instance().Register(actor);
    }

    // UI StartUI
    {
        const char* filename = "Data/Sprite/Start Button.png";
        std::shared_ptr<Actor> actor = ActorSceneLoadManager::Instance().Create();
        actor->SetName("UITitle");
        actor->AddComponent<SpriteControll>();
        actor->GetComponent<SpriteControll>()->LoadSprite(filename);
        actor->AddComponent<TransForm2D>();
        // 位置　角度　スケール情報
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
        //DirectX::XMFLOAT2 pos = { screenWidth - 450, 800 };

        transform2D->SetPosition(startCommandPos);
        // 元の位置
        transform2D->SetTexPosition(commandSelectTexPos);

        float angle = 0;
        transform2D->SetAngle(angle);
        transform2D->SetScale(commandScale);
        // 元の大きさ
        transform2D->SetTexScale(commandSelectTexScale);

        actor->AddComponent<Ui>();
        // 描画チェック
        std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
        ui->SetDrawCheck(false);

        // 非選択状態透明度
        ui->SetAlpha(commandAlphaSelect);

        // これが２Dかの確認
        bool check2d = true;
        actor->SetCheck2d(check2d);

        UiLoadingManager::Instance().Register(actor);
    }

    // UI ボタン
    {
        const char* filename = "Data/Sprite/Select button.png";
        std::shared_ptr<Actor> actor = ActorSceneLoadManager::Instance().Create();
        actor->SetName("UI Button");
        actor->AddComponent<SpriteControll>();
        actor->GetComponent<SpriteControll>()->LoadSprite(filename);
        actor->AddComponent<TransForm2D>();
        // 位置　角度　スケール情報
        std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();

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
        std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
        ui->SetDrawCheck(false);

        // これが２Dかの確認
        bool check2d = true;
        actor->SetCheck2d(check2d);

        UiLoadingManager::Instance().Register(actor);
    }

    stateTime = 0.0f;
    scaleLoad.x = 300;
}

void SceneLoading::RotateLoadingIcon(float elapsedTime)
{
    // 回転する
    constexpr float speed = 180;
    angleLoading += speed * elapsedTime;

    stateTime += elapsedTime;
    // 実際の回転
    {
        UiLoadingManager::Instance().GetUies(
            (int)UiLoadingManager::UiCountLoading::LodingIcon)->GetComponent<TransForm2D>()->SetAngle(angleLoading);
    }

    UiLoadingManager::Instance().GetUies(
        (int)UiLoadingManager::UiCountLoading::NowLoading1)->GetComponent<TransForm2D>()->SetTexScale(LoadingSelectTexScale);
    
    UiLoadingManager::Instance().GetUies(
        (int)UiLoadingManager::UiCountLoading::NowLoading1)->GetComponent<TransForm2D>()->SetScale(scaleLoad);

    // 点1
    if (stateTimeMax.x <= stateTime)
    {
        LoadingSelectTexScale.x = texScaleLoad1;
        scaleLoad.x = 350;
    }
    // 点2
    if (stateTimeMax.y <= stateTime)
    {
        LoadingSelectTexScale.x = texScaleLoad2;
        scaleLoad.x = 400;
    }
    // 点3
    if (stateTimeMax.z <= stateTime)
    {
        LoadingSelectTexScale.x = texScaleLoad3;
        scaleLoad.x = 450;
    }

    // 点0
    if (stateTimeMax.w <= stateTime)
    {
        LoadingSelectTexScale.x = texScaleLoadMin;
        scaleLoad.x = 350;
        stateTime = 0.0f;
    }
    
}
// ページ数変更
void SceneLoading::Select(float elapsedTime)
{
    const GamePadButton anyButton =
        GamePad::BTN_B;
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();

    switch (selectPush)
    {
    // 操作説明ｘボックス
    case (int)Select::Xbox:
    {
        // ページ数
        UiLoadingManager::Instance().GetUies((int)UiLoadingManager::UiCountLoading::SelectNumbar)->
            GetComponent<TransForm2D>()->
            SetTexPosition({ LoadingConfig::selectNumbarOneTexPositionX,selectNumbarTexPosition.y });

        UiLoadingManager::Instance().GetUies((int)UiLoadingManager::UiCountLoading::Control)->
            GetComponent<Ui>()->SetDrawCheck(draw2d);

        UiLoadingManager::Instance().GetUies((int)UiLoadingManager::UiCountLoading::ControlKeyBoard)->
            GetComponent<Ui>()->SetDrawCheck(draw2dEmpty);
        break;
    }
    // 操作説明キーボード
    case (int)Select::KeyBord:
    {
        // ページ数
        UiLoadingManager::Instance().GetUies((int)UiLoadingManager::UiCountLoading::SelectNumbar)->
            GetComponent<TransForm2D>()->
            SetTexPosition({ LoadingConfig::selectNumbarTwoTexPositionX,selectNumbarTexPosition.y });

        UiLoadingManager::Instance().GetUies((int)UiLoadingManager::UiCountLoading::ControlKeyBoard)->
            GetComponent<Ui>()->SetDrawCheck(draw2d);

        UiLoadingManager::Instance().GetUies((int)UiLoadingManager::UiCountLoading::Control)->
            GetComponent<Ui>()->SetDrawCheck(draw2dEmpty);
        break;
    }

    default:
        UiLoadingManager::Instance().GetUies((int)UiLoadingManager::UiCountLoading::Control)->
            GetComponent<Ui>()->SetDrawCheck(draw2dEmpty);

        UiLoadingManager::Instance().GetUies((int)UiLoadingManager::UiCountLoading::ControlKeyBoard)->
            GetComponent<Ui>()->SetDrawCheck(draw2dEmpty);
        break;
    }

    // コマンド操作スティック操作用
    if (gamePad.GetButton() & GamePad::BTN_LEFT || ax < 0)
    {
        stickHoldTimerX += elapsedTime;

        if (gamePad.GetButtonDown() & GamePad::BTN_LEFT || stickHoldTimerX >= stickHoldTime)
        {
            stickHoldTimerX = stickHoldTimerXStart;
            --selectPush;
        }
    }


    if (gamePad.GetButton() & GamePad::BTN_RIGHT || ax > 0)
    {
        stickHoldTimerX += elapsedTime;

        if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT || stickHoldTimerX >= stickHoldTime)
        {
            stickHoldTimerX = stickHoldTimerXStart;
            ++selectPush;
        }
    }
    // 入力が無かったら初期化
    if (ax == 0.0f)
    {
        stickHoldTimerX = stickHoldTimerXStart;
    }

    // 最大値
    if (selectPush > selectPushMax)
    {
        selectPush = selectPushMin;
    }

    // 最小値
    if (selectPush < selectPushMin)
    {
        selectPush = selectPushMax;
    }
}

// 不透明度点滅
void SceneLoading::FadeAlphaPulse(float elapsedTime)
{
    numbarAlpha += numbarAlphaValue * elapsedTime;

    // 数字のアルファ値、最低値
    if (numbarAlpha <= numbarAlphaMin)
        numbarAlphaValue = 0.5f;
    
    // 数字のアルファ値、最大値
    if (numbarAlpha >= numbarAlphaMax)
        numbarAlphaValue = -0.5f;

    // ページ数
    UiLoadingManager::Instance().GetUies((int)UiLoadingManager::UiCountLoading::SelectNumbar)->
        GetComponent<Ui>()->SetAlpha(numbarAlpha);
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
