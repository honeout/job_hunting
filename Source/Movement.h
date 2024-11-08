#pragma once

#include <DirectXMath.h>
#include "Component.h"
#include "Collision.h"
#include "Transform.h"
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
	void MoveLocal(const DirectX::XMFLOAT3& direction, float elapsedTime);

	// ����
	//void Turn(float elapsedTime, float vx, float vz, float speed);
	bool Turn(const DirectX::XMFLOAT3& direction,float speed, float elapsedTime);

	// ���n�������ɌĂׂ��
	void OnLanding();

	// �W�����v����
	void JumpVelocity  ( float speed);

	// �Ռ��g
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

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

    void SetVelocity(DirectX::XMFLOAT3 velocity) {  this->velocity = velocity; }

	void SetJumpCount(int jumpCount) { this->jumpCount = jumpCount; }

	int GetJumpCount() const { return jumpCount; }

	// ������̒�~
	void SetStopFall(bool stopFall) { this->stopFall = stopFall; }
	
	// �ړ��̒�~
	void SetStopMove(bool stopMove) { this->stopMove = stopMove; }

	// �d�͐ݒ�
	void SetGravity(float gravity) { this->gravity = gravity; }

private:
	float		moveSpeed = 5.0f;
	float		turnSpeed = 6.28f;
	float       jumpSpeed = 0;

	std::shared_ptr<Collision>	collision;
	std::shared_ptr<Transform> transformid;
	// �ő�W�����v��
	int jumpCount = 0;

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
};
