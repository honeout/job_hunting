#pragma once
#include "StateBase.h"
#include "Input\GamePad.h"
#include "Input/Input.h"
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
	std::shared_ptr<EnemySlime> enemyid;
	std::shared_ptr<ModelControll> modelid;
	std::shared_ptr<Movement> moveid;
	//std::shared_ptr<Actor> playerid;
	
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
	std::shared_ptr<EnemySlime> enemyid;
	//std::shared_ptr<ModelControll> modelControllid;
	
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
	//std::shared_ptr<Actor> playerid;
	std::shared_ptr<EnemySlime> enemyid;
	std::shared_ptr<HP> hpid;
	

	float				stateTimer = 0.0f;
	float				attackRange = 0.0f;

	int                 attackRound = 0;
	int                 attackCountMax = 0;

	int                 attackType = 0;

	// �Đ����[�v
	bool  loop = true;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;

	// ����s����������郂�[�V����������B
	int mortionLimit = 0;
	// �s�������ő吔
	int mortionLimitMax = 5;
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
	std::shared_ptr<EnemySlime> enemyid;

	std::shared_ptr<HP> hpid;

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
	std::shared_ptr<EnemySlime> enemyid;

	std::shared_ptr<HP> hpid;
	//std::shared_ptr<ProjectileRuby> projectileRubyid;

	float				stateTimer = 0.0f;

	int                 attackCount = 0;
	int                 attackCountMax = 0;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

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
	std::shared_ptr<EnemySlime> enemyid;

	std::shared_ptr<Transform> transformid;


	float				stateTimer = 0.0f;

	int                 attackCount = 0;
	int                 attackCountMax = 0;
	bool				turnPermission = false;

	float               turnSpeed = DirectX::XMConvertToRadians(320);

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSecondsf = 0.0f;

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
	std::shared_ptr<EnemySlime> enemyid;

	//std::shared_ptr<Transform> transformid;



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
	std::shared_ptr<EnemySlime> enemyid;



	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;
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
	//std::shared_ptr<Actor> playerid;
	std::shared_ptr<Movement> moveid;

	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = true;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;
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

	std::shared_ptr<Movement> moveid;


	// �Đ����[�v
	bool  loop = true;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;
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
	std::shared_ptr<ModelControll> modelControllid;
	std::shared_ptr<Movement> moveid;

	float				stateTimer = 0.0f;
	float				attackRange = 1.5f;

	bool                jumpCheck = false;
	//bool				loop = false;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;
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
	std::shared_ptr<ModelControll> modelControllid;
	std::shared_ptr<Movement> moveid;

	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.5f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;
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
	std::shared_ptr<ModelControll> modelControllid;
	std::shared_ptr<Movement> moveid;

	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;

	// ��]�X�s�[�h
	float turnSpeed = DirectX::XMConvertToRadians(720);


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
	std::shared_ptr<ModelControll> modelControllid;
	std::shared_ptr<Movement> moveid;
	float				stateTimer = 0.0f;
	bool				button = false;
	bool				buttonSeconde = false;

	float				turnSpeed = DirectX::XMConvertToRadians(90);

	float               speed = 10;

	// �ŏ��̈�񂾂��Ăт�������
	bool                InitializationCheck = false;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;

	// ��]����
	bool rotateCheck = false;

	// ����
	float length = 0;

	DirectX::XMFLOAT3 vector = {0,0,0};

	// �U���T�|�[�g�͈�
	float attackCheckRange = 10;

	float attackCheckRangeMin = 4;

	float gravity = -0.2f;

	// �����x
	DirectX::XMFLOAT3 velocity = { 0,0,0 };

	// �R�}���h�m�F
	std::vector<GamePadButton> commandSeconde;
	std::vector<GamePadButton> commandThrede;
	

	bool deleteCheck = false;
};

// ���@�X�e�[�g�I�u�W�F�N�g
class PlayerMagicState : public State
{
public:
	// �R���X�g���N�^
	PlayerMagicState(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerMagicState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

private:
	std::shared_ptr<ModelControll> modelControllid;
	std::shared_ptr<Movement> moveid;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;

	// ���@���ˎ��
	int                       magicType = 0;

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
	std::shared_ptr<ModelControll> modelControllid;


	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;
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
	std::shared_ptr<Transform> transformid;
	std::shared_ptr<Movement> moveid;
	std::shared_ptr<HP> hpid;


	float				stateTimer = 0.0f;

	float               moveSpeed = 2.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.3f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;
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
	float blendSeconds = 0.5f;
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
	std::shared_ptr<ModelControll> modelControllid;
	std::shared_ptr<HP> hpid;
	std::shared_ptr<Transform> transformid;
	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;

	// �o�ߎ��ԍő�
	float stateTimerMax = 40;
	//float stateTimerMax = 240;
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
	float blendSeconds = 0.5f;
};