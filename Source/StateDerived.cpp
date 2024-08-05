#include "EnemySlime.h"
#include "StateDerived.h"
#include "Input\GamePad.h"
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
	// �꒣��
	owner->GetComponent<EnemySlime>()->SetRandomTargetPosition();

	// �A�j���[�V�����Đ�
	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetComponent<EnemySlime>()->GetAnimationStateWalk(), true);
}

// �p�j�X�e�[�g�Ŏ��s���郁�\�b�h
void WanderState::Execute(float elapsedTime)
{
	// �ړI�n�_�܂ł�XZ���ʂł̋�������
	float vx = owner->GetComponent<EnemySlime>()->GetTargetPosition().x - owner->GetComponent<EnemySlime>()->GetPosition().x;
	float vz = owner->GetComponent<EnemySlime>()->GetTargetPosition().z - owner->GetComponent<EnemySlime>()->GetPosition().z;
	float distSq = vx * vx + vz * vz;

	// �ړI�n�֒�����
	float radius = owner->GetComponent<EnemySlime>()->GetRadius();
	if (distSq < radius * radius)
	{
		// �ҋ@�X�e�[�g�֑J��
		// ChangeState�N���X��State��؂�ւ���
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Idle));
	}
	// �ړI�n�_�ֈړ�
	owner->GetComponent<EnemySlime>()->MoveToTarget(elapsedTime, 0.5f);
	// �v���C���[���G
	if (owner->GetComponent<EnemySlime>()->SearchPlayer())
	{
		// ����������ǐՃX�e�[�g�֑J��
		// ChangeState�N���X��State��؂�ւ���
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	}

}

// �p�j�X�e�[�g����o�Ă����Ƃ��̃��\�b�h
void WanderState::Exit()
{

}

// ������
void IdleState::Enter()
{
	owner->GetComponent<EnemySlime>()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetComponent<EnemySlime>()->GetAnimationStateNormal(), true);
	// �^�C�}�[�������_���ݒ�
	//stateTimer = Mathf::RandomRange(3.0f, 5.0f);
	owner->GetComponent<EnemySlime>()->SetStateTimer(Mathf::RandomRange(3.0f, 5.0f));

}

// update
void IdleState::Execute(float elapsedTime)
{
	// �^�C�}�[����
     //stateTimer -= elapsedTime;
	/* owner->SetStateTimer(stateTimer);*/
	owner->GetComponent<EnemySlime>()->SetStateTimer(owner->GetComponent<EnemySlime>()->GetStateTimer() - elapsedTime);
	// TODO 03 
	// �ҋ@���Ԃ��o�߂����Ƃ��p�j�X�e�[�g�֑J�ڂ��Ȃ���
	if (owner->GetComponent<EnemySlime>()->GetStateTimer() < 0.0f)
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Wander));
	// TODO 03 
	// �v���C���[�����������Ƃ��ǐՃX�e�[�g�֑J�ڂ��Ȃ���
	if (owner->GetComponent<EnemySlime>()->SearchPlayer())
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
}

// �Ō�ɂ�肽�������S��
void IdleState::Exit()
{
}
// ������
void PursuitState::Enter()
{
	
	owner->GetComponent<EnemySlime>()->GetActor()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetComponent<EnemySlime>()->GetAnimationStateWalk(), true);

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


	owner->GetComponent<EnemySlime>()->SetTargetPosition(targetPosition);

	// �ړI�n�_�ֈړ�
	owner->GetComponent<EnemySlime>()->MoveToTarget(elapsedTime, 1.0);

	
	// �^�C�}�[����
	stateTimer -= elapsedTime;
	/*owner->SetStateTimer(stateTimer);*/

	// TODO 03 
	// �ǐՎ��Ԃ��o�߂����Ƃ��ҋ@�X�e�[�g�֑J�ڂ��Ȃ���
	if (stateTimer < 0.0f)
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Idle));


	float vx = targetPosition.x - owner->GetComponent<EnemySlime>()->GetPosition().x;
	float vy = targetPosition.y - owner->GetComponent<EnemySlime>()->GetPosition().y;
	float vz = targetPosition.z - owner->GetComponent<EnemySlime>()->GetPosition().z;
	float dist = sqrtf(vx * vx + vy * vy + vz * vz);

	// TODO 03 
	// �U���͈͂ɓ������Ƃ��U���X�e�[�g�֑J�ڂ��Ȃ���
	if (dist < attackRange)
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Attack));

}
// �Ō�ɂ�肽�������S��
void PursuitState::Exit()
{
}
// ������
void AttackState::Enter()
{

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(owner->GetComponent<EnemySlime>()->GetAnimationStateAttack(), false);
}
// update
void AttackState::Execute(float elapsedTime)
{


	// �U�����[�V�������I�������Ƃ��ǐՃX�e�[�g�ֈڍs
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		owner->GetComponent<EnemySlime>()->SetCounterJudgment(false);
		owner->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Pursuit));
	}

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentANimationSeconds();
	if (animationTime >= 0.2f && animationTime <= 0.35f)
	{
		// �ڋʃm�[�h�ƃv���C���[�̏Փˏ���
		owner->GetComponent<EnemySlime>()->CollisitionNodeVsPlayer("EyeBall", 0.2f);
	}
	if (animationTime >= 0.2f && animationTime <= 0.5f)
	{
		owner->GetComponent<EnemySlime>()->SetCounterJudgment(true);
	}


}
// �Ō�ɂ�肽�������S��
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

	//// ���˓��͏���
	//if (owner->GetComponent<Player>()->InputAvoidance())
	//{
	//	owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Reflection));

	//}

	// �W�����v���͏���
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





	// �e�ۓ��͏���
	else if (owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::SpecialAttack::MagicFire)
	{
		owner->GetComponent<Player>()->InputProjectile();
		//TransitionAttackState();
	}

	// ����U��
	if (owner->GetComponent<Player>()->InputSpecialAttackCharge())
	{
		//TransitionAttackState();
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
	}
	// ���ꖂ�@
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

	owner->GetComponent<Player>()->InputSelectCheck();

	// �ʏ�U��
	if (owner->GetComponent<Player>()->InputAttack() && owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::SpecialAttack::Attack)
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
	}



	// �e�ۓ��͏���
	else if (owner->GetComponent<Player>()->GetSelectCheck() == (int)Player::SpecialAttack::MagicFire)
	{
		owner->GetComponent<Player>()->InputProjectile();
	}

	// ����U��
	if (owner->GetComponent<Player>()->InputSpecialAttackCharge())
	{
		//TransitionAttackState();
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
	}
	// ���ꖂ�@
	if (owner->GetComponent<Player>()->InputSpecialShotCharge())
	{
		owner->GetComponent<Player>()->InputProjectile();
	}
	// �������n
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

	// �A�j���[�V�������[��
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Normal);

}

void PlayerJumpState::Execute(float elapsedTime)
{
	// �W�����v���͏���
	if (owner->GetComponent<Player>()->InputJump() && owner->GetComponent<Player>()->GetJumpCount() >= 1)
	{
		//TransitionJumpFlipState();
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::JumpFlip));
	}

	// �e�ۓ��͏���
	if (owner->GetComponent<Player>()->InputProjectile())
	{
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
		//TransitionAttackState();
	}


	// �������n
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

	if (owner->GetComponent<Player>()->InputMove(elapsedTime))
		bool afterimagemove = true;
	// �W�����v���͏���
	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{

		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Jump));
	}

	// �������n
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

	// �A�j���[�V�������[��
	owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Doble);

}

void PlayerAttackState::Execute(float elapsedTime)
{
	if (owner->GetComponent<Player>()->InputAttack())
	{
		button = true;
	}
	// �����I�������ҋ@�ɕύX
	if (owner->GetComponent<Player>()->GetUpdateAnim() == UpAnim::Doble && !owner->GetComponent<ModelControll>()->GetModel()->IsPlayUpeerBodyAnimation())
	{
		owner->GetComponent<Player>()->SetAttackCollisionFlag(false);
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(button ? Player::State::Attack : Player::State::Move));
		button = false;
		owner->GetComponent<Player>()->SetSpecialAttackTime(false);
		//attackCollisionFlag = false;
		//TransitionMoveState();


	}
	// �����Đ�
	else if (owner->GetComponent<Player>()->GetUpdateAnim() == UpAnim::Normal && !owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		owner->GetComponent<Player>()->SetAttackCollisionFlag(false);
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Move));
		owner->GetComponent<Player>()->SetSpecialAttackTime(false);
	}
	// �����ړ����Ɏ�𗣂�����
	if (owner->GetComponent<Player>()->GetUpdateAnim() == UpAnim::Doble && !owner->GetComponent<Player>()->InputMove(elapsedTime))
	{
		//updateanim = UpAnim::Doble;
		owner->GetComponent<Player>()->SetUpdateAnim(UpAnim::Doble);

		bool loop = false;

			owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
					owner->GetComponent<Player>()->Anim_Attack, loop);
	}

	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	float animationTime = owner->GetComponent<ModelControll>()->GetModel()->GetCurrentAnimationSecondsUpeer();
	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B
	bool CollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;

	owner->GetComponent<Player>()->SetAttackCollisionFlag(CollisionFlag);

	//if (updateanim == UpAnim::Doble)
	//{
	//	// �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
	//	float animationTime = model->GetCurrentAnimationSecondsUpeer();
	//	// ��肭�s���ΓG������s��������Ă����s����p�ӏo����B
	//	attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
	//}
	if (owner->GetComponent<Player>()->GetAttackCollisionFlag())
	{
		// ����m�[�h�ƃG�l�~�[�̏Փˏ���
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

// ���������
void PlayerAvoidanceState::Enter()
{
	bool loop = false;

	owner->GetComponent<ModelControll>()->GetModel()->PlayAnimation(
		owner->GetComponent<Player>()->Anim_Jump_Flip, loop);

	// �����蔻��̗L��
	owner->GetComponent<Player>()->DmageInvalidJudment(false);


}

// ����X�V
void PlayerAvoidanceState::Execute(float elapsedTime)
{
	// �ړ����͏���
	owner->GetComponent<Player>()->InputMove(elapsedTime);

	if (!owner->GetComponent<ModelControll>()->GetModel()->IsPlayAnimation())
	{
		// �����蔻��̗L��
		owner->GetComponent<Player>()->DmageInvalidJudment(true);
		owner->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));

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
	bool CollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;

	//owner->GetComponent<Player>()->SetAttackCollisionFlag(CollisionFlag);

	if (CollisionFlag)
	{
		// ����m�[�h�ƃG�l�~�[�̏Փˏ���
		owner->GetComponent<Player>()->CollisionNodeVsEnemiesCounter("mixamorig:LeftHand", owner->GetComponent<Player>()->GetLeftHandRadius());
	}
}

// ���ˏI��
void PlayerReflectionState::Exit()
{
}


