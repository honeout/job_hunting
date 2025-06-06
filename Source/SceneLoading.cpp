#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "SceneLoading.h"
#include "SceneManager.h"

// 初期化
void SceneLoading::Initialize()
{
    // スプライト初期化
    sprite = new Sprite("Data/Sprite/LoadingIcon.png");

    // スレッド開始      (関数名、引数の中身 自分自信)スレッド立ち上げる
    // 引数を何個でも入れられる。関数の引数の長さによる
    this->thread = new std::thread(LoadingThread,this);
}

// 終了化
void SceneLoading::Finalize()
{
    // スレッド終了化
    if (this->thread)
    {
        // 処理している最中にデリートするとまずいだからまつ
        thread->join();
        delete thread;
        //delete thread;
        thread = nullptr;
    }
    
    // スプライト終了化
    if (this->sprite)
    {
        delete sprite;
        sprite = nullptr;
    }
}
// 更新処理
void SceneLoading::Update(float elapsedTime)
{
    // 回転する
    constexpr float speed = 180;
    angle += speed * elapsedTime;

        // 次のシーンの準備が完了したらシーンを切り替える
    if (nextScene->IsReady())
    {
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

    // 画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f }; // RGBA(0.0〜1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // ２Dスプライト描画
    {
        // 画面右下にローディングアイコンを描画
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());
        float positionX = screenWidth - textureWidth;
        float positionY = screenHeight - textureHeight;

        sprite->Render(dc,
            positionX, positionY, textureWidth, textureHeight,
            0, 0, textureWidth, textureHeight,
            angle,
            1, 1, 1, 1);
    }
}


// ローディングスレッド
void SceneLoading::LoadingThread(SceneLoading* scene)
{
    // COM関連の初期化でスレッド毎に呼ぶ必要がある
    // サウンド、テクスチャ等の処理はCOMを使ってやっている。
    // スレッドごとの初期化しないといけない
    // 毎回やってる
    CoInitialize(nullptr);

    // 次のシーンの初期化を行う
    //scene->Initialize();
    scene->nextScene->Initialize();


    // スレッドが終わる前にCOM関連の終了化
    // 必要ないなら終了化
    CoUninitialize();

    // 次のシーンの準備完了設定
    scene->nextScene->SetReady();
}
