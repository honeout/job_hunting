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

#include "SceneLoading.h"
#include "SceneManager.h"
#include "SceneGameOver.h"



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


}

// 徘徊ステートから出ていくときのメソッド
void WanderState::Exit()
{

}

// 初期化
void IdleState::Enter()
{
	owner->GetComponent<EnemySlime>()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Anim_Standby, loop, currentAnimationStartSeconds,blendSeconds);


	// アニメーションルール
	owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Normal);

}

// update
void IdleState::Execute(float elapsedTime)
{
	// TODO 03 
	// 待機時間が経過したとき徘徊ステートへ遷移しなさい
	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
}

// 最後にやりたい処理全般
void IdleState::Exit()
{
}
// 初期化
void PursuitState::Enter()
{



	--mortionLimit;
	owner->GetComponent<EnemySlime>()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Standby, loop,currentAnimationStartSeconds,  blendSeconds);

	// アニメーションルール
	owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Normal);

	// 数秒間追跡するタイマーをランダム設定
	stateTimer = Mathf::RandomRange(3.0f, 5.0f);

	attackRange = owner->GetComponent<EnemySlime>()->GetAttackRightFootRange();

	// 敵ライフ
	attackRound = owner->GetComponent<HP>()->GetLife();

	switch (attackRound)
	{
	case AttackChange::Round1:
	{
		
		break;
	}
	case AttackChange::Round2:
	{
		//attackType = rand() % 2;
		attackType = 0;
		break;
	}
	case AttackChange::ROund3:
	{
		//attackType = rand() % 3;
		attackType = rand() % 2;
		break;
	}
	default:
		break;
	}
	

}
// update
void PursuitState::Execute(float elapsedTime)
{
	// 目標地点をプレイヤー位置に設定
		// プレイヤーid
	std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
	// 目標地点ををプレイヤー位置に設定
	DirectX::XMFLOAT3 targetPosition = playerid->GetComponent<Transform>()->GetPosition();


	owner->GetComponent<EnemySlime>()->SetTargetPosition(targetPosition);

	if (mortionLimit < 0)
	{
		// 待機時間再設定
		mortionLimit = Mathf::RandomRange(3.0f, mortionLimitMax);
		// 限界がきたので休憩
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::IdleBattle));
	}

	// タイマー処理
	stateTimer -= elapsedTime;


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
		// 時間が過ぎたので
		if (stateTimer < 0.0f)
		{
			
			owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));
		}

		break;
	}

	case AttackChange::Round2:
	{
		if (stateTimer < 0.0f && attackType == 0)
			owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));

		//if (stateTimer < 0.0f && attackType == 1)
		//	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::ShotThrowing));

		break;
	}

	// HP残機０
	case AttackChange::ROund3:
	{

		//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::ShotThrowing));
		if (stateTimer < 0.0f && attackType == 0)
			owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));

		//if (stateTimer < 0.0f && attackType == 1)
		//	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::ShotThrowing));

		if (stateTimer < 0.0f && attackType == 1)
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
		// アニメ再生
		currentAnimationStartSeconds = 0.8f;
		owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Attack, loop, currentAnimationStartSeconds, blendSeconds);

		
		++attackMemory;
	}
	// 繰り返し踏みつけ最後
	else if (attackMemory == attackMemoryMax)
	{
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
		
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
		//	// 左足ノードとプレイヤーの衝突処理
		owner->GetComponent<EnemySlime>()->InputProjectile();

	}

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
	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&playerid->GetComponent<Transform>()->GetPosition());

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
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// 宝石取って設定
	if (animationTime <= 3.9f && animationTime >= 3.89f && !turnPermission)
	{
		////// 正面の向きベクトル
  //       direction.x = sinf(angle.y);// 三角を斜めにして位置を変えた
  //       direction.y = sinf(angle.x);
  //       //direction.y = DirectX::XMConvertToRadians(0);
  //       direction.z = cosf(angle.y);		

		 
		//	遠距離攻撃登録
		owner->GetComponent<EnemySlime>()->InputThrowingRuby(playerid->GetComponent<Transform>()->GetPosition());

		

	}
	// 回転
	if (animationTime <= 3.6f && animationTime >= 3.0f && !turnPermission)
	{

		owner->GetComponent<EnemySlime>()->SetTargetPosition(playerid->GetComponent<Transform>()->GetPosition());
		owner->GetComponent<EnemySlime>()->TurnToTarget(elapsedTime, turnSpeed);



		for (int i = 0; i < ProjectileManager::Instance().GetProjectileCount() ; ++i)
		{


			std::shared_ptr<Actor> projectileManager = ProjectileManager::Instance().GetProjectile(i);
			if (projectileManager->GetComponent<ProjectileThrowing>())
			{
				
				projectileManager->GetComponent<Transform>()->SetPosition(
					{ 
						owner->GetComponent<Transform>()->GetPosition().x,
						owner->GetComponent<Transform>()->GetPosition().y + owner->GetComponent<Transform>()->GetHeight(),
						owner->GetComponent<Transform>()->GetPosition().z

					});

				projectileManager->GetComponent<BulletFiring>()->TurnFull(turnSpeed, playerid->GetComponent<Transform>()->GetPosition(), elapsedTime);

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
				DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&playerid->GetComponent<Transform>()->GetPosition());
				DirectX::XMVECTOR projectilePosition = DirectX::XMLoadFloat3(&projectileManager->GetComponent<Transform>()->GetPosition());

				//DirectX::XMVECTOR vectorVec = DirectX::XMVectorSubtract(playerPosition, projectilePosition);
				//vectorVec = DirectX::XMVector3Normalize(vectorVec);
				//DirectX::XMFLOAT3 vector;
				//DirectX::XMStoreFloat3(&vector, vectorVec);


				projectileManager->GetComponent<Transform>()->SetPosition(owner->GetComponent<Transform>()->GetPosition());
				
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
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	}




}
// 終了処理
void AttackShotThrowingState::Exit()
{
}

void DamageState::Enter()
{

	owner->GetComponent<ModelControll>()->GetModel()->
		PlayAnimation(
			EnemySlime::Animation::Anim_Die, loop);
	// アニメーションルール
	owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Normal);



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
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
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
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Idle, loop,currentAnimationStartSeconds,blendSeconds
	);
	
	// アニメーションルール
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);
	

	// 落ちる
	bool stop = false;
	owner->GetComponent<Movement>()->SetStopMove(stop);
}

void PlayerIdleState::Execute(float elapsedTime)
{
	// ロックオン処理
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);

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



}

void PlayerIdleState::Exit()
{
}

void PlayerMovestate::Enter()
{
		//Player::Anim_Move, loop,
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Running, loop,
		currentAnimationStartSeconds, blendSeconds
	);

		//Player::Anim_Move, loop,
	owner->GetComponent<ModelControll>()->GetModel()->PlayUpeerBodyAnimation(
		Player::Anim_Running, loop,
		currentAnimationStartSeconds, blendSeconds
	);


	// アニメーションルール
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Doble);


	// 落ちる
	bool stop = false;
	owner->GetComponent<Movement>()->SetStopMove(stop);
}

void PlayerMovestate::Execute(float elapsedTime)
{
	// ロックオン処理
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);

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
		
	}



}

void PlayerMovestate::Exit()
{
}

void PlayerJumpState::Enter()
{
	

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Jump, loop,
	currentAnimationStartSeconds, blendSeconds);

	// アニメーションルール
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


	// 落ちる
	bool stop = false;
	owner->GetComponent<Movement>()->SetStopMove(stop);
}

void PlayerJumpState::Execute(float elapsedTime)
{

	// ロックオン処理
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);

	// ジャンプ入力処理
	if (owner->GetComponent<Player>()->InputJump())
	{
		
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
	}

	// 移動
	owner->GetComponent<Player>()->InputMove(elapsedTime);

	// 回避
	if (owner->GetComponent<Player>()->InputAvoidance())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}


	//if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	//{
	//	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
	//		Player::Anim_Jump, loop,
	//		currentAnimationStartSeconds, blendSeconds);
	//}

	if (owner->GetComponent<Movement>()->GetOnLadius())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Land));
	}


}

void PlayerJumpState::Exit()
{
}

void PlayerLandState::Enter()
{

	owner->GetComponent<Movement>()->SetOnLadius(false);

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Jump, loop,
		currentAnimationStartSeconds, blendSeconds
	);

	// アニメーションルール
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Reverseplayback);
}

void PlayerLandState::Execute(float elapsedTime)
{
	// ロックオン処理
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		owner->GetComponent<Movement>()->SetOnLadius(false);
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));


	}
}

void PlayerLandState::Exit()
{
}

void PlayerJumpFlipState::Enter()
{

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Jump, loop,
		currentAnimationStartSeconds , blendSeconds);

	// アニメーションルール
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerJumpFlipState::Execute(float elapsedTime)
{
	// ロックオン処理
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);
	// 移動
	if (owner->GetComponent<Player>()->InputMove(elapsedTime))
		bool afterimagemove = true;
	// ジャンプ入力処理
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{

		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
	}

}

void PlayerJumpFlipState::Exit()
{


}

void PlayerAttackState::Enter()
{
	bool loop = false;


	std::shared_ptr<Player> playerId = owner->GetComponent<Player>();

		//Player::Anim_Slash, loop,
	// アニメーション再生
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Attack, loop,
		currentAnimationStartSeconds, blendSeconds
	);
	// アニメーション再生
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

	
	owner->GetComponent<Movement>()->SetGravity(gravity);


	// 移動の停止
	bool stopMove = true;
	owner->GetComponent<Movement>()->SetStopMove(stopMove);

	// 回転するかチェック
	rotateCheck = false;
}

void PlayerAttackState::Execute(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	// ロックオン処理
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);

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


			Vector = DirectX::XMVectorSubtract(enemyPosition, playerPosition);

			LengthSq = DirectX::XMVector3Length(Vector);

			DirectX::XMStoreFloat3(&vector, Vector);

			DirectX::XMStoreFloat(&length, LengthSq);
			// 距離
			if (length < attackCheckRange && length > attackCheckRangeMin)
			{
				bool stop = false;
				owner->GetComponent<Movement>()->SetStopMove(stop);
				
				
				
				
				// 正面
				if (owner->GetComponent<Movement>()->Turn(vector, turnSpeed, elapsedTime))
				{
						//Player::Anim_Slash, loop,
					rotateCheck = true;
					owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
						Player::Anim_Attack, loop,
						currentAnimationStartSeconds, blendSeconds
					);
					// アニメーション再生
					owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

					owner->GetComponent<Movement>()->Move(vector, speed, elapsedTime);
					
				}

			}
			else
			{

				bool stop = true;
				owner->GetComponent<Movement>()->SetStopMove(stop);
			}


		}

	}


	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		owner->GetComponent<Player>()->SetAttackCollisionFlag(false);
		// 入力確認でステート変更
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(button ? Player::State::Attack : Player::State::Move));

		
	
		// 移動の停止
		owner->GetComponent<Movement>()->SetGravity(button ?
			gravity :
			owner->GetComponent<Player>()->GetGravity());

		bool stop = false;
		owner->GetComponent<Movement>()->SetStopMove(stop);


		button = false;
		
	}
	

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。
	//bool CollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;

	//if (CollisionFlag)
	//{
	//	// 左手ノードとエネミーの衝突処理
	//	owner->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius(),"shoulder");
	//}

	// 左手ノードとエネミーの衝突処理
	//owner->GetComponent<Player>()->CollisionNodeVsEnemies("Maria_sword", owner->GetComponent<Player>()->GetLeftHandRadius(), "shoulder");
	owner->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius(), "shoulder");
}

void PlayerAttackState::Exit()
{
	
}

void PlayerDamageState::Enter()
{
	bool loop = false;

		//Player::Anim_Pain, loop,
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_GetHit1, loop,
		currentAnimationStartSeconds, blendSeconds);

	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerDamageState::Execute(float elapsedTime)
{
	// ロックオン処理
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);

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
		//Player::Anim_Deth, loop,
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Death, loop,
		currentAnimationStartSeconds ,blendSeconds
	);

	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

	stateTimer = 0.0f;


}

void PlayerDeathState::Execute(float elapsedTime)
{


	if (stateTimer >= stateTimerMax)
	{

		owner->GetComponent<HP>()->SetDead(false);

	}

	    // 死亡時はそれっぽいカメラアングルで死亡
    // 例えばコレを必殺技などで上手く利用できればかっこいいカメラ演出が作れますね
    MessageData::CAMERACHANGEMOTIONMODEDATA	p;

	DirectX::XMFLOAT3 position = owner->GetComponent<Transform>()->GetPosition();
	DirectX::XMFLOAT3 angle = owner->GetComponent<Transform>()->GetAngle();



    float vx = sinf(angle.y) * 6;
    float vz = cosf(angle.y) * 6;
    p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
    p.data.push_back({ 95, {position.x + vx + 13, position.y + 3, position.z + vz }, position });
	
    Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。
	//bool CollisionFlag = animationTime >= 1.5f && animationTime <= 1.6f;
	bool CollisionFlag = animationTime >= 0.5f && animationTime <= 1.6f;
	if (CollisionFlag)
	{
		++stateTimer;
		
		owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Stop);


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
		//Player::Anim_Slash, loop
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Attack, loop
	,currentAnimationStartSeconds, blendSeconds);


	// 当たり判定の有無
	owner->GetComponent<Player>()->DmageInvalidJudment(false);

	moveSpeed = 10.0f;

	// アニメーション種類 通常
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

	// 落ちるの停止
	bool stopFall = true;
	owner->GetComponent<Movement>()->SetStopFall(stopFall);

}

// 回避更新
void PlayerAvoidanceState::Execute(float elapsedTime)
{
	// ロックオン処理
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);

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

		//Player::Anim_Counter, loop,
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Attack, loop,
		currentAnimationStartSeconds, blendSeconds);

	// アニメーション種類 通常
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


}

// 反射更新
void PlayerReflectionState::Execute(float elapsedTime)
{
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		// 当たり判定の有無
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));

	}


	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。
	bool CollisionFlag = animationTime >= 0.3f && animationTime <= 0.5f;

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


