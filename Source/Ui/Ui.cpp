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
}

void Ui::Render2D(RenderContext& rc, SpriteShader& shader)
{
    Graphics& graphics = Graphics::Instance();
    // スケール計算用
    float uiScale = graphics.CalcUIScale();

    // 座標用
    float uiScaleX = graphics.CalcUIScaleX();
    float uiScaleY = graphics.CalcUIScaleY();

    // サイズ
    DirectX::XMFLOAT2 scale =
    {
        transForm2D->GetScale().x * uiScale,
        transForm2D->GetScale().y * uiScale
    };

    // 座標
    DirectX::XMFLOAT2 pos =
    {
        transForm2D->GetPosition().x * uiScaleX,
        transForm2D->GetPosition().y * uiScaleY
    };

    // 2D
    if (drawCheck)
    { 
        		// 描画
        sprite->Render(rc.deviceContext,
            pos.x,
            pos.y,
			scale.x,
            scale.y,
            transForm2D->GetTexPosition().x, transForm2D->GetTexPosition().y,
            transForm2D->GetTexScale().x > 0 ? transForm2D->GetTexScale().x : static_cast<float> (sprite->GetTextureWidth()),
            transForm2D->GetTexScale().y > 0 ? transForm2D->GetTexScale().y : static_cast<float> (sprite->GetTextureHeight()),
            transForm2D->GetAngle(),
            color.x,color.y,color.z,color.w);
    }
}

#ifdef _DEBUG
void Ui::OnGUI()
{
    ImGui::SliderFloat4("Color", &color.x, 0, 1);
}
#endif // _DEBUG
void Ui::UiTimeUpdate()
{
    if (timeMax > 0)
    --countDown;
}

void Ui::IncrementToAlpha(float increment)
{
    if (color.w - FLT_EPSILON <= alphaMax + FLT_EPSILON)
        color.w += increment;
}

