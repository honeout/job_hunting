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
	WanderState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// �f�X�g���N�^
	~WanderState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	// �o�����ɍU���񐔂�ς���B
	int                 attackCountMax = 2;
	int                 attackCountMin = 0;
	int                 attackCount = attackCountMin;
	int                 applyDamage = 8;
	// �G�t�F�N�g�X�P�[��
	float               scaleEffect = 0.3f;
	// �G�t�F�N�g
	std::unique_ptr<Effect> smorker;
	bool loopSe = true;
	// ���n�u��
	bool                upOnLading = false;
	// �������x
	float moveSpeed = 3;
	// ��]���x
	float turnSpeed = DirectX::XMConvertToRadians(320);
	// �^�[�Q�b�g�ʒu
	DirectX::XMFLOAT3 targetPosition = {.0f,.0f,.0f};
	RadialBlurData radialBlurData;
	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
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

private:
	float               stateTimerEnd = 0.0f;
	float				stateTimer = 0.0f;
	float				stateTimerMax = 2.5f;
	float				stateTimerMin = 1.0f;
	// �s�����
	int  randRoot = 0;
	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
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

private:
	float				stateTimer = 0.0f;
	float				stateTimerMin = 0.0f;
	float				attackRange = 0.0f;
	int                 attackRound = 0;
	int                 attackCountMax = 0;
	int                 attackType = 0;
	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
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

private:
	// �^�_��
	int                 applyDamageJumpStart = 5;
	int                 applyDamageJumpStamp = 8;

	// �G�t�F�N�g�X�P�[��
	float               scaleEffect = 0.3f;

	// �G�t�F�N�g
	std::unique_ptr<Effect> smorker;

	// ���[�v���邩Se
	bool loopSe = false;

	// ���n�u��
	bool                upOnLading = false;

	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;

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

	RadialBlurData radialBlurData;
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

private:
	DirectX::XMFLOAT3 angle;

	// �`���[�W����
	float				stateChargeTimer = 0.0f;
	float				stateChargeTimerEnd = 0.0f;
	float				stateChargeTimerMax = 1.0f;

	// �`���[�W������̃_�b�V������
	float				stateChargeCompletionTimer = 0.0f;
	float				stateChargeCompletionTimerEnd = 0.0f;
	float				stateChargeCompletionTimerMax = 1.0f;

	// �G�t�F�N�g�X�P�[��
	float               scaleEffect = 0.3f;

	// �^�_��
	int                 applyDamageAttack = 8;

	// �U����
	int					attackMemory = 0;
	int					attackMemoryMax = 3;

	// �G�t�F�N�g
	std::unique_ptr<Effect> smorker;
	std::unique_ptr<Effect> charge;
	std::unique_ptr<Effect> chargeCompleate;
	
	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;

	// �������x
	float moveSpeed = 10;

	// �^�[�Q�b�g�ʒu
	DirectX::XMFLOAT3 targetPosition = { .0f,.0f,.0f };

	// ��]���x
	float turnSpeed = DirectX::XMConvertToRadians(320);

	// �`���[�W������
	bool chargeInitilize = false;

	// �A�j���[�V�����I��
	bool dushStart = false;

	DirectX::XMFLOAT3 direction;

	DirectX::XMFLOAT3 directionP;
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
private:
	bool loopSe = false;
	float				stateTimer = 0.0f;
	float				stateTimerMax = 5.0f;
	float				stateTimerEnd = 0.0f;
	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
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
private:
	bool loopSe = false;
	float				stateTimer = 0.0f;
	float				stateTimerMax = 2.8f;
	float				stateTimerEnd = 0.0f;
	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
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
private:
	bool loopSe = false;
	float				stateTimer = 0.0f;
	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
	// �N���A�m�F
	bool clearCheck = true;
};

// �o���X�e�[�g�I�u�W�F�N�g
class AwakeStartState : public State
{
public:
	// �R���X�g���N�^
	AwakeStartState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// �f�X�g���N�^
	~AwakeStartState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
	// �N���A�m�F
	bool clearCheck = true;
	// �J��������
	float distance = 3.0f;
	// �J�����̗h��̐[��
	float cameraShakePower = 0.1f;
	// �J�����̗h�ꎞ��
	float cameraShakeTime = 1.0f;
	// �A�j���[�V�����̎���
	float animationPose = 6.70f;
	RadialBlurData radialBlurData;
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
private:
	bool loopSe = false;
	float				stateTimer = 0.0f;
	// �N���A�m�F
	bool clearCheck = true;
	// �J��������
	float distance = 3.0f;
};

// �N���A�X�e�[�g�I�u�W�F�N�g
class ClearReviveState : public State
{
public:
	// �R���X�g���N�^
	ClearReviveState(std::weak_ptr<Actor> enemy) :State(enemy) {};
	// �f�X�g���N�^
	~ClearReviveState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	bool loopSe = false;
	float				stateTimer = 0.0f;
	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
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
private:
	float				stateTimer = 0.0f;
	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
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
private:
	bool loopSe = true;

	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
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
private:
	bool loopSe = false;
	float				stateTimer = 0.0f;
	float				attackRange = 1.5f;
	bool                jumpCheck = false;
	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
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
private:
	bool loopSe = false;

	float				stateTimer = 0.0f;

	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
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
private:
	bool loopSe = false;

	float				stateTimer = 0.0f;

	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;

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
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	bool loopSe = false;
	std::unique_ptr<Effect> tellePort;
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
	float attackCheckRange = 6.0f;

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
	DirectX::XMFLOAT2 angleRange = { 0.1f,0.1f };

	bool              oneAttackCheck = false;
	// �`��
	int				  isPlayerDrawCheck = 0;
	// �z�[�~���O��񂾂�
	bool			  isHomingStartCheck = false;
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
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	bool loopSe = false;

	float				stateTimer = 0.0f;
	bool				button = false;
	bool				buttonSeconde = false;

	float				turnSpeed = DirectX::XMConvertToRadians(720);

	float               speed = 10;

	float               attackSpeed = 3.0f;

	// �ŏ��̈�񂾂��Ăт�������
	bool                InitializationCheck = false;

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
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	bool loopSe = false;

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
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	bool loopSe = false;

	DirectX::XMFLOAT3 position = {0.0f,0.0f,0.0f};
	DirectX::XMFLOAT3 angle = { 0.0f,0.0f,0.0f };

	float				stateTimer = 0.0f;

	std::unique_ptr<Effect> lightning;
	std::unique_ptr<Effect> lightningAttack;
	std::unique_ptr<Effect> lightningHit;

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
private:
	// �񕜖��@
	std::unique_ptr<Effect> heale;

	// se
	bool loopSe = false;

	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;

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
private:
	enum  AttackMemory
	{
		OnePushu = 1,
		TwoPushu,
		ThreePushu,
	};
private:
	bool loopSe = false;

	DirectX::XMFLOAT3 position = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 angle = { 0.0f,0.0f,0.0f };

	float				stateTimer = 0.0f;

	std::unique_ptr<Effect> fire;
	std::unique_ptr<Effect> fireAttack;

	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;

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

private:
	bool loopSe = false;

	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
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

private:
	bool loopSe = false;

	// �G�t�F�N�g
	std::unique_ptr<Effect> wind;

	float				stateTimer = 0.0f;

	float               moveSpeed = 10.0f;
	float               speed = 4.5f;
	float               flySpeed = 3.0f;

	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
	Model::ModelAnim modelAnimUpper;
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
private:
	bool loopSe = false;

	float				stateTimer = 0.0f;

	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
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
private:
	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
	// �o�ߎ��ԍő�
	float stateTimer = 0.0f;
	float stateTimerMax = 40;

	ColorGradingData colorGradingData;
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
private:
	float				stateTimer = 0.0f;

	// �X�e�[�g�ő�l
	int					stateSize = 2;

	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
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
private:
	float				stateTimer = 0.0f;
	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;

	// �߂�����
	bool secondeMortion = false;

	// �G�t�F�N�g
	std::unique_ptr<Effect> lightning;
	std::unique_ptr<Effect> lightningAttack;
	std::unique_ptr<Effect> lightningHit;

	RadialBlurData radialBlurData;
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

private:
	float				stateTimer = 0.0f;

	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
};



// �v���C���[�N���A�p�ҋ@�X�e�[�g�I�u�W�F�N�g
class PlayerClearEscapeState : public State
{
public:
	// �R���X�g���N�^
	PlayerClearEscapeState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerClearEscapeState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;
private:
	float				stateTimer = 0.0f;
	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
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
private:
	float				stateTimer = 0.0f;
};

// �v���C���[�N���A�p�ҋ@�X�e�[�g�I�u�W�F�N�g
class PlayerOverReviveState : public State
{
public:
	// �R���X�g���N�^
	PlayerOverReviveState(std::weak_ptr<Actor> player) :State(player) {};
	// �f�X�g���N�^
	~PlayerOverReviveState() {}
	// �X�e�[�g�ɓ��������̃��\�b�h
	void Enter()override;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	void Execute(float elapsedTime)override;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	void Exit()override;

private:
	float				stateTimer = 0.0f;

	// �A�j���[�V�������[��
	Model::ModelAnim modelAnim;
	
	// �|�[�Y�؂�ւ�
	bool isPoseStarte = false;

	// �I���
	bool endState = true;
};


