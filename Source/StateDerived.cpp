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



// TODO 02_03 State�����N���X�Ƃ��Ċe��State�N���X��p�ӂ���B
// Wander�̓T���v���Ƃ��Ă��łɋL�q�ς�
// �e��Enter�֐��̓��e�͊eTransition����State�֐����Q�l��
// �e��Execute�֐��̓��e�͊eUpdate����State�֐����Q�l��

// �p�j�X�e�[�g�ɓ��������̃��\�b�h
void WanderState::Enter()
{
	//owner->GetActor()->GetComponent<EnemySlime>()->
	// �꒣��
	owner->GetComponent<EnemySlime>()->SetRandomTargetPosition();

	// �A�j���[�V�����Đ�
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Attack, false);
	//owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetComponent<EnemySlime>()->GetAnimationStateWalk(), true);
	// �A�j���[�V�������[��
	owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Reverseplayback);

	stateTimer = Mathf::RandomRange(6.0f, 8.0f);

	// ���n�u��
	upOnLading = false;

}

// �p�j�X�e�[�g�Ŏ��s���郁�\�b�h
void WanderState::Execute(float elapsedTime)
{
	stateTimer -= elapsedTime;
	// �v���C���[id
	std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
	// �ڕW�n�_�����v���C���[�ʒu�ɐݒ�
	DirectX::XMFLOAT3 targetPosition = playerid->GetComponent<Transform>()->GetPosition();

	owner->GetComponent<EnemySlime>()->SetTargetPosition(targetPosition);
	
	// �ړI�n�_�܂ł�XZ���ʂł̋�������
	float vx = targetPosition.x - owner->GetComponent<EnemySlime>()->GetPosition().x;
	float vz = targetPosition.z - owner->GetComponent<EnemySlime>()->GetPosition().z;
	float distSq = vx * vx + vz * vz;

	// �ړI�n�֒�����
	if (stateTimer < 0.0f)
	{
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::IdleBattle));
	}
	// �ړI�n�ɂ��̔��a��������
	float radius = owner->GetComponent<EnemySlime>()->GetAttackRightFootRange();



	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	if (animationTime >= 0.6f && animationTime <= 0.7f)
	{
		// �A�j���[�V�������[�� ��~
		owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Stop);
	
		if (distSq > radius * radius)
			// �ړI�n�_�ֈړ�
			owner->GetComponent<EnemySlime>()->MoveToTarget(elapsedTime, 0.8f);


		// �W�����v
		owner->GetComponent<EnemySlime>()->InputJump();

		// ���n�ŏՌ��g���΂�
		if (owner->GetComponent<Movement>()->GetOnLadius() && !upOnLading)
		{

			Model::Node* node = owner->GetComponent<ModelControll>()->GetModel()->FindNode("boss_right_foot1");
			DirectX::XMFLOAT3 pos(
				node->worldTransform._41,
				node->worldTransform._42,
				node->worldTransform._43
			);
			owner->GetComponent<EnemySlime>()->InputImpact(pos);
			// ���n�u��
			upOnLading = true;
		}
		if (!owner->GetComponent<Movement>()->GetOnLadius())
		{

			// ���n�u��
			upOnLading = false;
		}
	}

	//// �v���C���[���G
	//if (owner->GetComponent<EnemySlime>()->SearchPlayer())
	//{
	//	// ����������ǐՃX�e�[�g�֑J��
	//	// ChangeState�N���X��State��؂�ւ���
	//	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	//}

}

// �p�j�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void WanderState::Exit()
{

}

// ������
void IdleState::Enter()
{
	owner->GetComponent<EnemySlime>()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Anim_Standby, true);
	//owner->GetComponent<EnemySlime>()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetComponent<EnemySlime>()->GetAnimationStateNormal(), true);
	
	// �A�j���[�V�������[��
	owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Normal);
	
	// �^�C�}�[�������_���ݒ�
	//stateTimer = Mathf::RandomRange(3.0f, 5.0f);
	//owner->GetComponent<EnemySlime>()->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));

}

// update
void IdleState::Execute(float elapsedTime)
{
	// �^�C�}�[����
     //stateTimer -= elapsedTime;
	/* owner->SetStateTimer(stateTimer);*/
	//owner->GetComponent<EnemySlime>()->SetStateTimer(owner->GetComponent<EnemySlime>()->GetStateTimer() - elapsedTime);
	// TODO 03 
	// �ҋ@���Ԃ��o�߂����Ƃ��p�j�X�e�[�g�֑J�ڂ��Ȃ���
	//if (owner->GetComponent<EnemySlime>()->GetStateTimer() < 0.0f)
	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	// TODO 03 
	// �v���C���[�����������Ƃ��ǐՃX�e�[�g�֑J�ڂ��Ȃ���
	//if (owner->GetComponent<EnemySlime>()->SearchPlayer())
	//	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
}

// �Ō�ɂ�肽�������S��
void IdleState::Exit()
{
}
// ������
void PursuitState::Enter()
{
	
	owner->GetComponent<EnemySlime>()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Standby, true);
	//owner->GetComponent<EnemySlime>()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Walk, true);
	//owner->GetComponent<EnemySlime>()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetComponent<EnemySlime>()->GetAnimationStateWalk(), true);

	// �A�j���[�V�������[��
	owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Normal);

	// ���b�ԒǐՂ���^�C�}�[�������_���ݒ�
	stateTimer = Mathf::RandomRange(3.0f, 5.0f);

	attackRange = owner->GetComponent<EnemySlime>()->GetAttackRightFootRange();

	//// ���C�t
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
	// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
	//owner->SetTargetPosition(Player::Instance().GetPosition());
	//DirectX::XMFLOAT3 targetPosition = ;
		// �v���C���[id
	std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
	// �ڕW�n�_�����v���C���[�ʒu�ɐݒ�
	DirectX::XMFLOAT3 targetPosition = playerid->GetComponent<Transform>()->GetPosition();


	owner->GetComponent<EnemySlime>()->SetTargetPosition(targetPosition);

	// �ړI�n�_�ֈړ�
	//owner->GetComponent<EnemySlime>()->MoveToTarget(elapsedTime, 1.0);


	// �^�C�}�[����
	stateTimer -= elapsedTime;
	/*owner->SetStateTimer(stateTimer);*/



	// TODO 03 
	// �ǐՎ��Ԃ��o�߂����Ƃ��������X�e�[�g�֑J�ڂ��Ȃ���
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
	// �U���͈͂ɓ������Ƃ��U���X�e�[�g�֑J�ڂ��Ȃ���
	if (dist < attackRange)
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Attack));


	switch (attackRound)
	{
	case AttackChange::Round1:
	{
		if (stateTimer < 0.0f)
			owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));
		//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::ShotThrowing));


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

	// HP�c�@�O
	case AttackChange::ROund3:
	{

		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::ShotThrowing));
		//if (stateTimer < 0.0f && attackType == 0)
		//	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));

		//if (stateTimer < 0.0f && attackType == 1)
		//	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::ShotThrowing));

		//if (stateTimer < 0.0f && attackType == 2)
		//	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Wander));

		break;
	}


	// HP�c�@�@�Q�ƂP
	default:
	{


		break;
	}
	}


	//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));

}
// �Ō�ɂ�肽�������S��
void PursuitState::Exit()
{
}
// ������
void AttackState::Enter()
{

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Attack, false);
	//owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetComponent<EnemySlime>()->GetAnimationStateAttack(), false);

	// �A�j���[�V�������[��
	owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Normal);

		// ���C�t
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


	// �U�����[�V�������I�������Ƃ��ǐՃX�e�[�g�ֈڍs
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation() && attackMemory < attackMemoryMax)
	{
		//owner->GetComponent<EnemySlime>()->SetCounterJudgment(false);
		//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Attack));
		//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
		// �A�j���Đ�
		float currentAnimationSeconds = 0.8f;
		owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Attack, false, currentAnimationSeconds);

		
		++attackMemory;
	}
	// �J��Ԃ����݂��Ō�
	else if (attackMemory == attackMemoryMax)
	{
		//owner->GetComponent<EnemySlime>()->SetCounterJudgment(false);
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::IdleBattle));
		
		int attackMemoryStart = 0;
		attackMemory = attackMemoryStart;
	}

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	if (animationTime >= 1.0f && animationTime <= 2.0f)
	{
		// �����m�[�h�ƃv���C���[�̏Փˏ���
		owner->GetComponent<EnemySlime>()->CollisitionNodeVsPlayer("boss_left_foot1", 2);
	}
	// �p���B
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
// �Ō�ɂ�肽�������S��
void AttackState::Exit()
{
}

// �����ݒ�
void AttackShotState::Enter()
{
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Shot, false);

	// �A�j���[�V�������[��
	owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Normal);

	// ���C�t
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
// �X�V����
void AttackShotState::Execute(float elapsedTime)
{

	// �U�����[�V�������I�������Ƃ��ǐՃX�e�[�g�ֈڍs
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		//owner->GetComponent<EnemySlime>()->SetCounterJudgment(false);
		//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));
		//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
		// �A�j���Đ�
		float currentAnimationSeconds = 0.1f;
		owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Shot, false, currentAnimationSeconds);
		++attackCount;


	}
	if (attackCount >= attackCountMax)
	{
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
		attackCount = 0;
	}

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// efekt�ݒ�
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
		//	// �����m�[�h�ƃv���C���[�̏Փˏ���
		owner->GetComponent<EnemySlime>()->InputProjectile();

	}
	//// �e�ۏo��
	//if (animationTime >= 0.7f && animationTime <= 0.8f)
	//{

	//}
	// �e�۔���
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
// �I������
void AttackShotState::Exit()
{
}

// ��������
void AttackShotThrowingState::Enter()
{
	owner->GetComponent<ModelControll>()->GetModel()->PlayReverseAnimation(EnemySlime::Animation::Anim_jewelattack, false);
	// �A�j���[�V�������[��
	owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Reverseplayback);

	// ������
	turnPermission = false;

}
// �X�V����
void AttackShotThrowingState::Execute(float elapsedTime)
{

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// ��Ύ���Đݒ�
	if (animationTime <= 3.9f && animationTime >= 3.89f && !turnPermission)
	{
		// �v���C���[id
		std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
		

		//	�������U���o�^
		owner->GetComponent<EnemySlime>()->InputThrowingRuby(playerid->GetComponent<Transform>()->GetPosition());

		

	}
	// ��]
	if (animationTime <= 3.6f && animationTime >= 3.0f && !turnPermission)
	{
		// �v���C���[id
		std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);

		owner->GetComponent<EnemySlime>()->SetTargetPosition(playerid->GetComponent<Transform>()->GetPosition());
		owner->GetComponent<EnemySlime>()->TurnToTarget(elapsedTime, turnSpeed);
		//owner->GetComponent<Transform>()->SetAngle(playerid->GetComponent<Transform>()->GetAngle());

		DirectX::XMFLOAT3 direction;
		DirectX::XMFLOAT3 angle = owner->GetComponent<Transform>()->GetAngle();

		direction.x = sinf(angle.y);// �O�p���΂߂ɂ��Ĉʒu��ς���
		direction.y = cosf(angle.x);
		direction.z = cosf(angle.y);

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
				projectileManager->GetComponent<Transform>()->SetDirection(direction);
				projectileManager->GetComponent<Transform>()->SetAngle(angle);
			}
		}

	}
	// �\��
	if (animationTime >= 3.89f && !turnPermission)
	{

		DirectX::XMFLOAT3 direction;
		DirectX::XMFLOAT3 angle = owner->GetComponent<Transform>()->GetAngle();

		// �v���C���[id
		std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);


		direction.x = sinf(angle.y);// �O�p���΂߂ɂ��Ĉʒu��ς���
		direction.y = cosf(angle.x);
		direction.z = cosf(angle.y);
		for (int i = 0; i < ProjectileManager::Instance().GetProjectileCount(); ++i)
		{

			std::shared_ptr<Actor> projectileManager = ProjectileManager::Instance().GetProjectile(i);
			if (projectileManager->GetComponent<ProjectileThrowing>())
			{
				projectileManager->GetComponent<Transform>()->SetPosition(owner->GetComponent<Transform>()->GetPosition());
				//projectileManager->GetComponent<Transform>()->SetAngle(owner->GetComponent<Transform>()->GetAngle());

				projectileManager->GetComponent<Transform>()->SetDirection(direction);
			}
		}
	}

	// �A�j���[�V�����ˌ�����
	if (animationTime <= 0.8f && !turnPermission)
	{
		bool end = true;
		owner->GetComponent<ModelControll>()->GetModel()->SetAnimationEndFlag(end);
		bool blend = false;

		owner->GetComponent<ModelControll>()->GetModel()->ReverseplaybackAnimation(elapsedTime, blend);
		// �A�j���[�V�����Đ�����
		float currentAnimationSeconds = 1.0f;

		owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Shot, false, currentAnimationSeconds);
		// �A�j���[�V�������[��
		owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Normal);


		turnPermission = true;
	}
	// �ˌ�����
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation() && turnPermission)
	{
		bool movementCheck = true;
		for (int i = 0; i < ProjectileManager::Instance().GetProjectileCount(); ++i)
		{
			std::shared_ptr<Actor> projectileManager = ProjectileManager::Instance().GetProjectile(i);
			if (projectileManager->GetComponent<ProjectileThrowing>())
				projectileManager->GetComponent<ProjectileThrowing>()->SetMovementCheck(movementCheck);
		}
		
		// �I���A�j���[�V����
		//owner->GetComponent<ModelControll>()->GetModel()->ReverseplaybackAnimation(elapsedTime, blend);
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	}

	//// �U�����[�V�������I�������Ƃ��ǐՃX�e�[�g�ֈڍs
	//if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation() && turnPermission)
	//{
	//	//owner->GetComponent<EnemySlime>()->SetCounterJudgment(false);
	//	//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));
	//	// animation�ύX
	//	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));

	//}



}
// �I������
void AttackShotThrowingState::Exit()
{
}

void DamageState::Enter()
{
	bool loop = false;

	owner->GetComponent<ModelControll>()->GetModel()->
		PlayAnimation(
			EnemySlime::Animation::Anim_Die, loop);
	// �A�j���[�V�������[��
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

// �����J�n
void ConfusionState::Enter()
{
	bool loop = true;

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_CloseEye, loop);
	// �A�j���[�V�������[��
	owner->GetComponent<EnemySlime>()->SetUpdateAnim(EnemySlime::UpAnim::Normal);

	// �X�e�[�g�ҋ@����
	stateTimer = Mathf::RandomRange(6.0f, 8.0f);

}
// �����X�V
void ConfusionState::Execute(float elapsedTime)
{
	stateTimer -= elapsedTime;

	if (stateTimer < 0.0f)
	{
		// �X�e�[�g�ύX
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Idle));
	}


}
// �����I��
void ConfusionState::Exit()
{

}

// �v���C���[
void PlayerIdleState::Enter()
{
	bool loop = true;
	//owner->GetComponent<Player>()
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Idle, loop
	);
	
	// �A�j���[�V�������[��
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);
	
}

void PlayerIdleState::Execute(float elapsedTime)
{
	

	// �ړ����͏���
	if (owner->GetComponent<Player>()->InputMove(elapsedTime))
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
		//TransitionMoveState();
	}

	// ���˓��͏���
	if (owner->GetComponent<Player>()->InputAvoidance())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Reflection));

	}


	// �W�����v���͏���
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

	//// �e�ۓ��͏���
	//// ��
	//if (owner->GetComponent<Player>()->GetSelectMagicCheck() == (int)Player::CommandMagic::Fire && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Magic)
	//{
	//	owner->GetComponent<Player>()->InputMagicframe();
	//	//TransitionAttackState();
	//}
	//// ��
	//if (owner->GetComponent<Player>()->GetSelectMagicCheck() == (int)Player::CommandMagic::Thander && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Magic)
	//{
	//	owner->GetComponent<Player>()->InputMagicLightning();
	//	//TransitionAttackState();
	//}
	//// �X
	//if (owner->GetComponent<Player>()->GetSelectMagicCheck() == (int)Player::CommandMagic::Ice && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Magic)
	//{
	//	owner->GetComponent<Player>()->InputMagicIce();
	//	//TransitionAttackState();
	//}


	//// ����U��
	//if (owner->GetComponent<Player>()->InputSpecialAttackCharge())
	//{
	//	//TransitionAttackState();
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
	//}
	//// ���ꖂ�@
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

	// �A�j���[�V�������[��
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Doble);
}

void PlayerMovestate::Execute(float elapsedTime)
{

	// �ړ����͏���
	if (!owner->GetComponent<Player>()->InputMove(elapsedTime))
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
		//TransitionIdleState();
	}


	// �����͏���
	if (owner->GetComponent<Player>()->InputAvoidance())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

	}

	// �W�����v���͏���
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

	//// �e�ۓ��͏���
	//// ��
	//if (owner->GetComponent<Player>()->GetSelectMagicCheck() == (int)Player::CommandMagic::Fire &&owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Magic)
	//{
	//	owner->GetComponent<Player>()->InputMagicframe();
	//	//TransitionAttackState();
	//}
	//// ��
	//if (owner->GetComponent<Player>()->GetSelectMagicCheck() == (int)Player::CommandMagic::Thander && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Magic)
	//{
	//	owner->GetComponent<Player>()->InputMagicLightning();
	//	//TransitionAttackState();
	//}
	//// �X
	//if (owner->GetComponent<Player>()->GetSelectMagicCheck() == (int)Player::CommandMagic::Ice && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Magic)
	//{
	//	owner->GetComponent<Player>()->InputMagicIce();
	//	//TransitionAttackState();
	//}



	//// ����U��
	//if (owner->GetComponent<Player>()->InputSpecialAttackCharge())
	//{
	//	//TransitionAttackState();
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
	//}
	//// ���ꖂ�@
	//if (owner->GetComponent<Player>()->InputSpecialShotCharge())
	//{
	//	owner->GetComponent<Player>()->InputProjectile();
	//}
	// �������n
	//owner->GetComponent<Player>()->Ground();


}

void PlayerMovestate::Exit()
{
}

void PlayerJumpState::Enter()
{
	loop = false;

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Jump, loop);

	// �A�j���[�V�������[��
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);

}

void PlayerJumpState::Execute(float elapsedTime)
{



	// �W�����v���͏���
	if (owner->GetComponent<Player>()->InputJump())
	{
		//TransitionJumpFlipState();
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
	}

	// �ړ�
	owner->GetComponent<Player>()->InputMove(elapsedTime);

	// ���
	if (owner->GetComponent<Player>()->InputAvoidance())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}

	//// ���˓��͏���
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

	//// �e�ۓ��͏���
	//// ��
	//if (owner->GetComponent<Player>()->GetSelectMagicCheck() == (int)Player::CommandMagic::Fire && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Magic)
	//{
	//	owner->GetComponent<Player>()->InputMagicframe();
	//	//TransitionAttackState();
	//}
	//// ��
	//if (owner->GetComponent<Player>()->GetSelectMagicCheck() == (int)Player::CommandMagic::Thander && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Magic)
	//{
	//	owner->GetComponent<Player>()->InputMagicLightning();
	//	//TransitionAttackState();
	//}
	//// �X
	//if (owner->GetComponent<Player>()->GetSelectMagicCheck() == (int)Player::CommandMagic::Ice && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::CommandAttack::Magic)
	//{
	//	owner->GetComponent<Player>()->InputMagicIce();
	//	//TransitionAttackState();
	//}


	//// ����U��
	//if (owner->GetComponent<Player>()->InputSpecialAttackCharge())
	//{
	//	//TransitionAttackState();
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
	//}
	//// ���ꖂ�@
	//if (owner->GetComponent<Player>()->InputSpecialShotCharge())
	//{
	//	owner->GetComponent<Player>()->InputProjectile();
	//}

	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
			owner->GetComponent<Player>()->Anim_Falling, loop);
	}

	if (owner->GetComponent<Movement>()->GetOnLadius())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Land));
	}

	//// �������n
	//if (owner->GetComponent<Player>()->Ground())
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Land));


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

	// �A�j���[�V�������[��
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);
}

void PlayerLandState::Execute(float elapsedTime)
{
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		owner->GetComponent<Movement>()->SetOnLadius(false);
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));


	}

	//// �ړ�
	//if (owner->GetComponent<Player>()->InputMove(elapsedTime) && owner->GetComponent<Player>()->InputAvoidance())
	//{
	//	// ���
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

	//}

	//// ���˓��͏���
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
	bool loop = false;

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Jump_Flip, loop);

	// �A�j���[�V�������[��
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);
}

void PlayerJumpFlipState::Execute(float elapsedTime)
{
	// �ړ�
	if (owner->GetComponent<Player>()->InputMove(elapsedTime))
		bool afterimagemove = true;
	// �W�����v���͏���
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{

		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
	}

	// ���
	if (owner->GetComponent<Player>()->InputAvoidance())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}


	//// �������n
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

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Attack, loop
	);

	// �A�j���[�V�������[��
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);


	std::shared_ptr<Player> playerId = owner->GetComponent<Player>();

	// ������̒�~
	bool stopFall = true;
	owner->GetComponent<Movement>()->SetStopFall(stopFall);

	// �ړ��̒�~
	bool stopMove = true;
	owner->GetComponent<Movement>()->SetStopMove(stopMove);

}

void PlayerAttackState::Execute(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	// �U������
	if (owner->GetComponent<Player>()->InputAttack()&&
		owner->GetComponent<Player>()->GetSelectCheck() == 
		(int)Player::CommandAttack::Attack)
	{
		button = true;
	}
	// �R�}���h�m�F
	std::vector<GamePadButton> command;
	command.push_back(GamePad::BTN_X);
	command.push_back(GamePad::BTN_X);
	command.push_back(GamePad::BTN_A);
	int frame = 60;
	// �R�}���h�m�F
	if (gamePad.ConfirmCommand(command, frame))
	{
		button = false;
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
	}

	
	//// �����Đ�
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
	// �����I�������ҋ@�ɕύX
	//if (owner->GetComponent<Player>()->GetUpdateAnim() == UpAnim::Normal && !owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		owner->GetComponent<Player>()->SetAttackCollisionFlag(false);
		// ���͊m�F�ŃX�e�[�g�ύX
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(button ? Player::State::Attack : Player::State::Move));

		// ������̍ĊJ
		bool stopFallFalse = false;
		bool stopFallTrue = true;
		owner->GetComponent<Movement>()->SetStopFall(button ? stopFallTrue : stopFallFalse);

		// �ړ��̒�~
		bool stopMoveFalse = false;
		bool stopMoveTrue = false;
		owner->GetComponent<Movement>()->SetStopMove(button ? stopMoveTrue : stopMoveFalse);


		button = false;
		//owner->GetComponent<Player>()->SetSpecialAttackTime(false);


		//owner->GetComponent<Player>()->SetAngleCheck(false);
	}
	//// �����ړ����Ɏ�𗣂�����
	//if (owner->GetComponent<Player>()->GetUpdateAnim() == UpAnim::Doble && !owner->GetComponent<Player>()->InputMove(elapsedTime))
	//{
	//	//updateanim = UpAnim::Doble;
	//	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Doble);

	//	bool loop = false;

	//		owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
	//				owner->GetComponent<Player>()->Anim_Attack, loop);
	//}

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B
	bool CollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;

	/*owner->GetComponent<Player>()->SetAttackCollisionFlag(CollisionFlag);*/

	//if (updateanim == UpAnim::Doble)
	//{
	//	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	//	float animationTime = model->GetCurrentAnimationSecondsUpeer();
	//	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B
	//	attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
	//}
	if (CollisionFlag)
	{
		// ����m�[�h�ƃG�l�~�[�̏Փˏ���
		owner->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius());
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

// ���������
void PlayerAvoidanceState::Enter()
{
	bool loop = false;

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Attack, loop);

	// �����蔻��̗L��
	owner->GetComponent<Player>()->DmageInvalidJudment(false);

	moveSpeed = 10.0f;

	// �A�j���[�V������� �ʏ�
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);

	// ������̒�~
	bool stopFall = true;
	owner->GetComponent<Movement>()->SetStopFall(stopFall);

}

// ����X�V
void PlayerAvoidanceState::Execute(float elapsedTime)
{
	// �ړ����͏���
	//owner->GetComponent<Player>()->InputMove(elapsedTime);

	// �]����_�b�V��
	DirectX::XMFLOAT3 dir;
	dir.x = sinf(owner->GetComponent<Transform>()->GetAngle().y);
	dir.y = cosf(owner->GetComponent<Transform>()->GetAngle().x);
	dir.z = cosf(owner->GetComponent<Transform>()->GetAngle().y);

	
	// �C�ӂ̃A�j���[�V�����Đ����
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// ���̎��Ԃ�
	if (animationTime >= 0.2f )
	{
		owner->GetComponent<Movement>()->Move(dir, moveSpeed, elapsedTime);
	}
	// �A�j���[�V�����I��
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{

		// �����蔻��̗L��
		owner->GetComponent<Player>()->DmageInvalidJudment(true);
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


		// ������̒�~
		bool stopFall = false;
		owner->GetComponent<Movement>()->SetStopFall(stopFall);
	}
}

// ����I��
void PlayerAvoidanceState::Exit()
{

}

// ���ˊJ�n
void PlayerReflectionState::Enter()
{
	bool loop = false;

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Attack, loop);

	// �A�j���[�V������� �ʏ�
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);


	// �����蔻��̗L��
	//owner->GetComponent<Player>()->DmageInvalidJudment(false);

}

// ���ˍX�V
void PlayerReflectionState::Execute(float elapsedTime)
{
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		// �����蔻��̗L��
		//owner->GetComponent<Player>()->DmageInvalidJudment(true);
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));

	}


	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B
	bool CollisionFlag = animationTime >= 0.3f && animationTime <= 1.0f;

	//owner->GetComponent<Player>()->SetAttackCollisionFlag(CollisionFlag);

	if (CollisionFlag)
	{
		// ����m�[�h�ƃG�l�~�[�̏Փˏ���
		owner->GetComponent<Player>()->CollisionNodeVsEnemiesCounter("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius());
		owner->GetComponent<Player>()->CollisionNodeVsRubyCounter("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius());
		owner->GetComponent<Player>()->CollisionNodeVsRubyCounterBulletFring("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius());
		
	}
}

// ���ˏI��
void PlayerReflectionState::Exit()
{
}


