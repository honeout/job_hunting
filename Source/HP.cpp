#include <algorithm>
#include <imgui.h>
#include "Math.h"
#include "Movement.h"
#include "HP.h"

// コンストラクタ
HP::HP()
{
    dead = false;
}

// デストラクタ
HP::~HP()
{

}
#ifdef _DEBUG
// GUI描画
void HP::OnGUI()
{
	ImGui::SliderInt("Hp", &health,1,maxHealth);
}
#endif // _DEBUG
void HP::UpdateInbincibleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}

bool HP::ApplyDamage(int damage, float invincibleTime)
{
    // ダメージが０の場合は健康状態を変更する必要がない
    if (damage == 0) return false;

    std::shared_ptr<Actor> actor = GetActor();
  /*  int health = actor->GetHealth();*/

    onDamage = false;
    // 死亡している場合は健康状態を変更しない
    if (health <= 0)return false;
    if (invincibleTimer > 0.0f)return false;

    // 何秒無敵
    invincibleTimer = invincibleTime;

    // ダメージ処理
    health -= damage;



    // ライフ最大値から一つ減る。
    if (health <= 0)
    {
        --life;
    }
    // 死亡通知
    if (life <= -1)
    {
        //OnDead();
        dead = true;
        return true;
    }
    // ダメージ通知
    else
    {
        onDamage = true;
        return true;
    }

    //actor->SetHealth(health);
    // 健康状態が変更した場合はtrueを返す
    return false;
}

bool HP::DamageDrawCheck()
{
    --stateTimer;

    if (stateTimer < 0.0f)
    {
        stateTimer = stateTimerMax;
        return false;
    }

    if (stateTimer >= 0.0f)
        return true;
}
bool HP::OnDamaged()
{
    //--health;
    return true;
}

bool HP::OnDead()
{
    ActorManager::Instance().Remove(GetActor());
    return true;
}

bool HP::HealthPinch()
{
    if (health <= (maxHealth / 4))
        return true;
    
    return false;
}

bool HP::InvincibleTimerCheck()
{
    if (invincibleTimer <= 0)
        return false;

    if (invincibleTimer > 0)
        return true;
}

bool HP::OnHit(float elapsedTime)
{

    if (onDamage)
    {
        blinkingTime = 0;
    }

    if (blinkingTime < blinkingTimeMax)
    {
        ++blinkingTime;

        return true;
    }



    return false;
}
