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

	// GUI描画
	void OnGUI() override;

	// 無敵
	void UpdateInbincibleTimer(float elapsedTime);


	// ダメージを与える ダメージを受けたら他に渡す。
	bool ApplyDamage(int damage, float invincibleTime);

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
private:
	// 最大値HP
	int          maxHealth = 5;

	// HP
	int          health = maxHealth;

	// 無敵時間
	float   invincibleTimer = 1.0f;
};
