#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"


// 初期化
void SceneTitle::Initialize()
{
    // スプライト初期化

    //sprite = std::make_unique<Sprite>("Data/Sprite/Title.png");
    sprite = std::make_unique<Sprite>("Data/Sprite/タイトル.png");
    spritePush = std::make_unique<Sprite>("Data/Sprite/コマンドPUSH.png");

}
// 終了化
void SceneTitle::Finalize()
{

}
// 更新処理
void SceneTitle::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    // 何かボタンを押したらローディングをはさんでゲームシーンへ切り替え
    const GamePadButton anyButton =
        GamePad::BTN_A|
    GamePad::BTN_B|
    GamePad::BTN_X|
    GamePad::BTN_Y;

    if (gamePad.GetButtonUp() & anyButton)// ロードの次ゲームという書き方
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));

}
// 描画処理
void SceneTitle::Render()
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

        float texturePushWidth = static_cast<float>(spritePush->GetTextureWidth());
        float texturePushHeight = static_cast<float>(spritePush->GetTextureHeight());
        // 描画
        sprite->Render(dc,
            0, 0, screenWidth, screenHeight,
            0, 0, textureWidth, textureHeight,
            0,
            1, 1, 1, 1);
        // {位置}{サイズ}{画像どこから}{画像何処まで}
        // dc , ｛範囲｝｛｝

        // Push
        spritePush->Render(dc,
            position.x, position.y, texturePushWidth, texturePushHeight,
            0, 0, texturePushWidth, texturePushHeight,
            0,
            1, 1, 1, 1);
        // {位置}{サイズ}{画像どこ
    }
}

