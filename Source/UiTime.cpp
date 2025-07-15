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
    // ‰æ‘œ
    sprite = GetActor()->GetComponent<SpriteControll>()->GetSprite();

    // ˆÊ’u“™
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
    // •ÏŠ·s—ñ
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&rc.view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&rc.projection);

    float    add = 0;
    // 2D
    if (drawCheck)
    {
        // ‚¢‚­‚Â‚Ì”Žš‚ð•`‰æ‚·‚é‚©
        int number  = 10;
        // •`‰æŒ³‚ÌX‚É‚¢‚­‚Â”Žš‚ª‚ ‚é‚©
        int numberX = 10;
        // Œ…‚²‚Æ‚ÉŒvŽZ‚·‚é‚½‚ß
        int sc = countDown / 60;
        for ( int i = digit; i > 0; i--)
        {
            int num = sc % number;
            float texPosX = num % numberX * (float)transForm2D->GetTexScale().x;
            // •`‰æ
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