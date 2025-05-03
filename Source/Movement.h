#pragma once
#include <DirectXMath.h>
#include "Component.h"
#include "Collision.h"
#include "Transform.h"

// �s���͈�
struct Area
{
	DirectX::XMFLOAT3 min;
	DirectX::XMFLOAT3 max;
};

// �ړ�
class Movement : public Component
{
public:
	Movement();
	virtual ~Movement();

	// ���O�擾
	const char* GetName() const override { return "Movement"; }

#ifdef _DEBUG
	// GUI�`��
	void OnGUI() override;
#endif // _DEBUG

	// �ړ�
	void Move(const DirectX::XMFLOAT3& direction,  float speed,  float elapsedTime);

	// ����
	bool Turn(const DirectX::XMFLOAT3& direction,float speed, float elapsedTime);
	
	// �p�x�𒲐����Ĉ��p�x�Ȃ�Ok
	bool TurnCheck(const DirectX::XMFLOAT3& direction, DirectX::XMFLOAT2 angleRange,float elapsedTime);

	// ���n�������ɌĂׂ��
	void OnLanding();

	// �W�����v����
	void JumpVelocity  ( float speed);

	// �Ռ��g
	void AddImpulse(const DirectX::XMFLOAT3& impulse, bool initialize = false);

	// �������͍X�V����
	void UpdateHorizontalVelocity( float elapsedFrame);
	// �����ړ��X�V����
	void UpdateHorizontalMove( float elapsedTime);

	// �������͍X�V����
	void UpdateVerticalVelocity(float elapsedFrame);

	// �����ړ��X�V����
	void UpdateVerticalMove(float elapsedTime);
	// ���͏����X�V
	void UpdateVelocity( float elapsedTime);

	// ���̈ʒu�Q�b�g
	float GetStepOffSet()const { return stepOffSet; }
	// ���̈ʒu�Z�b�g
	void SetStepOffSet(float stepOffSet) { this->stepOffSet = stepOffSet; }
	// �g���Q�b�g
	float GetHeight() { return this->height; }
	// �g���Z�b�g
	void SetHegith(float height) { this->height = height; }

	void SetMoveVecX(float moveVecX) { this->moveVecX = moveVecX; }
	void SetMoveVecZ(float moveVecZ) { this->moveVecZ = moveVecZ; }

	bool GetOnLadius() { return onLadius; }

	void SetOnLadius(bool onLadius) {  this->onLadius = onLadius; }

	DirectX::XMFLOAT3 GetVelocity() { return velocity; }

    void SetVelocity(DirectX::XMFLOAT3 velocity)
	{  this->velocity = velocity; }

	void SetJumpCount(int jumpCount) { this->jumpCount = jumpCount; }

	int GetJumpCount() const { return jumpCount; }

	// ������̒�~
	void SetStopFall(bool stopFall) { this->stopFall = stopFall; }
	
	// �ړ��̒�~
	void SetStopMove(bool stopMove) { this->stopMove = stopMove; }

	// �d�͐ݒ�
	void SetGravity(float gravity) { this->gravity = gravity; }

	// �G���A�O�@�s���͈�
	bool IsOutOfAreaX(const float& positionX);
	bool IsOutOfAreaZ(const float& positionZ);
	bool IsOutOfAreaY(const float& positionY);

	// �s���G���A�ݒ�
	void SetArea(DirectX::XMFLOAT3 min, DirectX::XMFLOAT3 max)
	{
		// �s���͈͍Œ�
		area.min.x = min.x;
		area.min.y = min.y;
		area.min.z = min.z;
		// �s���͈͍ō�
		area.max.x = max.x;
		area.max.y = max.y;
		area.max.z = max.z;
	};

private:
	float		moveSpeed = 5.0f;
	float		turnSpeed = 6.28f;
	float       jumpSpeed = 0;
	// �ő�W�����v��
	int jumpCount = 0;
	int jumpCountMin = 0;

	// ����
	float acceleration = 1.0f;

	// speed�ő�l
	float maxMoveSpeed = 5.0f;

	// ���C
	float   friction = 0.5f;

	// �X�Η� 
	float slopeRate = 1.0f;

	// ���x
	DirectX::XMFLOAT3  velocity = { 0,0,0 };

	// �ړ�����X�Ƒ��x
	float moveVecX = 0.0f;

	// �ړ�����Y�Ƒ��x
	float moveVecZ = 0.0f;

	// �ړ�Y�̍ő�l
	float posYMax = -3.620f;

	// ����
	float flame = 60.0f;

	// �ꎞ��~
	float stopVelocity = 0.0f;

	// ���͗L��
	float isImputEmpty = 0.0f;

	// �d��
	float gravity = -1.0f;

	// �n�ʊm�F
	bool         isGround = false;

	// ��
	float stepOffSet = 1.0f;

	// �g�����[�g��
	float               height = 2.0f;

	float speed = 0;

	// ���n����
	bool onLadius = false;

	float airControl = 0.3f;

	DirectX::XMFLOAT3 positionX = { 0,0,0 };

	int butten = 0;

	// ������̒�~
	bool stopFall = false;

	// �ړ��̒�~
	bool stopMove = false;

	// �s���͈�
	Area area;

	// �Ռ��g��
	bool isImpactV = false;
};
