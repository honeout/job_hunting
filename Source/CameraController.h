#pragma once

#include <DirectXMath.h>
#include <vector>

#include "Component.h"
#include "MessageData.h"
#include "Messenger.h"
#include "Collision.h"

// �J�����R���g���[���[
class CameraController
{
	// ���[�h
	enum class Mode
	{
		FreeSelectCamera, // �t���[�Z���N�g�J����
		FreeCamera,		// �t���[�J����
		LockonCamera,	// ���b�N�I���J����
		LockonHeightCamera,	// ���b�N�I���J����
		MotionCamera,	// ���[�V�����J����
	};

public:
	CameraController();
	~CameraController();

	// �X�V����
	void Update(float elapsedTime);
#ifdef _DEBUG
	void OnGUI();
#endif // _DEBUG
	bool GetCameraMortionDataTime();
private:
	// �t���[�Z���N�g�J�����X�V����
	void FreeSelectCamera(float elapsedTime);
	// �t���[�J�����X�V����
	void FreeCamera(float elapsedTime);
	// ���b�N�I���J�����X�V����
	void LockonCamera(float elapsedTime);
	// ���b�N�I�������J�����X�V����
	void LockonTopHeightCamera(float elapsedTime);
	// ���[�V�����J�����X�V����
	void MotionCamera(float elapsedTime);
private:
	// �t���Z���N�g�[�J����
	void OnFreeSelectMode(void* data);
	// �t���[�J����
	void OnFreeMode(void* data);
	// ���b�N�I���J����
	void OnLockonMode(void* data);
	void OnLockonTopHeightMode(void* data);
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
	float				range = 7.0f;
	float				rangeRock = 5.0f;
	float				maxAngleX = DirectX::XMConvertToRadians(+45);
	float				minAngleX = DirectX::XMConvertToRadians(-71);
	float				maxLockTopHeightAngleX = DirectX::XMConvertToRadians(+45);
	float				minLockTopHeightAngleX = DirectX::XMConvertToRadians(-71);
	// ���b�N�I������
	DirectX::XMFLOAT3	newPosition = { 0, 0, 0 };
	DirectX::XMFLOAT3	newTarget = { 0, 0, 0 };
	DirectX::XMFLOAT3	targetWork[2] = { { 0, 0, 0 }, { 0, 0, 0 } };	// 0 : ���W, 1 : �����_
	float				lengthLimit[2] = { 6, 8 };
	//float				lengthLimit[2] = { 5, 7 };
	float				sideValue = 1;
	// ���[�V�����J����
	float				motionTimer = 0;
	std::vector<CameraMotionData>	motionData;
	// �J�����h��
	float				shakeTimer = 0;
	float				shakePower = 1;
	// �^�[�Q�b�g�̍Œ዗��
	float               lengthMin = 8.5f;
	// �^�[�Q�b�g�̍U�����Œ዗��
	float				attackMinRange = 7.0f;

	// ���ȉ��ŃJ�����@�\����
	float				cameraDisableRange = 4.0f;
	float cameraRandeDebug = 0.0f;
	// �����_���炷�l
	float				lookAtOffset = 0.5f;
	// �G�Ƃ�
	float				lengthMinRock = 10.0f;
	float				lengthRock = 8.0f;
	float				heightMaxRock = 0.0f;
	float				heightAttackMaxRock = -1.0f;

	// �J�����̋���
	float				attacklengthMinRock = 10.0f;
	float				attacklengthMinRockScale = 1.3f;

	// �^�[�Q�b�g�̈��ȏ㍂��
	float               topHeight = 2.0f;
	// �^�[�Q�b�g�̍���
	float				topTargetY = -1.0f;
	// ���b�Z�[�W�L�[
	uint64_t			CAMERACHANGEFREESELECTMODEKEY;
	uint64_t			CAMERACHANGEFREEMODEKEY;
	uint64_t			CAMERACHANGELOCKONMODEKEY;
	uint64_t			CAMERACHANGELOCKONTOPHEIGHTMODEKEY;
	uint64_t			CAMERACHANGEMOTIONMODEKEY;
	uint64_t			CAMERASHAKEKEY;
	float               minPositionY = -3.525f;
	bool				isEffect = false;
};

