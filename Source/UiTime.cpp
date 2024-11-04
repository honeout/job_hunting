#include <imgui.h>
#include "UiTime.h"

UiTime::~UiTime()
{
    if (transForm2D)
        transForm2D.reset();

    if (!sprite)
    {
        delete sprite;
        sprite = nullptr;
    }
}

void UiTime::Start()
{
    // 画像

    //sprite = std::make_unique<Sprite>(GetActor()->GetComponent<SpriteControll>()->GetSprite());

    sprite = GetActor()->GetComponent<SpriteControll>()->GetSprite();

    // 位置等
    transForm2D = GetActor()->GetComponent<TransForm2D>();
}

void UiTime::Update(float elapsedTime)
{
    if (timeMax > 0)
        --countDown;

    if (countDown / 60 <= 0)
        timeUp = true;
}

void UiTime::Render2D(RenderContext& rc, SpriteShader& shader)
{
    // ビューポート 画面のサイズ等
    // ビューポートとは2Dの画面に描画範囲の指定(クリッピング指定も出来る)位置を指定
    //D3D11_VIEWPORT viewport;
    //UINT numViewports = 1;
    ////// ラスタライザーステートにバインドされているビューポート配列を取得
    //rc.deviceContext->RSGetViewports(&numViewports, &viewport);

    // 変換行列
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&rc.view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&rc.projection);
    // ローカルからワールドに行くときにいる奴相手のポジションを渡す。
    //DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();
    float    add = 0;
    // 2D
    if (drawCheck)
    {
        // いくつの数字を描画するか
        int number  = 10;
        // 描画元のXにいくつ数字があるか
        int numberX = 10;
        // 桁ごとに計算するため
        int sc = countDown / 60;
        for ( int i = digit; i > 0; i--)
        {
            int num = sc % number;
            float texPosX = num % numberX * (float)transForm2D->GetTexScale().x;
            //float texPosY = num / 5 * (float)transForm2D->GetTexScale().y;
            // 描画
            sprite->Render(rc.deviceContext,
                transForm2D->GetPosition().x - add,
                transForm2D->GetPosition().y
                , transForm2D->GetScale().x
                , transForm2D->GetScale().y,
                texPosX, transForm2D->GetTexPosition().y,
                transForm2D->GetTexScale().x,
                transForm2D->GetTexScale().y,
                transForm2D->GetAngle(),
                1, 1, 1, 1);
            add += transForm2D->GetScale().x;
            sc /= 10;
        }
    }
}

#ifdef _DEBUG
void UiTime::OnGUI()
{
    ImGui::InputInt("countDown", &countDown);
}
#endif // _DEBUG