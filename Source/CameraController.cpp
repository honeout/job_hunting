#include <imgui.h>
#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"
#include "StageMain.h"



CameraController::CameraController()
{
	position = Camera::Instance().GetEye();
	newPosition = Camera::Instance().GetEye();
	CAMERACHANGEFREEMODEKEY = Messenger::Instance().AddReceiver(MessageData::CAMERACHANGEFREEMODE, [&](void* data) { OnFreeMode(data); });
	CAMERACHANGELOCKONMODEKEY = Messenger::Instance().AddReceiver(MessageData::CAMERACHANGELOCKONMODE, [&](void* data) { OnLockonMode(data); });
	CAMERACHANGEMOTIONMODEKEY = Messenger::Instance().AddReceiver(MessageData::CAMERACHANGEMOTIONMODE, [&](void* data) { OnMotionMode(data); });
	CAMERASHAKEKEY = Messenger::Instance().AddReceiver(MessageData::CAMERASHAKE, [&](void* data) { OnShake(data); });
}

CameraController::~CameraController()
{
	Messenger::Instance().RemoveReceiver(CAMERACHANGEFREEMODEKEY);
	Messenger::Instance().RemoveReceiver(CAMERACHANGELOCKONMODEKEY);
	Messenger::Instance().RemoveReceiver(CAMERACHANGEMOTIONMODEKEY);
	Messenger::Instance().RemoveReceiver(CAMERASHAKEKEY);
}

// �X�V����
void CameraController::Update(float elapsedTime)
{
	// �e���[�h�ł̏���
	switch (mode)
	{
	case	Mode::FreeCamera:	FreeCamera(elapsedTime);	break;
	case	Mode::LockonCamera:	LockonCamera(elapsedTime);	break;
	case	Mode::MotionCamera:	MotionCamera(elapsedTime);	break;
	}

	// �J�����h��
	if (shakeTimer > 0)
	{
		newPosition.x += (rand() % 3 - 1) * shakePower;
		newPosition.y += (rand() % 3 - 1) * shakePower;
		newPosition.z += (rand() % 3 - 1) * shakePower;
		shakeTimer -= elapsedTime;
	}

	// �n�`�Ƃ̓����蔻����s��
	//HitResult	hitResult;
	//if (Collision::RayCast(newTarget, newPosition, hitResult))
	//{
	//	DirectX::XMVECTOR	p = DirectX::XMLoadFloat3(&hitResult.position);
	//	DirectX::XMVECTOR	cuv = DirectX::XMVectorSet(0, 1, 0, 0);
	//	p = DirectX::XMVectorMultiplyAdd(DirectX::XMVectorReplicate(4), cuv, p);
	//	DirectX::XMStoreFloat3(&newPosition, p);
	//}

		// X���̃J������]�𐧌�
	if (angle.x < minAngleX)
	{
		angle.x = minAngleX;
	}
	if (angle.x > maxAngleX)
	{
		angle.x = maxAngleX;
	}
	// Y���̉�]�l��-3.14�`3.14�Ɏ��܂�悤�ɂ���
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}

	// ���X�ɖڕW�ɋ߂Â���
	static	constexpr	float	Speed = 1.0f / 8.0f;
	position.x += (newPosition.x - position.x) * Speed;
	position.y += (newPosition.y - position.y) * Speed;
	position.z += (newPosition.z - position.z) * Speed;
	target.x += (newTarget.x - target.x) * Speed;
	target.y += (newTarget.y - target.y) * Speed;
	target.z += (newTarget.z - target.z) * Speed;

	// �J�����Ɏ��_�𒍎��_��ݒ�
	Camera::Instance().SetLookAt(position, target, DirectX::XMFLOAT3(0, 1, 0));
}

void CameraController::FreeCamera(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisRX();
	float ay = gamePad.GetAxisRY();
	// �J�����̉�]���x
	float speed = rollSpeed * elapsedTime;

	// �X�e�B�b�N�̓��͒l�ɍ��킹��X����Y������]
	angle.x += ay * speed;
	angle.y += ax * speed;

	// X���̃J������]�𐧌�
	if (angle.x < minAngleX)
	{
		angle.x = minAngleX;
	}
	if (angle.x > maxAngleX)
	{
		angle.x = maxAngleX;
	}
	// Y���̉�]�l��-3.14�`3.14�Ɏ��܂�悤�ɂ���
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}

	// �J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	// ��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	// �����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	newPosition.x = target.x - front.x * range;
	newPosition.y = target.y - front.y * range;
	newPosition.z = target.z - front.z * range;
}

void CameraController::LockonCamera(float elapsedTime)
{
	//	����΂Ɉړ�������
	DirectX::XMVECTOR	t0 = DirectX::XMVectorSet(targetWork[0].x, targetWork[0].y, targetWork[0].z, 0);
	DirectX::XMVECTOR	t1 = DirectX::XMVectorSet(targetWork[1].x, targetWork[1].y, targetWork[1].z, 0);
	DirectX::XMVECTOR	crv = DirectX::XMLoadFloat3(&Camera::Instance().GetRight());
	DirectX::XMVECTOR	cuv = DirectX::XMVectorSet(0, 1, 0, 0);
	DirectX::XMVECTOR	v = DirectX::XMVectorSubtract(t1, t0);
	DirectX::XMVECTOR	l = DirectX::XMVector3Length(v);
	float len;
	DirectX::XMStoreFloat(&len,v);

	t0 = DirectX::XMLoadFloat3(&targetWork[0]);
	t1 = DirectX::XMLoadFloat3(&targetWork[1]);

	//	�V���������_���Z�o
	DirectX::XMStoreFloat3(&newTarget, DirectX::XMVectorMultiplyAdd(v, DirectX::XMVectorReplicate(0.5f), t0));

	//if (len < lengthMin)
	//{
	//	DirectX::XMVECTOR	vv = DirectX::XMVectorSubtract(t0, t1);
	//	t0 = DirectX::XMVectorMultiply(t0, vv);
	//}
	//	�V�������W���Z�o
	l = DirectX::XMVectorClamp(l
		, DirectX::XMVectorReplicate(lengthLimit[0])
		, DirectX::XMVectorReplicate(lengthLimit[1]));
	t0 = DirectX::XMVectorMultiplyAdd(l, DirectX::XMVector3Normalize(DirectX::XMVectorNegate(v)), t0);
	// �����E
	//t0 = DirectX::XMVectorMultiplyAdd(crv, DirectX::XMVectorReplicate(sideValue * 3.0f), t0);
	// ������
	t0 = DirectX::XMVectorMultiplyAdd(cuv, DirectX::XMVectorReplicate(3.0f), t0);
	DirectX::XMStoreFloat3(&newPosition, t0);
}

void CameraController::MotionCamera(float elapsedTime)
{
	if (motionData.empty())
		return;

	motionTimer += elapsedTime * 60;
	if (motionData.size() == 1)
	{
		if (motionTimer >= motionData[0].time)
		{
			newPosition = motionData[0].position;
			newTarget = motionData[0].target;
			position = newPosition;
			target = newTarget;
		}
	}
	else
	{
		bool set = false;
		for (int i = 0; i < motionData.size() - 1; ++i)
		{
			if (motionData[i].time <= motionTimer && motionTimer < motionData[i + 1].time)
			{
				set = true;
				float	value = motionData[i + 1].time - motionData[i].time;
				value = (motionTimer - motionData[i].time) / value;
				newPosition = motionData[i].position;
				newPosition.x += (motionData[i + 1].position.x - motionData[i].position.x) * value;
				newPosition.y += (motionData[i + 1].position.y - motionData[i].position.y) * value;
				newPosition.z += (motionData[i + 1].position.z - motionData[i].position.z) * value;
				newTarget = motionData[i].target;
				newTarget.x += (motionData[i + 1].target.x - motionData[i].target.x) * value;
				newTarget.y += (motionData[i + 1].target.y - motionData[i].target.y) * value;
				newTarget.z += (motionData[i + 1].target.z - motionData[i].target.z) * value;
				position = newPosition;
				target = newTarget;
				break;
			}
		}
		if (!set)
		{
			if (motionTimer >= motionData[motionData.size() - 1].time)
			{
				newPosition = motionData[motionData.size() - 1].position;
				newTarget = motionData[motionData.size() - 1].target;
				position = newPosition;
				target = newTarget;
			}
		}
	}
}

void CameraController::OnFreeMode(void* data)
{
	MessageData::CAMERACHANGEFREEMODEDATA* p = static_cast<MessageData::CAMERACHANGEFREEMODEDATA*>(data);
	if (this->mode != Mode::FreeCamera)
	{
		// �p�x�Z�o
		DirectX::XMFLOAT3	v;
		v.x = newPosition.x - newTarget.x;
		v.y = newPosition.y - newTarget.y;
		v.z = newPosition.z - newTarget.z;
		angle.y = atan2f(v.x, v.z) + DirectX::XM_PI;
		angle.x = atan2f(v.y, v.z);
		//	�p�x�̐��K��
		angle.y = atan2f(sinf(angle.y), cosf(angle.y));
		angle.x = atan2f(sinf(angle.x), cosf(angle.x));
	}
	this->mode = Mode::FreeCamera;
	this->newTarget = p->target;
	this->newTarget.y += 0.01f;
}

void CameraController::OnLockonMode(void* data)
{
	MessageData::CAMERACHANGELOCKONMODEDATA* p = static_cast<MessageData::CAMERACHANGELOCKONMODEDATA*>(data);
	if (this->mode != Mode::LockonCamera)
		sideValue = CalcSide(p->start, p->target);

	this->mode = Mode::LockonCamera;
	targetWork[0] = p->start;
	targetWork[1] = p->target;
	targetWork[0].y += 0.01f;
	targetWork[1].y += 0.01f;
}

void CameraController::OnMotionMode(void* data)
{
	MessageData::CAMERACHANGEMOTIONMODEDATA* p = static_cast<MessageData::CAMERACHANGEMOTIONMODEDATA*>(data);
	if (this->mode != Mode::MotionCamera)
		motionTimer = 0;
	this->mode = Mode::MotionCamera;
	motionData.clear();
	motionData = p->data;
}

void CameraController::OnShake(void* data)
{
	MessageData::CAMERASHAKEDATA* p = static_cast<MessageData::CAMERASHAKEDATA*>(data);
	shakeTimer = p->shakeTimer;
	shakePower = p->shakePower;
}

float CameraController::CalcSide(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2)
{
	// �O�ς�p���ĉ����̃Y�������Z�o
	DirectX::XMFLOAT2	v;
	v.x = position.x - target.x;
	v.y = position.z - target.z;
	float	l = sqrtf(v.x * v.x + v.y * v.y);
	v.y /= l;
	v.y /= l;
	DirectX::XMFLOAT2	n;
	n.x = p1.x - p2.x;
	n.y = p1.z - p2.z;
	l = sqrtf(n.x * n.x + n.y * n.y);
	n.x /= l;
	n.y /= l;
	return	((v.x * n.y) - (v.y * n.x) < 0) ? +1.0f : -1.0f;
}


//
//// �X�V����
//void CameraController::Update(float elasedTime)
//{
//    GamePad& gamePad = Input::Instance().GetGamePad();
//    float ax = gamePad.GetAxisRX();
//    float ay = gamePad.GetAxisRY();
//    // �J�����̉�]���x
//    float speed = rollSpeed * elasedTime ;
//
//    // �X�e�B�b�N�̓��͒l�ɂ��킹��X����Y������]
//
//    angle.x += ay * speed;// ��ɓ|���Ə㉺
//    angle.y += ax * speed;// ���E�͎����̎��肾���炙
//    
//    // �J������]�l����]�s��ɕϊ�
//    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x,angle.y,angle.z);
//   
//    // ��]�s�񂩂�O�����x�N�g�������o��
//    DirectX::XMVECTOR Front = Transform.r[2];// row�̂���
//    DirectX::XMFLOAT3 front;
//    DirectX::XMStoreFloat3(&front, Front);
//
//    // �����_������x�N�g�������Ɉ�苗���ꂽ�J�������_�����߂�
//    DirectX::XMFLOAT3 eye;
//
//    // �����ɒ����Ŗ���L�΂���A
//    // �����o���ā@�L�΂�
//    eye.x = target.x - front.x * range ;
//    eye.y = target.y - front.y * range ;
//    eye.z = target.z - front.z * range ;
//
//
//
//
//    // �J�����̎��_�ƒ����_��ݒ�
//    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
//    // X ���̃J������]�𐧌�
//
//    if (angle.x < minAngleX)
//    {
//        angle.x = minAngleX;
//    }
//
//
//    if (angle.x > maxAngleX)
//    {
//        angle.x = maxAngleX;
//    }
//
//    // Y���̉�]�l��-3.14�`3.13�ɂ����܂�悤�ɂ���
//    if (angle.y < -DirectX::XM_PI)
//    {
//        angle.y += DirectX::XM_2PI;
//    }
//
//
//    if (angle.y > DirectX::XM_PI)
//    {
//        angle.y -= DirectX::XM_2PI;
//    }
//    
//}
////
////void CameraController::RockUpdate(float elapsedTime, const DirectX::XMFLOAT3& cameraPosition)
////{
////    //GamePad& gamePad = Input::Instance().GetGamePad();
////    //float ax = gamePad.GetAxisRX();
////    //float ay = gamePad.GetAxisRY();
////    //// �J�����̉�]���x
////    //float speed = rollSpeed * elapsedTime;
////
////    //// �X�e�B�b�N�̓��͒l�ɂ��킹��X����Y������]
////
////    //angle.x += ay * speed;// ��ɓ|���Ə㉺
////    //angle.y += ax * speed;// ���E�͎����̎��肾���炙
////
////    // �J������]�l����]�s��ɕϊ�
////    //DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(frontAngle.x, frontAngle.y, frontAngle.z);
////
////    // //��]�s�񂩂�O�����x�N�g�������o��
////    //DirectX::XMVECTOR Front = Transform.r[2];// row�̂���
////    //DirectX::XMFLOAT3 front;
////    //DirectX::XMStoreFloat3(&front, Front);
////
////    //// y���W�͂���Ȃ��̂ŏ����܂��B
////    //frontAngle.y = 0;
////    //
////    //DirectX::XMVECTOR FrontDirection = DirectX::XMLoadFloat3(&frontAngle);
////    //// ���������~�����̂�
////    //DirectX::XMVector3Normalize(FrontDirection);
////
////    //DirectX::XMStoreFloat3(&frontAngle, FrontDirection);
////
////    //// �����_������x�N�g�������Ɉ�苗���ꂽ�J�������_�����߂�
////    //DirectX::XMFLOAT3 eye;
////    ////frontAngle.z *= range;
////
////    //// �����ɒ����Ŗ���L�΂���A
////    //// �����o���ā@�L�΂�
////    //eye.x = cameraPosition.x + frontAngle.x + 0.5f;
////    //eye.y = cameraPosition.y + frontAngle.y + 1;
////    ////eye.y = target.y - frontAngle.y * range;
////    ////if (frontAngle.z)
////    //
////    //eye.z = cameraPosition.z + frontAngle.z - 0.5f;
////    ////eye.x = target.x - frontAngle.x + range;
////    ////eye.y = target.y;
////    //////eye.y = target.y - frontAngle.y * range;
////    ////eye.z = target.z - frontAngle.z + range;
////
////
////
////
////    //// �J�����̎��_�ƒ����_��ݒ�
////    //Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
////    //// X ���̃J������]�𐧌�
////
////    //if (angle.x < minAngleX)
////    //{
////    //    angle.x = minAngleX;
////    //}
////
////
////    //if (angle.x > maxAngleX)
////    //{
////    //    angle.x = maxAngleX;
////    //}
////
////    //// Y���̉�]�l��-3.14�`3.13�ɂ����܂�悤�ɂ���
////    //if (angle.y < -DirectX::XM_PI)
////    //{
////    //    angle.y += DirectX::XM_2PI;
////    //}
////
////
////    //if (angle.y > DirectX::XM_PI)
////    //{
////    //    angle.y -= DirectX::XM_2PI;
////    //}
////
////    //GamePad& gamePad = Input::Instance().GetGamePad();
////    //float ax = gamePad.GetAxisRX();
////    //float ay = gamePad.GetAxisRY();
////    //// �J�����̉�]���x
////    //float speed = rollSpeed * elapsedTime;
////
////    //// �X�e�B�b�N�̓��͒l�ɂ��킹��X����Y������]
////
////    //angle.x += ay * speed;// ��ɓ|���Ə㉺
////    //angle.y += ax * speed;// ���E�͎����̎��肾���炙
////
////    //// �J������]�l����]�s��ɕϊ�
////    //DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
////
////    //// ��]�s�񂩂�O�����x�N�g�������o��
////    //DirectX::XMVECTOR Front = Transform.r[2];// row�̂���
////    //DirectX::XMFLOAT3 front;
////    //DirectX::XMStoreFloat3(&front, Front);
////
////    //// �����_������x�N�g�������Ɉ�苗���ꂽ�J�������_�����߂�
////    //DirectX::XMFLOAT3 eye;
////
////    //// �����ɒ����Ŗ���L�΂���A
////    //// �����o���ā@�L�΂�
////    //eye.x = target.x - front.x * range;
////    //eye.y = target.y - front.y * range;
////    //eye.z = target.z - front.z * range;
////
////
////
////
////    //// �J�����̎��_�ƒ����_��ݒ�
////    //Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
////    //// X ���̃J������]�𐧌�
////
////    //if (angle.x < minAngleX)
////    //{
////    //    angle.x = minAngleX;
////    //}
////
////
////    //if (angle.x > maxAngleX)
////    //{
////    //    angle.x = maxAngleX;
////    //}
////
////    //// Y���̉�]�l��-3.14�`3.13�ɂ����܂�悤�ɂ���
////    //if (angle.y < -2)
////    //{
////    //    angle.y += 2;
////    //}
////
////
////    //if (angle.y > 2)
////    //{
////    //    angle.y -= 2;
////    //}
////}
//
//void CameraController::DrawDebugGUI()
//{
//    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
//    {
//        // �g�����X�t�H�[��
//        if (ImGui::CollapsingHeader("Transform0.2", ImGuiTreeNodeFlags_DefaultOpen))
//        {
//            // �ʒu�@// ���l��������
//            ImGui::InputFloat3("angle", &angle.x);
//            // ��]
//            DirectX::XMFLOAT3 a;
//            // XMConvertToDegrees���ʂ̐��������x�ɂ���
//            // float�P�ʂŏo�����W�A����x�P�ʊp�x��
//            a.x = DirectX::XMConvertToDegrees(angle.x);
//            a.y = DirectX::XMConvertToDegrees(angle.y);
//            a.z = DirectX::XMConvertToDegrees(angle.z);
//            ImGui::InputFloat3("Angle", &a.x);
//            // �p�x���o����������W�A�����񂢂Ɂ@
//            angle.x = DirectX::XMConvertToRadians(a.x);
//            angle.y = DirectX::XMConvertToRadians(a.y);
//            angle.z = DirectX::XMConvertToRadians(a.z);
//
//        }
//    }
//    ImGui::End();
//}
