#include <math.h>
#include "EnemySlime.h"
#include "StateDerived.h"

#include "Player.h"
#include "Mathf.h"

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
	
	enemyid = owner->GetComponent<EnemySlime>();


	moveid = owner->GetComponent<Movement>();
	// 縄張り
	enemyid->SetRandomTargetPosition();

	// アニメーション再生
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Attack, loop, currentAnimationStartSeconds, blendSeconds);
	// アニメーションルール
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Reverseplayback);

	stateTimer = Mathf::RandomRange(6.0f, 8.0f);
	//stateTimer = 60.0f;

	// 着地瞬間
	upOnLading = false;

}

// 徘徊ステートで実行するメソッド
void WanderState::Execute(float elapsedTime)
{
	stateTimer -= elapsedTime;
	// プレイヤーid

	// 目標地点ををプレイヤー位置に設定
	DirectX::XMFLOAT3 targetPosition = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();

	enemyid->SetTargetPosition(targetPosition);
	
	// 目的地点までのXZ平面での距離判定
	float vx = targetPosition.x - enemyid->GetPosition().x;
	float vz = targetPosition.z - enemyid->GetPosition().z;
	float distSq = vx * vx + vz * vz;

	// 目的地へ着いた
	if (stateTimer < 0.0f)
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::IdleBattle));
	}
	// 目的地にこの半径入ったら
	float radius = enemyid->GetAttackRightFootRange();



	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	if (animationTime >= 0.6f && animationTime <= 0.7f)
	{
		// アニメーションルール 停止
		enemyid->SetUpdateAnim(EnemySlime::UpAnim::Stop);
	
		if (distSq > radius * radius)
			// 目的地点へ移動
			enemyid->MoveToTarget(elapsedTime, 0.8f);


		// ジャンプ
		enemyid->InputJump();

		// 着地で衝撃波を飛ばす
		if (moveid->GetOnLadius() && !upOnLading)
		{

			Model::Node* node = owner->GetComponent<ModelControll>()->GetModel()->FindNode("boss_right_foot1");
			DirectX::XMFLOAT3 pos(
				node->worldTransform._41,
				node->worldTransform._42,
				node->worldTransform._43
			);
			enemyid->InputImpact(pos);
			// 着地瞬間
			upOnLading = true;
		}
		if (!moveid->GetOnLadius())
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

	enemyid = owner->GetComponent<EnemySlime>();
	//modelControllid = owner->GetComponent<ModelControll>();
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Anim_Standby, loop, currentAnimationStartSeconds,blendSeconds);
	// アニメーションルール
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);

}

// update
void IdleState::Execute(float elapsedTime)
{

	// TODO 03 
	// 待機時間が経過したとき徘徊ステートへ遷移しなさい
	enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
}

// 最後にやりたい処理全般
void IdleState::Exit()
{
}
// 初期化
void PursuitState::Enter()
{
	// プレイヤーコンポーネントアクセス
	//playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Player>();

	--mortionLimit;
	// エネミーアクセス用
	enemyid = owner->GetComponent<EnemySlime>();
	enemyid->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Standby, loop,currentAnimationStartSeconds,  blendSeconds);

	// アニメーションルール
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);

	// 数秒間追跡するタイマーをランダム設定
	stateTimer = Mathf::RandomRange(3.0f, 5.0f);

	attackRange = enemyid->GetAttackRightFootRange();

	hpid = owner->GetComponent<HP>();

	// 敵ライフ
	attackRound = hpid->GetLife();

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
	
	// 目標地点ををプレイヤー位置に設定
	DirectX::XMFLOAT3 targetPosition = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();


	enemyid->SetTargetPosition(targetPosition);

	if (mortionLimit < 0)
	{
		// 待機時間再設定
		//mortionLimit = Mathf::RandomRange(3.0f, mortionLimitMax);
		mortionLimit = rand() % mortionLimitMax + 3;
		// 限界がきたので休憩
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::IdleBattle));
	}

	// タイマー処理
	stateTimer -= elapsedTime;


	float vx = targetPosition.x - enemyid->GetPosition().x;
	float vy = targetPosition.y - enemyid->GetPosition().y;
	float vz = targetPosition.z - enemyid->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	// TODO 03 
	// 攻撃範囲に入ったとき攻撃ステートへ遷移しなさい
	if (dist < attackRange)
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Attack));


	switch (attackRound)
	{
	case AttackChange::Round1:
	{
		// 時間が過ぎたので
		if (stateTimer < 0.0f)
		{
			
			enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));
		}

		break;
	}

	case AttackChange::Round2:
	{
		if (stateTimer < 0.0f && attackType == 0)
			enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));

		//if (stateTimer < 0.0f && attackType == 1)
		//	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::ShotThrowing));

		break;
	}

	// HP残機０
	case AttackChange::ROund3:
	{

		//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::ShotThrowing));
		if (stateTimer < 0.0f && attackType == 0)
			enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));

		//if (stateTimer < 0.0f && attackType == 1)
		//	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::ShotThrowing));

		if (stateTimer < 0.0f && attackType == 1)
			enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Wander));

		break;
	}


	// HP残機　２と１
	default:
	{


		break;
	}
	}

}
// 最後にやりたい処理全般
void PursuitState::Exit()
{
}
// 初期化
void AttackState::Enter()
{

	enemyid = owner->GetComponent<EnemySlime>();

	hpid = owner->GetComponent<HP>();

	currentAnimationStartSeconds = 0.0f;
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Attack, loop, currentAnimationStartSeconds, blendSeconds);
	

	// アニメーションルール
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);

		// ライフ
	int life;
	life = hpid->GetLife();

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
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
		
		int attackMemoryStart = 0;
		attackMemory = attackMemoryStart;
	}

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	if (animationTime >= 1.0f && animationTime <= 2.0f)
	{
		// 左足ノードとプレイヤーの衝突処理
		enemyid->CollisitionNodeVsPlayer("boss_left_foot1", 2);
	}

	if (animationTime >= 1.30f && animationTime <= 1.33f)
	{
		Model::Node* node = owner->GetComponent<ModelControll>()->GetModel()->FindNode("boss_left_foot1");
		DirectX::XMFLOAT3 pos(
			node->worldTransform._41,
			node->worldTransform._42,
			node->worldTransform._43
		);
		enemyid->InputImpact(pos);
	}


}
// 最後にやりたい処理全般
void AttackState::Exit()
{
}

// 初期設定
void AttackShotState::Enter()
{
	enemyid = owner->GetComponent<EnemySlime>();
	//modelid = owner->GetComponent<ModelControll>();
	hpid = owner->GetComponent<HP>();
	

	currentAnimationStartSeconds = 0.0f;
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Shot, loop, currentAnimationStartSeconds, blendSeconds);

	// アニメーションルール
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);

	// ライフ
	int life;
	life = hpid->GetLife();

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
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
		attackCount = 0;
	}

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// efekt設定
	if (animationTime >= 0.1f && animationTime <= 0.11f)
	{
		//	// 左足ノードとプレイヤーの衝突処理
		enemyid->InputProjectile();

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
	enemyid = owner->GetComponent<EnemySlime>();
	transformid = owner->GetComponent<Transform>();

	currentAnimationStartSecondsf = 0.0f;
	owner->GetComponent<ModelControll>()->GetModel()->PlayReverseAnimation(EnemySlime::Animation::Anim_jewelattack, loop,currentAnimationStartSecondsf,blendSeconds);
	// アニメーションルール
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Reverseplayback);

	// 投げる
	turnPermission = false;

	// 地面判定一度消す
	bool pushuThrow = false;
	enemyid->SetPushuThrow(pushuThrow);

}
// 更新処理
void AttackShotThrowingState::Execute(float elapsedTime)
{
	// 正面
	//DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 angle = transformid->GetAngle();
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
		enemyid->InputThrowingRuby(playerid->GetComponent<Transform>()->GetPosition());

		

	}
	// 回転
	if (animationTime <= 3.6f && animationTime >= 3.0f && !turnPermission)
	{

		enemyid->SetTargetPosition(playerid->GetComponent<Transform>()->GetPosition());
		enemyid->TurnToTarget(elapsedTime, turnSpeed);



		for (int i = 0; i < ProjectileManager::Instance().GetProjectileCount() ; ++i)
		{


			std::shared_ptr<Actor> projectileManager = ProjectileManager::Instance().GetProjectile(i);
			if (projectileManager->GetComponent<ProjectileThrowing>())
			{
				
				projectileManager->GetComponent<Transform>()->SetPosition(
					{ 
						transformid->GetPosition().x,
						transformid->GetPosition().y + transformid->GetHeight(),
						transformid->GetPosition().z

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
		enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);


		turnPermission = true;
	}
	// 射撃許可
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation() && turnPermission)
	{
		// 地面判定発生
		bool pushuThrow = true;
		enemyid->SetPushuThrow(pushuThrow);
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
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	}




}
// 終了処理
void AttackShotThrowingState::Exit()
{
}

void DamageState::Enter()
{
	enemyid = owner->GetComponent<EnemySlime>();

	owner->GetComponent<ModelControll>() = owner->GetComponent<ModelControll>();
	owner->GetComponent<ModelControll>()->GetModel()->
		PlayAnimation(
			EnemySlime::Animation::Anim_Die, loop);
	// アニメーションルール
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);



}

void DamageState::Execute(float elapsedTime)
{
		if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));

}

void DamageState::Exit()
{
}

// 混乱開始
void ConfusionState::Enter()
{
	enemyid = owner->GetComponent<EnemySlime>();

	owner->GetComponent<ModelControll>() = owner->GetComponent<ModelControll>();

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Die, loop, currentAnimationStartSeconds, blendSeconds);
	// アニメーションルール
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);

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
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	}

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// ひざまつく
	if (animationTime >= 1.1)
	{
		// アニメーションルール
		enemyid->SetUpdateAnim(EnemySlime::UpAnim::Stop);

	}

}
// 混乱終了
void ConfusionState::Exit()
{

}

void DeathState::Enter()
{
	enemyid = owner->GetComponent<EnemySlime>();
	transformid = owner->GetComponent<Transform>();


	owner->GetComponent<ModelControll>() = owner->GetComponent<ModelControll>();
	owner->GetComponent<ModelControll>()->GetModel()->
		PlayAnimation(
			EnemySlime::Animation::Anim_Die, loop);
	// アニメーションルール
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);

	stateTimer = 3;

	bool freeCheckCamera = false;

	int playerCountMax = PlayerManager::Instance().GetPlayerCount();
	if (playerCountMax > 0)
	PlayerManager::Instance().GetPlayer(playerCountMax - 1)->GetComponent<Player>()->SetFreeCameraCheck(freeCheckCamera);
}

void DeathState::Execute(float elapsedTime)
{

	stateTimer -= elapsedTime; 
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

	if (stateTimer < 0)
	{

		enemyid->SetClearCheck(clearCheck);
	}
}

void DeathState::Exit()
{
	return;
}

// プレイヤー
void PlayerIdleState::Enter()
{
	//playerid = owner->GetComponent<Player>();

	moveid = owner->GetComponent<Movement>();

	//modelid = owner->GetComponent<ModelControll>();

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Idle, loop,currentAnimationStartSeconds,blendSeconds
	);
	
	// アニメーションルール
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);
	

	// 落ちる
	bool stop = false;
	moveid->SetStopMove(stop);
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
	owner->GetComponent<Player>() = owner->GetComponent<Player>();

	moveid = owner->GetComponent<Movement>();

	//modelid = owner->GetComponent<ModelControll>();

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
	moveid->SetStopMove(stop);
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
	modelControllid = owner->GetComponent<ModelControll>();
	moveid = owner->GetComponent<Movement>();

	modelControllid->GetModel()->PlayAnimation(
		Player::Anim_Jump, loop,
	currentAnimationStartSeconds, blendSeconds);

	// アニメーションルール
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


	// 落ちる
	bool stop = false;
	moveid->SetStopMove(stop);
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

	if (moveid->GetOnLadius())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Land));
	}


}

void PlayerJumpState::Exit()
{
}

void PlayerLandState::Enter()
{
	modelControllid = owner->GetComponent<ModelControll>();
	moveid = owner->GetComponent<Movement>();
	//moveid->SetOnLadius(false);

	modelControllid->GetModel()->PlayAnimation(
		Player::Anim_Land, loop,
		currentAnimationStartSeconds, blendSeconds
	);

	// アニメーションルール
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerLandState::Execute(float elapsedTime)
{
	// ロックオン処理
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);



	if (!modelControllid->GetModel()->IsPlayAnimation())
	{
		//moveid->SetOnLadius(false);
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));


	}
}

void PlayerLandState::Exit()
{
}

void PlayerJumpFlipState::Enter()
{
	modelControllid = owner->GetComponent<ModelControll>();
	//moveid = owner->GetComponent<Movement>();

	modelControllid->GetModel()->PlayAnimation(
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
	if (!modelControllid->GetModel()->IsPlayAnimation())
	{

		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
	}

	// 回避
	if (owner->GetComponent<Player>()->InputAvoidance())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}

}

void PlayerJumpFlipState::Exit()
{


}

void PlayerQuickJabState::Enter()
{
	bool loop = false;

	modelControllid = owner->GetComponent<ModelControll>();
	moveid = owner->GetComponent<Movement>();
	transformid = owner->GetComponent<Transform>();

	//std::shared_ptr<Player> playerId = owner->GetComponent<Player>();

		//Player::Anim_Slash, loop,
		// １回

		// 再生開始
		currentAnimationStartSeconds = 0.0f;
		// アニメーション再生
		modelControllid->GetModel()->PlayAnimation(
			Player::Anim_Slash, loop,
			currentAnimationStartSeconds, blendSeconds
			, currentAnimationAddSeconds
		);
		// 攻撃三回で一度強制終了
		++attackMemory;


	

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
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

	
	//moveid->SetGravity(gravity);
	// 重力オフ
	bool stopFall = true;
	moveid->SetStopFall(stopFall);



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

	// ロックオン処理
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);




	if (owner->GetComponent<Player>()->InputAttack() && 
		owner->GetComponent<Player>()->GetSelectCheck() ==
		(int)Player::CommandAttack::Attack)
	{
		// コマンド確認2弾攻撃
		if (gamePad.ConfirmCommand(commandSeconde, frame) && !button && !buttonSeconde)
		{
			button = true;
			//owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
		}
	}

	// 回避
	if (owner->GetComponent<Player>()->InputAvoidance())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

		button = false;
		buttonSeconde = false;
	}

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
	}
	if (length > attackCheckRangeMin)
	{
		bool stop = false;
		moveid->SetStopMove(stop);

		angle = transformid->GetAngle();

		DirectX::XMFLOAT3 direction;
		direction.x = sinf(angle.y) * 6;
		direction.y = 0;
		direction.z = cosf(angle.y) * 6;

		moveid->Move(direction, attackSpeed, elapsedTime);
	}
	else
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
				//bool stop = false;
				//moveid->SetStopMove(stop);
				
				// 正面
				if (moveid->Turn(vector, turnSpeed, elapsedTime))
				{

					moveid->Move(vector, speed, elapsedTime);
					
				}
				//stop = true;
				//moveid->SetStopMove(stop);
			}
			else
			{

			

				rotateCheck = true;
			}


		

	}
	if (attackMemory > attackMemoryMax)
	{
		// 入力確認でステート変更
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


		// 攻撃最大値調整
		attackMemory = 0;
	}



	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = modelControllid->GetModel()->GetCurrentANimationSeconds();
	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。


	// 1撃目
	if (animationTime >= 0.8f )
	{
		// １回目の攻撃なら
		oneAttackCheck = false;

		owner->GetComponent<Player>()->SetAttackCollisionFlag(false);
		deleteCheck = false;
		// 入力確認でステート変更
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(button  ? Player::State::SideCut : Player::State::Move));
		return;
	}



	owner->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");
}

void PlayerQuickJabState::Exit()
{
	// 移動の停止
	moveid->SetGravity(
		owner->GetComponent<Player>()->GetGravity());




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

}


void PlayerSideCutState::Enter()
{
	bool loop = false;

	modelControllid = owner->GetComponent<ModelControll>();
	moveid = owner->GetComponent<Movement>();
	transformid = owner->GetComponent<Transform>();

	// 二回

		// 再生開始
		currentAnimationStartSeconds = 0.2f;

		// アニメーション再生
		modelControllid->GetModel()->PlayAnimation(
			Player::Anim_SlashBeside, loop,
			currentAnimationStartSeconds, blendSeconds
			, currentAnimationAddSeconds
		);

	

	if (!InitializationCheck)
	{

		// コマンド設定二回目攻撃
		commandSeconde.push_back(GamePad::BTN_B);


		// コマンド設定三回目攻撃
		commandThrede.push_back(GamePad::BTN_B);


	}
	// アニメーション再生
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


	//moveid->SetGravity(gravity);
	// 重力オフ
	bool stopFall = true;
	moveid->SetStopFall(stopFall);

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

	// ロックオン処理
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);



	//angle = transformid->GetAngle();

	//DirectX::XMFLOAT3 direction;
	//direction.x = sinf(angle.y) * 6;
	//direction.y = 0;
	//direction.z = cosf(angle.y) * 6;

	//moveid->Move(direction, attackSpeed, elapsedTime);

	if (owner->GetComponent<Player>()->InputAttack() &&
		owner->GetComponent<Player>()->GetSelectCheck() ==
		(int)Player::CommandAttack::Attack)
	{
		// コマンド確認3弾攻撃
		if (gamePad.ConfirmCommand(commandThrede, frame))
		{
			buttonSeconde = true;
			//owner->GetComponent<Pl/ayer>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
		}

	}

	// 回避
	if (owner->GetComponent<Player>()->InputAvoidance())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

		button = false;
		buttonSeconde = false;
	}


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
	}

	if (length > attackCheckRangeMin)
	{
		bool stop = false;
		moveid->SetStopMove(stop);

		angle = transformid->GetAngle();

		DirectX::XMFLOAT3 direction;
		direction.x = sinf(angle.y) * 6;
		direction.y = 0;
		direction.z = cosf(angle.y) * 6;

		moveid->Move(direction, attackSpeed, elapsedTime);
	}
	else
	{
	
		bool stop = true;
		moveid->SetStopMove(stop);

	}

	// 回転
	if (!rotateCheck)
	{
		//DirectX::XMVECTOR Vector;
		//DirectX::XMVECTOR LengthSq;

		//DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&owner->GetComponent<Transform>()->GetPosition());
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
				&& moveid->TurnCheck(
					vector, angleRange, elapsedTime))
			{
				//bool stop = false;
				//moveid->SetStopMove(stop);

				// 正面
				if (moveid->Turn(vector, turnSpeed, elapsedTime))
				{

					moveid->Move(vector, speed, elapsedTime);

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
	float animationTime = modelControllid->GetModel()->GetCurrentANimationSeconds();
	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。


	// 2撃目
	if (animationTime >= 1.1f)
	{
		owner->GetComponent<Player>()->SetAttackCollisionFlag(false);
		deleteCheck = false;



		// 入力確認でステート変更
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(buttonSeconde ? Player::State::CycloneStrike : Player::State::Move));

		return;
	}


	owner->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");

}

void PlayerSideCutState::Exit()
{
	// 移動の停止
	moveid->SetGravity(
		owner->GetComponent<Player>()->GetGravity());


	// 重力オフ
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
	
}


void PlayerCycloneStrikeState::Enter()
{
	bool loop = false;

	modelControllid = owner->GetComponent<ModelControll>();
	moveid = owner->GetComponent<Movement>();
	transformid = owner->GetComponent<Transform>();

		// 再生開始
		currentAnimationStartSeconds = 0.7f;
		// アニメーション再生
		modelControllid->GetModel()->PlayAnimation(
			Player::Anim_SlashThree, loop,
			currentAnimationStartSeconds, blendSeconds
			, currentAnimationAddSeconds
		);



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
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

	// 重力オフ
	bool stopFall = true;
	moveid->SetStopFall(stopFall);

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

	// ロックオン処理
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);


	//angle = transformid->GetAngle();

	//DirectX::XMFLOAT3 direction;
	//direction.x = sinf(angle.y) * 6;
	//direction.y = 0;
	//direction.z = cosf(angle.y) * 6;

	//moveid->Move(direction, attackSpeed, elapsedTime);

	// 回避
	if (owner->GetComponent<Player>()->InputAvoidance())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

		button = false;
		buttonSeconde = false;
	}


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
	}

	if (length > attackCheckRangeMin)
	{
		bool stop = false;
		moveid->SetStopMove(stop);

		angle = transformid->GetAngle();

		DirectX::XMFLOAT3 direction;
		direction.x = sinf(angle.y) * 6;
		direction.y = 0;
		direction.z = cosf(angle.y) * 6;

		moveid->Move(direction, attackSpeed, elapsedTime);
	}
	else
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
				//bool stop = false;
				//moveid->SetStopMove(stop);

				// 正面
				if (moveid->Turn(vector, turnSpeed, elapsedTime))
				{

					moveid->Move(vector, speed, elapsedTime);

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
	float animationTime = modelControllid->GetModel()->GetCurrentANimationSeconds();
	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。


	// 3撃目
	if (animationTime >= 2.7f )
	{
		owner->GetComponent<Player>()->SetAttackCollisionFlag(false);
		deleteCheck = false;



		// 入力確認でステート変更
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
		return;
	}


	owner->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");

}

void PlayerCycloneStrikeState::Exit()
{
	// 移動の停止
	moveid->SetGravity(
		owner->GetComponent<Player>()->GetGravity());



	// 重力オフ
	bool stopFall = false;
	moveid->SetStopFall(stopFall);

}


void PlayerSpecialAttackState::Enter()
{

	transformid = owner->GetComponent<Transform>();
	modelControllid = owner->GetComponent<ModelControll>();
	moveid = owner->GetComponent<Movement>();




	enemyTransform = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<Transform>();
	enemyHpId = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<HP>();

	lightning = std::make_unique<Effect>("Data/Effect/sunder.efk");
	lightningAttack = std::make_unique<Effect>("Data/Effect/HitThunder.efk");

	// アニメーション再生
	modelControllid->GetModel()->PlayAnimation(
		Player::Anim_MagicSeconde, loop,
		currentAnimationStartSeconds, blendSeconds
		, currentAnimationAddSeconds
	);

	Model::Node* pHPosiiton = modelControllid->GetModel()->FindNode("mixamorig:LeftHand");

	DirectX::XMFLOAT3 pPosition =
	{
				pHPosiiton->worldTransform._41,
				pHPosiiton->worldTransform._42,
				pHPosiiton->worldTransform._43
	};

	lightning->Play(pPosition);
	// アニメーション再生
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

	// 落ちるの停止
	bool stopFall = true;
	moveid->SetStopFall(stopFall);

	button = true;

	// フラッシュ
	flashOn = true;


	// 加速
	currentAnimationAddSeconds = 0.0f;
}

void PlayerSpecialAttackState::Execute(float elapsedTime)
{
	if (button)
	{

		MessageData::CAMERACHANGEMOTIONMODEDATA	p;

		position = transformid->GetPosition();
		angle = transformid->GetAngle();



		//float vx = sinf(angle.y) * 6;
		//float vz = cosf(angle.y) * 6;
		//p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
		//p.data.push_back({ 95, {position.x + vx , position.y + 3, position.z + vz }, position });

		//float vx = sinf(angle.y) * 6;
		//float vz = cosf(angle.y) * 6;

		//float vx2 = sinf(angle.y);
		//float vz2 = cosf(angle.y);
		//p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
		//p.data.push_back({ 60, {position.x - vx, position.y + 3, position.z - vz }, position });
		//p.data.push_back({ 90, {position.x - vx , position.y + 3, position.z - vz }, position });

		float vx = sinf(angle.y) * 6;
		float vz = cosf(angle.y) * 6;

		float vx2 = sinf(angle.y) - 10;
		float vz2 = cosf(angle.y) * 7;
		float vx3 = sinf(angle.y);

		p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
	    p.data.push_back({ 50, {position.x + vx, position.y + 3, position.z + vz }, position });
		p.data.push_back({ 100, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
		p.data.push_back({ 140, {position.x + vx3 , position.y + 1, (position.z + 0.1f) - vz2 }, position });
		p.data.push_back({ 200, {position.x + vx3 , position.y + 0.5f, (position.z + 0.1f) - vz2 }, position });



		Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

					// 任意のアニメーション再生区間でのみ衝突判定処理をする
		float animationTime = modelControllid->GetModel()->GetCurrentANimationSeconds();


		// アニメーション
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON)
		{
	

			// アニメーション再生
			modelControllid->GetModel()->PlayAnimation(
				Player::Anim_SpecialAttack, loop,
				currentAnimationStartSeconds, blendSeconds
				, currentAnimationAddSeconds
			);
			button = false;


			lightning->Stop(lightning->GetEfeHandle());


			//rockCheck = true;
			//owner->GetComponent<Player>()->SetRockCheck(rockCheck);


			
		}
	}
	else
	{
		// 例えばコレを必殺技などで上手く利用できればかっこいいカメラ演出が作れますね
		//MessageData::CAMERACHANGEMOTIONMODEDATA	p;

		//DirectX::XMFLOAT3 position = transformid->GetPosition();
		//DirectX::XMFLOAT3 angle = transformid->GetAngle();



		/*float vx = sinf(angle.y);
		float vz = cosf(angle.y);*/
		//p.data.push_back({ 0, {position.x * vx , position.y + 3, position.z - vz }, position });

		// ロックオン
		//rockCheck = true;
		//owner->GetComponent<Player>()->SetRockCheck(rockCheck);

		//owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);

		// フリーカメラのロックを外す
		//bool freeCameraCheck = false;
		//owner->GetComponent<Player>()->SetFreeCameraCheck(freeCameraCheck);

		//position = transformid->GetPosition();
		//angle = transformid->GetAngle();


		//DirectX::XMVECTOR PPosition = DirectX::XMLoadFloat3(&position);
		//DirectX::XMVECTOR EPosition = DirectX::XMLoadFloat3(&enemyTransform->GetPosition());

		//DirectX::XMVECTOR vector = DirectX::XMVectorSubtract(EPosition,PPosition);
		//vector = DirectX::XMVector3Normalize(vector);

		//DirectX::XMFLOAT3 direction;
		//DirectX::XMStoreFloat3(&direction, vector);

		//DirectX::XMFLOAT3 pPosition;

		//pPosition =
		//{
		//	position.x * direction.x ,
		//	position.y * direction.y,
		//	position.z * direction.z
		//};
		//MessageData::CAMERACHANGEMOTIONMODEDATA	p;
		//p.data.push_back({ 0, pPosition, position });
		//Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);



		//Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

		//owner->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");

		/*lightningAttack->SetPosition(lightningAttack->GetEfeHandle(), modelControllid->GetModel()->FindNode("mixamorig:LeftHand")->position);*/

			// 任意のアニメーション再生区間でのみ衝突判定処理をする
		float animationTime = modelControllid->GetModel()->GetCurrentANimationSeconds();


		// スローモーションと色を明るく
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON)
		{
			owner->GetComponent<Player>()->SetFlashOn(flashOn);
			owner->GetComponent<Player>()->SetHitCheck(true);
		}
		// アニメーション切りつけ
		if (animationTime >= 1.6f)
		{
			float invincibleTimer = 0.0f;
			owner->GetComponent<HP>()->SetInvincibleTimer(invincibleTimer);




			// カメラ振動
			MessageData::CAMERASHAKEDATA cameraShakeData;

			float shakeTimer = 0.5f;
			float shakePower = 0.8f;
			cameraShakeData = { shakeTimer , shakePower };

			Messenger::Instance().SendData(MessageData::CAMERASHAKE, &cameraShakeData);

			Model::Node* pHPosiiton = modelControllid->GetModel()->FindNode("mixamorig:LeftHand");

			DirectX::XMFLOAT3 pPosition =
			{
						pHPosiiton->worldTransform._41,
						pHPosiiton->worldTransform._42,
						pHPosiiton->worldTransform._43
			};



			lightningAttack->Play(pPosition);


			enemyHpId->ApplyDamage(10, 0.5f);
			owner->GetComponent<Player>()->SetHitCheck(false);

			//owner->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");
		}
		// アニメーション終了
		if (!modelControllid->GetModel()->IsPlayAnimation())
		{
			// 入力確認でステート変更
			owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));

		}
	}

}

void PlayerSpecialAttackState::Exit()
{
	// 落ちるの再開
	bool stopFall = false;
	moveid->SetStopFall(stopFall);



	//rockCheck = false;
	//owner->GetComponent<Player>()->SetRockCheck(rockCheck);

	//// フリーカメラのロックをとどめる
	//bool freeCameraCheck = true;
	//owner->GetComponent<Player>()->SetFreeCameraCheck(freeCameraCheck);

}

void PlayerMagicState::Enter()
{

	modelControllid = owner->GetComponent<ModelControll>();
	moveid = owner->GetComponent<Movement>();


	// サンダー系
	if (owner->GetComponent<Player>()->GetSelectMagicCheck() ==
		(int)Player::CommandMagic::Thander)
	{
		// アニメーション再生
		modelControllid->GetModel()->PlayAnimation(
			Player::Anim_MagicSeconde, loop,
			currentAnimationStartSeconds, blendSeconds
		);


	}
	else
	{
		// アニメーション再生
		modelControllid->GetModel()->PlayAnimation(
			Player::Anim_Magic, loop,
			currentAnimationStartSeconds, blendSeconds
		);
	}

	// アニメーション再生
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


	magicType = owner->GetComponent<Player>()->GetSelectMagicCheck();
	// 落ちるの停止
	bool stopFall = true;
	moveid->SetStopFall(stopFall);

}

void PlayerMagicState::Execute(float elapsedTime)
{

	// ロックオン処理
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = modelControllid->GetModel()->GetCurrentANimationSeconds();
	// アニメーション終わったら通る



	// 魔法の種類で終わりを帰る。
	switch (magicType)
	{
	case (int)Player::CommandMagic::Fire:
	{
		// 時間
		if (animationTime <= 0.5f)return;
		// 炎発射
		owner->GetComponent<Player>()->InputMagicframe();

		owner->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Idle);
		break;
	}

	case (int)Player::CommandMagic::Thander:
	{
		// 時間
		if (animationTime <= 1.1f)return;

		// 雷発射
		owner->GetComponent<Player>()->InputMagicLightning();
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Idle);
		break;
	}
	case (int)Player::CommandMagic::Ice:
	{
		// 時間
		if (animationTime <= 0.5f)return;
		// 氷発射
		owner->GetComponent<Player>()->InputMagicIce();
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Idle);
		break;
	}

	default:
		break;
	}

}

void PlayerMagicState::Exit()
{
	// 魔法の選択をゼロに
	owner->GetComponent<Player>()->SetSelectCheck((int)Player::CommandMagic::Normal);

	// 落ちるの再開
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
}

void PlayerSpecialMagicState::Enter()
{

	transformid = owner->GetComponent<Transform>();
	modelControllid = owner->GetComponent<ModelControll>();
	moveid = owner->GetComponent<Movement>();

	fire = std::make_unique<Effect>("Data/Effect/fire.efk");
	fireAttack = std::make_unique<Effect>("Data/Effect/hit fire.efk");
	// 再生開始時間
	currentAnimationStartSeconds = 0.0f;

	// アニメーション再生
	modelControllid->GetModel()->PlayAnimation(
		Player::Anim_SlashThree, loop,
		currentAnimationStartSeconds, blendSeconds
		, currentAnimationAddSeconds
	);
	// アニメーション再生
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


	Model::Node* pHPosiiton = modelControllid->GetModel()->FindNode("mixamorig:LeftHand");

	DirectX::XMFLOAT3 pPosition =
	{
				pHPosiiton->worldTransform._41,
				pHPosiiton->worldTransform._42,
				pHPosiiton->worldTransform._43
	};

	fire->Play(pPosition);
	// 落ちるの停止
	bool stopFall = true;
	moveid->SetStopFall(stopFall);

	button = true;

	// スタート値
	specialMoveWaitTime = specialMoveWaitStartTime;


}

void PlayerSpecialMagicState::Execute(float elapsedTime)
{
	if (button)
	{

		// カメラモーション
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
		p.data.push_back({ 350, {position.x + vx3 , position.y + 0.5f, (position.z + 0.1f) - vz2 }, position });

		// モーション設定
		Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

		// 任意のアニメーション再生区間でのみ衝突判定処理をする
		//float animationTime = modelControllid->GetModel()->GetCurrentANimationSeconds();
		
		// エフェクト更新
		if(fire->GetEfeHandle())
		{
			Model::Node* pHPosiiton = modelControllid->GetModel()->FindNode("mixamorig:LeftHand");

			DirectX::XMFLOAT3 pPosition =
			{
						pHPosiiton->worldTransform._41,
						pHPosiiton->worldTransform._42,
						pHPosiiton->worldTransform._43
			};

			fire->SetPosition(fire->GetEfeHandle(),
				pPosition);
		}



		// アニメーション
		if (!modelControllid->GetModel()->IsPlayAnimation())
		{
			// 再生開始時間
			currentAnimationStartSeconds = 0.0f;

			// アニメーション再生
			modelControllid->GetModel()->PlayAnimation(
				Player::Anim_MagicSeconde, loop,
				currentAnimationStartSeconds, blendSeconds
				, currentAnimationAddSeconds
			);
			button = false;

			// 手の炎エフェクト停止
			fire->Stop(fire->GetEfeHandle());


			//rockCheck = true;
			//owner->GetComponent<Player>()->SetRockCheck(rockCheck);


			
		}
	}
	else
	{
			// 任意のアニメーション再生区間でのみ衝突判定処理をする
		float animationTime = modelControllid->GetModel()->GetCurrentANimationSeconds();

		// アニメーション
		if (animationTime >= 1.1f)
		{

			owner->GetComponent<Player>()->SetFlashOn(true);
			//owner->GetComponent<Player>()->SetHitCheck(true);

			// アニメーション停止
			owner->GetComponent<Player>()->SetUpdateAnim
			(Player::UpAnim::Stop);


			// カメラ振動
			MessageData::CAMERASHAKEDATA cameraShakeData;

			// カメラ振動値
			float shakeTimer = 0.5f;
			float shakePower = 0.8f;
			cameraShakeData = { shakeTimer , shakePower };
			// カメラ振動設定
			Messenger::Instance().SendData(MessageData::CAMERASHAKE, &cameraShakeData);

			Model::Node* pHPosiiton = modelControllid->GetModel()->FindNode("mixamorig:LeftHand");

			DirectX::XMFLOAT3 pPosition =
			{
						pHPosiiton->worldTransform._41,
						pHPosiiton->worldTransform._42,
						pHPosiiton->worldTransform._43
			};


			// 魔法発射
			owner->GetComponent<Player>()->InputSpecialMagicframe();


			// 必殺技を放った後の経過時間
			specialMoveWaitTime += elapsedTime;

		}
	}



	// ダメージ判定
	owner->GetComponent<Player>()->SpecialApplyDamageInRadius();



	// 待機時間が過ぎたので次のステートへ
	if (specialMoveWaitTime >= specialMoveWaitTimeMax)
	{
		// 次のステートへ
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


		return;
	}

	


}

void PlayerSpecialMagicState::Exit()
{
	// 落ちるの再開
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
}

void PlayerDamageState::Enter()
{
	modelControllid = owner->GetComponent<ModelControll>();

	bool loop = false;

		//Player::Anim_Pain, loop,
	modelControllid->GetModel()->PlayAnimation(
		Player::Anim_Pain, loop,
		currentAnimationStartSeconds, blendSeconds);

	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerDamageState::Execute(float elapsedTime)
{
	// ロックオン処理
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);

	if (!modelControllid->GetModel()->IsPlayAnimation())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));

	}
}

void PlayerDamageState::Exit()
{
}

void PlayerDeathState::Enter()
{
	modelControllid = owner->GetComponent<ModelControll>();
	hpid = owner->GetComponent<HP>();
	transformid = owner->GetComponent<Transform>();
		//Player::Anim_Deth, loop,
	modelControllid->GetModel()->PlayAnimation(
		Player::Anim_Deth, loop,
		currentAnimationStartSeconds ,blendSeconds
	);

	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

	stateTimer = 0.0f;


}

void PlayerDeathState::Execute(float elapsedTime)
{

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
	transformid = owner->GetComponent<Transform>();
	moveid = owner->GetComponent<Movement>();
	hpid = owner->GetComponent<HP>();

		//Player::Anim_Slash, loop
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Dush, loop
	,currentAnimationStartSeconds, blendSeconds);


	// 当たり判定の有無
	owner->GetComponent<Player>()->DmageInvalidJudment(false);

	moveSpeed = 10.0f;

	// アニメーション種類 通常
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

	// 落ちるの停止
	bool stopFall = true;
	moveid->SetStopFall(stopFall);

}

// 回避更新
void PlayerAvoidanceState::Execute(float elapsedTime)
{
	// 動き自由
	bool stopMove = false;
	moveid->SetStopMove(stopMove);

	// ロックオン処理
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);

	// 転がりダッシュ
	DirectX::XMFLOAT3 dir;
	dir.x = sinf(transformid->GetAngle().y);
	dir.y = cosf(transformid->GetAngle().x);
	dir.z = cosf(transformid->GetAngle().y);

	
	// 任意のアニメーション再生区間
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// この時間で
	if (animationTime >= 0.7f && animationTime <= 1.5f)
	{
		moveid->Move(dir, moveSpeed, elapsedTime);
	}
	// アニメーション終了
	//if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	if (animationTime >= 2.0f)
	{

		// 当たり判定の有無
		owner->GetComponent<Player>()->DmageInvalidJudment(true);
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));



	}
}

// 回避終了
void PlayerAvoidanceState::Exit()
{
	// 落ちるの停止
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
}

// 反射開始
void PlayerReflectionState::Enter()
{
	bool loop = false;

		//Player::Anim_Counter, loop,
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Counter, loop,
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


