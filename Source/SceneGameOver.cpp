#include "SceneGameOver.h"
#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"
void SceneGameOver::Initialize()
{
    // スプライト初期化
    sprite = new Sprite("Data/Sprite/仮End.png");

    // カメラ初期化
    cameraControlle = nullptr;
    cameraControlle = new CameraController();
}

void SceneGameOver::Finalize()
{
    // スプライト終了化
    if (this->sprite)
    {
        delete sprite;
        sprite = nullptr;
    }

    if (cameraControlle != nullptr)
    {
        delete cameraControlle;
        cameraControlle = nullptr;
    }

}

void SceneGameOver::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    // カメラコントローラー更新処理
    cameraControlle->Update(elapsedTime);

    // 何かボタンを押したらローディングをはさんでゲームシーンへ切り替え
    const GamePadButton anyButton =
        GamePad::BTN_A |
        GamePad::BTN_B |
        GamePad::BTN_X |
        GamePad::BTN_Y;

    if (gamePad.GetButtonUp() & anyButton)// ロードの次ゲームという書き方
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));

}

void SceneGameOver::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // 画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };   // RGBA(0.0~1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // 2Dスプライト描画
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());
        // 描画
        sprite->Render(dc,
            0, 0, screenWidth, screenHeight,
            0, 0, textureWidth, textureHeight,
            0,
            1, 1, 1, 1);
        // {位置}{サイズ}{画像どこから}{画像何処まで}
        // dc , ｛範囲｝｛｝
    }
}
