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
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
        return false;
    }

    invincibleTimer = imvincibleTimerMax;
    return true;
}

bool Mp::ApplyConsumption(int mpConsumption)
{
    // ダメージが０の場合は健康状態を変更する必要がない
    if (mpConsumption == 0) return false;

    std::weak_ptr<Actor> actor = GetActor();

      // 死亡している場合は健康状態を変更しない
    if (magic <= 0) 
    {
        mpEmpth = true;
        return false;
    }
    // 魔力消費しました
    else
    {
        // ダメージ処理
        magic -= mpConsumption;
        return true;
    }
}
// チャージ中
bool Mp::MpCharge(float elapedTime)
{


    if (magic <= 0)
    {
        mpEmpth = true;
     
    }
    if (magic >= maxMagic)
    {
        mpEmpth = false;
    }

    if (mpEmpth && UpdateInbincibleTimer(elapedTime))
    {
        magic += 1;
        return true;
    }


    
    return false;
}



bool Mp::Consumption()
{
    //--health;
    return true;
}

bool Mp::OnDead()
{
    ActorManager::Instance().Remove(GetActor());
    return true;
}