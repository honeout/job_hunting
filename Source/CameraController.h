#pragma once

#include <DirectXMath.h>
#include <vector>

#include "MessageData.h"
#include "Messenger.h"
#include "Collision.h"

// �J�����R���g���[���[
class CameraController
{

	// ���[�h
	enum class Mode
	{
		FreeCamera,		// �t���[�J����
		LockonCamera,	// ���b�N�I���J����
		MotionCamera,	// ���[�V�����J����
	};

public:
	CameraController();
	~CameraController();

	// �X�V����
	void Update(float elapsedTime);

private:
	// �t���[�J�����X�V����
	void FreeCamera(float elapsedTime);

	// ���b�N�I���J�����X�V����
	void LockonCamera(float elapsedTime);

	// ���[�V�����J�����X�V����
	void MotionCamera(float elapsedTime);

private:
	// �t���[�J����
	void OnFreeMode(void* data);

	// ���b�N�I���J����
	void OnLockonMode(void* data);

	// ���[�V�����J����
	void OnMotionMode(void* data);

	// �J�����h��
	void OnShake(void* data);

	// �����̃Y�������Z�o
	float CalcSide(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2);

private:
	Mode				mode = Mode::FreeCamera;
	DirectX::XMFLOAT3	position = { 0, 0, 0 };
	DirectX::XMFLOAT3	target = { 0, 0, 0 };
	DirectX::XMFLOAT3	angle = { 0, 0, 0 };
	float				rollSpeed = DirectX::XMConvertToRadians(90);
	float				range = 10.0f;
	float				maxAngleX = DirectX::XMConvertToRadians(+45);
	float				minAngleX = DirectX::XMConvertToRadians(-45);
	// ���b�N�I������
	DirectX::XMFLOAT3	newPosition = { 0, 0, 0 };
	DirectX::XMFLOAT3	newTarget = { 0, 0, 0 };
	DirectX::XMFLOAT3	targetWork[2] = { { 0, 0, 0 }, { 0, 0, 0 } };	// 0 : ���W, 1 : �����_
	float				lengthLimit[2] = { 5, 7 };
	float				sideValue = 1;
	// ���[�V�����J����
	float				motionTimer = 0;
	std::vector<CameraMotionData>	motionData;
	// �J�����h��
	float				shakeTimer = 0;
	float				shakePower = 1;

	// ���b�Z�[�W�L�[
	uint64_t			CAMERACHANGEFREEMODEKEY;
	uint64_t			CAMERACHANGELOCKONMODEKEY;
	uint64_t			CAMERACHANGEMOTIONMODEKEY;
	uint64_t			CAMERASHAKEKEY;
	//public:
//    CameraController() {}
//    ~CameraController() {}
//
//    // �X�V����
//    void Update(float elasedTime);
//
//    void RockUpdate(float elapsedTime, const DirectX::XMFLOAT3& cameraPosition);
//
//    // �^�[�Q�b�g�ʒu�ݒ� �����ƕϐ�������
//    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }
//
//    // �f�o�b�O�pGUI�`��
//    void DrawDebugGUI();
//
//    // �p�x�Z�b�g
//    void SetAngle(DirectX::XMFLOAT3 angle) 
//    {
//        this->angle = angle;
//    }
//
//    // �p�xY���Z�b�g
//    void SetAngleY(float angleY)
//    {
//        this->angle.y = angleY;
//    }
//
//    void SetFrontAngle(DirectX::XMFLOAT3 frontAngle)
//    {
//        this->frontAngle = frontAngle;
//    }
//
//private:
//    DirectX::XMFLOAT3       target = { 0, 0, 0 };
//    DirectX::XMFLOAT3       angle = { 0, 0, 0 };
//    float                   rollSpeed = DirectX::XMConvertToRadians(90);
//    // �͈�
//    float                   range = 10.0f;
//    float                   maxAngleX = DirectX::XMConvertToRadians(35);
//    float                   minAngleX = DirectX::XMConvertToRadians(6);
//
//    DirectX::XMFLOAT3 frontAngle;
};

