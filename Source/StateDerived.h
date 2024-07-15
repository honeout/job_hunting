#pragma once
#include "StateBase.h"


// 徘徊ステートオブジェクト
class WanderState : public State
{
public:
	// コンストラクタ
	WanderState(EnemySlime* enemy) :State(enemy) {};
	// デストラクタ
	~WanderState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};

// 待機ステートオブジェクト
class IdleState : public State
{
public:
	// コンストラクタ
	IdleState(EnemySlime* enemy) :State(enemy) {};
	// デストラクタ
	~IdleState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;
};

// 追跡ステートオブジェクト
class PursuitState : public State
{
public:
	// コンストラクタ
	PursuitState(EnemySlime* enemy) :State(enemy) {};
	// デストラクタ
	~PursuitState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;
	float				attackRange = 1.5f;
};

// 攻撃ステートオブジェクト
class AttackState : public State
{
public:
	// コンストラクタ
	AttackState(EnemySlime* enemy) :State(enemy) {};
	// デストラクタ
	~AttackState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;
};





//////////////////////////////
// プレイヤー
//////////////////////////////


// 待機ステートオブジェクト
class PlayerIdleState : public StatePlayer
{
public:
	// コンストラクタ
	PlayerIdleState(Player* player) :StatePlayer(player) {};
	// デストラクタ
	~PlayerIdleState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;
};

// 徘徊ステートオブジェクト
class PlayerMovestate : public StatePlayer
{
public:
	// コンストラクタ
	PlayerMovestate(Player* player) :StatePlayer(player) {};
	// デストラクタ
	~PlayerMovestate() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
};


// ジャンプステートオブジェクト
class PlayerJumpState : public StatePlayer
{
public:
	// コンストラクタ
	PlayerJumpState(Player* player) :StatePlayer(player) {};
	// デストラクタ
	~PlayerJumpState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;
	float				attackRange = 1.5f;
};

// 着地ステートオブジェクト
class PlayerLandState : public StatePlayer
{
public:
	// コンストラクタ
	PlayerLandState(Player* player) :StatePlayer(player) {};
	// デストラクタ
	~PlayerLandState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;
};

// ジャンプ中ステートオブジェクト
class PlayerJumpFlipState : public StatePlayer
{
public:
	// コンストラクタ
	PlayerJumpFlipState(Player* player) :StatePlayer(player) {};
	// デストラクタ
	~PlayerJumpFlipState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;
};


// 攻撃ステートオブジェクト
class PlayerAttackState : public StatePlayer
{
public:
	// コンストラクタ
	PlayerAttackState(Player* player) :StatePlayer(player) {};
	// デストラクタ
	~PlayerAttackState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;
};

// よろけステートオブジェクト
class PlayerDamageState : public StatePlayer
{
public:
	// コンストラクタ
	PlayerDamageState(Player* player) :StatePlayer(player) {};
	// デストラクタ
	~PlayerDamageState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;
};

// 死亡ステートオブジェクト
class PlayerDeathState : public StatePlayer
{
public:
	// コンストラクタ
	PlayerDeathState(Player* player) :StatePlayer(player) {};
	// デストラクタ
	~PlayerDeathState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;
};

// 復活ステートオブジェクト
class PlayerReviveState : public StatePlayer
{
public:
	// コンストラクタ
	PlayerReviveState(Player* player) :StatePlayer(player) {};
	// デストラクタ
	~PlayerReviveState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;
};