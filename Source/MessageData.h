#pragma once

#include <DirectXMath.h>
#include <vector>

// カメラモーション情報
struct CameraMotionData
{
	float time;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 target;
	// カメラ補完スキップ
	bool setSkipMoveCheck = false;
	// エフェクトを作動刺せるか？
	bool isEffect = false;
};

namespace	MessageData
{
// フリーカメラ
static	constexpr	char*	CAMERACHANGEFREEMODE	= "CAMERA CHANGE FREEMODE";
struct	CAMERACHANGEFREEMODEDATA
{
	DirectX::XMFLOAT3	target;
};

// ロックオンカメラ
static	constexpr	char*	CAMERACHANGELOCKONMODE	= "CAMERA CHANGE LOCKONMODE";
struct	CAMERACHANGELOCKONMODEDATA
{
	DirectX::XMFLOAT3	start;
	DirectX::XMFLOAT3	target;
};

// ロックオン高さカメラ
static	constexpr	char* CAMERACHANGELOCKONTOPHEIGHTMODE = "CAMERA CHANGE LOCKONHEIGHTMODE";
struct	CAMERACHANGELOCKONHEIGHTMODEDATA
{
	DirectX::XMFLOAT3	start;
	DirectX::XMFLOAT3	target;
};


// エリアルカメラ
static	constexpr	char*	CAMERACHANGEARIELMODE	= "CAMERA CHANGE ARIELMODE";
struct	CAMERACHANGEARIELMODEDATA
{
	DirectX::XMFLOAT3	start;
	DirectX::XMFLOAT3	target;
	DirectX::XMFLOAT3	lockonTarget;
};

// モーションカメラ
static	constexpr	char*	CAMERACHANGEMOTIONMODE	= "CAMERA CHANGE MOTIONMODE";
struct	CAMERACHANGEMOTIONMODEDATA
{
	std::vector<CameraMotionData>	data;
};

// カメラ揺れエフェクト
static	constexpr	char*	CAMERASHAKE	= "CAMERA SHAKE";
struct	CAMERASHAKEDATA
{
	float	shakeTimer;
	float	shakePower;
};

};
