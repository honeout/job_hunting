#pragma once
#include "StateBase.h"
#include "Input\GamePad.h"
#include "Input/Input.h"
#include "SceneGameClear.h"
#include "Actor.h"
#include "Effect.h"
#include "ProjectileTornade.h"
//#include "Audio/AudioSource.h"


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
	WanderState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// �f�X�g���N�^
	~WanderState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;
	////std::shared_ptr<ModelControll> modelid;
	//std::shared_ptr<Movement> moveid = nullptr;
	////std::shared_ptr<Actor> playerid;

	//Model* model = nullptr;
	
	//float				stateTimerEnd = 0.0f;
	//float				stateTimer = stateTimerEnd;
	//float				stateTimerMax = 4.0f;

	// �o�����ɍU���񐔂�ς���B
	int                 attackCountMax = 2;
	int                 attackCountMin = 0;
	int                 attackCount = attackCountMin;

	// �G�t�F�N�g�X�P�[��
	float               scaleEffect = 0.3f;

	// �G�t�F�N�g
	std::unique_ptr<Effect> smorker;

	// ���sSE
	//std::unique_ptr<AudioSeSource> walkSe;
	bool loopSe = true;

	// ���n�u��
	bool                upOnLading = false;


	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 3.3f;

	// �Đ��I������
	float currentAnimationEndSeconds = 300.0f;

	// �Đ����ԉ��Z���̒l
	float currentAnimationAddSeconds = 0.00f;
	//float currentAnimationAddSeconds = 0.06f;

	// �L�[�t���[���̏I��
	float keyFrameEnd = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.7f;

	// �������x
	float moveSpeed = 3;

	// ��]���x
	float turnSpeed = DirectX::XMConvertToRadians(320);

	// �^�[�Q�b�g�ʒu
	DirectX::XMFLOAT3 targetPosition = {.0f,.0f,.0f};


};

// �ҋ@�X�e�[�g�I�u�W�F�N�g
class IdleState : public State
{
public:
	// �R���X�g���N�^
	IdleState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// �f�X�g���N�^
	~IdleState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;
	//std::shared_ptr<ModelControll> model;
	//Model*  model = nullptr;
	//std::shared_ptr<Movement> move = nullptr;
	//std::shared_ptr<HP> hp = nullptr;
	
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
	PursuitState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// �f�X�g���N�^
	~PursuitState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	//std::shared_ptr<Actor> playerid;
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;
	//std::shared_ptr<HP> hpid = nullptr;
	

	float				stateTimer = 0.0f;
	float				stateTimerMin = 0.0f;
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


	// �s�������ő吔
	int mortionLimitMax = 5;
	// ����s����������郂�[�V����������B
	int mortionLimit = mortionLimitMax;

	// �s���������_����
	int randamAttack = 0;
};


// �W�����v�n�X�e�[�g�I�u�W�F�N�g
class JumpState : public State
{
public:
	// �R���X�g���N�^
	JumpState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// �f�X�g���N�^
	~JumpState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;
	////std::shared_ptr<ModelControll> modelid;
	//std::shared_ptr<Movement> moveid = nullptr;
	//std::shared_ptr<HP> hpid = nullptr;
	////std::shared_ptr<Actor> playerid;

	////float				stateTimer = 0.0f;

	//Model* model = nullptr;

	//// �W�����v
	//std::unique_ptr<AudioSeSource> janpSe;

	//// ���n
	//std::unique_ptr<AudioSeSource> landSe;


	//// �o�����̍U����
	//int                 attackCountMax = 2;
	//int                 attackCountMin = 0;
	//int                 attackCount = attackCountMin;

	// �G�t�F�N�g�X�P�[��
	float               scaleEffect = 0.3f;

	// �G�t�F�N�g
	std::unique_ptr<Effect> smorker;

	// ���[�v���邩Se
	bool loopSe = false;

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
	DirectX::XMFLOAT3 targetPosition = { .0f,.0f,.0f };

	// �A�j���[�V�����I��
	bool jumpStart = false;
};


// �U���X�e�[�g�I�u�W�F�N�g
class AttackState : public State
{
public:
	// �R���X�g���N�^
	AttackState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// �f�X�g���N�^
	~AttackState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;

	//std::shared_ptr<HP> hpid = nullptr;

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
	AttackShotState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// �f�X�g���N�^
	~AttackShotState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;

	//std::shared_ptr<HP> hpid = nullptr;
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
	AttackShotThrowingState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// �f�X�g���N�^
	~AttackShotThrowingState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;

	//std::shared_ptr<Transform> transformid = nullptr;


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
	DamageState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// �f�X�g���N�^
	~DamageState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;

	//std::shared_ptr<Transform> transformid;

	// ��_���[�W
	//std::unique_ptr<AudioSource> damageSe;
	bool loopSe = false;

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
	ConfusionState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// �f�X�g���N�^
	~ConfusionState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;

	// ����
	//std::unique_ptr<AudioSource> confusionSe;
	bool loopSe = false;

	float				stateTimer = 0.0f;
	float				stateTimerMax = 0.8f;

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

	// �U�����
		// �s���������_����
	int randamAttack = 0;
};



// ���S�X�e�[�g�I�u�W�F�N�g
class DeathState : public State
{
public:
	// �R���X�g���N�^
	DeathState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// �f�X�g���N�^
	~DeathState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;

	//std::shared_ptr<Transform> transformid = nullptr;

	// ���S����
	//std::unique_ptr<AudioSource> deathSe;
	//std::unique_ptr<AudioSeSource> deathSe;
	bool loopSe = false;

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


// �N���A�X�e�[�g�I�u�W�F�N�g
class ClearState : public State
{
public:
	// �R���X�g���N�^
	ClearState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// �f�X�g���N�^
	~ClearState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	//std::shared_ptr<EnemyBoss> enemyid = nullptr;

	//std::shared_ptr<Transform> transformid = nullptr;

	// ���S����
	//std::unique_ptr<AudioSource> deathSe;
	//std::unique_ptr<AudioSeSource> deathSe;
	bool loopSe = false;

	float				stateTimer = 0.0f;


	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.7f;

	// �N���A�m�F
	bool clearCheck = true;

	// �J��������
	float distance = 3.0f;
};

//////////////////////////////
// �v���C���[
//////////////////////////////


// �ҋ@�X�e�[�g�I�u�W�F�N�g
class PlayerIdleState : public State
{
public:
	// �R���X�g���N�^
	PlayerIdleState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerIdleState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	//std::shared_ptr<Actor> playerid;
	//std::shared_ptr<Movement> moveid;

	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = true;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;
};

// �ړ��X�e�[�g�I�u�W�F�N�g
class PlayerMovestate : public State
{
public:
	// �R���X�g���N�^
	PlayerMovestate(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerMovestate() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:

	// ���s
	//std::unique_ptr<AudioSource> walkSe;
	bool loopSe = true;

	//std::shared_ptr<Movement> moveid;


	// �Đ����[�v
	bool  loop = true;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;

	// �Đ����ԉ��Z���̒l
	float currentAnimationAddSeconds = 0.05f;
};


// �W�����v�X�e�[�g�I�u�W�F�N�g
class PlayerJumpState : public State
{
public:
	// �R���X�g���N�^
	PlayerJumpState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerJumpState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	//std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;

	// �W�����v
	//std::unique_ptr<AudioSource> janpSe;
	bool loopSe = false;

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
	PlayerLandState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerLandState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	//std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;

	// ���n
	//std::unique_ptr<AudioSource> landSe;

	bool loopSe = false;

	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.35f;

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
	PlayerJumpFlipState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerJumpFlipState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	//std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;


	// �W�����v
	//std::unique_ptr<AudioSource> janpSe;
	bool loopSe = false;

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
	PlayerQuickJabState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerQuickJabState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	//std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;
	//std::shared_ptr<Transform> transformid;

	// �����Ƃɗ���
	//std::unique_ptr<Effect> areWork;

	// �a��
	//std::unique_ptr<AudioSource> slashSe;
	bool loopSe = false;
	

	//DirectX::XMFLOAT3 angle; 

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
	float attackCheckRange = 4.5f;

	float attackCheckRangeMin = 2.6f;

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
	PlayerSideCutState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerSideCutState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	//std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;
	//std::shared_ptr<Transform> transformid;

	// �a��
	//std::unique_ptr<AudioSource> slashSe;
	bool loopSe = false;

	//DirectX::XMFLOAT3 angle;

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
	PlayerCycloneStrikeState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerCycloneStrikeState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	//std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;
	//std::shared_ptr<Transform> transformid;

	// �a��
	//std::unique_ptr<AudioSource> slashSe;
	bool loopSe = false;

	//DirectX::XMFLOAT3 angle;

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
	PlayerSpecialAttackState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerSpecialAttackState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	//std::shared_ptr<Transform> transformid;
	////std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;

	//// �G�l�~�[���ꕨ
	//std::shared_ptr<Transform> enemyTransform;
	//std::shared_ptr<HP> enemyHpId;

	//// �a��
	//std::unique_ptr<AudioSource> slashSe;

	//// �K�E�Z��
	//std::unique_ptr<AudioSource> lighteningStrikeSpecialSe;
	//// �K�E�Z������
	//std::unique_ptr<AudioSource> lighteningStrikeSpecialSaveSe;
	bool loopSe = false;

	DirectX::XMFLOAT3 position = {0.0f,0.0f,0.0f};
	DirectX::XMFLOAT3 angle = { 0.0f,0.0f,0.0f };

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

	// ��]�m�F
	bool isRotate = false;

	// ����
	DirectX::XMFLOAT3 vector = { 0.0f,0.0f,0.0f };
	// ����
	float length = 0.0f;

	// �����̍ő�l
	float attackCheckRangeMax = 10.5f;
	// �����̍Œ�l
	float attackCheckRangeMin = 2.6f;

	// �ő�̉�]
	DirectX::XMFLOAT2 angleRange = { 0.9f,0.9f };

};

// ���@�X�e�[�g�I�u�W�F�N�g
class PlayerMagicState : public State
{
public:
	// �R���X�g���N�^
	PlayerMagicState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerMagicState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;

private:
	//std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;


	//// ������
	//std::unique_ptr<AudioSource> flameStarteSe;
	//// ������
	////std::unique_ptr<AudioSource> flameDurationSe;
	//// �����e��
	//std::unique_ptr<AudioSource> flameimpactSe;


	//// �X����
	//std::unique_ptr<AudioSource> iceStarteSe;
	//// �X����
	//std::unique_ptr<AudioSource> iceDurationSe;
	//// �X���e��
	//std::unique_ptr<AudioSource> iceimpactSe;

	//// ���q�b�g��
	//std::unique_ptr<AudioSource> lightningSe;

	// se
	bool loopSe = false;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;

	// ���@���ˎ��
	int                       magicType = 0;

	// ��]���x
	float				turnSpeed = DirectX::XMConvertToRadians(720);

	// �������x
	float moveSpeed = 5.0f;

	// ��]����
	bool                      isRotate = false;

	// ����
	DirectX::XMFLOAT3 vector = { 0.0f,0.0f,0.0f };
	// ����
    float length = 0.0f;

	// �p�x�͈�
	DirectX::XMFLOAT2 angleRange = { 0.9f,0.9f };

	// �ړ�����
	bool isMove = false;


};


// �ߐڕK�E�Z�X�e�[�g�I�u�W�F�N�g
class PlayerSpecialMagicState : public State
{
public:
	// �R���X�g���N�^
	PlayerSpecialMagicState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerSpecialMagicState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	//std::shared_ptr<Transform> transformid;
	////std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;

	// �G�l�~�[���ꕨ
	//std::shared_ptr<Transform> enemyTransform;
	//std::shared_ptr<HP> enemyHpId;

	// ������
	//std::unique_ptr<AudioSource> flameDurationSe;

	// �K�E�Z��
	//std::unique_ptr<AudioSource> flameSpecialStarteSe;
	// �K�E�Z������
	//std::unique_ptr<AudioSource> flameSpecialSaveSe;
	bool loopSe = false;


	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 angle = { 0.0f,0.0f,0.0f };

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


	// ��]�m�F
	bool isRotate = false;

	// ����
	DirectX::XMFLOAT3 vector = { 0.0f,0.0f,0.0f };
	// ����
	float length = 0.0f;

	// �����̍ő�l
	float attackCheckRangeMax = 10.5f;
	// �����̍Œ�l
	float attackCheckRangeMin = 2.6f;

	// �ő�̉�]
	DirectX::XMFLOAT2 angleRange = { 0.9f,0.9f };

};


// �K�E�Z�X���@
class PlayerSpecialMagicIceState : public State
{
public:
	// �R���X�g���N�^
	PlayerSpecialMagicIceState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerSpecialMagicIceState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	//std::shared_ptr<Transform> transformid;
	//std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;

	// �G�l�~�[���ꕨ
	//std::shared_ptr<Transform> enemyTransform;
	//std::shared_ptr<HP> enemyHpId;

	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 angle = { 0.0f,0.0f,0.0f };

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

	// �L�[�t���[���I��
	float keyFrameEnd = 64.00f;


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
	PlayerSpecialThanderMagicState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerSpecialThanderMagicState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	//std::shared_ptr<Transform> transformid;
	////std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<Movement> moveid;

	//// �G�l�~�[���ꕨ
	//std::shared_ptr<Transform> enemyTransform;
	//std::shared_ptr<HP> enemyHpId;

	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 angle = { 0.0f,0.0f,0.0f };

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
	PlayerDamageState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerDamageState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	//std::shared_ptr<ModelControll> model;
	//std::shared_ptr<Movement> moveid;
	//Model* model;

	// ��_���[�W
	//std::unique_ptr<AudioSource> damageSe;
	bool loopSe = false;

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
	PlayerAvoidanceState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerAvoidanceState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	//std::shared_ptr<Transform> transformid;
	//std::shared_ptr<Movement> moveid;
	//std::shared_ptr<HP> hpid;

	// �_�b�V��
	//std::unique_ptr<AudioSource> dushSe;
	bool loopSe = false;

	// �G�t�F�N�g
	std::unique_ptr<Effect> wind;


	float				stateTimer = 0.0f;

	float               moveSpeed = 10.0f;
	float               speed = 4.5f;
	float               flySpeed = 3.0f;


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
	PlayerReflectionState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerReflectionState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	

	// ����
	//std::unique_ptr<AudioSource> reflectionStop;
	bool loopSe = false;

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
	PlayerDeathState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerDeathState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	//std::shared_ptr<ModelControll> model;
	//Model* model;
	//std::shared_ptr<HP> hpid;
	//std::shared_ptr<Transform> transformid;
	//float				stateTimer = 0.0f;

	// ���S�����G
	//std::unique_ptr<AudioSource> deathSe;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;

	// �o�ߎ��ԍő�
	float stateTimer = 0.0f;
	float stateTimerMax = 40;
	//float stateTimerMax = 240;
};

// �����X�e�[�g�I�u�W�F�N�g
class PlayerReviveState : public State
{
public:
	// �R���X�g���N�^
	PlayerReviveState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerReviveState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;
};


// �v���C���[�^�C�g���p�ҋ@�X�e�[�g�I�u�W�F�N�g
class PlayerTitleIdleState : public State
{
public:
	// �R���X�g���N�^
	PlayerTitleIdleState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerTitleIdleState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = true;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;
};



// �v���C���[�^�C�g���p����X�e�[�g�I�u�W�F�N�g
class PlayerTitlePushState : public State
{
public:
	// �R���X�g���N�^
	PlayerTitlePushState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerTitlePushState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;

	// �߂�����
	bool secondeMortion = false;

	// �G�t�F�N�g
	std::unique_ptr<Effect> lightning;
	std::unique_ptr<Effect> lightningAttack;
	std::unique_ptr<Effect> lightningHit;
};



// �v���C���[�N���A�p�ҋ@�X�e�[�g�I�u�W�F�N�g
class PlayerClearIdleState : public State
{
public:
	// �R���X�g���N�^
	PlayerClearIdleState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerClearIdleState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = true;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 0.0f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;
};



// �v���C���[�N���A�p�ҋ@�X�e�[�g�I�u�W�F�N�g
class PlayerOverIdleState : public State
{
public:
	// �R���X�g���N�^
	PlayerOverIdleState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerOverIdleState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

	// �I������
	void End()override;
private:
	float				stateTimer = 0.0f;

	// �Đ����[�v
	bool  loop = false;

	// �Đ��J�n���� 
	float currentAnimationStartSeconds = 1.7f;

	// �A�j���[�V�����u�����h
	float blendSeconds = 0.5f;

	// �Đ����ԉ��Z���̒l
	float currentAnimationAddSeconds = 0.00f;

	// �L�[�t���[���I��
	float keyFrameEnd = 105.00f;
};

