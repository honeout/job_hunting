#pragma once

#include <DirectXMath.h>
#include <vector>

#include "MessageData.h"
#include "Messenger.h"
#include "Collision.h"

// カメラコントローラー
class CameraController
{

	// モード
	enum class Mode
	{
		FreeCamera,		// フリーカメラ
		LockonCamera,	// ロックオンカメラ
		MotionCamera,	// モーションカメラ
	};

public:
	CameraController();
	~CameraController();

	// 更新処理
	void Update(float elapsedTime);

private:
	// フリーカメラ更新処理
	void FreeCamera(float elapsedTime);

	// ロックオンカメラ更新処理
	void LockonCamera(float elapsedTime);

	// モーションカメラ更新処理
	void MotionCamera(float elapsedTime);

private:
	// フリーカメラ
	void OnFreeMode(void* data);

	// ロックオンカメラ
	void OnLockonMode(void* data);

	// モーションカメラ
	void OnMotionMode(void* data);

	// カメラ揺れ
	void OnShake(void* data);

	// 横軸のズレ方向算出
	float CalcSide(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2);

private:
	Mode				mode = Mode::FreeCamera;
	DirectX::XMFLOAT3	position = { 0, 0, 0 };
	DirectX::XMFLOAT3	target = { 0, 0, 0 };
	DirectX::XMFLOAT3	angle = { 0, 0, 0 };
	float				rollSpeed = DirectX::XMConvertToRadians(90);
	float				range = 10.0f;
	float				maxAngleX = DirectX::XMConvertToRadians(+45);
	float				minAngleX = DirectX::XMConvertToRadians(+5);
	// ロックオン処理
	DirectX::XMFLOAT3	newPosition = { 0, 0, 0 };
	DirectX::XMFLOAT3	newTarget = { 0, 0, 0 };
	DirectX::XMFLOAT3	targetWork[2] = { { 0, 0, 0 }, { 0, 0, 0 } };	// 0 : 座標, 1 : 注視点
	float				lengthLimit[2] = { 5, 7 };
	float				sideValue = 1;
	// モーションカメラ
	float				motionTimer = 0;
	std::vector<CameraMotionData>	motionData;
	// カメラ揺れ
	float				shakeTimer = 0;
	float				shakePower = 1;

	float               lengthMin = 1;

	// メッセージキー
	uint64_t			CAMERACHANGEFREEMODEKEY;
	uint64_t			CAMERACHANGELOCKONMODEKEY;
	uint64_t			CAMERACHANGEMOTIONMODEKEY;
	uint64_t			CAMERASHAKEKEY;
	
};

