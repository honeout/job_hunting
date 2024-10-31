#include <imgui.h>
#include "Ui.h"

Ui::~Ui()
{
    if (transForm2D)
        transForm2D.reset();


}

void Ui::Start()
{
    // 画像
    sprite = GetActor()->GetComponent<SpriteControll>()->GetSprite();
    // 位置等
    transForm2D = GetActor()->GetComponent<TransForm2D>();
}

void Ui::Update(float elapsedTime)
{
    UiTimeUpdate();
}

void Ui::Render2D(RenderContext& rc, SpriteShader& shader)
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

    // 2D
    if (drawCheck)
    {
        		// 描画
        sprite->Render(rc.deviceContext,
            transForm2D->GetPosition().x,
            transForm2D->GetPosition().y
			, transForm2D->GetScale().x
			, transForm2D->GetScale().y,
            transForm2D->GetTexPosition().x, transForm2D->GetTexPosition().y,
            transForm2D->GetTexScale().x > 0 ?  transForm2D->GetTexScale().x : static_cast<float> (sprite->GetTextureWidth()),
            transForm2D->GetTexScale().y > 0 ?  transForm2D->GetTexScale().y : static_cast<float> (sprite->GetTextureHeight()),
            transForm2D->GetAngle(),
            color.x,color.y,color.z,color.w);
    }
}

void Ui::OnGUI()
{

    ImGui::SliderFloat4("Color", &color.x, 0, 1);
}

void Ui::UiTimeUpdate()
{
    if (timeMax > 0)
    --countDown;
}
