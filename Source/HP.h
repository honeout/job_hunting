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


	// ダメージを与える ダメージを受けたら他に渡す。
	bool ApplyDamage(int damage, float invincibleTime);

	//// HP状態を取得
	//int GetHealth() const { return health; }

	// ダメージを受けた時に呼ばれる
	bool OnDamaged();
	// 死亡した時に呼ばれる
	bool OnDead();
private:
	// HP
	int          health = 5;

	// 最大値HP
	//int          maxHealth = 5;
	// 無敵時間
	float   invincibleTimer = 1.0f;
};
