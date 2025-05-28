#include <imgui.h>
#include "Ui.h"

Ui::~Ui()
{
    if (transForm2D)
        transForm2D.reset();
}

void Ui::Start()
{
    // ‰æ‘œ
    sprite = GetActor()->GetComponent<SpriteControll>()->GetSprite();
    // ˆÊ’u“™
    transForm2D = GetActor()->GetComponent<TransForm2D>();
}

void Ui::Update(float elapsedTime)
{
}

void Ui::Render2D(RenderContext& rc, SpriteShader& shader)
{
    // •ÏŠ·s—ñ
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&rc.view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&rc.projection);
    // 2D
    if (drawCheck)
    {
        		// •`‰æ
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

