#include <algorithm>
#include <imgui.h>
#include "Math.h"
#include "Movement.h"
#include "HP.h"

// コンストラクタ
HP::HP()
{

}

// デストラクタ
HP::~HP()
{

}

// GUI描画
void HP::OnGUI()
{
	ImGui::InputInt("Hp", &health);
}

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

    // 死亡している場合は健康状態を変更しない
    if (health <= 0)return false;

    if (this->invincibleTimer >= 0.0f)return false;

    // 何秒無敵
    this->invincibleTimer = invincibleTime;

    // ダメージ処理
    health -= damage;


    // 死亡通知
    if (health <= 0)
    {
        OnDead();
        return true;
    }
    // ダメージ通知
    else
    {
        OnDamaged();
        return true;
    }

    //actor->SetHealth(health);

    // 健康状態が変更した場合はtrueを返す
    return false;
}



bool HP::OnDamaged()
{
    --health;
    return true;
}

bool HP::OnDead()
{
    ActorManager::Instance().Remove(GetActor());
    return true;
}
