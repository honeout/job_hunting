#pragma once
#include "StateBase.h"
#include "Actor.h"


enum AttackChange
{
	Round1 = 2,
	Round2 = 1,
	ROund3 = 0,
};

// �p�j�X�e�[�g�I�u�W�F�N�g
class WanderState : public State
{
public:
	// �R���X�g���N�^
	WanderState(Actor* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~WanderState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;


	// ���n�u��
	bool                upOnLading = false;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;
};

// �ҋ@�X�e�[�g�I�u�W�F�N�g
class IdleState : public State
{
public:
	// �R���X�g���N�^
	IdleState(Actor* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~IdleState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = true;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;
};

// �ǐՃX�e�[�g�I�u�W�F�N�g
class PursuitState : public State
{
public:
	// �R���X�g���N�^
	PursuitState(Actor* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~PursuitState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;
	float				attackRange = 0;

	int                 attackRound;
	int                 attackCountMax;

	int                 attackType = 0;

	// �Đ����[�v
	bool  loop = true;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;
};

// �U���X�e�[�g�I�u�W�F�N�g
class AttackState : public State
{
public:
	// �R���X�g���N�^
	AttackState(Actor* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~AttackState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	// �U����
	int					attackMemory = 0;
	int					attackMemoryMax = 3;
	
	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;

};

// �ˌ��X�e�[�g�I�u�W�F�N�g
class AttackShotState : public State
{
public:
	// �R���X�g���N�^
	AttackShotState(Actor* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~AttackShotState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	int                 attackCount = 0;
	int                 attackCountMax = 0;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;
};
// �ˌ��ʃX�e�[�g�I�u�W�F�N�g
class AttackShotThrowingState : public State
{
public:
	// �R���X�g���N�^
	AttackShotThrowingState(Actor* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~AttackShotThrowingState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	int                 attackCount = 0;
	int                 attackCountMax = 0;
	bool				turnPermission = false;

	float               turnSpeed = 20.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSecondsf;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;
};

// �_���[�W�X�e�[�g�I�u�W�F�N�g
class DamageState : public State
{
public:
	// �R���X�g���N�^
	DamageState(Actor* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~DamageState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;


	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;
};

// �����X�e�[�g�I�u�W�F�N�g
class ConfusionState : public State
{
public:
	// �R���X�g���N�^
	ConfusionState(Actor* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~ConfusionState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 1.0f;
};





//////////////////////////////
// �v���C���[
//////////////////////////////


// �ҋ@�X�e�[�g�I�u�W�F�N�g
class PlayerIdleState : public State
{
public:
	// �R���X�g���N�^
	PlayerIdleState(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerIdleState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = true;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 1.0f;
};

// �p�j�X�e�[�g�I�u�W�F�N�g
class PlayerMovestate : public State
{
public:
	// �R���X�g���N�^
	PlayerMovestate(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerMovestate() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:

	// �Đ����[�v
	bool  loop = true;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 1.0f;
};


// �W�����v�X�e�[�g�I�u�W�F�N�g
class PlayerJumpState : public State
{
public:
	// �R���X�g���N�^
	PlayerJumpState(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerJumpState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;
	float				attackRange = 1.5f;

	bool                jumpCheck = false;
	//bool				loop = false;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 1.0f;
};

// ���n�X�e�[�g�I�u�W�F�N�g
class PlayerLandState : public State
{
public:
	// �R���X�g���N�^
	PlayerLandState(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerLandState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 1.0f;
};

// �W�����v���X�e�[�g�I�u�W�F�N�g
class PlayerJumpFlipState : public State
{
public:
	// �R���X�g���N�^
	PlayerJumpFlipState(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerJumpFlipState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 1.0f;
};


// �U���X�e�[�g�I�u�W�F�N�g
class PlayerAttackState : public State
{
public:
	// �R���X�g���N�^
	PlayerAttackState(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerAttackState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;
	bool				button = false;

	float				turnSpeed = 10;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 1.0f;
};

// ��낯�X�e�[�g�I�u�W�F�N�g
class PlayerDamageState : public State
{
public:
	// �R���X�g���N�^
	PlayerDamageState(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerDamageState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 1.0f;
};

// ����X�e�[�g�I�u�W�F�N�g
class PlayerAvoidanceState : public State
{
public:
	// �R���X�g���N�^
	PlayerAvoidanceState(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerAvoidanceState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	float               moveSpeed = 2.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.3f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 1.0f;
};

// ���˃X�e�[�g�I�u�W�F�N�g
class PlayerReflectionState : public State
{
public:
	// �R���X�g���N�^
	PlayerReflectionState(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerReflectionState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 1.0f;
};

// ���S�X�e�[�g�I�u�W�F�N�g
class PlayerDeathState : public State
{
public:
	// �R���X�g���N�^
	PlayerDeathState(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerDeathState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 1.0f;
};

// �����X�e�[�g�I�u�W�F�N�g
class PlayerReviveState : public State
{
public:
	// �R���X�g���N�^
	PlayerReviveState(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerReviveState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 1.0f;
};