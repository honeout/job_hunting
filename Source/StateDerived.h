#pragma once
#include "StateBase.h"
#include "Input\GamePad.h"
#include "Input/Input.h"
#include "SceneGameClear.h"
#include "Actor.h"
#include "Effect.h"
#include "ProjectileTornade.h"
//#include "Audio/AudioSource.h"


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

	// 終了処理
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;
	////std::shared_ptr<ModelControll> modelid;
	//std::shared_ptr<Movement> moveid = nullptr;
	////std::shared_ptr<Actor> playerid;

	//Model* model = nullptr;
	
	//float				stateTimerEnd = 0.0f;
	//float				stateTimer = stateTimerEnd;
	//float				stateTimerMax = 4.0f;

	// 覚醒時に攻撃回数を変える。
	int                 attackCountMax = 2;
	int                 attackCountMin = 0;
	int                 attackCount = attackCountMin;

	// エフェクトスケール
	float               scaleEffect = 0.3f;

	// エフェクト
	std::unique_ptr<Effect> smorker;

	// 歩行SE
	//std::unique_ptr<AudioSeSource> walkSe;
	bool loopSe = true;

	// 着地瞬間
	bool                upOnLading = false;


	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 3.3f;

	// 再生終了時間
	float currentAnimationEndSeconds = 300.0f;

	// 再生時間加算分の値
	float currentAnimationAddSeconds = 0.00f;
	//float currentAnimationAddSeconds = 0.06f;

	// キーフレームの終了
	float keyFrameEnd = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.7f;

	// 歩き速度
	float moveSpeed = 3;

	// 回転速度
	float turnSpeed = DirectX::XMConvertToRadians(320);

	// ターゲット位置
	DirectX::XMFLOAT3 targetPosition = {.0f,.0f,.0f};


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

	// 終了処理
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;
	//std::shared_ptr<ModelControll> model;
	//Model*  model = nullptr;
	//std::shared_ptr<Movement> move = nullptr;
	//std::shared_ptr<HP> hp = nullptr;
	
	float               stateTimerEnd = 0.0f;
	float				stateTimer = 0.0f;
	float				stateTimerMax = 3.0f;
	float				stateTimerMin = 1.0f;

	// 再生ループ
	bool  loop = true;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.7f;
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

	// 終了処理
	void End()override;
private:
	//std::shared_ptr<Actor> playerid;
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;
	//std::shared_ptr<HP> hpid = nullptr;
	

	float				stateTimer = 0.0f;
	float				stateTimerMin = 0.0f;
	float				attackRange = 0.0f;

	int                 attackRound = 0;
	int                 attackCountMax = 0;

	int                 attackType = 0;

	// 再生ループ
	bool  loop = true;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.7f;


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

	// 終了処理
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;
	////std::shared_ptr<ModelControll> modelid;
	//std::shared_ptr<Movement> moveid = nullptr;
	//std::shared_ptr<HP> hpid = nullptr;
	////std::shared_ptr<Actor> playerid;

	////float				stateTimer = 0.0f;

	//Model* model = nullptr;

	//// ジャンプ
	//std::unique_ptr<AudioSeSource> janpSe;

	//// 着地
	//std::unique_ptr<AudioSeSource> landSe;


	//// 覚醒時の攻撃回数
	//int                 attackCountMax = 2;
	//int                 attackCountMin = 0;
	//int                 attackCount = attackCountMin;

	// エフェクトスケール
	float               scaleEffect = 0.3f;

	// エフェクト
	std::unique_ptr<Effect> smorker;

	// ループするかSe
	bool loopSe = false;

	// 着地瞬間
	bool                upOnLading = false;


	// 再生ループ
	bool  loop = true;

	// 再生開始時間 
	float currentAnimationStartSeconds = 2.0f;

	// 再生時間加算分の値
	float currentAnimationAddSeconds = 0.025f;

	// キーフレームの終了
	float keyFrameEnd = 0.8f;

	// アニメーションブレンド
	float blendSeconds = 0.7f;

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

	// 終了処理
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;

	//std::shared_ptr<HP> hpid = nullptr;

	float				stateTimer = 0.0f;

	// 攻撃回数
	int					attackMemory = 0;
	int					attackMemoryMax = 3;
	
	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.7f;

};

// 射撃ステートオブジェクト
class AttackShotState : public State
{
public:
	// コンストラクタ
	AttackShotState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// デストラクタ
	~AttackShotState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;

	// 終了処理
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;

	//std::shared_ptr<HP> hpid = nullptr;
	//std::shared_ptr<ProjectileRuby> projectileRubyid;

	float				stateTimer = 0.0f;

	int                 attackCount = 0;
	int                 attackCountMax = 0;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.7f;
};
// 射撃弧ステートオブジェクト
class AttackShotThrowingState : public State
{
public:
	// コンストラクタ
	AttackShotThrowingState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// デストラクタ
	~AttackShotThrowingState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;

	// 終了処理
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;

	//std::shared_ptr<Transform> transformid = nullptr;


	float				stateTimer = 0.0f;

	int                 attackCount = 0;
	int                 attackCountMax = 0;
	bool				turnPermission = false;

	float               turnSpeed = DirectX::XMConvertToRadians(320);

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSecondsf = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.7f;
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

	// 終了処理
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;

	//std::shared_ptr<Transform> transformid;

	// 被ダメージ
	//std::unique_ptr<AudioSource> damageSe;
	bool loopSe = false;

	float				stateTimer = 0.0f;
	float				stateTimerMax = 5.0f;
	float				stateTimerEnd = 0.0f;


	//// 再生ループ
	//bool  loop = false;

	//// 再生開始時間 
	//float currentAnimationStartSeconds = 0.0f;

	//// アニメーションブレンド
	//float blendSeconds = 0.7f;

	// 再生ループ
	bool  loop = true;

	// 再生開始時間 
	float currentAnimationStartSeconds = 2.5f;

	// 再生時間加算分の値
	float currentAnimationAddSeconds = 0.025f;

	// キーフレームの終了
	float keyFrameEnd = 3.0f;

	// アニメーションブレンド
	float blendSeconds = 0.7f;
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

	// 終了処理
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;

	// 混乱
	//std::unique_ptr<AudioSource> confusionSe;
	bool loopSe = false;

	float				stateTimer = 0.0f;
	float				stateTimerMax = 0.8f;

	float				stateTimerEnd = 0.0f;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.426f;

	// 再生時間加算分の値
	float currentAnimationAddSeconds = 0.00f;

	// キーフレームの終了
	float keyFrameEnd = 70.0f;

	// アニメーションブレンド
	float blendSeconds = 0.35f;

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

	// 終了処理
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;

	//std::shared_ptr<Transform> transformid = nullptr;

	// 死亡音声
	//std::unique_ptr<AudioSource> deathSe;
	//std::unique_ptr<AudioSeSource> deathSe;
	bool loopSe = false;

	float				stateTimer = 0.0f;


	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.7f;

	// クリア確認
	bool clearCheck = true;
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

	// 終了処理
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;

	//std::shared_ptr<Transform> transformid = nullptr;

	// 死亡音声
	//std::unique_ptr<AudioSource> deathSe;
	//std::unique_ptr<AudioSeSource> deathSe;
	bool loopSe = false;

	float				stateTimer = 0.0f;


	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.7f;

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

	// 終了処理
	void End()override;
private:
	//std::shared_ptr<Actor> playerid;
	//std::shared_ptr<Movement> moveid;

	float				stateTimer = 0.0f;

	// 再生ループ
	bool  loop = true;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;
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

	// 終了処理
	void End()override;
private:

	// 歩行
	//std::unique_ptr<AudioSource> walkSe;
	bool loopSe = true;

	//std::shared_ptr<Movement> moveid;


	// 再生ループ
	bool  loop = true;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;

	// 再生時間加算分の値
	float currentAnimationAddSeconds = 0.05f;
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

	// 終了処理
	void End()override;
private:
	//std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;

	// ジャンプ
	//std::unique_ptr<AudioSource> janpSe;
	bool loopSe = false;

	float				stateTimer = 0.0f;
	float				attackRange = 1.5f;

	bool                jumpCheck = false;
	//bool				loop = false;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;
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

	// 終了処理
	void End()override;
private:
	//std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;

	// 着地
	//std::unique_ptr<AudioSource> landSe;

	bool loopSe = false;

	float				stateTimer = 0.0f;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.35f;

	// 再生時間加算分の値
	float currentAnimationAddSeconds = 0.00f;

	// キーフレームの終了
	float keyFrameEnd = 40.0f;


	// アニメーションブレンド
	float blendSeconds = 0.5f;
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

	// 終了処理
	void End()override;
private:
	//std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;


	// ジャンプ
	//std::unique_ptr<AudioSource> janpSe;
	bool loopSe = false;

	float				stateTimer = 0.0f;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.3f;

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

	// 終了処理
	void End()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	//std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;
	//std::shared_ptr<Transform> transformid;

	// 足もとに竜巻
	//std::unique_ptr<Effect> areWork;

	// 斬撃
	//std::unique_ptr<AudioSource> slashSe;
	bool loopSe = false;
	

	//DirectX::XMFLOAT3 angle; 

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
	float attackCheckRange = 4.5f;

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
	DirectX::XMFLOAT2 angleRange = { 0.9f,0.9f };

	bool              oneAttackCheck = false;
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

	// 終了処理
	void End()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	//std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;
	//std::shared_ptr<Transform> transformid;

	// 斬撃
	//std::unique_ptr<AudioSource> slashSe;
	bool loopSe = false;

	//DirectX::XMFLOAT3 angle;

	float				stateTimer = 0.0f;
	bool				button = false;
	bool				buttonSeconde = false;

	float				turnSpeed = DirectX::XMConvertToRadians(720);

	float               speed = 10;

	float               attackSpeed = 3.0f;

	// 最初の一回だけ呼びたいから
	bool                InitializationCheck = false;

	//// 再生ループ
	//bool  loop = false;

	//// 再生開始時間 
	//float currentAnimationStartSeconds = 0.0f;
	//// 再生時間加算分の値
	//float currentAnimationAddSeconds = 0.03f;

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

	// 終了処理
	void End()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	//std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;
	//std::shared_ptr<Transform> transformid;

	// 斬撃
	//std::unique_ptr<AudioSource> slashSe;
	bool loopSe = false;

	//DirectX::XMFLOAT3 angle;

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

	// 終了処理
	void End()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	//std::shared_ptr<Transform> transformid;
	////std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;

	//// エネミー入れ物
	//std::shared_ptr<Transform> enemyTransform;
	//std::shared_ptr<HP> enemyHpId;

	//// 斬撃
	//std::unique_ptr<AudioSource> slashSe;

	//// 必殺技雷
	//std::unique_ptr<AudioSource> lighteningStrikeSpecialSe;
	//// 必殺技雷ため
	//std::unique_ptr<AudioSource> lighteningStrikeSpecialSaveSe;
	bool loopSe = false;

	DirectX::XMFLOAT3 position = {0.0f,0.0f,0.0f};
	DirectX::XMFLOAT3 angle = { 0.0f,0.0f,0.0f };

	float				stateTimer = 0.0f;

	std::unique_ptr<Effect> lightning;
	std::unique_ptr<Effect> lightningAttack;
	std::unique_ptr<Effect> lightningHit;

	//// 再生ループ
	//bool  loop = false;

	//// 再生開始時間 
	//float currentAnimationStartSeconds = 0.0f;
	//// 再生時間加算分の値
	//float currentAnimationAddSeconds = 0.00f;

	//// アニメーションブレンド
	//float blendSeconds = 0.5f;


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

	// 終了処理
	void End()override;

private:
	//std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;


	//// 炎発射
	//std::unique_ptr<AudioSource> flameStarteSe;
	//// 炎持続
	////std::unique_ptr<AudioSource> flameDurationSe;
	//// 炎着弾時
	//std::unique_ptr<AudioSource> flameimpactSe;


	//// 氷発射
	//std::unique_ptr<AudioSource> iceStarteSe;
	//// 氷持続
	//std::unique_ptr<AudioSource> iceDurationSe;
	//// 氷着弾時
	//std::unique_ptr<AudioSource> iceimpactSe;

	//// 雷ヒット時
	//std::unique_ptr<AudioSource> lightningSe;

	// se
	bool loopSe = false;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;

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

	// 終了処理
	void End()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	//std::shared_ptr<Transform> transformid;
	////std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;

	// エネミー入れ物
	//std::shared_ptr<Transform> enemyTransform;
	//std::shared_ptr<HP> enemyHpId;

	// 炎持続
	//std::unique_ptr<AudioSource> flameDurationSe;

	// 必殺技炎
	//std::unique_ptr<AudioSource> flameSpecialStarteSe;
	// 必殺技炎ため
	//std::unique_ptr<AudioSource> flameSpecialSaveSe;
	bool loopSe = false;


	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 angle = { 0.0f,0.0f,0.0f };

	float				stateTimer = 0.0f;

	std::unique_ptr<Effect> fire;
	std::unique_ptr<Effect> fireAttack;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;
	// 再生時間加算分の値
	float currentAnimationAddSeconds = 0.00f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;


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


// 必殺技氷魔法
class PlayerSpecialMagicIceState : public State
{
public:
	// コンストラクタ
	PlayerSpecialMagicIceState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerSpecialMagicIceState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;

	// 終了処理
	void End()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	//std::shared_ptr<Transform> transformid;
	//std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;

	// エネミー入れ物
	//std::shared_ptr<Transform> enemyTransform;
	//std::shared_ptr<HP> enemyHpId;

	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 angle = { 0.0f,0.0f,0.0f };

	float				stateTimer = 0.0f;

	std::unique_ptr<Effect> ice;
	//std::unique_ptr<Effect> fireAttack;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;
	// 再生時間加算分の値
	float currentAnimationAddSeconds = 0.00f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;

	// キーフレーム終了
	float keyFrameEnd = 64.00f;


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

};


// 必殺技雷魔法
class PlayerSpecialThanderMagicState : public State
{
public:
	// コンストラクタ
	PlayerSpecialThanderMagicState(std::weak_ptr<Actor> player) :State(player) {};
	// デストラクタ
	~PlayerSpecialThanderMagicState() {}
	// ステートに入った時のメソッド
	void Enter()override;
	// ステートで実行するメソッド
	void Execute(float elapsedTime)override;
	// ステートから出ていくときのメソッド
	void Exit()override;

	// 終了処理
	void End()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	//std::shared_ptr<Transform> transformid;
	////std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;

	//// エネミー入れ物
	//std::shared_ptr<Transform> enemyTransform;
	//std::shared_ptr<HP> enemyHpId;

	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 angle = { 0.0f,0.0f,0.0f };

	float				stateTimer = 0.0f;

	std::unique_ptr<Effect> fire;
	std::unique_ptr<Effect> fireAttack;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;
	// 再生時間加算分の値
	float currentAnimationAddSeconds = 0.00f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;


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

	// 終了処理
	void End()override;
private:
	//std::shared_ptr<ModelControll> model;
	//std::shared_ptr<Movement> moveid;
	//Model* model;

	// 被ダメージ
	//std::unique_ptr<AudioSource> damageSe;
	bool loopSe = false;

	float				stateTimer = 0.0f;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;
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

	// 終了処理
	void End()override;
private:
	//std::shared_ptr<Transform> transformid;
	//std::shared_ptr<Movement> moveid;
	//std::shared_ptr<HP> hpid;

	// ダッシュ
	//std::unique_ptr<AudioSource> dushSe;
	bool loopSe = false;

	// エフェクト
	std::unique_ptr<Effect> wind;


	float				stateTimer = 0.0f;

	float               moveSpeed = 10.0f;
	float               speed = 4.5f;
	float               flySpeed = 3.0f;


	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.3f;

	// 再生時間加算分の値
	float currentAnimationAddSeconds = 0.025f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;

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

	// 終了処理
	void End()override;
private:
	

	// 反射
	//std::unique_ptr<AudioSource> reflectionStop;
	bool loopSe = false;

	float				stateTimer = 0.0f;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;
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

	// 終了処理
	void End()override;
private:
	//std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<HP> hpid;
	//std::shared_ptr<Transform> transformid;
	//float				stateTimer = 0.0f;

	// 死亡音声敵
	//std::unique_ptr<AudioSource> deathSe;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;

	// 経過時間最大
	float stateTimer = 0.0f;
	float stateTimerMax = 40;
	//float stateTimerMax = 240;
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

	// 終了処理
	void End()override;
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

	// 終了処理
	void End()override;
private:
	float				stateTimer = 0.0f;

	// 再生ループ
	bool  loop = true;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;
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

	// 終了処理
	void End()override;
private:
	float				stateTimer = 0.0f;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;

	// 過ぎたら
	bool secondeMortion = false;

	// エフェクト
	std::unique_ptr<Effect> lightning;
	std::unique_ptr<Effect> lightningAttack;
	std::unique_ptr<Effect> lightningHit;
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

	// 終了処理
	void End()override;
private:
	float				stateTimer = 0.0f;

	// 再生ループ
	bool  loop = true;

	// 再生開始時間 
	float currentAnimationStartSeconds = 0.0f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;
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

	// 終了処理
	void End()override;
private:
	float				stateTimer = 0.0f;

	// 再生ループ
	bool  loop = false;

	// 再生開始時間 
	float currentAnimationStartSeconds = 1.7f;

	// アニメーションブレンド
	float blendSeconds = 0.5f;

	// 再生時間加算分の値
	float currentAnimationAddSeconds = 0.00f;

	// キーフレーム終了
	float keyFrameEnd = 105.00f;
};

