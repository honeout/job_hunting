#include <math.h>
#include "EnemyBoss.h"
#include "StateDerived.h"

#include "Player.h"
#include "Mathf.h"

//#include "Audio\Audio.h"
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
	


	// owner�Ăяo��
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();


	//walkSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/Enemy���n.wav", "walk");
	//walkSe->Play("walk",loopSe);
	//float seSpeed = 0.5f;
	//walkSe->SetSpeed("walk",seSpeed);

	//moveid = owner.lock()->GetComponent<Movement>();
	// �꒣��
	enemyid.lock()->SetRandomTargetPosition();

	// �A�j���[�V�����Đ�
	model->
		PlayAnimation(EnemyBoss::Animation::Anim_Movie,
			loop, currentAnimationStartSeconds, blendSeconds,
			currentAnimationAddSeconds, currentAnimationEndSeconds);

	//model->
	//	PlayAnimation(EnemyBoss::Animation::Anim_Walk,
	//		loop, currentAnimationStartSeconds, blendSeconds, 
	//		currentAnimationAddSeconds);
	// �A�j���[�V�������[��
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);

	stateTimer = stateTimerMax;
	//stateTimer = 60.0f;

	// ���n�u��
	upOnLading = false;

	// �v���C���[id

	// ���G�t�F�N�g
	smorker = std::make_unique<Effect>("Data/Effect/smorkeDash.efk");


}

// �p�j�X�e�[�g�Ŏ��s���郁�\�b�h
void WanderState::Execute(float elapsedTime)
{
	stateTimer -= elapsedTime;

	// owner�Ăяo��
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();


	// �ڕW�n�_�����v���C���[�ʒu�ɐݒ�
	targetPosition = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();

	enemyid.lock()->SetTargetPosition(targetPosition);
	
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
	DirectX::XMVECTOR enemyPositionVec = DirectX::XMLoadFloat3(&enemyid.lock()->GetPosition());

	DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(targetPositionVec, enemyPositionVec);

	DirectX::XMFLOAT3 direction;

	DirectX::XMStoreFloat3(&direction, directionVec);

	

	// �ړI�n�֒�����
	//if (stateTimer <= stateTimerEnd)
	if (!model->IsPlayAnimation())
	{
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
	}
	// �ړ�
	if (animationTime <= 4.6f)
	{
		moveid.lock()->Turn(direction, turnSpeed, elapsedTime);

		moveid.lock()->Move(direction, moveSpeed, elapsedTime);

	}



	// �ړI�n�ɂ��̔��a��������
	float radius = enemyid.lock()->GetAttackRightFootRange();

	//moveid.lock()->Move(direction ,moveSpeed, elapsedTime);

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

		enemyid.lock()->DetectHitByBodyPart(bossLeftFootPosition);

		// �E��
		DirectX::XMFLOAT3 bossRightFootPosition;
		bossRightFootPosition = model->ConvertLocalToWorld(bossRightFoot);

		enemyid.lock()->DetectHitByBodyPart(bossRightFootPosition);

		// �E�r
		DirectX::XMFLOAT3 bossRightHandPosition;
		bossRightHandPosition = model->ConvertLocalToWorld(bossRightHand);

		enemyid.lock()->DetectHitByBodyPart(bossRightHandPosition);

		// ���r
		DirectX::XMFLOAT3 bossLeftHandPosition;
		bossLeftHandPosition = model->ConvertLocalToWorld(bossLeftHand);

		enemyid.lock()->DetectHitByBodyPart(bossLeftHandPosition);

		// �����n�ʂɂ�
		if (animationTime + FLT_EPSILON >= 4.0f - FLT_EPSILON &&
			animationTime - FLT_EPSILON <= 4.1f + FLT_EPSILON)
		{
			smorker->Play(bossRightFootPosition);
		}

		// �����n�ʂɂ�
		if (animationTime + FLT_EPSILON >= 4.6f - FLT_EPSILON &&
			animationTime - FLT_EPSILON <= 4.7f + FLT_EPSILON)
		{
			smorker->Play(bossLeftFootPosition);
		}

	}
	//enemyid->CollisionPlayerWithRushEnemy();

	//// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	//float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	//if (animationTime >= 0.6f && animationTime <= 0.7f)
	//{
	//	// �A�j���[�V�������[�� ��~
	//	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Stop);
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
	//walkSe->Stop("walk");
}

void WanderState::End()
{
	// ���
	//owner.lock().reset();
}

// ������
void IdleState::Enter()
{
	// owner�Ăяo��
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	//model = owner.lock()->GetComponent<ModelControll>();
	//move = owner->GetComponent<Movement>();
	std::weak_ptr<HP> hp = owner.lock()->GetComponent<HP>();

	//model = owner->GetComponent<ModelControll>();
	model->PlayAnimation(EnemyBoss::Anim_Standby, loop, currentAnimationStartSeconds,blendSeconds);
	// �A�j���[�V�������[��
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	
	// �ҋ@����
	//stateTimer = stateTimerMax;
	stateTimer = Mathf::RandomRange(stateTimerMin, stateTimerMax);

	// ���������������
	hp.lock()->ResetOnDamageThresholdTime();
}

// update
void IdleState::Execute(float elapsedTime)
{
	 stateTimer -= elapsedTime;


	 std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	 Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	 std::weak_ptr <HP> hp = owner.lock()->GetComponent<HP>();

	if (stateTimer <= stateTimerEnd)
	{
		// TODO 03 
		// �ҋ@���Ԃ��o�߂����Ƃ��p�j�X�e�[�g�֑J�ڂ��Ȃ���
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Pursuit));
	}
	// ���_���[�W�̍ő�l
	float damageThreshold = 10;
	float timeLimit = 5;
	if (hp.lock()->CheckDamageThresholdWithinTime(elapsedTime, damageThreshold, timeLimit))
	{
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Jump));
	}

	// �_���[�W���[�V�����΍�
	if (!model->IsPlayAnimation())
	{
		model->PlayAnimation(EnemyBoss::Anim_Standby, loop, currentAnimationStartSeconds, blendSeconds);
	}
}

// �Ō�ɂ�肽�������S��
void IdleState::Exit()
{
	std::weak_ptr<HP> hp = owner.lock()->GetComponent<HP>();

	// ���������������
	hp.lock()->ResetOnDamageThresholdTime();
}
void IdleState::End()
{
	// ���
	//owner.lock().reset();
}
// ������
void PursuitState::Enter()
{
	// �v���C���[�R���|�[�l���g�A�N�Z�X
	//playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Player>();

	--mortionLimit;

	// �G�l�~�[�A�N�Z�X�p
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	enemyid.lock()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		EnemyBoss::Animation::Anim_Standby, loop,currentAnimationStartSeconds,  blendSeconds);

	// �A�j���[�V�������[��
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);

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

	// �G�l�~�[�A�N�Z�X�p
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	
	// �ڕW�n�_�����v���C���[�ʒu�ɐݒ�
	DirectX::XMFLOAT3 targetPosition = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();


	enemyid.lock()->SetTargetPosition(targetPosition);

	if (mortionLimit < 0)
	{
		// �ҋ@���ԍĐݒ�
		//mortionLimit = Mathf::RandomRange(3.0f, mortionLimitMax);
		mortionLimit = rand() % mortionLimitMax + 3;
		// ���E�������̂ŋx�e
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::IdleBattle));
		return;
	}

	// �^�C�}�[����
	stateTimer -= elapsedTime;


	float vx = targetPosition.x - enemyid.lock()->GetPosition().x;
	float vy = targetPosition.y - enemyid.lock()->GetPosition().y;
	float vz = targetPosition.z - enemyid.lock()->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	// TODO 03 
	// �U���͈͂ɓ������Ƃ��U���X�e�[�g�֑J�ڂ��Ȃ���
	//if (dist < attackRange)
	//	enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Attack));

	if (stateTimer >= stateTimerMin) return;

	switch (randamAttack)
	{
	case (int)EnemyBoss::AttackMode::AssaultAttack:
	{
		enemyid.lock()->GetStateMachine()->ChangeState(
			static_cast<int>(EnemyBoss::State::Wander));
		break;
	}
	case (int)EnemyBoss::AttackMode::JumpStompAttack:
	{
		enemyid.lock()->GetStateMachine()->ChangeState(
			static_cast<int>(EnemyBoss::State::Jump));
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
	//		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Shot));
	//	}

	//	break;
	//}

	//case AttackChange::Round2:
	//{
	//	if (stateTimer < 0.0f && attackType == 0)
	//		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Shot));

	//	//if (stateTimer < 0.0f && attackType == 1)
	//	//	owner->GetComponent<EnemyBoss>()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::ShotThrowing));

	//	break;
	//}

	//// HP�c�@�O
	//case AttackChange::ROund3:
	//{

	//	//owner->GetComponent<EnemyBoss>()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::ShotThrowing));
	//	if (stateTimer < 0.0f && attackType == 0)
	//		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Shot));

	//	//if (stateTimer < 0.0f && attackType == 1)
	//	//	owner->GetComponent<EnemyBoss>()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::ShotThrowing));

	//	if (stateTimer < 0.0f && attackType == 1)
	//		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Wander));

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
	//owner.lock().reset();
}

// ������
void JumpState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();

	// SE �ېݒ�
	//janpSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/Enemy���n.wav","janp");
	//landSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/Enemy���n.wav","land");

	//moveid = owner.lock()->GetComponent<Movement>();
	Model* model =  owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<HP> hpid = owner.lock()->GetComponent<HP>();
	// �꒣��
	enemyid.lock()->SetRandomTargetPosition();

	// �A�j���[�V�����Đ�
	model->
		PlayAnimation(EnemyBoss::Animation::Anim_Attack,
			loop, currentAnimationStartSeconds, blendSeconds,
			currentAnimationAddSeconds, keyFrameEnd);
	// �A�j���[�V�������[��
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Reverseplayback);

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
	if (hpid.lock()->GetIsOverDamageRule())
	{
		targetPosition = { 0,enemyid.lock()->GetPosition().y,0 };
		targetPosition.x += Mathf::RandomRange(-enemyid.lock()->GetSearchRange(), enemyid.lock()->GetSearchRange());
		targetPosition.z += Mathf::RandomRange(-enemyid.lock()->GetSearchRange(), enemyid.lock()->GetSearchRange());

		// ���_���[�W�m�F����
		bool isOverDamageRule = false;
		hpid.lock()->SetIsOverDamageRule(isOverDamageRule);
	}

	// ���G�t�F�N�g
	smorker = std::make_unique<Effect>("Data/Effect/smorkeDash.efk");
}
// �X�V����
void JumpState::Execute(float elapsedTime)
{
	//stateTimer -= elapsedTime;

	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();


	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<HP> hpid = owner.lock()->GetComponent<HP>();


	enemyid.lock()->SetTargetPosition(targetPosition);


	DirectX::XMVECTOR targetPositionVec = DirectX::XMLoadFloat3(&targetPosition);
	DirectX::XMVECTOR enemyPositionVec = DirectX::XMLoadFloat3(&enemyid.lock()->GetPosition());

	DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(targetPositionVec, enemyPositionVec);

	DirectX::XMFLOAT3 direction;

	DirectX::XMStoreFloat3(&direction, directionVec);

	//// �ړI�n�֒�����
	//if (stateTimer < 0.0f)
	//{
	//	enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
	//}
	// �ړI�n�ɂ��̔��a��������
	float radius = enemyid.lock()->GetAttackRightFootRange();

	//Model::Node* bossLeftHand = model->FindNode("boss_left_hand1");

	// �����{�[��
	Model::Node* bossLeftFoot = model->FindNode("boss_left_foot1");
	// �m�[�h�ʒu�擾
	// ����
	DirectX::XMFLOAT3 bossLeftFootPosition;
	bossLeftFootPosition = model->ConvertLocalToWorld(bossLeftFoot);

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();

	if (animationTime - FLT_EPSILON <= 0.8f + FLT_EPSILON && !jumpStart)
	{
		//landSe->Play("janp",loopSe);

		bool blurCheck = true;
		enemyid.lock()->SetBlurCheck(blurCheck);

		enemyid.lock()->CollisionPlayerWithRushEnemy();

		moveid.lock()->JumpVelocity(jumpSpeed);

		jumpStart = true;

		smorker->Play(bossLeftFootPosition);

		//janpSe->Play("janp",loopSe);

		return;
	}

	if (!moveid.lock()->GetOnLadius())
	{
		moveid.lock()->Move(direction, moveSpeed, elapsedTime);
		moveid.lock()->Turn(direction, turnSpeed, elapsedTime);
	}

	if (moveid.lock()->GetOnLadius() && jumpStart)
	{
		//landSe->Play("land",loopSe);
		//enemyid->InputProjectile();
		enemyid.lock()->InputImpact(enemyid.lock()->GetPosition());
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));

		// ���G�t�F�N�g
		smorker->Play(bossLeftFootPosition);
		//landSe->Play(loopSe);
	}
	enemyid.lock()->CollisitionNodeVsPlayer("boss_left_foot1",enemyid.lock()->GetRadius());
}
// �I������
void JumpState::Exit()
{
	//jumpStart = false;

	//janpSe->Stop("janp");
	//landSe->Stop("land");
}

void JumpState::End()
{
	// ���
	//owner.lock().reset();
}


// ������
void AttackState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();

	std::weak_ptr<HP> hpid = owner.lock()->GetComponent<HP>();

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	currentAnimationStartSeconds = 0.0f;
	model->PlayAnimation(EnemyBoss::Animation::Anim_Attack, loop, currentAnimationStartSeconds, blendSeconds);
	

	// �A�j���[�V�������[��
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);

		// ���C�t
	int life;
	life = hpid.lock()->GetLife();

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
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	// �U�����[�V�������I�������Ƃ��ǐՃX�e�[�g�ֈڍs
	if (!owner.lock()->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation() && attackMemory < attackMemoryMax)
	{
		// �A�j���Đ�
		currentAnimationStartSeconds = 0.8f;
		owner.lock()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemyBoss::Animation::Anim_Attack, loop, currentAnimationStartSeconds, blendSeconds);

		
		++attackMemory;
	}
	// �J��Ԃ����݂��Ō�
	else if (attackMemory == attackMemoryMax)
	{
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Pursuit));
		
		int attackMemoryStart = 0;
		attackMemory = attackMemoryStart;
	}

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner.lock()->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	if (animationTime >= 1.0f && animationTime <= 2.0f)
	{
		// �����m�[�h�ƃv���C���[�̏Փˏ���
		enemyid.lock()->CollisitionNodeVsPlayer("boss_left_foot1", 2);
	}

	if (animationTime >= 1.30f && animationTime <= 1.33f)
	{
		Model::Node* node = owner.lock()->GetComponent<ModelControll>()->GetModel()->FindNode("boss_left_foot1");
		DirectX::XMFLOAT3 pos(
			node->worldTransform._41,
			node->worldTransform._42,
			node->worldTransform._43
		);
		enemyid.lock()->InputImpact(pos);
	}


}
// �Ō�ɂ�肽�������S��
void AttackState::Exit()
{
}

void AttackState::End()
{
	// ���
	//owner.lock().reset();
}

// �����ݒ�
void AttackShotState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<HP> hpid = owner.lock()->GetComponent<HP>();
	

	currentAnimationStartSeconds = 0.0f;
	model->PlayAnimation(EnemyBoss::Animation::Anim_Shot, loop, currentAnimationStartSeconds, blendSeconds);

	// �A�j���[�V�������[��
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);

	// ���C�t
	int life;
	life = hpid.lock()->GetLife();

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
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<HP> hpid = owner.lock()->GetComponent<HP>();

	// �U�����[�V�������I�������Ƃ��ǐՃX�e�[�g�ֈڍs
	if (model->IsPlayAnimation())
	{
		// �A�j���Đ�
		currentAnimationStartSeconds = 0.1f;
		model->PlayAnimation(EnemyBoss::Animation::Anim_Shot, loop, currentAnimationStartSeconds ,blendSeconds);
		++attackCount;


	}
	if (attackCount >= attackCountMax)
	{
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Pursuit));
		attackCount = 0;
	}

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();
	// efekt�ݒ�
	if (animationTime >= 0.1f && animationTime <= 0.11f)
	{
		//	// �����m�[�h�ƃv���C���[�̏Փˏ���
		enemyid.lock()->InputProjectile();

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
	//owner.lock().reset();
}

// ��������
void AttackShotThrowingState::Enter()
{
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;

	//std::shared_ptr<Transform> transformid = nullptr;

	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	//std::shared_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	currentAnimationStartSecondsf = 0.0f;
	model->PlayReverseAnimation(EnemyBoss::Animation::Anim_jewelattack, loop,currentAnimationStartSecondsf,blendSeconds);
	// �A�j���[�V�������[��
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Reverseplayback);

	// ������
	turnPermission = false;

	// �n�ʔ����x����
	bool pushuThrow = false;
	enemyid.lock()->SetPushuThrow(pushuThrow);

}
// �X�V����
void AttackShotThrowingState::Execute(float elapsedTime)
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	// ����
	//DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();
	// �v���C���[id
	std::weak_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&playerid.lock()->GetComponent<Transform>()->GetPosition());

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
	float animationTime = model->GetCurrentANimationSeconds();
	// ��Ύ���Đݒ�
	if (animationTime <= 3.9f && animationTime >= 3.89f && !turnPermission)
	{
		////// ���ʂ̌����x�N�g��
  //       direction.x = sinf(angle.y);// �O�p���΂߂ɂ��Ĉʒu��ς���
  //       direction.y = sinf(angle.x);
  //       //direction.y = DirectX::XMConvertToRadians(0);
  //       direction.z = cosf(angle.y);		

		 
		//	�������U���o�^
		enemyid.lock()->InputThrowingRuby(playerid.lock()->GetComponent<Transform>()->GetPosition());

		

	}
	// ��]
	if (animationTime <= 3.6f && animationTime >= 3.0f && !turnPermission)
	{

		enemyid.lock()->SetTargetPosition(playerid.lock()->GetComponent<Transform>()->GetPosition());
		enemyid.lock()->TurnToTarget(elapsedTime, turnSpeed);



		for (int i = 0; i < ProjectileManager::Instance().GetProjectileCount() ; ++i)
		{


			std::shared_ptr<Actor> projectileManager = ProjectileManager::Instance().GetProjectile(i);
			if (projectileManager->GetComponent<ProjectileThrowing>())
			{
				
				projectileManager->GetComponent<Transform>()->SetPosition(
					{ 
						transformid.lock()->GetPosition().x,
						transformid.lock()->GetPosition().y + transformid.lock()->GetHeight(),
						transformid.lock()->GetPosition().z

					});

				projectileManager->GetComponent<BulletFiring>()->TurnFull(turnSpeed, playerid.lock()->GetComponent<Transform>()->GetPosition(), elapsedTime);

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
				DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&playerid.lock()->GetComponent<Transform>()->GetPosition());
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
		model->SetAnimationEndFlag(end);
		//bool blend = false;

		model->ReverseplaybackAnimation(elapsedTime);
		// �A�j���[�V�����Đ�����
		currentAnimationStartSecondsf = 0.7f;

		model->PlayAnimation(EnemyBoss::Animation::Anim_Shot, loop, currentAnimationStartSecondsf, blendSeconds);
		// �A�j���[�V�������[��
		enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);


		turnPermission = true;
	}
	// �ˌ�����
	if (!owner.lock()->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation() && turnPermission)
	{
		// �n�ʔ��蔭��
		bool pushuThrow = true;
		enemyid.lock()->SetPushuThrow(pushuThrow);
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
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Pursuit));
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
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();


	// �_���[�W
	//damageSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/����s��.wav");

	//owner.lock()->GetComponent<ModelControll>() = owner.lock()->GetComponent<ModelControll>();
	Model* momdel = owner.lock()->GetComponent<ModelControll>()->GetModel();
	momdel->
		PlayAnimation(
			EnemyBoss::Animation::Anim_Die, loop,
			currentAnimationStartSeconds,currentAnimationAddSeconds,
			keyFrameEnd);
	// �A�j���[�V�������[��
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);

	stateTimer = stateTimerMax;

	// �_���[�WSe
	//damageSe->Play(loopSe);

}

void DamageState::Execute(float elapsedTime)
{

	--stateTimer;

	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();

		if (stateTimer < stateTimerEnd)
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));

}

void DamageState::Exit()
{
	// �_���[�WSe��~
	//damageSe->Stop();
}

void DamageState::End()
{
	// ���
	owner.lock().reset();
}

// �����J�n
void ConfusionState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	//owner.lock()->GetComponent<ModelControll>() = owner.lock()->GetComponent<ModelControll>();
	//model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	model->PlayAnimation(EnemyBoss::Animation::Anim_Die, loop, currentAnimationStartSeconds, blendSeconds,
		currentAnimationAddSeconds, keyFrameEnd);
	// �A�j���[�V�������[��
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);

	// �X�e�[�g�ҋ@����
	stateTimer = stateTimerMax;

	// ����Se�ېݒ�
	//confusionSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/����s��.wav");
	//confusionSe->Play(loopSe);

}
// �����X�V
void ConfusionState::Execute(float elapsedTime)
{
	stateTimer -= elapsedTime;

	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	if (stateTimer < stateTimerEnd)
	{
		// �X�e�[�g�ύX
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Pursuit));
	}

	//// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	//float animationTime = owner.lock()->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	//// �Ђ��܂�
	//if (animationTime >= 1.1)
	//{
	//	// �A�j���[�V�������[��
	//	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Stop);

	//}

	if (!model->IsPlayAnimation())
		model->PlayAnimation(EnemyBoss::Animation::Anim_Die, loop, currentAnimationStartSeconds, blendSeconds,
			currentAnimationAddSeconds, keyFrameEnd);

}
// �����I��
void ConfusionState::Exit()
{
	//confusionSe->Stop();
}

void ConfusionState::End()
{
	// ���
	owner.lock().reset();
}

void DeathState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	//std::shared_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// ���S��Se
	//deathSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/Enemy���n.wav","death");


	Model* model;

	//owner.lock()->GetComponent<ModelControll>() = owner.lock()->GetComponent<ModelControll>();
	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	model->
		PlayAnimation(
			EnemyBoss::Animation::Anim_Die, loop);
	// �A�j���[�V�������[��
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);

	stateTimer = 3;

	bool freeCheckCamera = false;

	int playerCountMax = PlayerManager::Instance().GetPlayerCount();
	if (playerCountMax > 0)
	PlayerManager::Instance().GetPlayer(playerCountMax - 1)->GetComponent<Player>()->SetFreeCameraCheck(freeCheckCamera);
}

void DeathState::Execute(float elapsedTime)
{
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();;



	stateTimer -= elapsedTime; 

	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// ���S���͂�����ۂ��J�����A���O���Ŏ��S
// �Ⴆ�΃R����K�E�Z�Ȃǂŏ�肭���p�ł���΂����������J�������o�����܂���
	MessageData::CAMERACHANGEMOTIONMODEDATA	p;

	DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();
	DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();

	//if(animationTime + FLT_EPSILON >= 0.5f - FLT_EPSILON && animationTime - FLT_EPSILON <= 0.51f + FLT_EPSILON)
	//deathSe->Play("death",loopSe);


	float vx = sinf(angle.y) * 6;
	float vz = cosf(angle.y) * 6;
	p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
	p.data.push_back({ 95, {position.x + vx + 13, position.y + 3, position.z + vz }, position });

	Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

	if (stateTimer < 0)
	{

		enemyid.lock()->SetClearCheck(clearCheck);
	}
}

void DeathState::Exit()
{
	//deathSe->Stop("death");
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

	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();


	//modelid = owner.lock()->GetComponent<ModelControll>();

	model->PlayAnimation(
		Player::Anim_Idle, loop,currentAnimationStartSeconds,blendSeconds
	);
	
	// �A�j���[�V�������[��
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
	

	// ������
	//bool stop = false;
	moveid.lock()->SetStopMove(false);
}

void PlayerIdleState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	// ���b�N�I������
	playerid.lock()->UpdateCameraState(elapsedTime);

	// �ړ����͏���
	if (playerid.lock()->InputMove(elapsedTime))
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
		//TransitionMoveState();
	}

	// ���˓��͏���
	if (playerid.lock()->InputAvoidance())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Reflection));

	}


	// �W�����v���͏���
	if (playerid.lock()->InputJump())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
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
	//owner.lock()->GetComponent<Player>() = owner.lock()->GetComponent<Player>();


	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();


	// ����Se�ېݒ�
	//walkSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/����.wav");
	//if(moveid.lock()->GetOnLadius())
	//walkSe->Play(loopSe);

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();



	//modelid = owner.lock()->GetComponent<ModelControll>();

		//Player::Anim_Move, loop,
	model->PlayAnimation(
		Player::Anim_Running, loop,
		currentAnimationStartSeconds, blendSeconds
	);

	//	//Player::Anim_Move, loop,
	//owner.lock()->GetComponent<ModelControll>()->GetModel()->PlayUpeerBodyAnimation(
	//	Player::Anim_Running, loop,
	//	currentAnimationStartSeconds, blendSeconds
	//);


	// �A�j���[�V�������[��
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);


	// ������
	//bool stop = false;
	moveid.lock()->SetStopMove(false);

	// SE�Đ�
	playerid.lock()->PlaySE()->Play("walk");
	playerid.lock()->PlaySE()->SetSpeed("walk",0.5f);
}

void PlayerMovestate::Execute(float elapsedTime)
{


	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	//std::shared_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	// �ړ��l
	playerid.lock()->GetMoveVec(elapsedTime);

	// ���b�N�I������
	playerid.lock()->UpdateCameraState(elapsedTime);

	// �ړ����͏���
	if (!playerid.lock()->InputMove(elapsedTime))
	{
		//walkSe->Stop();
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
		//TransitionIdleState();
	}


	// �����͏���
	if (playerid.lock()->InputAvoidance())
	{
		//walkSe->Stop();
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

	}

	// �W�����v���͏���
	if (playerid.lock()->InputJump())
	{
		//walkSe->Stop();
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
		
	}



}

void PlayerMovestate::Exit()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	// SE�Đ�
	playerid.lock()->PlaySE()->Stop("walk");
	//walkSe->Stop();
}

void PlayerMovestate::End()
{
	// ���
	owner.lock().reset();
}

void PlayerJumpState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	// �ېݒ�Se
	//janpSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/����.wav");
	//janpSe->Play(loopSe);

	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	//model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	//moveid = owner.lock()->GetComponent<Movement>();

	model->PlayAnimation(
		Player::Anim_Jump, loop,
	currentAnimationStartSeconds, blendSeconds);

	// �A�j���[�V�������[��
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);


	// ������
	//bool stop = false;
	moveid.lock()->SetStopMove(false);

	playerid.lock()->PlaySE()->Play("land");
}

void PlayerJumpState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	//Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();


	// ���b�N�I������
	playerid.lock()->UpdateCameraState(elapsedTime);

	// �ړ�
	if (playerid.lock()->InputMove(elapsedTime))
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	}


	// �W�����v���͏���
	if (playerid.lock()->InputJump())
	{
		
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
	}




	// ���
	if (playerid.lock()->InputAvoidance())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}

	// ���n
	if (moveid.lock()->GetOnLadius())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Land));
	}





}

void PlayerJumpState::Exit()
{
	//janpSe->Stop();
}

void PlayerJumpState::End()
{
	// ���
	owner.lock().reset();
}

void PlayerLandState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	//// Se
	//landSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/����.wav");
	//landSe->Play(loopSe);

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	//std::shared_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	//model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	//moveid = owner.lock()->GetComponent<Movement>();
	//moveid->SetOnLadius(false);

	model->PlayAnimation(
		Player::Anim_Land, loop,
		currentAnimationStartSeconds, blendSeconds
	);

	// �A�j���[�V�������[��
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerLandState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	// ���b�N�I������
	playerid.lock()->UpdateCameraState(elapsedTime);

	//// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	//float animationTime = model->GetCurrentANimationSeconds();
	//// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B

	// ����
	if (playerid.lock()->InputMove(elapsedTime))
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	}

		// ���
	if (playerid.lock()->InputAvoidance())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}

	// �ҋ@
	if (!model->IsPlayAnimation())
	{
		
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


	}
}

void PlayerLandState::Exit()
{
	//landSe->Stop();
}

void PlayerLandState::End()
{
	// ���
	owner.lock().reset();
}

void PlayerJumpFlipState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	//// Se
	//janpSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/����.wav");
	//janpSe->Play(loopSe);

	//std::shared_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	/*model = owner.lock()->GetComponent<ModelControll>()->GetModel();*/
	//moveid = owner.lock()->GetComponent<Movement>();

	model->PlayAnimation(
		Player::Anim_Jump, loop,
		currentAnimationStartSeconds , blendSeconds);

	// �A�j���[�V�������[��
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerJumpFlipState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	// ���b�N�I������
	playerid.lock()->UpdateCameraState(elapsedTime);
	// �ړ�
	if (playerid.lock()->InputMove(elapsedTime))
	{
		
		bool afterimagemove = true;
	}
	// �W�����v���͏���
	if (!model->IsPlayAnimation())
	{

		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
	}

	// ���
	if (playerid.lock()->InputAvoidance())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}

}

void PlayerJumpFlipState::Exit()
{
	//janpSe->Stop();

}

void PlayerJumpFlipState::End()
{
	// ���
	owner.lock().reset();
}

void PlayerQuickJabState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	/*slashSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/�X���b�V���P���.wav");*/

	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	//model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	//moveid = owner.lock()->GetComponent<Movement>();
	//transformid = owner.lock()->GetComponent<Transform>();

	//std::shared_ptr<Player> playerId = owner.lock()->GetComponent<Player>();

		//Player::Anim_Slash, loop,
		// �P��
	
	if (!moveid.lock()->GetOnLadius())
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
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Doble);

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
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
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
	moveid.lock()->SetStopFall(stopFall);



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

	// �R���|�l���g�擾
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();

	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// ���b�N�I������
	playerid.lock()->UpdateCameraState(elapsedTime);



	if (playerid.lock()->InputAttack() && 
		playerid.lock()->GetSelectCheck() ==
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
	if (playerid.lock()->InputAvoidance() && playerid.lock()->InputMove(elapsedTime))
	{
		bool stop = false;
		moveid.lock()->SetStopMove(stop);

		DirectX::XMFLOAT3 vec;
		vec = playerid.lock()->GetMoveVec(elapsedTime);

		moveid.lock()->Turn(vec, turnSpeed,elapsedTime);

		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

		button = false;
		buttonSeconde = false;

		return;
	}

	DirectX::XMVECTOR Vector;
	DirectX::XMVECTOR LengthSq;

	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid.lock()->GetPosition());
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
	{
		DirectX::XMVECTOR enemyPosition = DirectX::XMLoadFloat3(&enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition());


		Vector = DirectX::XMVectorSubtract(enemyPosition, playerPosition);

		LengthSq = DirectX::XMVector3Length(Vector);

		Vector = DirectX::XMVector3Normalize(Vector);

		DirectX::XMStoreFloat3(&vector, Vector);

		DirectX::XMStoreFloat(&length, LengthSq);
	}
	//// ����̕�������
	//if (length > attackCheckRangeMin && length < attackCheckRange && moveid->GetOnLadius())
	//{
	//	bool stop = false;
	//	moveid->SetStopMove(stop);

	//	//rotateCheck = true;

	//	//angle = transformid->GetAngle();

	//	DirectX::XMFLOAT3 direction = vector;

	//	//direction.x = sinf(angle.y) * 6;
	//	//direction.y = 0;
	//	//direction.z = cosf(angle.y) * 6;

	//	moveid->Turn(direction,turnSpeed,elapsedTime);

	//	//moveid->Move(direction, attackSpeed, elapsedTime);
	//}
	//else
	//{

	//	bool stop = true;
	//	moveid->SetStopMove(stop);

	//}


	// ����̕�������
	// ��]
	if (!rotateCheck)
	{

			// ����
			if (length < attackCheckRange && length > attackCheckRangeMin
				&& moveid.lock()->TurnCheck(
					vector, angleRange, elapsedTime)&&
				moveid.lock()->GetOnLadius())
			{
				//bool stop = false;
				//moveid->SetStopMove(stop);
				bool stop = false;
				moveid.lock()->SetStopMove(stop);
				// ����
				if (moveid.lock()->Turn(vector, turnSpeed, elapsedTime))
				{

					moveid.lock()->Move(vector, speed, elapsedTime);
					
				}
				//stop = true;
				//moveid->SetStopMove(stop);
			}
			else
			{


				bool stop = true;
				moveid.lock()->SetStopMove(stop);

				rotateCheck = true;
			}


		

	}
	if (attackMemory > attackMemoryMax)
	{
		// ���͊m�F�ŃX�e�[�g�ύX
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


		// �U���ő�l����
		attackMemory = 0;
	}


	
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = moveid.lock()->GetOnLadius() ? 
		model->GetCurrentANimationSeconds() : 
		model->GetCurrentAnimationSecondsUpeer();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B


	// 1����
	if (animationTime >= 0.8f )
	{
		// �P��ڂ̍U���Ȃ�
		oneAttackCheck = false;

		playerid.lock()->SetAttackCollisionFlag(false);
		deleteCheck = false;
		// ���͊m�F�ŃX�e�[�g�ύX
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(button  ? Player::State::SideCut : Player::State::Move));
		return;
	}



	if (playerid.lock()->CollisionNodeVsEnemies("mixamorig:LeftHand",
		owner.lock()->GetComponent<Player>()->GetLeftHandRadius(),
		"body2", "boss_left_hand2", "boss_right_hand2"))
	{
		//slashSe->Play(loopSe);
	}
}

void PlayerQuickJabState::Exit()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// �ړ��̒�~
	moveid.lock()->SetGravity(
		playerid.lock()->GetGravity());


	// ��������
	bool stopMove = false;
	moveid.lock()->SetStopMove(stopMove);

	// �d�̓I�t
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);
	if (deleteCheck )
	{
		// �_���[�W�H��������ǂ���
		button = false;
		buttonSeconde = false;

		// �U���ő�l����
		attackMemory = 0;
	}

	//slashSe->Stop();

}

void PlayerQuickJabState::End()
{
	// ���
	owner.lock().reset();
}


void PlayerSideCutState::Enter()
{
	bool loop = false;

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// Se
	//slashSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/�X���b�V���Q���.wav");

	// ���

	if (!moveid.lock()->GetOnLadius())
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
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Doble);
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
	    playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
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
	moveid.lock()->SetStopFall(stopFall);

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


	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// ���b�N�I������
	playerid.lock()->UpdateCameraState(elapsedTime);



	//angle = transformid->GetAngle();

	//DirectX::XMFLOAT3 direction;
	//direction.x = sinf(angle.y) * 6;
	//direction.y = 0;
	//direction.z = cosf(angle.y) * 6;

	//moveid->Move(direction, attackSpeed, elapsedTime);

	if (playerid.lock()->InputAttack() &&
		playerid.lock()->GetSelectCheck() ==
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
	if (playerid.lock()->InputAvoidance() && playerid.lock()->InputMove(elapsedTime))
	{
		bool stop = false;
		moveid.lock()->SetStopMove(stop);

		DirectX::XMFLOAT3 vec;
		vec = owner.lock()->GetComponent<Player>()->GetMoveVec(elapsedTime);

		moveid.lock()->Turn(vec, turnSpeed, elapsedTime);

		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

		button = false;
		buttonSeconde = false;

		return;
	}


	DirectX::XMVECTOR Vector;
	DirectX::XMVECTOR LengthSq;

	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid.lock()->GetPosition());
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
		moveid.lock()->SetStopMove(stop);

		DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();

		DirectX::XMFLOAT3 direction;
		direction.x = sinf(angle.y) * 6;
		direction.y = 0;
		direction.z = cosf(angle.y) * 6;

		//moveid->Move(direction, attackSpeed, elapsedTime);
	}
	else if (!moveid.lock()->GetOnLadius())
	{
	
		bool stop = true;
		moveid.lock()->SetStopMove(stop);

	}

	// ��]
	if (!rotateCheck && !playerid.lock()->InputAvoidance())
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
				&& moveid.lock()->TurnCheck(
					vector, angleRange, elapsedTime))
			{
				//bool stop = false;
				//moveid->SetStopMove(stop);

				// ����
				if (moveid.lock()->Turn(vector, turnSpeed, elapsedTime))
				{

					moveid.lock()->Move(vector, speed, elapsedTime);

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
	float animationTime = moveid.lock()->GetOnLadius() ?
		model->GetCurrentANimationSeconds() :
		model->GetCurrentAnimationSecondsUpeer();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B

	//owner.lock()->GetComponent<Player>()->InputAvoidance();

	// 2����
	if (animationTime >= 1.1f)
	{
		playerid.lock()->SetAttackCollisionFlag(false);
		deleteCheck = false;



		// ���͊m�F�ŃX�e�[�g�ύX
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(buttonSeconde ? Player::State::CycloneStrike : Player::State::Move));

		return;
	}


	if (playerid.lock()->CollisionNodeVsEnemies("mixamorig:LeftHand"
		, playerid.lock()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2"))
	{
		//slashSe->Play(loopSe);
	}

}

void PlayerSideCutState::Exit()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// �ړ��̒�~
	moveid.lock()->SetGravity(
		playerid.lock()->GetGravity());


	// ��������
	bool stopMove = false;
	moveid.lock()->SetStopMove(stopMove);

	// �d�̓I�t
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);

	//slashSe->Stop();
	
}

void PlayerSideCutState::End()
{
	// ���
	owner.lock().reset();
}


void PlayerCycloneStrikeState::Enter()
{
	bool loop = false;

	// �R���|�l���g�擾
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// se
	//slashSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/�R����.wav");


	if (!moveid.lock()->GetOnLadius())
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
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Doble);
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
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
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
	moveid.lock()->SetStopFall(stopFall);

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

	// �R���|�l���g�擾
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();


	// ���b�N�I������
	playerid.lock()->UpdateCameraState(elapsedTime);


	//angle = transformid->GetAngle();

	//DirectX::XMFLOAT3 direction;
	//direction.x = sinf(angle.y) * 6;
	//direction.y = 0;
	//direction.z = cosf(angle.y) * 6;

	//moveid->Move(direction, attackSpeed, elapsedTime);

	// ���
	if (playerid.lock()->InputAvoidance())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));

		button = false;
		buttonSeconde = false;
	}


	DirectX::XMVECTOR Vector;
	DirectX::XMVECTOR LengthSq;

	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid.lock()->GetPosition());
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
		moveid.lock()->SetStopMove(stop);

		DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();

		DirectX::XMFLOAT3 direction;
		direction.x = sinf(angle.y) * 6;
		direction.y = 0;
		direction.z = cosf(angle.y) * 6;

		//moveid->Move(direction, attackSpeed, elapsedTime);
	}
	else if (!moveid.lock()->GetOnLadius())
	{

		bool stop = true;
		moveid.lock()->SetStopMove(stop);

	}

	// ��]
	if (!rotateCheck)
	{

			// ����
			if (length < attackCheckRange && length > attackCheckRangeMin
				&& moveid.lock()->TurnCheck(
					vector, angleRange, elapsedTime))
			{
				//bool stop = false;
				//moveid->SetStopMove(stop);

				// ����
				if (moveid.lock()->Turn(vector, turnSpeed, elapsedTime))
				{

					moveid.lock()->Move(vector, speed, elapsedTime);

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
	float animationTime = moveid.lock()->GetOnLadius() ?
		model->GetCurrentANimationSeconds() :
		model->GetCurrentAnimationSecondsUpeer();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B


	// 3����
	if (animationTime >= 2.7f )
	{
		playerid.lock()->SetAttackCollisionFlag(false);
		deleteCheck = false;



		// ���͊m�F�ŃX�e�[�g�ύX
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
		return;
	}


	if (playerid.lock()->CollisionNodeVsEnemies("mixamorig:LeftHand",
		playerid.lock()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2"))
	{
		//slashSe->Play(loopSe);
	}

}

void PlayerCycloneStrikeState::Exit()
{
	// �R���|�l���g�擾
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// �ړ��̒�~
	moveid.lock()->SetGravity(
		playerid.lock()->GetGravity());



	// ��������
	bool stopMove = false;
	moveid.lock()->SetStopMove(stopMove);

	// �d�̓I�t
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);

	//slashSe->Stop();

}

void PlayerCycloneStrikeState::End()
{
	// ���
	owner.lock().reset();
}


void PlayerSpecialAttackState::Enter()
{
	// �R���|�l���g�擾
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	//transformid = owner.lock()->GetComponent<Transform>();
	//model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	//moveid = owner.lock()->GetComponent<Movement>();

	// se �o�^
	//slashSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/�X���b�V���Q���.wav");
	//lighteningStrikeSpecialSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/�K�E�Z��.wav");
	//lighteningStrikeSpecialSaveSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/�K�E�Z������.wav");

	loopSe = false;
	//lighteningStrikeSpecialSe->Play(loopSe);

	std::weak_ptr<Transform> enemyTransform = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<Transform>();
	std::weak_ptr<HP> enemyHpId = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<HP>();

	lightning = std::make_unique<Effect>("Data/Effect/sunder.efk");
	lightningAttack = std::make_unique<Effect>("Data/Effect/HitThunder.efk");
	lightningHit = std::make_unique<Effect>("Data/Effect/lightningStrike.efk");




	if (!moveid.lock()->GetOnLadius())
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
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Doble);
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
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);

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
	moveid.lock()->SetStopFall(stopFall);

	button = true;

	// �t���b�V��
	flashOn = true;


	


	//p.data.push_back({ 180, {position.x + (pos.x * length) ,
	//position.y + (pos.y * endLength) + 1,
	//position.z + (pos.z * endLength) }, position });

	//p.data.push_back({ 180, position, {(position.x * pos.x) * length ,
	//	(position.y * pos.y) * length ,
	//	(position.z * pos.z) * length } });




	// ��]����
	isRotate = true;

	//// ����
	//currentAnimationAddSeconds = 0.0f;
}

void PlayerSpecialAttackState::Execute(float elapsedTime)
{
	// �R���|�l���g�擾
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<HP> enemyHpId ;

	// �G�l�~�[�̑O�Ɍ���
	{
		DirectX::XMVECTOR Vector;
		DirectX::XMVECTOR LengthSq;

		DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid.lock()->GetPosition());
		EnemyManager& enemyManager = EnemyManager::Instance();
		int enemyCount = enemyManager.GetEnemyCount();
		for (int i = 0; i < enemyCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
		{
			DirectX::XMVECTOR enemyPosition = DirectX::XMLoadFloat3(&enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition());

			enemyHpId = enemyManager.GetEnemy(i)->GetComponent<HP>();

			Vector = DirectX::XMVectorSubtract(enemyPosition, playerPosition);

			LengthSq = DirectX::XMVector3Length(Vector);

			Vector = DirectX::XMVector3Normalize(Vector);

			DirectX::XMStoreFloat3(&vector, Vector);

			DirectX::XMStoreFloat(&length, LengthSq);
		}
		// ����̕�������
		// ��]
		if (isRotate)
		{
			//length < attackCheckRangeMax && length > attackCheckRangeMin&&
	
			if (moveid.lock()->Turn(vector, turnSpeed, elapsedTime))
			{
				// �J��������
				{
					MessageData::CAMERACHANGEMOTIONMODEDATA	p;

					position = transformid.lock()->GetPosition();
					angle = transformid.lock()->GetAngle();

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
					{ 0,0,0 };
					float length;
					float endLength = 8.0f;

					length = 10.0f;

					//�@�J�����ړ��@�x�N�g���l��
					if (enemyManagerCount > 0)
					{

						std::shared_ptr<EnemyBoss> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemyBoss>();

						DirectX::XMVECTOR positionVec = DirectX::XMLoadFloat3(&position);
						DirectX::XMVECTOR ePositionVec = DirectX::XMLoadFloat3(&enemy->GetPosition());

						posVec = DirectX::XMVectorSubtract(positionVec, ePositionVec);

						posVec = DirectX::XMVector3Normalize(posVec);

						DirectX::XMStoreFloat3(&pos, posVec);

					}



					p.data.push_back({ 190, {position.x + (pos.x * length) ,
						position.y + (pos.y * length) + 1,
						position.z + (pos.z * length) }, position });


					Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);
				}
			}
			//�@�K�E�Z�̑ΏۂɌ�������
			else
			{
	
				isRotate = false;
				
			}
		}
	}
	if (button)
	{

					// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
		float animationTime = moveid.lock()->GetOnLadius() ?
			model->GetCurrentANimationSeconds() :
			model->GetCurrentAnimationSecondsUpeer();

		// �A�j���[�V����
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON)
		{

			if (!moveid.lock()->GetOnLadius())
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

			//Se
			//lighteningStrikeSpecialSe->Play(loopSe);
			loopSe = true;
			//lighteningStrikeSpecialSaveSe->Play(loopSe);


			//rockCheck = true;
			//owner.lock()->GetComponent<Player>()->SetRockCheck(rockCheck);


			
		}
	}
	else
	{
	
			// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
			float animationTime = moveid.lock()->GetOnLadius() ?
				model->GetCurrentANimationSeconds() :
				model->GetCurrentAnimationSecondsUpeer();

		// �X���[���[�V�����ƐF�𖾂邭
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON)
		{
			playerid.lock()->SetFlashOn(flashOn);
			playerid.lock()->SetHitCheck(true);
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
			

			
			if (enemyHpId.lock()->ApplyDamage(10, 0.5f)) 
			{
				
				lightningHit->Play(pPosition);
			}
			playerid.lock()->SetHitCheck(false);

			//owner.lock()->GetComponent<Player>()->CollisionNodeVsEnemies("mixamorig:LeftHand", owner.lock()->GetComponent<Player>()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");
		}
		bool animationEnd = moveid.lock()->GetOnLadius() ?
			model->IsPlayAnimation() :
			model->IsPlayUpeerBodyAnimation();

		// �A�j���[�V�����I��
		if (!animationEnd)
		{
			// ���͊m�F�ŃX�e�[�g�ύX
			playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));

		}
	}

}

void PlayerSpecialAttackState::Exit()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// ������̍ĊJ
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);


	// �G�l�~�[�Ăԓz
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyManagerCount = enemyManager.GetEnemyCount();

	// ���삳���邩�ǂ���
	if (enemyManagerCount > 0)
	{

		std::shared_ptr<EnemyBoss> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemyBoss>();
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

	//se ��~
	//slashSe->Stop();
	//lighteningStrikeSpecialSe->Stop();
	//lighteningStrikeSpecialSaveSe->Stop();

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

	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// Se
	//flameStarteSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/�����e��.wav");
	//flameimpactSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/����s��.wav");

	//iceStarteSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/�X����.wav");
	//iceimpactSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/�X���e��.wav");

	//lightningSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/��.wav");

	//// �T���_�[�n
	//if (playerid.lock()->GetSelectMagicCheck() ==
	//	(int)Player::CommandMagic::Thander)
	//{
	//	// �A�j���[�V�����Đ�
	//	model->PlayAnimation(
	//		Player::Anim_Magic, loop,
	//		currentAnimationStartSeconds, blendSeconds
	//	);

	//	// ��se
	//	lightningSe->Play(loopSe);
	//}
	//else
	//{
	//	// �A�j���[�V�����Đ�
	//	model->PlayAnimation(
	//		Player::Anim_Magic, loop,
	//		currentAnimationStartSeconds, blendSeconds
	//	);

	//}


	magicType = playerid.lock()->GetSelectMagicCheck();

	switch (magicType)
	{
	case (int)Player::CommandMagic::Fire:
	{
		// �A�j���[�V�����Đ�
		model->PlayAnimation(
			Player::Anim_Magic, loop,
			currentAnimationStartSeconds, blendSeconds
		);


		break;
	}
	case (int)Player::CommandMagic::Ice:
	{
		// �A�j���[�V�����Đ�
		model->PlayAnimation(
			Player::Anim_Magic, loop,
			currentAnimationStartSeconds, blendSeconds
		);

		break;
	}

	case ((int)Player::CommandMagic::Thander):
	{
		// �A�j���[�V�����Đ�
		model->PlayAnimation(
			Player::Anim_MagicSeconde, loop,
			currentAnimationStartSeconds, blendSeconds
		);


		break;
	}

	default:
		break;
	}

	// �A�j���[�V�����Đ�
	owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);


	// ������̒�~
	bool stopFall = true;
	moveid.lock()->SetStopFall(stopFall);


	// ��]����
	isRotate = true;

	// �ړ�����
	isMove = false;

	// SE
	playerid.lock()->PlaySE()->Play("flame");
	
}

void PlayerMagicState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// ���b�N�I������
	playerid.lock()->UpdateCameraState(elapsedTime);

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();
	// �A�j���[�V�����I�������ʂ�


	DirectX::XMVECTOR vectorXM;
	DirectX::XMVECTOR lengthSqXM;

	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid.lock()->GetPosition());
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
	{
		DirectX::XMVECTOR enemyPosition = DirectX::XMLoadFloat3(&enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition());


		vectorXM = DirectX::XMVectorSubtract(enemyPosition, playerPosition);

		lengthSqXM = DirectX::XMVector3Length(vectorXM);

		vectorXM = DirectX::XMVector3Normalize(vectorXM);

		DirectX::XMStoreFloat3(&vector, vectorXM);

		DirectX::XMStoreFloat(&length, lengthSqXM);
	}

	// ���@���Ȃ���ړ�
	if (playerid.lock()->InputMove(elapsedTime) && !isMove)
	{
		bool stop = false;
		moveid.lock()->SetStopMove(stop);

		// �ړ��l
		playerid.lock()->GetMagicMoveVec(elapsedTime);
	}
	else
	{
		// �������u�Ԃ���ړ����Ă��Ȃ��Ƃ��݂Ȃ�
		isMove = true;
	}



	

	// ����̕�������
	// ��]
	if (isRotate)
	{

		// ����
		if (moveid.lock()->TurnCheck(
				vector, angleRange, elapsedTime) &&
			playerid.lock()->GetRockCheck())
		{
			//bool stop = false;
			//moveid->SetStopMove(stop);

			// ����
			moveid.lock()->Turn(vector,turnSpeed,elapsedTime);
			//stop = true;
			//moveid->SetStopMove(stop);
		}
		else
		{


			//bool stop = true;
			//moveid->SetStopMove(stop);

			isRotate = false;
		}




	}

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
		// ��se
		//flameStarteSe->Play(loopSe);

		break;
	}

	case (int)Player::CommandMagic::Thander:
	{
		// ����
		if (animationTime <= 1.1f)return;

		// ������
		owner.lock()->GetComponent<Player>()->InputMagicLightning();
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Idle);

		// ��se
		///lightningSe->Play(loopSe);
		break;
	}
	case (int)Player::CommandMagic::Ice:
	{
		// ����
		if (animationTime <= 0.5f)return;
		// �X����
		owner.lock()->GetComponent<Player>()->InputMagicIce();
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Idle);

		// �Xse
		//iceStarteSe->Play(loopSe);
		break;
	}

	default:
		break;
	}

}

void PlayerMagicState::Exit()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// ���@�̑I�����[����
	//owner.lock()->GetComponent<Player>()->SetSelectCheck((int)Player::CommandMagic::Normal);
	if (!playerid.lock()->InputShortCutkeyMagic())
	{
		// ���@�I������
		playerid.lock()->SetMagicAction(false);

		// �U����ޑI��
		playerid.lock()->SetSelectMagicCheck((int)Player::CommandAttack::Attack);

	}

	// ������̍ĊJ
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);

	// se
	//flameStarteSe->Stop();
	//iceStarteSe->Stop();
	//lightningSe->Stop();
}

void PlayerMagicState::End()
{
	// ���
	owner.lock().reset();
}

void PlayerSpecialMagicState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// Se
	//flameSpecialStarteSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/�K�E�Z��.wav");
	//flameSpecialSaveSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/�K�E�Z������.wav");
	//flameSpecialSaveSe->Play(loopSe);

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
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);


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
	moveid.lock()->SetStopFall(stopFall);

	button = true;

	// �X�^�[�g�l
	specialMoveWaitTime = specialMoveWaitStartTime;

	// �ŏ��������@�𔭓����邽��
	startMagic = false;

	// �J������]�������J�n
	isRotate = true;




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

	//// �G�l�~�[�Ăԓz
	//EnemyManager& enemyManager = EnemyManager::Instance();
	//int enemyManagerCount = enemyManager.GetEnemyCount();

	//DirectX::XMFLOAT3 pos;
	//DirectX::XMVECTOR posVec;


	//pos =
	//{ 0,0,0 };
	//float length;

	//length = 10.0f;

	////�@�J�����ړ��@�x�N�g���l��
	//if (enemyManagerCount > 0)
	//{

	//	std::shared_ptr<EnemyBoss> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemyBoss>();

	//	DirectX::XMVECTOR positionVec = DirectX::XMLoadFloat3(&position);
	//	DirectX::XMVECTOR ePositionVec = DirectX::XMLoadFloat3(&enemy->GetPosition());

	//	posVec = DirectX::XMVectorSubtract(positionVec, ePositionVec);

	//	posVec = DirectX::XMVector3Normalize(posVec);

	//	DirectX::XMStoreFloat3(&pos, posVec);

	//}




	//p.data.push_back({ 330,{position.x + (pos.x * length) ,
	//	position.y + (pos.y * length) + 1,
	//	position.z + (pos.z * length) }, position });




	//Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);


}

void PlayerSpecialMagicState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// �G�l�~�[�̑O�Ɍ���
	{
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

			Vector = DirectX::XMVector3Normalize(Vector);

			DirectX::XMStoreFloat3(&vector, Vector);

			DirectX::XMStoreFloat(&length, LengthSq);
		}
		// ����̕�������
		// ��]
		if (isRotate)
		{
			//length < attackCheckRangeMax && length > attackCheckRangeMin&&

			if (moveid.lock()->Turn(vector, turnSpeed, elapsedTime))
			{
				// �J��������
				{
					MessageData::CAMERACHANGEMOTIONMODEDATA	p;

					position = transformid.lock()->GetPosition();
					angle = transformid.lock()->GetAngle();

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

						std::shared_ptr<EnemyBoss> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemyBoss>();

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
			}
			////�@�K�E�Z�̑ΏۂɌ�������
			else
			{
			
				isRotate = false;

			}
		}
	}
	if (button)
	{

		
		
		//// �G�t�F�N�g�X�V
		//if(fire->GetEfeHandle())
		//{
		//	Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");

		//	DirectX::XMFLOAT3 pPosition =
		//	{
		//				pHPosiiton->worldTransform._41,
		//				pHPosiiton->worldTransform._42,
		//				pHPosiiton->worldTransform._43
		//	};

		//	fire->SetPosition(fire->GetEfeHandle(),
		//		pPosition);
		//}



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

			// Se
			//flameSpecialSaveSe->Stop();

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

			playerid.lock()->SetFlashOn(true);
			//owner.lock()->GetComponent<Player>()->SetHitCheck(true);

			// �A�j���[�V������~
			playerid.lock()->SetUpdateAnim
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
			playerid.lock()->
				InputSpecialMagicframe();

			// �ŏ��������@�𔭓����邽��
			startMagic = true;


			// �K�E�Z���������̌o�ߎ���
			specialMoveWaitTime += elapsedTime;

			// Se
			//flameSpecialStarteSe->Play(loopSe);

		}
	}



	// �_���[�W����
	playerid.lock()->SpecialApplyDamageInRadius();



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
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


		return;
	}

	


}

void PlayerSpecialMagicState::Exit()
{
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// ������̍ĊJ
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);


	// �G�l�~�[�Ăԓz
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyManagerCount = enemyManager.GetEnemyCount();


	// ���삳���邩�ǂ���
	if (enemyManagerCount > 0)
	{

		std::shared_ptr<EnemyBoss> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemyBoss>();
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

	//// Se
	//flameSpecialStarteSe->Stop();
	//flameSpecialSaveSe->Stop();
}

void PlayerSpecialMagicState::End()
{
	// ���
	owner.lock().reset();
}


void PlayerSpecialMagicIceState::Enter()
{
	// �R���|�l���g�擾
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

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
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);


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
	moveid.lock()->SetStopFall(stopFall);

	button = true;

	// �X�^�[�g�l
	specialMoveWaitTime = specialMoveWaitStartTime;

	// �ŏ��������@�𔭓����邽��
	startMagic = false;


	MessageData::CAMERACHANGEMOTIONMODEDATA	p;

	position = transformid.lock()->GetPosition();
	angle = transformid.lock()->GetAngle();


	// ���[�V�����L�^
	float vx = sinf(angle.y) * 6;
	float vz = cosf(angle.y) * 6;
	float vx2 = sinf(angle.y) - 10;
	float vz2 = cosf(angle.y) * 7;
	float vx3 = sinf(angle.y);

	//p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
	//p.data.push_back({ 50, {position.x + vx, position.y + 3, position.z + vz }, position });
	//p.data.push_back({ 100, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
	//p.data.push_back({ 150, {position.x - vx2, position.y + 5, position.z - vz2 }, position });
	//p.data.push_back({ 170, {position.x + vx, position.y + 3, position.z + vz }, position });
	//p.data.push_back({ 200, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
	//p.data.push_back({ 250, {position.x + vx3 , position.y + 1, (position.z + 0.1f) - vz2 }, position });

	p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
	p.data.push_back({ 50, {position.x + vx2, position.y + 3, position.z + vz }, position });
	p.data.push_back({ 100, {position.x + vx2, position.y + 5, position.z - vz2 }, position ,true });
	p.data.push_back({ 150, {position.x - vx2, position.y + 5, position.z - vz2 }, position ,true });

	p.data.push_back({ 230, {position.x + vx3 , position.y + 1, (position.z + 0.1f) - vz2 }, position ,true });
	

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

		std::shared_ptr<EnemyBoss> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemyBoss>();

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
	// �R���|�l���g�擾
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
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
				Player::Anim_Slash, loop,
				currentAnimationStartSeconds, blendSeconds
				, currentAnimationAddSeconds
			);
			//model->PlayAnimation(
			//	Player::Anim_MagicSeconde, loop,
			//	currentAnimationStartSeconds, blendSeconds
			//	, currentAnimationAddSeconds
			//);
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

			playerid.lock()->SetFlashOn(true);
			//owner.lock()->GetComponent<Player>()->SetHitCheck(true);

			// �A�j���[�V������~
			playerid.lock()->SetUpdateAnim
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
				playerid.lock()->
				InputSpecialMagicframe();

			// �ŏ��������@�𔭓����邽��
			startMagic = true;


			// �K�E�Z���������̌o�ߎ���
			specialMoveWaitTime += elapsedTime;



		}
	}



	// �_���[�W����
	playerid.lock()->SpecialApplyDamageInRadius();



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
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));


		return;
	}
}

void PlayerSpecialMagicIceState::Exit()
{
	// �R���|�l���g�擾
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// ������̍ĊJ
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);

	// �G�l�~�[�Ăԓz
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyManagerCount = enemyManager.GetEnemyCount();

	// ���삳���邩�ǂ���
	if (enemyManagerCount > 0)
	{

		std::shared_ptr<EnemyBoss> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemyBoss>();
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
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	// ������̍ĊJ
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);

	// �G�l�~�[�Ăԓz
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyManagerCount = enemyManager.GetEnemyCount();

	// ���삳���邩�ǂ���
	if (enemyManagerCount > 0)
	{

		std::shared_ptr<EnemyBoss> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemyBoss>();
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
	// �R���|�l���g�擾
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	//std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// Se�ېݒ�
	//damageSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/�����e��.wav");
	//damageSe->Play(loopSe);
	//moveid = owner.lock()->GetComponent<Movement>();

	//model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	bool loop = false;

	// ��������
	bool stopMove = false;
	moveid.lock()->SetStopMove(stopMove);

	// �d�̓I�t
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);


		//Player::Anim_Pain, loop,
	model->PlayAnimation(
		Player::Anim_Pain, loop,
		currentAnimationStartSeconds, blendSeconds);

	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerDamageState::Execute(float elapsedTime)
{
	// �R���|�l���g�擾
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	// ���b�N�I������
	playerid.lock()->UpdateCameraState(elapsedTime);

	if (!model->IsPlayAnimation())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));

	}
}

void PlayerDamageState::Exit()
{
	//damageSe->Stop();
}

void PlayerDamageState::End()
{
	// ���
	owner.lock().reset();
}

void PlayerDeathState::Enter()
{
	// �R���|�l���g�擾
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	
	float				stateTimer = 0.0f;


		//Player::Anim_Deth, loop,
	model->PlayAnimation(
		Player::Anim_Deth, loop,
		currentAnimationStartSeconds ,blendSeconds
	);

	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);

	stateTimer = 0.0f;




}

void PlayerDeathState::Execute(float elapsedTime)
{
	// �R���|�l���g�擾
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<HP> hpid = owner.lock()->GetComponent<HP>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	if (stateTimer >= stateTimerMax)
	{

		hpid.lock()->SetDead(false);

	}

	    // ���S���͂�����ۂ��J�����A���O���Ŏ��S
    // �Ⴆ�΃R����K�E�Z�Ȃǂŏ�肭���p�ł���΂����������J�������o�����܂���
    MessageData::CAMERACHANGEMOTIONMODEDATA	p;

	DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();
	DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();



    float vx = sinf(angle.y) * 6;
    float vz = cosf(angle.y) * 6;
    p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
    p.data.push_back({ 95, {position.x + vx + 13, position.y + 3, position.z + vz }, position });
	
    Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B
	//bool CollisionFlag = animationTime >= 1.5f && animationTime <= 1.6f;
	bool CollisionFlag = animationTime >= 0.5f && animationTime <= 1.6f;
	if (CollisionFlag)
	{
		++stateTimer;
		
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Stop);


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
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();


	//Player::Anim_Slash, loop
	model->PlayAnimation(
		Player::Anim_Dush, loop
		, currentAnimationStartSeconds, blendSeconds,
		currentAnimationAddSeconds);


	// �����蔻��̗L��
	//owner.lock()->GetComponent<Player>()->DmageInvalidJudment(false);



	// �A�j���[�V������� �ʏ�
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);

	// ������̒�~
	bool stopFall = true;
	moveid.lock()->SetStopFall(stopFall);


	// �G�t�F�N�g�ݒ�
	wind = std::make_unique<Effect>("Data/Effect/dashu.efk");

	// �G�t�F�N�g�Đ�����
	wind->Play({ 
		transformid.lock()->GetPosition().x,
		transformid.lock()->GetPosition().y + addHeight,
		transformid.lock()->GetPosition().z
		});


	// �p�x�ύX
	wind->SetAngle(wind->GetEfeHandle(), transformid.lock()->GetAngle());

	// Se�ېݒ�
	//dushSe = Audio::Instance().LoadAudioSource("Data/Audio/SE/�����e��.wav");
	//dushSe->Play(loopSe);
}

// ����X�V
void PlayerAvoidanceState::Execute(float elapsedTime)
{
	// �R���|�[�l���g�Ăяo��
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// �������R
	bool stopMove = false;
	moveid.lock()->SetStopMove(stopMove);

	// ���b�N�I������
	owner.lock()->GetComponent<Player>()->UpdateCameraState(elapsedTime);

	// �]����_�b�V��
	DirectX::XMFLOAT3 dir;
	dir.x = sinf(transformid.lock()->GetAngle().y);
	dir.y = cosf(transformid.lock()->GetAngle().x);
	dir.z = cosf(transformid.lock()->GetAngle().y);

	
	// �C�ӂ̃A�j���[�V�����Đ����
	float animationTime = owner.lock()->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	
	// �ʒu�X�V
	wind->SetPosition(wind->GetEfeHandle(),
		{
			transformid.lock()->GetPosition().x,
			transformid.lock()->GetPosition().y + addHeight,
			transformid.lock()->GetPosition().z
		});

	//if (animationTime + FLT_EPSILON >= 0.7f + FLT_EPSILON && animationTime <= 0.71f)
	//if (animationTime + FLT_EPSILON >= 0.7f + FLT_EPSILON && animationTime <= 0.71f)

	
	// �n��_�b�V��
	if (animationTime >= 0.7f && animationTime <= 0.8f && moveid.lock()->GetOnLadius())
	{
		DirectX::XMFLOAT3 impulse =
		{
			dir.x * speed,
			0,
			dir.z * speed,
		};

		moveid.lock()->AddImpulse(impulse);

		//moveid->Move(dir, moveSpeed, elapsedTime);
	}
	// �󒆃_�b�V��
	if (animationTime >= 0.7f  && animationTime <= 0.8f && !moveid.lock()->GetOnLadius())
	{
		//moveid->Move(dir, moveSpeed, elapsedTime);

		DirectX::XMFLOAT3 impulse =
		{
			dir.x * flySpeed,
			0,
			dir.z * flySpeed,
		};
	
		moveid.lock()->AddImpulse(impulse);
		
	}

	// �A�j���[�V�����_�b�V����ʏ�ړ�
	if (animationTime >= 1.0f && owner.lock()->GetComponent<Player>()->InputMove(elapsedTime))
	{

		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	}
	
	// �A�j���[�V�����I��
	//if (!owner.lock()->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	if (animationTime >= 2.0f)
	{

		// �����蔻��̗L��
		//owner.lock()->GetComponent<Player>()->DmageInvalidJudment(true);
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));



	}
}

// ����I��
void PlayerAvoidanceState::Exit()
{
	// �R���|�[�l���g�Ăяo��
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();

	// ������̒�~
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);

	wind->Stop(wind->GetEfeHandle());

	// se
	//dushSe->Stop();
}

void PlayerAvoidanceState::End()
{
	// ���
	owner.lock().reset();
}

// ���ˊJ�n
void PlayerReflectionState::Enter()
{


	// �R���|�[�l���g�Ăяo��
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	bool loop = false;



		//Player::Anim_Counter, loop,
	model->PlayAnimation(
		Player::Anim_Counter, loop,
		currentAnimationStartSeconds, blendSeconds);

	// �A�j���[�V������� �ʏ�
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);

	// Se�ېݒ�
	//reflectionStop = Audio::Instance().LoadAudioSource("Data/Audio/SE/�q�b�g�X�g�b�v.wav");


}

// ���ˍX�V
void PlayerReflectionState::Execute(float elapsedTime)
{
	// �R���|�[�l���g�Ăяo��
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();

	if (!model->IsPlayAnimation())
	{
		// �����蔻��̗L��
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));

	}


	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B
	bool CollisionFlag = animationTime >= 0.2f && animationTime <= 0.4f;

	if (CollisionFlag)
	{
		// ����m�[�h�ƃG�l�~�[�̏Փˏ���
		playerid.lock()->CollisionNodeVsEnemiesCounter("mixamorig:LeftHand", owner.lock()->GetComponent<Player>()->GetLeftHandRadius());
		playerid.lock()->CollisionNodeVsRubyCounter("mixamorig:LeftHand", owner.lock()->GetComponent<Player>()->GetLeftHandRadius());
		playerid.lock()->CollisionNodeVsRubyCounterBulletFring("mixamorig:LeftHand", owner.lock()->GetComponent<Player>()->GetLeftHandRadius());
		
	}
	//if (playerid.lock()->GetHitCheck())
	//{
	//	//reflectionStop->Play(loopSe);
	//}
}

// ���ˏI��
void PlayerReflectionState::Exit()
{
	//reflectionStop->Stop();
}

void PlayerReflectionState::End()
{
	// ���
	owner.lock().reset();
}


