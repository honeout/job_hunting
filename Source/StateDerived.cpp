#include "EnemySlime.h"
#include "StateDerived.h"
#include "Input\GamePad.h"
#include "Player.h"
#include "Mathf.h"



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
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetComponent<EnemySlime>()->GetAnimationStateWalk(), true);
}

// 徘徊ステートで実行するメソッド
void WanderState::Execute(float elapsedTime)
{
	// 目的地点までのXZ平面での距離判定
	float vx = owner->GetComponent<EnemySlime>()->GetTargetPosition().x - owner->GetComponent<EnemySlime>()->GetPosition().x;
	float vz = owner->GetComponent<EnemySlime>()->GetTargetPosition().z - owner->GetComponent<EnemySlime>()->GetPosition().z;
	float distSq = vx * vx + vz * vz;

	// 目的地へ着いた
	float radius = owner->GetComponent<EnemySlime>()->GetRadius();
	if (distSq < radius * radius)
	{
		// 待機ステートへ遷移
		// ChangeStateクラスでStateを切り替える
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Idle));
	}
	// 目的地点へ移動
	owner->GetComponent<EnemySlime>()->MoveToTarget(elapsedTime, 0.5f);
	// プレイヤー索敵
	if (owner->GetComponent<EnemySlime>()->SearchPlayer())
	{
		// 見つかったら追跡ステートへ遷移
		// ChangeStateクラスでStateを切り替える
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	}

}

// 徘徊ステートから出ていくときのメソッド
void WanderState::Exit()
{

}

// 初期化
void IdleState::Enter()
{
	owner->GetComponent<EnemySlime>()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetComponent<EnemySlime>()->GetAnimationStateNormal(), true);
	// タイマーをランダム設定
	//stateTimer = Mathf::RandomRange(3.0f, 5.0f);
	owner->GetComponent<EnemySlime>()->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));

}

// update
void IdleState::Execute(float elapsedTime)
{
	// タイマー処理
     //stateTimer -= elapsedTime;
	/* owner->SetStateTimer(stateTimer);*/
	owner->GetComponent<EnemySlime>()->SetStateTimer(owner->GetComponent<EnemySlime>()->GetStateTimer() - elapsedTime);
	// TODO 03 
	// 待機時間が経過したとき徘徊ステートへ遷移しなさい
	if (owner->GetComponent<EnemySlime>()->GetStateTimer() < 0.0f)
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Wander));
	// TODO 03 
	// プレイヤーが見つかったとき追跡ステートへ遷移しなさい
	if (owner->GetComponent<EnemySlime>()->SearchPlayer())
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
}

// 最後にやりたい処理全般
void IdleState::Exit()
{
}
// 初期化
void PursuitState::Enter()
{
	
	owner->GetComponent<EnemySlime>()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetComponent<EnemySlime>()->GetAnimationStateWalk(), true);

	// 数秒間追跡するタイマーをランダム設定
	stateTimer = Mathf::RandomRange(3.0f, 5.0f);
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
	owner->GetComponent<EnemySlime>()->MoveToTarget(elapsedTime, 1.0);

	
	// タイマー処理
	stateTimer -= elapsedTime;
	/*owner->SetStateTimer(stateTimer);*/

	// TODO 03 
	// 追跡時間が経過したとき待機ステートへ遷移しなさい
	if (stateTimer < 0.0f)
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Idle));


	float vx = targetPosition.x - owner->GetComponent<EnemySlime>()->GetPosition().x;
	float vy = targetPosition.y - owner->GetComponent<EnemySlime>()->GetPosition().y;
	float vz = targetPosition.z - owner->GetComponent<EnemySlime>()->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	// TODO 03 
	// 攻撃範囲に入ったとき攻撃ステートへ遷移しなさい
	if (dist < attackRange)
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Attack));

}
// 最後にやりたい処理全般
void PursuitState::Exit()
{
}
// 初期化
void AttackState::Enter()
{

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetComponent<EnemySlime>()->GetAnimationStateAttack(), false);
}
// update
void AttackState::Execute(float elapsedTime)
{


	// 攻撃モーションが終了したとき追跡ステートへ移行
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		owner->GetComponent<EnemySlime>()->SetCounterJudgment(false);
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	}

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	if (animationTime >= 0.2f && animationTime <= 0.35f)
	{
		// 目玉ノードとプレイヤーの衝突処理
		owner->GetComponent<EnemySlime>()->CollisitionNodeVsPlayer("EyeBall", 0.2f);
	}
	if (animationTime >= 0.2f && animationTime <= 0.5f)
	{
		owner->GetComponent<EnemySlime>()->SetCounterJudgment(true);
	}


}
// 最後にやりたい処理全般
void AttackState::Exit()
{
}


void DamageState::Enter()
{
	bool loop = false;

	owner->GetComponent<ModelControll>()->GetModel()->
		PlayAnimation(
			owner->GetComponent<EnemySlime>()->GetAnimationStateDie(), loop);


}

void DamageState::Execute(float elapsedTime)
{
		if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));

}

void DamageState::Exit()
{
}

// プレイヤー
void PlayerIdleState::Enter()
{
	bool loop = true;
	//owner->GetComponent<Player>()
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Idle, loop
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

	//// 反射入力処理
	//if (owner->GetComponent<Player>()->InputAvoidance())
	//{
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Reflection));

	//}

	// ジャンプ入力処理
	if (owner->GetComponent<Player>()->InputJump())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
		//TransitionJumpState();
	}


	owner->GetComponent<Player>()->InputSelectCheck();

	if (owner->GetComponent<Player>()->InputAttack() && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::SpecialAttack::Attack)
	{
		//stated = state;
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
		//TransitionAttackState();
	}





	// 弾丸入力処理
	else if (owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::SpecialAttack::MagicFire)
	{
		owner->GetComponent<Player>()->InputProjectile();
		//TransitionAttackState();
	}

	// 特殊攻撃
	if (owner->GetComponent<Player>()->InputSpecialAttackCharge())
	{
		//TransitionAttackState();
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
	}
	// 特殊魔法
	if (owner->GetComponent<Player>()->InputSpecialShotCharge())
	{
		owner->GetComponent<Player>()->InputProjectile();
	}
}

void PlayerIdleState::Exit()
{
}

void PlayerMovestate::Enter()
{
	bool loop = true;
	//owner->GetComponent<Player>()
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Running, loop
	);

	owner->GetComponent<ModelControll>()->GetModel()->PlayUpeerBodyAnimation(
		owner->GetComponent<Player>()->Anim_Running, loop
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

	owner->GetComponent<Player>()->InputSelectCheck();

	// 通常攻撃
	if (owner->GetComponent<Player>()->InputAttack() && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::SpecialAttack::Attack)
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
	}



	// 弾丸入力処理
	else if (owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::SpecialAttack::MagicFire)
	{
		owner->GetComponent<Player>()->InputProjectile();
	}

	// 特殊攻撃
	if (owner->GetComponent<Player>()->InputSpecialAttackCharge())
	{
		//TransitionAttackState();
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
	}
	// 特殊魔法
	if (owner->GetComponent<Player>()->InputSpecialShotCharge())
	{
		owner->GetComponent<Player>()->InputProjectile();
	}
	// 落下着地
	owner->GetComponent<Player>()->Ground();


}

void PlayerMovestate::Exit()
{
}

void PlayerJumpState::Enter()
{
	bool loop = false;

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Jump, loop);

	// アニメーションルール
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);

}

void PlayerJumpState::Execute(float elapsedTime)
{
	// ジャンプ入力処理
	if (owner->GetComponent<Player>()->InputJump() && owner->GetComponent<Player>()->GetJumpCount() >= 1)
	{
		//TransitionJumpFlipState();
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
	}

	// 弾丸入力処理
	if (owner->GetComponent<Player>()->InputProjectile())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
		//TransitionAttackState();
	}


	// 落下着地
	if (owner->GetComponent<Player>()->Ground())
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Land));

	
}

void PlayerJumpState::Exit()
{
}

void PlayerLandState::Enter()
{
	bool loop = false;

	owner->GetComponent<Movement>()->SetOnLadius(false);

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Landing, loop
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
}

void PlayerLandState::Exit()
{
}

void PlayerJumpFlipState::Enter()
{
	bool loop = false;

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Jump_Flip, loop);

	// アニメーションルール
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);
}

void PlayerJumpFlipState::Execute(float elapsedTime)
{

	if (owner->GetComponent<Player>()->InputMove(elapsedTime))
		bool afterimagemove = true;
	// ジャンプ入力処理
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{

		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
	}

	// 落下着地
	if (owner->GetComponent<Player>()->Ground())
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Land));

}

void PlayerJumpFlipState::Exit()
{


}

void PlayerAttackState::Enter()
{
	bool loop = false;

	//owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
	//	owner->GetComponent<Player>()->Anim_Attack, loop);

	owner->GetComponent<ModelControll>()->GetModel()->PlayUpeerBodyAnimation(
		owner->GetComponent<Player>()->Anim_Attack, loop
	);

	// アニメーションルール
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Doble);

}

void PlayerAttackState::Execute(float elapsedTime)
{
	if (owner->GetComponent<Player>()->InputAttack())
	{
		button = true;
	}
	// もし終わったら待機に変更
	if (owner->GetComponent<Player>()->GetUpdateAnim() == UpAnim::Doble && !owner->GetComponent<ModelControll>()->GetModel()->IsPlayUpeerBodyAnimation())
	{
		owner->GetComponent<Player>()->SetAttackCollisionFlag(false);
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(button ? Player::State::Attack : Player::State::Move));
		button = false;
		owner->GetComponent<Player>()->SetSpecialAttackTime(false);
		//attackCollisionFlag = false;
		//TransitionMoveState();


	}
	// 部分再生
	else if (owner->GetComponent<Player>()->GetUpdateAnim() == UpAnim::Normal && !owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		owner->GetComponent<Player>()->SetAttackCollisionFlag(false);
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
		owner->GetComponent<Player>()->SetSpecialAttackTime(false);
	}
	// もし移動中に手を離したら
	if (owner->GetComponent<Player>()->GetUpdateAnim() == UpAnim::Doble && !owner->GetComponent<Player>()->InputMove(elapsedTime))
	{
		//updateanim = UpAnim::Doble;
		owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Doble);

		bool loop = false;

			owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
					owner->GetComponent<Player>()->Anim_Attack, loop);
	}

	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentAnimationSecondsUpeer();
	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。
	bool CollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;

	owner->GetComponent<Player>()->SetAttackCollisionFlag(CollisionFlag);

	//if (updateanim == UpAnim::Doble)
	//{
	//	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	//	float animationTime = model->GetCurrentAnimationSecondsUpeer();
	//	// 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。
	//	attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
	//}
	if (owner->GetComponent<Player>()->GetAttackCollisionFlag())
	{
		// 左手ノードとエネミーの衝突処理
		owner->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius());
	}

	owner->GetComponent<Player>()->Ground();
}

void PlayerAttackState::Exit()
{
	
}

void PlayerDamageState::Enter()
{
	bool loop = false;

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_GetHit1, loop);

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
	bool loop = false;

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Death, loop);

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
	bool loop = false;

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Jump_Flip, loop);

	// 当たり判定の有無
	owner->GetComponent<Player>()->DmageInvalidJudment(false);


}

// 回避更新
void PlayerAvoidanceState::Execute(float elapsedTime)
{
	// 移動入力処理
	owner->GetComponent<Player>()->InputMove(elapsedTime);

	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		// 当たり判定の有無
		owner->GetComponent<Player>()->DmageInvalidJudment(true);
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));

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
		owner->GetComponent<Player>()->Anim_Attack, loop);

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
	bool CollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;

	//owner->GetComponent<Player>()->SetAttackCollisionFlag(CollisionFlag);

	if (CollisionFlag)
	{
		// 左手ノードとエネミーの衝突処理
		owner->GetComponent<Player>()->CollisionNodeVsEnemiesCounter("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius());
	}
}

// 反射終了
void PlayerReflectionState::Exit()
{
}


