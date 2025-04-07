#pragma once

#include <DirectXMath.h>
#include <vector>

#include "Component.h"
#include "MessageData.h"
#include "Messenger.h"
#include "Collision.h"

// カメラコントローラー
class CameraController
{
	// モード
	enum class Mode
	{
		FreeSelectCamera, // フリーセレクトカメラ
		FreeCamera,		// フリーカメラ
		LockonCamera,	// ロックオンカメラ
		LockonHeightCamera,	// ロックオンカメラ
		MotionCamera,	// モーションカメラ
	};

public:
	CameraController();
	~CameraController();

	// 更新処理
	void Update(float elapsedTime);
#ifdef _DEBUG
	void OnGUI();
#endif // _DEBUG
	bool GetCameraMortionDataTime();
private:
	// フリーセレクトカメラ更新処理
	void FreeSelectCamera(float elapsedTime);
	// フリーカメラ更新処理
	void FreeCamera(float elapsedTime);
	// ロックオンカメラ更新処理
	void LockonCamera(float elapsedTime);
	// ロックオン高さカメラ更新処理
	void LockonTopHeightCamera(float elapsedTime);
	// モーションカメラ更新処理
	void MotionCamera(float elapsedTime);
private:
	// フリセレクトーカメラ
	void OnFreeSelectMode(void* data);
	// フリーカメラ
	void OnFreeMode(void* data);
	// ロックオンカメラ
	void OnLockonMode(void* data);
	void OnLockonTopHeightMode(void* data);
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
	float				range = 7.0f;
	float				rangeRock = 5.0f;
	float				maxAngleX = DirectX::XMConvertToRadians(+45);
	float				minAngleX = DirectX::XMConvertToRadians(-71);
	float				maxLockTopHeightAngleX = DirectX::XMConvertToRadians(+45);
	float				minLockTopHeightAngleX = DirectX::XMConvertToRadians(-71);
	// ロックオン処理
	DirectX::XMFLOAT3	newPosition = { 0, 0, 0 };
	DirectX::XMFLOAT3	newTarget = { 0, 0, 0 };
	DirectX::XMFLOAT3	targetWork[2] = { { 0, 0, 0 }, { 0, 0, 0 } };	// 0 : 座標, 1 : 注視点
	float				lengthLimit[2] = { 6, 8 };
	//float				lengthLimit[2] = { 5, 7 };
	float				sideValue = 1;
	// モーションカメラ
	float				motionTimer = 0;
	std::vector<CameraMotionData>	motionData;
	// カメラ揺れ
	float				shakeTimer = 0;
	float				shakePower = 1;
	// ターゲットの最低距離
	float               lengthMin = 8.5f;
	// ターゲットの攻撃許可最低距離
	float				attackMinRange = 7.0f;

	// 一定以下でカメラ機能無視
	float				cameraDisableRange = 4.0f;
	float cameraRandeDebug = 0.0f;
	// 注視点ずらす値
	float				lookAtOffset = 0.5f;
	// 敵との
	float				lengthMinRock = 10.0f;
	float				lengthRock = 8.0f;
	float				heightMaxRock = 0.0f;
	float				heightAttackMaxRock = -1.0f;

	// カメラの距離
	float				attacklengthMinRock = 10.0f;
	float				attacklengthMinRockScale = 1.3f;

	// ターゲットの一定以上高い
	float               topHeight = 2.0f;
	// ターゲットの高さ
	float				topTargetY = -1.0f;
	// メッセージキー
	uint64_t			CAMERACHANGEFREESELECTMODEKEY;
	uint64_t			CAMERACHANGEFREEMODEKEY;
	uint64_t			CAMERACHANGELOCKONMODEKEY;
	uint64_t			CAMERACHANGELOCKONTOPHEIGHTMODEKEY;
	uint64_t			CAMERACHANGEMOTIONMODEKEY;
	uint64_t			CAMERASHAKEKEY;
	float               minPositionY = -3.525f;
	bool				isEffect = false;
};

