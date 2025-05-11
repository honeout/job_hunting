#include <math.h>
#include "EnemyBoss.h"
#include "StateDerived.h"
#include "Player.h"
#include "Mathf.h"
#include "ProjectileHoming.h"
#include "ProjectileFullHoming.h"
#include "ProjectileStraight.h"
#include "ProjectileManager.h"
#include "BulletFiring.h"
#include "SceneLoading.h"
#include "SceneManager.h"
#include "SceneGameOver.h"
#include "SceneGame.h"

// �p�j�X�e�[�g�ɓ��������̃��\�b�h
void WanderState::Enter()
{
	// �R���|�l���g�擾
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// owner�Ăяo��
	std::shared_ptr<EnemyBoss> enemyid =sharedId->GetComponent<EnemyBoss>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	// �꒣��
	if (!enemyid) return;
	enemyid->SetRandomTargetPosition();
	// �U����
	attackCount = attackCountMax;
	// �o�����̐ݒ�
	if (!enemyid) return;
	if (enemyid->GetIsEnemyAwakened())
	{
		attackCount = attackCountMin;
		modelAnim.keyFrameEnd = 288;
	}

	// �A�j���[�V�����ݒ�
	modelAnim.index = (int)EnemyBoss::Animation::Anim_Movie;
	modelAnim.loop = false;
	modelAnim.currentanimationseconds = 3.3f;
	modelAnim.blendSeconds = 0.7f;
	modelAnim.currentAnimationAddSeconds = 0.008f;
	modelAnim.keyFrameEnd = 300.0f;

	// �A�j���[�V�����Đ�
	model->
		PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	if (!enemyid) return;
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	// ���n�u��
	upOnLading = false;
	// ���G�t�F�N�g
	smorker = std::make_unique<Effect>("Data/Effect/smorkeDash.efk");
}

// �p�j�X�e�[�g�Ŏ��s���郁�\�b�h
void WanderState::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;
	// owner�Ăяo��
	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();

	// �ڕW�n�_�����v���C���[�ʒu�ɐݒ�
	targetPosition = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();
	enemyid->SetTargetPosition(targetPosition);
	DirectX::XMVECTOR targetPositionVec = DirectX::XMLoadFloat3(&targetPosition);
	DirectX::XMVECTOR enemyPositionVec = DirectX::XMLoadFloat3(&enemyid->GetPosition());
	DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(targetPositionVec, enemyPositionVec);
	DirectX::XMFLOAT3 direction;
	DirectX::XMStoreFloat3(&direction, directionVec);

	// �{�X�̊o���A�j���[�V�����I��;
	if (!model->IsPlayAnimation() && enemyid->GetIsEnemyAwakened() && attackCount < attackCountMax)
	{
		++attackCount;
		// �A�j���[�V�����I���L�[�t���[��
		modelAnim.currentAnimationAddSeconds = 0.00f;
		modelAnim.keyFrameEnd = 288;

		// �A�j���[�V�����Đ�
		model->
			PlayAnimation(modelAnim);
	}

	// �A�j���[�V�����I���@�ҋ@��
	if (!model->IsPlayAnimation())
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
	}

	// �ړ�
	if (animationTime <= 4.6f)
	{
		moveid->Turn(direction, turnSpeed, elapsedTime);
		moveid->Move(direction, moveSpeed, elapsedTime);
		// �|�X�g�G�t�F�N�g�C���X�^���X�Q�b�g
		PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
		// ���W�A���u���[
		radialBlurData.radius = 30.0f;
		postprocessingRenderer.SetRadialBlurData(radialBlurData);
	}

	// �ŏ��̑��x���x����ɒʏ�ɖ߂�
	if (animationTime >= slowWalkTime)
	{
		modelAnim.currentAnimationAddSeconds = 0.00f;
		model->AnimationRuleChanger(modelAnim.currentAnimationAddSeconds);
	}
	// ��ύX
	// �\�����쒆�͓����蔻�薳��
	else
		return;

	// �ړI�n�ɂ��̔��a��������
	float radius = enemyid->GetAttackRightFootRange();
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
		// �E��
		DirectX::XMFLOAT3 bossRightFootPosition;
		bossRightFootPosition = model->ConvertLocalToWorld(bossRightFoot);
		// �E�r
		DirectX::XMFLOAT3 bossRightHandPosition;
		bossRightHandPosition = model->ConvertLocalToWorld(bossRightHand);
		// ���r
		DirectX::XMFLOAT3 bossLeftHandPosition;
		bossLeftHandPosition = model->ConvertLocalToWorld(bossLeftHand);
		// �����n�ʂɂ�
		if (animationTime + FLT_EPSILON >= 4.0f - FLT_EPSILON &&
			animationTime - FLT_EPSILON <= 4.1f + FLT_EPSILON)
		{
			// ��
			enemyid->InputJampSe();
			smorker->Play(bossRightFootPosition, scaleEffect);
			// ����
			enemyid->DetectHitByBodyPart(bossLeftFootPosition, applyDamage);
			// �E��
			enemyid->DetectHitByBodyPart(bossRightFootPosition, applyDamage);
			// �E�r
			enemyid->DetectHitByBodyPart(bossRightHandPosition, applyDamage);
			// ���r
			enemyid->DetectHitByBodyPart(bossLeftHandPosition, applyDamage);
			// ���S
			enemyid->DetectHitByBodyPart(transformid->GetPosition(), applyDamage);
		}

		// �����n�ʂɂ�
		if (animationTime + FLT_EPSILON >= 4.6f - FLT_EPSILON &&
			animationTime - FLT_EPSILON <= 4.7f + FLT_EPSILON)
		{
			// ��
			enemyid->InputJampSe();
			smorker->Play(bossLeftFootPosition, scaleEffect);
			// ����
			enemyid->DetectHitByBodyPart(bossLeftFootPosition, applyDamage);
			// �E��
			enemyid->DetectHitByBodyPart(bossRightFootPosition, applyDamage);
			// �E�r
			enemyid->DetectHitByBodyPart(bossRightHandPosition, applyDamage);
			// ���r
			enemyid->DetectHitByBodyPart(bossLeftHandPosition, applyDamage);
			// ���S
			enemyid->DetectHitByBodyPart(transformid->GetPosition(), applyDamage);
		}
	}
}

// �p�j�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void WanderState::Exit()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;
	// owner�Ăяo��
	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	// �A���U��
	attackCount = attackCountMin;
	enemyid->InputSlashSe();
}

// ������
void IdleState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;
	// owner�Ăяo��
	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<HP> hp = sharedId->GetComponent<HP>();
	modelAnim.index = EnemyBoss::Anim_Standby;
	modelAnim.loop = true;
	modelAnim.currentanimationseconds = 0.0f;
	modelAnim.blendSeconds = 0.7f;
	model->PlayAnimation(modelAnim);

	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	stateTimer = stateTimerMax;

	// ���������������
	hp->ResetOnDamageThresholdTime();
	randRoot = rand() % 2;
}

// update
void IdleState::Execute(float elapsedTime)
{
	// �������Ԍo��
	 stateTimer -= elapsedTime;

	 auto sharedId = owner.lock();
	 if (!sharedId)
		 return;

	 std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	 Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	 std::shared_ptr <HP> hp = sharedId->GetComponent<HP>();
	 // �������ԉ߂�����
	if (stateTimer <= stateTimerEnd)
	{
		// playerSearch
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Pursuit));
	}

	// ���_���[�W�̍ő�l
	float damageThreshold = 10;
	float timeLimit = 5;

	// ����
	if (hp->CheckDamageThresholdWithinTime(elapsedTime, damageThreshold, timeLimit)&&
		randRoot == (int)EnemyBoss::ExitRoot::ExitJamp)
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Jump));
	}

	// �_���[�W���[�V�����΍�
	if (!model->IsPlayAnimation())
	{
		model->PlayAnimation(modelAnim);
	}
}

// �Ō�ɂ�肽�������S��
void IdleState::Exit()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;
	std::shared_ptr<HP> hp = sharedId->GetComponent<HP>();
	// ���������������
	hp->ResetOnDamageThresholdTime();
}

// ������
void PursuitState::Enter()
{
	// �A�j���[�V�����ݒ�
	modelAnim.index = EnemyBoss::Animation::Anim_Standby;
	modelAnim.loop = true;
	modelAnim.currentanimationseconds = 0.0f;
	modelAnim.blendSeconds = 0.7f;

	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// �G�l�~�[�A�N�Z�X�p
	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	enemyid->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		modelAnim);

	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);

	// �U���̎�ނ��o��
	randamAttack = rand() % 3;
}
// update
void PursuitState::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// �G�l�~�[�A�N�Z�X�p
	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	// �ڕW�n�_�����v���C���[�ʒu�ɐݒ�
	DirectX::XMFLOAT3 targetPosition = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();
	enemyid->SetTargetPosition(targetPosition);
	float vx = targetPosition.x - enemyid->GetPosition().x;
	float vy = targetPosition.y - enemyid->GetPosition().y;
	float vz = targetPosition.z - enemyid->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
	// �U���̎��
	switch (randamAttack)
	{
	case (int)EnemyBoss::AttackMode::AssaultAttack:
	{
		enemyid->GetStateMachine()->ChangeState(
			static_cast<int>(EnemyBoss::State::Wander));
		break;
	}
	case (int)EnemyBoss::AttackMode::JumpStompAttack:
	{
		enemyid->GetStateMachine()->ChangeState(
			static_cast<int>(EnemyBoss::State::Jump));
		break;
	}
	case (int)EnemyBoss::AttackMode::DushAttack:
	{
		enemyid->GetStateMachine()->ChangeState(
			static_cast<int>(EnemyBoss::State::Attack));
		break;
	}
	}
}
// �Ō�ɂ�肽�������S��
void PursuitState::Exit()
{
}

// ������
void JumpState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	// �A�j���[�V�����ݒ�
	modelAnim.index = EnemyBoss::Animation::Anim_Attack;
	modelAnim.loop = true;
	modelAnim.currentanimationseconds = 2.0f;
	modelAnim.blendSeconds = 0.7f;
	modelAnim.currentAnimationAddSeconds = 0.01f;

	Model* model =  sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<HP> hpid = sharedId->GetComponent<HP>();
	// �꒣��
	enemyid->SetRandomTargetPosition();
	// �A�j���[�V�����Đ�
	model->
		PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Reverseplayback);
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
	// ���G�t�F�N�g
	smorker = std::make_unique<Effect>("Data/Effect/smorkeDash.efk");
}

// ��ύX
// �X�V����
void JumpState::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<HP> hpid = sharedId->GetComponent<HP>();
	enemyid->SetTargetPosition(targetPosition);
	DirectX::XMVECTOR targetPositionVec = DirectX::XMLoadFloat3(&targetPosition);
	DirectX::XMVECTOR enemyPositionVec = DirectX::XMLoadFloat3(&enemyid->GetPosition());
	DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(targetPositionVec, enemyPositionVec);
	DirectX::XMFLOAT3 direction;
	DirectX::XMStoreFloat3(&direction, directionVec);
	// �ړI�n�ɂ��̔��a��������
	float radius = enemyid->GetAttackRightFootRange();
	// �����{�[��
	Model::Node* bossLeftFoot = model->FindNode("boss_left_foot1");
	// �m�[�h�ʒu�擾
	// ����
	DirectX::XMFLOAT3 bossLeftFootPosition;
	bossLeftFootPosition = model->ConvertLocalToWorld(bossLeftFoot);
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();
	// ��ύX
	if (animationTime - FLT_EPSILON <= 0.8f + FLT_EPSILON && !jumpStart)
	{
		bool blurCheck = true;
		enemyid->SetBlurCheck(blurCheck);
		enemyid->CollisionPlayerWithRushEnemy();
		moveid->JumpVelocity(jumpSpeed);
		jumpStart = true;
		smorker->Play(bossLeftFootPosition, scaleEffect);
		PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance().Instance();
		// ���W�A���u���[
		radialBlurData.radius = 15;
		postprocessingRenderer.SetRadialBlurData(radialBlurData);
		return;
	}
	else
	{
		moveid->Turn(direction, turnSpeed, elapsedTime);
	}

	if (!moveid->GetOnLadius())
	{
		moveid->Move(direction, moveSpeed, elapsedTime);
		moveid->Turn(direction, turnSpeed, elapsedTime);
	}

	if (moveid->GetOnLadius() && jumpStart)
	{
		enemyid->InputImpact(enemyid->GetPosition());
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
		enemyid->InputJump();
		// ���G�t�F�N�g
		smorker->Play(bossLeftFootPosition, scaleEffect);
		PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance().Instance();
		// ���W�A���u���[
		radialBlurData.radius = 30;
		postprocessingRenderer.SetRadialBlurData(radialBlurData);
		 // ���������蔻��
		enemyid->DetectHitByBodyPart(bossLeftFootPosition, applyDamageJumpStart);
	}
}
// �I������
void JumpState::Exit()
{
}

// ������
void AttackState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<HP> hpid = sharedId->GetComponent<HP>();
	// �꒣��
	enemyid->SetRandomTargetPosition();
	modelAnim.index = EnemyBoss::Animation::Anim_Attack;
	modelAnim.currentanimationseconds = 2.8f;
	modelAnim.loop = false;
	modelAnim.blendSeconds = 0.7f;
	modelAnim.currentAnimationAddSeconds = 0.015f;
	modelAnim.keyFrameEnd = 0.0f;
	// �A�j���[�V�����Đ�
	model->
		PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Reverseplayback);
	// �A�j���[�V�����I��
	dushStart = false;
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
	// ���G�t�F�N�g
	smorker = std::make_unique<Effect>("Data/Effect/smorkeDash.efk");
	charge = std::make_unique<Effect>("Data/Effect/effectCharge.efk");
	chargeCompleate = std::make_unique<Effect>("Data/Effect/chargecompleted.efk");
}

// update
void AttackState::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
    std::shared_ptr<HP> hpid = sharedId->GetComponent<HP>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	if (PlayerManager::Instance().GetPlayerCount() >= 1)
	{
		std::shared_ptr<Transform> pTransformId = PlayerManager::Instance().GetPlayer(0)->GetComponent<Transform>();

		targetPosition = pTransformId->GetPosition();
	}
	DirectX::XMVECTOR targetPositionVec = DirectX::XMLoadFloat3(&targetPosition);
	DirectX::XMVECTOR enemyPositionVec = DirectX::XMLoadFloat3(&enemyid->GetPosition());
	DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(targetPositionVec, enemyPositionVec);
	DirectX::XMStoreFloat3(&directionP, directionVec);
	angle = transformid->GetAngle();
	direction.x = sinf(angle.y);// �O�p���΂߂ɂ��Ĉʒu��ς���
	direction.y = 0;
	direction.z = cosf(angle.y);
	// �ړI�n�ɂ��̔��a��������
	float radius = enemyid->GetAttackRightFootRange();
	// �{�[�����T��
	Model::Node* bossRightFoot = model->FindNode("boss_right_foot1");
	Model::Node* bossLeftFoot = model->FindNode("boss_left_foot1");
	Model::Node* bossEye = model->FindNode("body2");
	// �m�[�h�ʒu�擾
	// ����
	DirectX::XMFLOAT3 bossRightFootPosition;
	bossRightFootPosition = model->ConvertLocalToWorld(bossRightFoot);
	DirectX::XMFLOAT3 bossLeftFootPosition;
	bossLeftFootPosition = model->ConvertLocalToWorld(bossLeftFoot);
	DirectX::XMFLOAT3 bossEyePosition = model->ConvertLocalToWorld(bossEye);
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();
	// �`���[�W��
	if (animationTime - FLT_EPSILON >= 2.3f + FLT_EPSILON &&
		animationTime - FLT_EPSILON <= 2.4f + FLT_EPSILON && !chargeInitilize)
	{
		enemyid->InputJampSe();
		enemyid->InputChargeSe();
		charge->Play(bossEyePosition);
		chargeCompleate->Play(bossEyePosition);
		// �`���[�W��
		stateChargeTimer = stateChargeTimerMax;
		chargeInitilize = true;
		// �A�j���[�V�������[��
		enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Stop);
	}

	// �_�b�V������܂�
	if (!dushStart)
	{
		stateChargeTimer -= 0.01f;
		moveid->Turn(directionP, turnSpeed, elapsedTime);
	}

	// �_�b�V��������
	if (chargeInitilize && stateChargeTimer <= stateChargeCompletionTimerEnd && !dushStart)
	{
		modelAnim.index = EnemyBoss::Animation::Anim_Walk;
		modelAnim.currentanimationseconds = 0.0f;
		modelAnim.loop = true;
		modelAnim.currentAnimationAddSeconds = 0.2f;
		modelAnim.keyFrameEnd = 0.0f;
		// �A�j���[�V�����Đ�
		model->
			PlayAnimation(modelAnim);
		// �A�j���[�V�������[��
		enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
		// �`���[�W������̃_�b�V������
		stateChargeCompletionTimer = stateChargeCompletionTimerMax;
	}

	// �`���[�W����
	if (chargeInitilize && stateChargeTimer <= stateChargeCompletionTimerEnd)
	{
		dushStart = true;
	}
	
	// �_�b�V����
	if (dushStart)
	{
		enemyid->InputDashSe();
		charge->Stop(charge->GetEfeHandle());
		// ���G�t�F�N�g
		smorker->Play(bossLeftFootPosition, scaleEffect);
		moveid->Move(direction, moveSpeed, elapsedTime);
		stateChargeCompletionTimer -= 0.01f;
		// �E�������蔻��
		enemyid->DetectHitByBodyPart(bossRightFootPosition, applyDamageAttack);
		// ���������蔻��
		enemyid->DetectHitByBodyPart(bossLeftFootPosition, applyDamageAttack);
	}

	// �_�b�V���I��
	if (dushStart && stateChargeCompletionTimer - FLT_EPSILON <= stateChargeCompletionTimerEnd + FLT_EPSILON)
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
	}
}

// ��ύX
// �Ō�ɂ�肽�������S��
void AttackState::Exit()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	enemyid->InputStopDashSe();
	chargeInitilize = false;
	if (charge->GetEfeHandle())
		charge->Stop(charge->GetEfeHandle());
	if (chargeCompleate->GetEfeHandle())
		chargeCompleate->Stop(chargeCompleate->GetEfeHandle());
}

// ��ύX
// �_���[�W����
void DamageState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	enemyid->InputDamageSe();
	modelAnim.index = EnemyBoss::Animation::Anim_Die;
	modelAnim.loop = true;
	modelAnim.currentanimationseconds = 2.5f;
	modelAnim.blendSeconds = 0.7f;
	modelAnim.currentAnimationAddSeconds = 0.025f;
	modelAnim.keyFrameEnd = 3.0f;
	Model* momdel = sharedId->GetComponent<ModelControll>()->GetModel();
	momdel->
		PlayAnimation(
			modelAnim);
	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	stateTimer = stateTimerMax;
}

void DamageState::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	--stateTimer;
	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
		if (stateTimer < stateTimerEnd)
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
}

void DamageState::Exit()
{
}
// �����J�n
void ConfusionState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = EnemyBoss::Animation::Anim_Die;
	modelAnim.loop = false;
	modelAnim.currentanimationseconds = 0.426f;
	modelAnim.blendSeconds = 0.35f;
	modelAnim.currentAnimationAddSeconds = 0.000f;
	modelAnim.keyFrameEnd = 70.0f;

	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	// �X�e�[�g�ҋ@����
	stateTimer = stateTimerMax;
	// �U�����
	randamAttack = rand() % 2;
}
// �����X�V
void ConfusionState::Execute(float elapsedTime)
{
	stateTimer -= elapsedTime;

	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	if (!model->IsPlayAnimation())
		model->PlayAnimation(modelAnim);
	if (stateTimer > stateTimerEnd)return;
	switch (randamAttack)
	{
	case (int)EnemyBoss::AttackMode::AssaultAttack:
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Wander));
		break;
	}
	case (int)EnemyBoss::AttackMode::JumpStompAttack:
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Jump));
		break;
	}
	default:
		break;
	}
}
// �����I��
void ConfusionState::Exit()
{
}
// �|���
void DeathState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	modelAnim.index = EnemyBoss::Animation::Anim_Die;
	modelAnim.loop = false;
	Model* model;
	model = sharedId->GetComponent<ModelControll>()->GetModel();
	model->
		PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	stateTimer = 3;
	bool freeCheckCamera = false;
	int playerCountMax = PlayerManager::Instance().GetPlayerCount();
	if (playerCountMax > 0)
	PlayerManager::Instance().GetPlayer(playerCountMax - 1)->GetComponent<Player>()->SetFreeCameraCheck(freeCheckCamera);
}

void DeathState::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	stateTimer -= elapsedTime; 
	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	// ���S���͂�����ۂ��J�����A���O���Ŏ��S
// �Ⴆ�΃R����K�E�Z�Ȃǂŏ�肭���p�ł���΂����������J�������o�����܂���
	MessageData::CAMERACHANGEMOTIONMODEDATA	p;
	DirectX::XMFLOAT3 position = transformid->GetPosition();
	DirectX::XMFLOAT3 angle = transformid->GetAngle();
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();
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
}

// �������
void AwakeStartState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = EnemyBoss::Animation::Anim_Movie;
	modelAnim.loop = false;
	modelAnim.currentanimationseconds = 5.829f;
	modelAnim.blendSeconds = 0.35f;
	modelAnim.currentAnimationAddSeconds = 0.025f;
	modelAnim.keyFrameEnd = 3.0f;
	model->
		PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
}

void AwakeStartState::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();
	// �A�j���[�V�����̃|�[�Y
	if (animationTime >= animationPose)
	{
		MessageData::CAMERASHAKEDATA p;
		p.shakePower = cameraShakePower;
		p.shakeTimer = cameraShakeTime;
		Messenger::Instance().SendData(MessageData::CAMERASHAKE, &p);
		// �|�X�g�G�t�F�N�g�C���X�^���X�Q�b�g
		PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
		radialBlurData.radius = 30.0f;
		postprocessingRenderer.SetRadialBlurMaxData(radialBlurData);
	}

	// �A�j���[�V�����I��
	if (!model->IsPlayAnimation())
	{
		enemyid->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
	}
}

void AwakeStartState::Exit()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	// �o������
	enemyid->ResetAwakeTime();
}
// �N���A�|��đҋ@��
void ClearState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	// �ʒu���
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	DirectX::XMFLOAT3 position = transformid->GetPosition();
	position.z -= 5.5f;
	transformid->SetPosition(position);
	DirectX::XMFLOAT3 angle = transformid->GetAngle();
	angle.y += 1.5f;
	transformid->SetAngle(angle);
	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
	modelAnim.index = EnemyBoss::Animation::Anim_Die;
	modelAnim.loop = false;
	modelAnim.blendSeconds = 0.7f;
	Model* model;
	model = sharedId->GetComponent<ModelControll>()->GetModel();
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	stateTimer = 3;
}

void ClearState::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	GamePad& gamePad = Input::Instance().GetGamePad();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	stateTimer -= elapsedTime;
	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();
	const GamePadButton anyButton =
		GamePad::BTN_B;
	if (!model->IsPlayAnimation() )// ���[�h�̎��Q�[���Ƃ���������
	{
		enemyid->SetClearCheck(clearCheck);
	}
}

void ClearState::Exit()
{
}
// �N���A��������
void ClearReviveState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<EnemyBoss> enemyid = sharedId->GetComponent<EnemyBoss>();
	// �ʒu���
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	DirectX::XMFLOAT3 position = transformid->GetPosition();
	position.z -= 5.5f;
	transformid->SetPosition(position);
	DirectX::XMFLOAT3 angle = transformid->GetAngle();
	angle.y += 1.5f;
	transformid->SetAngle(angle);
	modelAnim.index = EnemyBoss::Animation::Anim_Die;
	modelAnim.loop = false;
	modelAnim.currentanimationseconds = 1.5f;
	modelAnim.blendSeconds = 0.7f;
	Model* model;
	model = sharedId->GetComponent<ModelControll>()->GetModel();
	model->
		PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	enemyid->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	stateTimer = 3;
}

void ClearReviveState::Execute(float elapsedTime)
{
}

void ClearReviveState::Exit()
{
}

// �v���C���[�ҋ@
void PlayerIdleState::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Anim_Idle;
	modelAnim.loop = true;
	modelAnim.blendSeconds = 0.5f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	moveid->SetStopMove(false);
}

void PlayerIdleState::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	// ���b�N�I������
	playerid->UpdateCameraState(elapsedTime);
	// �ړ����͏���
	if (playerid->InputMove())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	}
	// �����͏���
	if (playerid->InputAvoidance())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}
	// �W�����v���͏���
	if (playerid->InputJump())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
	}
}

void PlayerIdleState::Exit()
{
}
// �v���C���[�ړ�
void PlayerMovestate::Enter()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	// se�Đ�
	audioParam.filename = "Data/Audio/SE/����.wav";
	audioParam.loop = loopSe;
	audioParam.volume = volumeSe;
	playerid->InputSe(audioParam);

	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Anim_Running;
	modelAnim.loop = true;
	modelAnim.blendSeconds = 0.7f;
	modelAnim.currentAnimationAddSeconds = 0.025f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	// ������
	moveid->SetStopMove(false);
}

void PlayerMovestate::Execute(float elapsedTime)
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	// �ړ��l
	playerid->GetMoveVec(elapsedTime);
	// ���b�N�I������
	playerid->UpdateCameraState(elapsedTime);
	// �ړ����͏���
	if (!playerid->InputMove())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
	}
	// �����͏���
	if (playerid->InputAvoidance())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}
	// �W�����v���͏���
	if (playerid->InputJump())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
	}
}

void PlayerMovestate::Exit()
{
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	//se�Đ��폜
	playerid->StopSe(audioParam);
}

// �v���C���[�W�����v
void PlayerJumpState::Enter()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Animation::Anim_Jump;
	modelAnim.keyFrameEnd = 46.0f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	// ������
	moveid->SetStopMove(false);
	// se�쐬
	playerid->InputJampSE();
}

void PlayerJumpState::Execute(float elapsedTime)
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	// ���b�N�I������
	playerid->UpdateCameraState(elapsedTime);
	// �ړ�
	if (playerid->InputMove())
	{
		playerid->GetMoveVec(elapsedTime);
	}
	// �W�����v���͏���
	if (playerid->InputJump())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
	}
	// ���
	if (playerid->InputAvoidance())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}
	// ���n
	if (moveid->GetOnLadius())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Land));
	}
	// ���n�ړ�
	if (moveid->GetOnLadius() && playerid->InputMove())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	}
}

void PlayerJumpState::Exit()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	// �W�����v��
	playerid->InputJampSE();
}
// �v���C���[���n
void PlayerLandState::Enter()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Anim_Land;
	modelAnim.currentanimationseconds = 0.35f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	// se�쐬
	playerid->InputJampSE();
}

void PlayerLandState::Execute(float elapsedTime)
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	// ���b�N�I������
	playerid->UpdateCameraState(elapsedTime);
	// ����
	if (playerid->InputMove())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	}
	// ���
	if (playerid->InputAvoidance())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}
	// �ҋ@
	if (!model->IsPlayAnimation())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
	}
}

void PlayerLandState::Exit()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	// �W�����v��
	playerid->InputJampSE();
}

// ��ύX
// �v���C���[�W�����v��
void PlayerJumpFlipState::Enter()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Anim_Jump;
	modelAnim.currentAnimationAddSeconds = 0.03f;
	modelAnim.keyFrameEnd = 25.0f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	// se�쐬
	playerid->InputJampSE();
}

void PlayerJumpFlipState::Execute(float elapsedTime)
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	// ���b�N�I������
	playerid->UpdateCameraState(elapsedTime);
	// �ړ�
	if (playerid->InputMove())
	{
		bool afterimagemove = true;
	}
	// �W�����v���͏���
	if (!model->IsPlayAnimation())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
	}
	// ���
	if (playerid->InputAvoidance())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}
}

void PlayerJumpFlipState::Exit()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	playerid->InputStopJampSE();
}
// �v���C���[����
void PlayerQuickJabState::Enter()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	tellePort = std::make_unique<Effect>("Data/Effect/telleport.efk");

	if (!InitializationCheck)
	{
		// �a�����ڃ{�^��B
		commandSecondeButtonB.push_back(GamePad::BTN_B);
		commandSecondeButtonB.push_back(GamePad::BTN_B);
		// �a�����ڃ{�^���\���L�[
		commandSecondeButtonRight.push_back(GamePad::BTN_RIGHT);
		commandSecondeButtonRight.push_back(GamePad::BTN_RIGHT);
	}
	
	// �d�̓I�t
	bool stopFall = true;
	moveid->SetStopFall(stopFall);

	// ��]���邩�`�F�b�N
	rotateCheck = false;

	// �_���[�W�H��������ɍU�����P����ɖ߂�����
	deleteCheck = true;

	InitializationCheck = true;

	// �`��
	isPlayerDrawCheck = 0;

	// �z�[�~���O���邩
	isHomingStartCheck = true;

	// �G�l�~�[��ڐG
	isEnemyHit = false;
	// �ڐG
	EnemyHit = true;
	// ��ڐG
	EnemySafe = false;
}

// ��ύX
void PlayerQuickJabState::Execute(float elapsedTime)
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	GamePad& gamePad = Input::Instance().GetGamePad();
	// �R���|�l���g�擾
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	// ���b�N�I������
	playerid->UpdateCameraState(elapsedTime);

	if (playerid->InputAttack() &&
		playerid->GetSelectCheck() ==
		(int)Player::CommandAttack::Attack)
	{
		// �R�}���h�m�F2�e�U��
		if (gamePad.ConfirmCommand(commandSecondeButtonB, frame) && !button && !buttonSeconde)
		{
			button = true;
		}

		if (gamePad.ConfirmCommand(commandSecondeButtonRight, frame) && !button && !buttonSeconde)
		{
			button = true;
		}
	}

	// ���
	if (playerid->InputAvoidance() && playerid->InputMove())
	{
		bool stop = false;
		moveid->SetStopMove(stop);
		DirectX::XMFLOAT3 vec;
		vec = playerid->GetMoveVec(elapsedTime);
		moveid->Turn(vec, turnSpeed, elapsedTime);
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
		button = false;
		buttonSeconde = false;
		return;
	}
	DirectX::XMVECTOR Vector;
	DirectX::XMVECTOR VectorXZ;
	DirectX::XMVECTOR LengthSq;
	playerPosXZ.x = transformid->GetPosition().x;
	playerPosXZ.y = transformid->GetPosition().z;
	DirectX::XMVECTOR playerPositionXZ = DirectX::XMLoadFloat2(&playerPosXZ);
	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid->GetPosition());
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
	{
		enemyPosXZ.x = enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition().x;
		enemyPosXZ.y = enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition().z;
		enemyPos = enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition();
		DirectX::XMVECTOR enemyPositionXZ = DirectX::XMLoadFloat2(&enemyPosXZ);
		DirectX::XMVECTOR enemyPosition = DirectX::XMLoadFloat3(&enemyPos);

		VectorXZ = DirectX::XMVectorSubtract(enemyPositionXZ, playerPositionXZ);
		Vector = DirectX::XMVectorSubtract(enemyPosition, playerPosition);

		LengthSq = DirectX::XMVector2Length(VectorXZ);

		Vector = DirectX::XMVector3Normalize(Vector);

		DirectX::XMStoreFloat3(&vector, Vector);

		DirectX::XMStoreFloat(&length, LengthSq);
	}

	// �U���ʍő�
	if (attackMemory > attackMemoryMax ||!model->IsPlayAnimation())
	{
		// ���͊m�F�ŃX�e�[�g�ύX
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
		// �U���ő�l����
		attackMemory = 0;
	}

	// ����̕�������
	// ��]
	if (!rotateCheck)
	{
		isEnemyHit = !playerid->GetIsEnemyHit() && !playerid->GetIsEnemyHitBody() ?
			EnemySafe : EnemyHit;
		// ����
		if (length < attackCheckRange && !isEnemyHit)
		{
			bool stop = false;
			moveid->SetStopMove(stop);
			// ����
			isPlayerDrawCheck = 0;
			playerid->SetPlayeDrawCheck(isPlayerDrawCheck);
			// ����
			moveid->Turn(vector, turnSpeed, elapsedTime);
			moveid->Move(vector, speed, elapsedTime);

			// �z�[�~���O���̂ݍĐ�
			if (isHomingStartCheck)
			{
				// Se�Đ�
				playerid->PlayTellePortSe();
				// �G�t�F�N�g
				tellePort->Play(transformid->GetPosition());
				// �z�[�~���O���݂̂̂���
				isHomingStartCheck = false;
			}

			return;
		}
		else
		{
			// �`��
			isPlayerDrawCheck = 1;
			playerid->SetPlayeDrawCheck(isPlayerDrawCheck);
			bool stop = true;
			moveid->SetStopMove(stop);
			rotateCheck = true;
			// �z�[�~���O�I��
			isHomingStartCheck = false;

			// �A�j���[�V�����Đ�
			// ��
			if (!moveid->GetOnLadius())
			{
				//// �󒆍U���ōs���K������
				//playerid->AreAttackDecreaseAmount();
				// �A�j���[�V�������[��
				Model::ModelAnim modelAnim;
				Model::ModelAnim modelAnimUpperBody;
				modelAnim.index = Player::Anim_Jump;
				modelAnimUpperBody.index = Player::Anim_Slash;
				modelAnimUpperBody.currentAnimationAddSeconds = 0.030f;
				modelAnim.loop = true;
				modelAnim.currentAnimationAddSeconds = 0.03f;
				// �A�j���[�V�����Đ�
				model->PlayAnimation(modelAnim);
				// �A�j���[�V�����㔼�g�Đ�
				model->PlayUpeerBodyAnimation(modelAnimUpperBody);
				// �U���O��ň�x�����I��
				++attackMemory;
				// �A�j���[�V�����Đ�
				playerid->SetUpdateAnim(Player::UpAnim::Doble);
			}
			// �n��
			else
			{
				// �A�j���[�V�������[��
				Model::ModelAnim modelAnim;
				modelAnim.index = Player::Animation::Anim_Slash;
				modelAnim.currentAnimationAddSeconds = 0.031f;
				// �A�j���[�V�����Đ�
				model->PlayAnimation(modelAnim);
				// �U���O��ň�x�����I��
				++attackMemory;
				// �A�j���[�V�����Đ�
				playerid->SetUpdateAnim(Player::UpAnim::Normal);
			}
		}
	}

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = moveid->GetOnLadius() ? 
		model->GetCurrentANimationSeconds() : 
		model->GetCurrentAnimationSecondsUpeer();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B

	// 1����
	if (animationTime >= 0.8f  && playerid->InputAttack())
	{
		// �P��ڂ̍U���Ȃ�
		oneAttackCheck = false;
		playerid->SetAttackCollisionFlag(false);
		deleteCheck = false;
		// ���͊m�F�ŃX�e�[�g�ύX
		playerid->GetStateMachine()->ChangeState(static_cast<int>(button  ? Player::State::SideCut : Player::State::Move));
		return;
	}
	// �a���̓����蔻��
	playerid->CollisionNodeVsEnemies("mixamorig:LeftHand",
		playerid->GetLeftHandRadius(),
		"body2", "boss_left_hand2", "boss_right_hand2");
}

void PlayerQuickJabState::Exit()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	// �ړ��̒�~
	moveid->SetGravity(
		playerid->GetGravity());

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

	// �`��
	isPlayerDrawCheck = 1;
	// ��\���̏ꍇ�\������
	playerid->SetPlayeDrawCheck(isPlayerDrawCheck);
}

// 2�A����
void PlayerSideCutState::Enter()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	bool loop = false;
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	if (!moveid->GetOnLadius())
	{
		// �A�j���[�V�������[��
		Model::ModelAnim modelAnim;
		Model::ModelAnim modelAnimUpperBody;
		modelAnim.index = Player::Animation::Anim_Jump;
		modelAnimUpperBody.index = Player::Animation::Anim_SlashBeside;

		// �Đ��J�n���� 
		modelAnim.currentanimationseconds = 0.6f;
		// ���[�v
		bool loop = true;
		modelAnim.loop = true;
		// �Đ����ԉ��Z���̒l
		modelAnim.currentAnimationAddSeconds = 0.03f;
		modelAnimUpperBody.currentAnimationAddSeconds = 0.03f;
		modelAnim.keyFrameEnd = 0.63f;
		// �A�j���[�V�����Đ�
		model->PlayAnimation(modelAnim);
		// �A�j���[�V�����㔼�g�Đ�
		model->PlayUpeerBodyAnimation(modelAnimUpperBody);
		// �U���O��ň�x�����I��
		++attackMemory;
		// �A�j���[�V�����Đ�
		playerid->SetUpdateAnim(Player::UpAnim::Doble);
	}
	else
	{
		// �A�j���[�V�������[��
		Model::ModelAnim modelAnim;
		modelAnim.index = Player::Animation::Anim_SlashBeside;
		modelAnim.currentanimationseconds = 0.2f;
		modelAnim.currentAnimationAddSeconds = 0.03f;
		// �A�j���[�V�����Đ�
		model->PlayAnimation(modelAnim);
		// �U���O��ň�x�����I��
		++attackMemory;
		// �A�j���[�V�����Đ�
	    playerid->SetUpdateAnim(Player::UpAnim::Normal);
	}
		
	if (!InitializationCheck)
	{
		//// �R�}���h�ݒ���ڍU��
		//commandSeconde.push_back(GamePad::BTN_B);
		// �R�}���h�ݒ�O��ڍU��
		commandThredeButtonB.push_back(GamePad::BTN_B);
		commandThredeButtonRight.push_back(GamePad::BTN_RIGHT);
	}
	// �d�̓I�t
	bool stopFall = true;
	moveid->SetStopFall(stopFall);
	// ��]���邩�`�F�b�N
	rotateCheck = false;
	InitializationCheck = true;
}

void PlayerSideCutState::Execute(float elapsedTime)
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	GamePad& gamePad = Input::Instance().GetGamePad();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	// ���b�N�I������
	playerid->UpdateCameraState(elapsedTime);

	// �A���{�^���m�F
	if (playerid->InputAttack() &&
		playerid->GetSelectCheck() ==
		(int)Player::CommandAttack::Attack)
	{
		// �R�}���h�m�F3�i�U�� �{�^��B
		if (gamePad.ConfirmCommand(commandThredeButtonB, frame))
		{
			buttonSeconde = true;
		}
		// �R�}���h�m�F3�i�U�� �\���L�[�E
		if (gamePad.ConfirmCommand(commandThredeButtonRight, frame))
		{
			buttonSeconde = true;
		}
	}
	// ���
	if (playerid->InputAvoidance() && playerid->InputMove())
	{
		bool stop = false;
		moveid->SetStopMove(stop);
		DirectX::XMFLOAT3 vec;
		vec = sharedId->GetComponent<Player>()->GetMoveVec(elapsedTime);
		moveid->Turn(vec, turnSpeed, elapsedTime);
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
		button = false;
		buttonSeconde = false;
		return;
	}
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = moveid->GetOnLadius() ?
		model->GetCurrentANimationSeconds() :
		model->GetCurrentAnimationSecondsUpeer();
	// 2����
	if (animationTime >= 1.1f)
	{
		playerid->SetAttackCollisionFlag(false);
		deleteCheck = false;
		// ���͊m�F�ŃX�e�[�g�ύX
		playerid->GetStateMachine()->ChangeState(static_cast<int>(buttonSeconde ? Player::State::CycloneStrike : Player::State::Move));
		return;
	}
	// �U�������蔻��
	playerid->CollisionNodeVsEnemies("mixamorig:LeftHand"
		, playerid->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");
}

void PlayerSideCutState::Exit()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	// �ړ��̒�~
	moveid->SetGravity(
		playerid->GetGravity());
	// ��������
	bool stopMove = false;
	moveid->SetStopMove(stopMove);
	// �d�̓I�t
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
}
// 3����
void PlayerCycloneStrikeState::Enter()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	bool loop = false;
	// �R���|�l���g�擾
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	if (!moveid->GetOnLadius())
	{
	    // �A�j���[�V�������[��
		Model::ModelAnim modelAnimUpper;
		modelAnimUpper.index = Player::Animation::Anim_SlashThree;
		modelAnimUpper.currentanimationseconds = 0.03f;
		// �A�j���[�V�����㔼�g�Đ�
		model->PlayUpeerBodyAnimation(modelAnimUpper);
		// �U���O��ň�x�����I��
		++attackMemory;
		// �A�j���[�V�����Đ�
		playerid->SetUpdateAnim(Player::UpAnim::Doble);
	}
	else
	{
		// �A�j���[�V�������[��
		Model::ModelAnim modelAnim;
		modelAnim.index = Player::Animation::Anim_SlashThree;
		modelAnim.currentanimationseconds = 0.7f;
		modelAnim.currentAnimationAddSeconds = 0.03f;
		// �A�j���[�V�����Đ�
		model->PlayAnimation(modelAnim);
		// �U���O��ň�x�����I��
		++attackMemory;
		// �A�j���[�V�����Đ�
		playerid->SetUpdateAnim(Player::UpAnim::Normal);
	}
	// �d�̓I�t
	bool stopFall = true;
	moveid->SetStopFall(stopFall);
	// ��]���邩�`�F�b�N
	rotateCheck = false;
	InitializationCheck = true;
}

void PlayerCycloneStrikeState::Execute(float elapsedTime)
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	GamePad& gamePad = Input::Instance().GetGamePad();
	// �R���|�l���g�擾
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	// ���b�N�I������
	playerid->UpdateCameraState(elapsedTime);
	// ���
	if (playerid->InputAvoidance() && playerid->InputMove())
	{
		bool stop = false;
		moveid->SetStopMove(stop);
		DirectX::XMFLOAT3 vec;
		vec = playerid->GetMoveVec(elapsedTime);
		moveid->Turn(vec, turnSpeed, elapsedTime);
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
		button = false;
		buttonSeconde = false;
		return;
	}
	DirectX::XMVECTOR Vector;
	DirectX::XMVECTOR LengthSq;
	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid->GetPosition());
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
		DirectX::XMFLOAT3 angle = transformid->GetAngle();
		DirectX::XMFLOAT3 direction;
		direction.x = sinf(angle.y) * 6;
		direction.y = 0;
		direction.z = cosf(angle.y) * 6;
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
				// ����
				if (moveid->Turn(vector, turnSpeed, elapsedTime))
				{

					moveid->Move(vector, speed, elapsedTime);

				}
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
	// 3����
	if (animationTime >= 2.7f )
	{
		playerid->SetAttackCollisionFlag(false);
		deleteCheck = false;
		// ���͊m�F�ŃX�e�[�g�ύX
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
		return;
	}
	// �����蔻��
	playerid->CollisionNodeVsEnemies("mixamorig:LeftHand",
		playerid->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");
}

void PlayerCycloneStrikeState::Exit()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// �R���|�l���g�擾
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	// �ړ��̒�~
	moveid->SetGravity(
		playerid->GetGravity());
	// ��������
	bool stopMove = false;
	moveid->SetStopMove(stopMove);
	// �d�̓I�t
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
}
// ����Z�a��
void PlayerSpecialAttackState::Enter()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// �R���|�l���g�擾
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	loopSe = false;
	std::shared_ptr<Transform> enemyTransform = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<Transform>();
	std::shared_ptr<HP> enemyHpId = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<HP>();
	lightning = std::make_unique<Effect>("Data/Effect/sunder.efk");
	lightningAttack = std::make_unique<Effect>("Data/Effect/HitThunder.efk");
	lightningHit = std::make_unique<Effect>("Data/Effect/lightningStrike.efk");
	if (!moveid->GetOnLadius())
	{
		// �A�j���[�V�������[��
		Model::ModelAnim modelAnim;
		Model::ModelAnim modelAnimUpperBody;
		modelAnim.index = Player::Animation::Anim_Jump;
		modelAnimUpperBody.index = Player::Animation::Anim_MagicSeconde;
		modelAnim.loop = true;
		modelAnim.keyFrameEnd = 0.63f;
		// �A�j���[�V�����Đ�
		model->PlayAnimation(modelAnim);
		// �A�j���[�V�����㔼�g�Đ�
		model->PlayUpeerBodyAnimation(modelAnimUpperBody);
		// �A�j���[�V�����㔼�g�����g�Đ�
		playerid->SetUpdateAnim(Player::UpAnim::Doble);
	}
	else
	{
		// �Đ����[�v
		bool  loop = false;
		// �Đ��J�n���� 
		float currentAnimationStartSeconds = 0.0f;
		// �A�j���[�V�����u�����h
		float blendSeconds = 0.5f;
		// �A�j���[�V�������[��
		Model::ModelAnim modelAnim;
		modelAnim.index = Player::Animation::Anim_MagicSeconde;
		// �A�j���[�V�����Đ�
		model->PlayAnimation(modelAnim);
		// �A�j���[�V�����Đ�
		playerid->SetUpdateAnim(Player::UpAnim::Normal);
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
	playerid->InputAttackThanderSE();
	// ��]����
	isRotate = true;
	bool specialRockOff = true;
	playerid->SetSpecialRockOff(specialRockOff);
}

void PlayerSpecialAttackState::Execute(float elapsedTime)
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<HP> enemyHpId ;
	// �G�l�~�[�̑O�Ɍ���
	{
		DirectX::XMVECTOR Vector;
		DirectX::XMVECTOR LengthSq;
		DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid->GetPosition());
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
			if (moveid->Turn(vector, turnSpeed, elapsedTime))
			{
				// �J��������
				{
					MessageData::CAMERACHANGEMOTIONMODEDATA	p;
					position = transformid->GetPosition();
					angle = transformid->GetAngle();
					float vx = sinf(angle.y) * 6.0f;
					float vz = cosf(angle.y) * 6.0f;
					float vx2 = sinf(angle.y) - 10.0f;
					float vz2 = cosf(angle.y) * 7.0f;
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
		float animationTime = moveid->GetOnLadius() ?
			model->GetCurrentANimationSeconds() :
			model->GetCurrentAnimationSecondsUpeer();
		// �A�j���[�V����
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON)
		{
			if (!moveid->GetOnLadius())
			{
				// �A�j���[�V�������[��
				Model::ModelAnim modelAnim;
				Model::ModelAnim modelAnimUpperBody;
				modelAnim.index = Player::Animation::Anim_Jump;
				modelAnimUpperBody.index = Player::Animation::Anim_SpecialAttack;
				modelAnim.loop = true;
				modelAnim.keyFrameEnd = 0.63f;
				// �A�j���[�V�����Đ�
				model->PlayAnimation(modelAnim);
				// �A�j���[�V�����㔼�g�Đ�
				model->PlayUpeerBodyAnimation(modelAnimUpperBody);
			}
			else
			{
				// �A�j���[�V�������[��
				Model::ModelAnim modelAnim;
				modelAnim.index = Player::Animation::Anim_SpecialAttack;
				// �A�j���[�V�����Đ�
				model->PlayAnimation(modelAnim);
			}
			button = false;
			lightning->Stop(lightning->GetEfeHandle());
			loopSe = true;
			// ����~
			playerid->InputStopAttackThanderSE();
		}
	}
	else
	{
			// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
			float animationTime = moveid->GetOnLadius() ?
				model->GetCurrentANimationSeconds() :
				model->GetCurrentAnimationSecondsUpeer();
		// �X���[���[�V�����ƐF�𖾂邭
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.5f + FLT_EPSILON)
		{
			PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
			ColorGradingData colorGradingData;
			colorGradingData.brigthness = 1.5f;
			colorGradingData.saturation = 0.0f;
			float colorGradingAdd = 0.01f;
			float radialAdd = 0.01f;
			postprocessingRenderer.StepValueColor(colorGradingAdd);
			postprocessingRenderer.StepValueRadial(radialAdd);
			postprocessingRenderer.SetColorGradingMaxData(colorGradingData);
			postprocessingRenderer.SetColorGradingMinData(colorGradingData);
			playerid->SetHitCheck(true);
		}
		// �A�j���[�V�����؂��
		if (animationTime >= 1.6f)
		{
			PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
			ColorGradingData colorGradingData;
			colorGradingData.brigthness = 5.5f;
			RadialBlurData radialBlurData;
			radialBlurData.radius = 500;
			radialBlurData.mask_radius = 300;
			radialBlurData.samplingCount = 10;
			postprocessingRenderer.SetColorGradingMaxData(colorGradingData);
			postprocessingRenderer.SetRadialBlurMaxData(radialBlurData);
			float value = 5;
			postprocessingRenderer.StepValueRadial(value);
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
			// �K�E�Z��
			playerid->InputAttackSlashSpecileLightningStrikeSE();
			if (enemyHpId->ApplyDamage(10, 0.5f)) 
			{
				lightningHit->Play(pPosition);
			}
			playerid->SetHitCheck(false);
		}
		bool animationEnd = moveid->GetOnLadius() ?
			model->IsPlayAnimation() :
			model->IsPlayUpeerBodyAnimation();
		// �A�j���[�V�����I��
		if (!animationEnd)
		{
			// ���͊m�F�ŃX�e�[�g�ύX
			playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));

		}
	}
}

void PlayerSpecialAttackState::Exit()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	// ������̍ĊJ
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
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
	PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
	// �Œ�l�ݒ�
	postprocessingRenderer.StepValueMin();
    ColorGradingData colorGradingData;
	postprocessingRenderer.SetColorGradingMinData(colorGradingData);
	bool specialRockOff = false;
	playerid->SetSpecialRockOff(specialRockOff);
}
// ����Z�Ζ��@
void PlayerMagicState::Enter()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	std::shared_ptr<Mp> mpId = sharedId->GetComponent<Mp>();

	// se�Đ�
	seParam.filename = "Data/Audio/SE/�`���[�W���G.wav";
	seParam.volume = 0.5f;
	playerid->InputSe(seParam);

	// �G�t�F�N�g
	charge = std::make_unique<Effect>("Data/Effect/magicCharge.efk");
	chargeComplet = std::make_unique<Effect>("Data/Effect/magicChargeComplet.efk");
	
	magicType = playerid->GetSelectMagicCheck();
	switch (magicType)
	{
		case ((int)Player::CommandMagic::Heale):
		{
			modelAnim.index = Player::Animation::Anim_MagicSeconde;
			modelAnim.keyFrameEnd = 0.0f;
			// �A�j���[�V�����Đ�
			model->PlayAnimation(modelAnim);
			// �G�t�F�N�g��
			heale = std::make_unique<Effect>("Data/Effect/heale.efk");
			// �G�t�F�N�g�Đ�
			heale->Play(transformid->GetPosition());

			// ���@�`���[�W����
			magicCharge = magicChargeMax;
			break;
		}
		default:
		{
			modelAnim.index = Player::Animation::Anim_MagicArch;
			// ���@����
			magicCharge = magicChargeEnd;

			// �A�j���[�V�����Đ�
			model->PlayAnimation(modelAnim);

			// �G�t�F�N�g�Đ�
			charge->Play(transformid->GetPosition());
			break;
		}
	}
	// �A�j���[�V�����Đ�
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	// ������̒�~
	bool stopFall = true;
	moveid->SetStopFall(stopFall);
	// ��]����
	isRotate = true;
	// ����
	// �ړ�����
	isMove = false;

	// ���@�J�n
	magicStart = false;

	// ��]���x
	rotationSpeed = DirectX::XM_2PI;

	// �p�x�ǂ����邩
	isAngleAddX = true;
	isAngleAddY = true;

	// �p�x
	angle = transformid->GetAngle();

	angle.x += DirectX::XMConvertToRadians(20);
	angle.y -= DirectX::XMConvertToRadians(60);
}

void PlayerMagicState::Execute(float elapsedTime)
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	// ���b�N�I������
	playerid->UpdateCameraState(elapsedTime);
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();

	// ����
	// ���@����
	if (magicCharge < magicChargeMax && animationTime >= 1.6f &&
		playerid->InputMagick())
	{
		// �A�j���[�V�����ۍĐ�
		{
			modelAnim.index = Player::Animation::Anim_MagicArch;
			// �A�j���[�V�����Đ�
			model->PlayAnimation(modelAnim);
		}
		// ���@�`���[�W����
		magicCharge = magicChargeMax;

		// �`���[�W�G�t�F�N�g��~
		if (charge->GetEfeHandle())
			charge->Stop(charge->GetEfeHandle());

		// �`���[�W����
		chargeComplet->Play(transformid->GetPosition());
	}

	// ���@���ߏI��
	if (magicCharge >= magicChargeMax || 
		!playerid->InputMagick() && 
		!magicStart)
	{
		// se��~
		playerid->StopSe(seParam);

		// �G�t�F�N�g�Đ����Ă����~
		if (charge->GetEfeHandle())
		{
			// �`���[�W�G�t�F�N�g��~
			charge->Stop(charge->GetEfeHandle());

			// �`���[�W����
			chargeComplet->Play(transformid->GetPosition());
		}

		// ���@���
		switch (magicType)
		{
		case (int)Player::CommandMagic::Fire:
		{
			modelAnim.index = Player::Animation::Anim_Magic;
			modelAnim.keyFrameEnd = 0.0f;
			// �A�j���[�V�����Đ�
			model->PlayAnimation(modelAnim);
			break;
		}

		case (int)Player::CommandMagic::Ice:
		{
			modelAnim.index = Player::Animation::Anim_Magic;
			modelAnim.keyFrameEnd = 0.0f;
			// �A�j���[�V�����Đ�
			model->PlayAnimation(modelAnim);
			break;
		}

		case ((int)Player::CommandMagic::Thander):
		{
			modelAnim.index = Player::Animation::Anim_MagicSeconde;
			modelAnim.keyFrameEnd = 0.0f;
			// �A�j���[�V�����Đ�
			model->PlayAnimation(modelAnim);
			break;
		}

		default:
			break;
		}

		//�@���@����
		magicStart = true;
	}

	if (!magicStart) return;

	// �A�j���[�V�����I�������ʂ�
	DirectX::XMVECTOR vectorXM;
	DirectX::XMVECTOR lengthSqXM;
	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid->GetPosition());
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

	// ����̕�������
	// ��]
	if (isRotate)
	{
		// ����
		if (moveid->TurnCheck(
				vector, angleRange, elapsedTime) &&
			playerid->GetRockCheck())
		{
			// ����
			moveid->Turn(vector,turnSpeed,elapsedTime);
		}
		else
		{
			isRotate = false;
		}
	}

	// UI�����̏�����
	playerid->StartMagicUiFire();
	// ���@�̎�ނŏI�����A��B
	switch (magicType)
	{
	case (int)Player::CommandMagic::Fire:
	{
		// ����
		if (animationTime <= 0.5f)return;

		// �p�x����
		if (angle.x > angleMax)
			isAngleAddX = false;

		// �p�x���
		if (angle.x < angleMin)
			isAngleAddX = true;

		// �p�x����
		if (!isAngleAddX)
		{
			angle.x -= rotationSpeed * angleAddX;
		}
		// �p�x���
		else
		{
			angle.x += rotationSpeed * angleAddX;
		}

		angle.y += rotationSpeed * angleAddY;

		// �P��
		if (magicCharge <= magicChargeEnd)
		{
			// �p�x
			angle = transformid->GetAngle();
			playerid->InputMagicframe();
		}
		// ����
		else
		{
			// ������
			playerid->PushMagicFrame(angle);
		}
		// SE��
		playerid->InputAttackFlameSE();

		// ���ߏI���܂Ō���
		if (magicCharge <= magicChargeEnd)
			playerid->GetStateMachine()->ChangeState((int)Player::State::Idle);

		else
			--magicCharge;
		break;
	}

	case (int)Player::CommandMagic::Thander:
	{
		// ����
		if (animationTime <= 1.1f)return;

		// ����
		playerid->InputAttackThanderSE();

		// ������
		playerid->InputMagicLightning();

		// ���ߏI���܂Ō���
		if (magicCharge <= magicChargeEnd)
			playerid->GetStateMachine()->ChangeState((int)Player::State::Idle);

		else
			--magicCharge;

		break;
	}
	case (int)Player::CommandMagic::Ice:
	{
		// ����
		if (animationTime <= 0.5f)return;

		// �p�x����
		if (angle.x > angleMax)
			isAngleAddX = false;

		// �p�x���
		if (angle.x < angleMin)
			isAngleAddX = true;

		// �p�x����
		if (!isAngleAddX)
		{
			angle.x -= rotationSpeed * angleAddX;
		}
		// �p�x���
		else
		{
			angle.x += rotationSpeed * angleAddX;
		}

		// �X��
		playerid->InputAttackIceSE();

		// �P��
		if (magicCharge <= magicChargeEnd)
		{
			// �X����
			playerid->InputMagicIce();
		}
		else
		{
			// �p�x
			angle = transformid->GetAngle();
			playerid->PushMagicIce(angle);
		}

		

		// ���ߏI���܂Ō���
		if (magicCharge <= magicChargeEnd)
		{
			int pCount = ProjectileManager::Instance().GetProjectileCount();
			pCount -= 2;
			for (int i = 0; i < pCount; ++i)
			{
				if (ProjectileManager::Instance().GetProjectile(i)->GetComponent<ProjectileStraight>())
				ProjectileManager::Instance().GetProjectile(i)->GetComponent<ProjectileStraight>()->SetMovementCheck(iceMagicMoveCheck);
			}
			playerid->GetStateMachine()->ChangeState((int)Player::State::Idle);
		}
		else
			--magicCharge;

		break;
	}
	case (int)Player::CommandMagic::Heale:
	{
		// ����
		if (animationTime <= 1.1f)return;
		// �񕜉�
		playerid->InputAttackHealeSE();
		// �񕜔���
		playerid->InputMagicHealing();
		playerid->GetStateMachine()->ChangeState((int)Player::State::Idle);
		break;
	}
	default:
		break;
	}
}

void PlayerMagicState::Exit()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// ��񂾂�
	GamePad& gamePad = Input::Instance().GetGamePad();
	bool isPush = true;
	gamePad.SetButtonDownCountinue(isPush);

	// �G�t�F�N�g�Đ����Ă����~
	if (charge->GetEfeHandle())
	{
		// �`���[�W�G�t�F�N�g��~
		charge->Stop(charge->GetEfeHandle());
	}

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	if (!playerid->InputShortCutkeyMagic())
	{
		// ���@�I������
		playerid->SetMagicAction(false);
		// �U����ޑI��
		playerid->SetSelectMagicCheck((int)Player::CommandAttack::Attack);
	}
	// ������̍ĊJ
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
}

void PlayerSpecialMagicState::Enter()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	fire = std::make_unique<Effect>("Data/Effect/fire.efk");
	fireAttack = std::make_unique<Effect>("Data/Effect/hit fire.efk");
	modelAnim.index = Player::Animation::Anim_SlashThree;
	// �A�j���[�V�����Đ�
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�����Đ�
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");
	DirectX::XMFLOAT3 pPosition =
	{
				pHPosiiton->worldTransform._41,
				pHPosiiton->worldTransform._42,
				pHPosiiton->worldTransform._43
	};
	// ����
	playerid->InputAttackFlameSE();
	fire->Play(pPosition);
	// ������̒�~
	bool stopFall = true;
	moveid->SetStopFall(stopFall);
	button = true;
	// �X�^�[�g�l
	specialMoveWaitTime = specialMoveWaitStartTime;
	// �ŏ��������@�𔭓����邽��
	startMagic = false;
	// �J������]�������J�n
	isRotate = true;
	bool specialRockOff = true;
	playerid->SetSpecialRockOff(specialRockOff);
}

void PlayerSpecialMagicState::Execute(float elapsedTime)
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	// �G�l�~�[�̑O�Ɍ���
	{
		DirectX::XMVECTOR Vector;
		DirectX::XMVECTOR LengthSq;
		DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid->GetPosition());
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
			if (moveid->Turn(vector, turnSpeed, elapsedTime))
			{
				// �J��������
				{
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
		// �A�j���[�V����
		if (!model->IsPlayAnimation())
		{
			modelAnim.index = Player::Animation::Anim_MagicSeconde;
			// �A�j���[�V�����Đ�
			model->PlayAnimation(modelAnim);
			button = false;
			// ��̉��G�t�F�N�g��~
			fire->Stop(fire->GetEfeHandle());
		}
	}
	else
	{
		// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
		float animationTime = model->GetCurrentANimationSeconds();
		// �A�j���[�V����
		if (animationTime >= 1.1f)
		{
			playerid->SetFlashOn(true);
			// �A�j���[�V������~
			playerid->SetUpdateAnim
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
			// �K�E�Z���������̌o�ߎ���
			specialMoveWaitTime += elapsedTime;
			PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
			ColorGradingData colorGradingData;
			colorGradingData.brigthness = 1.5f;
			postprocessingRenderer.SetColorGradingData(colorGradingData);
		}
		if (animationTime >= 1.1f && animationTime <= 1.2f)
		{
			playerid->
				InputSpecialMagicframe();
			playerid->InputAttackFlameSpecileSE();
		}
	}
	// �_���[�W����
	playerid->SpecialApplyDamageInRadius();

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
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
		return;
	}
}

void PlayerSpecialMagicState::Exit()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	// ������̍ĊJ
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
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
	PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
	// �Œ�l�ݒ�
	postprocessingRenderer.StepValueMin();
	bool specialRockOff = false;
	playerid->SetSpecialRockOff(specialRockOff);
}

void PlayerDamageState::Enter()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// �R���|�l���g�擾
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	bool loop = false;
	// ��������
	bool stopMove = false;
	moveid->SetStopMove(stopMove);
	// �d�̓I��
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
	modelAnim.index = Player::Animation::Anim_Pain;
	model->PlayAnimation(modelAnim);
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerDamageState::Execute(float elapsedTime)
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// �R���|�l���g�擾
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	// ���b�N�I������
	playerid->UpdateCameraState(elapsedTime);
	if (!model->IsPlayAnimation())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
	}
}

void PlayerDamageState::Exit()
{
}

void PlayerDeathState::Enter()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// �R���|�l���g�擾
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	float				stateTimer = 0.0f;
	modelAnim.index = Player::Animation::Anim_Deth;
	model->PlayAnimation(modelAnim);
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	stateTimer = 0.0f;
	PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
	colorGradingData.saturation = 0.0f;
	postprocessingRenderer.SetColorGradingMinData(colorGradingData);
}

void PlayerDeathState::Execute(float elapsedTime)
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// �R���|�l���g�擾
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	std::shared_ptr<HP> hpid = sharedId->GetComponent<HP>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
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
	float animationTime = model->GetCurrentANimationSeconds();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B
	bool CollisionFlag = animationTime >= 0.5f && animationTime <= 1.6f;
	if (CollisionFlag)
	{
		++stateTimer;
		playerid->SetUpdateAnim(Player::UpAnim::Stop);
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
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	// �����g
	modelAnim.index = Player::Animation::Anim_Dush;
	modelAnim.currentanimationseconds = 0.3f;
	modelAnim.currentAnimationAddSeconds = 0.025f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V������� �ʏ�
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	// ������̒�~
	bool stopFall = true;
	moveid->SetStopFall(stopFall);
	playerid->InputDashSE();
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
	// �Ռ��g������
	impactInitialize = true;
}

// ����X�V
void PlayerAvoidanceState::Execute(float elapsedTime)
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// �R���|�[�l���g�Ăяo��
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	// �������R
	bool stopMove = false;
	moveid->SetStopMove(stopMove);
	// ���b�N�I������
	playerid->UpdateCameraState(elapsedTime);
	// �]����_�b�V��
	DirectX::XMFLOAT3 dir;
	dir.x = sinf(transformid->GetAngle().y);
	dir.y = cosf(transformid->GetAngle().x);
	dir.z = cosf(transformid->GetAngle().y);
	// �C�ӂ̃A�j���[�V�����Đ����
	float animationTime = sharedId->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	// �ʒu�X�V
	wind->SetPosition(wind->GetEfeHandle(),
		{
			transformid->GetPosition().x,
			transformid->GetPosition().y + addHeight,
			transformid->GetPosition().z
		});
	// ��]
	if (animationTime <= 0.5f)
	{
		// ���͏����擾
		GamePad& gamePad = Input::Instance().GetGamePad();
		rotateVec.x = gamePad.GetAxisLX();
		rotateVec.y = 0.0f;
		rotateVec.z = gamePad.GetAxisLY();

		moveid->Turn(rotateVec, rotateSpeed, elapsedTime);
	}
	// �n��_�b�V��
	if (animationTime >= 0.7f && animationTime <= 0.8f)
	{
		DirectX::XMFLOAT3 impulse =
		{
			dir.x * speed,
			0,
			dir.z * speed,
		};
		moveid->AddImpulse(impulse, impactInitialize);
		impactInitialize = false;
	}

	// �A�j���[�V�����_�b�V����ʏ�ړ�
	if (animationTime >= 1.0f && sharedId->GetComponent<Player>()->InputMove())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	}
	// �ҋ@
	if (animationTime >= 2.0f)
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
	}
}

// ����I��
void PlayerAvoidanceState::Exit()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// �R���|�[�l���g�Ăяo��
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	// ������̒�~
	bool stopFall = false;
	moveid->SetStopFall(stopFall);
	wind->Stop(wind->GetEfeHandle());
}

// ���ˊJ�n
void PlayerReflectionState::Enter()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// �R���|�[�l���g�Ăяo��
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	bool loop = false;
	modelAnim.index = Player::Animation::Anim_Counter;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V������� �ʏ�
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
}

// ���ˍX�V
void PlayerReflectionState::Execute(float elapsedTime)
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// �R���|�[�l���g�Ăяo��
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	if (!model->IsPlayAnimation())
	{
		// �����蔻��̗L��
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
	}
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B
	bool CollisionFlag = animationTime >= 0.2f && animationTime <= 0.4f;
	if (CollisionFlag)
	{
		// ����m�[�h�ƃG�l�~�[�̏Փˏ���
		playerid->CollisionNodeVsEnemiesCounter("mixamorig:LeftHand", sharedId->GetComponent<Player>()->GetLeftHandRadius());
		playerid->CollisionNodeVsEnemiesCounter("mixamorig:RightHand", sharedId->GetComponent<Player>()->GetLeftHandRadius());
	}
}

// ���ˏI��
void PlayerReflectionState::Exit()
{
}

void PlayerTitleIdleState::Enter()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Animation::Anim_Idle;
	modelAnim.loop = true;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerTitleIdleState::Execute(float elapsedTime)
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	//// ���b�N�I������
    playerid->UpdateCameraState(elapsedTime);
	// �q�b�g
	if (playerid->InputAttack())
	{

		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::StateTitle::Push));
	}
}

void PlayerTitleIdleState::Exit()
{
}

void PlayerTitlePushState::Enter()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// �R���|�l���g�擾
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	lightning = std::make_unique<Effect>("Data/Effect/sunder.efk");
	lightningAttack = std::make_unique<Effect>("Data/Effect/HitThunder.efk");
	lightningHit = std::make_unique<Effect>("Data/Effect/lightningStrike.efk");
	modelAnim.index = Player::Animation::Anim_MagicSeconde;
	// �A�j���[�V�����Đ�
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�����Đ�
	playerid->SetUpdateAnim(Player::UpAnim::Normal);

	// ������̒�~
	bool stopFall = true;
	moveid->SetStopFall(stopFall);
	// ���[�V�����؂�ւ�
	secondeMortion = false;
}

void PlayerTitlePushState::Execute(float elapsedTime)
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	// �R���|�l���g�擾
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = sharedId->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	//// ���b�N�I������
	playerid->UpdateCameraState(elapsedTime);
	std::shared_ptr<HP> enemyHpId;
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
		float animationTime = moveid->GetOnLadius() ?
			model->GetCurrentANimationSeconds() :
			model->GetCurrentAnimationSecondsUpeer();
		// �A�j���[�V����
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON && !secondeMortion)
		{
			playerid->InputAttackThanderSE();
				// �Đ����[�v
				bool  loop = false;
				// �Đ��J�n���� 
				float currentAnimationStartSeconds = 0.0f;
				// �Đ����ԉ��Z���̒l
				float currentAnimationAddSeconds = 0.00f;
				// �A�j���[�V�����u�����h
				float blendSeconds = 0.5f;
				modelAnim.index = Player::Animation::Anim_SpecialAttack;
				// �A�j���[�V�����Đ�
				model->PlayAnimation(modelAnim);
				Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");
				DirectX::XMFLOAT3 pPosition =
				{
							pHPosiiton->worldTransform._41,
							pHPosiiton->worldTransform._42,
							pHPosiiton->worldTransform._43
				};
				lightningHit->Play(pPosition);
				secondeMortion = true;
				// �|�X�g�G�t�F�N�g�C���X�^���X�Q�b�g
				PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
				// ���W�A���u���[
				radialBlurData.radius = 40.0f;
				postprocessingRenderer.SetRadialBlurData(radialBlurData);
		}
		if (animationTime >= 1.5f - FLT_EPSILON && animationTime <= 1.6f + FLT_EPSILON && secondeMortion)
		{
			playerid->InputAttackSlashSpecileLightningStrikeSE();
			Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");
			DirectX::XMFLOAT3 pPosition =
			{
						pHPosiiton->worldTransform._41,
						pHPosiiton->worldTransform._42,
						pHPosiiton->worldTransform._43
			};
			lightningHit->Play(pPosition);
			// �|�X�g�G�t�F�N�g�C���X�^���X�Q�b�g
			PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
			// ���W�A���u���[
			radialBlurData.radius = 60.0f;
			postprocessingRenderer.SetRadialBlurData(radialBlurData);
		}
		if (!model->IsPlayAnimation() && secondeMortion)
		{
			// �V�[���I��
			playerid->SetEndState(true);
		}
}

void PlayerTitlePushState::Exit()
{
}

void PlayerClearIdleState::Enter()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Animation::Anim_MagicSeconde;
	modelAnim.keyFrameEnd = 68.0f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	DirectX::XMFLOAT3 position = transformid->GetPosition();
	position.x += -3.0f;
	transformid->SetPosition(position);
}

void PlayerClearIdleState::Execute(float elapsedTime)
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	Model::Node* PRock = model->FindNode("mixamorig:Spine1");
	DirectX::XMFLOAT3 pos = model->ConvertLocalToWorld(PRock);
	pos.x += 3.0f;
	MessageData::CAMERACHANGEFREEMODEDATA	p = { pos };
	Messenger::Instance().SendData(MessageData::CAMERACHANGEFREEMODE, &p);
	// �q�b�g
	if (playerid->InputAttack())
	{
		// �V�[���I��
		playerid->SetEndState(true);
	}
}

void PlayerClearIdleState::Exit()
{
}

void PlayerClearEscapeState::Enter()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Animation::Anim_MagicSeconde;
	modelAnim.keyFrameEnd = 68.0f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	std::shared_ptr<Transform> transformid = sharedId->GetComponent<Transform>();
	DirectX::XMFLOAT3 position = transformid->GetPosition();
	position.x += -3.0f;
	transformid->SetPosition(position);
}

void PlayerClearEscapeState::Execute(float elapsedTime)
{
}

void PlayerClearEscapeState::Exit()
{
}

void PlayerOverIdleState::Enter()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
	modelAnim.index = Player::Animation::Anim_Deth;
	modelAnim.currentanimationseconds = 1.7f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerOverIdleState::Execute(float elapsedTime)
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	//// ���b�N�I������
	playerid->UpdateCameraState(elapsedTime);
	// �q�b�g
	if (playerid->InputAttack())
	{
		playerid->GetStateMachine()->ChangeState(static_cast<int>(Player::StateOver::Revive));
	}
}

void PlayerOverIdleState::Exit()
{
}

void PlayerOverReviveState::Enter()
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Animation::Anim_Magic;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid->SetUpdateAnim(Player::UpAnim::Normal);
	isPoseStarte = false;
}

void PlayerOverReviveState::Execute(float elapsedTime)
{
	// ���S�`�F�b�N
	auto sharedId = owner.lock();
	if (!sharedId)
		return;

	Model* model = sharedId->GetComponent<ModelControll>()->GetModel();
	if (!model->IsPlayAnimation() && !isPoseStarte)
	{
		modelAnim.index = Player::Animation::Anim_Jump;
		model->PlayAnimation(modelAnim);
		isPoseStarte = true;
		return;
	}
	std::shared_ptr<Player> playerid = sharedId->GetComponent<Player>();
	if (!model->IsPlayAnimation() && isPoseStarte)
	{
		playerid->SetEndState(endState);
	}
}

void PlayerOverReviveState::Exit()
{
}
