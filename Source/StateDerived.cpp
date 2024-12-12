#include <math.h>
#include "EnemySlime.h"
#include "StateDerived.h"

#include "Player.h"
#include "Mathf.h"

#include "ProjectileHoming.h"
#include "ProjectileRuby.h"
#include "ProjectileThrowing.h"
#include "ProjectileManager.h"
#include "BulletFiring.h"

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
	
	enemyid = owner.lock()->GetComponent<EnemySlime>();
	model = owner.lock()->GetComponent<ModelControll>()->GetModel();


	moveid = owner.lock()->GetComponent<Movement>();
	// �꒣��
	enemyid->SetRandomTargetPosition();

	// �A�j���[�V�����Đ�
	owner.lock()->GetComponent<ModelControll>()->GetModel()->
		PlayAnimation(EnemySlime::Animation::Anim_Walk,
			loop, currentAnimationStartSeconds, blendSeconds, 
			currentAnimationAddSeconds);
	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);

	stateTimer = stateTimerMax;
	//stateTimer = 60.0f;

	// ���n�u��
	upOnLading = false;

	// �v���C���[id



}

// �p�j�X�e�[�g�Ŏ��s���郁�\�b�h
void WanderState::Execute(float elapsedTime)
{
	stateTimer -= elapsedTime;

	// �ڕW�n�_�����v���C���[�ʒu�ɐݒ�
	targetPosition = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();



	enemyid->SetTargetPosition(targetPosition);
	
	//// �ړI�n�_�܂ł�XZ���ʂł̋�������
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
	DirectX::XMVECTOR enemyPositionVec = DirectX::XMLoadFloat3(&enemyid->GetPosition());

	DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(targetPositionVec, enemyPositionVec);

	DirectX::XMFLOAT3 direction;

	DirectX::XMStoreFloat3(&direction, directionVec);

	moveid->Turn(direction, turnSpeed, elapsedTime);

	// �ړI�n�֒�����
	if (stateTimer <= stateTimerEnd)
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Idle));
	}
	// �ړI�n�ɂ��̔��a��������
	float radius = enemyid->GetAttackRightFootRange();

	moveid->Move(direction ,moveSpeed, elapsedTime);

	// �����蔻��
	{
		Model::Node * bossLeftFoot = model->FindNode("boss_left_foot1");
		Model::Node* bossRightFoot = model->FindNode("boss_right_foot1");
		Model::Node* bossRightHand = model->FindNode("boss_right_hand1");
		Model::Node* bossLeftHand = model->FindNode("boss_left_hand1");

		// �m�[�h�ʒu�擾
		// ����
		DirectX::XMFLOAT3 bossLeftFootPosition;
		bossLeftFootPosition = model->ConvertLocalToWorld(bossLeftFoot);

		enemyid->DetectHitByBodyPart(bossLeftFootPosition);

		// �E��
		DirectX::XMFLOAT3 bossRightFootPosition;
		bossRightFootPosition = model->ConvertLocalToWorld(bossRightFoot);

		enemyid->DetectHitByBodyPart(bossRightFootPosition);

		// �E�r
		DirectX::XMFLOAT3 bossRightHandPosition;
		bossRightHandPosition = model->ConvertLocalToWorld(bossRightHand);

		enemyid->DetectHitByBodyPart(bossRightHandPosition);

		// ���r
		DirectX::XMFLOAT3 bossLeftHandPosition;
		bossLeftHandPosition = model->ConvertLocalToWorld(bossLeftHand);

		enemyid->DetectHitByBodyPart(bossLeftHandPosition);

	}
	//enemyid->CollisionPlayerWithRushEnemy();

	//// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	//float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	//if (animationTime >= 0.6f && animationTime <= 0.7f)
	//{
	//	// �A�j���[�V�������[�� ��~
	//	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Stop);
	//
	//	if (distSq > radius * radius)
	//		// �ړI�n�_�ֈړ�
	//		enemyid->MoveToTarget(elapsedTime, 0.8f);


	//	// �W�����v
	//	enemyid->InputJump();

	//	// ���n�ŏՌ��g���΂�
	//	if (moveid->GetOnLadius() && !upOnLading)
	//	{

	//		Model::Node* node = owner->GetComponent<ModelControll>()->GetModel()->FindNode("boss_right_foot1");
	//		DirectX::XMFLOAT3 pos(
	//			node->worldTransform._41,
	//			node->worldTransform._42,
	//			node->worldTransform._43
	//		);
	//		enemyid->InputImpact(pos);
	//		// ���n�u��
	//		upOnLading = true;
	//	}
	//	if (!moveid->GetOnLadius())
	//	{

	//		// ���n�u��
	//		upOnLading = false;
	//	}
	//}


}

// �p�j�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void WanderState::Exit()
{

}

void WanderState::End()
{
	// ���
	owner.lock().reset();
}

// ������
void IdleState::Enter()
{

	enemyid = owner.lock()->GetComponent<EnemySlime>();

	
	
	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	//model = owner.lock()->GetComponent<ModelControll>();
	//move = owner->GetComponent<Movement>();
	hp = owner.lock()->GetComponent<HP>();
	//model = owner->GetComponent<ModelControll>();
	model->PlayAnimation(EnemySlime::Anim_Standby, loop, currentAnimationStartSeconds,blendSeconds);
	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);
	
	// �ҋ@����
	//stateTimer = stateTimerMax;
	stateTimer = Mathf::RandomRange(stateTimerMin, stateTimerMax);

	// ���������������
	hp->ResetOnDamageThresholdTime();
}

// update
void IdleState::Execute(float elapsedTime)
{
	 stateTimer -= elapsedTime;

	if (stateTimer <= stateTimerEnd)
	{
		// TODO 03 
		// �ҋ@���Ԃ��o�߂����Ƃ��p�j�X�e�[�g�֑J�ڂ��Ȃ���
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	}
	// ���_���[�W�̍ő�l
	float damageThreshold = 10;
	float timeLimit = 5;
	if (hp->CheckDamageThresholdWithinTime(elapsedTime, damageThreshold, timeLimit))
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Jump));
	}

	// �_���[�W���[�V�����΍�
	if (!model->IsPlayAnimation())
	{
		owner.lock()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Anim_Standby, loop, currentAnimationStartSeconds, blendSeconds);
	}
}

// �Ō�ɂ�肽�������S��
void IdleState::Exit()
{
	// ���������������
	hp->ResetOnDamageThresholdTime();
}
void IdleState::End()
{
	// ���
	owner.lock().reset();
}
// ������
void PursuitState::Enter()
{
	// �v���C���[�R���|�[�l���g�A�N�Z�X
	//playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Player>();

	--mortionLimit;
	// �G�l�~�[�A�N�Z�X�p
	enemyid = owner.lock()->GetComponent<EnemySlime>();
	enemyid->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		EnemySlime::Animation::Anim_Standby, loop,currentAnimationStartSeconds,  blendSeconds);

	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);

	// ���b�ԒǐՂ���^�C�}�[�������_���ݒ�
	stateTimer = Mathf::RandomRange(3.0f, 5.0f);

	// �U���̎�ނ��o��
	randamAttack = rand() % 2;

	//attackRange = enemyid->GetAttackRightFootRange();

	//hpid = owner->GetComponent<HP>();

	// �G���C�t
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
		return;
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


	switch (randamAttack)
	{
	case (int)EnemySlime::AttackMode::AssaultAttack:
	{
		enemyid->GetStateMachine()->ChangeState(
			static_cast<int>(EnemySlime::State::Wander));
		break;
	}
	case (int)EnemySlime::AttackMode::JumpStompAttack:
	{
		enemyid->GetStateMachine()->ChangeState(
			static_cast<int>(EnemySlime::State::Jump));
		break;
	}

	}


	//switch (attackRound)
	//{
	//case AttackChange::Round1:
	//{
	//	// ���Ԃ��߂����̂�
	//	if (stateTimer < 0.0f)
	//	{
	//		
	//		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));
	//	}

	//	break;
	//}

	//case AttackChange::Round2:
	//{
	//	if (stateTimer < 0.0f && attackType == 0)
	//		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));

	//	//if (stateTimer < 0.0f && attackType == 1)
	//	//	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::ShotThrowing));

	//	break;
	//}

	//// HP�c�@�O
	//case AttackChange::ROund3:
	//{

	//	//owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::ShotThrowing));
	//	if (stateTimer < 0.0f && attackType == 0)
	//		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Shot));

	//	//if (stateTimer < 0.0f && attackType == 1)
	//	//	owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::ShotThrowing));

	//	if (stateTimer < 0.0f && attackType == 1)
	//		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Wander));

	//	break;
	//}


	//// HP�c�@�@�Q�ƂP
	//default:
	//{


	//	break;
	//}
	//}

}
// �Ō�ɂ�肽�������S��
void PursuitState::Exit()
{

}

void PursuitState::End()
{
	// ���
	owner.lock().reset();
}

// ������
void JumpState::Enter()
{

	enemyid = owner.lock()->GetComponent<EnemySlime>();


	moveid = owner.lock()->GetComponent<Movement>();
	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	hpid = owner.lock()->GetComponent<HP>();
	// �꒣��
	enemyid->SetRandomTargetPosition();

	// �A�j���[�V�����Đ�
	model->
		PlayAnimation(EnemySlime::Animation::Anim_Attack,
			loop, currentAnimationStartSeconds, blendSeconds,
			currentAnimationAddSeconds, keyFrameEnd);
	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Reverseplayback);

	//stateTimer = Mathf::RandomRange(6.0f, 8.0f);
	//stateTimer = 60.0f;

	// ���n�u��
	upOnLading = false;

	// �A�j���[�V�����I��
	jumpStart = false;

	// �ڕW�n�_�����v���C���[�ʒu�ɐݒ�
	targetPosition =
		PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();

	// ������������Ȃ炱����
	if (hpid->GetIsOverDamageRule())
	{
		targetPosition = { 0,enemyid->GetPosition().y,0 };
		targetPosition.x += Mathf::RandomRange(-enemyid->GetSearchRange(), enemyid->GetSearchRange());
		targetPosition.z += Mathf::RandomRange(-enemyid->GetSearchRange(), enemyid->GetSearchRange());

		// ���_���[�W�m�F����
		bool isOverDamageRule = false;
		hpid->SetIsOverDamageRule(isOverDamageRule);
	}

}
// �X�V����
void JumpState::Execute(float elapsedTime)
{
	//stateTimer -= elapsedTime;




	enemyid->SetTargetPosition(targetPosition);


	DirectX::XMVECTOR targetPositionVec = DirectX::XMLoadFloat3(&targetPosition);
	DirectX::XMVECTOR enemyPositionVec = DirectX::XMLoadFloat3(&enemyid->GetPosition());

	DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(targetPositionVec, enemyPositionVec);

	DirectX::XMFLOAT3 direction;

	DirectX::XMStoreFloat3(&direction, directionVec);

	//// �ړI�n�֒�����
	//if (stateTimer < 0.0f)
	//{
	//	enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Idle));
	//}
	// �ړI�n�ɂ��̔��a��������
	float radius = enemyid->GetAttackRightFootRange();

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();

	if (animationTime <= 0.8f && !jumpStart)
	{
		bool blurCheck = true;
		enemyid->SetBlurCheck(blurCheck);

		enemyid->CollisionPlayerWithRushEnemy();

		moveid->JumpVelocity(jumpSpeed);

		jumpStart = true;

		return;
	}

	if (!moveid->GetOnLadius())
	{
		moveid->Move(direction, moveSpeed, elapsedTime);
		moveid->Turn(direction, turnSpeed, elapsedTime);
	}

	if (moveid->GetOnLadius() && jumpStart)
	{
		
		//enemyid->InputProjectile();
		enemyid->InputImpact(enemyid->GetPosition());
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Idle));
	}
	enemyid->CollisitionNodeVsPlayer("boss_left_foot1",enemyid->GetRadius());
}
// �I������
void JumpState::Exit()
{
	jumpStart = false;
}

void JumpState::End()
{
	// ���
	owner.lock().reset();
}


// ������
void AttackState::Enter()
{

	enemyid = owner.lock()->GetComponent<EnemySlime>();

	hpid = owner.lock()->GetComponent<HP>();

	currentAnimationStartSeconds = 0.0f;
	owner.lock()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Attack, loop, currentAnimationStartSeconds, blendSeconds);
	

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
	if (!owner.lock()->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation() && attackMemory < attackMemoryMax)
	{
		// �A�j���Đ�
		currentAnimationStartSeconds = 0.8f;
		owner.lock()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Attack, loop, currentAnimationStartSeconds, blendSeconds);

		
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
	float animationTime = owner.lock()->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	if (animationTime >= 1.0f && animationTime <= 2.0f)
	{
		// �����m�[�h�ƃv���C���[�̏Փˏ���
		enemyid->CollisitionNodeVsPlayer("boss_left_foot1", 2);
	}

	if (animationTime >= 1.30f && animationTime <= 1.33f)
	{
		Model::Node* node = owner.lock()->GetComponent<ModelControll>()->GetModel()->FindNode("boss_left_foot1");
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

void AttackState::End()
{
	// ���
	owner.lock().reset();
}

// �����ݒ�
void AttackShotState::Enter()
{
	enemyid = owner.lock()->GetComponent<EnemySlime>();
	//modelid = owner.lock()->GetComponent<ModelControll>();
	hpid = owner.lock()->GetComponent<HP>();
	

	currentAnimationStartSeconds = 0.0f;
	owner.lock()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Shot, loop, currentAnimationStartSeconds, blendSeconds);

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
	if (!owner.lock()->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		// �A�j���Đ�
		currentAnimationStartSeconds = 0.1f;
		owner.lock()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Shot, loop, currentAnimationStartSeconds ,blendSeconds);
		++attackCount;


	}
	if (attackCount >= attackCountMax)
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
		attackCount = 0;
	}

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner.lock()->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
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

void AttackShotState::End()
{
	// ���
	owner.lock().reset();
}

// ��������
void AttackShotThrowingState::Enter()
{
	enemyid = owner.lock()->GetComponent<EnemySlime>();
	transformid = owner.lock()->GetComponent<Transform>();

	currentAnimationStartSecondsf = 0.0f;
	owner.lock()->GetComponent<ModelControll>()->GetModel()->PlayReverseAnimation(EnemySlime::Animation::Anim_jewelattack, loop,currentAnimationStartSecondsf,blendSeconds);
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
	float animationTime = owner.lock()->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
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


				projectileManager->GetComponent<Transform>()->SetPosition(owner.lock()->GetComponent<Transform>()->GetPosition());
				
			}
		}
	}

	// �A�j���[�V�����ˌ�����
	if (animationTime <= 0.8f && !turnPermission)
	{
		// �����A�j���[�V�����I��
		bool end = true;
		owner.lock()->GetComponent<ModelControll>()->GetModel()->SetAnimationEndFlag(end);
		//bool blend = false;

		owner.lock()->GetComponent<ModelControll>()->GetModel()->ReverseplaybackAnimation(elapsedTime);
		// �A�j���[�V�����Đ�����
		currentAnimationStartSecondsf = 0.7f;

		owner.lock()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Shot, loop, currentAnimationStartSecondsf, blendSeconds);
		// �A�j���[�V�������[��
		enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);


		turnPermission = true;
	}
	// �ˌ�����
	if (!owner.lock()->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation() && turnPermission)
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

void AttackShotThrowingState::End()
{
	// ���
	owner.lock().reset();
}

void DamageState::Enter()
{
	enemyid = owner.lock()->GetComponent<EnemySlime>();

	owner.lock()->GetComponent<ModelControll>() = owner.lock()->GetComponent<ModelControll>();
	owner.lock()->GetComponent<ModelControll>()->GetModel()->
		PlayAnimation(
			EnemySlime::Animation::Anim_Die, loop,
			currentAnimationStartSeconds,currentAnimationAddSeconds,
			keyFrameEnd);
	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);

	stateTimer = stateTimerMax;

}

void DamageState::Execute(float elapsedTime)
{

	--stateTimer;

		if (stateTimer < stateTimerEnd)
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Idle));

}

void DamageState::Exit()
{
}

void DamageState::End()
{
	// ���
	owner.lock().reset();
}

// �����J�n
void ConfusionState::Enter()
{
	enemyid = owner.lock()->GetComponent<EnemySlime>();

	owner.lock()->GetComponent<ModelControll>() = owner.lock()->GetComponent<ModelControll>();

	owner.lock()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Die, loop, currentAnimationStartSeconds, blendSeconds,
		currentAnimationAddSeconds, keyFrameEnd);
	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Normal);

	// �X�e�[�g�ҋ@����
	stateTimer = stateTimerMax;

	

}
// �����X�V
void ConfusionState::Execute(float elapsedTime)
{
	stateTimer -= elapsedTime;

	if (stateTimer < stateTimerEnd)
	{
		// �X�e�[�g�ύX
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	}

	//// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	//float animationTime = owner.lock()->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	//// �Ђ��܂�
	//if (animationTime >= 1.1)
	//{
	//	// �A�j���[�V�������[��
	//	enemyid->SetUpdateAnim(EnemySlime::UpAnim::Stop);

	//}

	if (!owner.lock()->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
		owner.lock()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Die, loop, currentAnimationStartSeconds, blendSeconds,
			currentAnimationAddSeconds, keyFrameEnd);

}
// �����I��
void ConfusionState::Exit()
{

}

void ConfusionState::End()
{
	// ���
	owner.lock().reset();
}

void DeathState::Enter()
{
	enemyid = owner.lock()->GetComponent<EnemySlime>();
	transformid = owner.lock()->GetComponent<Transform>();


	owner.lock()->GetComponent<ModelControll>() = owner.lock()->GetComponent<ModelControll>();
	owner.lock()->GetComponent<ModelControll>()->GetModel()->
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

void DeathState::End()
{
	// ���
	owner.lock().reset();
}

// �v���C���[
void PlayerIdleState::Enter()
{
	//playerid = owner.lock()->GetComponent<Player>();

	moveid = owner.lock()->GetComponent<Movement>();

	//modelid = owner.lock()->GetComponent<ModelControll>();

	owner.lock()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Idle, loop,currentAnimationStartSeconds,blendSeconds
	);
	
	// �A�j���[�V�������[��
	owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);
	

	// ������
	bool stop = false;
	moveid->SetStopMove(stop);
}

void PlayerIdleState::Execute(float elapsedTime)
{
	// ���b�N�I������
	owner.lock()->GetComponent<Player>()->UpdateCameraState(elapsedTime);

	// �ړ����͏���
	if (owner.lock()->GetComponent<Player>()->InputMove(elapsedTime))
	{
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
		//TransitionMoveState();
	}

	// ���˓��͏���
	if (owner.lock()->GetComponent<Player>()->InputAvoidance())
	{
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Reflection));

	}


	// �W�����v���͏���
	if (owner.lock()->GetComponent<Player>()->InputJump())
	{
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
		//TransitionJumpState();
	}



}

void PlayerIdleState::Exit()
{
}

void PlayerIdleState::End()
{
	// ���
	owner.lock().reset();
}

void PlayerMovestate::Enter()
{
	owner.lock()->GetComponent<Player>() = owner.lock()->GetComponent<Player>();

	moveid = owner.lock()->GetComponent<Movement>();

	//modelid = owner.lock()->GetComponent<ModelControll>();

		//Player::Anim_Move, loop,
	owner.lock()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Running, loop,
		currentAnimationStartSeconds, blendSeconds
	);

	//	//Player::Anim_Move, loop,
	//owner.lock()->GetComponent<ModelControll>()->GetModel()->PlayUpeerBodyAnimation(
	//	Player::Anim_Running, loop,
	//	currentAnimationStartSeconds, blendSeconds
	//);


	// �A�j���[�V�������[��
	owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


	// ������
	bool stop = false;
	moveid->SetStopMove(stop);
}

void PlayerMovestate::Execute(float elapsedTime)
{
	// ���b�N�I������
	owner.lock()->GetComponent<Player>()->UpdateCameraState(elapsedTime);

	// �ړ����͏���
	if (!owner.lock()->GetComponent<Player>()->InputMove(elapsedTime))
	{
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
		//TransitionIdleState();
	}


	// �����͏���
	if (owner.lock()->GetComponent<Player>()->InputAvoidance())
	{
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

	}

	// �W�����v���͏���
	if (owner.lock()->GetComponent<Player>()->InputJump())
	{

		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
		
	}



}

void PlayerMovestate::Exit()
{
}

void PlayerMovestate::End()
{
	// ���
	owner.lock().reset();
}

void PlayerJumpState::Enter()
{
	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	moveid = owner.lock()->GetComponent<Movement>();

	model->PlayAnimation(
		Player::Anim_Jump, loop,
	currentAnimationStartSeconds, blendSeconds);

	// �A�j���[�V�������[��
	owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


	// ������
	bool stop = false;
	moveid->SetStopMove(stop);
}

void PlayerJumpState::Execute(float elapsedTime)
{

	// ���b�N�I������
	owner.lock()->GetComponent<Player>()->UpdateCameraState(elapsedTime);

	// �W�����v���͏���
	if (owner.lock()->GetComponent<Player>()->InputJump())
	{
		
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
	}




	// ���
	if (owner.lock()->GetComponent<Player>()->InputAvoidance())
	{
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}

	// ���n
	if (moveid->GetOnLadius())
	{
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Land));
	}


	//// �ړ�
	//if (owner.lock()->GetComponent<Player>()->InputMove(elapsedTime) && moveid->GetOnLadius())
	//{
	//	owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	//}


}

void PlayerJumpState::Exit()
{
}

void PlayerJumpState::End()
{
	// ���
	owner.lock().reset();
}

void PlayerLandState::Enter()
{
	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	moveid = owner.lock()->GetComponent<Movement>();
	//moveid->SetOnLadius(false);

	model->PlayAnimation(
		Player::Anim_Land, loop,
		currentAnimationStartSeconds, blendSeconds
	);

	// �A�j���[�V�������[��
	owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerLandState::Execute(float elapsedTime)
{
	// ���b�N�I������
	owner.lock()->GetComponent<Player>()->UpdateCameraState(elapsedTime);

	//// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	//float animationTime = model->GetCurrentANimationSeconds();
	//// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B

	// ����
	if (owner.lock()->GetComponent<Player>()->InputMove(elapsedTime))
	{
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	}

		// ���
	if (owner.lock()->GetComponent<Player>()->InputAvoidance())
	{
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}

	// �ҋ@
	if (!model->IsPlayAnimation())
	{
		
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


	}
}

void PlayerLandState::Exit()
{
}

void PlayerLandState::End()
{
	// ���
	owner.lock().reset();
}

void PlayerJumpFlipState::Enter()
{
	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	//moveid = owner.lock()->GetComponent<Movement>();

	model->PlayAnimation(
		Player::Anim_Jump, loop,
		currentAnimationStartSeconds , blendSeconds);

	// �A�j���[�V�������[��
	owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerJumpFlipState::Execute(float elapsedTime)
{
	// ���b�N�I������
	owner.lock()->GetComponent<Player>()->UpdateCameraState(elapsedTime);
	// �ړ�
	if (owner.lock()->GetComponent<Player>()->InputMove(elapsedTime))
		bool afterimagemove = true;
	// �W�����v���͏���
	if (!model->IsPlayAnimation())
	{

		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
	}

	// ���
	if (owner.lock()->GetComponent<Player>()->InputAvoidance())
	{
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}

}

void PlayerJumpFlipState::Exit()
{


}

void PlayerJumpFlipState::End()
{
	// ���
	owner.lock().reset();
}

void PlayerQuickJabState::Enter()
{
	

	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	moveid = owner.lock()->GetComponent<Movement>();
	transformid = owner.lock()->GetComponent<Transform>();

	//std::shared_ptr<Player> playerId = owner.lock()->GetComponent<Player>();

		//Player::Anim_Slash, loop,
		// �P��
	
	if (!moveid->GetOnLadius())
	{
		// �Đ��J�n���� 
		float currentAnimationStartSeconds = 0.6f;
		float currentAnimationUpperStartSeconds = 0.0f;

		// ���[�v
		bool loop = true;
		bool loopUpper = false;

		// �Đ����ԉ��Z���̒l
		float currentAnimationAddSeconds = 0.03f;
		float currentAnimationUpperAddSeconds = 0.03f;

		float keyFrameEnd = 40.0f;

		// �A�j���[�V�����Đ�
		model->PlayAnimation(
			Player::Anim_Jump, loop,
			currentAnimationStartSeconds, blendSeconds
			, currentAnimationAddSeconds, keyFrameEnd
		);


		// �A�j���[�V�����㔼�g�Đ�
		model->PlayUpeerBodyAnimation(
			Player::Anim_Slash, loopUpper,
			currentAnimationUpperStartSeconds, blendSeconds,
			currentAnimationUpperAddSeconds
			
		);
		// �U���O��ň�x�����I��
		++attackMemory;

		// �A�j���[�V�����Đ�
		owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Doble);

		// �G�t�F�N�g����
		//areWork = std::make_unique<Effect>("Data/Effect/tornade.efk");

		// �G�t�F�N�g�Đ�
		//areWork->Play(transformid->GetPosition());
	}
	else
	{
		// �Đ��J�n���� 
		float currentAnimationStartSeconds = 0.0f;

		// ���[�v
		bool loop = false;

		// �Đ����ԉ��Z���̒l
		float currentAnimationAddSeconds = 0.03f;


		// �A�j���[�V�����Đ�
		model->PlayAnimation(
			Player::Anim_Slash, loop,
			currentAnimationStartSeconds, blendSeconds
			, currentAnimationAddSeconds
		);
		// �U���O��ň�x�����I��
		++attackMemory;

		// �A�j���[�V�����Đ�
		owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);
	}
	

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
	owner.lock()->GetComponent<Player>()->UpdateCameraState(elapsedTime);



	if (owner.lock()->GetComponent<Player>()->InputAttack() && 
		owner.lock()->GetComponent<Player>()->GetSelectCheck() ==
		(int)Player::CommandAttack::Attack)
	{
		// �R�}���h�m�F2�e�U��
		if (gamePad.ConfirmCommand(commandSeconde, frame) && !button && !buttonSeconde)
		{
			button = true;
			//owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
		}
	}

	// ���
	if (owner.lock()->GetComponent<Player>()->InputAvoidance() && owner.lock()->GetComponent<Player>()->InputMove(elapsedTime))
	{
		bool stop = false;
		moveid->SetStopMove(stop);

		DirectX::XMFLOAT3 vec;
		vec = owner.lock()->GetComponent<Player>()->GetMoveVec(elapsedTime);

		moveid->Turn(vec, turnSpeed,elapsedTime);

		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

		button = false;
		buttonSeconde = false;

		return;
	}

	DirectX::XMVECTOR Vector;
	DirectX::XMVECTOR LengthSq;

	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&owner.lock()->GetComponent<Transform>()->GetPosition());
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

		//moveid->Move(direction, attackSpeed, elapsedTime);
	}
	else if (!moveid->GetOnLadius())
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
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


		// �U���ő�l����
		attackMemory = 0;
	}


	
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = moveid->GetOnLadius() ? 
		model->GetCurrentANimationSeconds() : 
		model->GetCurrentAnimationSecondsUpeer();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B


	// 1����
	if (animationTime >= 0.8f )
	{
		// �P��ڂ̍U���Ȃ�
		oneAttackCheck = false;

		owner.lock()->GetComponent<Player>()->SetAttackCollisionFlag(false);
		deleteCheck = false;
		// ���͊m�F�ŃX�e�[�g�ύX
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(button  ? Player::State::SideCut : Player::State::Move));
		return;
	}



	owner.lock()->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner.lock()->GetComponent<Player>()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");
}

void PlayerQuickJabState::Exit()
{
	// �ړ��̒�~
	moveid->SetGravity(
		owner.lock()->GetComponent<Player>()->GetGravity());


	// ��������
	bool stopMove = false;
	moveid->SetStopMove(stopMove);

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

void PlayerQuickJabState::End()
{
	// ���
	owner.lock().reset();
}


void PlayerSideCutState::Enter()
{
	bool loop = false;

	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	moveid = owner.lock()->GetComponent<Movement>();
	transformid = owner.lock()->GetComponent<Transform>();

	// ���

	if (!moveid->GetOnLadius())
	{
		// �Đ��J�n���� 
		float currentAnimationStartSeconds = 0.6f;
		float currentAnimationUpperStartSeconds = 0.0f;

		// ���[�v
		bool loop = true;
		bool loopUpper = false;

		// �Đ����ԉ��Z���̒l
		float currentAnimationAddSeconds = 0.03f;
		float currentAnimationUpperAddSeconds = 0.03f;

		float keyFrameEnd = 0.63f;

		// �A�j���[�V�����Đ�
		//model->GetModel()->PlayAnimation(
		//	Player::Anim_Jump, loop,
		//	currentAnimationStartSeconds, blendSeconds
		//	, currentAnimationAddSeconds, keyFrameEnd
		//);


		// �A�j���[�V�����㔼�g�Đ�
		model->PlayUpeerBodyAnimation(
			Player::Anim_SlashBeside, loopUpper,
			currentAnimationUpperStartSeconds, blendSeconds

		);
		// �U���O��ň�x�����I��
		++attackMemory;

		// �A�j���[�V�����Đ�
		owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Doble);
	}
	else
	{
		// �Đ��J�n���� 
		float currentAnimationStartSeconds = 0.2f;

		// ���[�v
		bool loop = false;

		// �Đ����ԉ��Z���̒l
		float currentAnimationAddSeconds = 0.03f;


		// �A�j���[�V�����Đ�
		model->PlayAnimation(
			Player::Anim_SlashBeside, loop,
			currentAnimationStartSeconds, blendSeconds
			, currentAnimationAddSeconds
		);
		// �U���O��ň�x�����I��
		++attackMemory;

		// �A�j���[�V�����Đ�
		owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);
	}
		//// �Đ��J�n
		//currentAnimationStartSeconds = 0.2f;

		//// �A�j���[�V�����Đ�
		//model->GetModel()->PlayAnimation(
		//	Player::Anim_SlashBeside, loop,
		//	currentAnimationStartSeconds, blendSeconds
		//	, currentAnimationAddSeconds
		//);

	

	if (!InitializationCheck)
	{

		// �R�}���h�ݒ���ڍU��
		commandSeconde.push_back(GamePad::BTN_B);


		// �R�}���h�ݒ�O��ڍU��
		commandThrede.push_back(GamePad::BTN_B);


	}
	// �A�j���[�V�����Đ�
	//owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


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
	owner.lock()->GetComponent<Player>()->UpdateCameraState(elapsedTime);



	//angle = transformid->GetAngle();

	//DirectX::XMFLOAT3 direction;
	//direction.x = sinf(angle.y) * 6;
	//direction.y = 0;
	//direction.z = cosf(angle.y) * 6;

	//moveid->Move(direction, attackSpeed, elapsedTime);

	if (owner.lock()->GetComponent<Player>()->InputAttack() &&
		owner.lock()->GetComponent<Player>()->GetSelectCheck() ==
		(int)Player::CommandAttack::Attack)
	{
		// �R�}���h�m�F3�e�U��
		if (gamePad.ConfirmCommand(commandThrede, frame))
		{
			buttonSeconde = true;
			//owner.lock()->GetComponent<Pl/ayer>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
		}

	}

	// ���
	if (owner.lock()->GetComponent<Player>()->InputAvoidance() && owner.lock()->GetComponent<Player>()->InputMove(elapsedTime))
	{
		bool stop = false;
		moveid->SetStopMove(stop);

		DirectX::XMFLOAT3 vec;
		vec = owner.lock()->GetComponent<Player>()->GetMoveVec(elapsedTime);

		moveid->Turn(vec, turnSpeed, elapsedTime);

		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

		button = false;
		buttonSeconde = false;

		return;
	}


	DirectX::XMVECTOR Vector;
	DirectX::XMVECTOR LengthSq;

	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&owner.lock()->GetComponent<Transform>()->GetPosition());
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

		//moveid->Move(direction, attackSpeed, elapsedTime);
	}
	else if (!moveid->GetOnLadius())
	{
	
		bool stop = true;
		moveid->SetStopMove(stop);

	}

	// ��]
	if (!rotateCheck && !owner.lock()->GetComponent<Player>()->InputAvoidance())
	{
		//DirectX::XMVECTOR Vector;
		//DirectX::XMVECTOR LengthSq;

		//DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&owner.lock()->GetComponent<Transform>()->GetPosition());
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
	float animationTime = moveid->GetOnLadius() ?
		model->GetCurrentANimationSeconds() :
		model->GetCurrentAnimationSecondsUpeer();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B

	//owner.lock()->GetComponent<Player>()->InputAvoidance();

	// 2����
	if (animationTime >= 1.1f)
	{
		owner.lock()->GetComponent<Player>()->SetAttackCollisionFlag(false);
		deleteCheck = false;



		// ���͊m�F�ŃX�e�[�g�ύX
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(buttonSeconde ? Player::State::CycloneStrike : Player::State::Move));

		return;
	}


	owner.lock()->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner.lock()->GetComponent<Player>()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");

}

void PlayerSideCutState::Exit()
{
	// �ړ��̒�~
	moveid->SetGravity(
		owner.lock()->GetComponent<Player>()->GetGravity());


	// ��������
	bool stopMove = false;
	moveid->SetStopMove(stopMove);

	// �d�̓I�t
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
	
}

void PlayerSideCutState::End()
{
	// ���
	owner.lock().reset();
}


void PlayerCycloneStrikeState::Enter()
{
	bool loop = false;

	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	moveid = owner.lock()->GetComponent<Movement>();
	transformid = owner.lock()->GetComponent<Transform>();


	if (!moveid->GetOnLadius())
	{
		// �Đ��J�n���� 
		float currentAnimationStartSeconds = 0.6f;
		float currentAnimationUpperStartSeconds = 0.0f;

		// ���[�v
		bool loop = true;
		bool loopUpper = false;

		// �Đ����ԉ��Z���̒l
		float currentAnimationAddSeconds = 0.03f;
		float currentAnimationUpperAddSeconds = 0.03f;

		float keyFrameEnd = 0.63f;

		// �A�j���[�V�����Đ�
		//model->GetModel()->PlayAnimation(
		//	Player::Anim_Jump, loop,
		//	currentAnimationStartSeconds, blendSeconds
		//	, currentAnimationAddSeconds, keyFrameEnd
		//);


		// �A�j���[�V�����㔼�g�Đ�
		model->PlayUpeerBodyAnimation(
			Player::Anim_SlashThree, loopUpper,
			currentAnimationUpperStartSeconds, blendSeconds

		);
		// �U���O��ň�x�����I��
		++attackMemory;

		// �A�j���[�V�����Đ�
		owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Doble);
	}
	else
	{
		// �Đ��J�n���� 
		float currentAnimationStartSeconds = 0.7f;

		// ���[�v
		bool loop = false;

		// �Đ����ԉ��Z���̒l
		float currentAnimationAddSeconds = 0.03f;


		// �A�j���[�V�����Đ�
		model->PlayAnimation(
			Player::Anim_SlashThree, loop,
			currentAnimationStartSeconds, blendSeconds
			, currentAnimationAddSeconds
		);
		// �U���O��ň�x�����I��
		++attackMemory;

		// �A�j���[�V�����Đ�
		owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);
	}
		//// �Đ��J�n
		//currentAnimationStartSeconds = 0.7f;
		//// �A�j���[�V�����Đ�
		//model->GetModel()->PlayAnimation(
		//	Player::Anim_SlashThree, loop,
		//	currentAnimationStartSeconds, blendSeconds
		//	, currentAnimationAddSeconds
		//);



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
	//owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

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
	owner.lock()->GetComponent<Player>()->UpdateCameraState(elapsedTime);


	//angle = transformid->GetAngle();

	//DirectX::XMFLOAT3 direction;
	//direction.x = sinf(angle.y) * 6;
	//direction.y = 0;
	//direction.z = cosf(angle.y) * 6;

	//moveid->Move(direction, attackSpeed, elapsedTime);

	// ���
	if (owner.lock()->GetComponent<Player>()->InputAvoidance())
	{
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

		button = false;
		buttonSeconde = false;
	}


	DirectX::XMVECTOR Vector;
	DirectX::XMVECTOR LengthSq;

	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&owner.lock()->GetComponent<Transform>()->GetPosition());
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

		//moveid->Move(direction, attackSpeed, elapsedTime);
	}
	else if (!moveid->GetOnLadius())
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
	float animationTime = moveid->GetOnLadius() ?
		model->GetCurrentANimationSeconds() :
		model->GetCurrentAnimationSecondsUpeer();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B


	// 3����
	if (animationTime >= 2.7f )
	{
		owner.lock()->GetComponent<Player>()->SetAttackCollisionFlag(false);
		deleteCheck = false;



		// ���͊m�F�ŃX�e�[�g�ύX
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
		return;
	}


	owner.lock()->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner.lock()->GetComponent<Player>()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");

}

void PlayerCycloneStrikeState::Exit()
{
	// �ړ��̒�~
	moveid->SetGravity(
		owner.lock()->GetComponent<Player>()->GetGravity());



	// ��������
	bool stopMove = false;
	moveid->SetStopMove(stopMove);

	// �d�̓I�t
	bool stopFall = false;
	moveid->SetStopFall(stopFall);

}

void PlayerCycloneStrikeState::End()
{
	// ���
	owner.lock().reset();
}


void PlayerSpecialAttackState::Enter()
{

	transformid = owner.lock()->GetComponent<Transform>();
	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	moveid = owner.lock()->GetComponent<Movement>();



	enemyTransform = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<Transform>();
	enemyHpId = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<HP>();

	lightning = std::make_unique<Effect>("Data/Effect/sunder.efk");
	lightningAttack = std::make_unique<Effect>("Data/Effect/HitThunder.efk");
	lightningHit = std::make_unique<Effect>("Data/Effect/lightningStrike.efk");




	if (!moveid->GetOnLadius())
	{
		// �Đ��J�n���� 
		float currentAnimationStartSeconds = 0.6f;
		float currentAnimationUpperStartSeconds = 0.0f;

		// ���[�v
		bool loop = true;
		bool loopUpper = false;

		// �Đ����ԉ��Z���̒l
		float currentAnimationAddSeconds = 0.00f;


		// �A�j���[�V�����u�����h
		float blendSeconds = 0.5f;

		float keyFrameEnd = 0.63f;

		// �A�j���[�V�����Đ�
		model->PlayAnimation(
			Player::Anim_Jump, loop,
			currentAnimationStartSeconds, blendSeconds
			, currentAnimationAddSeconds, keyFrameEnd
		);


		// �A�j���[�V�����㔼�g�Đ�
		model->PlayUpeerBodyAnimation(
			Player::Anim_MagicSeconde, loopUpper,
			currentAnimationUpperStartSeconds, blendSeconds

		);

		// �A�j���[�V�����㔼�g�����g�Đ�
		owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Doble);
	}
	else
	{
		// �Đ����[�v
		bool  loop = false;

		// �Đ��J�n���� 
		float currentAnimationStartSeconds = 0.0f;


		// �A�j���[�V�����u�����h
		float blendSeconds = 0.5f;

		// �A�j���[�V�����Đ�
		model->PlayAnimation(
			Player::Anim_MagicSeconde, loop,
			currentAnimationStartSeconds, blendSeconds
			
		);

		// �A�j���[�V�����Đ�
		owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

	}

	Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");

	DirectX::XMFLOAT3 pPosition =
	{
				pHPosiiton->worldTransform._41,
				pHPosiiton->worldTransform._42,
				pHPosiiton->worldTransform._43
	};

	lightning->Play(pPosition);

	// ������̒�~
	bool stopFall = true;
	moveid->SetStopFall(stopFall);

	button = true;

	// �t���b�V��
	flashOn = true;



	MessageData::CAMERACHANGEMOTIONMODEDATA	p;

	position = transformid->GetPosition();
	angle = transformid->GetAngle();

	float vx = sinf(angle.y) * 6;
	float vz = cosf(angle.y) * 6;

	float vx2 = sinf(angle.y) - 10;
	float vz2 = cosf(angle.y) * 7;
	float vx3 = sinf(angle.y);

	p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
	p.data.push_back({ 50, {position.x + vx, position.y + 3, position.z + vz }, position });
	p.data.push_back({ 100, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
	p.data.push_back({ 140, {position.x + vx3 , position.y + 1, (position.z + 0.1f) - vz2 }, position });

	// �G�l�~�[�Ăԓz
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyManagerCount = enemyManager.GetEnemyCount();

	DirectX::XMFLOAT3 pos;
	DirectX::XMVECTOR posVec;


	pos = 
	{0,0,0};
	float length;
	float endLength = 8.0f;

	length = 10.0f;

	//�@�J�����ړ��@�x�N�g���l��
	if (enemyManagerCount > 0)
	{

		std::shared_ptr<EnemySlime> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemySlime>();

		DirectX::XMVECTOR positionVec = DirectX::XMLoadFloat3(&position);
		DirectX::XMVECTOR ePositionVec = DirectX::XMLoadFloat3(&enemy->GetPosition());

		posVec = DirectX::XMVectorSubtract(positionVec , ePositionVec);

		posVec = DirectX::XMVector3Normalize(posVec);

		DirectX::XMStoreFloat3(&pos, posVec);

	}



	p.data.push_back({ 190, {position.x + ( pos.x * length) ,
		position.y + (pos.y* length) + 1,
		position.z +  (pos.z *  length) }, position });


	//p.data.push_back({ 180, {position.x + (pos.x * length) ,
	//position.y + (pos.y * endLength) + 1,
	//position.z + (pos.z * endLength) }, position });

	//p.data.push_back({ 180, position, {(position.x * pos.x) * length ,
	//	(position.y * pos.y) * length ,
	//	(position.z * pos.z) * length } });



	Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);



	//// ����
	//currentAnimationAddSeconds = 0.0f;
}

void PlayerSpecialAttackState::Execute(float elapsedTime)
{
	
	if (button)
	{




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

		//float vx = sinf(angle.y) * 6;
		//float vz = cosf(angle.y) * 6;

		//float vx2 = sinf(angle.y) - 10;
		//float vz2 = cosf(angle.y) * 7;
		//float vx3 = sinf(angle.y);

		//p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
	 //   p.data.push_back({ 50, {position.x + vx, position.y + 3, position.z + vz }, position });
		//p.data.push_back({ 100, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
		//p.data.push_back({ 140, {position.x + vx3 , position.y + 1, (position.z + 0.1f) - vz2 }, position });

		//// �G�l�~�[�Ăԓz
		//EnemyManager& enemyManager = EnemyManager::Instance();
		//int enemyManagerCount = enemyManager.GetEnemyCount();

		//// ���삳���邩�ǂ���
		//if (enemyManagerCount > 0)
		//{

		//	std::shared_ptr<EnemySlime> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemySlime>();
		//	std::shared_ptr<Movement> enemyMove = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<Movement>();

		//	bool moveCheck = true;
		//	enemy->SetMoveCheck(moveCheck);

		//	// ���x��~
		//	bool stopVelocity = false;
		//	enemyMove->SetStopMove(stopVelocity);
		//	// ������̒�~
		//	bool stopFall = false;
		//	enemyMove->SetStopFall(stopFall);
		//}

		//DirectX::XMFLOAT3 pos;

		//pos = 
		//{
		//	position.x
		//};

		//p.data.push_back({ 200, {position.x + vx3 , position.y + 0.5f, (position.z + 0.1f) - vz2 }, position });



		//Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

					// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
		float animationTime = moveid->GetOnLadius() ?
			model->GetCurrentANimationSeconds() :
			model->GetCurrentAnimationSecondsUpeer();

		// �A�j���[�V����
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON)
		{

			if (!moveid->GetOnLadius())
			{
				// �Đ��J�n���� 
				float currentAnimationStartSeconds = 0.6f;
				float currentAnimationUpperStartSeconds = 0.0f;

				// ���[�v
				bool loop = true;
				bool loopUpper = false;

				// �Đ����ԉ��Z���̒l
				float currentAnimationAddSeconds = 0.00f;


				// �A�j���[�V�����u�����h
				float blendSeconds = 0.5f;

				float keyFrameEnd = 0.63f;

				// �A�j���[�V�����Đ�
				model->PlayAnimation(
					Player::Anim_Jump, loop,
					currentAnimationStartSeconds, blendSeconds
					, currentAnimationAddSeconds, keyFrameEnd
				);


				// �A�j���[�V�����㔼�g�Đ�
				model->PlayUpeerBodyAnimation(
					Player::Anim_SpecialAttack, loopUpper,
					currentAnimationUpperStartSeconds, blendSeconds

				);


			}
			else
			{
				// �Đ����[�v
				bool  loop = false;

				// �Đ��J�n���� 
				float currentAnimationStartSeconds = 0.0f;
				// �Đ����ԉ��Z���̒l
				float currentAnimationAddSeconds = 0.00f;

				// �A�j���[�V�����u�����h
				float blendSeconds = 0.5f;


				// �A�j���[�V�����Đ�
				model->PlayAnimation(
					Player::Anim_SpecialAttack, loop,
					currentAnimationStartSeconds, blendSeconds
					, currentAnimationAddSeconds
				);
			}
			button = false;


			lightning->Stop(lightning->GetEfeHandle());


			//rockCheck = true;
			//owner.lock()->GetComponent<Player>()->SetRockCheck(rockCheck);


			
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
		//owner.lock()->GetComponent<Player>()->SetRockCheck(rockCheck);

		//owner.lock()->GetComponent<Player>()->UpdateCameraState(elapsedTime);

		// �t���[�J�����̃��b�N���O��
		//bool freeCameraCheck = false;
		//owner.lock()->GetComponent<Player>()->SetFreeCameraCheck(freeCameraCheck);

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

		//owner.lock()->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner.lock()->GetComponent<Player>()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");

		/*lightningAttack->SetPosition(lightningAttack->GetEfeHandle(), model->GetModel()->FindNode("mixamorig:LeftHand")->position);*/

			// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
			float animationTime = moveid->GetOnLadius() ?
				model->GetCurrentANimationSeconds() :
				model->GetCurrentAnimationSecondsUpeer();

		// �X���[���[�V�����ƐF�𖾂邭
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON)
		{
			owner.lock()->GetComponent<Player>()->SetFlashOn(flashOn);
			owner.lock()->GetComponent<Player>()->SetHitCheck(true);
		}
		// �A�j���[�V�����؂��
		if (animationTime >= 1.6f)
		{
			/*float invincibleTimer = 0.0f;
			owner.lock()->GetComponent<HP>()->SetInvincibleTimer(invincibleTimer);*/




			// �J�����U��
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
			


			if (enemyHpId->ApplyDamage(10, 0.5f)) 
			{
				
				lightningHit->Play(pPosition);
			}
			owner.lock()->GetComponent<Player>()->SetHitCheck(false);

			//owner.lock()->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner.lock()->GetComponent<Player>()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");
		}
		bool animationEnd = moveid->GetOnLadius() ?
			model->IsPlayAnimation() :
			model->IsPlayUpeerBodyAnimation();

		// �A�j���[�V�����I��
		if (!animationEnd)
		{
			// ���͊m�F�ŃX�e�[�g�ύX
			owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));

		}
	}

}

void PlayerSpecialAttackState::Exit()
{

	// ������̍ĊJ
	bool stopFall = false;
	moveid->SetStopFall(stopFall);

	// �G�l�~�[�Ăԓz
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyManagerCount = enemyManager.GetEnemyCount();

	// ���삳���邩�ǂ���
	if (enemyManagerCount > 0)
	{

		std::shared_ptr<EnemySlime> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemySlime>();
		std::shared_ptr<Movement> enemyMove = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<Movement>();

		bool moveCheck = true;
		enemy->SetMoveCheck(moveCheck);

		// ���x��~
		bool stopVelocity = false;
		enemyMove->SetStopMove(stopVelocity);
		// ������̒�~
		bool stopFall = false;
		enemyMove->SetStopFall(stopFall);
	}

	//rockCheck = false;
	//owner.lock()->GetComponent<Player>()->SetRockCheck(rockCheck);

	//// �t���[�J�����̃��b�N���Ƃǂ߂�
	//bool freeCameraCheck = true;
	//owner.lock()->GetComponent<Player>()->SetFreeCameraCheck(freeCameraCheck);

}

void PlayerSpecialAttackState::End()
{
	// ���
	owner.lock().reset();
}

void PlayerMagicState::Enter()
{

	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	moveid = owner.lock()->GetComponent<Movement>();


	// �T���_�[�n
	if (owner.lock()->GetComponent<Player>()->GetSelectMagicCheck() ==
		(int)Player::CommandMagic::Thander)
	{
		// �A�j���[�V�����Đ�
		model->PlayAnimation(
			Player::Anim_MagicSeconde, loop,
			currentAnimationStartSeconds, blendSeconds
		);


	}
	else
	{
		// �A�j���[�V�����Đ�
		model->PlayAnimation(
			Player::Anim_Magic, loop,
			currentAnimationStartSeconds, blendSeconds
		);
	}

	// �A�j���[�V�����Đ�
	owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


	magicType = owner.lock()->GetComponent<Player>()->GetSelectMagicCheck();
	// ������̒�~
	bool stopFall = true;
	moveid->SetStopFall(stopFall);



}

void PlayerMagicState::Execute(float elapsedTime)
{

	// ���b�N�I������
	owner.lock()->GetComponent<Player>()->UpdateCameraState(elapsedTime);

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();
	// �A�j���[�V�����I�������ʂ�



	// ���@�̎�ނŏI�����A��B
	switch (magicType)
	{
	case (int)Player::CommandMagic::Fire:
	{
		// ����
		if (animationTime <= 0.5f)return;
		// ������
		owner.lock()->GetComponent<Player>()->InputMagicframe();

		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Idle);
		break;
	}

	case (int)Player::CommandMagic::Thander:
	{
		// ����
		if (animationTime <= 1.1f)return;

		// ������
		owner.lock()->GetComponent<Player>()->InputMagicLightning();
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Idle);
		break;
	}
	case (int)Player::CommandMagic::Ice:
	{
		// ����
		if (animationTime <= 0.5f)return;
		// �X����
		owner.lock()->GetComponent<Player>()->InputMagicIce();
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Idle);
		break;
	}

	default:
		break;
	}

}

void PlayerMagicState::Exit()
{
	// ���@�̑I�����[����
	owner.lock()->GetComponent<Player>()->SetSelectCheck((int)Player::CommandMagic::Normal);

	// ������̍ĊJ
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
}

void PlayerMagicState::End()
{
	// ���
	owner.lock().reset();
}

void PlayerSpecialMagicState::Enter()
{

	transformid = owner.lock()->GetComponent<Transform>();
	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	moveid = owner.lock()->GetComponent<Movement>();

	fire = std::make_unique<Effect>("Data/Effect/fire.efk");
	fireAttack = std::make_unique<Effect>("Data/Effect/hit fire.efk");
	// �Đ��J�n����
	currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����Đ�
	model->PlayAnimation(
		Player::Anim_SlashThree, loop,
		currentAnimationStartSeconds, blendSeconds
		, currentAnimationAddSeconds
	);
	// �A�j���[�V�����Đ�
	owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


	Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");

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

	// �ŏ��������@�𔭓����邽��
	startMagic = false;




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

	// �G�l�~�[�Ăԓz
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyManagerCount = enemyManager.GetEnemyCount();

	DirectX::XMFLOAT3 pos;
	DirectX::XMVECTOR posVec;


	pos =
	{ 0,0,0 };
	float length;

	length = 10.0f;

	//�@�J�����ړ��@�x�N�g���l��
	if (enemyManagerCount > 0)
	{

		std::shared_ptr<EnemySlime> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemySlime>();

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

void PlayerSpecialMagicState::Execute(float elapsedTime)
{
	if (button)
	{

		//// �J�������[�V����
		//MessageData::CAMERACHANGEMOTIONMODEDATA	p;

		//position = transformid->GetPosition();
		//angle = transformid->GetAngle();


		//// ���[�V�����L�^
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

		//// ���[�V�����ݒ�
		//Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

		// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
		//float animationTime = model->GetModel()->GetCurrentANimationSeconds();
		
		// �G�t�F�N�g�X�V
		if(fire->GetEfeHandle())
		{
			Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");

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
		if (!model->IsPlayAnimation())
		{
			// �Đ��J�n����
			currentAnimationStartSeconds = 0.0f;

			// �A�j���[�V�����Đ�
			model->PlayAnimation(
				Player::Anim_MagicSeconde, loop,
				currentAnimationStartSeconds, blendSeconds
				, currentAnimationAddSeconds
			);
			button = false;

			// ��̉��G�t�F�N�g��~
			fire->Stop(fire->GetEfeHandle());


			//rockCheck = true;
			//owner.lock()->GetComponent<Player>()->SetRockCheck(rockCheck);


			
		}
	}
	else
	{
			// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
		float animationTime = model->GetCurrentANimationSeconds();

		// �A�j���[�V����
		if (animationTime >= 1.1f)
		{

			owner.lock()->GetComponent<Player>()->SetFlashOn(true);
			//owner.lock()->GetComponent<Player>()->SetHitCheck(true);

			// �A�j���[�V������~
			owner.lock()->GetComponent<Player>()->SetUpdateAnim
			(Player::UpAnim::Stop);


			// �J�����U��
			MessageData::CAMERASHAKEDATA cameraShakeData;

			// �J�����U���l
			float shakeTimer = 0.5f;
			float shakePower = 0.8f;
			cameraShakeData = { shakeTimer , shakePower };
			// �J�����U���ݒ�
			Messenger::Instance().SendData(MessageData::CAMERASHAKE, &cameraShakeData);

			Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");

			DirectX::XMFLOAT3 pPosition =
			{
						pHPosiiton->worldTransform._41,
						pHPosiiton->worldTransform._42,
						pHPosiiton->worldTransform._43
			};



			// ���@���˂̃^�C�~���O
			if (!startMagic)
			owner.lock()->GetComponent<Player>()->
				InputSpecialMagicframe();

			// �ŏ��������@�𔭓����邽��
			startMagic = true;


			// �K�E�Z���������̌o�ߎ���
			specialMoveWaitTime += elapsedTime;



		}
	}



	// �_���[�W����
	owner.lock()->GetComponent<Player>()->SpecialApplyDamageInRadius();



	// �ҋ@���Ԃ��߂����̂Ŏ��̃X�e�[�g��
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

		// ���̃X�e�[�g��
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


		return;
	}

	


}

void PlayerSpecialMagicState::Exit()
{
	// ������̍ĊJ
	bool stopFall = false;
	moveid->SetStopFall(stopFall);


	// �G�l�~�[�Ăԓz
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyManagerCount = enemyManager.GetEnemyCount();


	// ���삳���邩�ǂ���
	if (enemyManagerCount > 0)
	{

		std::shared_ptr<EnemySlime> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemySlime>();
		std::shared_ptr<Movement> enemyMove = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<Movement>();

		bool moveCheck = true;
		enemy->SetMoveCheck(moveCheck);

		// ���x��~
		bool stopVelocity = false;
		enemyMove->SetStopMove(stopVelocity);
		// ������̒�~
		bool stopFall = false;
		enemyMove->SetStopFall(stopFall);
	}
}

void PlayerSpecialMagicState::End()
{
	// ���
	owner.lock().reset();
}


void PlayerSpecialMagicIceState::Enter()
{

	transformid = owner.lock()->GetComponent<Transform>();
	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	moveid = owner.lock()->GetComponent<Movement>();

	ice = std::make_unique<Effect>("Data/Effect/brezerd.efk");
	//fireAttack = std::make_unique<Effect>("Data/Effect/hit fire.efk");
	// �Đ��J�n����
	currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����Đ�
	model->PlayAnimation(
		Player::Anim_SlashThree, loop,
		currentAnimationStartSeconds, blendSeconds
		, currentAnimationAddSeconds
	);
	// �A�j���[�V�����Đ�
	owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


	Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");

	DirectX::XMFLOAT3 pPosition =
	{
				pHPosiiton->worldTransform._41,
				pHPosiiton->worldTransform._42,
				pHPosiiton->worldTransform._43
	};

	ice->Play(pPosition);
	// ������̒�~
	bool stopFall = true;
	moveid->SetStopFall(stopFall);

	button = true;

	// �X�^�[�g�l
	specialMoveWaitTime = specialMoveWaitStartTime;

	// �ŏ��������@�𔭓����邽��
	startMagic = false;


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
	

	// �G�l�~�[�Ăԓz
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyManagerCount = enemyManager.GetEnemyCount();

	DirectX::XMFLOAT3 pos;
	DirectX::XMVECTOR posVec;


	pos =
	{ 0,0,0 };
	float length;

	length = 10.0f;

	//�@�J�����ړ��@�x�N�g���l��
	if (enemyManagerCount > 0)
	{

		std::shared_ptr<EnemySlime> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemySlime>();

		DirectX::XMVECTOR positionVec = DirectX::XMLoadFloat3(&position);
		DirectX::XMVECTOR ePositionVec = DirectX::XMLoadFloat3(&enemy->GetPosition());

		posVec = DirectX::XMVectorSubtract(positionVec, ePositionVec);

		posVec = DirectX::XMVector3Normalize(posVec);

		DirectX::XMStoreFloat3(&pos, posVec);

	}


	p.data.push_back({ 300, {position.x + (pos.x * length) ,
		position.y + (pos.y * length) + 1,
		position.z + (pos.z * length) }, position });
}

void PlayerSpecialMagicIceState::Execute(float elapsedTime)
{
	if (button)
	{

		//// �J�������[�V����
		//MessageData::CAMERACHANGEMOTIONMODEDATA	p;

		//position = transformid->GetPosition();
		//angle = transformid->GetAngle();


		//// ���[�V�����L�^
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

		//// ���[�V�����ݒ�
		//Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

		// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
		//float animationTime = model->GetModel()->GetCurrentANimationSeconds();

		// �G�t�F�N�g�X�V
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



		// �A�j���[�V����
		if (!model->IsPlayAnimation())
		{
			// �Đ��J�n����
			currentAnimationStartSeconds = 0.0f;

			// �A�j���[�V�����Đ�
			model->PlayAnimation(
				Player::Anim_MagicSeconde, loop,
				currentAnimationStartSeconds, blendSeconds
				, currentAnimationAddSeconds
			);
			button = false;

			// ��̉��G�t�F�N�g��~
			ice->Stop(ice->GetEfeHandle());


			//rockCheck = true;
			//owner.lock()->GetComponent<Player>()->SetRockCheck(rockCheck);



		}
	}
	else
	{
		// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
		float animationTime = model->GetCurrentANimationSeconds();

		// �A�j���[�V����
		if (animationTime >= 1.1f)
		{

			owner.lock()->GetComponent<Player>()->SetFlashOn(true);
			//owner.lock()->GetComponent<Player>()->SetHitCheck(true);

			// �A�j���[�V������~
			owner.lock()->GetComponent<Player>()->SetUpdateAnim
			(Player::UpAnim::Stop);


			// �J�����U��
			MessageData::CAMERASHAKEDATA cameraShakeData;

			// �J�����U���l
			float shakeTimer = 0.5f;
			float shakePower = 0.8f;
			cameraShakeData = { shakeTimer , shakePower };
			// �J�����U���ݒ�
			Messenger::Instance().SendData(MessageData::CAMERASHAKE, &cameraShakeData);

			Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");

			DirectX::XMFLOAT3 pPosition =
			{
						pHPosiiton->worldTransform._41,
						pHPosiiton->worldTransform._42,
						pHPosiiton->worldTransform._43
			};



			// ���@���˂̃^�C�~���O
			if (!startMagic)
				owner.lock()->GetComponent<Player>()->
				InputSpecialMagicframe();

			// �ŏ��������@�𔭓����邽��
			startMagic = true;


			// �K�E�Z���������̌o�ߎ���
			specialMoveWaitTime += elapsedTime;



		}
	}



	// �_���[�W����
	owner.lock()->GetComponent<Player>()->SpecialApplyDamageInRadius();



	// �ҋ@���Ԃ��߂����̂Ŏ��̃X�e�[�g��
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

		// ���̃X�e�[�g��
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


		return;
	}
}

void PlayerSpecialMagicIceState::Exit()
{
	// ������̍ĊJ
	bool stopFall = false;
	moveid->SetStopFall(stopFall);

	// �G�l�~�[�Ăԓz
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyManagerCount = enemyManager.GetEnemyCount();

	// ���삳���邩�ǂ���
	if (enemyManagerCount > 0)
	{

		std::shared_ptr<EnemySlime> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemySlime>();
		std::shared_ptr<Movement> enemyMove = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<Movement>();

		bool moveCheck = true;
		enemy->SetMoveCheck(moveCheck);

		// ���x��~
		bool stopVelocity = false;
		enemyMove->SetStopMove(stopVelocity);
		// ������̒�~
		bool stopFall = false;
		enemyMove->SetStopFall(stopFall);
	}
}

void PlayerSpecialMagicIceState::End()
{
	// ���
	owner.lock().reset();
}


void PlayerSpecialThanderMagicState::Enter()
{
}

void PlayerSpecialThanderMagicState::Execute(float elapsedTime)
{
}

void PlayerSpecialThanderMagicState::Exit()
{
	// ������̍ĊJ
	bool stopFall = false;
	moveid->SetStopFall(stopFall);

	// �G�l�~�[�Ăԓz
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyManagerCount = enemyManager.GetEnemyCount();

	// ���삳���邩�ǂ���
	if (enemyManagerCount > 0)
	{

		std::shared_ptr<EnemySlime> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemySlime>();
		std::shared_ptr<Movement> enemyMove = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<Movement>();

		bool moveCheck = true;
		enemy->SetMoveCheck(moveCheck);

		// ���x��~
		bool stopVelocity = false;
		enemyMove->SetStopMove(stopVelocity);
		// ������̒�~
		bool stopFall = false;
		enemyMove->SetStopFall(stopFall);
	}
}

void PlayerSpecialThanderMagicState::End()
{
	// ���
	owner.lock().reset();
}


void PlayerDamageState::Enter()
{
	model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	bool loop = false;

	bool stopMove = false;
	owner.lock()->GetComponent<Movement>()->SetStopMove(stopMove);
	bool stopFall = false;
	owner.lock()->GetComponent<Movement>()->SetStopFall(stopFall);

		//Player::Anim_Pain, loop,
	model->PlayAnimation(
		Player::Anim_Pain, loop,
		currentAnimationStartSeconds, blendSeconds);

	owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerDamageState::Execute(float elapsedTime)
{
	// ���b�N�I������
	owner.lock()->GetComponent<Player>()->UpdateCameraState(elapsedTime);

	if (!model->IsPlayAnimation())
	{
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));

	}
}

void PlayerDamageState::Exit()
{
}

void PlayerDamageState::End()
{
	// ���
	owner.lock().reset();
}

void PlayerDeathState::Enter()
{
	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	hpid = owner.lock()->GetComponent<HP>();
	transformid = owner.lock()->GetComponent<Transform>();
		//Player::Anim_Deth, loop,
	model->PlayAnimation(
		Player::Anim_Deth, loop,
		currentAnimationStartSeconds ,blendSeconds
	);

	owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

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
	float animationTime = owner.lock()->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B
	//bool CollisionFlag = animationTime >= 1.5f && animationTime <= 1.6f;
	bool CollisionFlag = animationTime >= 0.5f && animationTime <= 1.6f;
	if (CollisionFlag)
	{
		++stateTimer;
		
		owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Stop);


	}


}

void PlayerDeathState::Exit()
{
}

void PlayerDeathState::End()
{
	// ���
	owner.lock().reset();
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

void PlayerReviveState::End()
{
	// ���
	owner.lock().reset();
}

// ���������
void PlayerAvoidanceState::Enter()
{
	transformid = owner.lock()->GetComponent<Transform>();
	moveid = owner.lock()->GetComponent<Movement>();
	hpid = owner.lock()->GetComponent<HP>();

	//Player::Anim_Slash, loop
	owner.lock()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Dush, loop
		, currentAnimationStartSeconds, blendSeconds,
		currentAnimationAddSeconds);


	// �����蔻��̗L��
	owner.lock()->GetComponent<Player>()->DmageInvalidJudment(false);



	// �A�j���[�V������� �ʏ�
	owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);

	// ������̒�~
	bool stopFall = true;
	moveid->SetStopFall(stopFall);


	// �G�t�F�N�g�ݒ�
	wind = std::make_unique<Effect>("Data/Effect/dashu.efk");

	// �G�t�F�N�g�Đ�����
	wind->Play({ 
		transformid->GetPosition().x,
		transformid->GetPosition().y + addHeight,
		transformid->GetPosition().z
		});


	// �p�x�ύX
	wind->SetAngle(wind->GetEfeHandle(), transformid->GetAngle());
}

// ����X�V
void PlayerAvoidanceState::Execute(float elapsedTime)
{
	// �������R
	bool stopMove = false;
	moveid->SetStopMove(stopMove);

	// ���b�N�I������
	owner.lock()->GetComponent<Player>()->UpdateCameraState(elapsedTime);

	// �]����_�b�V��
	DirectX::XMFLOAT3 dir;
	dir.x = sinf(transformid->GetAngle().y);
	dir.y = cosf(transformid->GetAngle().x);
	dir.z = cosf(transformid->GetAngle().y);

	
	// �C�ӂ̃A�j���[�V�����Đ����
	float animationTime = owner.lock()->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	
	// �ʒu�X�V
	wind->SetPosition(wind->GetEfeHandle(),
		{
			transformid->GetPosition().x,
			transformid->GetPosition().y + addHeight,
			transformid->GetPosition().z
		});

	//if (animationTime + FLT_EPSILON >= 0.7f + FLT_EPSILON && animationTime <= 0.71f)
	//if (animationTime + FLT_EPSILON >= 0.7f + FLT_EPSILON && animationTime <= 0.71f)

	
	// �n��_�b�V��
	if (animationTime >= 0.7f && animationTime <= 0.8f && moveid->GetOnLadius())
	{
		DirectX::XMFLOAT3 impulse =
		{
			dir.x * speed,
			0,
			dir.z * speed,
		};

		moveid->AddImpulse(impulse);

		//moveid->Move(dir, moveSpeed, elapsedTime);
	}
	// �󒆃_�b�V��
	if (animationTime >= 0.7f  && animationTime <= 0.8f && !moveid->GetOnLadius())
	{
		//moveid->Move(dir, moveSpeed, elapsedTime);

		DirectX::XMFLOAT3 impulse =
		{
			dir.x * flySpeed,
			0,
			dir.z * flySpeed,
		};
	
		moveid->AddImpulse(impulse);
		
	}

	
	// �A�j���[�V�����I��
	//if (!owner.lock()->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	if (animationTime >= 2.0f)
	{

		// �����蔻��̗L��
		owner.lock()->GetComponent<Player>()->DmageInvalidJudment(true);
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));



	}
}

// ����I��
void PlayerAvoidanceState::Exit()
{
	// ������̒�~
	bool stopFall = false;
	moveid->SetStopFall(stopFall);

	wind->Stop(wind->GetEfeHandle());
}

void PlayerAvoidanceState::End()
{
	// ���
	owner.lock().reset();
}

// ���ˊJ�n
void PlayerReflectionState::Enter()
{
	bool loop = false;

		//Player::Anim_Counter, loop,
	owner.lock()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		Player::Anim_Counter, loop,
		currentAnimationStartSeconds, blendSeconds);

	// �A�j���[�V������� �ʏ�
	owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


}

// ���ˍX�V
void PlayerReflectionState::Execute(float elapsedTime)
{
	if (!owner.lock()->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		// �����蔻��̗L��
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));

	}


	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner.lock()->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B
	bool CollisionFlag = animationTime >= 0.2f && animationTime <= 0.4f;

	if (CollisionFlag)
	{
		// ����m�[�h�ƃG�l�~�[�̏Փˏ���
		owner.lock()->GetComponent<Player>()->CollisionNodeVsEnemiesCounter("mixamorig:LeftHand", owner.lock()->GetComponent<Player>()->GetLeftHandRadius());
		owner.lock()->GetComponent<Player>()->CollisionNodeVsRubyCounter("mixamorig:LeftHand", owner.lock()->GetComponent<Player>()->GetLeftHandRadius());
		owner.lock()->GetComponent<Player>()->CollisionNodeVsRubyCounterBulletFring("mixamorig:LeftHand", owner.lock()->GetComponent<Player>()->GetLeftHandRadius());
		
	}
}

// ���ˏI��
void PlayerReflectionState::Exit()
{
}

void PlayerReflectionState::End()
{
	// ���
	owner.lock().reset();
}


