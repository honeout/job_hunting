#include <algorithm>
#include <imgui.h>
#include "Math.h"
#include "Movement.h"
#include "Mp.h"

// コンストラクタ
Mp::Mp()
{
    mpEmpth = false;
}

// デストラクタ
Mp::~Mp()
{
}
#ifdef _DEBUG
// GUI描画
void Mp::OnGUI()
{
    ImGui::SliderInt("Mp", &magic, 1, maxMagic);
}
#endif // _DEBUG
bool Mp::UpdateInbincibleTimer(float elapsedTime)
{
    if (invincibleTimer > invincibleTimerMin)
    {
        invincibleTimer -= elapsedTime;
        return false;
    }
    invincibleTimer = imvincibleTimerMax;
    return true;
}

bool Mp::ApplyConsumption(int mpConsumption)
{
    // mp消費が０の場合は変更する必要がない
    if (mpConsumption == mpValueMin) return false;
    std::weak_ptr<Actor> actor = GetActor();
      // ｍｐがない場合は変更しない
    if (magic <= mpMin)
    {
        mpEmpth = true;
        return false;
    }
    // 魔力消費しました
    else
    {
        magic -= mpConsumption;
        return true;
    }
}
// チャージ中
bool Mp::MpCharge(float elapedTime)
{
    if (magic <= mpMin)
    {
        mpEmpth = true;
     
    }
    if (magic >= maxMagic)
    {
        mpEmpth = false;
    }
    if (mpEmpth && UpdateInbincibleTimer(elapedTime))
    {
        magic += mpValue;
        return true;
    }
    return false;
}

bool Mp::OnDead()
{
    ActorManager::Instance().Remove(GetActor());
    return true;
}