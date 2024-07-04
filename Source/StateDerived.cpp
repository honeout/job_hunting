#include "EnemySlime.h"
#include "StateDerived.h"
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

	owner->SetRandomTargetPosition();
	owner->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetAnimationStateWalk(), true);
}

// 徘徊ステートで実行するメソッド
void WanderState::Execute(float elapsedTime)
{
	// 目的地点までのXZ平面での距離判定
	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
	float distSq = vx * vx + vz * vz;

	// 目的地へ着いた
	float radius = owner->GetRadius();
	if (distSq < radius * radius)
	{
		// 待機ステートへ遷移
		// ChangeStateクラスでStateを切り替える
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Idle));
	}
	// 目的地点へ移動
	owner->MoveToTarget(elapsedTime, 0.5f);
	// プレイヤー索敵
	if (owner->SearchPlayer())
	{
		// 見つかったら追跡ステートへ遷移
		// ChangeStateクラスでStateを切り替える
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	}

}

// 徘徊ステートから出ていくときのメソッド
void WanderState::Exit()
{

}

// 初期化
void IdleState::Enter()
{
	owner->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetAnimationStateNormal(), true);
	// タイマーをランダム設定
	//stateTimer = Mathf::RandomRange(3.0f, 5.0f);
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));

}

// update
void IdleState::Execute(float elapsedTime)
{
	// タイマー処理
     //stateTimer -= elapsedTime;
	/* owner->SetStateTimer(stateTimer);*/
	 owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);
	// TODO 03 
	// 待機時間が経過したとき徘徊ステートへ遷移しなさい
	if (owner->GetStateTimer() < 0.0f)
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Wander));
	// TODO 03 
	// プレイヤーが見つかったとき追跡ステートへ遷移しなさい
	if (owner->SearchPlayer())
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
}

// 最後にやりたい処理全般
void IdleState::Exit()
{
}
// 初期化
void PursuitState::Enter()
{
	
	owner->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetAnimationStateWalk(), true);

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


	owner->SetTargetPosition(targetPosition);

	// 目的地点へ移動
	owner->MoveToTarget(elapsedTime, 1.0);

	
	// タイマー処理
	stateTimer -= elapsedTime;
	/*owner->SetStateTimer(stateTimer);*/

	// TODO 03 
	// 追跡時間が経過したとき待機ステートへ遷移しなさい
	if (stateTimer < 0.0f)
	owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Idle));


	float vx = targetPosition.x - owner->GetPosition().x;
	float vy = targetPosition.y - owner->GetPosition().y;
	float vz = targetPosition.z - owner->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	// TODO 03 
	// 攻撃範囲に入ったとき攻撃ステートへ遷移しなさい
	if (dist < attackRange)
	owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Attack));

}
// 最後にやりたい処理全般
void PursuitState::Exit()
{
}
// 初期化
void AttackState::Enter()
{

	owner->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetAnimationStateAttack(), false);
}
// update
void AttackState::Execute(float elapsedTime)
{
	// 攻撃モーションが終了したとき追跡ステートへ移行
	if (!owner->GetActor()->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));


	// 任意のアニメーション再生区間でのみ衝突判定処理をする
	float animationTime = owner->GetActor()->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	if (animationTime >= 0.2f && animationTime <= 0.35f)
	{
		// 目玉ノードとプレイヤーの衝突処理
		owner->CollisitionNodeVsPlayer("EyeBall", 0.2f);
	}
}
// 最後にやりたい処理全般
void AttackState::Exit()
{
}
