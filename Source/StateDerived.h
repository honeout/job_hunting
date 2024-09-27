#pragma once
#include "StateBase.h"
#include "Actor.h"


enum AttackChange
{
	Round1 = 2,
	Round2 = 1,
	ROund3 = 0,
};

// 徘徊ステートオブジェクト
class WanderState : public State
{
public:
	// コンストラクタ
	WanderState(Actor* enemy) :State(enemy) {};
	// デストラクタ
	~WanderState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;


	// 着地瞬間
	bool                upOnLading = false;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;
};

// 待機ステートオブジェクト
class IdleState : public State
{
public:
	// コンストラクタ
	IdleState(Actor* enemy) :State(enemy) {};
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

	// 再生ループ
	bool  loop = true;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;
};

// 追跡ステートオブジェクト
class PursuitState : public State
{
public:
	// コンストラクタ
	PursuitState(Actor* enemy) :State(enemy) {};
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
	float				attackRange = 0;

	int                 attackRound;
	int                 attackCountMax;

	int                 attackType = 0;

	// 再生ループ
	bool  loop = true;

	// 再生開始時間 
	float currentAnimationStartSeconds;

	// アニメーションブレンド
	float blendSeconds = 0.5f;
};

// 攻撃ステートオブジェクト
class AttackState : public State
{
public:
	// コンストラクタ
	AttackState(Actor* enemy) :State(enemy) {};
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

	// 攻撃回数
	int					attackMemory = 0;
	int					attackMemoryMax = 3;
	
	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;

};

// 射撃ステートオブジェクト
class AttackShotState : public State
{
public:
	// コンストラクタ
	AttackShotState(Actor* enemy) :State(enemy) {};
	// デストラクタ
	~AttackShotState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	int                 attackCount = 0;
	int                 attackCountMax = 0;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds;

	// アニメーションブレンド
	float blendSeconds = 0.5f;
};
// 射撃弧ステートオブジェクト
class AttackShotThrowingState : public State
{
public:
	// コンストラクタ
	AttackShotThrowingState(Actor* enemy) :State(enemy) {};
	// デストラクタ
	~AttackShotThrowingState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	int                 attackCount = 0;
	int                 attackCountMax = 0;
	bool				turnPermission = false;

	float               turnSpeed = 20.0f;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSecondsf;

	// アニメーションブレンド
	float blendSeconds = 0.5f;
};

// ダメージステートオブジェクト
class DamageState : public State
{
public:
	// コンストラクタ
	DamageState(Actor* enemy) :State(enemy) {};
	// デストラクタ
	~DamageState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;


	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;
};

// 混乱ステートオブジェクト
class ConfusionState : public State
{
public:
	// コンストラクタ
	ConfusionState(Actor* enemy) :State(enemy) {};
	// デストラクタ
	~ConfusionState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 1.0f;
};





//////////////////////////////
// プレイヤー
//////////////////////////////


// 待機ステートオブジェクト
class PlayerIdleState : public State
{
public:
	// コンストラクタ
	PlayerIdleState(Actor* player) :State(player) {};
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

	// 再生ループ
	bool  loop = true;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 1.0f;
};

// 徘徊ステートオブジェクト
class PlayerMovestate : public State
{
public:
	// コンストラクタ
	PlayerMovestate(Actor* player) :State(player) {};
	// デストラクタ
	~PlayerMovestate() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:

	// 再生ループ
	bool  loop = true;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 1.0f;
};


// ジャンプステートオブジェクト
class PlayerJumpState : public State
{
public:
	// コンストラクタ
	PlayerJumpState(Actor* player) :State(player) {};
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

	bool                jumpCheck = false;
	//bool				loop = false;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 1.0f;
};

// 着地ステートオブジェクト
class PlayerLandState : public State
{
public:
	// コンストラクタ
	PlayerLandState(Actor* player) :State(player) {};
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

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 1.0f;
};

// ジャンプ中ステートオブジェクト
class PlayerJumpFlipState : public State
{
public:
	// コンストラクタ
	PlayerJumpFlipState(Actor* player) :State(player) {};
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

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 1.0f;
};


// 攻撃ステートオブジェクト
class PlayerAttackState : public State
{
public:
	// コンストラクタ
	PlayerAttackState(Actor* player) :State(player) {};
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
	bool				button = false;

	float				turnSpeed = 10;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 1.0f;
};

// よろけステートオブジェクト
class PlayerDamageState : public State
{
public:
	// コンストラクタ
	PlayerDamageState(Actor* player) :State(player) {};
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

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 1.0f;
};

// 回避ステートオブジェクト
class PlayerAvoidanceState : public State
{
public:
	// コンストラクタ
	PlayerAvoidanceState(Actor* player) :State(player) {};
	// デストラクタ
	~PlayerAvoidanceState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	float               moveSpeed = 2.0f;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.3f;

	// アニメーションブレンド
	float blendSeconds = 1.0f;
};

// 反射ステートオブジェクト
class PlayerReflectionState : public State
{
public:
	// コンストラクタ
	PlayerReflectionState(Actor* player) :State(player) {};
	// デストラクタ
	~PlayerReflectionState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 1.0f;
};

// 死亡ステートオブジェクト
class PlayerDeathState : public State
{
public:
	// コンストラクタ
	PlayerDeathState(Actor* player) :State(player) {};
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

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 1.0f;
};

// 復活ステートオブジェクト
class PlayerReviveState : public State
{
public:
	// コンストラクタ
	PlayerReviveState(Actor* player) :State(player) {};
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

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 1.0f;
};