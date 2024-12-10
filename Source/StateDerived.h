#pragma once
#include "StateBase.h"
#include "Input\GamePad.h"
#include "Input/Input.h"
#include "SceneGameClear.h"
#include "Actor.h"
#include "Effect.h"
#include "ProjectileTornade.h"


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
	
	float				stateTimerEnd = 0.0f;
	float				stateTimer = stateTimerEnd;
	float				stateTimerMax = 4.0f;


	// ���n�u��
	bool                upOnLading = false;


	// �Đ����[�v
	bool  loop = true;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �Đ����ԉ��Z���̒l
	float currentAnimationAddSeconds = 0.06f;

	// �L�[�t���[���̏I��
	float keyFrameEnd = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.7f;

	// �������x
	float moveSpeed = 3;

	// ��]���x
	float turnSpeed = DirectX::XMConvertToRadians(320);

	// �^�[�Q�b�g�ʒu
	DirectX::XMFLOAT3 targetPosition;
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
	std::shared_ptr<ModelControll> modelControllid;
	std::shared_ptr<Movement> move;
	std::shared_ptr<HP> hp;
	
	float               stateTimerEnd = 0.0f;
	float				stateTimer = 0.0f;
	float				stateTimerMax = 3.0f;
	float				stateTimerMin = 1.0f;

	// �Đ����[�v
	bool  loop = true;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.7f;
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
	float blendSeconds = 0.7f;

	// ����s����������郂�[�V����������B
	int mortionLimit = 0;
	// �s�������ő吔
	int mortionLimitMax = 5;

	// �s���������_����
	int randamAttack = 0;
};


// �p�j�X�e�[�g�I�u�W�F�N�g
class JumpState : public State
{
public:
	// �R���X�g���N�^
	JumpState(Actor* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~JumpState() {}
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
	std::shared_ptr<HP> hpid;
	//std::shared_ptr<Actor> playerid;

	//float				stateTimer = 0.0f;



	// ���n�u��
	bool                upOnLading = false;


	// �Đ����[�v
	bool  loop = true;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 2.0f;

	// �Đ����ԉ��Z���̒l
	float currentAnimationAddSeconds = 0.025f;

	// �L�[�t���[���̏I��
	float keyFrameEnd = 0.8f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.7f;

	// �������x
	float moveSpeed = 10;

	// �W�����v���x
	float jumpSpeed = 30.0f;

	// ��]���x
	float turnSpeed = DirectX::XMConvertToRadians(320);

	// �^�[�Q�b�g�ʒu
	DirectX::XMFLOAT3 targetPosition;

	// �A�j���[�V�����I��
	bool jumpStart = false;
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
	float blendSeconds = 0.7f;

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
	float blendSeconds = 0.7f;
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
	float blendSeconds = 0.7f;
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
	float				stateTimerMax = 5.0f;
	float				stateTimerEnd = 0.0f;


	//// �Đ����[�v
	//bool  loop = false;

	//// �Đ��J�n���� 
	//float currentAnimationStartSeconds = 0.0f;

	//// �A�j���[�V�����u�����h
	//float blendSeconds = 0.7f;

	// �Đ����[�v
	bool  loop = true;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 2.5f;

	// �Đ����ԉ��Z���̒l
	float currentAnimationAddSeconds = 0.025f;

	// �L�[�t���[���̏I��
	float keyFrameEnd = 3.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.7f;
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
	float				stateTimerMax = 5.0f;

	float				stateTimerEnd = 0.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.426f;

	// �Đ����ԉ��Z���̒l
	float currentAnimationAddSeconds = 0.00f;

	// �L�[�t���[���̏I��
	float keyFrameEnd = 70.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.35f;
};



// ���S�X�e�[�g�I�u�W�F�N�g
class DeathState : public State
{
public:
	// �R���X�g���N�^
	DeathState(Actor* enemy) :State(enemy) {};
	// �f�X�g���N�^
	~DeathState() {}
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


	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.7f;

	// �N���A�m�F
	bool clearCheck = true;
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

	// �Đ����ԉ��Z���̒l
	float currentAnimationAddSeconds = 0.00f;

	// �L�[�t���[���̏I��
	float keyFrameEnd = 40.0f;


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
	float blendSeconds = 0.3f;

	// ��]�X�s�[�h
	float turnSpeed = DirectX::XMConvertToRadians(720);


};


// �U���X�e�[�g�I�u�W�F�N�g
class PlayerQuickJabState : public State
{
public:
	// �R���X�g���N�^
	PlayerQuickJabState(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerQuickJabState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	std::shared_ptr<ModelControll> modelControllid;
	std::shared_ptr<Movement> moveid;
	std::shared_ptr<Transform> transformid;

	// �����Ƃɗ���
	//std::unique_ptr<Effect> areWork;
	

	DirectX::XMFLOAT3 angle; 

	float				stateTimer = 0.0f;
	bool				button = false;
	bool				buttonSeconde = false;

	float				turnSpeed = DirectX::XMConvertToRadians(720);

	float               speed = 10;


	float               attackSpeed = 3.0f;

	// �ŏ��̈�񂾂��Ăт�������
	bool                InitializationCheck = false;

	// �Đ����[�v
	bool  loop = false;



	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;

	// ��]����
	bool rotateCheck = false;

	// ����
	float length = 0;

	DirectX::XMFLOAT3 vector = {0,0,0};


	// �U���T�|�[�g�͈�
	float attackCheckRange = 9;

	float attackCheckRangeMin = 7;

	float gravity = -0.2f;

	// �����x
	DirectX::XMFLOAT3 velocity = { 0,0,0 };

	// �R�}���h�m�F
	std::vector<GamePadButton> commandSeconde;
	std::vector<GamePadButton> commandThrede;
	
	// �_���[�W�H��������ɋ����I��
	bool deleteCheck = false;

	// �R��ڂ̍U���ŋ����I��
	int attackMemory = 0;
	int attackMemoryMax = 3;

	// �R�}���h����̋L�^�m�F����
	int frame = 150;

	// �p�x�͈�
	DirectX::XMFLOAT2 angleRange = { 0.9f,0.9f };

	bool              oneAttackCheck = false;
};



// �U���X�e�[�g�I�u�W�F�N�g
class PlayerSideCutState : public State
{
public:
	// �R���X�g���N�^
	PlayerSideCutState(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerSideCutState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	std::shared_ptr<ModelControll> modelControllid;
	std::shared_ptr<Movement> moveid;
	std::shared_ptr<Transform> transformid;

	DirectX::XMFLOAT3 angle;

	float				stateTimer = 0.0f;
	bool				button = false;
	bool				buttonSeconde = false;

	float				turnSpeed = DirectX::XMConvertToRadians(720);

	float               speed = 10;

	float               attackSpeed = 3.0f;

	// �ŏ��̈�񂾂��Ăт�������
	bool                InitializationCheck = false;

	//// �Đ����[�v
	//bool  loop = false;

	//// �Đ��J�n���� 
	//float currentAnimationStartSeconds = 0.0f;
	//// �Đ����ԉ��Z���̒l
	//float currentAnimationAddSeconds = 0.03f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;

	// ��]����
	bool rotateCheck = false;

	// ����
	float length = 0;

	DirectX::XMFLOAT3 vector = { 0,0,0 };


	// �U���T�|�[�g�͈�
	float attackCheckRange = 9;

	float attackCheckRangeMin = 7;

	float gravity = -0.2f;

	// �����x
	DirectX::XMFLOAT3 velocity = { 0,0,0 };

	// �R�}���h�m�F
	std::vector<GamePadButton> commandSeconde;
	std::vector<GamePadButton> commandThrede;

	// �_���[�W�H��������ɋ����I��
	bool deleteCheck = false;

	// �R��ڂ̍U���ŋ����I��
	int attackMemory = 0;
	int attackMemoryMax = 3;

	// �R�}���h����̋L�^�m�F����
	int frame = 150;

	// �p�x�͈�
	DirectX::XMFLOAT2 angleRange = { 0.9f,0.9f };

	bool              oneAttackCheck = false;
};



// �U���X�e�[�g�I�u�W�F�N�g
class PlayerCycloneStrikeState : public State
{
public:
	// �R���X�g���N�^
	PlayerCycloneStrikeState(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerCycloneStrikeState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	std::shared_ptr<ModelControll> modelControllid;
	std::shared_ptr<Movement> moveid;
	std::shared_ptr<Transform> transformid;

	DirectX::XMFLOAT3 angle;

	float				stateTimer = 0.0f;
	bool				button = false;
	bool				buttonSeconde = false;

	float				turnSpeed = DirectX::XMConvertToRadians(720);

	float               speed = 10;


	float               attackSpeed = 3.0f;

	// �ŏ��̈�񂾂��Ăт�������
	bool                InitializationCheck = false;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;
	// �Đ����ԉ��Z���̒l
	float currentAnimationAddSeconds = 0.03f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;

	// ��]����
	bool rotateCheck = false;

	// ����
	float length = 0;

	DirectX::XMFLOAT3 vector = { 0,0,0 };

	// �U���T�|�[�g�͈�
	float attackCheckRange = 9;

	float attackCheckRangeMin = 7;

	float gravity = -0.2f;

	// �����x
	DirectX::XMFLOAT3 velocity = { 0,0,0 };

	// �R�}���h�m�F
	std::vector<GamePadButton> commandSeconde;
	std::vector<GamePadButton> commandThrede;

	// �_���[�W�H��������ɋ����I��
	bool deleteCheck = false;

	// �R��ڂ̍U���ŋ����I��
	int attackMemory = 0;
	int attackMemoryMax = 3;

	// �R�}���h����̋L�^�m�F����
	int frame = 150;

	// �p�x�͈�
	DirectX::XMFLOAT2 angleRange = { 0.9f,0.9f };

	bool              oneAttackCheck = false;
};


// �ߐڕK�E�Z�X�e�[�g�I�u�W�F�N�g
class PlayerSpecialAttackState : public State
{
public:
	// �R���X�g���N�^
	PlayerSpecialAttackState(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerSpecialAttackState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	std::shared_ptr<Transform> transformid;
	std::shared_ptr<ModelControll> modelControllid;
	std::shared_ptr<Movement> moveid;

	// �G�l�~�[���ꕨ
	std::shared_ptr<Transform> enemyTransform;
	std::shared_ptr<HP> enemyHpId;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 angle;

	float				stateTimer = 0.0f;

	std::unique_ptr<Effect> lightning;
	std::unique_ptr<Effect> lightningAttack;
	std::unique_ptr<Effect> lightningHit;

	//// �Đ����[�v
	//bool  loop = false;

	//// �Đ��J�n���� 
	//float currentAnimationStartSeconds = 0.0f;
	//// �Đ����ԉ��Z���̒l
	//float currentAnimationAddSeconds = 0.00f;

	//// �A�j���[�V�����u�����h
	//float blendSeconds = 0.5f;


	bool button = false;

	// ���b�N�I�����[�h
	bool rockCheck = true;


	float				turnSpeed = DirectX::XMConvertToRadians(720);

	// ��u����
	bool flashOn = true;
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


// �ߐڕK�E�Z�X�e�[�g�I�u�W�F�N�g
class PlayerSpecialMagicState : public State
{
public:
	// �R���X�g���N�^
	PlayerSpecialMagicState(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerSpecialMagicState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	std::shared_ptr<Transform> transformid;
	std::shared_ptr<ModelControll> modelControllid;
	std::shared_ptr<Movement> moveid;

	// �G�l�~�[���ꕨ
	std::shared_ptr<Transform> enemyTransform;
	std::shared_ptr<HP> enemyHpId;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 angle;

	float				stateTimer = 0.0f;

	std::unique_ptr<Effect> fire;
	std::unique_ptr<Effect> fireAttack;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;
	// �Đ����ԉ��Z���̒l
	float currentAnimationAddSeconds = 0.00f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;


	bool button = false;

	// ���b�N�I�����[�h
	bool rockCheck = true;


	float				turnSpeed = DirectX::XMConvertToRadians(720);


	// �X�^�[�g�l
	float specialMoveWaitStartTime = 0.0f;

	// ���@�ł�����̑ҋ@����
	float specialMoveWaitTime = specialMoveWaitStartTime;
	float specialMoveWaitTimeMax = 3.0f;

	// �ŏ��������@�𔭓����邽��
	bool startMagic = false;
	bool isMagic = false;

};


// �K�E�Z�X���@
class PlayerSpecialMagicIceState : public State
{
public:
	// �R���X�g���N�^
	PlayerSpecialMagicIceState(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerSpecialMagicIceState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	std::shared_ptr<Transform> transformid;
	std::shared_ptr<ModelControll> modelControllid;
	std::shared_ptr<Movement> moveid;

	// �G�l�~�[���ꕨ
	std::shared_ptr<Transform> enemyTransform;
	std::shared_ptr<HP> enemyHpId;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 angle;

	float				stateTimer = 0.0f;

	std::unique_ptr<Effect> ice;
	//std::unique_ptr<Effect> fireAttack;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;
	// �Đ����ԉ��Z���̒l
	float currentAnimationAddSeconds = 0.00f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;


	bool button = false;

	// ���b�N�I�����[�h
	bool rockCheck = true;


	float				turnSpeed = DirectX::XMConvertToRadians(720);


	// �X�^�[�g�l
	float specialMoveWaitStartTime = 0.0f;

	// ���@�ł�����̑ҋ@����
	float specialMoveWaitTime = specialMoveWaitStartTime;
	float specialMoveWaitTimeMax = 3.0f;

	// �ŏ��������@�𔭓����邽��
	bool startMagic = false;
	bool isMagic = false;

};


// �K�E�Z�����@
class PlayerSpecialThanderMagicState : public State
{
public:
	// �R���X�g���N�^
	PlayerSpecialThanderMagicState(Actor* player) :State(player) {};
	// �f�X�g���N�^
	~PlayerSpecialThanderMagicState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	std::shared_ptr<Transform> transformid;
	std::shared_ptr<ModelControll> modelControllid;
	std::shared_ptr<Movement> moveid;

	// �G�l�~�[���ꕨ
	std::shared_ptr<Transform> enemyTransform;
	std::shared_ptr<HP> enemyHpId;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 angle;

	float				stateTimer = 0.0f;

	std::unique_ptr<Effect> fire;
	std::unique_ptr<Effect> fireAttack;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;
	// �Đ����ԉ��Z���̒l
	float currentAnimationAddSeconds = 0.00f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;


	bool button = false;

	// ���b�N�I�����[�h
	bool rockCheck = true;


	float				turnSpeed = DirectX::XMConvertToRadians(720);


	// �X�^�[�g�l
	float specialMoveWaitStartTime = 0.0f;

	// ���@�ł�����̑ҋ@����
	float specialMoveWaitTime = specialMoveWaitStartTime;
	float specialMoveWaitTimeMax = 3.0f;

	// �ŏ��������@�𔭓����邽��
	bool startMagic = false;
	bool isMagic = false;

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


	// �G�t�F�N�g
	std::unique_ptr<Effect> wind;


	float				stateTimer = 0.0f;

	float               moveSpeed = 8.0f;
	float               speed = 3.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.3f;

	// �Đ����ԉ��Z���̒l
	float currentAnimationAddSeconds = 0.025f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;

	// ���x
	float addHeight = 1.0f;
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