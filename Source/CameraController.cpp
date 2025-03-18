#include <imgui.h>
#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"
#include "StageMain.h"



CameraController::CameraController()
{
	position = Camera::Instance().GetEye();
	newPosition = Camera::Instance().GetEye();
	CAMERACHANGEFREESELECTMODEKEY = Messenger::Instance().AddReceiver(MessageData::CAMERACHANGEFREESELECTMODE, [&](void* data) { OnFreeSelectMode(data); });
	CAMERACHANGEFREEMODEKEY = Messenger::Instance().AddReceiver(MessageData::CAMERACHANGEFREEMODE, [&](void* data) { OnFreeMode(data); });
	CAMERACHANGELOCKONMODEKEY = Messenger::Instance().AddReceiver(MessageData::CAMERACHANGELOCKONMODE, [&](void* data) { OnLockonMode(data); });
	CAMERACHANGELOCKONTOPHEIGHTMODEKEY = Messenger::Instance().AddReceiver(MessageData::CAMERACHANGELOCKONTOPHEIGHTMODE, [&](void* data) { OnLockonTopHeightMode(data); });
	CAMERACHANGEMOTIONMODEKEY = Messenger::Instance().AddReceiver(MessageData::CAMERACHANGEMOTIONMODE, [&](void* data) { OnMotionMode(data); });
	CAMERASHAKEKEY = Messenger::Instance().AddReceiver(MessageData::CAMERASHAKE, [&](void* data) { OnShake(data); });
}

CameraController::~CameraController()
{
	Messenger::Instance().RemoveReceiver(CAMERACHANGEFREESELECTMODEKEY);
	Messenger::Instance().RemoveReceiver(CAMERACHANGEFREEMODEKEY);
	Messenger::Instance().RemoveReceiver(CAMERACHANGELOCKONMODEKEY);
	Messenger::Instance().RemoveReceiver(CAMERACHANGELOCKONTOPHEIGHTMODEKEY);
	Messenger::Instance().RemoveReceiver(CAMERACHANGEMOTIONMODEKEY);
	Messenger::Instance().RemoveReceiver(CAMERASHAKEKEY);
}

//void CameraController::Start()
//{
//	position = Camera::Instance().GetEye();
//	newPosition = Camera::Instance().GetEye();
//	CAMERACHANGEFREEMODEKEY = Messenger::Instance().AddReceiver(MessageData::CAMERACHANGEFREEMODE, [&](void* data) { OnFreeMode(data); });
//	CAMERACHANGELOCKONMODEKEY = Messenger::Instance().AddReceiver(MessageData::CAMERACHANGELOCKONMODE, [&](void* data) { OnLockonMode(data); });
//	CAMERACHANGEMOTIONMODEKEY = Messenger::Instance().AddReceiver(MessageData::CAMERACHANGEMOTIONMODE, [&](void* data) { OnMotionMode(data); });
//	CAMERASHAKEKEY = Messenger::Instance().AddReceiver(MessageData::CAMERASHAKE, [&](void* data) { OnShake(data); });
//}

// 更新処理
void CameraController::Update(float elapsedTime)
{
	// 各モードでの処理
	switch (mode)
	{
	case Mode::FreeSelectCamera: FreeSelectCamera(elapsedTime); break;
	case	Mode::FreeCamera:	FreeCamera(elapsedTime);	break;
	case	Mode::LockonCamera:	LockonCamera(elapsedTime);	break;
	case	Mode::LockonHeightCamera:	LockonTopHeightCamera(elapsedTime);	break;
	case	Mode::MotionCamera:	MotionCamera(elapsedTime);	break;
	}

	// カメラ揺れ
	if (shakeTimer > 0)
	{
		newPosition.x += (rand() % 3 - 1) * shakePower;
		newPosition.y += (rand() % 3 - 1) * shakePower;
		newPosition.z += (rand() % 3 - 1) * shakePower;
		shakeTimer -= elapsedTime;
	}



	// Todo　壁の最終範囲決めるまで使わないので一旦コメント
	// 地形との当たり判定を行う
	//HitResult	hitResult;
	//if (Collision::RayCast(newTarget, newPosition, hitResult))
	//{
	//	DirectX::XMVECTOR	p = DirectX::XMLoadFloat3(&hitResult.position);
	//	DirectX::XMVECTOR	cuv = DirectX::XMVectorSet(0, 1, 0, 0);
	//	p = DirectX::XMVectorMultiplyAdd(DirectX::XMVectorReplicate(4), cuv, p);
	//	DirectX::XMStoreFloat3(&newPosition, p);
	//}

		// X軸のカメラ回転を制限
	if (angle.x < minAngleX)
	{
		angle.x = minAngleX;
	}
	if (angle.x > maxAngleX)
	{
		angle.x = maxAngleX;
	}
	// Y軸の回転値を-3.14～3.14に収まるようにする
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}

	// 新しいカメラの最低位置
	if(newPosition.y - FLT_EPSILON <= minPositionY + FLT_EPSILON)
	{
		newPosition.y  = minPositionY;
	}


	// 徐々に目標に近づける
	static	constexpr	float	Speed = 1.0f / 8.0f;
	position.x += (newPosition.x - position.x) * Speed;
	position.y += (newPosition.y - position.y) * Speed;
	position.z += (newPosition.z - position.z) * Speed;
	target.x += (newTarget.x - target.x) * Speed;
	target.y += (newTarget.y - target.y) * Speed;
	target.z += (newTarget.z - target.z) * Speed;

	// カメラに視点を注視点を設定
	Camera::Instance().SetLookAt(position, target, DirectX::XMFLOAT3(0, 1, 0));
}
#ifdef _DEBUG
void CameraController::OnGUI()
{
	if (ImGui::TreeNode("camera"))
	{
		ImGui::SliderFloat3("Position", &position.x, 0.0f, 10.0f);
		ImGui::SliderFloat3("Angle", &angle.x, 0.0f, 10.0f);
		ImGui::SliderFloat("length", &lengthMinRock, 0.0f, 20.0f);
		ImGui::SliderFloat("cameraRandeDebug", &cameraRandeDebug, 0.0f, 20.0f);
		ImGui::SliderFloat("height", &heightMaxRock, -20.0f, 20.0f);
		ImGui::SliderFloat("maxLockTopHeightAngleX", &maxAngleX, -60.0f, 60.0f);
		ImGui::SliderFloat("minLockTopHeightAngleX", &minAngleX, -60.0f, 60.0f);
		
		//ImGui::SliderFloat("maxLockTopHeightAngleX", &maxLockTopHeightAngleX, -60.0f, 60.0f);
		//ImGui::SliderFloat("minLockTopHeightAngleX", &minLockTopHeightAngleX, -60.0f, 60.0f);

		ImGui::TreePop();
	}
	ImGui::Separator();
}
#endif // _DEBUG

bool CameraController::GetCameraMortionDataTime()
{

	// その瞬間だけ欲しい
	if (isEffect)
	{
		isEffect = false;

		return true;
	}

	return isEffect;
}

void CameraController::FreeSelectCamera(float elapsedTime)
{
	//GamePad& gamePad = Input::Instance().GetGamePad();
	//float ax = gamePad.GetAxisRX();
	//float ay = gamePad.GetAxisRY();
	//// カメラの回転速度
	//float speed = rollSpeed * elapsedTime;

	//// スティックの入力値に合わせてX軸とY軸を回転
	//angle.x += ay * speed;
	//angle.y += ax * speed;

	// X軸のカメラ回転を制限
	if (angle.x < minAngleX)
	{
		angle.x = minAngleX;
	}
	if (angle.x > maxAngleX)
	{
		angle.x = maxAngleX;
	}
	// Y軸の回転値を-3.14～3.14に収まるようにする
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}

	// カメラ回転値を回転行列に変換
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	// 回転行列から前方向ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	// 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
	newPosition.x = target.x - front.x * range;
	newPosition.y = target.y - front.y * range;
	newPosition.z = target.z - front.z * range;
}

void CameraController::FreeCamera(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisRX();
	float ay = gamePad.GetAxisRY();
	// カメラの回転速度
	float speed = rollSpeed * elapsedTime;

	// スティックの入力値に合わせてX軸とY軸を回転
	angle.x += ay * speed;
	angle.y += ax * speed;

	// X軸のカメラ回転を制限
	if (angle.x < minAngleX)
	{
		angle.x = minAngleX;
	}
	if (angle.x > maxAngleX)
	{
		angle.x = maxAngleX;
	}
	// Y軸の回転値を-3.14～3.14に収まるようにする
	if (angle.y < -DirectX::XM_PI)
	{
		angle.y += DirectX::XM_2PI;
	}
	if (angle.y > DirectX::XM_PI)
	{
		angle.y -= DirectX::XM_2PI;
	}

	// カメラ回転値を回転行列に変換
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	// 回転行列から前方向ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	// 注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
	newPosition.x = target.x - front.x * range;
	newPosition.y = target.y - front.y * range;
	newPosition.z = target.z - front.z * range;
}

void CameraController::LockonCamera(float elapsedTime)
{
	//	後方斜に移動させる
	DirectX::XMVECTOR	t0 = DirectX::XMVectorSet(targetWork[0].x, targetWork[0].y, targetWork[0].z, 0);
	DirectX::XMVECTOR	t1 = DirectX::XMVectorSet(targetWork[1].x, targetWork[1].y, targetWork[1].z, 0);
	DirectX::XMVECTOR	crv = DirectX::XMLoadFloat3(&Camera::Instance().GetRight());
	DirectX::XMVECTOR	cuv = DirectX::XMVectorSet(0, 1, 0, 0);
	DirectX::XMVECTOR   spacingValue = DirectX::XMVectorSet(0, 1, 1, 0);
	DirectX::XMVECTOR	v = DirectX::XMVectorSubtract(t1, t0);

	//DirectX::XMVECTOR   d = DirectX::XMVector3Normalize(v);
	DirectX::XMVECTOR	l = DirectX::XMVector3Length(v);
	float len;
	DirectX::XMStoreFloat(&len,l);

	cameraRandeDebug = len;

	//if (len < rangeRock)
	//{
	//	v = DirectX::XMVectorScale(d, range);
	//}

	t0 = DirectX::XMLoadFloat3(&targetWork[0]);
	t1 = DirectX::XMLoadFloat3(&targetWork[1]);

	//	新しい注視点を算出
	DirectX::XMStoreFloat3(&newTarget, DirectX::XMVectorMultiplyAdd(v, DirectX::XMVectorReplicate(lookAtOffset), t0));


	// ターゲットの高さ
	newTarget.y = topTargetY;

	// Todo導入途中　敵との距離一定で離れる。
	//if (len < lengthMin)
	//{
	//	DirectX::XMVECTOR	vv = DirectX::XMVectorSubtract(t0, t1);
	//	t0 = DirectX::XMVectorMultiply(t0, vv);
	//}
	//	新しい座標を算出
	l = DirectX::XMVectorClamp(l
		, DirectX::XMVectorReplicate(lengthLimit[0])
		, DirectX::XMVectorReplicate(lengthLimit[1]));
	t0 = DirectX::XMVectorMultiplyAdd(l, DirectX::XMVector3Normalize(DirectX::XMVectorNegate(v)), t0);
	// 少し右
	//t0 = DirectX::XMVectorMultiplyAdd(crv, DirectX::XMVectorReplicate(sideValue * 3.0f), t0);

	//// 高さ
	//if (targetWork[1].y >= topHeight)
	//{
	//	// 少し上
	//	t0 = DirectX::XMVectorMultiplyAdd(cuv, DirectX::XMVectorReplicate(3.0f), t0);
	//	// 少し離れる
	//	t0 = DirectX::XMVectorMultiplyAdd(DirectX::XMVector3Normalize(DirectX::XMVectorNegate(v)), DirectX::XMVectorReplicate(20.0f), t0);

	//	DirectX::XMStoreFloat3(&newPosition, t0);

	//	newPosition.y = targetWork[1].y - 1.0f;

	//	return;
	//}

	// 距離が一定以上
	if (lengthMin <= len)
	{
		// 少し上
		t0 = DirectX::XMVectorMultiplyAdd(cuv, DirectX::XMVectorReplicate(topHeight), t0);



		DirectX::XMStoreFloat3(&newPosition, t0);


	}
	else
	{
		// 少し上
		//t0 = DirectX::XMVectorMultiplyAdd(cuv, DirectX::XMVectorReplicate(heightMaxRock), t0);
		// 少し離れる
		t0 = DirectX::XMVectorMultiplyAdd(DirectX::XMVector3Normalize(DirectX::XMVectorNegate(v)), DirectX::XMVectorReplicate(lengthMinRock), t0);

		DirectX::XMStoreFloat3(&newPosition, t0);

		//newPosition.y = -2.5f;
		newPosition.y = heightMaxRock;
	}


	
}

void CameraController::LockonTopHeightCamera(float elapsedTime)
{
	//	後方斜に移動させる
	DirectX::XMVECTOR	t0 = DirectX::XMVectorSet(targetWork[0].x, 0.0f, targetWork[0].z, 0);
	DirectX::XMVECTOR	t1 = DirectX::XMVectorSet(targetWork[1].x, 0.0f, targetWork[1].z, 0);
	DirectX::XMVECTOR	crv = DirectX::XMLoadFloat3(&Camera::Instance().GetRight());
	DirectX::XMVECTOR	cuv = DirectX::XMVectorSet(0, 1, 0, 0);
	DirectX::XMVECTOR   spacingValue = DirectX::XMVectorSet(0, 1, 1, 0);
	DirectX::XMVECTOR	v = DirectX::XMVectorSubtract(t1, t0);
	DirectX::XMVECTOR	l = DirectX::XMVector3Length(v);
	float len;
	DirectX::XMStoreFloat(&len, l);

	cameraRandeDebug = len;

	//if (len <= cameraDisableRange)
	//	return;

	t0 = DirectX::XMLoadFloat3(&targetWork[0]);
	t1 = DirectX::XMLoadFloat3(&targetWork[1]);

	//	新しい注視点を算出
	DirectX::XMStoreFloat3(&newTarget, DirectX::XMVectorMultiplyAdd(v, DirectX::XMVectorReplicate(lookAtOffset), t0));

	// ターゲットの高さ
	newTarget.y = topTargetY;

	//	新しい座標を算出
	l = DirectX::XMVectorClamp(l
		, DirectX::XMVectorReplicate(lengthLimit[0])
		, DirectX::XMVectorReplicate(lengthLimit[1]));
	t0 = DirectX::XMVectorMultiplyAdd(l, DirectX::XMVector3Normalize(DirectX::XMVectorNegate(v)), t0);

	//// 高さ
	//if (targetWork[1].y >= topHeight)
	//{
		// 少し上
		//t0 = DirectX::XMVectorMultiplyAdd(cuv, DirectX::XMVectorReplicate(topHeight), t0);

	if (len > cameraDisableRange)
	// 少し離れる 16.0f
	t0 = DirectX::XMVectorMultiplyAdd(DirectX::XMVector3Normalize(DirectX::XMVectorNegate(v)), DirectX::XMVectorReplicate(lengthMinRock), t0);

	else
	{
		// 少し離れる 16.0f
		t0 = DirectX::XMVectorMultiplyAdd(DirectX::XMVector3Normalize(DirectX::XMVectorNegate(v)), DirectX::XMVectorReplicate(lengthRock), t0);

	}

	DirectX::XMStoreFloat3(&newPosition, t0);

	//newPosition.y = targetWork[1].y - 1.0f;
	newPosition.y = topHeight;





	//

	//// 距離が一定以上
	//if (lengthMin <= len)
	//{
	//	// 少し上
	//	t0 = DirectX::XMVectorMultiplyAdd(cuv, DirectX::XMVectorReplicate(3.0f), t0);

	//	DirectX::XMStoreFloat3(&newPosition, t0);


	//}
	//else
	//{
	//	// 少し上
	//	t0 = DirectX::XMVectorMultiplyAdd(cuv, DirectX::XMVectorReplicate(3.0f), t0);
	//	// 少し離れる
	//	t0 = DirectX::XMVectorMultiplyAdd(DirectX::XMVector3Normalize(DirectX::XMVectorNegate(v)), DirectX::XMVectorReplicate(20.0f), t0);

	//	DirectX::XMStoreFloat3(&newPosition, t0);

	//	newPosition.y = -2.0f;
	//}
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
			// 時間経過
			isEffect = motionData[0].isEffect ? motionData[0].isEffect : motionData[0].isEffect;
		}

	}
	else
	{
		bool set = false;
		for (int i = 0; i < motionData.size() - 1; ++i)
		{
			if (motionData[i].time <= motionTimer && motionTimer < motionData[i + 1].time && !motionData[i + 1].setSkipMoveCheck)
			{
				set = true;
				float	value = motionData[i + 1].time - motionData[i].time;
				value = (motionTimer - motionData[i].time) / value;
				// 時間によって位置の割り出しを変える。
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
				//// 時間経過
				//isEffect =  motionData[i].isEffect ? motionData[i].isEffect : isEffect;
				//motionData[i].isEffect = false;
				break;
			}
			// 一部モーションを飛ばす
			if (motionData[i].time <= motionTimer && motionTimer < motionData[i + 1].time && motionData[i].setSkipMoveCheck)
			{
				newPosition = motionData[i].position;
				newTarget = motionData[i].target;
				position = newPosition;
				target = newTarget;

				break;
			}

		}
		if (!set)
		{
			// 位置を最後の部分に合わせる。
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

void CameraController::OnFreeSelectMode(void* data)
{
	MessageData::CAMERACHANGEFREESELECTMODEDATA* p = static_cast<MessageData::CAMERACHANGEFREESELECTMODEDATA*>(data);
	if (this->mode != Mode::FreeSelectCamera)
	{
		// 角度算出
		DirectX::XMFLOAT3	v;
		v.x = newPosition.x - newTarget.x;
		v.y = newPosition.y - newTarget.y;
		v.z = newPosition.z - newTarget.z;
		angle.y = atan2f(v.x, v.z) + DirectX::XM_PI;
		angle.x = atan2f(v.y, v.z);
		//	角度の正規化
		angle.y = atan2f(sinf(angle.y), cosf(angle.y));
		angle.x = atan2f(sinf(angle.x), cosf(angle.x));
	}
	this->mode = Mode::FreeSelectCamera;
	this->position.y = -2.0f;
	this->newTarget = p->target;
	this->newTarget.y += 0.01f;
}

void CameraController::OnFreeMode(void* data)
{
	MessageData::CAMERACHANGEFREEMODEDATA* p = static_cast<MessageData::CAMERACHANGEFREEMODEDATA*>(data);
	if (this->mode != Mode::FreeCamera)
	{
		// 角度算出
		DirectX::XMFLOAT3	v;
		v.x = newPosition.x - newTarget.x;
		v.y = newPosition.y - newTarget.y;
		v.z = newPosition.z - newTarget.z;
		angle.y = atan2f(v.x, v.z) + DirectX::XM_PI;
		angle.x = atan2f(v.y, v.z);
		//	角度の正規化
		angle.y = atan2f(sinf(angle.y), cosf(angle.y));
		angle.x = atan2f(sinf(angle.x), cosf(angle.x));
	}
	this->mode = Mode::FreeCamera;
	this->position.y = -2.0f;
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

void CameraController::OnLockonTopHeightMode(void* data)
{
	MessageData::CAMERACHANGELOCKONHEIGHTMODEDATA* p = static_cast<MessageData::CAMERACHANGELOCKONHEIGHTMODEDATA*>(data);
	if (this->mode != Mode::LockonHeightCamera)
		sideValue = CalcSide(p->start, p->target);

	this->mode = Mode::LockonHeightCamera;
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

	this->motionData = p->data;
}

void CameraController::OnShake(void* data)
{
	MessageData::CAMERASHAKEDATA* p = static_cast<MessageData::CAMERASHAKEDATA*>(data);
	shakeTimer = p->shakeTimer;
	shakePower = p->shakePower;
}

float CameraController::CalcSide(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2)
{
	// 外積を用いて横軸のズレ方向算出
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


