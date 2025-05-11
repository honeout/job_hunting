#include <math.h>
#include "EnemyBoss.h"
#include "StateDerived.h"
#include "Player.h"
#include "Mathf.h"
#include "ProjectileHoming.h"
#include "ProjectileFullHoming.h"
#include "ProjectileStraight.h"
#include "ProjectileManager.h"
#include "BulletFiring.h"
#include "SceneLoading.h"
#include "SceneManager.h"
#include "SceneGameOver.h"
#include "SceneGame.h"

// 徘徊ステートに入った時のメソッド
void WanderState::Enter()
{
	// コンポネント取得
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// owner呼び出し
	std::shared_ptr<EnemyBoss> enemyid =sharedId->GetComponent<EnemyBoss>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	// 縄張り
	if (!enemyid) return;
	enemyid->SetRandomTargetPosition();
	// 攻撃回数
	attackCount = attackCountMax;
	// 覚醒時の設定
	if (!enemyid) return;
	if (enemyid->GetIsEnemyAwakened())
	{
		attackCount = attackCountMin;
		modelAnim.keyFrameEnd = 288;
	}

	// アニメーション設定
	modelAnim.index = (int)EnemyBoss::Animation::Anim_Movie;
	modelAnim.loop = false;
	modelAnim.currentanimationseconds = 3.3f;
	modelAnim.blendSeconds = 0.7f;
	modelAnim.currentAnimationAddSeconds = 0.008f;
	modelAnim.keyFrameEnd = 300.0f;

	// アニメーション再生
	model->
		PlayAnimation(modelAnim);
	// アニメーションルール
	if (!enemyid) return;
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	// 着地瞬間
	upOnLading = false;
	// 煙エフェクト
	smorker = std::make_unique<Effect>("Data/Effect/smorkeDash.efk");
}

// 徘徊ステートで実行するメソッド
void WanderState::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;
	// owner呼び出し
	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();

	// 目標地点ををプレイヤー位置に設定
	targetPosition = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentANimationSeconds();
	enemyid->SetTargetPosition(targetPosition);
	DirectX::XMVECTOR targetPositionVec = DirectX::XMLoadFloat3(&targetPosition);
	DirectX::XMVECTOR enemyPositionVec = DirectX::XMLoadFloat3(&enemyid->GetPosition());
	DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(targetPositionVec, enemyPositionVec);
	DirectX::XMFLOAT3 direction;
	DirectX::XMStoreFloat3(&direction, directionVec);

	// ボスの覚醒アニメーション終了;
	if (!model->IsPlayAnimation() && enemyid->GetIsEnemyAwakened() && attackCount < attackCountMax)
	{
		++attackCount;
		// アニメーション終了キーフレーム
		modelAnim.currentAnimationAddSeconds = 0.00f;
		modelAnim.keyFrameEnd = 288;

		// アニメーション再生
		model->
			PlayAnimation(modelAnim);
	}

	// アニメーション終わり　待機に
	if (!model->IsPlayAnimation())
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
	}

	// 移動
	if (animationTime <= 4.6f)
	{
		moveid->Turn(direction, turnSpeed, elapsedTime);
		moveid->Move(direction, moveSpeed, elapsedTime);
		// ポストエフェクトインスタンスゲット
		PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
		// ラジアルブラー
		radialBlurData.radius = 30.0f;
		postprocessingRenderer.SetRadialBlurData(radialBlurData);
	}

	// 最初の速度が遅い後に通常に戻す
	if (animationTime >= slowWalkTime)
	{
		modelAnim.currentAnimationAddSeconds = 0.00f;
		model->AnimationRuleChanger(modelAnim.currentAnimationAddSeconds);
	}
	// 後変更
	// 予備動作中は当たり判定無し
	else
		return;

	// 目的地にこの半径入ったら
	float radius = enemyid->GetAttackRightFootRange();
	// 当たり判定
	{
		Model::Node * bossLeftFoot = model->FindNode("boss_left_foot1");
		Model::Node* bossRightFoot = model->FindNode("boss_right_foot1");
		Model::Node* bossRightHand = model->FindNode("boss_right_hand1");
		Model::Node* bossLeftHand = model->FindNode("boss_left_hand1");
		// ノード位置取得
		// 左足
		DirectX::XMFLOAT3 bossLeftFootPosition;
		bossLeftFootPosition = model->ConvertLocalToWorld(bossLeftFoot);
		// 右足
		DirectX::XMFLOAT3 bossRightFootPosition;
		bossRightFootPosition = model->ConvertLocalToWorld(bossRightFoot);
		// 右腕
		DirectX::XMFLOAT3 bossRightHandPosition;
		bossRightHandPosition = model->ConvertLocalToWorld(bossRightHand);
		// 左腕
		DirectX::XMFLOAT3 bossLeftHandPosition;
		bossLeftHandPosition = model->ConvertLocalToWorld(bossLeftHand);
		// 足が地面につく
		if (animationTime + FLT_EPSILON >= 4.0f - FLT_EPSILON &&
			animationTime - FLT_EPSILON <= 4.1f + FLT_EPSILON)
		{
			// 音
			enemyid->InputJampSe();
			smorker->Play(bossRightFootPosition, scaleEffect);
			// 左足
			enemyid->DetectHitByBodyPart(bossLeftFootPosition, applyDamage);
			// 右足
			enemyid->DetectHitByBodyPart(bossRightFootPosition, applyDamage);
			// 右腕
			enemyid->DetectHitByBodyPart(bossRightHandPosition, applyDamage);
			// 左腕
			enemyid->DetectHitByBodyPart(bossLeftHandPosition, applyDamage);
			// 中心
			enemyid->DetectHitByBodyPart(transformid->GetPosition(), applyDamage);
		}

		// 足が地面につく
		if (animationTime + FLT_EPSILON >= 4.6f - FLT_EPSILON &&
			animationTime - FLT_EPSILON <= 4.7f + FLT_EPSILON)
		{
			// 音
			enemyid->InputJampSe();
			smorker->Play(bossLeftFootPosition, scaleEffect);
			// 左足
			enemyid->DetectHitByBodyPart(bossLeftFootPosition, applyDamage);
			// 右足
			enemyid->DetectHitByBodyPart(bossRightFootPosition, applyDamage);
			// 右腕
			enemyid->DetectHitByBodyPart(bossRightHandPosition, applyDamage);
			// 左腕
			enemyid->DetectHitByBodyPart(bossLeftHandPosition, applyDamage);
			// 中心
			enemyid->DetectHitByBodyPart(transformid->GetPosition(), applyDamage);
		}
	}
}

// 徘徊ステートから出ていくときのメソッド
void WanderState::Exit()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;
	// owner呼び出し
	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	// 連続攻撃
	attackCount = attackCountMin;
	enemyid->InputSlashSe();
}

// 初期化
void IdleState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;
	// owner呼び出し
	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<HP> hp = sharedId->GetComponent<HP>();
	modelAnim.index = EnemyBoss::Anim_Standby;
	modelAnim.loop = true;
	modelAnim.currentanimationseconds = 0.0f;
	modelAnim.blendSeconds = 0.7f;
	model->PlayAnimation(modelAnim);

	// アニメーションルール
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	stateTimer = stateTimerMax;

	// 逃げる条件初期化
	hp->ResetOnDamageThresholdTime();
	randRoot = rand() % 2;
}

// update
void IdleState::Execute(float elapsedTime)
{
	// 制限時間経過
	 stateTimer -= elapsedTime;

	 auto sharedId = owner.lock();
	 if (!sharedId)
		 return;

	 std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	 Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	 std::shared_ptr <HP> hp = sharedId->GetComponent<HP>();
	 // 制限時間過ぎたら
	if (stateTimer <= stateTimerEnd)
	{
		// playerSearch
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Pursuit));
	}

	// 一定ダメージの最大値
	float damageThreshold = 10;
	float timeLimit = 5;

	// 逃走
	if (hp->CheckDamageThresholdWithinTime(elapsedTime, damageThreshold, timeLimit)&&
		randRoot == (int)EnemyBoss::ExitRoot::ExitJamp)
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Jump));
	}

	// ダメージモーション対策
	if (!model->IsPlayAnimation())
	{
		model->PlayAnimation(modelAnim);
	}
}

// 最後にやりたい処理全般
void IdleState::Exit()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;
	std::shared_ptr<HP> hp = sharedId->GetComponent<HP>();
	// 逃げる条件初期化
	hp->ResetOnDamageThresholdTime();
}

// 初期化
void PursuitState::Enter()
{
	// アニメーション設定
	modelAnim.index = EnemyBoss::Animation::Anim_Standby;
	modelAnim.loop = true;
	modelAnim.currentanimationseconds = 0.0f;
	modelAnim.blendSeconds = 0.7f;

	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// エネミーアクセス用
	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	enemyid->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		modelAnim);

	// アニメーションルール
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);

	// 攻撃の種類を出す
	randamAttack = rand() % 3;
}
// update
void PursuitState::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// エネミーアクセス用
	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	// 目標地点ををプレイヤー位置に設定
	DirectX::XMFLOAT3 targetPosition = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();
	enemyid->SetTargetPosition(targetPosition);
	float vx = targetPosition.x - enemyid->GetPosition().x;
	float vy = targetPosition.y - enemyid->GetPosition().y;
	float vz = targetPosition.z - enemyid->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	// 攻撃の種類
	switch (randamAttack)
	{
	case (int)EnemyBoss::AttackMode::AssaultAttack:
	{
		enemyid->GetStateMachine()->ChangeState(
			static_cast<int>(EnemyBoss::State::Wander));
		break;
	}
	case (int)EnemyBoss::AttackMode::JumpStompAttack:
	{
		enemyid->GetStateMachine()->ChangeState(
			static_cast<int>(EnemyBoss::State::Jump));
		break;
	}
	case (int)EnemyBoss::AttackMode::DushAttack:
	{
		enemyid->GetStateMachine()->ChangeState(
			static_cast<int>(EnemyBoss::State::Attack));
		break;
	}
	}
}
// 最後にやりたい処理全般
void PursuitState::Exit()
{
}

// 初期化
void JumpState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	// アニメーション設定
	modelAnim.index = EnemyBoss::Animation::Anim_Attack;
	modelAnim.loop = true;
	modelAnim.currentanimationseconds = 2.0f;
	modelAnim.blendSeconds = 0.7f;
	modelAnim.currentAnimationAddSeconds = 0.01f;

	Model* model =  sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<HP> hpid = sharedId->GetComponent<HP>();
	// 縄張り
	enemyid->SetRandomTargetPosition();
	// アニメーション再生
	model->
		PlayAnimation(modelAnim);
	// アニメーションルール
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Reverseplayback);
	// 着地瞬間
	upOnLading = false;
	// アニメーション終了
	jumpStart = false;
	// 目標地点ををプレイヤー位置に設定
	targetPosition =
		PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();

	// もし逃走するならこっち
	if (hpid->GetIsOverDamageRule())
	{
		targetPosition = { 0,enemyid->GetPosition().y,0 };
		targetPosition.x += Mathf::RandomRange(-enemyid->GetSearchRange(), enemyid->GetSearchRange());
		targetPosition.z += Mathf::RandomRange(-enemyid->GetSearchRange(), enemyid->GetSearchRange());
		// 一定ダメージ確認解除
		bool isOverDamageRule = false;
		hpid->SetIsOverDamageRule(isOverDamageRule);
	}
	// 煙エフェクト
	smorker = std::make_unique<Effect>("Data/Effect/smorkeDash.efk");
}

// 後変更
// 更新処理
void JumpState::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<HP> hpid = sharedId->GetComponent<HP>();
	enemyid->SetTargetPosition(targetPosition);
	DirectX::XMVECTOR targetPositionVec = DirectX::XMLoadFloat3(&targetPosition);
	DirectX::XMVECTOR enemyPositionVec = DirectX::XMLoadFloat3(&enemyid->GetPosition());
	DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(targetPositionVec, enemyPositionVec);
	DirectX::XMFLOAT3 direction;
	DirectX::XMStoreFloat3(&direction, directionVec);
	// 目的地にこの半径入ったら
	float radius = enemyid->GetAttackRightFootRange();
	// 左足ボーン
	Model::Node* bossLeftFoot = model->FindNode("boss_left_foot1");
	// ノード位置取得
	// 左足
	DirectX::XMFLOAT3 bossLeftFootPosition;
	bossLeftFootPosition = model->ConvertLocalToWorld(bossLeftFoot);
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentANimationSeconds();
	// 後変更
	if (animationTime - FLT_EPSILON <= 0.8f + FLT_EPSILON && !jumpStart)
	{
		bool blurCheck = true;
		enemyid->SetBlurCheck(blurCheck);
		enemyid->CollisionPlayerWithRushEnemy();
		moveid->JumpVelocity(jumpSpeed);
		jumpStart = true;
		smorker->Play(bossLeftFootPosition, scaleEffect);
		PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance().Instance();
		// ラジアルブラー
		radialBlurData.radius = 15;
		postprocessingRenderer.SetRadialBlurData(radialBlurData);
		return;
	}
	else
	{
		moveid->Turn(direction, turnSpeed, elapsedTime);
	}

	if (!moveid->GetOnLadius())
	{
		moveid->Move(direction, moveSpeed, elapsedTime);
		moveid->Turn(direction, turnSpeed, elapsedTime);
	}

	if (moveid->GetOnLadius() && jumpStart)
	{
		enemyid->InputImpact(enemyid->GetPosition());
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
		enemyid->InputJump();
		// 煙エフェクト
		smorker->Play(bossLeftFootPosition, scaleEffect);
		PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance().Instance();
		// ラジアルブラー
		radialBlurData.radius = 30;
		postprocessingRenderer.SetRadialBlurData(radialBlurData);
		 // 左足当たり判定
		enemyid->DetectHitByBodyPart(bossLeftFootPosition, applyDamageJumpStart);
	}
}
// 終了処理
void JumpState::Exit()
{
}

// 初期化
void AttackState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<HP> hpid = sharedId->GetComponent<HP>();
	// 縄張り
	enemyid->SetRandomTargetPosition();
	modelAnim.index = EnemyBoss::Animation::Anim_Attack;
	modelAnim.currentanimationseconds = 2.8f;
	modelAnim.loop = false;
	modelAnim.blendSeconds = 0.7f;
	modelAnim.currentAnimationAddSeconds = 0.015f;
	modelAnim.keyFrameEnd = 0.0f;
	// アニメーション再生
	model->
		PlayAnimation(modelAnim);
	// アニメーションルール
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Reverseplayback);
	// アニメーション終了
	dushStart = false;
	// 目標地点ををプレイヤー位置に設定
	targetPosition =
		PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();
	// もし逃走するならこっち
	if (hpid->GetIsOverDamageRule())
	{
		targetPosition = { 0,enemyid->GetPosition().y,0 };
		targetPosition.x += Mathf::RandomRange(-enemyid->GetSearchRange(), enemyid->GetSearchRange());
		targetPosition.z += Mathf::RandomRange(-enemyid->GetSearchRange(), enemyid->GetSearchRange());

		// 一定ダメージ確認解除
		bool isOverDamageRule = false;
		hpid->SetIsOverDamageRule(isOverDamageRule);
	}
	// 煙エフェクト
	smorker = std::make_unique<Effect>("Data/Effect/smorkeDash.efk");
	charge = std::make_unique<Effect>("Data/Effect/effectCharge.efk");
	chargeCompleate = std::make_unique<Effect>("Data/Effect/chargecompleted.efk");
}

// update
void AttackState::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
    std::shared_ptr<HP> hpid = sharedId->GetComponent<HP>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	if (PlayerManager::Instance().GetPlayerCount() >= 1)
	{
		std::shared_ptr<Transform> pTransformId = PlayerManager::Instance().GetPlayer(0)->GetComponent<Transform>();

		targetPosition = pTransformId->GetPosition();
	}
	DirectX::XMVECTOR targetPositionVec = DirectX::XMLoadFloat3(&targetPosition);
	DirectX::XMVECTOR enemyPositionVec = DirectX::XMLoadFloat3(&enemyid->GetPosition());
	DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(targetPositionVec, enemyPositionVec);
	DirectX::XMStoreFloat3(&directionP, directionVec);
	angle = transformid->GetAngle();
	direction.x = sinf(angle.y);// 三角を斜めにして位置を変えた
	direction.y = 0;
	direction.z = cosf(angle.y);
	// 目的地にこの半径入ったら
	float radius = enemyid->GetAttackRightFootRange();
	// ボーン名探す
	Model::Node* bossRightFoot = model->FindNode("boss_right_foot1");
	Model::Node* bossLeftFoot = model->FindNode("boss_left_foot1");
	Model::Node* bossEye = model->FindNode("body2");
	// ノード位置取得
	// 左足
	DirectX::XMFLOAT3 bossRightFootPosition;
	bossRightFootPosition = model->ConvertLocalToWorld(bossRightFoot);
	DirectX::XMFLOAT3 bossLeftFootPosition;
	bossLeftFootPosition = model->ConvertLocalToWorld(bossLeftFoot);
	DirectX::XMFLOAT3 bossEyePosition = model->ConvertLocalToWorld(bossEye);
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentANimationSeconds();
	// チャージ中
	if (animationTime - FLT_EPSILON >= 2.3f + FLT_EPSILON &&
		animationTime - FLT_EPSILON <= 2.4f + FLT_EPSILON && !chargeInitilize)
	{
		enemyid->InputJampSe();
		enemyid->InputChargeSe();
		charge->Play(bossEyePosition);
		chargeCompleate->Play(bossEyePosition);
		// チャージ中
		stateChargeTimer = stateChargeTimerMax;
		chargeInitilize = true;
		// アニメーションルール
		enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Stop);
	}

	// ダッシュするまで
	if (!dushStart)
	{
		stateChargeTimer -= 0.01f;
		moveid->Turn(directionP, turnSpeed, elapsedTime);
	}

	// ダッシュ初期化
	if (chargeInitilize && stateChargeTimer <= stateChargeCompletionTimerEnd && !dushStart)
	{
		modelAnim.index = EnemyBoss::Animation::Anim_Walk;
		modelAnim.currentanimationseconds = 0.0f;
		modelAnim.loop = true;
		modelAnim.currentAnimationAddSeconds = 0.2f;
		modelAnim.keyFrameEnd = 0.0f;
		// アニメーション再生
		model->
			PlayAnimation(modelAnim);
		// アニメーションルール
		enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
		// チャージ完了後のダッシュ時間
		stateChargeCompletionTimer = stateChargeCompletionTimerMax;
	}

	// チャージ時間
	if (chargeInitilize && stateChargeTimer <= stateChargeCompletionTimerEnd)
	{
		dushStart = true;
	}
	
	// ダッシュ中
	if (dushStart)
	{
		enemyid->InputDashSe();
		charge->Stop(charge->GetEfeHandle());
		// 煙エフェクト
		smorker->Play(bossLeftFootPosition, scaleEffect);
		moveid->Move(direction, moveSpeed, elapsedTime);
		stateChargeCompletionTimer -= 0.01f;
		// 右足当たり判定
		enemyid->DetectHitByBodyPart(bossRightFootPosition, applyDamageAttack);
		// 左足当たり判定
		enemyid->DetectHitByBodyPart(bossLeftFootPosition, applyDamageAttack);
	}

	// ダッシュ終了
	if (dushStart && stateChargeCompletionTimer - FLT_EPSILON <= stateChargeCompletionTimerEnd + FLT_EPSILON)
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
	}
}

// 後変更
// 最後にやりたい処理全般
void AttackState::Exit()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	enemyid->InputStopDashSe();
	chargeInitilize = false;
	if (charge->GetEfeHandle())
		charge->Stop(charge->GetEfeHandle());
	if (chargeCompleate->GetEfeHandle())
		chargeCompleate->Stop(chargeCompleate->GetEfeHandle());
}

// 後変更
// ダメージ判定
void DamageState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	enemyid->InputDamageSe();
	modelAnim.index = EnemyBoss::Animation::Anim_Die;
	modelAnim.loop = true;
	modelAnim.currentanimationseconds = 2.5f;
	modelAnim.blendSeconds = 0.7f;
	modelAnim.currentAnimationAddSeconds = 0.025f;
	modelAnim.keyFrameEnd = 3.0f;
	Model* momdel = sharedId->GetComponent<ModelControll>()->GetModel();
	momdel->
		PlayAnimation(
			modelAnim);
	// アニメーションルール
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	stateTimer = stateTimerMax;
}

void DamageState::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	--stateTimer;
	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
		if (stateTimer < stateTimerEnd)
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
}

void DamageState::Exit()
{
}
// 混乱開始
void ConfusionState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = EnemyBoss::Animation::Anim_Die;
	modelAnim.loop = false;
	modelAnim.currentanimationseconds = 0.426f;
	modelAnim.blendSeconds = 0.35f;
	modelAnim.currentAnimationAddSeconds = 0.000f;
	modelAnim.keyFrameEnd = 70.0f;

	model->PlayAnimation(modelAnim);
	// アニメーションルール
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	// ステート待機時間
	stateTimer = stateTimerMax;
	// 攻撃種類
	randamAttack = rand() % 2;
}
// 混乱更新
void ConfusionState::Execute(float elapsedTime)
{
	stateTimer -= elapsedTime;

	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	if (!model->IsPlayAnimation())
		model->PlayAnimation(modelAnim);
	if (stateTimer > stateTimerEnd)return;
	switch (randamAttack)
	{
	case (int)EnemyBoss::AttackMode::AssaultAttack:
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Wander));
		break;
	}
	case (int)EnemyBoss::AttackMode::JumpStompAttack:
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Jump));
		break;
	}
	default:
		break;
	}
}
// 混乱終了
void ConfusionState::Exit()
{
}
// 倒れる
void DeathState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	modelAnim.index = EnemyBoss::Animation::Anim_Die;
	modelAnim.loop = false;
	Model* model;
	model = sharedId->GetComponent<ModelControll>()->GetModel();
	model->
		PlayAnimation(modelAnim);
	// アニメーションルール
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	stateTimer = 3;
	bool freeCheckCamera = false;
	int playerCountMax = PlayerManager::Instance().GetPlayerCount();
	if (playerCountMax > 0)
	PlayerManager::Instance().GetPlayer(playerCountMax - 1)->GetComponent<Player>()->SetFreeCameraCheck(freeCheckCamera);
}

void DeathState::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	stateTimer -= elapsedTime; 
	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	// 死亡時はそれっぽいカメラアングルで死亡
// 例えばコレを必殺技などで上手く利用できればかっこいいカメラ演出が作れますね
	MessageData::CAMERACHANGEMOTIONMODEDATA	p;
	DirectX::XMFLOAT3 position = transformid->GetPosition();
	DirectX::XMFLOAT3 angle = transformid->GetAngle();
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentANimationSeconds();
	float vx = sinf(angle.y) * 6;
	float vz = cosf(angle.y) * 6;
	p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
	p.data.push_back({ 95, {position.x + vx + 13, position.y + 3, position.z + vz }, position });
	Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);
	if (stateTimer < 0)
	{
		enemyid->SetClearCheck(clearCheck);
	}
}

void DeathState::Exit()
{
}

// 興奮状態
void AwakeStartState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = EnemyBoss::Animation::Anim_Movie;
	modelAnim.loop = false;
	modelAnim.currentanimationseconds = 5.829f;
	modelAnim.blendSeconds = 0.35f;
	modelAnim.currentAnimationAddSeconds = 0.025f;
	modelAnim.keyFrameEnd = 3.0f;
	model->
		PlayAnimation(modelAnim);
	// アニメーションルール
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
}

void AwakeStartState::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentANimationSeconds();
	// アニメーションのポーズ
	if (animationTime >= animationPose)
	{
		MessageData::CAMERASHAKEDATA p;
		p.shakePower = cameraShakePower;
		p.shakeTimer = cameraShakeTime;
		Messenger::Instance().SendData(MessageData::CAMERASHAKE, &p);
		// ポストエフェクトインスタンスゲット
		PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
		radialBlurData.radius = 30.0f;
		postprocessingRenderer.SetRadialBlurMaxData(radialBlurData);
	}

	// アニメーション終了
	if (!model->IsPlayAnimation())
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
	}
}

void AwakeStartState::Exit()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	// 覚醒時間
	enemyid->ResetAwakeTime();
}
// クリア倒れて待機時
void ClearState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	// 位置情報
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	DirectX::XMFLOAT3 position = transformid->GetPosition();
	position.z -= 5.5f;
	transformid->SetPosition(position);
	DirectX::XMFLOAT3 angle = transformid->GetAngle();
	angle.y += 1.5f;
	transformid->SetAngle(angle);
	// アニメーションルール
	Model::ModelAnim modelAnim;
	modelAnim.index = EnemyBoss::Animation::Anim_Die;
	modelAnim.loop = false;
	modelAnim.blendSeconds = 0.7f;
	Model* model;
	model = sharedId->GetComponent<ModelControll>()->GetModel();
	model->PlayAnimation(modelAnim);
	// アニメーションルール
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	stateTimer = 3;
}

void ClearState::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	GamePad& gamePad = Input::Instance().GetGamePad();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	stateTimer -= elapsedTime;
	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentANimationSeconds();
	const GamePadButton anyButton =
		GamePad::BTN_B;
	if (!model->IsPlayAnimation() )// ロードの次ゲームという書き方
	{
		enemyid->SetClearCheck(clearCheck);
	}
}

void ClearState::Exit()
{
}
// クリア復活して
void ClearReviveState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	// 位置情報
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	DirectX::XMFLOAT3 position = transformid->GetPosition();
	position.z -= 5.5f;
	transformid->SetPosition(position);
	DirectX::XMFLOAT3 angle = transformid->GetAngle();
	angle.y += 1.5f;
	transformid->SetAngle(angle);
	modelAnim.index = EnemyBoss::Animation::Anim_Die;
	modelAnim.loop = false;
	modelAnim.currentanimationseconds = 1.5f;
	modelAnim.blendSeconds = 0.7f;
	Model* model;
	model = sharedId->GetComponent<ModelControll>()->GetModel();
	model->
		PlayAnimation(modelAnim);
	// アニメーションルール
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	stateTimer = 3;
}

void ClearReviveState::Execute(float elapsedTime)
{
}

void ClearReviveState::Exit()
{
}

// プレイヤー待機
void PlayerIdleState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Anim_Idle;
	modelAnim.loop = true;
	modelAnim.blendSeconds = 0.5f;
	model->PlayAnimation(modelAnim);
	// アニメーションルール
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	moveid->SetStopMove(false);
}

void PlayerIdleState::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	// ロックオン処理
	playerid->UpdateCameraState(elapsedTime);
	// 移動入力処理
	if (playerid->InputMove())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	}
	// 回避入力処理
	if (playerid->InputAvoidance())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}
	// ジャンプ入力処理
	if (playerid->InputJump())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
	}
}

void PlayerIdleState::Exit()
{
}
// プレイヤー移動
void PlayerMovestate::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	// se再生
	audioParam.filename = "Data/Audio/SE/足音.wav";
	audioParam.loop = loopSe;
	audioParam.volume = volumeSe;
	playerid->InputSe(audioParam);

	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Anim_Running;
	modelAnim.loop = true;
	modelAnim.blendSeconds = 0.7f;
	modelAnim.currentAnimationAddSeconds = 0.025f;
	model->PlayAnimation(modelAnim);
	// アニメーションルール
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	// 落ちる
	moveid->SetStopMove(false);
}

void PlayerMovestate::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	// 移動値
	playerid->GetMoveVec(elapsedTime);
	// ロックオン処理
	playerid->UpdateCameraState(elapsedTime);
	// 移動入力処理
	if (!playerid->InputMove())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
	}
	// 回避入力処理
	if (playerid->InputAvoidance())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}
	// ジャンプ入力処理
	if (playerid->InputJump())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
	}
}

void PlayerMovestate::Exit()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	//se再生削除
	playerid->StopSe(audioParam);
}

// プレイヤージャンプ
void PlayerJumpState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Animation::Anim_Jump;
	modelAnim.keyFrameEnd = 46.0f;
	model->PlayAnimation(modelAnim);
	// アニメーションルール
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	// 落ちる
	moveid->SetStopMove(false);
	// se作成
	playerid->InputJampSE();
}

void PlayerJumpState::Execute(float elapsedTime)
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	// ロックオン処理
	playerid->UpdateCameraState(elapsedTime);
	// 移動
	if (playerid->InputMove())
	{
		playerid->GetMoveVec(elapsedTime);
	}
	// ジャンプ入力処理
	if (playerid->InputJump())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
	}
	// 回避
	if (playerid->InputAvoidance())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}
	// 着地
	if (moveid->GetOnLadius())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Land));
	}
	// 着地移動
	if (moveid->GetOnLadius() && playerid->InputMove())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	}
}

void PlayerJumpState::Exit()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	// ジャンプ音
	playerid->InputJampSE();
}
// プレイヤー着地
void PlayerLandState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Anim_Land;
	modelAnim.currentanimationseconds = 0.35f;
	model->PlayAnimation(modelAnim);
	// アニメーションルール
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	// se作成
	playerid->InputJampSE();
}

void PlayerLandState::Execute(float elapsedTime)
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	// ロックオン処理
	playerid->UpdateCameraState(elapsedTime);
	// 歩き
	if (playerid->InputMove())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	}
	// 回避
	if (playerid->InputAvoidance())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}
	// 待機
	if (!model->IsPlayAnimation())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
	}
}

void PlayerLandState::Exit()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	// ジャンプ音
	playerid->InputJampSE();
}

// 後変更
// プレイヤージャンプ中
void PlayerJumpFlipState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Anim_Jump;
	modelAnim.currentAnimationAddSeconds = 0.03f;
	modelAnim.keyFrameEnd = 25.0f;
	model->PlayAnimation(modelAnim);
	// アニメーションルール
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	// se作成
	playerid->InputJampSE();
}

void PlayerJumpFlipState::Execute(float elapsedTime)
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	// ロックオン処理
	playerid->UpdateCameraState(elapsedTime);
	// 移動
	if (playerid->InputMove())
	{
		bool afterimagemove = true;
	}
	// ジャンプ入力処理
	if (!model->IsPlayAnimation())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
	}
	// 回避
	if (playerid->InputAvoidance())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}
}

void PlayerJumpFlipState::Exit()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	playerid->InputStopJampSE();
}
// プレイヤー初撃
void PlayerQuickJabState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	tellePort = std::make_unique<Effect>("Data/Effect/telleport.efk");

	if (!InitializationCheck)
	{
		// 斬撃二回目ボタンB
		commandSecondeButtonB.push_back(GamePad::BTN_B);
		commandSecondeButtonB.push_back(GamePad::BTN_B);
		// 斬撃二回目ボタン十字キー
		commandSecondeButtonRight.push_back(GamePad::BTN_RIGHT);
		commandSecondeButtonRight.push_back(GamePad::BTN_RIGHT);
	}
	
	// 重力オフ
	bool stopFall = true;
	moveid->SetStopFall(stopFall);

	// 回転するかチェック
	rotateCheck = false;

	// ダメージ食らった時に攻撃を１からに戻すため
	deleteCheck = true;

	InitializationCheck = true;

	// 描画
	isPlayerDrawCheck = 0;

	// ホーミングするか
	isHomingStartCheck = true;

	// エネミー非接触
	isEnemyHit = false;
	// 接触
	EnemyHit = true;
	// 非接触
	EnemySafe = false;
}

// 後変更
void PlayerQuickJabState::Execute(float elapsedTime)
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	GamePad& gamePad = Input::Instance().GetGamePad();
	// コンポネント取得
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	// ロックオン処理
	playerid->UpdateCameraState(elapsedTime);

	if (playerid->InputAttack() &&
		playerid->GetSelectCheck() ==
		(int)Player::CommandAttack::Attack)
	{
		// コマンド確認2弾攻撃
		if (gamePad.ConfirmCommand(commandSecondeButtonB, frame) && !button && !buttonSeconde)
		{
			button = true;
		}

		if (gamePad.ConfirmCommand(commandSecondeButtonRight, frame) && !button && !buttonSeconde)
		{
			button = true;
		}
	}

	// 回避
	if (playerid->InputAvoidance() && playerid->InputMove())
	{
		bool stop = false;
		moveid->SetStopMove(stop);
		DirectX::XMFLOAT3 vec;
		vec = playerid->GetMoveVec(elapsedTime);
		moveid->Turn(vec, turnSpeed, elapsedTime);
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
		button = false;
		buttonSeconde = false;
		return;
	}
	DirectX::XMVECTOR Vector;
	DirectX::XMVECTOR VectorXZ;
	DirectX::XMVECTOR LengthSq;
	playerPosXZ.x = transformid->GetPosition().x;
	playerPosXZ.y = transformid->GetPosition().z;
	DirectX::XMVECTOR playerPositionXZ = DirectX::XMLoadFloat2(&playerPosXZ);
	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid->GetPosition());
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)//float 最大値ないにいる敵に向かう
	{
		enemyPosXZ.x = enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition().x;
		enemyPosXZ.y = enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition().z;
		enemyPos = enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition();
		DirectX::XMVECTOR enemyPositionXZ = DirectX::XMLoadFloat2(&enemyPosXZ);
		DirectX::XMVECTOR enemyPosition = DirectX::XMLoadFloat3(&enemyPos);

		VectorXZ = DirectX::XMVectorSubtract(enemyPositionXZ, playerPositionXZ);
		Vector = DirectX::XMVectorSubtract(enemyPosition, playerPosition);

		LengthSq = DirectX::XMVector2Length(VectorXZ);

		Vector = DirectX::XMVector3Normalize(Vector);

		DirectX::XMStoreFloat3(&vector, Vector);

		DirectX::XMStoreFloat(&length, LengthSq);
	}

	// 攻撃量最大
	if (attackMemory > attackMemoryMax ||!model->IsPlayAnimation())
	{
		// 入力確認でステート変更
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
		// 攻撃最大値調整
		attackMemory = 0;
	}

	// 相手の方を向く
	// 回転
	if (!rotateCheck)
	{
		isEnemyHit = !playerid->GetIsEnemyHit() && !playerid->GetIsEnemyHitBody() ?
			EnemySafe : EnemyHit;
		// 距離
		if (length < attackCheckRange && !isEnemyHit)
		{
			bool stop = false;
			moveid->SetStopMove(stop);
			// 消す
			isPlayerDrawCheck = 0;
			playerid->SetPlayeDrawCheck(isPlayerDrawCheck);
			// 正面
			moveid->Turn(vector, turnSpeed, elapsedTime);
			moveid->Move(vector, speed, elapsedTime);

			// ホーミング一回のみ再生
			if (isHomingStartCheck)
			{
				// Se再生
				playerid->PlayTellePortSe();
				// エフェクト
				tellePort->Play(transformid->GetPosition());
				// ホーミング一回のみのため
				isHomingStartCheck = false;
			}

			return;
		}
		else
		{
			// 描画
			isPlayerDrawCheck = 1;
			playerid->SetPlayeDrawCheck(isPlayerDrawCheck);
			bool stop = true;
			moveid->SetStopMove(stop);
			rotateCheck = true;
			// ホーミング終了
			isHomingStartCheck = false;

			// アニメーション再生
			// 空中
			if (!moveid->GetOnLadius())
			{
				//// 空中攻撃で行動階数減少
				//playerid->AreAttackDecreaseAmount();
				// アニメーションルール
				Model::ModelAnim modelAnim;
				Model::ModelAnim modelAnimUpperBody;
				modelAnim.index = Player::Anim_Jump;
				modelAnimUpperBody.index = Player::Anim_Slash;
				modelAnimUpperBody.currentAnimationAddSeconds = 0.030f;
				modelAnim.loop = true;
				modelAnim.currentAnimationAddSeconds = 0.03f;
				// アニメーション再生
				model->PlayAnimation(modelAnim);
				// アニメーション上半身再生
				model->PlayUpeerBodyAnimation(modelAnimUpperBody);
				// 攻撃三回で一度強制終了
				++attackMemory;
				// アニメーション再生
				playerid->SetUpdateAnim(Player::UpAnim::Doble);
			}
			// 地面
			else
			{
				// アニメーションルール
				Model::ModelAnim modelAnim;
				modelAnim.index = Player::Animation::Anim_Slash;
				modelAnim.currentAnimationAddSeconds = 0.031f;
				// アニメーション再生
				model->PlayAnimation(modelAnim);
				// 攻撃三回で一度強制終了
				++attackMemory;
				// アニメーション再生
				playerid->SetUpdateAnim(Player::UpAnim::Normal);
			}
		}
	}

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = moveid->GetOnLadius() ? 
		model->GetCurrentANimationSeconds() : 
		model->GetCurrentAnimationSecondsUpeer();
	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。

	// 1撃目
	if (animationTime >= 0.8f  && playerid->InputAttack())
	{
		// １回目の攻撃なら
		oneAttackCheck = false;
		playerid->SetAttackCollisionFlag(false);
		deleteCheck = false;
		// 入力確認でステート変更
		playerid->GetStateMachine()->ChangeState(static_cast<int>(button  ? Player::State::SideCut : Player::State::Move));
		return;
	}
	// 斬撃の当たり判定
	playerid->CollisionNodeVsEnemies("mixamorig:LeftHand",
		playerid->GetLeftHandRadius(),
		"body2", "boss_left_hand2", "boss_right_hand2");
}

void PlayerQuickJabState::Exit()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	// 移動の停止
	moveid->SetGravity(
		playerid->GetGravity());

	// 歩き許可
	bool stopMove = false;
	moveid->SetStopMove(stopMove);

	// 重力オフ
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
	if (deleteCheck )
	{
		// ダメージ食らったかどうか
		button = false;
		buttonSeconde = false;
		// 攻撃最大値調整
		attackMemory = 0;
	}

	// 描画
	isPlayerDrawCheck = 1;
	// 非表示の場合表示する
	playerid->SetPlayeDrawCheck(isPlayerDrawCheck);
}

// 2連撃目
void PlayerSideCutState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	bool loop = false;
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	if (!moveid->GetOnLadius())
	{
		// アニメーションルール
		Model::ModelAnim modelAnim;
		Model::ModelAnim modelAnimUpperBody;
		modelAnim.index = Player::Animation::Anim_Jump;
		modelAnimUpperBody.index = Player::Animation::Anim_SlashBeside;

		// 再生開始時間 
		modelAnim.currentanimationseconds = 0.6f;
		// ループ
		bool loop = true;
		modelAnim.loop = true;
		// 再生時間加算分の値
		modelAnim.currentAnimationAddSeconds = 0.03f;
		modelAnimUpperBody.currentAnimationAddSeconds = 0.03f;
		modelAnim.keyFrameEnd = 0.63f;
		// アニメーション再生
		model->PlayAnimation(modelAnim);
		// アニメーション上半身再生
		model->PlayUpeerBodyAnimation(modelAnimUpperBody);
		// 攻撃三回で一度強制終了
		++attackMemory;
		// アニメーション再生
		playerid->SetUpdateAnim(Player::UpAnim::Doble);
	}
	else
	{
		// アニメーションルール
		Model::ModelAnim modelAnim;
		modelAnim.index = Player::Animation::Anim_SlashBeside;
		modelAnim.currentanimationseconds = 0.2f;
		modelAnim.currentAnimationAddSeconds = 0.03f;
		// アニメーション再生
		model->PlayAnimation(modelAnim);
		// 攻撃三回で一度強制終了
		++attackMemory;
		// アニメーション再生
	    playerid->SetUpdateAnim(Player::UpAnim::Normal);
	}
		
	if (!InitializationCheck)
	{
		//// コマンド設定二回目攻撃
		//commandSeconde.push_back(GamePad::BTN_B);
		// コマンド設定三回目攻撃
		commandThredeButtonB.push_back(GamePad::BTN_B);
		commandThredeButtonRight.push_back(GamePad::BTN_RIGHT);
	}
	// 重力オフ
	bool stopFall = true;
	moveid->SetStopFall(stopFall);
	// 回転するかチェック
	rotateCheck = false;
	InitializationCheck = true;
}

void PlayerSideCutState::Execute(float elapsedTime)
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	GamePad& gamePad = Input::Instance().GetGamePad();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	// ロックオン処理
	playerid->UpdateCameraState(elapsedTime);

	// 連続ボタン確認
	if (playerid->InputAttack() &&
		playerid->GetSelectCheck() ==
		(int)Player::CommandAttack::Attack)
	{
		// コマンド確認3段攻撃 ボタンB
		if (gamePad.ConfirmCommand(commandThredeButtonB, frame))
		{
			buttonSeconde = true;
		}
		// コマンド確認3段攻撃 十字キー右
		if (gamePad.ConfirmCommand(commandThredeButtonRight, frame))
		{
			buttonSeconde = true;
		}
	}
	// 回避
	if (playerid->InputAvoidance() && playerid->InputMove())
	{
		bool stop = false;
		moveid->SetStopMove(stop);
		DirectX::XMFLOAT3 vec;
		vec = sharedId->GetComponent<Player>()->GetMoveVec(elapsedTime);
		moveid->Turn(vec, turnSpeed, elapsedTime);
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
		button = false;
		buttonSeconde = false;
		return;
	}
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = moveid->GetOnLadius() ?
		model->GetCurrentANimationSeconds() :
		model->GetCurrentAnimationSecondsUpeer();
	// 2撃目
	if (animationTime >= 1.1f)
	{
		playerid->SetAttackCollisionFlag(false);
		deleteCheck = false;
		// 入力確認でステート変更
		playerid->GetStateMachine()->ChangeState(static_cast<int>(buttonSeconde ? Player::State::CycloneStrike : Player::State::Move));
		return;
	}
	// 攻撃当たり判定
	playerid->CollisionNodeVsEnemies("mixamorig:LeftHand"
		, playerid->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");
}

void PlayerSideCutState::Exit()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	// 移動の停止
	moveid->SetGravity(
		playerid->GetGravity());
	// 歩き許可
	bool stopMove = false;
	moveid->SetStopMove(stopMove);
	// 重力オフ
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
}
// 3撃目
void PlayerCycloneStrikeState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	bool loop = false;
	// コンポネント取得
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	if (!moveid->GetOnLadius())
	{
	    // アニメーションルール
		Model::ModelAnim modelAnimUpper;
		modelAnimUpper.index = Player::Animation::Anim_SlashThree;
		modelAnimUpper.currentanimationseconds = 0.03f;
		// アニメーション上半身再生
		model->PlayUpeerBodyAnimation(modelAnimUpper);
		// 攻撃三回で一度強制終了
		++attackMemory;
		// アニメーション再生
		playerid->SetUpdateAnim(Player::UpAnim::Doble);
	}
	else
	{
		// アニメーションルール
		Model::ModelAnim modelAnim;
		modelAnim.index = Player::Animation::Anim_SlashThree;
		modelAnim.currentanimationseconds = 0.7f;
		modelAnim.currentAnimationAddSeconds = 0.03f;
		// アニメーション再生
		model->PlayAnimation(modelAnim);
		// 攻撃三回で一度強制終了
		++attackMemory;
		// アニメーション再生
		playerid->SetUpdateAnim(Player::UpAnim::Normal);
	}
	// 重力オフ
	bool stopFall = true;
	moveid->SetStopFall(stopFall);
	// 回転するかチェック
	rotateCheck = false;
	InitializationCheck = true;
}

void PlayerCycloneStrikeState::Execute(float elapsedTime)
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	GamePad& gamePad = Input::Instance().GetGamePad();
	// コンポネント取得
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	// ロックオン処理
	playerid->UpdateCameraState(elapsedTime);
	// 回避
	if (playerid->InputAvoidance() && playerid->InputMove())
	{
		bool stop = false;
		moveid->SetStopMove(stop);
		DirectX::XMFLOAT3 vec;
		vec = playerid->GetMoveVec(elapsedTime);
		moveid->Turn(vec, turnSpeed, elapsedTime);
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
		button = false;
		buttonSeconde = false;
		return;
	}
	DirectX::XMVECTOR Vector;
	DirectX::XMVECTOR LengthSq;
	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid->GetPosition());
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)//float 最大値ないにいる敵に向かう
	{
		DirectX::XMVECTOR enemyPosition = DirectX::XMLoadFloat3(&enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition());
		Vector = DirectX::XMVectorSubtract(enemyPosition, playerPosition);
		LengthSq = DirectX::XMVector3Length(Vector);
		DirectX::XMStoreFloat3(&vector, Vector);
		DirectX::XMStoreFloat(&length, LengthSq);
	}
	if (length > attackCheckRangeMin)
	{
		bool stop = false;
		moveid->SetStopMove(stop);
		DirectX::XMFLOAT3 angle = transformid->GetAngle();
		DirectX::XMFLOAT3 direction;
		direction.x = sinf(angle.y) * 6;
		direction.y = 0;
		direction.z = cosf(angle.y) * 6;
	}
	else if (!moveid->GetOnLadius())
	{
		bool stop = true;
		moveid->SetStopMove(stop);
	}
	// 回転
	if (!rotateCheck)
	{
			// 距離
			if (length < attackCheckRange && length > attackCheckRangeMin
				&& moveid->TurnCheck(
					vector, angleRange, elapsedTime))
			{
				// 正面
				if (moveid->Turn(vector, turnSpeed, elapsedTime))
				{

					moveid->Move(vector, speed, elapsedTime);

				}
			}
			else
			{
				rotateCheck = true;
			}
	}
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = moveid->GetOnLadius() ?
		model->GetCurrentANimationSeconds() :
		model->GetCurrentAnimationSecondsUpeer();
	// 3撃目
	if (animationTime >= 2.7f )
	{
		playerid->SetAttackCollisionFlag(false);
		deleteCheck = false;
		// 入力確認でステート変更
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
		return;
	}
	// 当たり判定
	playerid->CollisionNodeVsEnemies("mixamorig:LeftHand",
		playerid->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");
}

void PlayerCycloneStrikeState::Exit()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// コンポネント取得
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	// 移動の停止
	moveid->SetGravity(
		playerid->GetGravity());
	// 歩き許可
	bool stopMove = false;
	moveid->SetStopMove(stopMove);
	// 重力オフ
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
}
// 特殊技斬撃
void PlayerSpecialAttackState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// コンポネント取得
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	loopSe = false;
	std::shared_ptr<Transform> enemyTransform = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<Transform>();
	std::shared_ptr<HP> enemyHpId = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<HP>();
	lightning = std::make_unique<Effect>("Data/Effect/sunder.efk");
	lightningAttack = std::make_unique<Effect>("Data/Effect/HitThunder.efk");
	lightningHit = std::make_unique<Effect>("Data/Effect/lightningStrike.efk");
	if (!moveid->GetOnLadius())
	{
		// アニメーションルール
		Model::ModelAnim modelAnim;
		Model::ModelAnim modelAnimUpperBody;
		modelAnim.index = Player::Animation::Anim_Jump;
		modelAnimUpperBody.index = Player::Animation::Anim_MagicSeconde;
		modelAnim.loop = true;
		modelAnim.keyFrameEnd = 0.63f;
		// アニメーション再生
		model->PlayAnimation(modelAnim);
		// アニメーション上半身再生
		model->PlayUpeerBodyAnimation(modelAnimUpperBody);
		// アニメーション上半身下半身再生
		playerid->SetUpdateAnim(Player::UpAnim::Doble);
	}
	else
	{
		// 再生ループ
		bool  loop = false;
		// 再生開始時間 
		float currentAnimationStartSeconds = 0.0f;
		// アニメーションブレンド
		float blendSeconds = 0.5f;
		// アニメーションルール
		Model::ModelAnim modelAnim;
		modelAnim.index = Player::Animation::Anim_MagicSeconde;
		// アニメーション再生
		model->PlayAnimation(modelAnim);
		// アニメーション再生
		playerid->SetUpdateAnim(Player::UpAnim::Normal);
	}
	Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");
	DirectX::XMFLOAT3 pPosition =
	{
				pHPosiiton->worldTransform._41,
				pHPosiiton->worldTransform._42,
				pHPosiiton->worldTransform._43
	};
	lightning->Play(pPosition);
	// 落ちるの停止
	bool stopFall = true;
	moveid->SetStopFall(stopFall);
	button = true;
	// フラッシュ
	flashOn = true;
	playerid->InputAttackThanderSE();
	// 回転許可
	isRotate = true;
	bool specialRockOff = true;
	playerid->SetSpecialRockOff(specialRockOff);
}

void PlayerSpecialAttackState::Execute(float elapsedTime)
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<HP> enemyHpId ;
	// エネミーの前に向く
	{
		DirectX::XMVECTOR Vector;
		DirectX::XMVECTOR LengthSq;
		DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid->GetPosition());
		EnemyManager& enemyManager = EnemyManager::Instance();
		int enemyCount = enemyManager.GetEnemyCount();
		for (int i = 0; i < enemyCount; ++i)//float 最大値ないにいる敵に向かう
		{
			DirectX::XMVECTOR enemyPosition = DirectX::XMLoadFloat3(&enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition());
			enemyHpId = enemyManager.GetEnemy(i)->GetComponent<HP>();
			Vector = DirectX::XMVectorSubtract(enemyPosition, playerPosition);
			LengthSq = DirectX::XMVector3Length(Vector);
			Vector = DirectX::XMVector3Normalize(Vector);
			DirectX::XMStoreFloat3(&vector, Vector);
			DirectX::XMStoreFloat(&length, LengthSq);
		}
		// 相手の方を向く
		// 回転
		if (isRotate)
		{
			if (moveid->Turn(vector, turnSpeed, elapsedTime))
			{
				// カメラ向き
				{
					MessageData::CAMERACHANGEMOTIONMODEDATA	p;
					position = transformid->GetPosition();
					angle = transformid->GetAngle();
					float vx = sinf(angle.y) * 6.0f;
					float vz = cosf(angle.y) * 6.0f;
					float vx2 = sinf(angle.y) - 10.0f;
					float vz2 = cosf(angle.y) * 7.0f;
					float vx3 = sinf(angle.y);
					p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
					p.data.push_back({ 50, {position.x + vx, position.y + 3, position.z + vz }, position });
					p.data.push_back({ 100, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
					p.data.push_back({ 140, {position.x + vx3 , position.y + 1, (position.z + 0.1f) - vz2 }, position });
					// エネミー呼ぶ奴
					EnemyManager& enemyManager = EnemyManager::Instance();
					int enemyManagerCount = enemyManager.GetEnemyCount();
					DirectX::XMFLOAT3 pos;
					DirectX::XMVECTOR posVec;
					pos =
					{ 0,0,0 };
					float length;
					float endLength = 8.0f;
					length = 10.0f;
					//　カメラ移動　ベクトル獲得
					if (enemyManagerCount > 0)
					{
						std::shared_ptr<EnemyBoss> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemyBoss>();
						DirectX::XMVECTOR positionVec = DirectX::XMLoadFloat3(&position);
						DirectX::XMVECTOR ePositionVec = DirectX::XMLoadFloat3(&enemy->GetPosition());
						posVec = DirectX::XMVectorSubtract(positionVec, ePositionVec);
						posVec = DirectX::XMVector3Normalize(posVec);
						DirectX::XMStoreFloat3(&pos, posVec);
					}
					p.data.push_back({ 190, {position.x + (pos.x * length) ,
						position.y + (pos.y * length) + 1,
						position.z + (pos.z * length) }, position });
					Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);
				}
			}
			//　必殺技の対象に向いた後
			else
			{
				isRotate = false;
			}
		}
	}
	if (button)
	{
		// 任意のアニメーション再生区間でのみ衝突判定処理をする
		float animationTime = moveid->GetOnLadius() ?
			model->GetCurrentANimationSeconds() :
			model->GetCurrentAnimationSecondsUpeer();
		// アニメーション
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON)
		{
			if (!moveid->GetOnLadius())
			{
				// アニメーションルール
				Model::ModelAnim modelAnim;
				Model::ModelAnim modelAnimUpperBody;
				modelAnim.index = Player::Animation::Anim_Jump;
				modelAnimUpperBody.index = Player::Animation::Anim_SpecialAttack;
				modelAnim.loop = true;
				modelAnim.keyFrameEnd = 0.63f;
				// アニメーション再生
				model->PlayAnimation(modelAnim);
				// アニメーション上半身再生
				model->PlayUpeerBodyAnimation(modelAnimUpperBody);
			}
			else
			{
				// アニメーションルール
				Model::ModelAnim modelAnim;
				modelAnim.index = Player::Animation::Anim_SpecialAttack;
				// アニメーション再生
				model->PlayAnimation(modelAnim);
			}
			button = false;
			lightning->Stop(lightning->GetEfeHandle());
			loopSe = true;
			// 雷停止
			playerid->InputStopAttackThanderSE();
		}
	}
	else
	{
			// 任意のアニメーション再生区間でのみ衝突判定処理をする
			float animationTime = moveid->GetOnLadius() ?
				model->GetCurrentANimationSeconds() :
				model->GetCurrentAnimationSecondsUpeer();
		// スローモーションと色を明るく
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.5f + FLT_EPSILON)
		{
			PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
			ColorGradingData colorGradingData;
			colorGradingData.brigthness = 1.5f;
			colorGradingData.saturation = 0.0f;
			float colorGradingAdd = 0.01f;
			float radialAdd = 0.01f;
			postprocessingRenderer.StepValueColor(colorGradingAdd);
			postprocessingRenderer.StepValueRadial(radialAdd);
			postprocessingRenderer.SetColorGradingMaxData(colorGradingData);
			postprocessingRenderer.SetColorGradingMinData(colorGradingData);
			playerid->SetHitCheck(true);
		}
		// アニメーション切りつけ
		if (animationTime >= 1.6f)
		{
			PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
			ColorGradingData colorGradingData;
			colorGradingData.brigthness = 5.5f;
			RadialBlurData radialBlurData;
			radialBlurData.radius = 500;
			radialBlurData.mask_radius = 300;
			radialBlurData.samplingCount = 10;
			postprocessingRenderer.SetColorGradingMaxData(colorGradingData);
			postprocessingRenderer.SetRadialBlurMaxData(radialBlurData);
			float value = 5;
			postprocessingRenderer.StepValueRadial(value);
			// カメラ振動
			MessageData::CAMERASHAKEDATA cameraShakeData;
			float shakeTimer = 0.5f;
			float shakePower = 0.8f;
			cameraShakeData = { shakeTimer , shakePower };
			Messenger::Instance().SendData(MessageData::CAMERASHAKE, &cameraShakeData);
			Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");
			DirectX::XMFLOAT3 pPosition =
			{
						pHPosiiton->worldTransform._41,
						pHPosiiton->worldTransform._42,
						pHPosiiton->worldTransform._43
			};
			lightningAttack->Play(pPosition);
			// 必殺技音
			playerid->InputAttackSlashSpecileLightningStrikeSE();
			if (enemyHpId->ApplyDamage(10, 0.5f)) 
			{
				lightningHit->Play(pPosition);
			}
			playerid->SetHitCheck(false);
		}
		bool animationEnd = moveid->GetOnLadius() ?
			model->IsPlayAnimation() :
			model->IsPlayUpeerBodyAnimation();
		// アニメーション終了
		if (!animationEnd)
		{
			// 入力確認でステート変更
			playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));

		}
	}
}

void PlayerSpecialAttackState::Exit()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	// 落ちるの再開
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
	// エネミー呼ぶ奴
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyManagerCount = enemyManager.GetEnemyCount();
	// 動作させるかどうか
	if (enemyManagerCount > 0)
	{
		std::shared_ptr<EnemyBoss> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemyBoss>();
		std::shared_ptr<Movement> enemyMove = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<Movement>();
		bool moveCheck = true;
		enemy->SetMoveCheck(moveCheck);
		// 速度停止
		bool stopVelocity = false;
		enemyMove->SetStopMove(stopVelocity);
		// 落ちるの停止
		bool stopFall = false;
		enemyMove->SetStopFall(stopFall);
	}
	PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
	// 最低値設定
	postprocessingRenderer.StepValueMin();
    ColorGradingData colorGradingData;
	postprocessingRenderer.SetColorGradingMinData(colorGradingData);
	bool specialRockOff = false;
	playerid->SetSpecialRockOff(specialRockOff);
}
// 特殊技火魔法
void PlayerMagicState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	std::shared_ptr<Mp> mpId = sharedId->GetComponent<Mp>();

	// se再生
	seParam.filename = "Data/Audio/SE/チャージ音敵.wav";
	seParam.volume = 0.5f;
	playerid->InputSe(seParam);

	// エフェクト
	charge = std::make_unique<Effect>("Data/Effect/magicCharge.efk");
	chargeComplet = std::make_unique<Effect>("Data/Effect/magicChargeComplet.efk");
	
	magicType = playerid->GetSelectMagicCheck();
	switch (magicType)
	{
		case ((int)Player::CommandMagic::Heale):
		{
			modelAnim.index = Player::Animation::Anim_MagicSeconde;
			modelAnim.keyFrameEnd = 0.0f;
			// アニメーション再生
			model->PlayAnimation(modelAnim);
			// エフェクト回復
			heale = std::make_unique<Effect>("Data/Effect/heale.efk");
			// エフェクト再生
			heale->Play(transformid->GetPosition());

			// 魔法チャージ完了
			magicCharge = magicChargeMax;
			break;
		}
		default:
		{
			modelAnim.index = Player::Animation::Anim_MagicArch;
			// 魔法ため
			magicCharge = magicChargeEnd;

			// アニメーション再生
			model->PlayAnimation(modelAnim);

			// エフェクト再生
			charge->Play(transformid->GetPosition());
			break;
		}
	}
	// アニメーション再生
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	// 落ちるの停止
	bool stopFall = true;
	moveid->SetStopFall(stopFall);
	// 回転許可
	isRotate = true;
	// 試し
	// 移動許可
	isMove = false;

	// 魔法開始
	magicStart = false;

	// 回転速度
	rotationSpeed = DirectX::XM_2PI;

	// 角度どうするか
	isAngleAddX = true;
	isAngleAddY = true;

	// 角度
	angle = transformid->GetAngle();

	angle.x += DirectX::XMConvertToRadians(20);
	angle.y -= DirectX::XMConvertToRadians(60);
}

void PlayerMagicState::Execute(float elapsedTime)
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	// ロックオン処理
	playerid->UpdateCameraState(elapsedTime);
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentANimationSeconds();

	// 試し
	// 魔法ため
	if (magicCharge < magicChargeMax && animationTime >= 1.6f &&
		playerid->InputMagick())
	{
		// アニメーション際再生
		{
			modelAnim.index = Player::Animation::Anim_MagicArch;
			// アニメーション再生
			model->PlayAnimation(modelAnim);
		}
		// 魔法チャージ完了
		magicCharge = magicChargeMax;

		// チャージエフェクト停止
		if (charge->GetEfeHandle())
			charge->Stop(charge->GetEfeHandle());

		// チャージ完了
		chargeComplet->Play(transformid->GetPosition());
	}

	// 魔法ため終了
	if (magicCharge >= magicChargeMax || 
		!playerid->InputMagick() && 
		!magicStart)
	{
		// se停止
		playerid->StopSe(seParam);

		// エフェクト再生してたら停止
		if (charge->GetEfeHandle())
		{
			// チャージエフェクト停止
			charge->Stop(charge->GetEfeHandle());

			// チャージ完了
			chargeComplet->Play(transformid->GetPosition());
		}

		// 魔法種類
		switch (magicType)
		{
		case (int)Player::CommandMagic::Fire:
		{
			modelAnim.index = Player::Animation::Anim_Magic;
			modelAnim.keyFrameEnd = 0.0f;
			// アニメーション再生
			model->PlayAnimation(modelAnim);
			break;
		}

		case (int)Player::CommandMagic::Ice:
		{
			modelAnim.index = Player::Animation::Anim_Magic;
			modelAnim.keyFrameEnd = 0.0f;
			// アニメーション再生
			model->PlayAnimation(modelAnim);
			break;
		}

		case ((int)Player::CommandMagic::Thander):
		{
			modelAnim.index = Player::Animation::Anim_MagicSeconde;
			modelAnim.keyFrameEnd = 0.0f;
			// アニメーション再生
			model->PlayAnimation(modelAnim);
			break;
		}

		default:
			break;
		}

		//　魔法発射
		magicStart = true;
	}

	if (!magicStart) return;

	// アニメーション終わったら通る
	DirectX::XMVECTOR vectorXM;
	DirectX::XMVECTOR lengthSqXM;
	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid->GetPosition());
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)//float 最大値ないにいる敵に向かう
	{
		DirectX::XMVECTOR enemyPosition = DirectX::XMLoadFloat3(&enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition());
		vectorXM = DirectX::XMVectorSubtract(enemyPosition, playerPosition);
		lengthSqXM = DirectX::XMVector3Length(vectorXM);
		vectorXM = DirectX::XMVector3Normalize(vectorXM);
		DirectX::XMStoreFloat3(&vector, vectorXM);
		DirectX::XMStoreFloat(&length, lengthSqXM);
	}

	// 相手の方を向く
	// 回転
	if (isRotate)
	{
		// 距離
		if (moveid->TurnCheck(
				vector, angleRange, elapsedTime) &&
			playerid->GetRockCheck())
		{
			// 正面
			moveid->Turn(vector,turnSpeed,elapsedTime);
		}
		else
		{
			isRotate = false;
		}
	}

	// UI処理の初期化
	playerid->StartMagicUiFire();
	// 魔法の種類で終わりを帰る。
	switch (magicType)
	{
	case (int)Player::CommandMagic::Fire:
	{
		// 時間
		if (animationTime <= 0.5f)return;

		// 角度下に
		if (angle.x > angleMax)
			isAngleAddX = false;

		// 角度上に
		if (angle.x < angleMin)
			isAngleAddX = true;

		// 角度下に
		if (!isAngleAddX)
		{
			angle.x -= rotationSpeed * angleAddX;
		}
		// 角度上に
		else
		{
			angle.x += rotationSpeed * angleAddX;
		}

		angle.y += rotationSpeed * angleAddY;

		// １発
		if (magicCharge <= magicChargeEnd)
		{
			// 角度
			angle = transformid->GetAngle();
			playerid->InputMagicframe();
		}
		// 複数
		else
		{
			// 炎発射
			playerid->PushMagicFrame(angle);
		}
		// SE炎
		playerid->InputAttackFlameSE();

		// ため終わりまで撃つ
		if (magicCharge <= magicChargeEnd)
			playerid->GetStateMachine()->ChangeState((int)Player::State::Idle);

		else
			--magicCharge;
		break;
	}

	case (int)Player::CommandMagic::Thander:
	{
		// 時間
		if (animationTime <= 1.1f)return;

		// 雷音
		playerid->InputAttackThanderSE();

		// 雷発射
		playerid->InputMagicLightning();

		// ため終わりまで撃つ
		if (magicCharge <= magicChargeEnd)
			playerid->GetStateMachine()->ChangeState((int)Player::State::Idle);

		else
			--magicCharge;

		break;
	}
	case (int)Player::CommandMagic::Ice:
	{
		// 時間
		if (animationTime <= 0.5f)return;

		// 角度下に
		if (angle.x > angleMax)
			isAngleAddX = false;

		// 角度上に
		if (angle.x < angleMin)
			isAngleAddX = true;

		// 角度下に
		if (!isAngleAddX)
		{
			angle.x -= rotationSpeed * angleAddX;
		}
		// 角度上に
		else
		{
			angle.x += rotationSpeed * angleAddX;
		}

		// 氷音
		playerid->InputAttackIceSE();

		// １発
		if (magicCharge <= magicChargeEnd)
		{
			// 氷発射
			playerid->InputMagicIce();
		}
		else
		{
			// 角度
			angle = transformid->GetAngle();
			playerid->PushMagicIce(angle);
		}

		

		// ため終わりまで撃つ
		if (magicCharge <= magicChargeEnd)
		{
			int pCount = ProjectileManager::Instance().GetProjectileCount();
			pCount -= 2;
			for (int i = 0; i < pCount; ++i)
			{
				if (ProjectileManager::Instance().GetProjectile(i)->GetComponent<ProjectileStraight>())
				ProjectileManager::Instance().GetProjectile(i)->GetComponent<ProjectileStraight>()->SetMovementCheck(iceMagicMoveCheck);
			}
			playerid->GetStateMachine()->ChangeState((int)Player::State::Idle);
		}
		else
			--magicCharge;

		break;
	}
	case (int)Player::CommandMagic::Heale:
	{
		// 時間
		if (animationTime <= 1.1f)return;
		// 回復音
		playerid->InputAttackHealeSE();
		// 回復発動
		playerid->InputMagicHealing();
		playerid->GetStateMachine()->ChangeState((int)Player::State::Idle);
		break;
	}
	default:
		break;
	}
}

void PlayerMagicState::Exit()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// 一回だけ
	GamePad& gamePad = Input::Instance().GetGamePad();
	bool isPush = true;
	gamePad.SetButtonDownCountinue(isPush);

	// エフェクト再生してたら停止
	if (charge->GetEfeHandle())
	{
		// チャージエフェクト停止
		charge->Stop(charge->GetEfeHandle());
	}

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	if (!playerid->InputShortCutkeyMagic())
	{
		// 魔法選択解除
		playerid->SetMagicAction(false);
		// 攻撃種類選択
		playerid->SetSelectMagicCheck((int)Player::CommandAttack::Attack);
	}
	// 落ちるの再開
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
}

void PlayerSpecialMagicState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	fire = std::make_unique<Effect>("Data/Effect/fire.efk");
	fireAttack = std::make_unique<Effect>("Data/Effect/hit fire.efk");
	modelAnim.index = Player::Animation::Anim_SlashThree;
	// アニメーション再生
	model->PlayAnimation(modelAnim);
	// アニメーション再生
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");
	DirectX::XMFLOAT3 pPosition =
	{
				pHPosiiton->worldTransform._41,
				pHPosiiton->worldTransform._42,
				pHPosiiton->worldTransform._43
	};
	// 音炎
	playerid->InputAttackFlameSE();
	fire->Play(pPosition);
	// 落ちるの停止
	bool stopFall = true;
	moveid->SetStopFall(stopFall);
	button = true;
	// スタート値
	specialMoveWaitTime = specialMoveWaitStartTime;
	// 最初だけ魔法を発動するため
	startMagic = false;
	// カメラ回転処理を開始
	isRotate = true;
	bool specialRockOff = true;
	playerid->SetSpecialRockOff(specialRockOff);
}

void PlayerSpecialMagicState::Execute(float elapsedTime)
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	// エネミーの前に向く
	{
		DirectX::XMVECTOR Vector;
		DirectX::XMVECTOR LengthSq;
		DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid->GetPosition());
		EnemyManager& enemyManager = EnemyManager::Instance();
		int enemyCount = enemyManager.GetEnemyCount();
		for (int i = 0; i < enemyCount; ++i)//float 最大値ないにいる敵に向かう
		{
			DirectX::XMVECTOR enemyPosition = DirectX::XMLoadFloat3(&enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition());
			Vector = DirectX::XMVectorSubtract(enemyPosition, playerPosition);
			LengthSq = DirectX::XMVector3Length(Vector);
			Vector = DirectX::XMVector3Normalize(Vector);
			DirectX::XMStoreFloat3(&vector, Vector);
			DirectX::XMStoreFloat(&length, LengthSq);
		}
		// 相手の方を向く
		// 回転
		if (isRotate)
		{
			if (moveid->Turn(vector, turnSpeed, elapsedTime))
			{
				// カメラ向き
				{
					MessageData::CAMERACHANGEMOTIONMODEDATA	p;
					position = transformid->GetPosition();
					angle = transformid->GetAngle();
					// モーション記録
					float vx = sinf(angle.y) * 6;
					float vz = cosf(angle.y) * 6;
					float vx2 = sinf(angle.y) - 10;
					float vz2 = cosf(angle.y) * 7;
					float vx3 = sinf(angle.y);
					p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
					p.data.push_back({ 50, {position.x + vx, position.y + 3, position.z + vz }, position });
					p.data.push_back({ 100, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
					p.data.push_back({ 150, {position.x - vx2, position.y + 5, position.z - vz2 }, position });
					p.data.push_back({ 170, {position.x + vx, position.y + 3, position.z + vz }, position });
					p.data.push_back({ 200, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
					p.data.push_back({ 250, {position.x + vx3 , position.y + 1, (position.z + 0.1f) - vz2 }, position });
					// エネミー呼ぶ奴
					EnemyManager& enemyManager = EnemyManager::Instance();
					int enemyManagerCount = enemyManager.GetEnemyCount();
					DirectX::XMFLOAT3 pos;
					DirectX::XMVECTOR posVec;
					pos =
					{ 0,0,0 };
					float length;
					length = 10.0f;
					//　カメラ移動　ベクトル獲得
					if (enemyManagerCount > 0)
					{
						std::shared_ptr<EnemyBoss> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemyBoss>();
						DirectX::XMVECTOR positionVec = DirectX::XMLoadFloat3(&position);
						DirectX::XMVECTOR ePositionVec = DirectX::XMLoadFloat3(&enemy->GetPosition());
						posVec = DirectX::XMVectorSubtract(positionVec, ePositionVec);
						posVec = DirectX::XMVector3Normalize(posVec);
						DirectX::XMStoreFloat3(&pos, posVec);
					}
					p.data.push_back({ 330,{position.x + (pos.x * length) ,
						position.y + (pos.y * length) + 1,
						position.z + (pos.z * length) }, position });
					Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);
				}
			}
			////　必殺技の対象に向いた後
			else
			{
				isRotate = false;
			}
		}
	}
	if (button)
	{
		// アニメーション
		if (!model->IsPlayAnimation())
		{
			modelAnim.index = Player::Animation::Anim_MagicSeconde;
			// アニメーション再生
			model->PlayAnimation(modelAnim);
			button = false;
			// 手の炎エフェクト停止
			fire->Stop(fire->GetEfeHandle());
		}
	}
	else
	{
		// 任意のアニメーション再生区間でのみ衝突判定処理をする
		float animationTime = model->GetCurrentANimationSeconds();
		// アニメーション
		if (animationTime >= 1.1f)
		{
			playerid->SetFlashOn(true);
			// アニメーション停止
			playerid->SetUpdateAnim
			(Player::UpAnim::Stop);
			// カメラ振動
			MessageData::CAMERASHAKEDATA cameraShakeData;
			// カメラ振動値
			float shakeTimer = 0.5f;
			float shakePower = 0.8f;
			cameraShakeData = { shakeTimer , shakePower };
			// カメラ振動設定
			Messenger::Instance().SendData(MessageData::CAMERASHAKE, &cameraShakeData);
			Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");
			DirectX::XMFLOAT3 pPosition =
			{
						pHPosiiton->worldTransform._41,
						pHPosiiton->worldTransform._42,
						pHPosiiton->worldTransform._43
			};
			// 必殺技を放った後の経過時間
			specialMoveWaitTime += elapsedTime;
			PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
			ColorGradingData colorGradingData;
			colorGradingData.brigthness = 1.5f;
			postprocessingRenderer.SetColorGradingData(colorGradingData);
		}
		if (animationTime >= 1.1f && animationTime <= 1.2f)
		{
			playerid->
				InputSpecialMagicframe();
			playerid->InputAttackFlameSpecileSE();
		}
	}
	// ダメージ判定
	playerid->SpecialApplyDamageInRadius();

	// 待機時間が過ぎたので次のステートへ
	if (specialMoveWaitTime >= specialMoveWaitTimeMax)
	{
		int projectileMax = ProjectileManager::Instance().GetProjectileCount();
		for (int i = 0; i < projectileMax; ++i)
		{
			std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(i);
			if (projectile->GetComponent<ProjectileTornade>())
			{
				projectile->GetComponent<BulletFiring>()->Destroy();
			}
		}
		// 次のステートへ
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
		return;
	}
}

void PlayerSpecialMagicState::Exit()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	// 落ちるの再開
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
	// エネミー呼ぶ奴
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyManagerCount = enemyManager.GetEnemyCount();

	// 動作させるかどうか
	if (enemyManagerCount > 0)
	{
		std::shared_ptr<EnemyBoss> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemyBoss>();
		std::shared_ptr<Movement> enemyMove = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<Movement>();
		bool moveCheck = true;
		enemy->SetMoveCheck(moveCheck);
		// 速度停止
		bool stopVelocity = false;
		enemyMove->SetStopMove(stopVelocity);
		// 落ちるの停止
		bool stopFall = false;
		enemyMove->SetStopFall(stopFall);
	}
	PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
	// 最低値設定
	postprocessingRenderer.StepValueMin();
	bool specialRockOff = false;
	playerid->SetSpecialRockOff(specialRockOff);
}

void PlayerDamageState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// コンポネント取得
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	bool loop = false;
	// 歩き許可
	bool stopMove = false;
	moveid->SetStopMove(stopMove);
	// 重力オン
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
	modelAnim.index = Player::Animation::Anim_Pain;
	model->PlayAnimation(modelAnim);
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerDamageState::Execute(float elapsedTime)
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// コンポネント取得
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	// ロックオン処理
	playerid->UpdateCameraState(elapsedTime);
	if (!model->IsPlayAnimation())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
	}
}

void PlayerDamageState::Exit()
{
}

void PlayerDeathState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// コンポネント取得
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	float				stateTimer = 0.0f;
	modelAnim.index = Player::Animation::Anim_Deth;
	model->PlayAnimation(modelAnim);
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	stateTimer = 0.0f;
	PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
	colorGradingData.saturation = 0.0f;
	postprocessingRenderer.SetColorGradingMinData(colorGradingData);
}

void PlayerDeathState::Execute(float elapsedTime)
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// コンポネント取得
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<HP> hpid = sharedId->GetComponent<HP>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	if (stateTimer >= stateTimerMax)
	{
		hpid->SetDead(false);
	}
	    // 死亡時はそれっぽいカメラアングルで死亡
    // 例えばコレを必殺技などで上手く利用できればかっこいいカメラ演出が作れますね
    MessageData::CAMERACHANGEMOTIONMODEDATA	p;
	DirectX::XMFLOAT3 position = transformid->GetPosition();
	DirectX::XMFLOAT3 angle = transformid->GetAngle();

    float vx = sinf(angle.y) * 6;
    float vz = cosf(angle.y) * 6;
    p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
    p.data.push_back({ 95, {position.x + vx + 13, position.y + 3, position.z + vz }, position });
    Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentANimationSeconds();
	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。
	bool CollisionFlag = animationTime >= 0.5f && animationTime <= 1.6f;
	if (CollisionFlag)
	{
		++stateTimer;
		playerid->SetUpdateAnim(Player::UpAnim::Stop);
	}
}

void PlayerDeathState::Exit()
{
}

void PlayerReviveState::Enter()
{
}

void PlayerReviveState::Execute(float elapsedTime)
{
}

void PlayerReviveState::Exit()
{
}

// 回避初期化
void PlayerAvoidanceState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	// 下半身
	modelAnim.index = Player::Animation::Anim_Dush;
	modelAnim.currentanimationseconds = 0.3f;
	modelAnim.currentAnimationAddSeconds = 0.025f;
	model->PlayAnimation(modelAnim);
	// アニメーション種類 通常
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	// 落ちるの停止
	bool stopFall = true;
	moveid->SetStopFall(stopFall);
	playerid->InputDashSE();
	// エフェクト設定
	wind = std::make_unique<Effect>("Data/Effect/dashu.efk");
	// エフェクト再生時間
	wind->Play({ 
		transformid->GetPosition().x,
		transformid->GetPosition().y + addHeight,
		transformid->GetPosition().z
		});
	// 角度変更
	wind->SetAngle(wind->GetEfeHandle(), transformid->GetAngle());
	// 衝撃波初期化
	impactInitialize = true;
}

// 回避更新
void PlayerAvoidanceState::Execute(float elapsedTime)
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// コンポーネント呼び出し
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	// 動き自由
	bool stopMove = false;
	moveid->SetStopMove(stopMove);
	// ロックオン処理
	playerid->UpdateCameraState(elapsedTime);
	// 転がりダッシュ
	DirectX::XMFLOAT3 dir;
	dir.x = sinf(transformid->GetAngle().y);
	dir.y = cosf(transformid->GetAngle().x);
	dir.z = cosf(transformid->GetAngle().y);
	// 任意のアニメーション再生区間
	float animationTime = sharedId->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// 位置更新
	wind->SetPosition(wind->GetEfeHandle(),
		{
			transformid->GetPosition().x,
			transformid->GetPosition().y + addHeight,
			transformid->GetPosition().z
		});
	// 回転
	if (animationTime <= 0.5f)
	{
		// 入力情報を取得
		GamePad& gamePad = Input::Instance().GetGamePad();
		rotateVec.x = gamePad.GetAxisLX();
		rotateVec.y = 0.0f;
		rotateVec.z = gamePad.GetAxisLY();

		moveid->Turn(rotateVec, rotateSpeed, elapsedTime);
	}
	// 地上ダッシュ
	if (animationTime >= 0.7f && animationTime <= 0.8f)
	{
		DirectX::XMFLOAT3 impulse =
		{
			dir.x * speed,
			0,
			dir.z * speed,
		};
		moveid->AddImpulse(impulse, impactInitialize);
		impactInitialize = false;
	}

	// アニメーションダッシュ後通常移動
	if (animationTime >= 1.0f && sharedId->GetComponent<Player>()->InputMove())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	}
	// 待機
	if (animationTime >= 2.0f)
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
	}
}

// 回避終了
void PlayerAvoidanceState::Exit()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// コンポーネント呼び出し
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	// 落ちるの停止
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
	wind->Stop(wind->GetEfeHandle());
}

// 反射開始
void PlayerReflectionState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// コンポーネント呼び出し
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	bool loop = false;
	modelAnim.index = Player::Animation::Anim_Counter;
	model->PlayAnimation(modelAnim);
	// アニメーション種類 通常
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
}

// 反射更新
void PlayerReflectionState::Execute(float elapsedTime)
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// コンポーネント呼び出し
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	if (!model->IsPlayAnimation())
	{
		// 当たり判定の有無
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
	}
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentANimationSeconds();
	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。
	bool CollisionFlag = animationTime >= 0.2f && animationTime <= 0.4f;
	if (CollisionFlag)
	{
		// 両手ノードとエネミーの衝突処理
		playerid->CollisionNodeVsEnemiesCounter("mixamorig:LeftHand", sharedId->GetComponent<Player>()->GetLeftHandRadius());
		playerid->CollisionNodeVsEnemiesCounter("mixamorig:RightHand", sharedId->GetComponent<Player>()->GetLeftHandRadius());
	}
}

// 反射終了
void PlayerReflectionState::Exit()
{
}

void PlayerTitleIdleState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Animation::Anim_Idle;
	modelAnim.loop = true;
	model->PlayAnimation(modelAnim);
	// アニメーションルール
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerTitleIdleState::Execute(float elapsedTime)
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	//// ロックオン処理
    playerid->UpdateCameraState(elapsedTime);
	// ヒット
	if (playerid->InputAttack())
	{

		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::StateTitle::Push));
	}
}

void PlayerTitleIdleState::Exit()
{
}

void PlayerTitlePushState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// コンポネント取得
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	lightning = std::make_unique<Effect>("Data/Effect/sunder.efk");
	lightningAttack = std::make_unique<Effect>("Data/Effect/HitThunder.efk");
	lightningHit = std::make_unique<Effect>("Data/Effect/lightningStrike.efk");
	modelAnim.index = Player::Animation::Anim_MagicSeconde;
	// アニメーション再生
	model->PlayAnimation(modelAnim);
	// アニメーション再生
	playerid->SetUpdateAnim(Player::UpAnim::Normal);

	// 落ちるの停止
	bool stopFall = true;
	moveid->SetStopFall(stopFall);
	// モーション切り替え
	secondeMortion = false;
}

void PlayerTitlePushState::Execute(float elapsedTime)
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// コンポネント取得
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	//// ロックオン処理
	playerid->UpdateCameraState(elapsedTime);
	std::shared_ptr<HP> enemyHpId;
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
		float animationTime = moveid->GetOnLadius() ?
			model->GetCurrentANimationSeconds() :
			model->GetCurrentAnimationSecondsUpeer();
		// アニメーション
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON && !secondeMortion)
		{
			playerid->InputAttackThanderSE();
				// 再生ループ
				bool  loop = false;
				// 再生開始時間 
				float currentAnimationStartSeconds = 0.0f;
				// 再生時間加算分の値
				float currentAnimationAddSeconds = 0.00f;
				// アニメーションブレンド
				float blendSeconds = 0.5f;
				modelAnim.index = Player::Animation::Anim_SpecialAttack;
				// アニメーション再生
				model->PlayAnimation(modelAnim);
				Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");
				DirectX::XMFLOAT3 pPosition =
				{
							pHPosiiton->worldTransform._41,
							pHPosiiton->worldTransform._42,
							pHPosiiton->worldTransform._43
				};
				lightningHit->Play(pPosition);
				secondeMortion = true;
				// ポストエフェクトインスタンスゲット
				PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
				// ラジアルブラー
				radialBlurData.radius = 40.0f;
				postprocessingRenderer.SetRadialBlurData(radialBlurData);
		}
		if (animationTime >= 1.5f - FLT_EPSILON && animationTime <= 1.6f + FLT_EPSILON && secondeMortion)
		{
			playerid->InputAttackSlashSpecileLightningStrikeSE();
			Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");
			DirectX::XMFLOAT3 pPosition =
			{
						pHPosiiton->worldTransform._41,
						pHPosiiton->worldTransform._42,
						pHPosiiton->worldTransform._43
			};
			lightningHit->Play(pPosition);
			// ポストエフェクトインスタンスゲット
			PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
			// ラジアルブラー
			radialBlurData.radius = 60.0f;
			postprocessingRenderer.SetRadialBlurData(radialBlurData);
		}
		if (!model->IsPlayAnimation() && secondeMortion)
		{
			// シーン終了
			playerid->SetEndState(true);
		}
}

void PlayerTitlePushState::Exit()
{
}

void PlayerClearIdleState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Animation::Anim_MagicSeconde;
	modelAnim.keyFrameEnd = 68.0f;
	model->PlayAnimation(modelAnim);
	// アニメーションルール
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	DirectX::XMFLOAT3 position = transformid->GetPosition();
	position.x += -3.0f;
	transformid->SetPosition(position);
}

void PlayerClearIdleState::Execute(float elapsedTime)
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	Model::Node* PRock = model->FindNode("mixamorig:Spine1");
	DirectX::XMFLOAT3 pos = model->ConvertLocalToWorld(PRock);
	pos.x += 3.0f;
	MessageData::CAMERACHANGEFREEMODEDATA	p = { pos };
	Messenger::Instance().SendData(MessageData::CAMERACHANGEFREEMODE, &p);
	// ヒット
	if (playerid->InputAttack())
	{
		// シーン終了
		playerid->SetEndState(true);
	}
}

void PlayerClearIdleState::Exit()
{
}

void PlayerClearEscapeState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Animation::Anim_MagicSeconde;
	modelAnim.keyFrameEnd = 68.0f;
	model->PlayAnimation(modelAnim);
	// アニメーションルール
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	DirectX::XMFLOAT3 position = transformid->GetPosition();
	position.x += -3.0f;
	transformid->SetPosition(position);
}

void PlayerClearEscapeState::Execute(float elapsedTime)
{
}

void PlayerClearEscapeState::Exit()
{
}

void PlayerOverIdleState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	// アニメーションルール
	Model::ModelAnim modelAnim;
	modelAnim.index = Player::Animation::Anim_Deth;
	modelAnim.currentanimationseconds = 1.7f;
	model->PlayAnimation(modelAnim);
	// アニメーションルール
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerOverIdleState::Execute(float elapsedTime)
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	//// ロックオン処理
	playerid->UpdateCameraState(elapsedTime);
	// ヒット
	if (playerid->InputAttack())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::StateOver::Revive));
	}
}

void PlayerOverIdleState::Exit()
{
}

void PlayerOverReviveState::Enter()
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Animation::Anim_Magic;
	model->PlayAnimation(modelAnim);
	// アニメーションルール
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	isPoseStarte = false;
}

void PlayerOverReviveState::Execute(float elapsedTime)
{
	// 安全チェック
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	if (!model->IsPlayAnimation() && !isPoseStarte)
	{
		modelAnim.index = Player::Animation::Anim_Jump;
		model->PlayAnimation(modelAnim);
		isPoseStarte = true;
		return;
	}
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	if (!model->IsPlayAnimation() && isPoseStarte)
	{
		playerid->SetEndState(endState);
	}
}

void PlayerOverReviveState::Exit()
{
}
