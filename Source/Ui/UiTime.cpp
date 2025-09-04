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
    sprite = GetActor()->GetComponent<SpriteControll>()->GetSprite();

    // 位置等
    transForm2D = GetActor()->GetComponent<TransForm2D>();
}

void UiTime::Update(float elapsedTime)
{
    // 経過時間更新
    if (timeMax > 0)
    {
        // 時間経過
        coolDownLimit += coolDownSpeed * elapsedTime;
        
    }

    // 表示用時間を変更
    if (coolDownLimit >= coolDownSpeedMax)
    {
        coolDownLimit = coolDownLimitMin;
        --countDown;
    }

    if (countDown / 60 <= 0)
        timeUp = true;
}

void UiTime::Render2D(RenderContext& rc, SpriteShader& shader)
{
    //// 変換行列
    //DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&rc.view);
    //DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&rc.projection);

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
        transForm2D->GetPosition().x* uiScaleX,
        transForm2D->GetPosition().y* uiScaleY
    };


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
            // 描画
            sprite->Render(rc.deviceContext,
                pos.x - add,
                pos.y
                , scale.x
                , scale.y,
                texPosX, transForm2D->GetTexPosition().y,
                transForm2D->GetTexScale().x,
                transForm2D->GetTexScale().y,
                transForm2D->GetAngle(),
                1, 1, 1, 1);
            add += scale.x;
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