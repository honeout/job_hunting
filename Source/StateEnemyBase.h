#pragma once

#pragma once
#include <vector>
#include <memory>
#include "StateBase.h"
#include "Actor.h"
#include "EnemyBoss.h"

class EnemyState : State
{
public:
	// コンストラクタ
	EnemyState(std::weak_ptr<Actor> actor) :State(owner) {}
	virtual ~EnemyState() {}
	// 全て継承先で実装させる必要があるため純粋仮想関数で実装
	// ステートに入った時のメソッド
	virtual void Enter() = 0;
	// ステートで実行するメソッド
	virtual void Execute(float elapsedTime) = 0;
	// ステートから出ていくときのメソッド
	virtual void Exit() = 0;

protected:

	// アクターゲット
	std::shared_ptr<Actor> GetActor() {
		return owner.lock();
	}

	std::shared_ptr<EnemyBoss> GetEnemyId()
	{
		auto ptr = GetActor();
		return ptr ? ptr->GetComponent<EnemyBoss>() : nullptr;
	}

protected:
	std::weak_ptr<Actor> owner;
};
