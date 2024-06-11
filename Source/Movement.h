#pragma once

#include <DirectXMath.h>
#include "Component.h"

// �ړ�
class Movement : public Component
{
public:
	Movement();
	virtual ~Movement();

	// ���O�擾
	const char* GetName() const override { return "Movement"; }

	// GUI�`��
	void OnGUI() override;

	// �ړ�
	void Move(const DirectX::XMFLOAT3& direction, float elapsedTime);
	void MoveLocal(const DirectX::XMFLOAT3& direction, float elapsedTime);

	// ����
	void Turn(const DirectX::XMFLOAT3& direction, float elapsedTime);

	// ���n�������ɌĂׂ��
	bool OnLanding();

	// �W�����v����
	void JumpVelocity( float speed);

	// �������͍X�V����
	void UpdateHorizontalVelocity( float elapsedFrame);
	// �����ړ��X�V����
	void UpdateHorizontalMove( float elapsedTime);


	// �������͍X�V����
	void UpdateVerticalVelocity( float elapsedFrame);

	// �����ړ��X�V����
	void UpdateVerticalMove( float elapsedTime);
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

private:
	float		moveSpeed = 5.0f;
	float		turnSpeed = 6.28f;

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

	// �d��
	float gravity;


	// �n�ʊm�F
	bool         isGround = false;

	// ��
	float stepOffSet = 1.0f;

	// �g�����[�g��
	float               height = 2.0f;


};
