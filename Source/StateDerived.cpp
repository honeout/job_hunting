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
	// owner�Ăяo��
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	// �꒣��
	enemyid.lock()->SetRandomTargetPosition();
	// �U����
	attackCount = attackCountMax;
	// �o�����̐ݒ�
	if (enemyid.lock()->GetIsEnemyAwakened())
	{
		attackCount = attackCountMin;
		modelAnim.keyFrameEnd = 288;
	}
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
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	// ���n�u��
	upOnLading = false;
	// ���G�t�F�N�g
	smorker = std::make_unique<Effect>("Data/Effect/smorkeDash.efk");
}

// �p�j�X�e�[�g�Ŏ��s���郁�\�b�h
void WanderState::Execute(float elapsedTime)
{
	// owner�Ăяo��
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();

	// �ڕW�n�_�����v���C���[�ʒu�ɐݒ�
	targetPosition = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();
	enemyid.lock()->SetTargetPosition(targetPosition);
	DirectX::XMVECTOR targetPositionVec = DirectX::XMLoadFloat3(&targetPosition);
	DirectX::XMVECTOR enemyPositionVec = DirectX::XMLoadFloat3(&enemyid.lock()->GetPosition());
	DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(targetPositionVec, enemyPositionVec);
	DirectX::XMFLOAT3 direction;
	DirectX::XMStoreFloat3(&direction, directionVec);

	// �{�X�̊o���A�j���[�V�����I��
	if (!model->IsPlayAnimation() && enemyid.lock()->GetIsEnemyAwakened() && attackCount < attackCountMax)
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
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
	}
	// �ړ�
	if (animationTime <= 4.6f)
	{
		moveid.lock()->Turn(direction, turnSpeed, elapsedTime);
		moveid.lock()->Move(direction, moveSpeed, elapsedTime);
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
	// �\�����쒆�͓����蔻�薳��
	else
		return;

	// �ړI�n�ɂ��̔��a��������
	float radius = enemyid.lock()->GetAttackRightFootRange();
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
			enemyid.lock()->InputJampSe();
			smorker->Play(bossRightFootPosition, scaleEffect);
			// ����
			enemyid.lock()->DetectHitByBodyPart(bossLeftFootPosition, applyDamage);
			// �E��
			enemyid.lock()->DetectHitByBodyPart(bossRightFootPosition, applyDamage);
			// �E�r
			enemyid.lock()->DetectHitByBodyPart(bossRightHandPosition, applyDamage);
			// ���r
			enemyid.lock()->DetectHitByBodyPart(bossLeftHandPosition, applyDamage);
			// ���S
			enemyid.lock()->DetectHitByBodyPart(transformid.lock()->GetPosition(), applyDamage);
		}

		// �����n�ʂɂ�
		if (animationTime + FLT_EPSILON >= 4.6f - FLT_EPSILON &&
			animationTime - FLT_EPSILON <= 4.7f + FLT_EPSILON)
		{
			// ��
			enemyid.lock()->InputJampSe();
			smorker->Play(bossLeftFootPosition, scaleEffect);
			// ����
			enemyid.lock()->DetectHitByBodyPart(bossLeftFootPosition, applyDamage);
			// �E��
			enemyid.lock()->DetectHitByBodyPart(bossRightFootPosition, applyDamage);
			// �E�r
			enemyid.lock()->DetectHitByBodyPart(bossRightHandPosition, applyDamage);
			// ���r
			enemyid.lock()->DetectHitByBodyPart(bossLeftHandPosition, applyDamage);
			// ���S
			enemyid.lock()->DetectHitByBodyPart(transformid.lock()->GetPosition(), applyDamage);
		}
	}
}

// �p�j�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void WanderState::Exit()
{
	// owner�Ăяo��
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	// �A���U��
	attackCount = attackCountMin;
	enemyid.lock()->InputSlashSe();
}

// ������
void IdleState::Enter()
{
	// owner�Ăяo��
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<HP> hp = owner.lock()->GetComponent<HP>();
	modelAnim.index = EnemyBoss::Anim_Standby;
	modelAnim.loop = true;
	modelAnim.currentanimationseconds = 0.0f;
	modelAnim.blendSeconds = 0.7f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	stateTimer = stateTimerMax;
	// ���������������
	hp.lock()->ResetOnDamageThresholdTime();
	randRoot = rand() % 2;
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
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Pursuit));
	}
	// ���_���[�W�̍ő�l
	float damageThreshold = 10;
	float timeLimit = 5;
	// ����
	if (hp.lock()->CheckDamageThresholdWithinTime(elapsedTime, damageThreshold, timeLimit)&&
		randRoot == (int)EnemyBoss::ExitRoot::ExitJamp)
	{
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Jump));
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
	std::weak_ptr<HP> hp = owner.lock()->GetComponent<HP>();
	// ���������������
	hp.lock()->ResetOnDamageThresholdTime();
}

// ������
void PursuitState::Enter()
{
	modelAnim.index = EnemyBoss::Animation::Anim_Standby;
	modelAnim.loop = true;
	modelAnim.currentanimationseconds = 0.0f;
	modelAnim.blendSeconds = 0.7f;
	// �G�l�~�[�A�N�Z�X�p
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	enemyid.lock()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		modelAnim);
	// �A�j���[�V�������[��
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	// �U���̎�ނ��o��
	randamAttack = rand() % 3;
}
// update
void PursuitState::Execute(float elapsedTime)
{
	// �G�l�~�[�A�N�Z�X�p
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	// �ڕW�n�_�����v���C���[�ʒu�ɐݒ�
	DirectX::XMFLOAT3 targetPosition = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1)->GetComponent<Transform>()->GetPosition();
	enemyid.lock()->SetTargetPosition(targetPosition);
	float vx = targetPosition.x - enemyid.lock()->GetPosition().x;
	float vy = targetPosition.y - enemyid.lock()->GetPosition().y;
	float vz = targetPosition.z - enemyid.lock()->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);
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
	case (int)EnemyBoss::AttackMode::DushAttack:
	{
		enemyid.lock()->GetStateMachine()->ChangeState(
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
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	modelAnim.index = EnemyBoss::Animation::Anim_Attack;
	modelAnim.loop = true;
	modelAnim.currentanimationseconds = 2.0f;
	modelAnim.blendSeconds = 0.7f;
	modelAnim.currentAnimationAddSeconds = 0.01f;

	Model* model =  owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<HP> hpid = owner.lock()->GetComponent<HP>();
	// �꒣��
	enemyid.lock()->SetRandomTargetPosition();
	// �A�j���[�V�����Đ�
	model->
		PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Reverseplayback);
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
	// �ړI�n�ɂ��̔��a��������
	float radius = enemyid.lock()->GetAttackRightFootRange();
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
		bool blurCheck = true;
		enemyid.lock()->SetBlurCheck(blurCheck);
		enemyid.lock()->CollisionPlayerWithRushEnemy();
		moveid.lock()->JumpVelocity(jumpSpeed);
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
		moveid.lock()->Turn(direction, turnSpeed, elapsedTime);
	}

	if (!moveid.lock()->GetOnLadius())
	{
		moveid.lock()->Move(direction, moveSpeed, elapsedTime);
		moveid.lock()->Turn(direction, turnSpeed, elapsedTime);
	}

	if (moveid.lock()->GetOnLadius() && jumpStart)
	{
		enemyid.lock()->InputImpact(enemyid.lock()->GetPosition());
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
		enemyid.lock()->InputJump();
		// ���G�t�F�N�g
		smorker->Play(bossLeftFootPosition, scaleEffect);
		PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance().Instance();
		// ���W�A���u���[
		radialBlurData.radius = 30;
		postprocessingRenderer.SetRadialBlurData(radialBlurData);
		 // ���������蔻��
		enemyid.lock()->DetectHitByBodyPart(bossLeftFootPosition, applyDamageJumpStart);
	}
}
// �I������
void JumpState::Exit()
{
}

// ������
void AttackState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<HP> hpid = owner.lock()->GetComponent<HP>();
	// �꒣��
	enemyid.lock()->SetRandomTargetPosition();
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
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Reverseplayback);
	// �A�j���[�V�����I��
	dushStart = false;
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
	charge = std::make_unique<Effect>("Data/Effect/effectCharge.efk");
	chargeCompleate = std::make_unique<Effect>("Data/Effect/chargecompleted.efk");
}
// update
void AttackState::Execute(float elapsedTime)
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
    std::weak_ptr<HP> hpid = owner.lock()->GetComponent<HP>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	if (PlayerManager::Instance().GetPlayerCount() >= 1)
	{
		std::weak_ptr<Transform> pTransformId = PlayerManager::Instance().GetPlayer(0)->GetComponent<Transform>();

		targetPosition = pTransformId.lock()->GetPosition();
	}
	DirectX::XMVECTOR targetPositionVec = DirectX::XMLoadFloat3(&targetPosition);
	DirectX::XMVECTOR enemyPositionVec = DirectX::XMLoadFloat3(&enemyid.lock()->GetPosition());
	DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(targetPositionVec, enemyPositionVec);
	DirectX::XMStoreFloat3(&directionP, directionVec);
	angle = transformid.lock()->GetAngle();
	direction.x = sinf(angle.y);// �O�p���΂߂ɂ��Ĉʒu��ς���
	direction.y = 0;
	direction.z = cosf(angle.y);
	// �ړI�n�ɂ��̔��a��������
	float radius = enemyid.lock()->GetAttackRightFootRange();
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
		enemyid.lock()->InputJampSe();
		enemyid.lock()->InputChargeSe();
		charge->Play(bossEyePosition);
		chargeCompleate->Play(bossEyePosition);
		// �`���[�W��
		stateChargeTimer = stateChargeTimerMax;
		chargeInitilize = true;
		// �A�j���[�V�������[��
		enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Stop);
	}

	// �_�b�V������܂�
	if (!dushStart)
	{
		stateChargeTimer -= 0.01f;
		moveid.lock()->Turn(directionP, turnSpeed, elapsedTime);
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
		enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
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
		enemyid.lock()->InputDashSe();
		charge->Stop(charge->GetEfeHandle());
		// ���G�t�F�N�g
		smorker->Play(bossLeftFootPosition, scaleEffect);
		moveid.lock()->Move(direction, moveSpeed, elapsedTime);
		stateChargeCompletionTimer -= 0.01f;
		// �E�������蔻��
		enemyid.lock()->DetectHitByBodyPart(bossRightFootPosition, applyDamageAttack);
		// ���������蔻��
		enemyid.lock()->DetectHitByBodyPart(bossLeftFootPosition, applyDamageAttack);
	}

	// �_�b�V���I��
	if (dushStart && stateChargeCompletionTimer - FLT_EPSILON <= stateChargeCompletionTimerEnd + FLT_EPSILON)
	{
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
	}
}
// �Ō�ɂ�肽�������S��
void AttackState::Exit()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	enemyid.lock()->InputStopDashSe();
	chargeInitilize = false;
	if (charge->GetEfeHandle())
		charge->Stop(charge->GetEfeHandle());
	if (chargeCompleate->GetEfeHandle())
		chargeCompleate->Stop(chargeCompleate->GetEfeHandle());
}
// �_���[�W����
void DamageState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	enemyid.lock()->InputDamageSe();
	modelAnim.index = EnemyBoss::Animation::Anim_Die;
	modelAnim.loop = true;
	modelAnim.currentanimationseconds = 2.5f;
	modelAnim.blendSeconds = 0.7f;
	modelAnim.currentAnimationAddSeconds = 0.025f;
	modelAnim.keyFrameEnd = 3.0f;
	Model* momdel = owner.lock()->GetComponent<ModelControll>()->GetModel();
	momdel->
		PlayAnimation(
			modelAnim);
	// �A�j���[�V�������[��
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	stateTimer = stateTimerMax;
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
}
// �����J�n
void ConfusionState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = EnemyBoss::Animation::Anim_Die;
	modelAnim.loop = false;
	modelAnim.currentanimationseconds = 0.426f;
	modelAnim.blendSeconds = 0.35f;
	modelAnim.currentAnimationAddSeconds = 0.000f;
	modelAnim.keyFrameEnd = 70.0f;

	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	// �X�e�[�g�ҋ@����
	stateTimer = stateTimerMax;
	// �U�����
	randamAttack = rand() % 2;
}
// �����X�V
void ConfusionState::Execute(float elapsedTime)
{
	stateTimer -= elapsedTime;
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	if (!model->IsPlayAnimation())
		model->PlayAnimation(modelAnim);
	if (stateTimer > stateTimerEnd)return;
	switch (randamAttack)
	{
	case (int)EnemyBoss::AttackMode::AssaultAttack:
	{
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Wander));
		break;
	}
	case (int)EnemyBoss::AttackMode::JumpStompAttack:
	{
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Jump));
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
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	modelAnim.index = EnemyBoss::Animation::Anim_Die;
	modelAnim.loop = false;
	Model* model;
	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	model->
		PlayAnimation(modelAnim);
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
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
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
}

// �������
void AwakeStartState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
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
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
}

void AwakeStartState::Execute(float elapsedTime)
{
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
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
		enemyid.lock()->GetStateMachine()->ChangeState(static_cast<int>(EnemyBoss::State::Idle));
	}
}

void AwakeStartState::Exit()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	// �o������
	enemyid.lock()->ResetAwakeTime();
}
// �N���A�|��đҋ@��
void ClearState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	// �ʒu���
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();
	position.z -= 5.5f;
	transformid.lock()->SetPosition(position);
	DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();
	angle.y += 1.5f;
	transformid.lock()->SetAngle(angle);
	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
	modelAnim.index = EnemyBoss::Animation::Anim_Die;
	modelAnim.loop = false;
	modelAnim.blendSeconds = 0.7f;
	Model* model;
	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
	stateTimer = 3;
}

void ClearState::Execute(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	stateTimer -= elapsedTime;
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = model->GetCurrentANimationSeconds();
	const GamePadButton anyButton =
		GamePad::BTN_B;
	if (!model->IsPlayAnimation() )// ���[�h�̎��Q�[���Ƃ���������
	{
		enemyid.lock()->SetClearCheck(clearCheck);
	}
}

void ClearState::Exit()
{
}
// �N���A��������
void ClearReviveState::Enter()
{
	std::weak_ptr<EnemyBoss> enemyid = owner.lock()->GetComponent<EnemyBoss>();
	// �ʒu���
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();
	position.z -= 5.5f;
	transformid.lock()->SetPosition(position);
	DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();
	angle.y += 1.5f;
	transformid.lock()->SetAngle(angle);
	modelAnim.index = EnemyBoss::Animation::Anim_Die;
	modelAnim.loop = false;
	modelAnim.currentanimationseconds = 1.5f;
	modelAnim.blendSeconds = 0.7f;
	Model* model;
	model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	model->
		PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	enemyid.lock()->SetUpdateAnim(EnemyBoss::UpAnim::Normal);
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
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Anim_Idle;
	modelAnim.loop = true;
	modelAnim.blendSeconds = 0.5f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
	moveid.lock()->SetStopMove(false);
}

void PlayerIdleState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	// ���b�N�I������
	playerid.lock()->UpdateCameraState(elapsedTime);
	// �ړ����͏���
	if (playerid.lock()->InputMove())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	}
	// �����͏���
	if (playerid.lock()->InputAvoidance())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}
	// �W�����v���͏���
	if (playerid.lock()->InputJump())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
	}
}

void PlayerIdleState::Exit()
{
}
// �v���C���[�ړ�
void PlayerMovestate::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	// se�Đ�
	playerid.lock()->InputWalkSE();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Anim_Running;
	modelAnim.loop = true;
	modelAnim.blendSeconds = 0.7f;
	modelAnim.currentAnimationAddSeconds = 0.025f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
	// ������
	moveid.lock()->SetStopMove(false);
}

void PlayerMovestate::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	// �ړ��l
	playerid.lock()->GetMoveVec(elapsedTime);
	// ���b�N�I������
	playerid.lock()->UpdateCameraState(elapsedTime);
	// �ړ����͏���
	if (!playerid.lock()->InputMove())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
	}
	// �����͏���
	if (playerid.lock()->InputAvoidance())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
	}
	// �W�����v���͏���
	if (playerid.lock()->InputJump())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
	}
}

void PlayerMovestate::Exit()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	//se�Đ��폜
	playerid.lock()->InputStopWalkSE();
}

// �v���C���[�W�����v
void PlayerJumpState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Animation::Anim_Jump;
	modelAnim.keyFrameEnd = 46.0f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
	// ������
	moveid.lock()->SetStopMove(false);
	// se�쐬
	playerid.lock()->InputJampSE();
}

void PlayerJumpState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	// ���b�N�I������
	playerid.lock()->UpdateCameraState(elapsedTime);
	// �ړ�
	if (playerid.lock()->InputMove())
	{
		playerid.lock()->GetMoveVec(elapsedTime);
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
	// ���n�ړ�
	if (moveid.lock()->GetOnLadius() && playerid.lock()->InputMove())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	}
}

void PlayerJumpState::Exit()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	// �W�����v��
	playerid.lock()->InputJampSE();
}
// �v���C���[���n
void PlayerLandState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Anim_Land;
	modelAnim.currentanimationseconds = 0.35f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
	// se�쐬
	playerid.lock()->InputJampSE();
}

void PlayerLandState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	// ���b�N�I������
	playerid.lock()->UpdateCameraState(elapsedTime);
	// ����
	if (playerid.lock()->InputMove())
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
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
		// �W�����v��
	playerid.lock()->InputJampSE();
}
// �v���C���[�W�����v��
void PlayerJumpFlipState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Anim_Jump;
	modelAnim.currentAnimationAddSeconds = 0.03f;
	modelAnim.keyFrameEnd = 25.0f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
	// se�쐬
	playerid.lock()->InputJampSE();
}

void PlayerJumpFlipState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	// ���b�N�I������
	playerid.lock()->UpdateCameraState(elapsedTime);
	// �ړ�
	if (playerid.lock()->InputMove())
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
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	playerid.lock()->InputStopJampSE();
}
// �v���C���[����
void PlayerQuickJabState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
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
	moveid.lock()->SetStopFall(stopFall);

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
	if (playerid.lock()->InputAvoidance() && playerid.lock()->InputMove())
	{
		bool stop = false;
		moveid.lock()->SetStopMove(stop);
		DirectX::XMFLOAT3 vec;
		vec = playerid.lock()->GetMoveVec(elapsedTime);
		moveid.lock()->Turn(vec, turnSpeed, elapsedTime);
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Avoidance));
		button = false;
		buttonSeconde = false;
		return;
	}
	DirectX::XMVECTOR Vector;
	DirectX::XMVECTOR VectorXZ;
	DirectX::XMVECTOR LengthSq;
	playerPosXZ.x = transformid.lock()->GetPosition().x;
	playerPosXZ.y = transformid.lock()->GetPosition().z;
	DirectX::XMVECTOR playerPositionXZ = DirectX::XMLoadFloat2(&playerPosXZ);
	DirectX::XMVECTOR playerPosition = DirectX::XMLoadFloat3(&transformid.lock()->GetPosition());
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
	if (attackMemory > attackMemoryMax)
	{
		// ���͊m�F�ŃX�e�[�g�ύX
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
		// �U���ő�l����
		attackMemory = 0;
	}

	// ����̕�������
	// ��]
	if (!rotateCheck)
	{
		isEnemyHit = !playerid.lock()->GetIsEnemyHit() && !playerid.lock()->GetIsEnemyHitBody() ?
			EnemySafe : EnemyHit;
		// ����
		if (length < attackCheckRange && !isEnemyHit)
		{
			bool stop = false;
			moveid.lock()->SetStopMove(stop);
			// ����
			isPlayerDrawCheck = 0;
			playerid.lock()->SetPlayeDrawCheck(isPlayerDrawCheck);
			// ����
			moveid.lock()->Turn(vector, turnSpeed, elapsedTime);
			moveid.lock()->Move(vector, speed, elapsedTime);

			// �z�[�~���O���̂ݍĐ�
			if (isHomingStartCheck)
			{
				// Se�Đ�
				playerid.lock()->PlayTellePortSe();
				// �G�t�F�N�g
				tellePort->Play(transformid.lock()->GetPosition());
				// �z�[�~���O���݂̂̂���
				isHomingStartCheck = false;
			}

			return;
		}
		else
		{
			// �`��
			isPlayerDrawCheck = 1;
			playerid.lock()->SetPlayeDrawCheck(isPlayerDrawCheck);
			bool stop = true;
			moveid.lock()->SetStopMove(stop);
			rotateCheck = true;
			// �z�[�~���O�I��
			isHomingStartCheck = false;

			// �A�j���[�V�����Đ�
			// ��
			if (!moveid.lock()->GetOnLadius())
			{
				//// �󒆍U���ōs���K������
				//playerid.lock()->AreAttackDecreaseAmount();
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
				playerid.lock()->SetUpdateAnim(Player::UpAnim::Doble);
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
				playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
			}
		}
	}

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = moveid.lock()->GetOnLadius() ? 
		model->GetCurrentANimationSeconds() : 
		model->GetCurrentAnimationSecondsUpeer();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B

	// 1����
	if (animationTime >= 0.8f  && playerid.lock()->InputAttack())
	{
		// �P��ڂ̍U���Ȃ�
		oneAttackCheck = false;
		playerid.lock()->SetAttackCollisionFlag(false);
		deleteCheck = false;
		// ���͊m�F�ŃX�e�[�g�ύX
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(button  ? Player::State::SideCut : Player::State::Move));
		return;
	}
	// �a���̓����蔻��
	playerid.lock()->CollisionNodeVsEnemies("mixamorig:LeftHand",
		owner.lock()->GetComponent<Player>()->GetLeftHandRadius(),
		"body2", "boss_left_hand2", "boss_right_hand2");
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

	// �`��
	isPlayerDrawCheck = 1;
	// ��\���̏ꍇ�\������
	playerid.lock()->SetPlayeDrawCheck(isPlayerDrawCheck);
}
// 2�A����
void PlayerSideCutState::Enter()
{
	bool loop = false;
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	if (!moveid.lock()->GetOnLadius())
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
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Doble);
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
	    playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
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
	moveid.lock()->SetStopFall(stopFall);
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

	// �A���{�^���m�F
	if (playerid.lock()->InputAttack() &&
		playerid.lock()->GetSelectCheck() ==
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
	if (playerid.lock()->InputAvoidance() && playerid.lock()->InputMove())
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
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = moveid.lock()->GetOnLadius() ?
		model->GetCurrentANimationSeconds() :
		model->GetCurrentAnimationSecondsUpeer();
	// 2����
	if (animationTime >= 1.1f)
	{
		playerid.lock()->SetAttackCollisionFlag(false);
		deleteCheck = false;
		// ���͊m�F�ŃX�e�[�g�ύX
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(buttonSeconde ? Player::State::CycloneStrike : Player::State::Move));
		return;
	}
	// �U�������蔻��
	playerid.lock()->CollisionNodeVsEnemies("mixamorig:LeftHand"
		, playerid.lock()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");
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
}
// 3����
void PlayerCycloneStrikeState::Enter()
{
	bool loop = false;
	// �R���|�l���g�擾
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	if (!moveid.lock()->GetOnLadius())
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
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Doble);
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
		playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
	}
	// �d�̓I�t
	bool stopFall = true;
	moveid.lock()->SetStopFall(stopFall);
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
	// ���
	if (playerid.lock()->InputAvoidance() && playerid.lock()->InputMove())
	{
		bool stop = false;
		moveid.lock()->SetStopMove(stop);
		DirectX::XMFLOAT3 vec;
		vec = playerid.lock()->GetMoveVec(elapsedTime);
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
				// ����
				if (moveid.lock()->Turn(vector, turnSpeed, elapsedTime))
				{

					moveid.lock()->Move(vector, speed, elapsedTime);

				}
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
	// 3����
	if (animationTime >= 2.7f )
	{
		playerid.lock()->SetAttackCollisionFlag(false);
		deleteCheck = false;
		// ���͊m�F�ŃX�e�[�g�ύX
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
		return;
	}
	// �����蔻��
	playerid.lock()->CollisionNodeVsEnemies("mixamorig:LeftHand",
		playerid.lock()->GetLeftHandRadius(), "body2", "boss_left_hand2", "boss_right_hand2");
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
}
// ����Z�a��
void PlayerSpecialAttackState::Enter()
{
	// �R���|�l���g�擾
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	loopSe = false;
	std::weak_ptr<Transform> enemyTransform = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<Transform>();
	std::weak_ptr<HP> enemyHpId = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<HP>();
	lightning = std::make_unique<Effect>("Data/Effect/sunder.efk");
	lightningAttack = std::make_unique<Effect>("Data/Effect/HitThunder.efk");
	lightningHit = std::make_unique<Effect>("Data/Effect/lightningStrike.efk");
	if (!moveid.lock()->GetOnLadius())
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
		// �A�j���[�V�������[��
		Model::ModelAnim modelAnim;
		modelAnim.index = Player::Animation::Anim_MagicSeconde;
		// �A�j���[�V�����Đ�
		model->PlayAnimation(modelAnim);
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
	playerid.lock()->InputAttackThanderSE();
	// ��]����
	isRotate = true;
	bool specialRockOff = true;
	playerid.lock()->SetSpecialRockOff(specialRockOff);
}

void PlayerSpecialAttackState::Execute(float elapsedTime)
{
	// �R���|�l���g�擾
	auto a = owner.lock();
	if (!a)
		return;

	std::shared_ptr<Player> playerid = a->GetComponent<Player>();
	std::shared_ptr<Movement> moveid = a->GetComponent<Movement>();
	std::shared_ptr<Transform> transformid = a->GetComponent<Transform>();
	Model* model = a->GetComponent<ModelControll>()->GetModel();
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
			if (moveid.lock()->Turn(vector, turnSpeed, elapsedTime))
			{
				// �J��������
				{
					MessageData::CAMERACHANGEMOTIONMODEDATA	p;
					position = transformid.lock()->GetPosition();
					angle = transformid.lock()->GetAngle();
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
		float animationTime = moveid.lock()->GetOnLadius() ?
			model->GetCurrentANimationSeconds() :
			model->GetCurrentAnimationSecondsUpeer();
		// �A�j���[�V����
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON)
		{
			if (!moveid.lock()->GetOnLadius())
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
			playerid.lock()->InputStopAttackThanderSE();
		}
	}
	else
	{
			// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
			float animationTime = moveid.lock()->GetOnLadius() ?
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
			playerid.lock()->SetHitCheck(true);
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
			playerid.lock()->InputAttackSlashSpecileLightningStrikeSE();
			if (enemyHpId.lock()->ApplyDamage(10, 0.5f)) 
			{
				lightningHit->Play(pPosition);
			}
			playerid.lock()->SetHitCheck(false);
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
	PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
	// �Œ�l�ݒ�
	postprocessingRenderer.StepValueMin();
    ColorGradingData colorGradingData;
	postprocessingRenderer.SetColorGradingMinData(colorGradingData);
	bool specialRockOff = false;
	playerid.lock()->SetSpecialRockOff(specialRockOff);
}
// ����Z�Ζ��@
void PlayerMagicState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	std::weak_ptr<Mp> mpId = owner.lock()->GetComponent<Mp>();

	// se�Đ�
	seParam.filename = "Data/Audio/SE/�`���[�W���G.wav";
	seParam.volume = 0.5f;
	playerid.lock()->InputSe(seParam);

	// �G�t�F�N�g
	charge = std::make_unique<Effect>("Data/Effect/magicCharge.efk");
	chargeComplet = std::make_unique<Effect>("Data/Effect/magicChargeComplet.efk");
	
	magicType = playerid.lock()->GetSelectMagicCheck();
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
			heale->Play(transformid.lock()->GetPosition());

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
			charge->Play(transformid.lock()->GetPosition());
			break;
		}
	}
	// �A�j���[�V�����Đ�
	owner.lock()->GetComponent<Player>()->SetUpdateAnim(Player::UpAnim::Normal);
	// ������̒�~
	bool stopFall = true;
	moveid.lock()->SetStopFall(stopFall);
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
	angle = transformid.lock()->GetAngle();

	angle.x += DirectX::XMConvertToRadians(20);
	angle.y -= DirectX::XMConvertToRadians(60);
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

	// ����
	// ���@����
	if (magicCharge < magicChargeMax && animationTime >= 1.6f &&
		playerid.lock()->InputMagick())
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
		chargeComplet->Play(transformid.lock()->GetPosition());
	}

	// ���@���ߏI��
	if (magicCharge >= magicChargeMax || 
		!playerid.lock()->InputMagick() && 
		!magicStart)
	{
		// se��~
		playerid.lock()->StopSe(seParam);

		// �G�t�F�N�g�Đ����Ă����~
		if (charge->GetEfeHandle())
		{
			// �`���[�W�G�t�F�N�g��~
			charge->Stop(charge->GetEfeHandle());

			// �`���[�W����
			chargeComplet->Play(transformid.lock()->GetPosition());
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

	// ����̕�������
	// ��]
	if (isRotate)
	{
		// ����
		if (moveid.lock()->TurnCheck(
				vector, angleRange, elapsedTime) &&
			playerid.lock()->GetRockCheck())
		{
			// ����
			moveid.lock()->Turn(vector,turnSpeed,elapsedTime);
		}
		else
		{
			isRotate = false;
		}
	}

	// UI�����̏�����
	owner.lock()->GetComponent<Player>()->StartMagicUiFire();
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
			angle = transformid.lock()->GetAngle();
			owner.lock()->GetComponent<Player>()->InputMagicframe();
		}
		// ����
		else
		{
			// ������
			owner.lock()->GetComponent<Player>()->PushMagicFrame(angle);
		}
		// SE��
		owner.lock()->GetComponent<Player>()->InputAttackFlameSE();

		// ���ߏI���܂Ō���
		if (magicCharge <= magicChargeEnd)
			owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Idle);

		else
			--magicCharge;
		break;
	}

	case (int)Player::CommandMagic::Thander:
	{
		// ����
		if (animationTime <= 1.1f)return;

		// ����
		owner.lock()->GetComponent<Player>()->InputAttackThanderSE();

		// ������
		owner.lock()->GetComponent<Player>()->InputMagicLightning();

		// ���ߏI���܂Ō���
		if (magicCharge <= magicChargeEnd)
			owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Idle);

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
		playerid.lock()->InputAttackIceSE();

		// �P��
		if (magicCharge <= magicChargeEnd)
		{
			// �X����
			owner.lock()->GetComponent<Player>()->InputMagicIce();
		}
		else
		{
			// �p�x
			angle = transformid.lock()->GetAngle();
			playerid.lock()->PushMagicIce(angle);
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
			owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Idle);
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
		owner.lock()->GetComponent<Player>()->InputAttackHealeSE();
		// ������
		owner.lock()->GetComponent<Player>()->InputMagicHealing();
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Idle);
		break;
	}
	default:
		break;
	}
}

void PlayerMagicState::Exit()
{
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

	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
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
}

void PlayerSpecialMagicState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	fire = std::make_unique<Effect>("Data/Effect/fire.efk");
	fireAttack = std::make_unique<Effect>("Data/Effect/hit fire.efk");
	modelAnim.index = Player::Animation::Anim_SlashThree;
	// �A�j���[�V�����Đ�
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�����Đ�
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
	Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");
	DirectX::XMFLOAT3 pPosition =
	{
				pHPosiiton->worldTransform._41,
				pHPosiiton->worldTransform._42,
				pHPosiiton->worldTransform._43
	};
	// ����
	playerid.lock()->InputAttackFlameSE();
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
	bool specialRockOff = true;
	playerid.lock()->SetSpecialRockOff(specialRockOff);
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
			playerid.lock()->SetFlashOn(true);
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
			// �K�E�Z���������̌o�ߎ���
			specialMoveWaitTime += elapsedTime;
			PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
			ColorGradingData colorGradingData;
			colorGradingData.brigthness = 1.5f;
			postprocessingRenderer.SetColorGradingData(colorGradingData);
		}
		if (animationTime >= 1.1f && animationTime <= 1.2f)
		{
			playerid.lock()->
				InputSpecialMagicframe();
			playerid.lock()->InputAttackFlameSpecileSE();
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
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
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
	PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
	// �Œ�l�ݒ�
	postprocessingRenderer.StepValueMin();
	bool specialRockOff = false;
	playerid.lock()->SetSpecialRockOff(specialRockOff);
}

void PlayerDamageState::Enter()
{
	// �R���|�l���g�擾
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	bool loop = false;
	// ��������
	bool stopMove = false;
	moveid.lock()->SetStopMove(stopMove);
	// �d�̓I��
	bool stopFall = false;
	moveid.lock()->SetStopFall(stopFall);
	modelAnim.index = Player::Animation::Anim_Pain;
	model->PlayAnimation(modelAnim);
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
}

void PlayerDeathState::Enter()
{
	// �R���|�l���g�擾
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	float				stateTimer = 0.0f;
	modelAnim.index = Player::Animation::Anim_Deth;
	model->PlayAnimation(modelAnim);
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
	stateTimer = 0.0f;
	PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
	colorGradingData.saturation = 0.0f;
	postprocessingRenderer.SetColorGradingMinData(colorGradingData);
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
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	// �����g
	modelAnim.index = Player::Animation::Anim_Dush;
	modelAnim.currentanimationseconds = 0.3f;
	modelAnim.currentAnimationAddSeconds = 0.025f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V������� �ʏ�
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
	// ������̒�~
	bool stopFall = true;
	moveid.lock()->SetStopFall(stopFall);
	playerid.lock()->InputDashSE();
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
	// �Ռ��g������
	impactInitialize = true;
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
	// ��]
	if (animationTime <= 0.5f)
	{
		// ���͏����擾
		GamePad& gamePad = Input::Instance().GetGamePad();
		rotateVec.x = gamePad.GetAxisLX();
		rotateVec.y = 0.0f;
		rotateVec.z = gamePad.GetAxisLY();

		moveid.lock()->Turn(rotateVec, rotateSpeed, elapsedTime);
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
		moveid.lock()->AddImpulse(impulse, impactInitialize);
		impactInitialize = false;
	}

	// �A�j���[�V�����_�b�V����ʏ�ړ�
	if (animationTime >= 1.0f && owner.lock()->GetComponent<Player>()->InputMove())
	{
		owner.lock()->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
	}
	// �ҋ@
	if (animationTime >= 2.0f)
	{
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
}

// ���ˊJ�n
void PlayerReflectionState::Enter()
{
	// �R���|�[�l���g�Ăяo��
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	bool loop = false;
	modelAnim.index = Player::Animation::Anim_Counter;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V������� �ʏ�
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
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
		playerid.lock()->CollisionNodeVsEnemiesCounter("mixamorig:RightHand", owner.lock()->GetComponent<Player>()->GetLeftHandRadius());
	}
}

// ���ˏI��
void PlayerReflectionState::Exit()
{
}

void PlayerTitleIdleState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Animation::Anim_Idle;
	modelAnim.loop = true;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerTitleIdleState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	//// ���b�N�I������
    playerid.lock()->UpdateCameraState(elapsedTime);
	// �q�b�g
	if (playerid.lock()->InputAttack())
	{

		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::StateTitle::Push));
	}
}

void PlayerTitleIdleState::Exit()
{
}

void PlayerTitlePushState::Enter()
{
	// �R���|�l���g�擾
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	lightning = std::make_unique<Effect>("Data/Effect/sunder.efk");
	lightningAttack = std::make_unique<Effect>("Data/Effect/HitThunder.efk");
	lightningHit = std::make_unique<Effect>("Data/Effect/lightningStrike.efk");
	modelAnim.index = Player::Animation::Anim_MagicSeconde;
	// �A�j���[�V�����Đ�
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�����Đ�
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);

	// ������̒�~
	bool stopFall = true;
	moveid.lock()->SetStopFall(stopFall);
	// ���[�V�����؂�ւ�
	secondeMortion = false;
}

void PlayerTitlePushState::Execute(float elapsedTime)
{
	// �R���|�l���g�擾
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	std::weak_ptr<Movement> moveid = owner.lock()->GetComponent<Movement>();
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	//// ���b�N�I������
	playerid.lock()->UpdateCameraState(elapsedTime);
	std::weak_ptr<HP> enemyHpId;
	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
		float animationTime = moveid.lock()->GetOnLadius() ?
			model->GetCurrentANimationSeconds() :
			model->GetCurrentAnimationSecondsUpeer();
		// �A�j���[�V����
		if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON && !secondeMortion)
		{
			playerid.lock()->InputAttackThanderSE();
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
			playerid.lock()->InputAttackSlashSpecileLightningStrikeSE();
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
			playerid.lock()->SetEndState(true);
		}
}

void PlayerTitlePushState::Exit()
{
}

void PlayerClearIdleState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Animation::Anim_MagicSeconde;
	modelAnim.keyFrameEnd = 68.0f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();
	position.x += -3.0f;
	transformid.lock()->SetPosition(position);
}

void PlayerClearIdleState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	Model::Node* PRock = model->FindNode("mixamorig:Spine1");
	DirectX::XMFLOAT3 pos = model->ConvertLocalToWorld(PRock);
	pos.x += 3.0f;
	MessageData::CAMERACHANGEFREEMODEDATA	p = { pos };
	Messenger::Instance().SendData(MessageData::CAMERACHANGEFREEMODE, &p);
	// �q�b�g
	if (playerid.lock()->InputAttack())
	{
		// �V�[���I��
		playerid.lock()->SetEndState(true);
	}
}

void PlayerClearIdleState::Exit()
{
}

void PlayerClearEscapeState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Animation::Anim_MagicSeconde;
	modelAnim.keyFrameEnd = 68.0f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
	std::weak_ptr<Transform> transformid = owner.lock()->GetComponent<Transform>();
	DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();
	position.x += -3.0f;
	transformid.lock()->SetPosition(position);
}

void PlayerClearEscapeState::Execute(float elapsedTime)
{
}

void PlayerClearEscapeState::Exit()
{
}

void PlayerOverIdleState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
	modelAnim.index = Player::Animation::Anim_Deth;
	modelAnim.currentanimationseconds = 1.7f;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
}

void PlayerOverIdleState::Execute(float elapsedTime)
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	//// ���b�N�I������
	playerid.lock()->UpdateCameraState(elapsedTime);
	// �q�b�g
	if (playerid.lock()->InputAttack())
	{
		playerid.lock()->GetStateMachine()->ChangeState(static_cast<int>(Player::StateOver::Revive));
	}
}

void PlayerOverIdleState::Exit()
{
}

void PlayerOverReviveState::Enter()
{
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	modelAnim.index = Player::Animation::Anim_Magic;
	model->PlayAnimation(modelAnim);
	// �A�j���[�V�������[��
	playerid.lock()->SetUpdateAnim(Player::UpAnim::Normal);
	isPoseStarte = false;
}

void PlayerOverReviveState::Execute(float elapsedTime)
{
	Model* model = owner.lock()->GetComponent<ModelControll>()->GetModel();
	if (!model->IsPlayAnimation() && !isPoseStarte)
	{
		modelAnim.index = Player::Animation::Anim_Jump;
		model->PlayAnimation(modelAnim);
		isPoseStarte = true;
		return;
	}
	std::weak_ptr<Player> playerid = owner.lock()->GetComponent<Player>();
	if (!model->IsPlayAnimation() && isPoseStarte)
	{
		playerid.lock()->SetEndState(endState);
	}
}

void PlayerOverReviveState::Exit()
{
}
