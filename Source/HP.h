#pragma once

#include <DirectXMath.h>
#include "Component.h"

// 移動
class HP : public Component
{
public:
	HP();
	virtual ~HP();

	// 名前取得
	const char* GetName() const override { return "HP"; }


#ifdef _DEBUG
	// GUI描画
	void OnGUI() override;
#endif // _DEBUG

	// 無敵
	void UpdateInbincibleTimer(float elapsedTime);


	// ダメージを与える ダメージを受けたら他に渡す。
	bool ApplyDamage(int damage, float invincibleTime);


	bool DamageDrawCheck();

	// HP状態を取得
	int GetHealth() const { return health; }

	// HPを突っ込む
	void SetHealth(int health) {
		this->health = health;
	}

	// 最大HPを取り出す
	int GetMaxHealth() const { return maxHealth; }

	// 最大HPを突っ込む
	void SetMaxHealth(int maxHealth)  { this->maxHealth = maxHealth; }

	// ダメージを受けた時に呼ばれる
	bool OnDamaged();
	// 死亡した時に呼ばれる
	bool OnDead();

	bool GetDead() const { return dead; }

	void SetDead(bool dead) { this->dead = dead; }

	// 残機
	void SetLife(int life) { this->life = life; }
	// 残機
	int GetLife() const { return life; }

	bool HealthPinch();

	// 無敵時間中
	bool InvincibleTimerCheck();

	// ダメージ時を取得
	bool FlashTime(float elapsedTime);

	// 無敵時間
	void SetInvincibleTimer(float invincibleTimer) { this->invincibleTimer = invincibleTimer; }


private:
	// 最大値HP
	int          maxHealth = 5;

	// HP
	int          health = maxHealth;

	// 無敵時間
	float   invincibleTimer = 0.0f;

	// 判定
	bool dead = false;

	// 残機
	int life = 0;

	// ダメージ判定
	bool onDamage = false;

	// 経過時間
	float stateTimerMax = 30.0f;
	float stateTimer = stateTimerMax;

	// 反転用
	int blinkingTimeMax = 60;
	int blinkingTime = blinkingTimeMax;


};
