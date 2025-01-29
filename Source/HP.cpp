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
	ImGui::SliderInt("bonusHp", &bonusHp,0,maxHealth);
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
    // ヒットしたかどうか
    onDamage = false;
    // ダメージが０の場合は健康状態を変更する必要がない
    if (damage == 0) return false;

    std::weak_ptr<Actor> actor = GetActor();
   // std::shared_ptr<Actor> actor = GetActor();
  /*  int health = actor->GetHealth();*/


    // 死亡している場合は健康状態を変更しない
    if (health <= 0)return false;
    if (invincibleTimer > 0.0f)return false;
    // 何秒無敵
    invincibleTimer = invincibleTime;
    // 通常被ダメ
    if(!isBonusHpActive)
    // ダメージ処理
    health -= damage;
    // 耐久力ダメ
    else
    {
        bonusHp -= damage;
    }
    // 一定ダメージ数
    escapeOnFixedDamage += damage;


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
    // 追加HP
    if (bonusHp <= bonusHpEnd)
    {
        isBonusHpActive = false;
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

// 耐久追加
void HP::SetIsBonusHpActive(bool isBonusHpActive)
{
    this->isBonusHpActive = isBonusHpActive;

    if (this->isBonusHpActive)
    {
        // 追加HP
        bonusHp = maxHealth;
    }
    else
    {
        bonusHp = bonusHpEnd;
    }
}
bool HP::OnDamaged()
{
    //--health;
    return onDamage;
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

bool HP::FlashTime(float elapsedTime)
{

    if (onDamage)
    {
        blinkingTime = 0;
    }
    onDamage = false;
    // チェンジ時間
    if (blinkingTime < blinkingTimeMax)
    {
        ++blinkingTime;


        return true;
    }





    return false;
}
// 被ダメ一定数
bool HP::CheckDamageThresholdWithinTime(float elapsedTime,  float damageThreshold, float timeLimit)
{
    damageThresholdTime += elapsedTime;

    // 最大値
    escapeOnFixedDamageMax = damageThreshold;

    // 経過時間一定時間越え
    if (damageThresholdTime > timeLimit)
    {
        damageThresholdTime = damageThresholdStartTime;
        return false;
    }

    // 条件達成
    if (escapeOnFixedDamage > escapeOnFixedDamageMax)
    {
        // ダメージ初期化
        escapeOnFixedDamage = escapeOnFixedDamageStart;

        // 時間初期化
        damageThresholdTime = damageThresholdStartTime;

        // ダメージが一定以上確認
        isOverDamageRule = true;
        return true;
    }
    return false;
}
// 被ダメ解放
void HP::ResetOnDamageThresholdTime()
{
    // 被ダメチャージ初期化
    escapeOnFixedDamage = escapeOnFixedDamageStart;

    // 経過時間強制終了
    damageThresholdTime = damageThresholdStartTime;

}
