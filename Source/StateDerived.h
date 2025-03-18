#pragma once
#include "StateBase.h"
#include "Input\GamePad.h"
#include "Input/Input.h"
#include "SceneGameClear.h"
#include "Actor.h"
#include "Effect.h"
#include "ProjectileTornade.h"

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
	WanderState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// デストラクタ
	~WanderState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	// 覚醒時に攻撃回数を変える。
	int                 attackCountMax = 2;
	int                 attackCountMin = 0;
	int                 attackCount = attackCountMin;
	int                 applyDamage = 8;
	// エフェクトスケール
	float               scaleEffect = 0.3f;
	// エフェクト
	std::unique_ptr<Effect> smorker;
	bool loopSe = true;
	// 着地瞬間
	bool                upOnLading = false;
	// 歩き速度
	float moveSpeed = 3;
	// 回転速度
	float turnSpeed = DirectX::XMConvertToRadians(320);
	// ターゲット位置
	DirectX::XMFLOAT3 targetPosition = {.0f,.0f,.0f};
	RadialBlurData radialBlurData;
	// アニメーションルール
	Model::ModelAnim modelAnim;
};

// 待機ステートオブジェクト
class IdleState : public State
{
public:
	// コンストラクタ
	IdleState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// デストラクタ
	~IdleState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;

private:
	float               stateTimerEnd = 0.0f;
	float				stateTimer = 0.0f;
	float				stateTimerMax = 2.5f;
	float				stateTimerMin = 1.0f;
	// 行動種類
	int  randRoot = 0;
	// アニメーションルール
	Model::ModelAnim modelAnim;
};

// 追跡ステートオブジェクト
class PursuitState : public State
{
public:
	// コンストラクタ
	PursuitState(std::weak_ptr<Actor> enemy) :State(enemy) {};
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
	float				stateTimerMin = 0.0f;
	float				attackRange = 0.0f;
	int                 attackRound = 0;
	int                 attackCountMax = 0;
	int                 attackType = 0;
	// アニメーションルール
	Model::ModelAnim modelAnim;
	// 行動制限最大数
	int mortionLimitMax = 5;
	// 数回行動したら疲れるモーションを入れる。
	int mortionLimit = mortionLimitMax;
	// 行動をランダムで
	int randamAttack = 0;
};


// ジャンプ系ステートオブジェクト
class JumpState : public State
{
public:
	// コンストラクタ
	JumpState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// デストラクタ
	~JumpState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;

private:
	// 与ダメ
	int                 applyDamageJumpStart = 5;
	int                 applyDamageJumpStamp = 8;

	// エフェクトスケール
	float               scaleEffect = 0.3f;

	// エフェクト
	std::unique_ptr<Effect> smorker;

	// ループするかSe
	bool loopSe = false;

	// 着地瞬間
	bool                upOnLading = false;

	// アニメーションルール
	Model::ModelAnim modelAnim;

	// 歩き速度
	float moveSpeed = 10;

	// ジャンプ速度
	float jumpSpeed = 30.0f;

	// 回転速度
	float turnSpeed = DirectX::XMConvertToRadians(320);

	// ターゲット位置
	DirectX::XMFLOAT3 targetPosition = { .0f,.0f,.0f };

	// アニメーション終了
	bool jumpStart = false;

	RadialBlurData radialBlurData;
};


// 攻撃ステートオブジェクト
class AttackState : public State
{
public:
	// コンストラクタ
	AttackState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// デストラクタ
	~AttackState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;

private:
	DirectX::XMFLOAT3 angle;

	// チャージ時間
	float				stateChargeTimer = 0.0f;
	float				stateChargeTimerEnd = 0.0f;
	float				stateChargeTimerMax = 1.0f;

	// チャージ完了後のダッシュ時間
	float				stateChargeCompletionTimer = 0.0f;
	float				stateChargeCompletionTimerEnd = 0.0f;
	float				stateChargeCompletionTimerMax = 1.0f;

	// エフェクトスケール
	float               scaleEffect = 0.3f;

	// 与ダメ
	int                 applyDamageAttack = 8;

	// 攻撃回数
	int					attackMemory = 0;
	int					attackMemoryMax = 3;

	// エフェクト
	std::unique_ptr<Effect> smorker;
	std::unique_ptr<Effect> charge;
	std::unique_ptr<Effect> chargeCompleate;
	
	// アニメーションルール
	Model::ModelAnim modelAnim;

	// 歩き速度
	float moveSpeed = 10;

	// ターゲット位置
	DirectX::XMFLOAT3 targetPosition = { .0f,.0f,.0f };

	// 回転速度
	float turnSpeed = DirectX::XMConvertToRadians(320);

	// チャージ初期化
	bool chargeInitilize = false;

	// アニメーション終了
	bool dushStart = false;

	DirectX::XMFLOAT3 direction;

	DirectX::XMFLOAT3 directionP;
};

// ダメージステートオブジェクト
class DamageState : public State
{
public:
	// コンストラクタ
	DamageState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// デストラクタ
	~DamageState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	bool loopSe = false;
	float				stateTimer = 0.0f;
	float				stateTimerMax = 5.0f;
	float				stateTimerEnd = 0.0f;
	// アニメーションルール
	Model::ModelAnim modelAnim;
};

// 混乱ステートオブジェクト
class ConfusionState : public State
{
public:
	// コンストラクタ
	ConfusionState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// デストラクタ
	~ConfusionState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	bool loopSe = false;
	float				stateTimer = 0.0f;
	float				stateTimerMax = 2.8f;
	float				stateTimerEnd = 0.0f;
	// アニメーションルール
	Model::ModelAnim modelAnim;
	// 攻撃種類
	// 行動をランダムで
	int randamAttack = 0;
};

// 死亡ステートオブジェクト
class DeathState : public State
{
public:
	// コンストラクタ
	DeathState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// デストラクタ
	~DeathState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	bool loopSe = false;
	float				stateTimer = 0.0f;
	// アニメーションルール
	Model::ModelAnim modelAnim;
	// クリア確認
	bool clearCheck = true;
};

// 覚醒ステートオブジェクト
class AwakeStartState : public State
{
public:
	// コンストラクタ
	AwakeStartState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// デストラクタ
	~AwakeStartState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	// アニメーションルール
	Model::ModelAnim modelAnim;
	// クリア確認
	bool clearCheck = true;
	// カメラ距離
	float distance = 3.0f;
	// カメラの揺れの深さ
	float cameraShakePower = 0.1f;
	// カメラの揺れ時間
	float cameraShakeTime = 1.0f;
	// アニメーションの時間
	float animationPose = 6.70f;
	RadialBlurData radialBlurData;
};
// クリアステートオブジェクト
class ClearState : public State
{
public:
	// コンストラクタ
	ClearState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// デストラクタ
	~ClearState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	bool loopSe = false;
	float				stateTimer = 0.0f;
	// クリア確認
	bool clearCheck = true;
	// カメラ距離
	float distance = 3.0f;
};

// クリアステートオブジェクト
class ClearReviveState : public State
{
public:
	// コンストラクタ
	ClearReviveState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// デストラクタ
	~ClearReviveState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	bool loopSe = false;
	float				stateTimer = 0.0f;
	// アニメーションルール
	Model::ModelAnim modelAnim;
	// クリア確認
	bool clearCheck = true;
	// カメラ距離
	float distance = 3.0f;
};
//////////////////////////////
// プレイヤー
//////////////////////////////
// 待機ステートオブジェクト
class PlayerIdleState : public State
{
public:
	// コンストラクタ
	PlayerIdleState(std::weak_ptr<Actor> player) :State(player) {};
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
	// アニメーションルール
	Model::ModelAnim modelAnim;
};

// 移動ステートオブジェクト
class PlayerMovestate : public State
{
public:
	// コンストラクタ
	PlayerMovestate(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerMovestate() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	bool loopSe = true;

	// アニメーションルール
	Model::ModelAnim modelAnim;
};

// ジャンプステートオブジェクト
class PlayerJumpState : public State
{
public:
	// コンストラクタ
	PlayerJumpState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerJumpState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	bool loopSe = false;
	float				stateTimer = 0.0f;
	float				attackRange = 1.5f;
	bool                jumpCheck = false;
	// アニメーションルール
	Model::ModelAnim modelAnim;
};

// 着地ステートオブジェクト
class PlayerLandState : public State
{
public:
	// コンストラクタ
	PlayerLandState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerLandState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	bool loopSe = false;

	float				stateTimer = 0.0f;

	// アニメーションルール
	Model::ModelAnim modelAnim;
};

// ジャンプ中ステートオブジェクト
class PlayerJumpFlipState : public State
{
public:
	// コンストラクタ
	PlayerJumpFlipState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerJumpFlipState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	bool loopSe = false;

	float				stateTimer = 0.0f;

	// アニメーションルール
	Model::ModelAnim modelAnim;

	// 回転スピード
	float turnSpeed = DirectX::XMConvertToRadians(720);
};


// 攻撃ステートオブジェクト
class PlayerQuickJabState : public State
{
public:
	// コンストラクタ
	PlayerQuickJabState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerQuickJabState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	bool loopSe = false;
	std::unique_ptr<Effect> tellePort;
	float				stateTimer = 0.0f;
	bool				button = false;
	bool				buttonSeconde = false;

	float				turnSpeed = DirectX::XMConvertToRadians(720);

	float               speed = 10;

	float               attackSpeed = 3.0f;

	// 最初の一回だけ呼びたいから
	bool                InitializationCheck = false;

	// 再生ループ
	bool  loop = false;

	// アニメーションブレンド
	float blendSeconds = 0.5f;

	// 回転許可
	bool rotateCheck = false;

	// 距離
	float length = 0;

	DirectX::XMFLOAT3 vector = {0,0,0};

	// 攻撃サポート範囲
	float attackCheckRange = 6.0f;

	float attackCheckRangeMin = 2.6f;

	float gravity = -0.2f;

	// 加速度
	DirectX::XMFLOAT3 velocity = { 0,0,0 };

	// コマンド確認
	std::vector<GamePadButton> commandSeconde;
	std::vector<GamePadButton> commandThrede;
	
	// ダメージ食らった時に強制終了
	bool deleteCheck = false;

	// ３回目の攻撃で強制終了
	int attackMemory = 0;
	int attackMemoryMax = 3;

	// コマンド操作の記録確認時間
	int frame = 150;

	// 角度範囲
	DirectX::XMFLOAT2 angleRange = { 0.1f,0.1f };

	bool              oneAttackCheck = false;
	// 描画
	int				  isPlayerDrawCheck = 0;
	// ホーミング一回だけ
	bool			  isHomingStartCheck = false;
};

// 攻撃ステートオブジェクト
class PlayerSideCutState : public State
{
public:
	// コンストラクタ
	PlayerSideCutState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerSideCutState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	bool loopSe = false;

	float				stateTimer = 0.0f;
	bool				button = false;
	bool				buttonSeconde = false;

	float				turnSpeed = DirectX::XMConvertToRadians(720);

	float               speed = 10;

	float               attackSpeed = 3.0f;

	// 最初の一回だけ呼びたいから
	bool                InitializationCheck = false;

	// アニメーションブレンド
	float blendSeconds = 0.5f;

	// 回転許可
	bool rotateCheck = false;

	// 距離
	float length = 0;

	DirectX::XMFLOAT3 vector = { 0,0,0 };

	// 攻撃サポート範囲
	float attackCheckRange = 9;

	float attackCheckRangeMin = 7;

	float gravity = -0.2f;

	// 加速度
	DirectX::XMFLOAT3 velocity = { 0,0,0 };

	// コマンド確認
	std::vector<GamePadButton> commandSeconde;
	std::vector<GamePadButton> commandThrede;

	// ダメージ食らった時に強制終了
	bool deleteCheck = false;

	// ３回目の攻撃で強制終了
	int attackMemory = 0;
	int attackMemoryMax = 3;

	// コマンド操作の記録確認時間
	int frame = 150;

	// 角度範囲
	DirectX::XMFLOAT2 angleRange = { 0.9f,0.9f };

	bool              oneAttackCheck = false;
};

// 攻撃ステートオブジェクト
class PlayerCycloneStrikeState : public State
{
public:
	// コンストラクタ
	PlayerCycloneStrikeState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerCycloneStrikeState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	bool loopSe = false;

	float				stateTimer = 0.0f;
	bool				button = false;
	bool				buttonSeconde = false;

	float				turnSpeed = DirectX::XMConvertToRadians(720);

	float               speed = 10;

	float               attackSpeed = 3.0f;

	// 最初の一回だけ呼びたいから
	bool                InitializationCheck = false;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;
	// 再生時間加算分の値
	float currentAnimationAddSeconds = 0.03f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;

	// 回転許可
	bool rotateCheck = false;

	// 距離
	float length = 0;

	DirectX::XMFLOAT3 vector = { 0,0,0 };

	// 攻撃サポート範囲
	float attackCheckRange = 9;

	float attackCheckRangeMin = 7;

	float gravity = -0.2f;

	// 加速度
	DirectX::XMFLOAT3 velocity = { 0,0,0 };

	// コマンド確認
	std::vector<GamePadButton> commandSeconde;
	std::vector<GamePadButton> commandThrede;

	// ダメージ食らった時に強制終了
	bool deleteCheck = false;

	// ３回目の攻撃で強制終了
	int attackMemory = 0;
	int attackMemoryMax = 3;

	// コマンド操作の記録確認時間
	int frame = 150;

	// 角度範囲
	DirectX::XMFLOAT2 angleRange = { 0.9f,0.9f };

	bool              oneAttackCheck = false;
};


// 近接必殺技ステートオブジェクト
class PlayerSpecialAttackState : public State
{
public:
	// コンストラクタ
	PlayerSpecialAttackState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerSpecialAttackState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	bool loopSe = false;

	DirectX::XMFLOAT3 position = {0.0f,0.0f,0.0f};
	DirectX::XMFLOAT3 angle = { 0.0f,0.0f,0.0f };

	float				stateTimer = 0.0f;

	std::unique_ptr<Effect> lightning;
	std::unique_ptr<Effect> lightningAttack;
	std::unique_ptr<Effect> lightningHit;

	bool button = false;

	// ロックオンモード
	bool rockCheck = true;

	float				turnSpeed = DirectX::XMConvertToRadians(720);

	// 一瞬白く
	bool flashOn = true;

	// 回転確認
	bool isRotate = false;

	// 向き
	DirectX::XMFLOAT3 vector = { 0.0f,0.0f,0.0f };
	// 距離
	float length = 0.0f;

	// 距離の最大値
	float attackCheckRangeMax = 10.5f;
	// 距離の最低値
	float attackCheckRangeMin = 2.6f;

	// 最大の回転
	DirectX::XMFLOAT2 angleRange = { 0.9f,0.9f };
};

// 魔法ステートオブジェクト
class PlayerMagicState : public State
{
public:
	// コンストラクタ
	PlayerMagicState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerMagicState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	// 回復魔法
	std::unique_ptr<Effect> heale;

	// se
	bool loopSe = false;

	// アニメーションルール
	Model::ModelAnim modelAnim;

	// 魔法発射種類
	int                       magicType = 0;

	// 回転速度
	float				turnSpeed = DirectX::XMConvertToRadians(720);

	// 歩く速度
	float moveSpeed = 5.0f;

	// 回転許可
	bool                      isRotate = false;

	// 向き
	DirectX::XMFLOAT3 vector = { 0.0f,0.0f,0.0f };
	// 距離
    float length = 0.0f;

	// 角度範囲
	DirectX::XMFLOAT2 angleRange = { 0.9f,0.9f };

	// 移動許可
	bool isMove = false;
};

// 近接必殺技ステートオブジェクト
class PlayerSpecialMagicState : public State
{
public:
	// コンストラクタ
	PlayerSpecialMagicState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerSpecialMagicState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	bool loopSe = false;

	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 angle = { 0.0f,0.0f,0.0f };

	float				stateTimer = 0.0f;

	std::unique_ptr<Effect> fire;
	std::unique_ptr<Effect> fireAttack;

	// アニメーションルール
	Model::ModelAnim modelAnim;

	bool button = false;

	// ロックオンモード
	bool rockCheck = true;

	float				turnSpeed = DirectX::XMConvertToRadians(720);

	// スタート値
	float specialMoveWaitStartTime = 0.0f;

	// 魔法打った後の待機時間
	float specialMoveWaitTime = specialMoveWaitStartTime;
	float specialMoveWaitTimeMax = 3.0f;

	// 最初だけ魔法を発動するため
	bool startMagic = false;
	bool isMagic = false;

	// 回転確認
	bool isRotate = false;

	// 向き
	DirectX::XMFLOAT3 vector = { 0.0f,0.0f,0.0f };
	// 距離
	float length = 0.0f;

	// 距離の最大値
	float attackCheckRangeMax = 10.5f;
	// 距離の最低値
	float attackCheckRangeMin = 2.6f;

	// 最大の回転
	DirectX::XMFLOAT2 angleRange = { 0.9f,0.9f };
};

// よろけステートオブジェクト
class PlayerDamageState : public State
{
public:
	// コンストラクタ
	PlayerDamageState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerDamageState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;

private:
	bool loopSe = false;

	// アニメーションルール
	Model::ModelAnim modelAnim;
};

// 回避ステートオブジェクト
class PlayerAvoidanceState : public State
{
public:
	// コンストラクタ
	PlayerAvoidanceState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerAvoidanceState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;

private:
	bool loopSe = false;

	// エフェクト
	std::unique_ptr<Effect> wind;

	float				stateTimer = 0.0f;

	float               moveSpeed = 10.0f;
	float               speed = 4.5f;
	float               flySpeed = 3.0f;

	// アニメーションルール
	Model::ModelAnim modelAnim;
	Model::ModelAnim modelAnimUpper;
	// 可速度
	float addHeight = 1.0f;
};

// 反射ステートオブジェクト
class PlayerReflectionState : public State
{
public:
	// コンストラクタ
	PlayerReflectionState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerReflectionState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	bool loopSe = false;

	float				stateTimer = 0.0f;

	// アニメーションルール
	Model::ModelAnim modelAnim;
};

// 死亡ステートオブジェクト
class PlayerDeathState : public State
{
public:
	// コンストラクタ
	PlayerDeathState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerDeathState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	// アニメーションルール
	Model::ModelAnim modelAnim;
	// 経過時間最大
	float stateTimer = 0.0f;
	float stateTimerMax = 40;

	ColorGradingData colorGradingData;
};

// 復活ステートオブジェクト
class PlayerReviveState : public State
{
public:
	// コンストラクタ
	PlayerReviveState(std::weak_ptr<Actor> player) :State(player) {};
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
	float blendSeconds = 0.5f;
};


// プレイヤータイトル用待機ステートオブジェクト
class PlayerTitleIdleState : public State
{
public:
	// コンストラクタ
	PlayerTitleIdleState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerTitleIdleState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	// ステート最大値
	int					stateSize = 2;

	// アニメーションルール
	Model::ModelAnim modelAnim;
};

// プレイヤータイトル用決定ステートオブジェクト
class PlayerTitlePushState : public State
{
public:
	// コンストラクタ
	PlayerTitlePushState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerTitlePushState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;
	// アニメーションルール
	Model::ModelAnim modelAnim;

	// 過ぎたら
	bool secondeMortion = false;

	// エフェクト
	std::unique_ptr<Effect> lightning;
	std::unique_ptr<Effect> lightningAttack;
	std::unique_ptr<Effect> lightningHit;

	RadialBlurData radialBlurData;
};

// プレイヤークリア用待機ステートオブジェクト
class PlayerClearIdleState : public State
{
public:
	// コンストラクタ
	PlayerClearIdleState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerClearIdleState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;

private:
	float				stateTimer = 0.0f;

	// アニメーションルール
	Model::ModelAnim modelAnim;
};



// プレイヤークリア用待機ステートオブジェクト
class PlayerClearEscapeState : public State
{
public:
	// コンストラクタ
	PlayerClearEscapeState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerClearEscapeState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;
	// アニメーションルール
	Model::ModelAnim modelAnim;
};

// プレイヤークリア用待機ステートオブジェクト
class PlayerOverIdleState : public State
{
public:
	// コンストラクタ
	PlayerOverIdleState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerOverIdleState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;
private:
	float				stateTimer = 0.0f;
};

// プレイヤークリア用待機ステートオブジェクト
class PlayerOverReviveState : public State
{
public:
	// コンストラクタ
	PlayerOverReviveState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerOverReviveState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;

private:
	float				stateTimer = 0.0f;

	// アニメーションルール
	Model::ModelAnim modelAnim;
	
	// ポーズ切り替え
	bool isPoseStarte = false;

	// 終わり
	bool endState = true;
};


