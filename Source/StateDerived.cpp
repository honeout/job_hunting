#include <math.h>
#include "EnemySlime.h"
#include "StateDerived.h"
#include "Input\GamePad.h"
#include "Player.h"
#include "Mathf.h"
#include "Input/Input.h"
#include "ProjectileHoming.h"
#include "ProjectileRuby.h"
#include "ProjectileThrowing.h"
#include "ProjectileManager.h"



// TODO 02_03 Stateを基底クラスとして各種Stateクラスを用意する。
// Wanderはサンプルとしてすでに記述済み
// 各種Enter関数の内容は各Transition○○State関数を参考に
// 各種Execute関数の内容は各Update○○State関数を参考に

// 徘徊ステートに入った時のメソッド
void WanderState::Enter()
{
	//owner->GetActor()->GetComponent<EnemySlime>()->
	// 縄張り
	owner->GetComponent<EnemySlime>()->SetRandomTargetPosition();

	// アニメーション再生
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Attack, loop, currentAnimationStartSeconds, blendSeconds);
	//owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetComponent<EnemySlime>()->GetAnimationStateWalk(), true);
	// アニメーションルール
	owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Reverseplayback);

	stateTimer = Mathf::RandomRange(6.0f, 8.0f);

	// 着地瞬間
	upOnLading = false;

}

// 徘徊ステートで実行するメソッド
void WanderState::Execute(float elapsedTime)
{
	stateTimer -= elapsedTime;
	// プレイヤーid
	std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
	// 目標地点ををプレイヤー位置に設定
	DirectX::XMFLOAT3 targetPosition = playerid->GetComponent<Transform>()->GetPosition();

	owner->GetComponent<EnemySlime>()->SetTargetPosition(targetPosition);
	
	// 目的地点までのXZ平面での距離判定
	float vx = targetPosition.x - owner->GetComponent<EnemySlime>()->GetPosition().x;
	float vz = targetPosition.z - owner->GetComponent<EnemySlime>()->GetPosition().z;
	float distSq = vx * vx + vz * vz;

	// 目的地へ着いた
	if (stateTimer < 0.0f)
	{
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::IdleBattle));
	}
	// 目的地にこの半径入ったら
	float radius = owner->GetComponent<EnemySlime>()->GetAttackRightFootRange();



	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	if (animationTime >= 0.6f && animationTime <= 0.7f)
	{
		// アニメーションルール 停止
		owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Stop);
	
		if (distSq > radius * radius)
			// 目的地点へ移動
			owner->GetComponent<EnemySlime>()->MoveToTarget(elapsedTime, 0.8f);


		// ジャンプ
		owner->GetComponent<EnemySlime>()->InputJump();

		// 着地で衝撃波を飛ばす
		if (owner->GetComponent<Movement>()->GetOnLadius() && !upOnLading)
		{

			Model::Node* node = owner->GetComponent<ModelControll>()->GetModel()->FindNode("boss_right_foot1");
			DirectX::XMFLOAT3 pos(
				node->worldTransform._41,
				node->worldTransform._42,
				node->worldTransform._43
			);
			owner->GetComponent<EnemySlime>()->InputImpact(pos);
			// 着地瞬間
			upOnLading = true;
		}
		if (!owner->GetComponent<Movement>()->GetOnLadius())
		{

			// 着地瞬間
			upOnLading = false;
		}
	}

	//// プレイヤー索敵
	//if (owner->GetComponent<EnemySlime>()->SearchPlayer())
	//{
	//	// 見つかったら追跡ステートへ遷移
	//	// ChangeStateクラスでStateを切り替える
	//	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	//}

}

// 徘徊ステートから出ていくときのメソッド
void WanderState::Exit()
{

}

// 初期化
void IdleState::Enter()
{
	owner->GetComponent<EnemySlime>()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Anim_Standby, loop, currentAnimationStartSeconds,blendSeconds);
	//owner->GetComponent<EnemySlime>()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetComponent<EnemySlime>()->GetAnimationStateNormal(), true);
	
	// アニメーションルール
	owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Normal);
	
	// タイマーをランダム設定
	//stateTimer = Mathf::RandomRange(3.0f, 5.0f);
	//owner->GetComponent<EnemySlime>()->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));

}

// update
void IdleState::Execute(float elapsedTime)
{
	// タイマー処理
     //stateTimer -= elapsedTime;
	/* owner->SetStateTimer(stateTimer);*/
	//owner->GetComponent<EnemySlime>()->SetStateTimer(owner->GetComponent<EnemySlime>()->GetStateTimer() - elapsedTime);
	// TODO 03 
	// 待機時間が経過したとき徘徊ステートへ遷移しなさい
	//if (owner->GetComponent<EnemySlime>()->GetStateTimer() < 0.0f)
	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	// TODO 03 
	// プレイヤーが見つかったとき追跡ステートへ遷移しなさい
	//if (owner->GetComponent<EnemySlime>()->SearchPlayer())
	//	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
}

// 最後にやりたい処理全般
void IdleState::Exit()
{
}
// 初期化
void PursuitState::Enter()
{
	
	owner->GetComponent<EnemySlime>()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Standby, loop,currentAnimationStartSeconds,  blendSeconds);
	//owner->GetComponent<EnemySlime>()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Walk, true);
	//owner->GetComponent<EnemySlime>()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetComponent<EnemySlime>()->GetAnimationStateWalk(), true);

	// アニメーションルール
	owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Normal);

	// 数秒間追跡するタイマーをランダム設定
	stateTimer = Mathf::RandomRange(3.0f, 5.0f);

	attackRange = owner->GetComponent<EnemySlime>()->GetAttackRightFootRange();

	//// ライフ
	//int life;
	//life = owner->GetComponent<HP>()->GetLife();

	attackRound = owner->GetComponent<HP>()->GetLife();

	switch (attackRound)
	{
	case AttackChange::Round1:
	{
		//attackType = rand() % 2;
		break;
	}
	case AttackChange::Round2:
	{
		attackType = rand() % 2;
		break;
	}
	case AttackChange::ROund3:
	{
		attackType = rand() % 3;
		break;
	}
	default:
		break;
	}
	

	//switch (life)
	//{
	//case AttackChange::Round1:
	//{
	//	attackRound = 1;
	//	break;
	//}

	//case AttackChange::Round2:
	//{
	//	attackRound = 2;
	//	break;
	//}

	//case AttackChange::ROund3:
	//{
	//	attackRound = 3;
	//	break;
	//}

	//}
}
// update
void PursuitState::Execute(float elapsedTime)
{
	// 目標地点をプレイヤー位置に設定
	//owner->SetTargetPosition(Player::Instance().GetPosition());
	//DirectX::XMFLOAT3 targetPosition = ;
		// プレイヤーid
	std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
	// 目標地点ををプレイヤー位置に設定
	DirectX::XMFLOAT3 targetPosition = playerid->GetComponent<Transform>()->GetPosition();


	owner->GetComponent<EnemySlime>()->SetTargetPosition(targetPosition);

	// 目的地点へ移動
	//owner->GetComponent<EnemySlime>()->MoveToTarget(elapsedTime, 1.0);


	// タイマー処理
	stateTimer -= elapsedTime;
	/*owner->SetStateTimer(stateTimer);*/



	// TODO 03 
	// 追跡時間が経過したとき遠距離ステートへ遷移しなさい
	//if (stateTimer < 0.0f)
	//	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));
		//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Wander));
		//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::ShotThrowing));
		//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Idle));


	float vx = targetPosition.x - owner->GetComponent<EnemySlime>()->GetPosition().x;
	float vy = targetPosition.y - owner->GetComponent<EnemySlime>()->GetPosition().y;
	float vz = targetPosition.z - owner->GetComponent<EnemySlime>()->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	// TODO 03 
	// 攻撃範囲に入ったとき攻撃ステートへ遷移しなさい
	if (dist < attackRange)
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Attack));


	switch (attackRound)
	{
	case AttackChange::Round1:
	{
		if (stateTimer < 0.0f)
		{
			//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));
			owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::ShotThrowing));
		}

		break;
	}

	case AttackChange::Round2:
	{
		if (stateTimer < 0.0f && attackType == 0)
			owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));

		if (stateTimer < 0.0f && attackType == 1)
			owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::ShotThrowing));

		break;
	}

	// HP残機０
	case AttackChange::ROund3:
	{

		//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::ShotThrowing));
		if (stateTimer < 0.0f && attackType == 0)
			owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));

		if (stateTimer < 0.0f && attackType == 1)
			owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::ShotThrowing));

		if (stateTimer < 0.0f && attackType == 2)
			owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Wander));

		break;
	}


	// HP残機　２と１
	default:
	{


		break;
	}
	}


	//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));

}
// 最後にやりたい処理全般
void PursuitState::Exit()
{
}
// 初期化
void AttackState::Enter()
{
	currentAnimationStartSeconds = 0.0f;
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Attack, loop, currentAnimationStartSeconds, blendSeconds);
	//owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetComponent<EnemySlime>()->GetAnimationStateAttack(), false);

	// アニメーションルール
	owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Normal);

		// ライフ
	int life;
	life = owner->GetComponent<HP>()->GetLife();

	switch (life)
	{
	case AttackChange::Round1:
	{
		attackMemoryMax = 1;
		break;
	}

	case AttackChange::Round2:
	{
		attackMemoryMax = 3;
		break;
	}

	case AttackChange::ROund3:
	{
		attackMemoryMax = 5;
		break;
	}

	}
}
// update
void AttackState::Execute(float elapsedTime)
{


	// 攻撃モーションが終了したとき追跡ステートへ移行
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation() && attackMemory < attackMemoryMax)
	{
		//owner->GetComponent<EnemySlime>()->SetCounterJudgment(false);
		//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Attack));
		//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
		// アニメ再生
		//float currentAnimationSeconds = 0.8f;
		currentAnimationStartSeconds = 0.8f;
		owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Attack, loop, currentAnimationStartSeconds, blendSeconds);

		
		++attackMemory;
	}
	// 繰り返し踏みつけ最後
	else if (attackMemory == attackMemoryMax)
	{
		//owner->GetComponent<EnemySlime>()->SetCounterJudgment(false);
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::IdleBattle));
		
		int attackMemoryStart = 0;
		attackMemory = attackMemoryStart;
	}

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	if (animationTime >= 1.0f && animationTime <= 2.0f)
	{
		// 左足ノードとプレイヤーの衝突処理
		owner->GetComponent<EnemySlime>()->CollisitionNodeVsPlayer("boss_left_foot1", 2);
	}
	// パリィ
	//if (animationTime >= 0.8f && animationTime <= 1.5f)
	//{
	//	owner->GetComponent<EnemySlime>()->SetCounterJudgment(true);
	//}

	if (animationTime >= 1.30f && animationTime <= 1.33f)
	{
		Model::Node* node = owner->GetComponent<ModelControll>()->GetModel()->FindNode("boss_left_foot1");
		DirectX::XMFLOAT3 pos(
			node->worldTransform._41,
			node->worldTransform._42,
			node->worldTransform._43
		);
		owner->GetComponent<EnemySlime>()->InputImpact(pos);
	}


}
// 最後にやりたい処理全般
void AttackState::Exit()
{
}

// 初期設定
void AttackShotState::Enter()
{
	currentAnimationStartSeconds = 0.0f;
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Shot, loop, currentAnimationStartSeconds, blendSeconds);

	// アニメーションルール
	owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Normal);

	// ライフ
	int life;
	life = owner->GetComponent<HP>()->GetLife();

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

	// 攻撃モーションが終了したとき追跡ステートへ移行
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		//owner->GetComponent<EnemySlime>()->SetCounterJudgment(false);
		//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));
		//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
		// アニメ再生
		currentAnimationStartSeconds = 0.1f;
		owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Shot, loop, currentAnimationStartSeconds ,blendSeconds);
		++attackCount;


	}
	if (attackCount >= attackCountMax)
	{
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
		attackCount = 0;
	}

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// efekt設定
	if (animationTime >= 0.1f && animationTime <= 0.11f)
	{
		//Model::Node* node = owner->GetComponent<ModelControll>()->GetModel()->FindNode("boss_right_hand1");
		//DirectX::XMFLOAT3 pos(
		//	node->worldTransform._41,
		//	node->worldTransform._42,
		//	node->worldTransform._43
		//);
		////for (int i = 0; i < 5; ++i)
		////{
		//	// 左足ノードとプレイヤーの衝突処理
		owner->GetComponent<EnemySlime>()->InputProjectile();

	}
	//// 弾丸出現
	//if (animationTime >= 0.7f && animationTime <= 0.8f)
	//{

	//}
	// 弾丸発射
	if (animationTime >= 1.0f && animationTime <= 1.1f)
	{


		bool movementCheck = true;
		for (int i = 0; i < ProjectileManager::Instance().GetProjectileCount(); ++i)
		{
			if (owner->GetComponent<ProjectileRuby>())
			owner->GetComponent<ProjectileRuby>()->SetMovementCheck(movementCheck);
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
	currentAnimationStartSecondsf = 0.0f;
	owner->GetComponent<ModelControll>()->GetModel()->PlayReverseAnimation(EnemySlime::Animation::Anim_jewelattack, loop,currentAnimationStartSecondsf,blendSeconds);
	// アニメーションルール
	owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Reverseplayback);

	// 投げる
	turnPermission = false;

	// 地面判定一度消す
	bool pushuThrow = false;
	owner->GetComponent<EnemySlime>()->SetPushuThrow(pushuThrow);

}
// 更新処理
void AttackShotThrowingState::Execute(float elapsedTime)
{
	// 正面
	DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 angle = owner->GetComponent<Transform>()->GetAngle();
	// プレイヤーid
	std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);

	for (int i = 0; i < ProjectileManager::Instance().GetProjectileCount(); ++i)
	{

		std::shared_ptr<Actor> projectileManager = ProjectileManager::Instance().GetProjectile(i);
		if (projectileManager->GetComponent<ProjectileThrowing>())
		{
			// 球回転
			{
				DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&playerid->GetComponent<Transform>()->GetPosition());
				DirectX::XMVECTOR projectilePosition = DirectX::XMLoadFloat3(&projectileManager->GetComponent<Transform>()->GetPosition());
				//DirectX::XMFLOAT3 projectileAngle = projectileManager->GetComponent<Transform>()->GetAngle();

				DirectX::XMVECTOR vectorVec = DirectX::XMVectorSubtract(playerPosition, projectilePosition);
				vectorVec = DirectX::XMVector3Normalize(vectorVec);
				DirectX::XMFLOAT3 vector;
				DirectX::XMStoreFloat3(&vector, vectorVec);




				//projectileManager->GetComponent<Transform>()->SetPosition(owner->GetComponent<Transform>()->GetPosition());
				//projectileManager->GetComponent<Transform>()->SetAngle(owner->GetComponent<Transform>()->GetAngle());

				projectileManager->GetComponent<BulletFiring>()->Turn(turnSpeed, vector, elapsedTime);
			}
		}
	}
	



	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// 宝石取って設定
	if (animationTime <= 3.9f && animationTime >= 3.89f && !turnPermission)
	{
		//// 正面の向きベクトル
         direction.x = sinf(angle.y);// 三角を斜めにして位置を変えた
         //direction.y = sinf(angle.x);
         direction.y = DirectX::XMConvertToRadians(0);
         direction.z = cosf(angle.y);

		//// プレイヤーid
		//std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
		//
		//	遠距離攻撃登録
		owner->GetComponent<EnemySlime>()->InputThrowingRuby(direction);

		

	}
	// 回転
	if (animationTime <= 3.6f && animationTime >= 3.0f && !turnPermission)
	{
		// プレイヤーid
		//std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);

		owner->GetComponent<EnemySlime>()->SetTargetPosition(playerid->GetComponent<Transform>()->GetPosition());
		owner->GetComponent<EnemySlime>()->TurnToTarget(elapsedTime, turnSpeed);
		//owner->GetComponent<Transform>()->SetAngle(playerid->GetComponent<Transform>()->GetAngle());



		for (int i = 0; i < ProjectileManager::Instance().GetProjectileCount() ; ++i)
		{

			//// 正面の向きベクトル
   //         direction.x = sinf(angle.y);// 三角を斜めにして位置を変えた
   //         direction.y = cosf(angle.x);
   //         direction.z = cosf(angle.y);

			std::shared_ptr<Actor> projectileManager = ProjectileManager::Instance().GetProjectile(i);
			if (projectileManager->GetComponent<ProjectileThrowing>())
			{
				//// 球回転
				//{
				//	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&playerid->GetComponent<Transform>()->GetPosition());
				//	DirectX::XMVECTOR projectilePosition = DirectX::XMLoadFloat3(&projectileManager->GetComponent<Transform>()->GetPosition());

				//	DirectX::XMVECTOR vectorVec = DirectX::XMVectorSubtract(playerPosition, projectilePosition);
				//	vectorVec = DirectX::XMVector3Normalize(vectorVec);
				//	DirectX::XMFLOAT3 vector;
				//	DirectX::XMStoreFloat3(&vector, vectorVec);


				//	//projectileManager->GetComponent<Transform>()->SetPosition(owner->GetComponent<Transform>()->GetPosition());
				//	//projectileManager->GetComponent<Transform>()->SetAngle(owner->GetComponent<Transform>()->GetAngle());

				//	projectileManager->GetComponent<BulletFiring>()->Turn(turnSpeed, vector, elapsedTime);
				//}
				projectileManager->GetComponent<Transform>()->SetPosition(
					{ 
						owner->GetComponent<Transform>()->GetPosition().x,
						owner->GetComponent<Transform>()->GetPosition().y + owner->GetComponent<Transform>()->GetHeight(),
						owner->GetComponent<Transform>()->GetPosition().z

					});
				/*projectileManager->GetComponent<Transform>()->SetDirection(direction);
				projectileManager->GetComponent<Transform>()->SetAngle(angle);*/
			}
		}

	}
	// 構え
	if (animationTime >= 3.89f && !turnPermission)
	{

		//DirectX::XMFLOAT3 direction;
		//DirectX::XMFLOAT3 angle = owner->GetComponent<Transform>()->GetAngle();

		// プレイヤーid
		//std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);


		//direction.x = sinf(angle.y);// 三角を斜めにして位置を変えた
		//direction.y = cosf(angle.x);
		//direction.z = cosf(angle.y);
		for (int i = 0; i < ProjectileManager::Instance().GetProjectileCount(); ++i)
		{

			std::shared_ptr<Actor> projectileManager = ProjectileManager::Instance().GetProjectile(i);
			if (projectileManager->GetComponent<ProjectileThrowing>())
			{
				DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&playerid->GetComponent<Transform>()->GetPosition());
				DirectX::XMVECTOR projectilePosition = DirectX::XMLoadFloat3(&projectileManager->GetComponent<Transform>()->GetPosition());

				DirectX::XMVECTOR vectorVec = DirectX::XMVectorSubtract(playerPosition, projectilePosition);
				vectorVec = DirectX::XMVector3Normalize(vectorVec);
				DirectX::XMFLOAT3 vector;
				DirectX::XMStoreFloat3(&vector, vectorVec);


				projectileManager->GetComponent<Transform>()->SetPosition(owner->GetComponent<Transform>()->GetPosition());
				//projectileManager->GetComponent<Transform>()->SetAngle(owner->GetComponent<Transform>()->GetAngle());

				//projectileManager->GetComponent<BulletFiring>()->Turn(turnSpeed,vector,elapsedTime);

				//projectileManager->GetComponent<Transform>()->SetDirection(vector);
			}
		}
	}

	// アニメーション射撃発射
	if (animationTime <= 0.8f && !turnPermission)
	{
		// 強制アニメーション終了
		bool end = true;
		owner->GetComponent<ModelControll>()->GetModel()->SetAnimationEndFlag(end);
		//bool blend = false;

		owner->GetComponent<ModelControll>()->GetModel()->ReverseplaybackAnimation(elapsedTime);
		// アニメーション再生時間
		currentAnimationStartSecondsf = 0.7f;

		owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Shot, loop, currentAnimationStartSecondsf, blendSeconds);
		// アニメーションルール
		owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Normal);


		turnPermission = true;
	}
	// 射撃許可
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation() && turnPermission)
	{
		// 地面判定発生
		bool pushuThrow = true;
		owner->GetComponent<EnemySlime>()->SetPushuThrow(pushuThrow);
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
		//owner->GetComponent<ModelControll>()->GetModel()->ReverseplaybackAnimation(elapsedTime, blend);
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	}

	//// 攻撃モーションが終了したとき追跡ステートへ移行
	//if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation() && turnPermission)
	//{
	//	//owner->GetComponent<EnemySlime>()->SetCounterJudgment(false);
	//	//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));
	//	// animation変更
	//	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));

	//}



}
// 終了処理
void AttackShotThrowingState::Exit()
{
}

void DamageState::Enter()
{
	//bool loop = false;

	owner->GetComponent<ModelControll>()->GetModel()->
		PlayAnimation(
			EnemySlime::Animation::Anim_Die, loop);
	// アニメーションルール
	owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Normal);
		//owner->GetComponent<ModelControll>()->GetModel()->
		//PlayAnimation(
		//	owner->GetComponent<EnemySlime>()->GetAnimationStateDie(), loop);


}

void DamageState::Execute(float elapsedTime)
{
		if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));

}

void DamageState::Exit()
{
}

// 混乱開始
void ConfusionState::Enter()
{
	//bool loop = true;

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Die, loop, currentAnimationStartSeconds, blendSeconds);
	// アニメーションルール
	owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Normal);

	// ステート待機時間
	stateTimer = Mathf::RandomRange(6.0f, 8.0f);

}
// 混乱更新
void ConfusionState::Execute(float elapsedTime)
{
	stateTimer -= elapsedTime;

	if (stateTimer < 0.0f)
	{
		// ステート変更
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Idle));
	}

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// ひざまつく
	if (animationTime >= 1.1)
	{
		// アニメーションルール
		owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Stop);

	}

}
// 混乱終了
void ConfusionState::Exit()
{

}

// プレイヤー
void PlayerIdleState::Enter()
{
	//bool loop = true;
	//owner->GetComponent<Player>()
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Idle, loop,currentAnimationStartSeconds,blendSeconds
	);
	
	// アニメーションルール
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);
	
}

void PlayerIdleState::Execute(float elapsedTime)
{
	

	// 移動入力処理
	if (owner->GetComponent<Player>()->InputMove(elapsedTime))
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
		//TransitionMoveState();
	}

	// 反射入力処理
	if (owner->GetComponent<Player>()->InputAvoidance())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Reflection));

	}


	// ジャンプ入力処理
	if (owner->GetComponent<Player>()->InputJump())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
		//TransitionJumpState();
	}


	//owner->GetComponent<Player>()->InputSelectCheck();

	//owner->GetComponent<Player>()->InputShortCutkeyMagic();

	//if (owner->GetComponent<Player>()->InputAttack() && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Attack)
	//{
	//	//stated = state;
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
	//	//TransitionAttackState();
	//}

	//owner->GetComponent<Player>()->InputSelectMagicCheck();

	//// 弾丸入力処理
	//// 炎
	//if (owner->GetComponent<Player>()->GetSelectMagicCheck() == (int)Player::CommandMagic::Fire && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Magic)
	//{
	//	owner->GetComponent<Player>()->InputMagicframe();
	//	//TransitionAttackState();
	//}
	//// 雷
	//if (owner->GetComponent<Player>()->GetSelectMagicCheck() == (int)Player::CommandMagic::Thander && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Magic)
	//{
	//	owner->GetComponent<Player>()->InputMagicLightning();
	//	//TransitionAttackState();
	//}
	//// 氷
	//if (owner->GetComponent<Player>()->GetSelectMagicCheck() == (int)Player::CommandMagic::Ice && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Magic)
	//{
	//	owner->GetComponent<Player>()->InputMagicIce();
	//	//TransitionAttackState();
	//}


	//// 特殊攻撃
	//if (owner->GetComponent<Player>()->InputSpecialAttackCharge())
	//{
	//	//TransitionAttackState();
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
	//}
	//// 特殊魔法
	//if (owner->GetComponent<Player>()->InputSpecialShotCharge())
	//{
	//	owner->GetComponent<Player>()->InputProjectile();
	//}

}

void PlayerIdleState::Exit()
{
}

void PlayerMovestate::Enter()
{
	//bool loop = true;
	//owner->GetComponent<Player>()
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Running, loop,
		currentAnimationStartSeconds, blendSeconds
	);

	owner->GetComponent<ModelControll>()->GetModel()->PlayUpeerBodyAnimation(
		owner->GetComponent<Player>()->Anim_Running, loop,
		currentAnimationStartSeconds, blendSeconds
	);

	//owner->GetComponent<ModelControll>()->GetModel()->PlayAnimationBlend(
	//	 owner->GetComponent<Player>()->Anim_Walking,owner->GetComponent<Player>()->Anim_Running, loop,0.0f, owner->GetComponent<Player>()->GetMoveSpeedAnimation()
	//);

	// アニメーションルール
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Doble);
}

void PlayerMovestate::Execute(float elapsedTime)
{

	// 移動入力処理
	if (!owner->GetComponent<Player>()->InputMove(elapsedTime))
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
		//TransitionIdleState();
	}


	// 回避入力処理
	if (owner->GetComponent<Player>()->InputAvoidance())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

	}

	// ジャンプ入力処理
	if (owner->GetComponent<Player>()->InputJump())
	{

		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
		//TransitionJumpState();
	}

	//owner->GetComponent<Player>()->InputSelectCheck();

	//owner->GetComponent<Player>()->InputShortCutkeyMagic();

	//if (owner->GetComponent<Player>()->InputAttack() && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Attack)
	//{
	//	//stated = state;
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
	//	//TransitionAttackState();
	//}
	//owner->GetComponent<Player>()->InputSelectMagicCheck();

	//// 弾丸入力処理
	//// 炎
	//if (owner->GetComponent<Player>()->GetSelectMagicCheck() == (int)Player::CommandMagic::Fire &&owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Magic)
	//{
	//	owner->GetComponent<Player>()->InputMagicframe();
	//	//TransitionAttackState();
	//}
	//// 雷
	//if (owner->GetComponent<Player>()->GetSelectMagicCheck() == (int)Player::CommandMagic::Thander && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Magic)
	//{
	//	owner->GetComponent<Player>()->InputMagicLightning();
	//	//TransitionAttackState();
	//}
	//// 氷
	//if (owner->GetComponent<Player>()->GetSelectMagicCheck() == (int)Player::CommandMagic::Ice && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Magic)
	//{
	//	owner->GetComponent<Player>()->InputMagicIce();
	//	//TransitionAttackState();
	//}



	//// 特殊攻撃
	//if (owner->GetComponent<Player>()->InputSpecialAttackCharge())
	//{
	//	//TransitionAttackState();
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
	//}
	//// 特殊魔法
	//if (owner->GetComponent<Player>()->InputSpecialShotCharge())
	//{
	//	owner->GetComponent<Player>()->InputProjectile();
	//}
	// 落下着地
	//owner->GetComponent<Player>()->Ground();


}

void PlayerMovestate::Exit()
{
}

void PlayerJumpState::Enter()
{
	//loop = false;

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Jump, loop,
	currentAnimationStartSeconds, blendSeconds);

	// アニメーションルール
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);

}

void PlayerJumpState::Execute(float elapsedTime)
{



	// ジャンプ入力処理
	if (owner->GetComponent<Player>()->InputJump())
	{
		//TransitionJumpFlipState();
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
	}

	// 移動
	owner->GetComponent<Player>()->InputMove(elapsedTime);

	// 回避
	if (owner->GetComponent<Player>()->InputAvoidance())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}

	//// 反射入力処理
	//else if (owner->GetComponent<Player>()->InputAvoidance())
	//{
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Reflection));

	//}

	//owner->GetComponent<Player>()->InputSelectCheck();

	//owner->GetComponent<Player>()->InputShortCutkeyMagic();

	//if (owner->GetComponent<Player>()->InputAttack() && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Attack)
	//{
	//	//stated = state;
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
	//	//TransitionAttackState();
	//}
	//owner->GetComponent<Player>()->InputSelectMagicCheck();

	//// 弾丸入力処理
	//// 炎
	//if (owner->GetComponent<Player>()->GetSelectMagicCheck() == (int)Player::CommandMagic::Fire && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Magic)
	//{
	//	owner->GetComponent<Player>()->InputMagicframe();
	//	//TransitionAttackState();
	//}
	//// 雷
	//if (owner->GetComponent<Player>()->GetSelectMagicCheck() == (int)Player::CommandMagic::Thander && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Magic)
	//{
	//	owner->GetComponent<Player>()->InputMagicLightning();
	//	//TransitionAttackState();
	//}
	//// 氷
	//if (owner->GetComponent<Player>()->GetSelectMagicCheck() == (int)Player::CommandMagic::Ice && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Magic)
	//{
	//	owner->GetComponent<Player>()->InputMagicIce();
	//	//TransitionAttackState();
	//}


	//// 特殊攻撃
	//if (owner->GetComponent<Player>()->InputSpecialAttackCharge())
	//{
	//	//TransitionAttackState();
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
	//}
	//// 特殊魔法
	//if (owner->GetComponent<Player>()->InputSpecialShotCharge())
	//{
	//	owner->GetComponent<Player>()->InputProjectile();
	//}

	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
			owner->GetComponent<Player>()->Anim_Falling, loop,
			currentAnimationStartSeconds, blendSeconds);
	}

	if (owner->GetComponent<Movement>()->GetOnLadius())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Land));
	}

	//// 落下着地
	//if (owner->GetComponent<Player>()->Ground())
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Land));


}

void PlayerJumpState::Exit()
{
}

void PlayerLandState::Enter()
{
	//bool loop = false;

	owner->GetComponent<Movement>()->SetOnLadius(false);

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Landing, loop,
		currentAnimationStartSeconds, blendSeconds
	);

	// アニメーションルール
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);
}

void PlayerLandState::Execute(float elapsedTime)
{
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		owner->GetComponent<Movement>()->SetOnLadius(false);
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));


	}

	//// 移動
	//if (owner->GetComponent<Player>()->InputMove(elapsedTime) && owner->GetComponent<Player>()->InputAvoidance())
	//{
	//	// 回避
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

	//}

	//// 反射入力処理
	//if (owner->GetComponent<Player>()->InputAvoidance())
	//{
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Reflection));

	//}
}

void PlayerLandState::Exit()
{
}

void PlayerJumpFlipState::Enter()
{
	//bool loop = false;

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Jump_Flip, loop,
		currentAnimationStartSeconds , blendSeconds);

	// アニメーションルール
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);
}

void PlayerJumpFlipState::Execute(float elapsedTime)
{
	// 移動
	if (owner->GetComponent<Player>()->InputMove(elapsedTime))
		bool afterimagemove = true;
	// ジャンプ入力処理
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{

		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
	}

	//// 回避
	//if (owner->GetComponent<Player>()->InputAvoidance())
	//{
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	//}


	//// 落下着地
	//if (owner->GetComponent<Player>()->Ground())
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Land));

}

void PlayerJumpFlipState::Exit()
{


}

void PlayerAttackState::Enter()
{
	bool loop = false;

	//owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
	//	owner->GetComponent<Player>()->Anim_Attack, loop);






	std::shared_ptr<Player> playerId = owner->GetComponent<Player>();

	// アニメーション再生
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Attack, loop,
		currentAnimationStartSeconds, blendSeconds
	);
	// アニメーション再生
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);

	
	owner->GetComponent<Movement>()->SetGravity(gravity);

	//// 落ちるの停止
	//bool stopFall = true;
	//owner->GetComponent<Movement>()->SetStopFall(stopFall);

	// 移動の停止
	bool stopMove = true;
	owner->GetComponent<Movement>()->SetStopMove(stopMove);

	//owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
	//	owner->GetComponent<Player>()->Anim_Attack, loop,
	//	currentAnimationStartSeconds, blendSeconds
	//);

	//// アニメーションルール
	//owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);






	//DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(
	//	&owner->GetComponent<Transform>()->GetPosition());

	//DirectX::XMVECTOR Vector;
	//DirectX::XMVECTOR LengthSq;

	//int enemyCount = EnemyManager::Instance().GetEnemyCount();
	//for (int i = 0; i < enemyCount; ++i)
	//{
	//	std::shared_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);

	//	DirectX::XMVECTOR enemyPosition = DirectX::XMLoadFloat3(
	//		&enemy->GetComponent<Transform>()->GetPosition());

	//	Vector = DirectX::XMVectorSubtract(enemyPosition, playerPosition);

	//	LengthSq = DirectX::XMVector3LengthSq(Vector);

	//}

	//DirectX::XMStoreFloat(&length, LengthSq);
	//DirectX::XMStoreFloat3(&vector, Vector);

	//if (length < attackCheckRange)
	//{
	//}

	// 回転するかチェック
	rotateCheck = false;
}

void PlayerAttackState::Execute(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	// 攻撃複数
	if (owner->GetComponent<Player>()->InputAttack()&&
		owner->GetComponent<Player>()->GetSelectCheck() == 
		(int)Player::CommandAttack::Attack)
	{
		button = true;
	}
	// コマンド確認
	std::vector<GamePadButton> command;
	command.push_back(GamePad::BTN_X);
	command.push_back(GamePad::BTN_X);
	command.push_back(GamePad::BTN_A);
	int frame = 60;
	// コマンド確認
	if (gamePad.ConfirmCommand(command, frame))
	{
		button = false;
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
	}



	// 回転
	if (!rotateCheck)
	{
		DirectX::XMVECTOR Vector;
		DirectX::XMVECTOR LengthSq;

		DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&owner->GetComponent<Transform>()->GetPosition());
		EnemyManager& enemyManager = EnemyManager::Instance();
		int enemyCount = enemyManager.GetEnemyCount();
		for (int i = 0; i < enemyCount; ++i)//float 最大値ないにいる敵に向かう
		{
			DirectX::XMVECTOR enemyPosition = DirectX::XMLoadFloat3(&enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition());

			/*DirectX::XMFLOAT3 Direction =
			{
				enemyPosition.x - playerPosition.x,
				enemyPosition.y - playerPosition.y,
				enemyPosition.z - playerPosition.z
			};*/

			Vector = DirectX::XMVectorSubtract(enemyPosition, playerPosition);

			LengthSq = DirectX::XMVector3Length(Vector);

			DirectX::XMStoreFloat3(&vector, Vector);

			DirectX::XMStoreFloat(&length, LengthSq);
			// 距離
			if (length < attackCheckRange && length > attackCheckRangeMin)
			{
				bool stop = false;
				owner->GetComponent<Movement>()->SetStopMove(stop);
				//owner->GetComponent<Movement>()->Move(vector, speed, elapsedTime);
				
				
				
				// 正面
				if (owner->GetComponent<Movement>()->Turn(vector, turnSpeed, elapsedTime))
				{
					rotateCheck = true;
					owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
						owner->GetComponent<Player>()->Anim_Attack, loop,
						currentAnimationStartSeconds, blendSeconds
					);
					// アニメーション再生
					owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);

					owner->GetComponent<Movement>()->Move(vector, speed, elapsedTime);
					
				}

			}
			else
			{
				/*DirectX::XMFLOAT3 velocity = {0,0,0};
				owner->GetComponent<Movement>()->SetVelocity(velocity);*/
				bool stop = true;
				owner->GetComponent<Movement>()->SetStopMove(stop);

				
				//owner->GetComponent<Movement>()->Move(vector, speed, elapsedTime);
				//// 速度をゼロベクトルに
				//owner->GetComponent<Movement>()->SetVelocity(velocity);
			}


		}

	}


	
	//// 部分再生
	//if (owner->GetComponent<Player>()->GetUpdateAnim() == UpAnim::Doble && !owner->GetComponent<ModelControll>()->GetModel()->IsPlayUpeerBodyAnimation())
	//{
	//	owner->GetComponent<Player>()->SetAttackCollisionFlag(false);
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(button ? Player::State::Attack : Player::State::Move));
	//	button = false;
	//	owner->GetComponent<Player>()->SetSpecialAttackTime(false);


	//	//attackCollisionFlag = false;
	//	//TransitionMoveState();
	//	//owner->GetComponent<Player>()->SetAngleCheck(false);

	//}
	// もし終わったら待機に変更
	//if (owner->GetComponent<Player>()->GetUpdateAnim() == UpAnim::Normal && !owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		owner->GetComponent<Player>()->SetAttackCollisionFlag(false);
		// 入力確認でステート変更
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(button ? Player::State::Attack : Player::State::Move));

		//// 落ちるの再開
		//bool stopFallFalse = false;
		//bool stopFallTrue = true;
		//owner->GetComponent<Movement>()->SetStopFall(button ? stopFallTrue : stopFallFalse);
		
	
		// 移動の停止
		owner->GetComponent<Movement>()->SetGravity(button ?
			gravity :
			owner->GetComponent<Player>()->GetGravity());

		bool stop = false;
		owner->GetComponent<Movement>()->SetStopMove(stop);

		//// 速度をゼロベクトルに
		//owner->GetComponent<Movement>()->SetVelocity(velocity);

		button = false;
		//owner->GetComponent<Player>()->SetSpecialAttackTime(false);


		//owner->GetComponent<Player>()->SetAngleCheck(false);
	}
	//// もし移動中に手を離したら
	//if (owner->GetComponent<Player>()->GetUpdateAnim() == UpAnim::Doble && !owner->GetComponent<Player>()->InputMove(elapsedTime))
	//{
	//	//updateanim = UpAnim::Doble;
	//	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Doble);

	//	bool loop = false;

	//		owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
	//				owner->GetComponent<Player>()->Anim_Attack, loop);
	//}

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。
	bool CollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;

	/*owner->GetComponent<Player>()->SetAttackCollisionFlag(CollisionFlag);*/

	//if (updateanim == UpAnim::Doble)
	//{
	//	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	//	float animationTime = model->GetCurrentAnimationSecondsUpeer();
	//	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。
	//	attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
	//}
	if (CollisionFlag)
	{
		// 左手ノードとエネミーの衝突処理
		owner->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius(),"shoulder");
	}

	//owner->GetComponent<Player>()->Ground();
}

void PlayerAttackState::Exit()
{
	
}

void PlayerDamageState::Enter()
{
	bool loop = false;

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_GetHit1, loop,
		currentAnimationStartSeconds, blendSeconds);

	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);
}

void PlayerDamageState::Execute(float elapsedTime)
{
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));

	}
}

void PlayerDamageState::Exit()
{
}

void PlayerDeathState::Enter()
{
	//bool loop = false;

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Death, loop,
		currentAnimationStartSeconds ,blendSeconds
	);

	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);
}

void PlayerDeathState::Execute(float elapsedTime)
{

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
	//bool loop = false;

	//loop = true;
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Attack, loop
	,currentAnimationStartSeconds, blendSeconds);

	//loop = false;
	//owner->GetComponent<ModelControll>()->GetModel()->PlayUpeerBodyAnimation(
	//	owner->GetComponent<Player>()->Anim_Attack, loop
	//	, currentAnimationStartSeconds, blendSeconds);

	// 当たり判定の有無
	owner->GetComponent<Player>()->DmageInvalidJudment(false);

	moveSpeed = 10.0f;

	// アニメーション種類 通常
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);

	// 落ちるの停止
	bool stopFall = true;
	owner->GetComponent<Movement>()->SetStopFall(stopFall);

}

// 回避更新
void PlayerAvoidanceState::Execute(float elapsedTime)
{
	// 移動入力処理
	//owner->GetComponent<Player>()->InputMove(elapsedTime);

	// 転がりダッシュ
	DirectX::XMFLOAT3 dir;
	dir.x = sinf(owner->GetComponent<Transform>()->GetAngle().y);
	dir.y = cosf(owner->GetComponent<Transform>()->GetAngle().x);
	dir.z = cosf(owner->GetComponent<Transform>()->GetAngle().y);

	
	// 任意のアニメーション再生区間
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// この時間で
	if (animationTime >= 0.2f )
	{
		owner->GetComponent<Movement>()->Move(dir, moveSpeed, elapsedTime);
	}
	// アニメーション終了
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{

		// 当たり判定の有無
		owner->GetComponent<Player>()->DmageInvalidJudment(true);
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


		// 落ちるの停止
		bool stopFall = false;
		owner->GetComponent<Movement>()->SetStopFall(stopFall);
	}
}

// 回避終了
void PlayerAvoidanceState::Exit()
{

}

// 反射開始
void PlayerReflectionState::Enter()
{
	bool loop = false;

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Attack, loop,
		currentAnimationStartSeconds, blendSeconds);

	// アニメーション種類 通常
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);


	// 当たり判定の有無
	//owner->GetComponent<Player>()->DmageInvalidJudment(false);

}

// 反射更新
void PlayerReflectionState::Execute(float elapsedTime)
{
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		// 当たり判定の有無
		//owner->GetComponent<Player>()->DmageInvalidJudment(true);
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));

	}


	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。
	bool CollisionFlag = animationTime >= 0.3f && animationTime <= 1.0f;

	//owner->GetComponent<Player>()->SetAttackCollisionFlag(CollisionFlag);

	if (CollisionFlag)
	{
		// 左手ノードとエネミーの衝突処理
		owner->GetComponent<Player>()->CollisionNodeVsEnemiesCounter("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius());
		owner->GetComponent<Player>()->CollisionNodeVsRubyCounter("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius());
		owner->GetComponent<Player>()->CollisionNodeVsRubyCounterBulletFring("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius());
		
	}
}

// 反射終了
void PlayerReflectionState::Exit()
{
}


