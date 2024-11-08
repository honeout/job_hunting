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



// TODO 02_03 State�����N���X�Ƃ��Ċe��State�N���X��p�ӂ���B
// Wander�̓T���v���Ƃ��Ă��łɋL�q�ς�
// �e��Enter�֐��̓��e�͊eTransition����State�֐����Q�l��
// �e��Execute�֐��̓��e�͊eUpdate����State�֐����Q�l��

// �p�j�X�e�[�g�ɓ��������̃��\�b�h
void WanderState::Enter()
{
	
	enemyid = owner->GetComponent<EnemySlime>();


	moveid = owner->GetComponent<Movement>();
	// �꒣��
	enemyid->SetRandomTargetPosition();

	// �A�j���[�V�����Đ�
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Attack, loop, currentAnimationStartSeconds, blendSeconds);
	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Reverseplayback);

	stateTimer = Mathf::RandomRange(6.0f, 8.0f);

	// ���n�u��
	upOnLading = false;

}

// �p�j�X�e�[�g�Ŏ��s���郁�\�b�h
void WanderState::Execute(float elapsedTime)
{
	stateTimer -= elapsedTime;
	// �v���C���[id

	// �ڕW�n�_�����v���C���[�ʒu�ɐݒ�
	DirectX::XMFLOAT3 targetPosition = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();

	enemyid->SetTargetPosition(targetPosition);
	
	// �ړI�n�_�܂ł�XZ���ʂł̋�������
	float vx = targetPosition.x - enemyid->GetPosition().x;
	float vz = targetPosition.z - enemyid->GetPosition().z;
	float distSq = vx * vx + vz * vz;

	// �ړI�n�֒�����
	if (stateTimer < 0.0f)
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::IdleBattle));
	}
	// �ړI�n�ɂ��̔��a��������
	float radius = enemyid->GetAttackRightFootRange();



	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	if (animationTime >= 0.6f && animationTime <= 0.7f)
	{
		// �A�j���[�V�������[�� ��~
		enemyid->SetUpdateAnim(EnemySlime::UpAnim::Stop);
	
		if (distSq > radius * radius)
			// �ړI�n�_�ֈړ�
			enemyid->MoveToTarget(elapsedTime, 0.8f);


		// �W�����v
		enemyid->InputJump();

		// ���n�ŏՌ��g���΂�
		if (moveid->GetOnLadius() && !upOnLading)
		{

			Model::Node* node = owner->GetComponent<ModelControll>()->GetModel()->FindNode("boss_right_foot1");
			DirectX::XMFLOAT3 pos(
				node->worldTransform._41,
				node->worldTransform._42,
				node->worldTransform._43
			);
			enemyid->InputImpact(pos);
			// ���n�u��
			upOnLading = true;
		}
		if (!moveid->GetOnLadius())
		{

			// ���n�u��
			upOnLading = false;
		}
	}


}

// �p�j�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void WanderState::Exit()
{

}

// ������
void IdleState::Enter()
{

	enemyid = owner->GetComponent<EnemySlime>();
	//modelControllid = owner->GetComponent<ModelControll>();
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Anim_Standby, loop, currentAnimationStartSeconds,blendSeconds);
	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);

}

// update
void IdleState::Execute(float elapsedTime)
{

	// TODO 03 
	// �ҋ@���Ԃ��o�߂����Ƃ��p�j�X�e�[�g�֑J�ڂ��Ȃ���
	enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
}

// �Ō�ɂ�肽�������S��
void IdleState::Exit()
{
}
// ������
void PursuitState::Enter()
{
	// �v���C���[�R���|�[�l���g�A�N�Z�X
	//playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Player>();

	--mortionLimit;
	// �G�l�~�[�A�N�Z�X�p
	enemyid = owner->GetComponent<EnemySlime>();
	enemyid->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Standby, loop,currentAnimationStartSeconds,  blendSeconds);

	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);

	// ���b�ԒǐՂ���^�C�}�[�������_���ݒ�
	stateTimer = Mathf::RandomRange(3.0f, 5.0f);

	attackRange = enemyid->GetAttackRightFootRange();

	hpid = owner->GetComponent<HP>();

	// �G���C�t
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
	// �ڕW�n�_���v���C���[�ʒu�ɐݒ�
		// �v���C���[id
	
	// �ڕW�n�_�����v���C���[�ʒu�ɐݒ�
	DirectX::XMFLOAT3 targetPosition = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();


	enemyid->SetTargetPosition(targetPosition);

	if (mortionLimit < 0)
	{
		// �ҋ@���ԍĐݒ�
		//mortionLimit = Mathf::RandomRange(3.0f, mortionLimitMax);
		mortionLimit = rand() % mortionLimitMax + 3;
		// ���E�������̂ŋx�e
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::IdleBattle));
	}

	// �^�C�}�[����
	stateTimer -= elapsedTime;


	float vx = targetPosition.x - enemyid->GetPosition().x;
	float vy = targetPosition.y - enemyid->GetPosition().y;
	float vz = targetPosition.z - enemyid->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	// TODO 03 
	// �U���͈͂ɓ������Ƃ��U���X�e�[�g�֑J�ڂ��Ȃ���
	if (dist < attackRange)
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Attack));


	switch (attackRound)
	{
	case AttackChange::Round1:
	{
		// ���Ԃ��߂����̂�
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

	// HP�c�@�O
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


	// HP�c�@�@�Q�ƂP
	default:
	{


		break;
	}
	}

}
// �Ō�ɂ�肽�������S��
void PursuitState::Exit()
{
}
// ������
void AttackState::Enter()
{

	enemyid = owner->GetComponent<EnemySlime>();

	hpid = owner->GetComponent<HP>();

	currentAnimationStartSeconds = 0.0f;
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Attack, loop, currentAnimationStartSeconds, blendSeconds);
	

	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);

		// ���C�t
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


	// �U�����[�V�������I�������Ƃ��ǐՃX�e�[�g�ֈڍs
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation() && attackMemory < attackMemoryMax)
	{
		// �A�j���Đ�
		currentAnimationStartSeconds = 0.8f;
		owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Attack, loop, currentAnimationStartSeconds, blendSeconds);

		
		++attackMemory;
	}
	// �J��Ԃ����݂��Ō�
	else if (attackMemory == attackMemoryMax)
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
		
		int attackMemoryStart = 0;
		attackMemory = attackMemoryStart;
	}

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	if (animationTime >= 1.0f && animationTime <= 2.0f)
	{
		// �����m�[�h�ƃv���C���[�̏Փˏ���
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
// �Ō�ɂ�肽�������S��
void AttackState::Exit()
{
}

// �����ݒ�
void AttackShotState::Enter()
{
	enemyid = owner->GetComponent<EnemySlime>();
	//modelid = owner->GetComponent<ModelControll>();
	hpid = owner->GetComponent<HP>();
	

	currentAnimationStartSeconds = 0.0f;
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Shot, loop, currentAnimationStartSeconds, blendSeconds);

	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);

	// ���C�t
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
// �X�V����
void AttackShotState::Execute(float elapsedTime)
{

	// �U�����[�V�������I�������Ƃ��ǐՃX�e�[�g�ֈڍs
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		// �A�j���Đ�
		currentAnimationStartSeconds = 0.1f;
		owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Shot, loop, currentAnimationStartSeconds ,blendSeconds);
		++attackCount;


	}
	if (attackCount >= attackCountMax)
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
		attackCount = 0;
	}

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// efekt�ݒ�
	if (animationTime >= 0.1f && animationTime <= 0.11f)
	{
		//	// �����m�[�h�ƃv���C���[�̏Փˏ���
		enemyid->InputProjectile();

	}

	// �e�۔���
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
// �I������
void AttackShotState::Exit()
{
}

// ��������
void AttackShotThrowingState::Enter()
{
	enemyid = owner->GetComponent<EnemySlime>();
	transformid = owner->GetComponent<Transform>();

	currentAnimationStartSecondsf = 0.0f;
	owner->GetComponent<ModelControll>()->GetModel()->PlayReverseAnimation(EnemySlime::Animation::Anim_jewelattack, loop,currentAnimationStartSecondsf,blendSeconds);
	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Reverseplayback);

	// ������
	turnPermission = false;

	// �n�ʔ����x����
	bool pushuThrow = false;
	enemyid->SetPushuThrow(pushuThrow);

}
// �X�V����
void AttackShotThrowingState::Execute(float elapsedTime)
{
	// ����
	//DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 angle = transformid->GetAngle();
	// �v���C���[id
	std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&playerid->GetComponent<Transform>()->GetPosition());

	//for (int i = 0; i < ProjectileManager::Instance().GetProjectileCount(); ++i)
	//{

	//	std::shared_ptr<Actor> projectileManager = ProjectileManager::Instance().GetProjectile(i);
	//	if (projectileManager->GetComponent<ProjectileThrowing>())
	//	{
	//		// ����]
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
	



	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// ��Ύ���Đݒ�
	if (animationTime <= 3.9f && animationTime >= 3.89f && !turnPermission)
	{
		////// ���ʂ̌����x�N�g��
  //       direction.x = sinf(angle.y);// �O�p���΂߂ɂ��Ĉʒu��ς���
  //       direction.y = sinf(angle.x);
  //       //direction.y = DirectX::XMConvertToRadians(0);
  //       direction.z = cosf(angle.y);		

		 
		//	�������U���o�^
		enemyid->InputThrowingRuby(playerid->GetComponent<Transform>()->GetPosition());

		

	}
	// ��]
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
	// �\��
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

	// �A�j���[�V�����ˌ�����
	if (animationTime <= 0.8f && !turnPermission)
	{
		// �����A�j���[�V�����I��
		bool end = true;
		owner->GetComponent<ModelControll>()->GetModel()->SetAnimationEndFlag(end);
		//bool blend = false;

		owner->GetComponent<ModelControll>()->GetModel()->ReverseplaybackAnimation(elapsedTime);
		// �A�j���[�V�����Đ�����
		currentAnimationStartSecondsf = 0.7f;

		owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Shot, loop, currentAnimationStartSecondsf, blendSeconds);
		// �A�j���[�V�������[��
		enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);


		turnPermission = true;
	}
	// �ˌ�����
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation() && turnPermission)
	{
		// �n�ʔ��蔭��
		bool pushuThrow = true;
		enemyid->SetPushuThrow(pushuThrow);
		// ������
		bool movementCheck = true;
		for (int i = 0; i < ProjectileManager::Instance().GetProjectileCount(); ++i)
		{
			std::shared_ptr<Actor> projectileManager = ProjectileManager::Instance().GetProjectile(i);
			if (projectileManager->GetComponent<ProjectileThrowing>())
			{
				projectileManager->GetComponent<ProjectileThrowing>()->SetMovementCheck(movementCheck);
			}
		}
		
		// �I���A�j���[�V����
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	}




}
// �I������
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
	// �A�j���[�V�������[��
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

// �����J�n
void ConfusionState::Enter()
{
	enemyid = owner->GetComponent<EnemySlime>();

	owner->GetComponent<ModelControll>() = owner->GetComponent<ModelControll>();

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Die, loop, currentAnimationStartSeconds, blendSeconds);
	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);

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
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	}

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// �Ђ��܂�
	if (animationTime >= 1.1)
	{
		// �A�j���[�V�������[��
		enemyid->SetUpdateAnim(EnemySlime::UpAnim::Stop);

	}

}
// �����I��
void ConfusionState::Exit()
{

}

// �v���C���[
void PlayerIdleState::Enter()
{
	//playerid = owner->GetComponent<Player>();

	moveid = owner->GetComponent<Movement>();

	//modelid = owner->GetComponent<ModelControll>();

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Idle, loop,currentAnimationStartSeconds,blendSeconds
	);
	
	// �A�j���[�V�������[��
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);
	

	// ������
	bool stop = false;
	moveid->SetStopMove(stop);
}

void PlayerIdleState::Execute(float elapsedTime)
{
	// ���b�N�I������
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);

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


	// �A�j���[�V�������[��
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Doble);


	// ������
	bool stop = false;
	moveid->SetStopMove(stop);
}

void PlayerMovestate::Execute(float elapsedTime)
{
	// ���b�N�I������
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);

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

	// �A�j���[�V�������[��
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


	// ������
	bool stop = false;
	moveid->SetStopMove(stop);
}

void PlayerJumpState::Execute(float elapsedTime)
{

	// ���b�N�I������
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);

	// �W�����v���͏���
	if (owner->GetComponent<Player>()->InputJump())
	{
		
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
	}

	// �ړ�
	owner->GetComponent<Player>()->InputMove(elapsedTime);

	// ���
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
	moveid->SetOnLadius(false);

	modelControllid->GetModel()->PlayAnimation(
		Player::Anim_Land, loop,
		currentAnimationStartSeconds, blendSeconds
	);

	// �A�j���[�V�������[��
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Reverseplayback);
}

void PlayerLandState::Execute(float elapsedTime)
{
	// ���b�N�I������
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);
	if (!modelControllid->GetModel()->IsPlayAnimation())
	{
		moveid->SetOnLadius(false);
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

	// �A�j���[�V�������[��
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerJumpFlipState::Execute(float elapsedTime)
{
	// ���b�N�I������
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);
	// �ړ�
	if (owner->GetComponent<Player>()->InputMove(elapsedTime))
		bool afterimagemove = true;
	// �W�����v���͏���
	if (!modelControllid->GetModel()->IsPlayAnimation())
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

	modelControllid = owner->GetComponent<ModelControll>();
	moveid = owner->GetComponent<Movement>();

	//std::shared_ptr<Player> playerId = owner->GetComponent<Player>();

		//Player::Anim_Slash, loop,
	// �A�j���[�V�����Đ�
	modelControllid->GetModel()->PlayAnimation(
		Player::Anim_Slash, loop,
		currentAnimationStartSeconds, blendSeconds
	);
	// �A�j���[�V�����Đ�
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

	
	moveid->SetGravity(gravity);


	// �ړ��̒�~
	bool stopMove = true;
	moveid->SetStopMove(stopMove);

	// ��]���邩�`�F�b�N
	rotateCheck = false;
}

void PlayerAttackState::Execute(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	// ���b�N�I������
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);

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



	// ��]
	if (!rotateCheck)
	{
		DirectX::XMVECTOR Vector;
		DirectX::XMVECTOR LengthSq;

		DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&owner->GetComponent<Transform>()->GetPosition());
		EnemyManager& enemyManager = EnemyManager::Instance();
		int enemyCount = enemyManager.GetEnemyCount();
		for (int i = 0; i < enemyCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
		{
			DirectX::XMVECTOR enemyPosition = DirectX::XMLoadFloat3(&enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition());


			Vector = DirectX::XMVectorSubtract(enemyPosition, playerPosition);

			LengthSq = DirectX::XMVector3Length(Vector);

			DirectX::XMStoreFloat3(&vector, Vector);

			DirectX::XMStoreFloat(&length, LengthSq);
			// ����
			if (length < attackCheckRange && length > attackCheckRangeMin)
			{
				bool stop = false;
				moveid->SetStopMove(stop);
				
				
				
				
				// ����
				if (moveid->Turn(vector, turnSpeed, elapsedTime))
				{
						//Player::Anim_Slash, loop,
					rotateCheck = true;
					modelControllid->GetModel()->PlayAnimation(
						Player::Anim_Slash, loop,
						currentAnimationStartSeconds, blendSeconds
					);
					// �A�j���[�V�����Đ�
					owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

					moveid->Move(vector, speed, elapsedTime);
					
				}

			}
			else
			{

				bool stop = true;
				moveid->SetStopMove(stop);
			}


		}

	}


	if (!modelControllid->GetModel()->IsPlayAnimation())
	{
		owner->GetComponent<Player>()->SetAttackCollisionFlag(false);
		// ���͊m�F�ŃX�e�[�g�ύX
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(button ? Player::State::Attack : Player::State::Move));

		
	
		// �ړ��̒�~
		moveid->SetGravity(button ?
			gravity :
			owner->GetComponent<Player>()->GetGravity());

		bool stop = false;
		moveid->SetStopMove(stop);


		button = false;
		
	}
	

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = modelControllid->GetModel()->GetCurrentANimationSeconds();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B

	owner->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius(), "shoulder");
}

void PlayerAttackState::Exit()
{
	
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
	// ���b�N�I������
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

	    // ���S���͂�����ۂ��J�����A���O���Ŏ��S
    // �Ⴆ�΃R����K�E�Z�Ȃǂŏ�肭���p�ł���΂����������J�������o�����܂���
    MessageData::CAMERACHANGEMOTIONMODEDATA	p;

	DirectX::XMFLOAT3 position = transformid->GetPosition();
	DirectX::XMFLOAT3 angle = transformid->GetAngle();



    float vx = sinf(angle.y) * 6;
    float vz = cosf(angle.y) * 6;
    p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
    p.data.push_back({ 95, {position.x + vx + 13, position.y + 3, position.z + vz }, position });
	
    Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B
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

// ���������
void PlayerAvoidanceState::Enter()
{
	transformid = owner->GetComponent<Transform>();
	moveid = owner->GetComponent<Movement>();
	hpid = owner->GetComponent<HP>();

		//Player::Anim_Slash, loop
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Dush, loop
	,currentAnimationStartSeconds, blendSeconds);


	// �����蔻��̗L��
	owner->GetComponent<Player>()->DmageInvalidJudment(false);

	moveSpeed = 10.0f;

	// �A�j���[�V������� �ʏ�
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

	// ������̒�~
	bool stopFall = true;
	moveid->SetStopFall(stopFall);

}

// ����X�V
void PlayerAvoidanceState::Execute(float elapsedTime)
{
	// ���b�N�I������
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);

	// �]����_�b�V��
	DirectX::XMFLOAT3 dir;
	dir.x = sinf(transformid->GetAngle().y);
	dir.y = cosf(transformid->GetAngle().x);
	dir.z = cosf(transformid->GetAngle().y);

	
	// �C�ӂ̃A�j���[�V�����Đ����
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// ���̎��Ԃ�
	if (animationTime >= 0.7f && animationTime <= 1.5f)
	{
		moveid->Move(dir, moveSpeed, elapsedTime);
	}
	// �A�j���[�V�����I��
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{

		// �����蔻��̗L��
		owner->GetComponent<Player>()->DmageInvalidJudment(true);
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


		// ������̒�~
		bool stopFall = false;
		moveid->SetStopFall(stopFall);
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

		//Player::Anim_Counter, loop,
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Counter, loop,
		currentAnimationStartSeconds, blendSeconds);

	// �A�j���[�V������� �ʏ�
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


}

// ���ˍX�V
void PlayerReflectionState::Execute(float elapsedTime)
{
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		// �����蔻��̗L��
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));

	}


	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B
	bool CollisionFlag = animationTime >= 0.3f && animationTime <= 0.5f;

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


