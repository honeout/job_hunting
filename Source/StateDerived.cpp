#include "EnemySlime.h"
#include "StateDerived.h"
#include "Player.h"
#include "Mathf.h"


// TODO 02_03 State�����N���X�Ƃ��Ċe��State�N���X��p�ӂ���B
// Wander�̓T���v���Ƃ��Ă��łɋL�q�ς�
// �e��Enter�֐��̓��e�͊eTransition����State�֐����Q�l��
// �e��Execute�֐��̓��e�͊eUpdate����State�֐����Q�l��

// �p�j�X�e�[�g�ɓ��������̃��\�b�h
void WanderState::Enter()
{
	//owner->GetActor()->GetComponent<EnemySlime>()->

	owner->SetRandomTargetPosition();
	owner->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetAnimationStateWalk(), true);
}

// �p�j�X�e�[�g�Ŏ��s���郁�\�b�h
void WanderState::Execute(float elapsedTime)
{
	// �ړI�n�_�܂ł�XZ���ʂł̋�������
	float vx = owner->GetTargetPosition().x - owner->GetPosition().x;
	float vz = owner->GetTargetPosition().z - owner->GetPosition().z;
	float distSq = vx * vx + vz * vz;

	// �ړI�n�֒�����
	float radius = owner->GetRadius();
	if (distSq < radius * radius)
	{
		// �ҋ@�X�e�[�g�֑J��
		// ChangeState�N���X��State��؂�ւ���
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Idle));
	}
	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsedTime, 0.5f);
	// �v���C���[���G
	if (owner->SearchPlayer())
	{
		// ����������ǐՃX�e�[�g�֑J��
		// ChangeState�N���X��State��؂�ւ���
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	}

}

// �p�j�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void WanderState::Exit()
{

}

// ������
void IdleState::Enter()
{
	owner->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetAnimationStateNormal(), true);
	// �^�C�}�[�������_���ݒ�
	//stateTimer = Mathf::RandomRange(3.0f, 5.0f);
	owner->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));

}

// update
void IdleState::Execute(float elapsedTime)
{
	// �^�C�}�[����
     //stateTimer -= elapsedTime;
	/* owner->SetStateTimer(stateTimer);*/
	 owner->SetStateTimer(owner->GetStateTimer() - elapsedTime);
	// TODO 03 
	// �ҋ@���Ԃ��o�߂����Ƃ��p�j�X�e�[�g�֑J�ڂ��Ȃ���
	if (owner->GetStateTimer() < 0.0f)
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Wander));
	// TODO 03 
	// �v���C���[�����������Ƃ��ǐՃX�e�[�g�֑J�ڂ��Ȃ���
	if (owner->SearchPlayer())
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
}

// �Ō�ɂ�肽�������S��
void IdleState::Exit()
{
}
// ������
void PursuitState::Enter()
{
	
	owner->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetAnimationStateWalk(), true);

	// ���b�ԒǐՂ���^�C�}�[�������_���ݒ�
	stateTimer = Mathf::RandomRange(3.0f, 5.0f);
}
// update
void PursuitState::Execute(float elapsedTime)
{
	// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
	//owner->SetTargetPosition(Player::Instance().GetPosition());
	//DirectX::XMFLOAT3 targetPosition = ;
		// �v���C���[id
	std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
	// �ڕW�n�_�����v���C���[�ʒu�ɐݒ�
	DirectX::XMFLOAT3 targetPosition = playerid->GetComponent<Transform>()->GetPosition();


	owner->SetTargetPosition(targetPosition);

	// �ړI�n�_�ֈړ�
	owner->MoveToTarget(elapsedTime, 1.0);

	
	// �^�C�}�[����
	stateTimer -= elapsedTime;
	/*owner->SetStateTimer(stateTimer);*/

	// TODO 03 
	// �ǐՎ��Ԃ��o�߂����Ƃ��ҋ@�X�e�[�g�֑J�ڂ��Ȃ���
	if (stateTimer < 0.0f)
	owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Idle));


	float vx = targetPosition.x - owner->GetPosition().x;
	float vy = targetPosition.y - owner->GetPosition().y;
	float vz = targetPosition.z - owner->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	// TODO 03 
	// �U���͈͂ɓ������Ƃ��U���X�e�[�g�֑J�ڂ��Ȃ���
	if (dist < attackRange)
	owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Attack));

}
// �Ō�ɂ�肽�������S��
void PursuitState::Exit()
{
}
// ������
void AttackState::Enter()
{

	owner->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetAnimationStateAttack(), false);
}
// update
void AttackState::Execute(float elapsedTime)
{
	// �U�����[�V�������I�������Ƃ��ǐՃX�e�[�g�ֈڍs
	if (!owner->GetActor()->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
		owner->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));


	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner->GetActor()->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	if (animationTime >= 0.2f && animationTime <= 0.35f)
	{
		// �ڋʃm�[�h�ƃv���C���[�̏Փˏ���
		owner->CollisitionNodeVsPlayer("EyeBall", 0.2f);
	}
}
// �Ō�ɂ�肽�������S��
void AttackState::Exit()
{
}
