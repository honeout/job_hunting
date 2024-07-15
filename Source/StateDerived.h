#pragma once
#include "StateBase.h"


// �p�j�X�e�[�g�I�u�W�F�N�g
class WanderState : public State
{
public:
	// �R���X�g���N�^
	WanderState(EnemySlime* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~WanderState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};

// �ҋ@�X�e�[�g�I�u�W�F�N�g
class IdleState : public State
{
public:
	// �R���X�g���N�^
	IdleState(EnemySlime* enemy) :State(enemy) {};
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
};

// �ǐՃX�e�[�g�I�u�W�F�N�g
class PursuitState : public State
{
public:
	// �R���X�g���N�^
	PursuitState(EnemySlime* enemy) :State(enemy) {};
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
	float				attackRange = 1.5f;
};

// �U���X�e�[�g�I�u�W�F�N�g
class AttackState : public State
{
public:
	// �R���X�g���N�^
	AttackState(EnemySlime* enemy) :State(enemy) {};
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
};





//////////////////////////////
// �v���C���[
//////////////////////////////


// �ҋ@�X�e�[�g�I�u�W�F�N�g
class PlayerIdleState : public StatePlayer
{
public:
	// �R���X�g���N�^
	PlayerIdleState(Player* player) :StatePlayer(player) {};
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
};

// �p�j�X�e�[�g�I�u�W�F�N�g
class PlayerMovestate : public StatePlayer
{
public:
	// �R���X�g���N�^
	PlayerMovestate(Player* player) :StatePlayer(player) {};
	// �f�X�g���N�^
	~PlayerMovestate() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
};


// �W�����v�X�e�[�g�I�u�W�F�N�g
class PlayerJumpState : public StatePlayer
{
public:
	// �R���X�g���N�^
	PlayerJumpState(Player* player) :StatePlayer(player) {};
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
};

// ���n�X�e�[�g�I�u�W�F�N�g
class PlayerLandState : public StatePlayer
{
public:
	// �R���X�g���N�^
	PlayerLandState(Player* player) :StatePlayer(player) {};
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
};

// �W�����v���X�e�[�g�I�u�W�F�N�g
class PlayerJumpFlipState : public StatePlayer
{
public:
	// �R���X�g���N�^
	PlayerJumpFlipState(Player* player) :StatePlayer(player) {};
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
};


// �U���X�e�[�g�I�u�W�F�N�g
class PlayerAttackState : public StatePlayer
{
public:
	// �R���X�g���N�^
	PlayerAttackState(Player* player) :StatePlayer(player) {};
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
};

// ��낯�X�e�[�g�I�u�W�F�N�g
class PlayerDamageState : public StatePlayer
{
public:
	// �R���X�g���N�^
	PlayerDamageState(Player* player) :StatePlayer(player) {};
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
};

// ���S�X�e�[�g�I�u�W�F�N�g
class PlayerDeathState : public StatePlayer
{
public:
	// �R���X�g���N�^
	PlayerDeathState(Player* player) :StatePlayer(player) {};
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
};

// �����X�e�[�g�I�u�W�F�N�g
class PlayerReviveState : public StatePlayer
{
public:
	// �R���X�g���N�^
	PlayerReviveState(Player* player) :StatePlayer(player) {};
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
};