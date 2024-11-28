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
	//stateTimer = 60.0f;

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

void DeathState::Enter()
{
	enemyid = owner->GetComponent<EnemySlime>();
	transformid = owner->GetComponent<Transform>();


	owner->GetComponent<ModelControll>() = owner->GetComponent<ModelControll>();
	owner->GetComponent<ModelControll>()->GetModel()->
		PlayAnimation(
			EnemySlime::Animation::Anim_Die, loop);
	// �A�j���[�V�������[��
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

	if (stateTimer < 0)
	{

		enemyid->SetClearCheck(clearCheck);
	}
}

void DeathState::Exit()
{
	return;
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
	//moveid->SetOnLadius(false);

	modelControllid->GetModel()->PlayAnimation(
		Player::Anim_Land, loop,
		currentAnimationStartSeconds, blendSeconds
	);

	// �A�j���[�V�������[��
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerLandState::Execute(float elapsedTime)
{
	// ���b�N�I������
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

	// ���
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
		// �P��

		// �Đ��J�n
		currentAnimationStartSeconds = 0.0f;
		// �A�j���[�V�����Đ�
		modelControllid->GetModel()->PlayAnimation(
			Player::Anim_Slash, loop,
			currentAnimationStartSeconds, blendSeconds
			, currentAnimationAddSeconds
		);
		// �U���O��ň�x�����I��
		++attackMemory;


	

	if (!InitializationCheck)
	{

		// �R�}���h�ݒ���ڍU��
		commandSeconde.push_back(GamePad::BTN_B);
		commandSeconde.push_back(GamePad::BTN_B);

		// �R�}���h�ݒ�O��ڍU��
		commandThrede.push_back(GamePad::BTN_B);
		commandThrede.push_back(GamePad::BTN_B);
		commandThrede.push_back(GamePad::BTN_B);
	}
	// �A�j���[�V�����Đ�
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

	
	//moveid->SetGravity(gravity);
	// �d�̓I�t
	bool stopFall = true;
	moveid->SetStopFall(stopFall);



	// �ړ��̒�~
	//bool stopMove = true;
	//moveid->SetStopMove(stopMove);

	// ��]���邩�`�F�b�N
	rotateCheck = false;


	// �_���[�W�H��������ɍU�����P����ɖ߂�����
	deleteCheck = true;

	InitializationCheck = true;

}

void PlayerQuickJabState::Execute(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	// ���b�N�I������
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);




	if (owner->GetComponent<Player>()->InputAttack() && 
		owner->GetComponent<Player>()->GetSelectCheck() ==
		(int)Player::CommandAttack::Attack)
	{
		// �R�}���h�m�F2�e�U��
		if (gamePad.ConfirmCommand(commandSeconde, frame) && !button && !buttonSeconde)
		{
			button = true;
			//owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
		}
	}

	// ���
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
	for (int i = 0; i < enemyCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
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



	// ��]
	if (!rotateCheck)
	{

			// ����
			if (length < attackCheckRange && length > attackCheckRangeMin
				&& moveid->TurnCheck(
					vector, angleRange, elapsedTime))
			{
				//bool stop = false;
				//moveid->SetStopMove(stop);
				
				// ����
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
		// ���͊m�F�ŃX�e�[�g�ύX
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


		// �U���ő�l����
		attackMemory = 0;
	}



	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = modelControllid->GetModel()->GetCurrentANimationSeconds();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B


	// 1����
	if (animationTime >= 0.8f )
	{
		// �P��ڂ̍U���Ȃ�
		oneAttackCheck = false;

		owner->GetComponent<Player>()->SetAttackCollisionFlag(false);
		deleteCheck = false;
		// ���͊m�F�ŃX�e�[�g�ύX
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(button  ? Player::State::SideCut : Player::State::Move));
		return;
	}



	owner->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");
}

void PlayerQuickJabState::Exit()
{
	// �ړ��̒�~
	moveid->SetGravity(
		owner->GetComponent<Player>()->GetGravity());




	// �d�̓I�t
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
	if (deleteCheck )
	{
		// �_���[�W�H��������ǂ���
		button = false;
		buttonSeconde = false;

		// �U���ő�l����
		attackMemory = 0;
	}

}


void PlayerSideCutState::Enter()
{
	bool loop = false;

	modelControllid = owner->GetComponent<ModelControll>();
	moveid = owner->GetComponent<Movement>();
	transformid = owner->GetComponent<Transform>();

	// ���

		// �Đ��J�n
		currentAnimationStartSeconds = 0.2f;

		// �A�j���[�V�����Đ�
		modelControllid->GetModel()->PlayAnimation(
			Player::Anim_SlashBeside, loop,
			currentAnimationStartSeconds, blendSeconds
			, currentAnimationAddSeconds
		);

	

	if (!InitializationCheck)
	{

		// �R�}���h�ݒ���ڍU��
		commandSeconde.push_back(GamePad::BTN_B);


		// �R�}���h�ݒ�O��ڍU��
		commandThrede.push_back(GamePad::BTN_B);


	}
	// �A�j���[�V�����Đ�
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


	//moveid->SetGravity(gravity);
	// �d�̓I�t
	bool stopFall = true;
	moveid->SetStopFall(stopFall);

	// �ړ��̒�~
	//bool stopMove = true;
	//moveid->SetStopMove(stopMove);

	// ��]���邩�`�F�b�N
	rotateCheck = false;


	InitializationCheck = true;
}

void PlayerSideCutState::Execute(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	// ���b�N�I������
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
		// �R�}���h�m�F3�e�U��
		if (gamePad.ConfirmCommand(commandThrede, frame))
		{
			buttonSeconde = true;
			//owner->GetComponent<Pl/ayer>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
		}

	}

	// ���
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
	for (int i = 0; i < enemyCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
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

	// ��]
	if (!rotateCheck)
	{
		//DirectX::XMVECTOR Vector;
		//DirectX::XMVECTOR LengthSq;

		//DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&owner->GetComponent<Transform>()->GetPosition());
		//EnemyManager& enemyManager = EnemyManager::Instance();
		//int enemyCount = enemyManager.GetEnemyCount();
		//for (int i = 0; i < enemyCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
		//{
		//	DirectX::XMVECTOR enemyPosition = DirectX::XMLoadFloat3(&enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition());


		//	Vector = DirectX::XMVectorSubtract(enemyPosition, playerPosition);

		//	LengthSq = DirectX::XMVector3Length(Vector);

		//	DirectX::XMStoreFloat3(&vector, Vector);

		//	DirectX::XMStoreFloat(&length, LengthSq);
			// ����
			if (length < attackCheckRange && length > attackCheckRangeMin
				&& moveid->TurnCheck(
					vector, angleRange, elapsedTime))
			{
				//bool stop = false;
				//moveid->SetStopMove(stop);

				// ����
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


	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = modelControllid->GetModel()->GetCurrentANimationSeconds();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B


	// 2����
	if (animationTime >= 1.1f)
	{
		owner->GetComponent<Player>()->SetAttackCollisionFlag(false);
		deleteCheck = false;



		// ���͊m�F�ŃX�e�[�g�ύX
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(buttonSeconde ? Player::State::CycloneStrike : Player::State::Move));

		return;
	}


	owner->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");

}

void PlayerSideCutState::Exit()
{
	// �ړ��̒�~
	moveid->SetGravity(
		owner->GetComponent<Player>()->GetGravity());


	// �d�̓I�t
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
	
}


void PlayerCycloneStrikeState::Enter()
{
	bool loop = false;

	modelControllid = owner->GetComponent<ModelControll>();
	moveid = owner->GetComponent<Movement>();
	transformid = owner->GetComponent<Transform>();

		// �Đ��J�n
		currentAnimationStartSeconds = 0.7f;
		// �A�j���[�V�����Đ�
		modelControllid->GetModel()->PlayAnimation(
			Player::Anim_SlashThree, loop,
			currentAnimationStartSeconds, blendSeconds
			, currentAnimationAddSeconds
		);



	if (!InitializationCheck)
	{

		// �R�}���h�ݒ���ڍU��
		commandSeconde.push_back(GamePad::BTN_B);
		commandSeconde.push_back(GamePad::BTN_B);

		// �R�}���h�ݒ�O��ڍU��
		commandThrede.push_back(GamePad::BTN_B);
		commandThrede.push_back(GamePad::BTN_B);
		commandThrede.push_back(GamePad::BTN_B);
	}
	// �A�j���[�V�����Đ�
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

	// �d�̓I�t
	bool stopFall = true;
	moveid->SetStopFall(stopFall);

	//// �ړ��̒�~
	//bool stopMove = true;
	//moveid->SetStopMove(stopMove);

	// ��]���邩�`�F�b�N
	rotateCheck = false;


	InitializationCheck = true;
}

void PlayerCycloneStrikeState::Execute(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	// ���b�N�I������
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);


	//angle = transformid->GetAngle();

	//DirectX::XMFLOAT3 direction;
	//direction.x = sinf(angle.y) * 6;
	//direction.y = 0;
	//direction.z = cosf(angle.y) * 6;

	//moveid->Move(direction, attackSpeed, elapsedTime);

	// ���
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
	for (int i = 0; i < enemyCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
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

	// ��]
	if (!rotateCheck)
	{

			// ����
			if (length < attackCheckRange && length > attackCheckRangeMin
				&& moveid->TurnCheck(
					vector, angleRange, elapsedTime))
			{
				//bool stop = false;
				//moveid->SetStopMove(stop);

				// ����
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


	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = modelControllid->GetModel()->GetCurrentANimationSeconds();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B


	// 3����
	if (animationTime >= 2.7f )
	{
		owner->GetComponent<Player>()->SetAttackCollisionFlag(false);
		deleteCheck = false;



		// ���͊m�F�ŃX�e�[�g�ύX
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
		return;
	}


	owner->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");

}

void PlayerCycloneStrikeState::Exit()
{
	// �ړ��̒�~
	moveid->SetGravity(
		owner->GetComponent<Player>()->GetGravity());



	// �d�̓I�t
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

	// �A�j���[�V�����Đ�
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
	// �A�j���[�V�����Đ�
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

	// ������̒�~
	bool stopFall = true;
	moveid->SetStopFall(stopFall);

	button = true;

	// �t���b�V��
	flashOn = true;


	// ����
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

					// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
		float animationTime = modelControllid->GetModel()->GetCurrentANimationSeconds();


		// �A�j���[�V����
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON)
		{
	

			// �A�j���[�V�����Đ�
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
		// �Ⴆ�΃R����K�E�Z�Ȃǂŏ�肭���p�ł���΂����������J�������o�����܂���
		//MessageData::CAMERACHANGEMOTIONMODEDATA	p;

		//DirectX::XMFLOAT3 position = transformid->GetPosition();
		//DirectX::XMFLOAT3 angle = transformid->GetAngle();



		/*float vx = sinf(angle.y);
		float vz = cosf(angle.y);*/
		//p.data.push_back({ 0, {position.x * vx , position.y + 3, position.z - vz }, position });

		// ���b�N�I��
		//rockCheck = true;
		//owner->GetComponent<Player>()->SetRockCheck(rockCheck);

		//owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);

		// �t���[�J�����̃��b�N���O��
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

			// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
		float animationTime = modelControllid->GetModel()->GetCurrentANimationSeconds();


		// �X���[���[�V�����ƐF�𖾂邭
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON)
		{
			owner->GetComponent<Player>()->SetFlashOn(flashOn);
			owner->GetComponent<Player>()->SetHitCheck(true);
		}
		// �A�j���[�V�����؂��
		if (animationTime >= 1.6f)
		{
			float invincibleTimer = 0.0f;
			owner->GetComponent<HP>()->SetInvincibleTimer(invincibleTimer);




			// �J�����U��
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
		// �A�j���[�V�����I��
		if (!modelControllid->GetModel()->IsPlayAnimation())
		{
			// ���͊m�F�ŃX�e�[�g�ύX
			owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));

		}
	}

}

void PlayerSpecialAttackState::Exit()
{
	// ������̍ĊJ
	bool stopFall = false;
	moveid->SetStopFall(stopFall);



	//rockCheck = false;
	//owner->GetComponent<Player>()->SetRockCheck(rockCheck);

	//// �t���[�J�����̃��b�N���Ƃǂ߂�
	//bool freeCameraCheck = true;
	//owner->GetComponent<Player>()->SetFreeCameraCheck(freeCameraCheck);

}

void PlayerMagicState::Enter()
{

	modelControllid = owner->GetComponent<ModelControll>();
	moveid = owner->GetComponent<Movement>();


	// �T���_�[�n
	if (owner->GetComponent<Player>()->GetSelectMagicCheck() ==
		(int)Player::CommandMagic::Thander)
	{
		// �A�j���[�V�����Đ�
		modelControllid->GetModel()->PlayAnimation(
			Player::Anim_MagicSeconde, loop,
			currentAnimationStartSeconds, blendSeconds
		);


	}
	else
	{
		// �A�j���[�V�����Đ�
		modelControllid->GetModel()->PlayAnimation(
			Player::Anim_Magic, loop,
			currentAnimationStartSeconds, blendSeconds
		);
	}

	// �A�j���[�V�����Đ�
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


	magicType = owner->GetComponent<Player>()->GetSelectMagicCheck();
	// ������̒�~
	bool stopFall = true;
	moveid->SetStopFall(stopFall);

}

void PlayerMagicState::Execute(float elapsedTime)
{

	// ���b�N�I������
	owner->GetComponent<Player>()->UpdateCameraState(elapsedTime);

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = modelControllid->GetModel()->GetCurrentANimationSeconds();
	// �A�j���[�V�����I�������ʂ�



	// ���@�̎�ނŏI�����A��B
	switch (magicType)
	{
	case (int)Player::CommandMagic::Fire:
	{
		// ����
		if (animationTime <= 0.5f)return;
		// ������
		owner->GetComponent<Player>()->InputMagicframe();

		owner->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Idle);
		break;
	}

	case (int)Player::CommandMagic::Thander:
	{
		// ����
		if (animationTime <= 1.1f)return;

		// ������
		owner->GetComponent<Player>()->InputMagicLightning();
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Idle);
		break;
	}
	case (int)Player::CommandMagic::Ice:
	{
		// ����
		if (animationTime <= 0.5f)return;
		// �X����
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
	// ���@�̑I�����[����
	owner->GetComponent<Player>()->SetSelectCheck((int)Player::CommandMagic::Normal);

	// ������̍ĊJ
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
	// �Đ��J�n����
	currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����Đ�
	modelControllid->GetModel()->PlayAnimation(
		Player::Anim_SlashThree, loop,
		currentAnimationStartSeconds, blendSeconds
		, currentAnimationAddSeconds
	);
	// �A�j���[�V�����Đ�
	owner->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


	Model::Node* pHPosiiton = modelControllid->GetModel()->FindNode("mixamorig:LeftHand");

	DirectX::XMFLOAT3 pPosition =
	{
				pHPosiiton->worldTransform._41,
				pHPosiiton->worldTransform._42,
				pHPosiiton->worldTransform._43
	};

	fire->Play(pPosition);
	// ������̒�~
	bool stopFall = true;
	moveid->SetStopFall(stopFall);

	button = true;

	// �X�^�[�g�l
	specialMoveWaitTime = specialMoveWaitStartTime;


}

void PlayerSpecialMagicState::Execute(float elapsedTime)
{
	if (button)
	{

		// �J�������[�V����
		MessageData::CAMERACHANGEMOTIONMODEDATA	p;

		position = transformid->GetPosition();
		angle = transformid->GetAngle();


		// ���[�V�����L�^
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

		// ���[�V�����ݒ�
		Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

		// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
		//float animationTime = modelControllid->GetModel()->GetCurrentANimationSeconds();
		
		// �G�t�F�N�g�X�V
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



		// �A�j���[�V����
		if (!modelControllid->GetModel()->IsPlayAnimation())
		{
			// �Đ��J�n����
			currentAnimationStartSeconds = 0.0f;

			// �A�j���[�V�����Đ�
			modelControllid->GetModel()->PlayAnimation(
				Player::Anim_MagicSeconde, loop,
				currentAnimationStartSeconds, blendSeconds
				, currentAnimationAddSeconds
			);
			button = false;

			// ��̉��G�t�F�N�g��~
			fire->Stop(fire->GetEfeHandle());


			//rockCheck = true;
			//owner->GetComponent<Player>()->SetRockCheck(rockCheck);


			
		}
	}
	else
	{
			// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
		float animationTime = modelControllid->GetModel()->GetCurrentANimationSeconds();

		// �A�j���[�V����
		if (animationTime >= 1.1f)
		{

			owner->GetComponent<Player>()->SetFlashOn(true);
			//owner->GetComponent<Player>()->SetHitCheck(true);

			// �A�j���[�V������~
			owner->GetComponent<Player>()->SetUpdateAnim
			(Player::UpAnim::Stop);


			// �J�����U��
			MessageData::CAMERASHAKEDATA cameraShakeData;

			// �J�����U���l
			float shakeTimer = 0.5f;
			float shakePower = 0.8f;
			cameraShakeData = { shakeTimer , shakePower };
			// �J�����U���ݒ�
			Messenger::Instance().SendData(MessageData::CAMERASHAKE, &cameraShakeData);

			Model::Node* pHPosiiton = modelControllid->GetModel()->FindNode("mixamorig:LeftHand");

			DirectX::XMFLOAT3 pPosition =
			{
						pHPosiiton->worldTransform._41,
						pHPosiiton->worldTransform._42,
						pHPosiiton->worldTransform._43
			};


			// ���@����
			owner->GetComponent<Player>()->InputSpecialMagicframe();


			// �K�E�Z���������̌o�ߎ���
			specialMoveWaitTime += elapsedTime;

		}
	}



	// �_���[�W����
	owner->GetComponent<Player>()->SpecialApplyDamageInRadius();



	// �ҋ@���Ԃ��߂����̂Ŏ��̃X�e�[�g��
	if (specialMoveWaitTime >= specialMoveWaitTimeMax)
	{
		// ���̃X�e�[�g��
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


		return;
	}

	


}

void PlayerSpecialMagicState::Exit()
{
	// ������̍ĊJ
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
	// �������R
	bool stopMove = false;
	moveid->SetStopMove(stopMove);

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
	//if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	if (animationTime >= 2.0f)
	{

		// �����蔻��̗L��
		owner->GetComponent<Player>()->DmageInvalidJudment(true);
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));



	}
}

// ����I��
void PlayerAvoidanceState::Exit()
{
	// ������̒�~
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
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


