#include <math.h>
#include "EnemyBoss.h"
#include "StateDerived.h"

#include "Player.h"
#include "Mathf.h"

//#include "Audio\Audio.h"
#include "ProjectileHoming.h"
#include "ProjectileRuby.h"
#include "ProjectileThrowing.h"
#include "ProjectileManager.h"
#include "BulletFiring.h"

#include "SceneLoading.h"
#include "SceneManager.h"
#include "SceneGameOver.h"
#include "SceneGame.h"



// TODO 02_03 Stateを基底クラスとして各種Stateクラスを用意する。
// Wanderはサンプルとしてすでに記述済み
// 各種Enter関数の内容は各Transition○○State関数を参考に
// 各種Execute関数の内容は各Update○○State関数を参考に

// 徘徊ステートに入った時のメソッド
void WanderState::Enter()
{
	

	// owner呼び出し
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	

	//walkSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/Enemy着地.wav", "walk");
	//walkSe->Play("walk",loopSe);
	//float seSpeed = 0.5f;
	//walkSe->SetSpeed("walk",seSpeed);

	//moveid = owner.lock()->GetComponent<Movement>();
	// 縄張り
	enemyid.lock()->SetRandomTargetPosition();
	// アニメーション終了キーフレーム
	//currentAnimationEndSeconds = 300.0f;
	// 攻撃回数
	attackCount = attackCountMax;

	// 覚醒時の設定
	if (enemyid.lock()->GetIsEnemyAwakened())
	{
		attackCount = attackCountMin;
		//currentAnimationEndSeconds = 288;
		modelAnim.keyFrameEnd = 288;
	}

	modelAnim.index = (int)EnemyBoss::Animation::Anim_Movie;

	modelAnim.loop = false;

	modelAnim.currentanimationseconds = 3.3f;

	modelAnim.blendSeconds = 0.7f;

	modelAnim.currentAnimationAddSeconds = 0.00f;

	modelAnim.keyFrameEnd = 300.0f;

	// アニメーション再生
	model->
		PlayAnimation(modelAnim);



	//model->
	//	PlayAnimation(EnemyBoss::Animation::Anim_Walk,
	//		loop, currentAnimationStartSeconds, blendSeconds, 
	//		currentAnimationAddSeconds);
	// アニメーションルール
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);

	//stateTimer = stateTimerMax;
	//stateTimer = 60.0f;

	// 着地瞬間
	upOnLading = false;

	// プレイヤーid

	// 煙エフェクト
	smorker = std::make_unique<Effect>("Data/Effect/smorkeDash.efk");


}

// 徘徊ステートで実行するメソッド
void WanderState::Execute(float elapsedTime)
{
	//stateTimer -= elapsedTime;

	// owner呼び出し
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();


	// 目標地点ををプレイヤー位置に設定
	targetPosition = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentANimationSeconds();

	enemyid.lock()->SetTargetPosition(targetPosition);
	
	//// 目的地点までのXZ平面での距離判定
	//float vx = targetPosition.x - enemyid->GetPosition().x;
	//float vz = targetPosition.z - enemyid->GetPosition().z;
	//float distSq = vx * vx + vz * vz;

	//DirectX::XMFLOAT3 direction = 
	//{
	//	targetPosition.x - enemyid->GetPosition().x,
	//	targetPosition.y - enemyid->GetPosition().y,
	//	targetPosition.z - enemyid->GetPosition().z
	//};

	DirectX::XMVECTOR targetPositionVec = DirectX::XMLoadFloat3(&targetPosition);
	DirectX::XMVECTOR enemyPositionVec = DirectX::XMLoadFloat3(&enemyid.lock()->GetPosition());

	DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(targetPositionVec, enemyPositionVec);

	DirectX::XMFLOAT3 direction;

	DirectX::XMStoreFloat3(&direction, directionVec);

	

	// ボスの覚醒アニメーション終了
	if (!model->IsPlayAnimation() && enemyid.lock()->GetIsEnemyAwakened() && attackCount < attackCountMax)
	{
		++attackCount;

		// アニメーション終了キーフレーム
		modelAnim.keyFrameEnd = 288;

		// アニメーション再生
		model->
			PlayAnimation(modelAnim);
		//PlayAnimation(EnemyBoss::Animation::Anim_Movie,
		//		loop, currentAnimationStartSeconds, blendSeconds,
		//		currentAnimationAddSeconds, currentAnimationEndSeconds);
	}

	// 目的地へ着いた
	//if (stateTimer <= stateTimerEnd)
	if (!model->IsPlayAnimation())
	{
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
	}
	// 移動
	if (animationTime <= 4.6f)
	{
		moveid.lock()->Turn(direction, turnSpeed, elapsedTime);

		moveid.lock()->Move(direction, moveSpeed, elapsedTime);


		// ポストエフェクトインスタンスゲット
		PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
		// ラジアルブラー
		radialBlurData.radius = 30.0f;

		postprocessingRenderer.SetRadialBlurData(radialBlurData);

	}



	// 目的地にこの半径入ったら
	float radius = enemyid.lock()->GetAttackRightFootRange();

	//moveid.lock()->Move(direction ,moveSpeed, elapsedTime);

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

		//enemyid.lock()->DetectHitByBodyPart(bossLeftFootPosition);

		// 右足
		DirectX::XMFLOAT3 bossRightFootPosition;
		bossRightFootPosition = model->ConvertLocalToWorld(bossRightFoot);

		//enemyid.lock()->DetectHitByBodyPart(bossRightFootPosition);

		// 右腕
		DirectX::XMFLOAT3 bossRightHandPosition;
		bossRightHandPosition = model->ConvertLocalToWorld(bossRightHand);

		//enemyid.lock()->DetectHitByBodyPart(bossRightHandPosition);

		// 左腕
		DirectX::XMFLOAT3 bossLeftHandPosition;
		bossLeftHandPosition = model->ConvertLocalToWorld(bossLeftHand);

		//enemyid.lock()->DetectHitByBodyPart(bossLeftHandPosition);

		// 足が地面につく
		if (animationTime + FLT_EPSILON >= 4.0f - FLT_EPSILON &&
			animationTime - FLT_EPSILON <= 4.1f + FLT_EPSILON)
		{
			// 音
			enemyid.lock()->InputJampSe();

			smorker->Play(bossRightFootPosition, scaleEffect);

			// 左足
			enemyid.lock()->DetectHitByBodyPart(bossLeftFootPosition, applyDamage);

			// 右足
			enemyid.lock()->DetectHitByBodyPart(bossRightFootPosition, applyDamage);

			// 右腕
			enemyid.lock()->DetectHitByBodyPart(bossRightHandPosition, applyDamage);
			// 左腕
			enemyid.lock()->DetectHitByBodyPart(bossLeftHandPosition, applyDamage);

			// 中心
			enemyid.lock()->DetectHitByBodyPart(transformid.lock()->GetPosition(), applyDamage);

		}

		// 足が地面につく
		if (animationTime + FLT_EPSILON >= 4.6f - FLT_EPSILON &&
			animationTime - FLT_EPSILON <= 4.7f + FLT_EPSILON)
		{
			// 音
			enemyid.lock()->InputJampSe();

			smorker->Play(bossLeftFootPosition, scaleEffect);
			// 左足
			enemyid.lock()->DetectHitByBodyPart(bossLeftFootPosition, applyDamage);

			// 右足
			enemyid.lock()->DetectHitByBodyPart(bossRightFootPosition, applyDamage);

			// 右腕
			enemyid.lock()->DetectHitByBodyPart(bossRightHandPosition, applyDamage);
			// 左腕
			enemyid.lock()->DetectHitByBodyPart(bossLeftHandPosition, applyDamage);

			// 中心
			enemyid.lock()->DetectHitByBodyPart(transformid.lock()->GetPosition(), applyDamage);
			
		}

	}
	//enemyid->CollisionPlayerWithRushEnemy();

	//// 任意のアニメーション再生区間でのみ衝突判定処理をする
	//float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	//if (animationTime >= 0.6f && animationTime <= 0.7f)
	//{
	//	// アニメーションルール 停止
	//	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Stop);
	//
	//	if (distSq > radius * radius)
	//		// 目的地点へ移動
	//		enemyid->MoveToTarget(elapsedTime, 0.8f);


	//	// ジャンプ
	//	enemyid->InputJump();

	//	// 着地で衝撃波を飛ばす
	//	if (moveid->GetOnLadius() && !upOnLading)
	//	{

	//		Model::Node* node = owner->GetComponent<ModelControll>()->GetModel()->FindNode("boss_right_foot1");
	//		DirectX::XMFLOAT3 pos(
	//			node->worldTransform._41,
	//			node->worldTransform._42,
	//			node->worldTransform._43
	//		);
	//		enemyid->InputImpact(pos);
	//		// 着地瞬間
	//		upOnLading = true;
	//	}
	//	if (!moveid->GetOnLadius())
	//	{

	//		// 着地瞬間
	//		upOnLading = false;
	//	}
	//}


}

// 徘徊ステートから出ていくときのメソッド
void WanderState::Exit()
{
	// owner呼び出し
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();

	//walkSe->Stop("walk");

	// 連続攻撃
	attackCount = attackCountMin;

	enemyid.lock()->InputSlashSe();
}


// 初期化
void IdleState::Enter()
{
	// owner呼び出し
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	//model = owner.lock()->GetComponent<ModelControll>();
	//move = owner->GetComponent<Movement>();
	std::weak_ptr<HP> hp = owner.lock()->GetComponent<HP>();

	//model = owner->GetComponent<ModelControll>();

	modelAnim.index = EnemyBoss::Anim_Standby;

	modelAnim.loop = true;

	modelAnim.currentanimationseconds = 0.0f;

	modelAnim.blendSeconds = 0.7f;

	model->PlayAnimation(modelAnim);
	// アニメーションルール
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	
	// 待機処理
	//stateTimer = stateTimerMax;
	//stateTimer = Mathf::RandomRange(stateTimerMin, stateTimerMax);
	stateTimer = stateTimerMax;

	// 逃げる条件初期化
	hp.lock()->ResetOnDamageThresholdTime();

	randRoot = rand() % 2;
}

// update
void IdleState::Execute(float elapsedTime)
{
	 stateTimer -= elapsedTime;


	 std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	 Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	 std::weak_ptr <HP> hp = owner.lock()->GetComponent<HP>();

	if (stateTimer <= stateTimerEnd)
	{
		// TODO 03 
		// 待機時間が経過したとき徘徊ステートへ遷移しなさい
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Pursuit));
	}
	// 一定ダメージの最大値
	float damageThreshold = 10;
	float timeLimit = 5;
	// 逃走
	if (hp.lock()->CheckDamageThresholdWithinTime(elapsedTime, damageThreshold, timeLimit)&&
		randRoot == (int)EnemyBoss::ExitRoot::ExitJamp)
	{
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Jump));
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
	std::weak_ptr<HP> hp = owner.lock()->GetComponent<HP>();

	// 逃げる条件初期化
	hp.lock()->ResetOnDamageThresholdTime();
}

// 初期化
void PursuitState::Enter()
{
	// プレイヤーコンポーネントアクセス
	//playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Player>();

	//--mortionLimit;

	modelAnim.index = EnemyBoss::Animation::Anim_Standby;

	modelAnim.loop = true;

	modelAnim.currentanimationseconds = 0.0f;

	modelAnim.blendSeconds = 0.7f;

	// エネミーアクセス用
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	enemyid.lock()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		modelAnim);

	// アニメーションルール
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);

	// 数秒間追跡するタイマーをランダム設定
	//stateTimer = Mathf::RandomRange(3.0f, 5.0f);

	// 攻撃の種類を出す
	randamAttack = rand() % 3;

	//attackRange = enemyid->GetAttackRightFootRange();

	//hpid = owner->GetComponent<HP>();

	// 敵ライフ
	//attackRound = hpid->GetLife();

	//switch (attackRound)
	//{
	//case AttackChange::Round1:
	//{
	//	
	//	break;
	//}
	//case AttackChange::Round2:
	//{
	//	//attackType = rand() % 2;
	//	attackType = 0;
	//	break;
	//}
	//case AttackChange::ROund3:
	//{
	//	//attackType = rand() % 3;
	//	attackType = rand() % 2;
	//	break;
	//}
	//default:
	//	break;
	//}
	//

}
// update
void PursuitState::Execute(float elapsedTime)
{
	// 目標地点をプレイヤー位置に設定
		// プレイヤーid

	// エネミーアクセス用
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	
	// 目標地点ををプレイヤー位置に設定
	DirectX::XMFLOAT3 targetPosition = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();


	enemyid.lock()->SetTargetPosition(targetPosition);

	//// ３階行動すると混乱
	//if (mortionLimit < 0)
	//{
	//	// 待機時間再設定
	//	//mortionLimit = Mathf::RandomRange(3.0f, mortionLimitMax);
	//	mortionLimit = rand() % mortionLimitMax + 3;
	//	// 限界がきたので休憩
	//	enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::IdleBattle));
	//	return;
	//}

	// タイマー処理
	//stateTimer -= elapsedTime;


	float vx = targetPosition.x - enemyid.lock()->GetPosition().x;
	float vy = targetPosition.y - enemyid.lock()->GetPosition().y;
	float vz = targetPosition.z - enemyid.lock()->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	// TODO 03 
	// 攻撃範囲に入ったとき攻撃ステートへ遷移しなさい
	//if (dist < attackRange)
	//	enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Attack));

	//if (stateTimer >= stateTimerMin) return;

	switch (randamAttack)
	{
	case (int)EnemyBoss::AttackMode::AssaultAttack:
	{
		enemyid.lock()->GetStateMachine()->ChangeState(
			static_cast<int>(EnemyBoss::State::Wander));

		//enemyid.lock()->GetStateMachine()->ChangeState(
		//	static_cast<int>(EnemyBoss::State::Attack));
		break;
	}
	case (int)EnemyBoss::AttackMode::JumpStompAttack:
	{
		enemyid.lock()->GetStateMachine()->ChangeState(
			static_cast<int>(EnemyBoss::State::Jump));

		//enemyid.lock()->GetStateMachine()->ChangeState(
		//	static_cast<int>(EnemyBoss::State::Attack));


		break;
	}
	case (int)EnemyBoss::AttackMode::DushAttack:
	{
		enemyid.lock()->GetStateMachine()->ChangeState(
			static_cast<int>(EnemyBoss::State::Attack));

		//enemyid.lock()->GetStateMachine()->ChangeState(
		//	static_cast<int>(EnemyBoss::State::Attack));


		break;
	}

	}


	//switch (attackRound)
	//{
	//case AttackChange::Round1:
	//{
	//	// 時間が過ぎたので
	//	if (stateTimer < 0.0f)
	//	{
	//		
	//		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Shot));
	//	}

	//	break;
	//}

	//case AttackChange::Round2:
	//{
	//	if (stateTimer < 0.0f && attackType == 0)
	//		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Shot));

	//	//if (stateTimer < 0.0f && attackType == 1)
	//	//	owner->GetComponent<EnemyBoss>()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::ShotThrowing));

	//	break;
	//}

	//// HP残機０
	//case AttackChange::ROund3:
	//{

	//	//owner->GetComponent<EnemyBoss>()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::ShotThrowing));
	//	if (stateTimer < 0.0f && attackType == 0)
	//		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Shot));

	//	//if (stateTimer < 0.0f && attackType == 1)
	//	//	owner->GetComponent<EnemyBoss>()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::ShotThrowing));

	//	if (stateTimer < 0.0f && attackType == 1)
	//		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Wander));

	//	break;
	//}


	//// HP残機　２と１
	//default:
	//{


	//	break;
	//}
	//}

}
// 最後にやりたい処理全般
void PursuitState::Exit()
{

}

// 初期化
void JumpState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();



	// SE 否設定
	//janpSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/Enemy着地.wav","janp");
	//landSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/Enemy着地.wav","land");

	//moveid = owner.lock()->GetComponent<Movement>();

	modelAnim.index = EnemyBoss::Animation::Anim_Attack;

	modelAnim.loop = true;

	modelAnim.currentanimationseconds = 2.0f;

	modelAnim.blendSeconds = 0.7f;

	modelAnim.currentAnimationAddSeconds = 0.01f;


	Model* model =  owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<HP> hpid = owner.lock()->GetComponent<HP>();
	// 縄張り
	enemyid.lock()->SetRandomTargetPosition();

	// アニメーション再生
	model->
		PlayAnimation(modelAnim);
	// アニメーションルール
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Reverseplayback);

	//stateTimer = Mathf::RandomRange(6.0f, 8.0f);
	//stateTimer = 60.0f;

	// 着地瞬間
	upOnLading = false;

	// アニメーション終了
	jumpStart = false;

	// 目標地点ををプレイヤー位置に設定
	targetPosition =
		PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();

	// もし逃走するならこっち
	if (hpid.lock()->GetIsOverDamageRule())
	{
		targetPosition = { 0,enemyid.lock()->GetPosition().y,0 };
		targetPosition.x += Mathf::RandomRange(-enemyid.lock()->GetSearchRange(), enemyid.lock()->GetSearchRange());
		targetPosition.z += Mathf::RandomRange(-enemyid.lock()->GetSearchRange(), enemyid.lock()->GetSearchRange());

		// 一定ダメージ確認解除
		bool isOverDamageRule = false;
		hpid.lock()->SetIsOverDamageRule(isOverDamageRule);
	}

	// 煙エフェクト
	smorker = std::make_unique<Effect>("Data/Effect/smorkeDash.efk");
}
// 更新処理
void JumpState::Execute(float elapsedTime)
{
	//stateTimer -= elapsedTime;

	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();


	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<HP> hpid = owner.lock()->GetComponent<HP>();


	enemyid.lock()->SetTargetPosition(targetPosition);


	DirectX::XMVECTOR targetPositionVec = DirectX::XMLoadFloat3(&targetPosition);
	DirectX::XMVECTOR enemyPositionVec = DirectX::XMLoadFloat3(&enemyid.lock()->GetPosition());

	DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(targetPositionVec, enemyPositionVec);

	DirectX::XMFLOAT3 direction;

	DirectX::XMStoreFloat3(&direction, directionVec);

	//// 目的地へ着いた
	//if (stateTimer < 0.0f)
	//{
	//	enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
	//}
	// 目的地にこの半径入ったら
	float radius = enemyid.lock()->GetAttackRightFootRange();

	//Model::Node* bossLeftHand = model->FindNode("boss_left_hand1");

	// 左足ボーン
	Model::Node* bossLeftFoot = model->FindNode("boss_left_foot1");
	// ノード位置取得
	// 左足
	DirectX::XMFLOAT3 bossLeftFootPosition;
	bossLeftFootPosition = model->ConvertLocalToWorld(bossLeftFoot);

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentANimationSeconds();

	if (animationTime - FLT_EPSILON <= 0.8f + FLT_EPSILON && !jumpStart)
	{
		//landSe->Play("janp",loopSe);

		bool blurCheck = true;
		enemyid.lock()->SetBlurCheck(blurCheck);

		enemyid.lock()->CollisionPlayerWithRushEnemy();

		moveid.lock()->JumpVelocity(jumpSpeed);

		jumpStart = true;

		smorker->Play(bossLeftFootPosition, scaleEffect);

		PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance().Instance();
		// ラジアルブラー
		radialBlurData.radius = 15;

		postprocessingRenderer.SetRadialBlurData(radialBlurData);

		//janpSe->Play("janp",loopSe);

		return;
	}
	else
	{
		moveid.lock()->Turn(direction, turnSpeed, elapsedTime);
	}

	if (!moveid.lock()->GetOnLadius())
	{
		moveid.lock()->Move(direction, moveSpeed, elapsedTime);
		moveid.lock()->Turn(direction, turnSpeed, elapsedTime);
	}

	if (moveid.lock()->GetOnLadius() && jumpStart)
	{
		//landSe->Play("land",loopSe);
		//enemyid->InputProjectile();
		enemyid.lock()->InputImpact(enemyid.lock()->GetPosition());
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));

		enemyid.lock()->InputJump();

		// 煙エフェクト
		smorker->Play(bossLeftFootPosition, scaleEffect);
		//landSe->Play(loopSe);


		PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance().Instance();
		// ラジアルブラー
		radialBlurData.radius = 30;

		postprocessingRenderer.SetRadialBlurData(radialBlurData);

	}
	//enemyid.lock()->CollisitionNodeVsPlayer("boss_left_foot1",enemyid.lock()->GetRadius());

	//Model::Node* bossLeftFoot = model->FindNode("boss_left_hand1");


	// 左足当たり判定
	enemyid.lock()->DetectHitByBodyPart(bossLeftFootPosition, applyDamageJumpStart);
}
// 終了処理
void JumpState::Exit()
{
	//jumpStart = false;

	//janpSe->Stop("janp");
	//landSe->Stop("land");
}

// 初期化
void AttackState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();



	//stateTimer = stateTimerMax;


	// SE 否設定
	//janpSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/Enemy着地.wav","janp");
	//landSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/Enemy着地.wav","land");

	//moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<HP> hpid = owner.lock()->GetComponent<HP>();
	// 縄張り
	enemyid.lock()->SetRandomTargetPosition();

	modelAnim.index = EnemyBoss::Animation::Anim_Attack;

	modelAnim.currentanimationseconds = 2.3f;

	modelAnim.loop = false;

	modelAnim.blendSeconds = 0.7f;

	modelAnim.currentAnimationAddSeconds = 0.015f;

	modelAnim.keyFrameEnd = 0.0f;

	//// 再生開始時間 
	//currentAnimationStartSeconds = 2.3f;

	//loop = false;

	//currentAnimationAddSeconds = 0.015f;

	// アニメーション再生
	model->
		PlayAnimation(modelAnim);
	// アニメーションルール
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Reverseplayback);

	//stateTimer = Mathf::RandomRange(6.0f, 8.0f);
	//stateTimer = 60.0f;

	// 着地瞬間
	//upOnLading = false;

	// アニメーション終了
	dushStart = false;

	// 目標地点ををプレイヤー位置に設定
	targetPosition =
		PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();

	// もし逃走するならこっち
	if (hpid.lock()->GetIsOverDamageRule())
	{
		targetPosition = { 0,enemyid.lock()->GetPosition().y,0 };
		targetPosition.x += Mathf::RandomRange(-enemyid.lock()->GetSearchRange(), enemyid.lock()->GetSearchRange());
		targetPosition.z += Mathf::RandomRange(-enemyid.lock()->GetSearchRange(), enemyid.lock()->GetSearchRange());

		// 一定ダメージ確認解除
		bool isOverDamageRule = false;
		hpid.lock()->SetIsOverDamageRule(isOverDamageRule);
	}

	// 煙エフェクト
	smorker = std::make_unique<Effect>("Data/Effect/smorkeDash.efk");
	charge = std::make_unique<Effect>("Data/Effect/effectCharge.efk");
	chargeCompleate = std::make_unique<Effect>("Data/Effect/chargecompleted.efk");




	//std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();

	//std::weak_ptr<HP> hpid = owner.lock()->GetComponent<HP>();

	//Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	//currentAnimationStartSeconds = 0.0f;
	//model->PlayAnimation(EnemyBoss::Animation::Anim_Attack, loop, currentAnimationStartSeconds, blendSeconds);
	//

	//// アニメーションルール
	//enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);

	//	// ライフ
	//int life;
	//life = hpid.lock()->GetLife();

	//switch (life)
	//{
	//case AttackChange::Round1:
	//{
	//	attackMemoryMax = 1;
	//	break;
	//}

	//case AttackChange::Round2:
	//{
	//	attackMemoryMax = 3;
	//	break;
	//}

	//case AttackChange::ROund3:
	//{
	//	attackMemoryMax = 5;
	//	break;
	//}

	//}
}
// update
void AttackState::Execute(float elapsedTime)
{
	

	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();


	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<HP> hpid = owner.lock()->GetComponent<HP>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	if (PlayerManager::Instance().GetPlayerCount() >= 1)
	{
		std::weak_ptr<Transform> pTransformId = PlayerManager::Instance().GetPlayer(0)->GetComponent<Transform>();

		targetPosition = pTransformId.lock()->GetPosition();
	}

	DirectX::XMVECTOR targetPositionVec = DirectX::XMLoadFloat3(&targetPosition);
	DirectX::XMVECTOR enemyPositionVec = DirectX::XMLoadFloat3(&enemyid.lock()->GetPosition());

	DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(targetPositionVec, enemyPositionVec);


	DirectX::XMStoreFloat3(&directionP, directionVec);

	angle = transformid.lock()->GetAngle();

	direction.x = sinf(angle.y);// 三角を斜めにして位置を変えた
	direction.y = 0;
	direction.z = cosf(angle.y);

	//// 目的地へ着いた
	//if (stateTimer < 0.0f)
	//{
	//	enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
	//}
	// 目的地にこの半径入ったら
	float radius = enemyid.lock()->GetAttackRightFootRange();

	//Model::Node* bossLeftHand = model->FindNode("boss_left_hand1");

	// 左足ボーン
	//Model::Node* bossLeftFoot = model->FindNode("boss_left_foot1");
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
	// 元々の奴
	//{
		//if (animationTime - FLT_EPSILON >= 2.0f + FLT_EPSILON && 
		//	animationTime - FLT_EPSILON <= 2.05f + FLT_EPSILON)
		//{

		//	charge->Play(bossEyePosition);

		//	chargeCompleate->Play(bossEyePosition);
		//}




		//if (animationTime - FLT_EPSILON <= 0.8f + FLT_EPSILON && !dushStart)
		//{
		//	//landSe->Play("janp",loopSe);

		//	bool blurCheck = true;
		//	enemyid.lock()->SetBlurCheck(blurCheck);

		//	enemyid.lock()->CollisionPlayerWithRushEnemy();

		//	//moveid.lock()->JumpVelocity(jumpSpeed);

		//	dushStart = true;


		//	//smorker->Play(bossLeftFootPosition, scaleEffect);

		//	//janpSe->Play("janp",loopSe);

		//	return;
		//}
		//else
		//{
		//	moveid.lock()->Turn(direction, turnSpeed, elapsedTime);
		//}

		//if (stateTimer > stateTimerEnd && dushStart)
		//{
		//	charge->Stop(charge->GetEfeHandle());

		//	stateTimer -= 0.1f;
		//	// 煙エフェクト
		//	smorker->Play(bossLeftFootPosition, scaleEffect);
		//	moveid.lock()->Move(direction, moveSpeed, elapsedTime);
		//	moveid.lock()->Turn(direction, turnSpeed, elapsedTime);
		//}

		//if (stateTimer <= stateTimerEnd && dushStart)
		//{
		//	//landSe->Play("land",loopSe);
		//	//enemyid->InputProjectile();
		//	//enemyid.lock()->InputImpact(enemyid.lock()->GetPosition());
		//	enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));

		//	// 煙エフェクト
		//	smorker->Play(bossLeftFootPosition, scaleEffect);
		//	//landSe->Play(loopSe);
		//}
		////enemyid.lock()->CollisitionNodeVsPlayer("boss_left_foot1",enemyid.lock()->GetRadius());

		////Model::Node* bossLeftFoot = model->FindNode("boss_left_hand1");


		//// 左足当たり判定
		//enemyid.lock()->DetectHitByBodyPart(bossLeftFootPosition);


	//}
	// チャージ中
	if (animationTime - FLT_EPSILON >= 1.5f + FLT_EPSILON &&
			animationTime - FLT_EPSILON <= 1.6f + FLT_EPSILON && !chargeInitilize)
	{
		enemyid.lock()->InputJampSe();
		enemyid.lock()->InputChargeSe();
		charge->Play(bossEyePosition);

		chargeCompleate->Play(bossEyePosition);

		// チャージ中
		stateChargeTimer = stateChargeTimerMax;


		chargeInitilize = true;

		// アニメーションルール
		enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Stop);
	}

	// ダッシュするまで
	if (!dushStart)
	{
		stateChargeTimer -= 0.01f;
		moveid.lock()->Turn(directionP, turnSpeed, elapsedTime);
	}


	// ダッシュ初期化
	if (chargeInitilize && stateChargeTimer <= stateChargeCompletionTimerEnd && !dushStart)
	{
		

		//// 再生開始時間 
		//currentAnimationStartSeconds = 0.0f;

		//loop = true;

		//currentAnimationAddSeconds = 0.2f;

		modelAnim.index = EnemyBoss::Animation::Anim_Walk;

		modelAnim.currentanimationseconds = 0.0f;

		modelAnim.loop = true;

		modelAnim.currentAnimationAddSeconds = 0.2f;

		modelAnim.keyFrameEnd = 0.0f;

		// アニメーション再生
		model->
			PlayAnimation(modelAnim);
		// アニメーションルール
		enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
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
		enemyid.lock()->InputDashSe();

		charge->Stop(charge->GetEfeHandle());

		// 煙エフェクト
		smorker->Play(bossLeftFootPosition, scaleEffect);

		moveid.lock()->Move(direction, moveSpeed, elapsedTime);

		stateChargeCompletionTimer -= 0.01f;

		// 右足当たり判定
		enemyid.lock()->DetectHitByBodyPart(bossRightFootPosition, applyDamageAttack);
		// 左足当たり判定
		enemyid.lock()->DetectHitByBodyPart(bossLeftFootPosition, applyDamageAttack);

	}


	// ダッシュ終了
	if (dushStart && stateChargeCompletionTimer - FLT_EPSILON <= stateChargeCompletionTimerEnd + FLT_EPSILON)
	{
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));




	}

	//if (model->GetCurrentAnimationIndex() == EnemyBoss::Animation::Anim_Movie &&
	//	model->IsPlayAnimation())
	//{

	//}


	//enemyid.lock()->CollisitionNodeVsPlayer("boss_left_foot1",enemyid.lock()->GetRadius());

	//Model::Node* bossLeftFoot = model->FindNode("boss_left_hand1");




	//std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	//Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	//// 攻撃モーションが終了したとき追跡ステートへ移行
	//if (!owner.lock()->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation() && attackMemory < attackMemoryMax)
	//{
	//	// アニメ再生
	//	currentAnimationStartSeconds = 0.8f;
	//	owner.lock()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemyBoss::Animation::Anim_Attack, loop, currentAnimationStartSeconds, blendSeconds);

	//	
	//	++attackMemory;
	//}
	//// 繰り返し踏みつけ最後
	//else if (attackMemory == attackMemoryMax)
	//{
	//	enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Pursuit));
	//	
	//	int attackMemoryStart = 0;
	//	attackMemory = attackMemoryStart;
	//}

	//// 任意のアニメーション再生区間でのみ衝突判定処理をする
	//float animationTime = owner.lock()->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	//if (animationTime >= 1.0f && animationTime <= 2.0f)
	//{
	//	// 左足ノードとプレイヤーの衝突処理
	//	enemyid.lock()->CollisitionNodeVsPlayer("boss_left_foot1", 2);
	//}

	//if (animationTime >= 1.30f && animationTime <= 1.33f)
	//{
	//	Model::Node* node = owner.lock()->GetComponent<ModelControll>()->GetModel()->FindNode("boss_left_foot1");
	//	DirectX::XMFLOAT3 pos(
	//		node->worldTransform._41,
	//		node->worldTransform._42,
	//		node->worldTransform._43
	//	);
	//	enemyid.lock()->InputImpact(pos);
	//}


}
// 最後にやりたい処理全般
void AttackState::Exit()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();

	enemyid.lock()->InputStopDashSe();

	chargeInitilize = false;

	if(charge->GetEfeHandle())
	charge->Stop(charge->GetEfeHandle());
}


// 初期設定
void AttackShotState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<HP> hpid = owner.lock()->GetComponent<HP>();
	

	currentAnimationStartSeconds = 0.0f;


	//model->PlayAnimation(EnemyBoss::Animation::Anim_Shot, loop, currentAnimationStartSeconds, blendSeconds);

	// アニメーションルール
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);

	// ライフ
	int life;
	life = hpid.lock()->GetLife();

	switch (life)
	{
	case AttackChange::Round1: 
	{
		attackCountMax = 1;
		break;
	}

	case AttackChange::Round2:
	{
		attackCountMax = 3;
		break;
	}

	case AttackChange::ROund3:
	{
		attackCountMax = 5;
		break;
	}

	}


}
// 更新処理
void AttackShotState::Execute(float elapsedTime)
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<HP> hpid = owner.lock()->GetComponent<HP>();

	// 攻撃モーションが終了したとき追跡ステートへ移行
	if (model->IsPlayAnimation())
	{
		// アニメ再生
		currentAnimationStartSeconds = 0.1f;
		//model->PlayAnimation(EnemyBoss::Animation::Anim_Shot, loop, currentAnimationStartSeconds ,blendSeconds);
		++attackCount;


	}
	if (attackCount >= attackCountMax)
	{
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Pursuit));
		attackCount = 0;
	}

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentANimationSeconds();
	// efekt設定
	if (animationTime >= 0.1f && animationTime <= 0.11f)
	{
		//	// 左足ノードとプレイヤーの衝突処理
		enemyid.lock()->InputProjectile();

	}

	// 弾丸発射
	if (animationTime >= 1.0f && animationTime <= 1.1f)
	{


		bool movementCheck = true;
		for (int i = 0; i < ProjectileManager::Instance().GetProjectileCount(); ++i)
		{
			
			if (ProjectileManager::Instance().GetProjectile(i)->GetComponent<ProjectileRuby>())
				ProjectileManager::Instance().GetProjectile(i)->GetComponent<ProjectileRuby>()->SetMovementCheck(movementCheck);
		}
	}
}
// 終了処理
void AttackShotState::Exit()
{
}

// 初期処理
void AttackShotThrowingState::Enter()
{
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;

	//std::shared_ptr<Transform> transformid = nullptr;

	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	//std::shared_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	currentAnimationStartSecondsf = 0.0f;
	//model->PlayReverseAnimation(EnemyBoss::Animation::Anim_jewelattack, loop,currentAnimationStartSecondsf,blendSeconds);
	// アニメーションルール
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Reverseplayback);

	// 投げる
	turnPermission = false;

	// 地面判定一度消す
	bool pushuThrow = false;
	enemyid.lock()->SetPushuThrow(pushuThrow);

}
// 更新処理
void AttackShotThrowingState::Execute(float elapsedTime)
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	// 正面
	//DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();
	// プレイヤーid
	std::weak_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&playerid.lock()->GetComponent<Transform>()->GetPosition());

	//for (int i = 0; i < ProjectileManager::Instance().GetProjectileCount(); ++i)
	//{

	//	std::shared_ptr<Actor> projectileManager = ProjectileManager::Instance().GetProjectile(i);
	//	if (projectileManager->GetComponent<ProjectileThrowing>())
	//	{
	//		// 球回転
	//		{

	//			DirectX::XMVECTOR projectilePosition = DirectX::XMLoadFloat3(&projectileManager->GetComponent<Transform>()->GetPosition());

	//			//DirectX::XMVECTOR vectorVec = DirectX::XMVectorSubtract(playerPosition, projectilePosition);
	//			////vectorVec = DirectX::XMVector3Normalize(vectorVec);
	//			//DirectX::XMFLOAT3 vector;
	//			//DirectX::XMStoreFloat3(&vector, vectorVec);

	//			projectileManager->GetComponent<BulletFiring>()->TurnFull(turnSpeed, playerid->GetComponent<Transform>()->GetPosition(), elapsedTime);
	//		}
	//	}
	//}
	



	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentANimationSeconds();
	// 宝石取って設定
	if (animationTime <= 3.9f && animationTime >= 3.89f && !turnPermission)
	{
		////// 正面の向きベクトル
  //       direction.x = sinf(angle.y);// 三角を斜めにして位置を変えた
  //       direction.y = sinf(angle.x);
  //       //direction.y = DirectX::XMConvertToRadians(0);
  //       direction.z = cosf(angle.y);		

		 
		//	遠距離攻撃登録
		enemyid.lock()->InputThrowingRuby(playerid.lock()->GetComponent<Transform>()->GetPosition());

		

	}
	// 回転
	if (animationTime <= 3.6f && animationTime >= 3.0f && !turnPermission)
	{

		enemyid.lock()->SetTargetPosition(playerid.lock()->GetComponent<Transform>()->GetPosition());
		enemyid.lock()->TurnToTarget(elapsedTime, turnSpeed);



		for (int i = 0; i < ProjectileManager::Instance().GetProjectileCount() ; ++i)
		{


			std::shared_ptr<Actor> projectileManager = ProjectileManager::Instance().GetProjectile(i);
			if (projectileManager->GetComponent<ProjectileThrowing>())
			{
				
				projectileManager->GetComponent<Transform>()->SetPosition(
					{ 
						transformid.lock()->GetPosition().x,
						transformid.lock()->GetPosition().y + transformid.lock()->GetHeight(),
						transformid.lock()->GetPosition().z

					});

				projectileManager->GetComponent<BulletFiring>()->TurnFull(turnSpeed, playerid.lock()->GetComponent<Transform>()->GetPosition(), elapsedTime);

			}
		}

	}
	// 構え
	if (animationTime >= 3.89f && !turnPermission)
	{

		for (int i = 0; i < ProjectileManager::Instance().GetProjectileCount(); ++i)
		{

			std::shared_ptr<Actor> projectileManager = ProjectileManager::Instance().GetProjectile(i);
			if (projectileManager->GetComponent<ProjectileThrowing>())
			{
				DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&playerid.lock()->GetComponent<Transform>()->GetPosition());
				DirectX::XMVECTOR projectilePosition = DirectX::XMLoadFloat3(&projectileManager->GetComponent<Transform>()->GetPosition());

				//DirectX::XMVECTOR vectorVec = DirectX::XMVectorSubtract(playerPosition, projectilePosition);
				//vectorVec = DirectX::XMVector3Normalize(vectorVec);
				//DirectX::XMFLOAT3 vector;
				//DirectX::XMStoreFloat3(&vector, vectorVec);


				projectileManager->GetComponent<Transform>()->SetPosition(owner.lock()->GetComponent<Transform>()->GetPosition());
				
			}
		}
	}

	// アニメーション射撃発射
	if (animationTime <= 0.8f && !turnPermission)
	{
		// 強制アニメーション終了
		bool end = true;
		model->SetAnimationEndFlag(end);
		//bool blend = false;

		model->ReverseplaybackAnimation(elapsedTime);
		// アニメーション再生時間
		currentAnimationStartSecondsf = 0.7f;

		//model->PlayAnimation(EnemyBoss::Animation::Anim_Shot, loop, currentAnimationStartSecondsf, blendSeconds);
		// アニメーションルール
		enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);


		turnPermission = true;
	}
	// 射撃許可
	if (!owner.lock()->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation() && turnPermission)
	{
		// 地面判定発生
		bool pushuThrow = true;
		enemyid.lock()->SetPushuThrow(pushuThrow);
		// 球発射
		bool movementCheck = true;
		for (int i = 0; i < ProjectileManager::Instance().GetProjectileCount(); ++i)
		{
			std::shared_ptr<Actor> projectileManager = ProjectileManager::Instance().GetProjectile(i);
			if (projectileManager->GetComponent<ProjectileThrowing>())
			{
				projectileManager->GetComponent<ProjectileThrowing>()->SetMovementCheck(movementCheck);
			}
		}
		
		// 終了アニメーション
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Pursuit));
	}




}
// 終了処理
void AttackShotThrowingState::Exit()
{
}


void DamageState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();

	enemyid.lock()->InputDamageSe();

	modelAnim.index = EnemyBoss::Animation::Anim_Die;

	modelAnim.loop = true;

	modelAnim.currentanimationseconds = 2.5f;

	modelAnim.blendSeconds = 0.7f;

	modelAnim.currentAnimationAddSeconds = 0.025f;

	modelAnim.keyFrameEnd = 3.0f;

	// ダメージ
	//damageSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/炎飛行時.wav");

	//owner.lock()->GetComponent<ModelControll>() = owner.lock()->GetComponent<ModelControll>();
	Model* momdel = owner.lock()->GetComponent<ModelControll>()->GetModel();
	momdel->
		PlayAnimation(
			modelAnim);
	// アニメーションルール
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);

	stateTimer = stateTimerMax;

	// ダメージSe
	//damageSe->Play(loopSe);

}

void DamageState::Execute(float elapsedTime)
{

	--stateTimer;

	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();

		if (stateTimer < stateTimerEnd)
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));

}

void DamageState::Exit()
{
	// ダメージSe停止
	//damageSe->Stop();
}


// 混乱開始
void ConfusionState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	//owner.lock()->GetComponent<ModelControll>() = owner.lock()->GetComponent<ModelControll>();
	//model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	modelAnim.index = EnemyBoss::Animation::Anim_Die;

	modelAnim.loop = false;

	modelAnim.currentanimationseconds = 0.426f;

	modelAnim.blendSeconds = 0.35f;

	modelAnim.currentAnimationAddSeconds = 0.000f;

	modelAnim.keyFrameEnd = 70.0f;


	model->PlayAnimation(modelAnim);
	// アニメーションルール
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);

	// ステート待機時間
	stateTimer = stateTimerMax;

	// 攻撃種類
	randamAttack = rand() % 2;

	// 混乱Se否設定
	//confusionSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/炎飛行時.wav");
	//confusionSe->Play(loopSe);

}
// 混乱更新
void ConfusionState::Execute(float elapsedTime)
{
	stateTimer -= elapsedTime;

	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	//if (stateTimer < stateTimerEnd)
	//{
	//	// ステート変更
	//	//enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Pursuit));
	//	enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Pursuit));
	//}

	//// 任意のアニメーション再生区間でのみ衝突判定処理をする
	//float animationTime = owner.lock()->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	//// ひざまつく
	//if (animationTime >= 1.1)
	//{
	//	// アニメーションルール
	//	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Stop);

	//}

	if (!model->IsPlayAnimation())
		model->PlayAnimation(modelAnim);

	if (stateTimer > stateTimerEnd)return;
	switch (randamAttack)
	{
	case (int)EnemyBoss::AttackMode::AssaultAttack:
	{
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Wander));
		break;
	}
	case (int)EnemyBoss::AttackMode::JumpStompAttack:
	{
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Jump));
		break;
	}
	default:
		break;
	}



}
// 混乱終了
void ConfusionState::Exit()
{
	//confusionSe->Stop();
}


void DeathState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	//std::shared_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// 死亡時Se
	//deathSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/Enemy着地.wav","death");


	modelAnim.index = EnemyBoss::Animation::Anim_Die;

	modelAnim.loop = false;


	Model* model;

	//owner.lock()->GetComponent<ModelControll>() = owner.lock()->GetComponent<ModelControll>();
	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	model->
		PlayAnimation(modelAnim);
	// アニメーションルール
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);

	stateTimer = 3;

	bool freeCheckCamera = false;

	int playerCountMax = PlayerManager::Instance().GetPlayerCount();
	if (playerCountMax > 0)
	PlayerManager::Instance().GetPlayer(playerCountMax - 1)->GetComponent<Player>()->SetFreeCameraCheck(freeCheckCamera);
}

void DeathState::Execute(float elapsedTime)
{
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();



	stateTimer -= elapsedTime; 

	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// 死亡時はそれっぽいカメラアングルで死亡
// 例えばコレを必殺技などで上手く利用できればかっこいいカメラ演出が作れますね
	MessageData::CAMERACHANGEMOTIONMODEDATA	p;

	DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();
	DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentANimationSeconds();

	//if(animationTime + FLT_EPSILON >= 0.5f - FLT_EPSILON && animationTime - FLT_EPSILON <= 0.51f + FLT_EPSILON)
	//deathSe->Play("death",loopSe);


	float vx = sinf(angle.y) * 6;
	float vz = cosf(angle.y) * 6;
	p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
	p.data.push_back({ 95, {position.x + vx + 13, position.y + 3, position.z + vz }, position });

	Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

	if (stateTimer < 0)
	{
		enemyid.lock()->SetClearCheck(clearCheck);
	}
}

void DeathState::Exit()
{
	//deathSe->Stop("death");
}

// 興奮状態
void AwakeStartState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();

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
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);


}

void AwakeStartState::Execute(float elapsedTime)
{
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();

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
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
	}
}

void AwakeStartState::Exit()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	// 覚醒時間
	enemyid.lock()->ResetAwakeTime();
	
}

void ClearState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	//std::shared_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// 死亡時Se
	//deathSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/Enemy着地.wav","death");

	// 位置情報
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();

	position.z -= 5.5f;

	transformid.lock()->SetPosition(position);

	DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();

	angle.y += 1.5f;

	transformid.lock()->SetAngle(angle);

	modelAnim.index = EnemyBoss::Animation::Anim_Die;

	modelAnim.loop = false;

	modelAnim.blendSeconds = 0.7f;

	modelAnim.currentanimationseconds = 1.5f;

	Model* model;

	//owner.lock()->GetComponent<ModelControll>() = owner.lock()->GetComponent<ModelControll>();
	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	model->
		PlayAnimation(modelAnim);
	// アニメーションルール
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);

	stateTimer = 3;

	//bool freeCheckCamera = false;

	//int playerCountMax = PlayerManager::Instance().GetPlayerCount();
	//if (playerCountMax > 0)
	//	PlayerManager::Instance().GetPlayer(playerCountMax - 1)->GetComponent<Player>()->SetFreeCameraCheck(freeCheckCamera);


	//std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();

	
}

void ClearState::Execute(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	//// カメラ関係
	//{
	//	// 死亡時はそれっぽいカメラアングルで死亡
	//// 例えばコレを必殺技などで上手く利用できればかっこいいカメラ演出が作れますね
	//	MessageData::CAMERACHANGEMOTIONMODEDATA	p;

	//	DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();
	//	DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();

	//	//if(animationTime + FLT_EPSILON >= 0.5f - FLT_EPSILON && animationTime - FLT_EPSILON <= 0.51f + FLT_EPSILON)
	//	//deathSe->Play("death",loopSe);
	//	float vx = sinf(angle.y) * 6;
	//	float vz = cosf(angle.y) * 6;

	//	float vx2 = sinf(angle.y) - 10;
	//	float vz2 = cosf(angle.y) * 7;
	//	float vx3 = sinf(angle.y);

	//	p.data.push_back({ 0, {position.x + vx2, position.y + 3, position.z + vz * distance }, position ,false,true});
	//	//p.data.push_back({ 150, {position.x + vx, position.y + 3, position.z + vz * distance }, position });
	//	p.data.push_back({ 180, {position.x + vx2, position.y + 5, position.z - vz2 * distance }, position ,true,true});
	//	p.data.push_back({ 240, {position.x + vx3 , position.y + 1, (position.z + 0.1f) - vz2 * distance }, position ,true,true});


	//	Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);
	//}

	//DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();
	//position.y += 5;
	//position.z += 3;

	//MessageData::CAMERACHANGEFREEMODEDATA	p = { position };
	//
	//Messenger::Instance().SendData(MessageData::CAMERACHANGEFREEMODE, &p);


	stateTimer -= elapsedTime;

	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	//std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();


	//DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();
	//DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();



	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentANimationSeconds();

	const GamePadButton anyButton =
		GamePad::BTN_B;

	if (!model->IsPlayAnimation() )// ロードの次ゲームという書き方
	{
		enemyid.lock()->SetClearCheck(clearCheck);
	}


}

void ClearState::Exit()
{
}

void ClearReviveState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();


	// 位置情報
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();

	position.z -= 5.5f;

	transformid.lock()->SetPosition(position);

	DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();

	angle.y += 1.5f;

	transformid.lock()->SetAngle(angle);

	modelAnim.index = EnemyBoss::Animation::Anim_Die;

	modelAnim.loop = false;

	modelAnim.currentanimationseconds = 1.5f;

	modelAnim.blendSeconds = 0.7f;

	Model* model;

	//owner.lock()->GetComponent<ModelControll>() = owner.lock()->GetComponent<ModelControll>();
	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	model->
		PlayAnimation(modelAnim);
	// アニメーションルール
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);

	stateTimer = 3;
}

void ClearReviveState::Execute(float elapsedTime)
{
}

void ClearReviveState::Exit()
{
}


// プレイヤー
void PlayerIdleState::Enter()
{
	//playerid = owner.lock()->GetComponent<Player>();

	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();


	//modelid = owner.lock()->GetComponent<ModelControll>();

	modelAnim.index = Player::Anim_Idle;

	modelAnim.loop = true;

	modelAnim.blendSeconds = 0.5f;

	model->PlayAnimation(modelAnim);
	
	// アニメーションルール
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
	

	// 落ちる
	//bool stop = false;
	moveid.lock()->SetStopMove(false);
}

void PlayerIdleState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	// ロックオン処理
	playerid.lock()->UpdateCameraState(elapsedTime);

	// 移動入力処理
	if (playerid.lock()->InputMove(elapsedTime))
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
		//TransitionMoveState();
	}

	// 反射入力処理
	if (playerid.lock()->InputAvoidance())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Reflection));

	}


	// ジャンプ入力処理
	if (playerid.lock()->InputJump())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
		//TransitionJumpState();
	}



}

void PlayerIdleState::Exit()
{
}

void PlayerMovestate::Enter()
{
	//owner.lock()->GetComponent<Player>() = owner.lock()->GetComponent<Player>();


	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// se再生
	playerid.lock()->InputWalkSE();


	// 歩きSe否設定
	//walkSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/足音.wav");
	//if(moveid.lock()->GetOnLadius())
	//walkSe->Play(loopSe);

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	modelAnim.index = Player::Anim_Running;

	modelAnim.loop = true;

	modelAnim.blendSeconds = 0.7f;

	modelAnim.currentAnimationAddSeconds = 0.025f;


	//modelid = owner.lock()->GetComponent<ModelControll>();

		//Player::Anim_Move, loop,
	model->PlayAnimation(modelAnim);

	//	//Player::Anim_Move, loop,
	//owner.lock()->GetComponent<ModelControll>()->GetModel()->PlayUpeerBodyAnimation(
	//	Player::Anim_Running, loop,
	//	currentAnimationStartSeconds, blendSeconds
	//);


	// アニメーションルール
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);


	// 落ちる
	//bool stop = false;
	moveid.lock()->SetStopMove(false);

	// SE再生
	//playerid.lock()->PlaySE()->Play("walk");
	//playerid.lock()->PlaySE()->SetSpeed("walk",0.5f);
}

void PlayerMovestate::Execute(float elapsedTime)
{


	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	//std::shared_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	// 移動値
	playerid.lock()->GetMoveVec(elapsedTime);

	// ロックオン処理
	playerid.lock()->UpdateCameraState(elapsedTime);

	// 移動入力処理
	if (!playerid.lock()->InputMove(elapsedTime))
	{
		//walkSe->Stop();
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
		//TransitionIdleState();
	}


	// 回避入力処理
	if (playerid.lock()->InputAvoidance())
	{
		//walkSe->Stop();
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

	}

	// ジャンプ入力処理
	if (playerid.lock()->InputJump())
	{
		//walkSe->Stop();
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
		
	}



}

void PlayerMovestate::Exit()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	//se再生削除
	playerid.lock()->InputStopWalkSE();

}


void PlayerJumpState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	// 否設定Se
	//janpSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/足音.wav");
	//janpSe->Play(loopSe);

	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	//model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	//moveid = owner.lock()->GetComponent<Movement>();

	modelAnim.index = Player::Animation::Anim_Jump;

	modelAnim.keyFrameEnd = 46.0f;

	

	model->PlayAnimation(modelAnim);

	// アニメーションルール
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);


	// 落ちる
	//bool stop = false;
	moveid.lock()->SetStopMove(false);

	// se作成
	playerid.lock()->InputJampSE();

	//playerid.lock()->PlaySE()->Play("land");
}

void PlayerJumpState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	//Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();


	// ロックオン処理
	playerid.lock()->UpdateCameraState(elapsedTime);

	//// 移動
	//if (playerid.lock()->InputMove(elapsedTime))
	//{
	//	playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	//}
	// 移動
	if (playerid.lock()->InputMove(elapsedTime))
	{
		playerid.lock()->GetMoveVec(elapsedTime);
	}

	// ジャンプ入力処理
	if (playerid.lock()->InputJump())
	{
		
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
	}




	// 回避
	if (playerid.lock()->InputAvoidance())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}




	// 着地
	if (moveid.lock()->GetOnLadius())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Land));
	}

	// 着地移動
	if (moveid.lock()->GetOnLadius() && playerid.lock()->InputMove(elapsedTime))
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	}





}

void PlayerJumpState::Exit()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	// ジャンプ音
	playerid.lock()->InputJampSE();

	//janpSe->Stop();
}


void PlayerLandState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	//// Se
	//landSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/足音.wav");
	//landSe->Play(loopSe);

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	//std::shared_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	//model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	//moveid = owner.lock()->GetComponent<Movement>();
	//moveid->SetOnLadius(false);

	modelAnim.index = Player::Anim_Land;

	modelAnim.currentanimationseconds = 0.35f;

	model->PlayAnimation(modelAnim);

	// アニメーションルール
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);

	// se作成
	playerid.lock()->InputJampSE();
}

void PlayerLandState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	// ロックオン処理
	playerid.lock()->UpdateCameraState(elapsedTime);

	//// 任意のアニメーション再生区間でのみ衝突判定処理をする
	//float animationTime = model->GetCurrentANimationSeconds();
	//// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。

	// 歩き
	if (playerid.lock()->InputMove(elapsedTime))
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	}

		// 回避
	if (playerid.lock()->InputAvoidance())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}

	// 待機
	if (!model->IsPlayAnimation())
	{
		
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


	}
}

void PlayerLandState::Exit()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

		// ジャンプ音
	playerid.lock()->InputJampSE();
}


void PlayerJumpFlipState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	//// Se
	//janpSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/足音.wav");
	//janpSe->Play(loopSe);

	//std::shared_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	/*model = owner.lock()->GetComponent<ModelControll>()->GetModel();*/
	//moveid = owner.lock()->GetComponent<Movement>();

	modelAnim.index = Player::Anim_Jump;

	modelAnim.currentAnimationAddSeconds = 0.03f;

	modelAnim.keyFrameEnd = 25.0f;

	model->PlayAnimation(modelAnim);

	// アニメーションルール
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);

	// se作成
	playerid.lock()->InputJampSE();
}

void PlayerJumpFlipState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	// ロックオン処理
	playerid.lock()->UpdateCameraState(elapsedTime);
	// 移動
	if (playerid.lock()->InputMove(elapsedTime))
	{
		
		bool afterimagemove = true;
	}
	// ジャンプ入力処理
	if (!model->IsPlayAnimation())
	{

		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
	}

	// 回避
	if (playerid.lock()->InputAvoidance())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}

}

void PlayerJumpFlipState::Exit()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	playerid.lock()->InputStopJampSE();
}

void PlayerQuickJabState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	/*slashSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/スラッシュ１回目.wav");*/

	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	//model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	//moveid = owner.lock()->GetComponent<Movement>();
	//transformid = owner.lock()->GetComponent<Transform>();

	//std::shared_ptr<Player> playerId = owner.lock()->GetComponent<Player>();

		//Player::Anim_Slash, loop,
		// １回
	
	if (!moveid.lock()->GetOnLadius())
	{
		// 空中攻撃で行動階数減少
		playerid.lock()->AreAttackDecreaseAmount();

		// アニメーションルール
		Model::ModelAnim modelAnim;
		Model::ModelAnim modelAnimUpperBody;

		modelAnim.index = Player::Anim_Slash;
		modelAnimUpperBody.index = Player::Anim_Jump;

		// 再生開始時間 
		//float currentAnimationStartSeconds = 0.6f;
		//float currentAnimationUpperStartSeconds = 0.0f;
		modelAnim.currentanimationseconds = 0.6f;

		// ループ
		//bool loop = true;
		//bool loopUpper = false;
		modelAnim.loop = true;

		// 再生時間加算分の値
		//float currentAnimationAddSeconds = 0.03f;
		//float currentAnimationUpperAddSeconds = 0.03f;
		modelAnim.currentAnimationAddSeconds = 0.03f;
		modelAnimUpperBody.currentAnimationAddSeconds = 0.03f;

		//float keyFrameEnd = 40.0f;
		modelAnimUpperBody.keyFrameEnd = 40.0f;

		// アニメーション再生
		model->PlayAnimation(modelAnim);


		// アニメーション上半身再生
		model->PlayUpeerBodyAnimation(modelAnimUpperBody);
		// 攻撃三回で一度強制終了
		++attackMemory;

		// アニメーション再生
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Doble);

		// エフェクト竜巻
		//areWork = std::make_unique<Effect>("Data/Effect/tornade.efk");

		// エフェクト再生
		//areWork->Play(transformid->GetPosition());
	}
	else
	{
		//// 再生開始時間 
		//float currentAnimationStartSeconds = 0.0f;

		//// ループ
		//bool loop = false;

		//// 再生時間加算分の値
		//float currentAnimationAddSeconds = 0.03f;

		// アニメーションルール
		Model::ModelAnim modelAnim;

		modelAnim.index = Player::Animation::Anim_Slash;

		modelAnim.currentAnimationAddSeconds = 0.03f;

		// アニメーション再生
		model->PlayAnimation(modelAnim);
		// 攻撃三回で一度強制終了
		++attackMemory;

		// アニメーション再生
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
	}
	

	if (!InitializationCheck)
	{

		// コマンド設定二回目攻撃
		commandSeconde.push_back(GamePad::BTN_B);
		commandSeconde.push_back(GamePad::BTN_B);

		// コマンド設定三回目攻撃
		commandThrede.push_back(GamePad::BTN_B);
		commandThrede.push_back(GamePad::BTN_B);
		commandThrede.push_back(GamePad::BTN_B);
	}


	
	//moveid->SetGravity(gravity);
	// 重力オフ
	bool stopFall = true;
	moveid.lock()->SetStopFall(stopFall);



	// 移動の停止
	//bool stopMove = true;
	//moveid->SetStopMove(stopMove);

	// 回転するかチェック
	rotateCheck = false;


	// ダメージ食らった時に攻撃を１からに戻すため
	deleteCheck = true;

	InitializationCheck = true;




}

void PlayerQuickJabState::Execute(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	// コンポネント取得
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// ロックオン処理
	playerid.lock()->UpdateCameraState(elapsedTime);



	if (playerid.lock()->InputAttack() && 
		playerid.lock()->GetSelectCheck() ==
		(int)Player::CommandAttack::Attack)
	{
		// コマンド確認2弾攻撃
		if (gamePad.ConfirmCommand(commandSeconde, frame) && !button && !buttonSeconde)
		{
			button = true;
			//owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
		}
	}

	// 回避
	if (playerid.lock()->InputAvoidance() && playerid.lock()->InputMove(elapsedTime))
	{
		bool stop = false;
		moveid.lock()->SetStopMove(stop);

		DirectX::XMFLOAT3 vec;
		vec = playerid.lock()->GetMoveVec(elapsedTime);

		moveid.lock()->Turn(vec, turnSpeed,elapsedTime);

		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

		button = false;
		buttonSeconde = false;

		return;
	}

	DirectX::XMVECTOR Vector;
	DirectX::XMVECTOR LengthSq;

	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid.lock()->GetPosition());
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
	//// 相手の方を向く
	//if (length > attackCheckRangeMin && length < attackCheckRange && moveid->GetOnLadius())
	//{
	//	bool stop = false;
	//	moveid->SetStopMove(stop);

	//	//rotateCheck = true;

	//	//angle = transformid->GetAngle();

	//	DirectX::XMFLOAT3 direction = vector;

	//	//direction.x = sinf(angle.y) * 6;
	//	//direction.y = 0;
	//	//direction.z = cosf(angle.y) * 6;

	//	moveid->Turn(direction,turnSpeed,elapsedTime);

	//	//moveid->Move(direction, attackSpeed, elapsedTime);
	//}
	//else
	//{

	//	bool stop = true;
	//	moveid->SetStopMove(stop);

	//}


	// 相手の方を向く
	// 回転
	if (!rotateCheck)
	{

			// 距離
			if (length < attackCheckRange && length > attackCheckRangeMin
				&& moveid.lock()->TurnCheck(
					vector, angleRange, elapsedTime)&&
				moveid.lock()->GetOnLadius())
			{
				//bool stop = false;
				//moveid->SetStopMove(stop);
				bool stop = false;
				moveid.lock()->SetStopMove(stop);
				// 正面
				if (moveid.lock()->Turn(vector, turnSpeed, elapsedTime))
				{

					moveid.lock()->Move(vector, speed, elapsedTime);
					
				}
				//stop = true;
				//moveid->SetStopMove(stop);
			}
			else
			{


				bool stop = true;
				moveid.lock()->SetStopMove(stop);

				rotateCheck = true;
			}


		

	}
	if (attackMemory > attackMemoryMax)
	{
		// 入力確認でステート変更
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


		// 攻撃最大値調整
		attackMemory = 0;
	}


	
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = moveid.lock()->GetOnLadius() ? 
		model->GetCurrentANimationSeconds() : 
		model->GetCurrentAnimationSecondsUpeer();
	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。


	// 1撃目
	if (animationTime >= 0.8f )
	{
		// １回目の攻撃なら
		oneAttackCheck = false;

		playerid.lock()->SetAttackCollisionFlag(false);
		deleteCheck = false;
		// 入力確認でステート変更
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(button  ? Player::State::SideCut : Player::State::Move));
		return;
	}



	if (playerid.lock()->CollisionNodeVsEnemies("mixamorig:LeftHand",
		owner.lock()->GetComponent<Player>()->GetLeftHandRadius(),
		"body2", "boss_left_hand2", "boss_right_hand2"))
	{
		//slashSe->Play(loopSe);
	}
}

void PlayerQuickJabState::Exit()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// 移動の停止
	moveid.lock()->SetGravity(
		playerid.lock()->GetGravity());


	// 歩き許可
	bool stopMove = false;
	moveid.lock()->SetStopMove(stopMove);

	// 重力オフ
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);
	if (deleteCheck )
	{
		// ダメージ食らったかどうか
		button = false;
		buttonSeconde = false;

		// 攻撃最大値調整
		attackMemory = 0;
	}

	//slashSe->Stop();

}


void PlayerSideCutState::Enter()
{
	bool loop = false;

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// Se
	//slashSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/スラッシュ２回目.wav");

	// 二回

	if (!moveid.lock()->GetOnLadius())
	{
		// アニメーションルール
		Model::ModelAnim modelAnim;
		Model::ModelAnim modelAnimUpperBody;
		modelAnim.index = Player::Animation::Anim_Jump;
		modelAnimUpperBody.index = Player::Animation::Anim_SlashBeside;

		// 再生開始時間 
		//float currentAnimationStartSeconds = 0.6f;
		//float currentAnimationUpperStartSeconds = 0.0f;
		modelAnim.currentanimationseconds = 0.6f;

		// ループ
		bool loop = true;
		//bool loopUpper = false;
		modelAnim.loop = true;

		// 再生時間加算分の値
		//float currentAnimationAddSeconds = 0.03f;
		//float currentAnimationUpperAddSeconds = 0.03f;
		modelAnim.currentAnimationAddSeconds = 0.03f;
		modelAnimUpperBody.currentAnimationAddSeconds = 0.03f;


		//float keyFrameEnd = 0.63f;
		modelAnim.keyFrameEnd = 0.63f;

		// アニメーション再生
		model->PlayAnimation(modelAnim);


		// アニメーション上半身再生
		model->PlayUpeerBodyAnimation(modelAnimUpperBody);
		// 攻撃三回で一度強制終了
		++attackMemory;

		// アニメーション再生
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Doble);
	}
	else
	{
		// アニメーションルール
		Model::ModelAnim modelAnim;

		//// 再生開始時間 
		//float currentAnimationStartSeconds = 0.2f;

		//// ループ
		//bool loop = false;

		//// 再生時間加算分の値
		//float currentAnimationAddSeconds = 0.03f;

		modelAnim.index = Player::Animation::Anim_SlashBeside;

		modelAnim.currentanimationseconds = 0.2f;

		modelAnim.currentAnimationAddSeconds = 0.03f;


		// アニメーション再生
		model->PlayAnimation(modelAnim);
		// 攻撃三回で一度強制終了
		++attackMemory;

		// アニメーション再生
	    playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
	}
		//// 再生開始
		//currentAnimationStartSeconds = 0.2f;

		//// アニメーション再生
		//model->GetModel()->PlayAnimation(
		//	Player::Anim_SlashBeside, loop,
		//	currentAnimationStartSeconds, blendSeconds
		//	, currentAnimationAddSeconds
		//);

	

	if (!InitializationCheck)
	{

		// コマンド設定二回目攻撃
		commandSeconde.push_back(GamePad::BTN_B);


		// コマンド設定三回目攻撃
		commandThrede.push_back(GamePad::BTN_B);


	}
	// アニメーション再生
	//owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


	//moveid->SetGravity(gravity);
	// 重力オフ
	bool stopFall = true;
	moveid.lock()->SetStopFall(stopFall);

	// 移動の停止
	//bool stopMove = true;
	//moveid->SetStopMove(stopMove);

	// 回転するかチェック
	rotateCheck = false;


	InitializationCheck = true;
}

void PlayerSideCutState::Execute(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();


	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// ロックオン処理
	playerid.lock()->UpdateCameraState(elapsedTime);



	//angle = transformid->GetAngle();

	//DirectX::XMFLOAT3 direction;
	//direction.x = sinf(angle.y) * 6;
	//direction.y = 0;
	//direction.z = cosf(angle.y) * 6;

	//moveid->Move(direction, attackSpeed, elapsedTime);

	if (playerid.lock()->InputAttack() &&
		playerid.lock()->GetSelectCheck() ==
		(int)Player::CommandAttack::Attack)
	{
		// コマンド確認3弾攻撃
		if (gamePad.ConfirmCommand(commandThrede, frame))
		{
			buttonSeconde = true;
			//owner.lock()->GetComponent<Pl/ayer>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
		}

	}

	// 回避
	if (playerid.lock()->InputAvoidance() && playerid.lock()->InputMove(elapsedTime))
	{
		bool stop = false;
		moveid.lock()->SetStopMove(stop);

		DirectX::XMFLOAT3 vec;
		vec = owner.lock()->GetComponent<Player>()->GetMoveVec(elapsedTime);

		moveid.lock()->Turn(vec, turnSpeed, elapsedTime);

		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

		button = false;
		buttonSeconde = false;

		return;
	}


	DirectX::XMVECTOR Vector;
	DirectX::XMVECTOR LengthSq;

	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid.lock()->GetPosition());
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
		moveid.lock()->SetStopMove(stop);

		DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();

		DirectX::XMFLOAT3 direction;
		direction.x = sinf(angle.y) * 6;
		direction.y = 0;
		direction.z = cosf(angle.y) * 6;

		//moveid->Move(direction, attackSpeed, elapsedTime);
	}
	else if (!moveid.lock()->GetOnLadius())
	{
	
		bool stop = true;
		moveid.lock()->SetStopMove(stop);

	}

	// 回転
	if (!rotateCheck && !playerid.lock()->InputAvoidance())
	{
		//DirectX::XMVECTOR Vector;
		//DirectX::XMVECTOR LengthSq;

		//DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&owner.lock()->GetComponent<Transform>()->GetPosition());
		//EnemyManager& enemyManager = EnemyManager::Instance();
		//int enemyCount = enemyManager.GetEnemyCount();
		//for (int i = 0; i < enemyCount; ++i)//float 最大値ないにいる敵に向かう
		//{
		//	DirectX::XMVECTOR enemyPosition = DirectX::XMLoadFloat3(&enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition());


		//	Vector = DirectX::XMVectorSubtract(enemyPosition, playerPosition);

		//	LengthSq = DirectX::XMVector3Length(Vector);

		//	DirectX::XMStoreFloat3(&vector, Vector);

		//	DirectX::XMStoreFloat(&length, LengthSq);
			// 距離
			if (length < attackCheckRange && length > attackCheckRangeMin
				&& moveid.lock()->TurnCheck(
					vector, angleRange, elapsedTime))
			{
				//bool stop = false;
				//moveid->SetStopMove(stop);

				// 正面
				if (moveid.lock()->Turn(vector, turnSpeed, elapsedTime))
				{

					moveid.lock()->Move(vector, speed, elapsedTime);

				}
				//stop = true;
				//moveid->SetStopMove(stop);
			}
			else
			{



				rotateCheck = true;
			}


		

	}



	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = moveid.lock()->GetOnLadius() ?
		model->GetCurrentANimationSeconds() :
		model->GetCurrentAnimationSecondsUpeer();
	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。

	//owner.lock()->GetComponent<Player>()->InputAvoidance();

	// 2撃目
	if (animationTime >= 1.1f)
	{
		playerid.lock()->SetAttackCollisionFlag(false);
		deleteCheck = false;



		// 入力確認でステート変更
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(buttonSeconde ? Player::State::CycloneStrike : Player::State::Move));

		return;
	}


	if (playerid.lock()->CollisionNodeVsEnemies("mixamorig:LeftHand"
		, playerid.lock()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2"))
	{
		//slashSe->Play(loopSe);
	}

}

void PlayerSideCutState::Exit()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// 移動の停止
	moveid.lock()->SetGravity(
		playerid.lock()->GetGravity());


	// 歩き許可
	bool stopMove = false;
	moveid.lock()->SetStopMove(stopMove);

	// 重力オフ
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);

	//slashSe->Stop();
	
}

void PlayerCycloneStrikeState::Enter()
{
	bool loop = false;

	// コンポネント取得
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// se
	//slashSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/３撃目.wav");


	if (!moveid.lock()->GetOnLadius())
	{
		//// 再生開始時間 
		//float currentAnimationStartSeconds = 0.6f;
		//float currentAnimationUpperStartSeconds = 0.0f;

		//// ループ
		//bool loop = true;
		//bool loopUpper = false;

		//// 再生時間加算分の値
		//float currentAnimationAddSeconds = 0.03f;
		//float currentAnimationUpperAddSeconds = 0.03f;

		//float keyFrameEnd = 0.63f;

		// アニメーション再生
		//model->GetModel()->PlayAnimation(
		//	Player::Anim_Jump, loop,
		//	currentAnimationStartSeconds, blendSeconds
		//	, currentAnimationAddSeconds, keyFrameEnd
		//);

	    // アニメーションルール
		Model::ModelAnim modelAnimUpper;

		modelAnimUpper.index = Player::Animation::Anim_SlashThree;

		modelAnimUpper.currentanimationseconds = 0.03f;


		// アニメーション上半身再生
		model->PlayUpeerBodyAnimation(modelAnimUpper);
		// 攻撃三回で一度強制終了
		++attackMemory;

		// アニメーション再生
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Doble);
	}
	else
	{
		//// 再生開始時間 
		//float currentAnimationStartSeconds = 0.7f;

		//// ループ
		//bool loop = false;

		//// 再生時間加算分の値
		//float currentAnimationAddSeconds = 0.03f;

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
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
	}
		//// 再生開始
		//currentAnimationStartSeconds = 0.7f;
		//// アニメーション再生
		//model->GetModel()->PlayAnimation(
		//	Player::Anim_SlashThree, loop,
		//	currentAnimationStartSeconds, blendSeconds
		//	, currentAnimationAddSeconds
		//);



	if (!InitializationCheck)
	{

		// コマンド設定二回目攻撃
		commandSeconde.push_back(GamePad::BTN_B);
		commandSeconde.push_back(GamePad::BTN_B);

		// コマンド設定三回目攻撃
		commandThrede.push_back(GamePad::BTN_B);
		commandThrede.push_back(GamePad::BTN_B);
		commandThrede.push_back(GamePad::BTN_B);
	}
	// アニメーション再生
	//owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

	// 重力オフ
	bool stopFall = true;
	moveid.lock()->SetStopFall(stopFall);

	//// 移動の停止
	//bool stopMove = true;
	//moveid->SetStopMove(stopMove);

	// 回転するかチェック
	rotateCheck = false;


	InitializationCheck = true;
}

void PlayerCycloneStrikeState::Execute(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	// コンポネント取得
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();


	// ロックオン処理
	playerid.lock()->UpdateCameraState(elapsedTime);


	//angle = transformid->GetAngle();

	//DirectX::XMFLOAT3 direction;
	//direction.x = sinf(angle.y) * 6;
	//direction.y = 0;
	//direction.z = cosf(angle.y) * 6;

	//moveid->Move(direction, attackSpeed, elapsedTime);

	// 回避
	if (playerid.lock()->InputAvoidance() && playerid.lock()->InputMove(elapsedTime))
	{
		bool stop = false;
		moveid.lock()->SetStopMove(stop);

		DirectX::XMFLOAT3 vec;
		vec = playerid.lock()->GetMoveVec(elapsedTime);

		moveid.lock()->Turn(vec, turnSpeed, elapsedTime);

		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

		button = false;
		buttonSeconde = false;

		return;
	}


	DirectX::XMVECTOR Vector;
	DirectX::XMVECTOR LengthSq;

	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid.lock()->GetPosition());
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
		moveid.lock()->SetStopMove(stop);

		DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();

		DirectX::XMFLOAT3 direction;
		direction.x = sinf(angle.y) * 6;
		direction.y = 0;
		direction.z = cosf(angle.y) * 6;

		//moveid->Move(direction, attackSpeed, elapsedTime);
	}
	else if (!moveid.lock()->GetOnLadius())
	{

		bool stop = true;
		moveid.lock()->SetStopMove(stop);

	}

	// 回転
	if (!rotateCheck)
	{

			// 距離
			if (length < attackCheckRange && length > attackCheckRangeMin
				&& moveid.lock()->TurnCheck(
					vector, angleRange, elapsedTime))
			{
				//bool stop = false;
				//moveid->SetStopMove(stop);

				// 正面
				if (moveid.lock()->Turn(vector, turnSpeed, elapsedTime))
				{

					moveid.lock()->Move(vector, speed, elapsedTime);

				}
				//stop = true;
				//moveid->SetStopMove(stop);
			}
			else
			{



				rotateCheck = true;
			}


		

	}



	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = moveid.lock()->GetOnLadius() ?
		model->GetCurrentANimationSeconds() :
		model->GetCurrentAnimationSecondsUpeer();
	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。


	// 3撃目
	if (animationTime >= 2.7f )
	{
		playerid.lock()->SetAttackCollisionFlag(false);
		deleteCheck = false;



		// 入力確認でステート変更
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
		return;
	}


	if (playerid.lock()->CollisionNodeVsEnemies("mixamorig:LeftHand",
		playerid.lock()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2"))
	{
		//slashSe->Play(loopSe);
	}

}

void PlayerCycloneStrikeState::Exit()
{
	// コンポネント取得
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// 移動の停止
	moveid.lock()->SetGravity(
		playerid.lock()->GetGravity());



	// 歩き許可
	bool stopMove = false;
	moveid.lock()->SetStopMove(stopMove);

	// 重力オフ
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);

	//slashSe->Stop();

}

void PlayerSpecialAttackState::Enter()
{
	// コンポネント取得
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	//transformid = owner.lock()->GetComponent<Transform>();
	//model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	//moveid = owner.lock()->GetComponent<Movement>();

	// se 登録
	//slashSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/スラッシュ２回目.wav");
	//lighteningStrikeSpecialSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/必殺技雷.wav");
	//lighteningStrikeSpecialSaveSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/必殺技雷ため.wav");

	loopSe = false;
	//lighteningStrikeSpecialSe->Play(loopSe);

	std::weak_ptr<Transform> enemyTransform = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<Transform>();
	std::weak_ptr<HP> enemyHpId = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<HP>();

	lightning = std::make_unique<Effect>("Data/Effect/sunder.efk");
	lightningAttack = std::make_unique<Effect>("Data/Effect/HitThunder.efk");
	lightningHit = std::make_unique<Effect>("Data/Effect/lightningStrike.efk");




	if (!moveid.lock()->GetOnLadius())
	{
		//// 再生開始時間 
		//float currentAnimationStartSeconds = 0.6f;
		//float currentAnimationUpperStartSeconds = 0.0f;

		//// ループ
		//bool loop = true;
		//bool loopUpper = false;

		//// 再生時間加算分の値
		//float currentAnimationAddSeconds = 0.00f;


		//// アニメーションブレンド
		//float blendSeconds = 0.5f;

		//float keyFrameEnd = 0.63f;

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
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Doble);
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
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);

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
	moveid.lock()->SetStopFall(stopFall);

	button = true;

	// フラッシュ
	flashOn = true;


	playerid.lock()->InputAttackThanderSE();


	//p.data.push_back({ 180, {position.x + (pos.x * length) ,
	//position.y + (pos.y * endLength) + 1,
	//position.z + (pos.z * endLength) }, position });

	//p.data.push_back({ 180, position, {(position.x * pos.x) * length ,
	//	(position.y * pos.y) * length ,
	//	(position.z * pos.z) * length } });




	// 回転許可
	isRotate = true;

	bool specialRockOff = true;
	playerid.lock()->SetSpecialRockOff(specialRockOff);

	//// 加速
	//currentAnimationAddSeconds = 0.0f;
}

void PlayerSpecialAttackState::Execute(float elapsedTime)
{
	// コンポネント取得
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<HP> enemyHpId ;

	// エネミーの前に向く
	{
		DirectX::XMVECTOR Vector;
		DirectX::XMVECTOR LengthSq;

		DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid.lock()->GetPosition());
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
			//length < attackCheckRangeMax && length > attackCheckRangeMin&&
	
			if (moveid.lock()->Turn(vector, turnSpeed, elapsedTime))
			{
				// カメラ向き
				{
					MessageData::CAMERACHANGEMOTIONMODEDATA	p;

					position = transformid.lock()->GetPosition();
					angle = transformid.lock()->GetAngle();

					float vx = sinf(angle.y) * 6;
					float vz = cosf(angle.y) * 6;

					float vx2 = sinf(angle.y) - 10;
					float vz2 = cosf(angle.y) * 7;
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
		float animationTime = moveid.lock()->GetOnLadius() ?
			model->GetCurrentANimationSeconds() :
			model->GetCurrentAnimationSecondsUpeer();

		// アニメーション
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON)
		{

			if (!moveid.lock()->GetOnLadius())
			{
				//// 再生開始時間 
				//float currentAnimationStartSeconds = 0.6f;
				//float currentAnimationUpperStartSeconds = 0.0f;

				//// ループ
				//bool loop = true;
				//bool loopUpper = false;

				//// 再生時間加算分の値
				//float currentAnimationAddSeconds = 0.00f;


				//// アニメーションブレンド
				//float blendSeconds = 0.5f;

				//float keyFrameEnd = 0.63f;

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
				//// 再生ループ
				//bool  loop = false;

				//// 再生開始時間 
				//float currentAnimationStartSeconds = 0.0f;
				//// 再生時間加算分の値
				//float currentAnimationAddSeconds = 0.00f;

				//// アニメーションブレンド
				//float blendSeconds = 0.5f;

				// アニメーションルール
				Model::ModelAnim modelAnim;

				modelAnim.index = Player::Animation::Anim_SpecialAttack;

				// アニメーション再生
				model->PlayAnimation(modelAnim);
			}
			button = false;


			lightning->Stop(lightning->GetEfeHandle());

			//Se
			//lighteningStrikeSpecialSe->Play(loopSe);
			loopSe = true;
			//lighteningStrikeSpecialSaveSe->Play(loopSe);


			//rockCheck = true;
			//owner.lock()->GetComponent<Player>()->SetRockCheck(rockCheck);

			// 雷停止
			playerid.lock()->InputStopAttackThanderSE();
			
		}
	}
	else
	{
	
			// 任意のアニメーション再生区間でのみ衝突判定処理をする
			float animationTime = moveid.lock()->GetOnLadius() ?
				model->GetCurrentANimationSeconds() :
				model->GetCurrentAnimationSecondsUpeer();

		// スローモーションと色を明るく
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON)
		{
			//playerid.lock()->SetFlashOn(flashOn);
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


			
			playerid.lock()->SetHitCheck(true);
		}
		// アニメーション切りつけ
		if (animationTime >= 1.6f)
		{
			/*float invincibleTimer = 0.0f;
			owner.lock()->GetComponent<HP>()->SetInvincibleTimer(invincibleTimer);*/

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
			playerid.lock()->InputAttackSlashSpecileLightningStrikeSE();
			
			if (enemyHpId.lock()->ApplyDamage(10, 0.5f)) 
			{
				
				lightningHit->Play(pPosition);
			}
			playerid.lock()->SetHitCheck(false);

			//owner.lock()->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner.lock()->GetComponent<Player>()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");
		}
		bool animationEnd = moveid.lock()->GetOnLadius() ?
			model->IsPlayAnimation() :
			model->IsPlayUpeerBodyAnimation();

		// アニメーション終了
		if (!animationEnd)
		{
			// 入力確認でステート変更
			playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));

		}
	}

}

void PlayerSpecialAttackState::Exit()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// 落ちるの再開
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);


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


	//se 停止
	//slashSe->Stop();
	//lighteningStrikeSpecialSe->Stop();
	//lighteningStrikeSpecialSaveSe->Stop();

	//rockCheck = false;
	//owner.lock()->GetComponent<Player>()->SetRockCheck(rockCheck);

	//// フリーカメラのロックをとどめる
	//bool freeCameraCheck = true;
	//owner.lock()->GetComponent<Player>()->SetFreeCameraCheck(freeCameraCheck);
	bool specialRockOff = false;
	playerid.lock()->SetSpecialRockOff(specialRockOff);
}

void PlayerMagicState::Enter()
{

	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// Se
	//flameStarteSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/炎着弾時.wav");
	//flameimpactSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/炎飛行時.wav");

	//iceStarteSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/氷発射.wav");
	//iceimpactSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/氷着弾時.wav");

	//lightningSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/雷.wav");

	//// サンダー系
	//if (playerid.lock()->GetSelectMagicCheck() ==
	//	(int)Player::CommandMagic::Thander)
	//{
	//	// アニメーション再生
	//	model->PlayAnimation(
	//		Player::Anim_Magic, loop,
	//		currentAnimationStartSeconds, blendSeconds
	//	);

	//	// 雷se
	//	lightningSe->Play(loopSe);
	//}
	//else
	//{
	//	// アニメーション再生
	//	model->PlayAnimation(
	//		Player::Anim_Magic, loop,
	//		currentAnimationStartSeconds, blendSeconds
	//	);

	//}


	magicType = playerid.lock()->GetSelectMagicCheck();

	switch (magicType)
	{
	case (int)Player::CommandMagic::Fire:
	{
		modelAnim.index = Player::Animation::Anim_Magic;
		// アニメーション再生
		model->PlayAnimation(modelAnim);


		break;
	}
	case (int)Player::CommandMagic::Ice:
	{
		modelAnim.index = Player::Animation::Anim_Magic;

		// アニメーション再生
		model->PlayAnimation(modelAnim);

		break;
	}

	case ((int)Player::CommandMagic::Thander):
	{
		modelAnim.index = Player::Animation::Anim_MagicSeconde;
		// アニメーション再生
		model->PlayAnimation(modelAnim);


		break;
	}

	case ((int)Player::CommandMagic::Heale):
	{
		modelAnim.index = Player::Animation::Anim_MagicSeconde;
		// アニメーション再生
		model->PlayAnimation(modelAnim);
		// エフェクト回復
		heale = std::make_unique<Effect>("Data/Effect/heale.efk");

		// エフェクト再生
		heale->Play(transformid.lock()->GetPosition());

		break;
	}

	default:
		break;
	}

	// アニメーション再生
	owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


	// 落ちるの停止
	bool stopFall = true;
	moveid.lock()->SetStopFall(stopFall);


	// 回転許可
	isRotate = true;

	// 移動許可
	isMove = false;

	// SE
	//playerid.lock()->PlaySE()->Play("flame");
	
}

void PlayerMagicState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// ロックオン処理
	playerid.lock()->UpdateCameraState(elapsedTime);

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentANimationSeconds();
	// アニメーション終わったら通る


	DirectX::XMVECTOR vectorXM;
	DirectX::XMVECTOR lengthSqXM;

	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid.lock()->GetPosition());
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

	// 魔法内ながら移動
	if (playerid.lock()->InputMove(elapsedTime) && !isMove
		&& magicType != (int)Player::CommandMagic::Heale)
	{
		bool stop = false;
		moveid.lock()->SetStopMove(stop);

		// 移動値
		playerid.lock()->GetMagicMoveVec(elapsedTime);
	}
	else
	{
		// 入った瞬間から移動していないといみなし
		isMove = true;
	}



	

	// 相手の方を向く
	// 回転
	if (isRotate)
	{

		// 距離
		if (moveid.lock()->TurnCheck(
				vector, angleRange, elapsedTime) &&
			playerid.lock()->GetRockCheck())
		{
			//bool stop = false;
			//moveid->SetStopMove(stop);

			// 正面
			moveid.lock()->Turn(vector,turnSpeed,elapsedTime);
			//stop = true;
			//moveid->SetStopMove(stop);
		}
		else
		{


			//bool stop = true;
			//moveid->SetStopMove(stop);

			isRotate = false;
		}




	}

	// 魔法の種類で終わりを帰る。
	switch (magicType)
	{
	case (int)Player::CommandMagic::Fire:
	{
		// 時間
		if (animationTime <= 0.5f)return;
		// 炎発射
		owner.lock()->GetComponent<Player>()->InputMagicframe();

		// SE炎
		owner.lock()->GetComponent<Player>()->InputAttackFlameSE();

		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Idle);
		// 炎se
		//flameStarteSe->Play(loopSe);

		break;
	}

	case (int)Player::CommandMagic::Thander:
	{
		// 時間
		if (animationTime <= 1.1f)return;

		// 雷音
		owner.lock()->GetComponent<Player>()->InputAttackThanderSE();

		// 雷発射
		owner.lock()->GetComponent<Player>()->InputMagicLightning();
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Idle);

		// 雷se
		///lightningSe->Play(loopSe);
		break;
	}
	case (int)Player::CommandMagic::Ice:
	{
		// 時間
		if (animationTime <= 0.5f)return;

		// 氷音
		owner.lock()->GetComponent<Player>()->InputAttackIceSE();

		// 氷発射
		owner.lock()->GetComponent<Player>()->InputMagicIce();
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Idle);

		// 氷se
		//iceStarteSe->Play(loopSe);
		break;
	}
	case (int)Player::CommandMagic::Heale:
	{


		// 時間
		if (animationTime <= 1.1f)return;

		// 回復音
		owner.lock()->GetComponent<Player>()->InputAttackHealeSE();
		// 雷発射
		owner.lock()->GetComponent<Player>()->InputMagicHealing();
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Idle);

		// 雷se
		///lightningSe->Play(loopSe);
		break;
	}

	default:
		break;
	}

}

void PlayerMagicState::Exit()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// 魔法の選択をゼロに
	//owner.lock()->GetComponent<Player>()->SetSelectCheck((int)Player::CommandMagic::Normal);
	if (!playerid.lock()->InputShortCutkeyMagic())
	{
		// 魔法選択解除
		playerid.lock()->SetMagicAction(false);

		// 攻撃種類選択
		playerid.lock()->SetSelectMagicCheck((int)Player::CommandAttack::Attack);

	}

	// 落ちるの再開
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);

	// se
	//flameStarteSe->Stop();
	//iceStarteSe->Stop();
	//lightningSe->Stop();
}

void PlayerSpecialMagicState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// Se
	//flameSpecialStarteSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/必殺技炎.wav");
	//flameSpecialSaveSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/必殺技炎ため.wav");
	//flameSpecialSaveSe->Play(loopSe);

	fire = std::make_unique<Effect>("Data/Effect/fire.efk");
	fireAttack = std::make_unique<Effect>("Data/Effect/hit fire.efk");
	// 再生開始時間
	//currentAnimationStartSeconds = 0.0f;

	modelAnim.index = Player::Animation::Anim_SlashThree;

	// アニメーション再生
	model->PlayAnimation(modelAnim);
	// アニメーション再生
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);


	Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");

	DirectX::XMFLOAT3 pPosition =
	{
				pHPosiiton->worldTransform._41,
				pHPosiiton->worldTransform._42,
				pHPosiiton->worldTransform._43
	};
	// 音炎
	playerid.lock()->InputAttackFlameSE();

	fire->Play(pPosition);
	// 落ちるの停止
	bool stopFall = true;
	moveid.lock()->SetStopFall(stopFall);

	button = true;

	// スタート値
	specialMoveWaitTime = specialMoveWaitStartTime;

	// 最初だけ魔法を発動するため
	startMagic = false;

	// カメラ回転処理を開始
	isRotate = true;




	//MessageData::CAMERACHANGEMOTIONMODEDATA	p;

	//position = transformid->GetPosition();
	//angle = transformid->GetAngle();

	//// モーション記録
	//float vx = sinf(angle.y) * 6;
	//float vz = cosf(angle.y) * 6;
	//float vx2 = sinf(angle.y) - 10;
	//float vz2 = cosf(angle.y) * 7;
	//float vx3 = sinf(angle.y);

	//p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
	//p.data.push_back({ 50, {position.x + vx, position.y + 3, position.z + vz }, position });
	//p.data.push_back({ 100, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
	//p.data.push_back({ 150, {position.x - vx2, position.y + 5, position.z - vz2 }, position });
	//p.data.push_back({ 170, {position.x + vx, position.y + 3, position.z + vz }, position });
	//p.data.push_back({ 200, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
	//p.data.push_back({ 250, {position.x + vx3 , position.y + 1, (position.z + 0.1f) - vz2 }, position });

	//// エネミー呼ぶ奴
	//EnemyManager& enemyManager = EnemyManager::Instance();
	//int enemyManagerCount = enemyManager.GetEnemyCount();

	//DirectX::XMFLOAT3 pos;
	//DirectX::XMVECTOR posVec;


	//pos =
	//{ 0,0,0 };
	//float length;

	//length = 10.0f;

	////　カメラ移動　ベクトル獲得
	//if (enemyManagerCount > 0)
	//{

	//	std::shared_ptr<EnemyBoss> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemyBoss>();

	//	DirectX::XMVECTOR positionVec = DirectX::XMLoadFloat3(&position);
	//	DirectX::XMVECTOR ePositionVec = DirectX::XMLoadFloat3(&enemy->GetPosition());

	//	posVec = DirectX::XMVectorSubtract(positionVec, ePositionVec);

	//	posVec = DirectX::XMVector3Normalize(posVec);

	//	DirectX::XMStoreFloat3(&pos, posVec);

	//}




	//p.data.push_back({ 330,{position.x + (pos.x * length) ,
	//	position.y + (pos.y * length) + 1,
	//	position.z + (pos.z * length) }, position });




	//Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

	bool specialRockOff = true;
	playerid.lock()->SetSpecialRockOff(specialRockOff);


}

void PlayerSpecialMagicState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// エネミーの前に向く
	{
		DirectX::XMVECTOR Vector;
		DirectX::XMVECTOR LengthSq;

		DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&owner.lock()->GetComponent<Transform>()->GetPosition());
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
			//length < attackCheckRangeMax && length > attackCheckRangeMin&&

			if (moveid.lock()->Turn(vector, turnSpeed, elapsedTime))
			{
				// カメラ向き
				{
					MessageData::CAMERACHANGEMOTIONMODEDATA	p;

					position = transformid.lock()->GetPosition();
					angle = transformid.lock()->GetAngle();

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

		
		
		//// エフェクト更新
		//if(fire->GetEfeHandle())
		//{
		//	Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");

		//	DirectX::XMFLOAT3 pPosition =
		//	{
		//				pHPosiiton->worldTransform._41,
		//				pHPosiiton->worldTransform._42,
		//				pHPosiiton->worldTransform._43
		//	};

		//	fire->SetPosition(fire->GetEfeHandle(),
		//		pPosition);
		//}



		// アニメーション
		if (!model->IsPlayAnimation())
		{
			// 再生開始時間
			//currentAnimationStartSeconds = 0.0f;

			modelAnim.index = Player::Animation::Anim_MagicSeconde;

			// アニメーション再生
			model->PlayAnimation(modelAnim);
			button = false;

			// 手の炎エフェクト停止
			fire->Stop(fire->GetEfeHandle());

			// Se
			//flameSpecialSaveSe->Stop();

			//rockCheck = true;
			//owner.lock()->GetComponent<Player>()->SetRockCheck(rockCheck);


			
		}
	}
	else
	{
			// 任意のアニメーション再生区間でのみ衝突判定処理をする
		float animationTime = model->GetCurrentANimationSeconds();

		// アニメーション
		if (animationTime >= 1.1f)
		{

			playerid.lock()->SetFlashOn(true);
			//owner.lock()->GetComponent<Player>()->SetHitCheck(true);

			// アニメーション停止
			playerid.lock()->SetUpdateAnim
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



			//// 魔法発射のタイミング
			//if (!startMagic)
			//{
			//	playerid.lock()->
			//		InputSpecialMagicframe();
			//	playerid.lock()->InputAttackFlameSpecileSE();
			//}
			//// 最初だけ魔法を発動するため
			//startMagic = true;


			// 必殺技を放った後の経過時間
			specialMoveWaitTime += elapsedTime;

			PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
			ColorGradingData colorGradingData;
			colorGradingData.brigthness = 1.5f;
			
			
			postprocessingRenderer.SetColorGradingData(colorGradingData);

			//float colorGradingValue = 0.1f;
			//postprocessingRenderer.StepValueColor(colorGradingValue);



			// Se
			//flameSpecialStarteSe->Play(loopSe);

		}

		if (animationTime >= 1.1f && animationTime <= 1.2f)
		{
			playerid.lock()->
				InputSpecialMagicframe();
			playerid.lock()->InputAttackFlameSpecileSE();
		}
	}



	// ダメージ判定
	playerid.lock()->SpecialApplyDamageInRadius();



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
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


		return;
	}

	


}

void PlayerSpecialMagicState::Exit()
{
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	// 落ちるの再開
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);


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

	//// Se
	//flameSpecialStarteSe->Stop();
	//flameSpecialSaveSe->Stop();

	bool specialRockOff = false;
	playerid.lock()->SetSpecialRockOff(specialRockOff);
}

void PlayerSpecialMagicIceState::Enter()
{
	// コンポネント取得
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	ice = std::make_unique<Effect>("Data/Effect/specialIce.efk");
	//ice = std::make_unique<Effect>("Data/Effect/brezerd.efk");
	//fireAttack = std::make_unique<Effect>("Data/Effect/hit fire.efk");
	//// 再生開始時間
	//currentAnimationStartSeconds = 0.0f;

	modelAnim.keyFrameEnd = 64.0f;

	// アニメーション再生
	model->PlayAnimation(modelAnim);
	// アニメーション再生
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);


	//Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");

	//DirectX::XMFLOAT3 pPosition =
	//{
	//			pHPosiiton->worldTransform._41,
	//			pHPosiiton->worldTransform._42,
	//			pHPosiiton->worldTransform._43
	//};

	//ice->Play(pPosition);
	// 落ちるの停止
	bool stopFall = true;
	moveid.lock()->SetStopFall(stopFall);

	button = true;

	// スタート値
	specialMoveWaitTime = specialMoveWaitStartTime;

	// 最初だけ魔法を発動するため
	startMagic = false;


	MessageData::CAMERACHANGEMOTIONMODEDATA	p;

	position = transformid.lock()->GetPosition();
	angle = transformid.lock()->GetAngle();


	// モーション記録
	float vx = sinf(angle.y) * 6;
	float vz = cosf(angle.y) * 6;
	float vx2 = sinf(angle.y) - 10;
	float vz2 = cosf(angle.y) * 7;
	float vx3 = sinf(angle.y);

	//p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
	//p.data.push_back({ 50, {position.x + vx, position.y + 3, position.z + vz }, position });
	//p.data.push_back({ 100, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
	//p.data.push_back({ 150, {position.x - vx2, position.y + 5, position.z - vz2 }, position });
	//p.data.push_back({ 170, {position.x + vx, position.y + 3, position.z + vz }, position });
	//p.data.push_back({ 200, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
	//p.data.push_back({ 250, {position.x + vx3 , position.y + 1, (position.z + 0.1f) - vz2 }, position });

	//p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
	//p.data.push_back({ 20, {position.x + vx2, position.y + 5, position.z - vz2 }, position ,true });
	//p.data.push_back({ 40, {position.x - vx2, position.y + 5, position.z - vz2 }, position ,true });

	//p.data.push_back({ 60, {position.x + vx3 , position.y + 1, (position.z + 0.1f) - vz2 }, position ,true });
	//



	p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
	//p.data.push_back({ 10, {(position.x + vx3) * 2, position.y + 3, position.z + vz2 }, position });
	p.data.push_back({ 30, {position.x + vx2, position.y + 5, position.z + vz2 }, position ,true });
	p.data.push_back({ 70, {position.x - vx2, position.y + 5, position.z + vz2 }, position ,true });

	p.data.push_back({ 110, {position.x + vx3 , position.y + 1, (position.z + 0.1f) + vz2 }, position ,true });



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


	//p.data.push_back({ 150, {position.x + (pos.x * length) ,
	//	position.y + (pos.y * length) + 1,
	//	position.z + (pos.z * length) }, position });

	//p.data.push_back({ 200, {position.x + (pos.x * length) ,
	//position.y + (pos.y * length) + 1,
	//position.z + (pos.z * length) }, position });

	p.data.push_back({ 150, {position.x + (pos.x * length) ,
		position.y + (pos.y * length) + 1,
		position.z + (pos.z * length) }, position });

	p.data.push_back({ 180, {position.x + (pos.x * length) ,
	position.y + (pos.y * length) + 1,
	position.z + (pos.z * length) }, position });

	Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

	//playerid.lock()->Set

	bool specialRockOff = true;
	playerid.lock()->SetSpecialRockOff(specialRockOff);
}

void PlayerSpecialMagicIceState::Execute(float elapsedTime)
{
	// コンポネント取得
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	if (button)
	{

		//// カメラモーション
		//MessageData::CAMERACHANGEMOTIONMODEDATA	p;

		//position = transformid->GetPosition();
		//angle = transformid->GetAngle();


		//// モーション記録
		//float vx = sinf(angle.y) * 6;
		//float vz = cosf(angle.y) * 6;
		//float vx2 = sinf(angle.y) - 10;
		//float vz2 = cosf(angle.y) * 7;
		//float vx3 = sinf(angle.y);

		//p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
		//p.data.push_back({ 50, {position.x + vx, position.y + 3, position.z + vz }, position });
		//p.data.push_back({ 100, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
		//p.data.push_back({ 150, {position.x - vx2, position.y + 5, position.z - vz2 }, position });
		//p.data.push_back({ 170, {position.x + vx, position.y + 3, position.z + vz }, position });
		//p.data.push_back({ 200, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
		//p.data.push_back({ 250, {position.x + vx3 , position.y + 1, (position.z + 0.1f) - vz2 }, position });
		//p.data.push_back({ 350, {position.x + vx3 , position.y + 0.5f, (position.z + 0.1f) - vz2 }, position });

		//// モーション設定
		//Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

		// 任意のアニメーション再生区間でのみ衝突判定処理をする
		//float animationTime = model->GetModel()->GetCurrentANimationSeconds();

		// エフェクト更新
		if (ice->GetEfeHandle())
		{
			Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");

			DirectX::XMFLOAT3 pPosition =
			{
						pHPosiiton->worldTransform._41,
						pHPosiiton->worldTransform._42,
						pHPosiiton->worldTransform._43
			};

			ice->SetPosition(ice->GetEfeHandle(),
				pPosition);
		}



		// アニメーション
		if (!model->IsPlayAnimation())
		{
			//// 再生開始時間
			//currentAnimationStartSeconds = 0.0f;

			modelAnim.index = Player::Animation::Anim_Slash;

			modelAnim.keyFrameEnd = 0.0f;

			// アニメーション再生
			model->PlayAnimation(modelAnim);
			//model->PlayAnimation(
			//	Player::Anim_MagicSeconde, loop,
			//	currentAnimationStartSeconds, blendSeconds
			//	, currentAnimationAddSeconds
			//);
			button = false;

			// 手の炎エフェクト停止
			//ice->Stop(ice->GetEfeHandle());
				// エネミー呼ぶ奴
			EnemyManager& enemyManager = EnemyManager::Instance();
			int enemyManagerCount = enemyManager.GetEnemyCount();

			// 動作させるかどうか
			if (enemyManagerCount > 0)
			{

				std::weak_ptr<Transform> enemyTId = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<Transform>();
				// エフェクトスケール
				float efScale = 5.0f;
				// 氷
				ice->Play(enemyTId.lock()->GetPosition(), efScale);
			}

			//rockCheck = true;
			//owner.lock()->GetComponent<Player>()->SetRockCheck(rockCheck);



		}
	}
	else
	{
		// 任意のアニメーション再生区間でのみ衝突判定処理をする
		float animationTime = model->GetCurrentANimationSeconds();

		// アニメーション
		if (animationTime >= 1.1f)
		{

			playerid.lock()->SetFlashOn(true);
			//owner.lock()->GetComponent<Player>()->SetHitCheck(true);

			// アニメーション停止
			playerid.lock()->SetUpdateAnim
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



			// 魔法発射のタイミング
			if (!startMagic)
				playerid.lock()->
				InputSpecialMagicframe();

			// 最初だけ魔法を発動するため
			startMagic = true;


			// 必殺技を放った後の経過時間
			specialMoveWaitTime += elapsedTime;



		}
	}



	// ダメージ判定
	playerid.lock()->SpecialApplyDamageInRadius();



	// 待機時間が過ぎたので次のステートへ
	if (specialMoveWaitTime >= specialMoveWaitTimeMax)
	{
		int projectileMax = ProjectileManager::Instance().GetProjectileCount();
		for (int i = 0; i < projectileMax; ++i)
		{
			std::weak_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(i);
			if (projectile.lock()->GetComponent<ProjectileTornade>())
			{
				projectile.lock()->GetComponent<BulletFiring>()->Destroy();
			}
		}

		// 次のステートへ
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


		return;
	}
}

void PlayerSpecialMagicIceState::Exit()
{
	// コンポネント取得
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	// 落ちるの再開
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);

	// エネミー呼ぶ奴
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyManagerCount = enemyManager.GetEnemyCount();

	// 動作させるかどうか
	if (enemyManagerCount > 0)
	{

		std::weak_ptr<EnemyBoss> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemyBoss>();
		std::weak_ptr<Movement> enemyMove = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<Movement>();

		bool moveCheck = true;
		enemy.lock()->SetMoveCheck(moveCheck);

		// 速度停止
		bool stopVelocity = false;
		enemyMove.lock()->SetStopMove(stopVelocity);
		// 落ちるの停止
		bool stopFall = false;
		enemyMove.lock()->SetStopFall(stopFall);
	}

	bool specialRockOff = false;
	playerid.lock()->SetSpecialRockOff(specialRockOff);
}

void PlayerSpecialThanderMagicState::Enter()
{


}

void PlayerSpecialThanderMagicState::Execute(float elapsedTime)
{
}

void PlayerSpecialThanderMagicState::Exit()
{
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	// 落ちるの再開
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);

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
}


void PlayerDamageState::Enter()
{
	// コンポネント取得
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	//std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// Se否設定
	//damageSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/炎着弾時.wav");
	//damageSe->Play(loopSe);
	//moveid = owner.lock()->GetComponent<Movement>();

	//model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	bool loop = false;

	// 歩き許可
	bool stopMove = false;
	moveid.lock()->SetStopMove(stopMove);

	// 重力オフ
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);

	modelAnim.index = Player::Animation::Anim_Pain;
		//Player::Anim_Pain, loop,
	model->PlayAnimation(modelAnim);

	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerDamageState::Execute(float elapsedTime)
{
	// コンポネント取得
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	// ロックオン処理
	playerid.lock()->UpdateCameraState(elapsedTime);

	if (!model->IsPlayAnimation())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));

	}
}

void PlayerDamageState::Exit()
{
	//damageSe->Stop();
}

void PlayerDeathState::Enter()
{
	// コンポネント取得
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	
	float				stateTimer = 0.0f;

	modelAnim.index = Player::Animation::Anim_Deth;
		//Player::Anim_Deth, loop,
	model->PlayAnimation(modelAnim);

	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);

	stateTimer = 0.0f;

	PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();

	//colorGradingData.saturation = 0.0f;

	//postprocessingRenderer.SetColorGradingMaxData(colorGradingData);

	colorGradingData.saturation = 0.0f;

	postprocessingRenderer.SetColorGradingMinData(colorGradingData);


}

void PlayerDeathState::Execute(float elapsedTime)
{
	// コンポネント取得
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<HP> hpid = owner.lock()->GetComponent<HP>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	if (stateTimer >= stateTimerMax)
	{

		hpid.lock()->SetDead(false);

	}

	    // 死亡時はそれっぽいカメラアングルで死亡
    // 例えばコレを必殺技などで上手く利用できればかっこいいカメラ演出が作れますね
    MessageData::CAMERACHANGEMOTIONMODEDATA	p;

	DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();
	DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();



    float vx = sinf(angle.y) * 6;
    float vz = cosf(angle.y) * 6;
    p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
    p.data.push_back({ 95, {position.x + vx + 13, position.y + 3, position.z + vz }, position });
	
    Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentANimationSeconds();
	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。
	//bool CollisionFlag = animationTime >= 1.5f && animationTime <= 1.6f;
	bool CollisionFlag = animationTime >= 0.5f && animationTime <= 1.6f;
	if (CollisionFlag)
	{
		++stateTimer;
		
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Stop);


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
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	// 下半身
	modelAnim.index = Player::Animation::Anim_Dush;

	modelAnim.currentanimationseconds = 0.3f;

	modelAnim.currentAnimationAddSeconds = 0.025f;



	//Player::Anim_Slash, loop
	model->PlayAnimation(modelAnim);


	// 当たり判定の有無
	//owner.lock()->GetComponent<Player>()->DmageInvalidJudment(false);



	// アニメーション種類 通常
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);

	// 落ちるの停止
	bool stopFall = true;
	moveid.lock()->SetStopFall(stopFall);

	playerid.lock()->InputDashSE();

	// エフェクト設定
	wind = std::make_unique<Effect>("Data/Effect/dashu.efk");

	// エフェクト再生時間
	wind->Play({ 
		transformid.lock()->GetPosition().x,
		transformid.lock()->GetPosition().y + addHeight,
		transformid.lock()->GetPosition().z
		});


	// 角度変更
	wind->SetAngle(wind->GetEfeHandle(), transformid.lock()->GetAngle());

	// Se否設定
	//dushSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/炎着弾時.wav");
	//dushSe->Play(loopSe);
}

// 回避更新
void PlayerAvoidanceState::Execute(float elapsedTime)
{
	// コンポーネント呼び出し
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// 動き自由
	bool stopMove = false;
	moveid.lock()->SetStopMove(stopMove);

	// ロックオン処理
	owner.lock()->GetComponent<Player>()->UpdateCameraState(elapsedTime);

	// 転がりダッシュ
	DirectX::XMFLOAT3 dir;
	dir.x = sinf(transformid.lock()->GetAngle().y);
	dir.y = cosf(transformid.lock()->GetAngle().x);
	dir.z = cosf(transformid.lock()->GetAngle().y);

	
	// 任意のアニメーション再生区間
	float animationTime = owner.lock()->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	//float animationTime = owner.lock()->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	
	// 位置更新
	wind->SetPosition(wind->GetEfeHandle(),
		{
			transformid.lock()->GetPosition().x,
			transformid.lock()->GetPosition().y + addHeight,
			transformid.lock()->GetPosition().z
		});

	//if (animationTime + FLT_EPSILON >= 0.7f + FLT_EPSILON && animationTime <= 0.71f)
	//if (animationTime + FLT_EPSILON >= 0.7f + FLT_EPSILON && animationTime <= 0.71f)

	
	// 地上ダッシュ
	if (animationTime >= 0.7f && animationTime <= 0.8f && moveid.lock()->GetOnLadius())
	{
		DirectX::XMFLOAT3 impulse =
		{
			dir.x * speed,
			0,
			dir.z * speed,
		};

		moveid.lock()->AddImpulse(impulse);

		//moveid->Move(dir, moveSpeed, elapsedTime);
	}
	// 空中ダッシュ
	if (animationTime >= 0.7f  && animationTime <= 0.8f && !moveid.lock()->GetOnLadius())
	{
		//moveid->Move(dir, moveSpeed, elapsedTime);

		DirectX::XMFLOAT3 impulse =
		{
			dir.x * flySpeed,
			0,
			dir.z * flySpeed,
		};
	
		moveid.lock()->AddImpulse(impulse);
		
	}

	// アニメーションダッシュ後通常移動
	if (animationTime >= 1.0f && owner.lock()->GetComponent<Player>()->InputMove(elapsedTime))
	{

		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	}
	
	// アニメーション終了
	//if (!owner.lock()->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	if (animationTime >= 2.0f)
	{

		// 当たり判定の有無
		//owner.lock()->GetComponent<Player>()->DmageInvalidJudment(true);
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));



	}
}

// 回避終了
void PlayerAvoidanceState::Exit()
{
	// コンポーネント呼び出し
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// 落ちるの停止
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);

	wind->Stop(wind->GetEfeHandle());

	// se
	//dushSe->Stop();
}

// 反射開始
void PlayerReflectionState::Enter()
{


	// コンポーネント呼び出し
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	bool loop = false;

	modelAnim.index = Player::Animation::Anim_Counter;

		//Player::Anim_Counter, loop,
	model->PlayAnimation(modelAnim);

	// アニメーション種類 通常
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);

	// Se否設定
	//reflectionStop = Audio::Instance().LoadAudioSource("Data/Audio/SE/ヒットストップ.wav");


}

// 反射更新
void PlayerReflectionState::Execute(float elapsedTime)
{
	// コンポーネント呼び出し
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	if (!model->IsPlayAnimation())
	{
		// 当たり判定の有無
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));

	}


	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = model->GetCurrentANimationSeconds();
	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。
	bool CollisionFlag = animationTime >= 0.2f && animationTime <= 0.4f;

	if (CollisionFlag)
	{
		// 左手ノードとエネミーの衝突処理
		playerid.lock()->CollisionNodeVsEnemiesCounter("mixamorig:LeftHand", owner.lock()->GetComponent<Player>()->GetLeftHandRadius());
		playerid.lock()->CollisionNodeVsRubyCounter("mixamorig:LeftHand", owner.lock()->GetComponent<Player>()->GetLeftHandRadius());
		playerid.lock()->CollisionNodeVsRubyCounterBulletFring("mixamorig:LeftHand", owner.lock()->GetComponent<Player>()->GetLeftHandRadius());
		
	}
	//if (playerid.lock()->GetHitCheck())
	//{
	//	//reflectionStop->Play(loopSe);
	//}
}

// 反射終了
void PlayerReflectionState::Exit()
{
	//reflectionStop->Stop();
}

void PlayerTitleIdleState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	modelAnim.index = Player::Animation::Anim_Idle;

	modelAnim.loop = true;
	model->PlayAnimation(modelAnim);

	// アニメーションルール
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);



}

void PlayerTitleIdleState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	//// ロックオン処理
    playerid.lock()->UpdateCameraState(elapsedTime);


	// ヒット
	if (playerid.lock()->InputAttack())
	{

		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::StateTitle::Push));
	}
	//// 移動入力処理
	//if (playerid.lock()->InputMove(elapsedTime))
	//{
	//	playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	//	//TransitionMoveState();
	//}
	//if (playerid.lock()->InputAttack())
	//{

	//}

	//// 反射入力処理
	//if (playerid.lock()->InputAvoidance())
	//{
	//	playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Reflection));

	//}


	//// ジャンプ入力処理
	//if (playerid.lock()->InputJump())
	//{
	//	playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
	//	//TransitionJumpState();


}

void PlayerTitleIdleState::Exit()
{
}

void PlayerTitlePushState::Enter()
{
	// コンポネント取得
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();


	//std::weak_ptr<Transform> enemyTransform = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<Transform>();
	//std::weak_ptr<HP> enemyHpId = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<HP>();

	lightning = std::make_unique<Effect>("Data/Effect/sunder.efk");
	lightningAttack = std::make_unique<Effect>("Data/Effect/HitThunder.efk");
	lightningHit = std::make_unique<Effect>("Data/Effect/lightningStrike.efk");


	//// 再生ループ
	//bool  loop = false;

	//// 再生開始時間 
	//float currentAnimationStartSeconds = 0.0f;


	//// アニメーションブレンド
	//float blendSeconds = 0.5f;

	modelAnim.index = Player::Animation::Anim_MagicSeconde;
	// アニメーション再生
	model->PlayAnimation(modelAnim);

	// アニメーション再生
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);




	// 落ちるの停止
	bool stopFall = true;
	moveid.lock()->SetStopFall(stopFall);

	// モーション切り替え
	secondeMortion = false;

}

void PlayerTitlePushState::Execute(float elapsedTime)
{
	// コンポネント取得
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	//// ロックオン処理
	playerid.lock()->UpdateCameraState(elapsedTime);

	std::weak_ptr<HP> enemyHpId;
	// 任意のアニメーション再生区間でのみ衝突判定処理をする
		float animationTime = moveid.lock()->GetOnLadius() ?
			model->GetCurrentANimationSeconds() :
			model->GetCurrentAnimationSecondsUpeer();

		// アニメーション
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON && !secondeMortion)
		{

			playerid.lock()->InputAttackThanderSE();

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
		

			//lightning->Stop(lightning->GetEfeHandle());

				secondeMortion = true;


				// ポストエフェクトインスタンスゲット
				PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
				// ラジアルブラー
				radialBlurData.radius = 40.0f;

				postprocessingRenderer.SetRadialBlurData(radialBlurData);

		}
		if (animationTime >= 1.5f - FLT_EPSILON && animationTime <= 1.6f + FLT_EPSILON && secondeMortion)
		{

			playerid.lock()->InputAttackSlashSpecileLightningStrikeSE();

			//// 再生ループ
			//bool  loop = false;

			//// 再生開始時間 
			//float currentAnimationStartSeconds = 0.0f;
			//// 再生時間加算分の値
			//float currentAnimationAddSeconds = 0.00f;

			//// アニメーションブレンド
			//float blendSeconds = 0.5f;


			//// アニメーション再生
			//model->PlayAnimation(
			//	Player::Anim_SpecialAttack, loop,
			//	currentAnimationStartSeconds, blendSeconds
			//	, currentAnimationAddSeconds
			//);

			Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");

			DirectX::XMFLOAT3 pPosition =
			{
						pHPosiiton->worldTransform._41,
						pHPosiiton->worldTransform._42,
						pHPosiiton->worldTransform._43
			};

			lightningHit->Play(pPosition);


			////lightning->Stop(lightning->GetEfeHandle());

			//secondeMortion = true;

			//playerid.lock()->SetFlashOn(true);



			// ポストエフェクトインスタンスゲット
			PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
			// ラジアルブラー
			radialBlurData.radius = 60.0f;

			postprocessingRenderer.SetRadialBlurData(radialBlurData);

		}
		if (!model->IsPlayAnimation() && secondeMortion)
		{
			// シーン終了
			playerid.lock()->SetEndState(true);
			//SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
		}
	
}

void PlayerTitlePushState::Exit()
{
	//std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	//std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	//// 落ちるの再開
	//bool stopFall = false;
	//moveid.lock()->SetStopFall(stopFall);


	//// エネミー呼ぶ奴
	//EnemyManager& enemyManager = EnemyManager::Instance();
	//int enemyManagerCount = enemyManager.GetEnemyCount();

	//// 動作させるかどうか
	//if (enemyManagerCount > 0)
	//{

	//	std::shared_ptr<EnemyBoss> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemyBoss>();
	//	std::shared_ptr<Movement> enemyMove = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<Movement>();

	//	bool moveCheck = true;
	//	enemy->SetMoveCheck(moveCheck);


	//	// 速度停止
	//	bool stopVelocity = false;
	//	enemyMove->SetStopMove(stopVelocity);
	//	// 落ちるの停止
	//	bool stopFall = false;
	//	enemyMove->SetStopFall(stopFall);
	//}

	//se 停止
	//slashSe->Stop();
	//lighteningStrikeSpecialSe->Stop();
	//lighteningStrikeSpecialSaveSe->Stop();

	//rockCheck = false;
	//owner.lock()->GetComponent<Player>()->SetRockCheck(rockCheck);

	//// フリーカメラのロックをとどめる
	//bool freeCameraCheck = true;
	//owner.lock()->GetComponent<Player>()->SetFreeCameraCheck(freeCameraCheck);
}

void PlayerClearIdleState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	modelAnim.index = Player::Animation::Anim_MagicSeconde;

	modelAnim.keyFrameEnd = 68.0f;

	model->PlayAnimation(modelAnim);

	// アニメーションルール
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);

	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();

	position.x += -3.0f;

	transformid.lock()->SetPosition(position);


	//Model::Node* PRock = model->FindNode("mixamorig:Spine1");

	//DirectX::XMFLOAT3 pos = model->ConvertLocalToWorld(PRock);

	////DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();
	////position.y += 2;
	////position.z += 3;

	////pos.x += 3.0f;
	////pos.z += 3;


	//MessageData::CAMERACHANGEFREEMODEDATA	p = { pos };

	//Messenger::Instance().SendData(MessageData::CAMERACHANGEFREEMODE, &p);



}

void PlayerClearIdleState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	Model::Node* PRock = model->FindNode("mixamorig:Spine1");

	DirectX::XMFLOAT3 pos = model->ConvertLocalToWorld(PRock);

	pos.x += 3.0f;
	//pos.z += 10;


	MessageData::CAMERACHANGEFREEMODEDATA	p = { pos };

	Messenger::Instance().SendData(MessageData::CAMERACHANGEFREEMODE, &p);

	//// ロックオン処理
	//playerid.lock()->UpdateCameraState(elapsedTime);

	// ヒット
	if (playerid.lock()->InputAttack())
	{
		// シーン終了
		playerid.lock()->SetEndState(true);
		//playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::StateTitle::Push));
	}
}

void PlayerClearIdleState::Exit()
{
}


void PlayerClearEscapeState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	modelAnim.index = Player::Animation::Anim_MagicSeconde;

	modelAnim.keyFrameEnd = 68.0f;

	model->PlayAnimation(modelAnim);

	// アニメーションルール
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);

	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();

	position.x += -3.0f;

	transformid.lock()->SetPosition(position);
}

void PlayerClearEscapeState::Execute(float elapsedTime)
{
}

void PlayerClearEscapeState::Exit()
{
}


void PlayerOverIdleState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	// アニメーションルール
	Model::ModelAnim modelAnim;

	modelAnim.index = Player::Animation::Anim_Deth;

	modelAnim.currentanimationseconds = 1.7f;

	//modelAnim.keyFrameEnd = 105.0f;

	model->PlayAnimation(modelAnim);

	// アニメーションルール
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerOverIdleState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	//// ロックオン処理
	playerid.lock()->UpdateCameraState(elapsedTime);

	// ヒット
	if (playerid.lock()->InputAttack())
	{

		// シーン終了
	/*	playerid.lock()->SetEndState(true);*/
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::StateOver::Revive));
	}
	
	//if (!model->IsPlayAnimation())
	//{
	//	model->PlayAnimation(
	//		Player::Anim_Deth, loop, currentAnimationStartSeconds, blendSeconds
	//	);
	//}
}

void PlayerOverIdleState::Exit()
{
}

void PlayerOverReviveState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	modelAnim.index = Player::Animation::Anim_Magic;

	model->PlayAnimation(modelAnim);

	// アニメーションルール
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);

	isPoseStarte = false;
}

void PlayerOverReviveState::Execute(float elapsedTime)
{
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	if (!model->IsPlayAnimation() && !isPoseStarte)
	{

		modelAnim.index = Player::Animation::Anim_Jump;
		model->PlayAnimation(modelAnim);

		isPoseStarte = true;
		return;
	}

	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	if (!model->IsPlayAnimation() && isPoseStarte)
	{
		
		playerid.lock()->SetEndState(endState);
	}
}

void PlayerOverReviveState::Exit()
{
}
