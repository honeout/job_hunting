#pragma once
#include <DirectXMath.h>
#include "Component.h"
#include "Collision.h"
#include "Transform.h"

// 行動範囲
struct Area
{
	DirectX::XMFLOAT3 min;
	DirectX::XMFLOAT3 max;
};

// 移動
class Movement : public Component
{
public:
	Movement();
	virtual ~Movement();

	// 名前取得
	const char* GetName() const override { return "Movement"; }

#ifdef _DEBUG
	// GUI描画
	void OnGUI() override;
#endif // _DEBUG

	// 移動
	void Move(const DirectX::XMFLOAT3& direction,  float speed,  float elapsedTime);

	// 旋回
	bool Turn(const DirectX::XMFLOAT3& direction,float speed, float elapsedTime);
	
	// 角度を調整して一定角度ならOk
	bool TurnCheck(const DirectX::XMFLOAT3& direction, DirectX::XMFLOAT2 angleRange,float elapsedTime);

	// 着地した時に呼べれる
	void OnLanding();

	// ジャンプ処理
	void JumpVelocity  ( float speed);

	// 衝撃波
	void AddImpulse(const DirectX::XMFLOAT3& impulse, bool initialize = false);

	// 水平速力更新処理
	void UpdateHorizontalVelocity( float elapsedFrame);
	// 水平移動更新処理
	void UpdateHorizontalMove( float elapsedTime);

	// 垂直速力更新処理
	void UpdateVerticalVelocity(float elapsedFrame);

	// 垂直移動更新処理
	void UpdateVerticalMove(float elapsedTime);
	// 速力処理更新
	void UpdateVelocity( float elapsedTime);

	// 腰の位置ゲット
	float GetStepOffSet()const { return stepOffSet; }
	// 腰の位置セット
	void SetStepOffSet(float stepOffSet) { this->stepOffSet = stepOffSet; }
	// 身長ゲット
	float GetHeight() { return this->height; }
	// 身長セット
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

	// 落ちるの停止
	void SetStopFall(bool stopFall) { this->stopFall = stopFall; }
	
	// 移動の停止
	void SetStopMove(bool stopMove) { this->stopMove = stopMove; }

	// 重力設定
	void SetGravity(float gravity) { this->gravity = gravity; }

	// エリア外　行動範囲
	bool IsOutOfAreaX(const float& positionX);
	bool IsOutOfAreaZ(const float& positionZ);
	bool IsOutOfAreaY(const float& positionY);

	// 行動エリア設定
	void SetArea(DirectX::XMFLOAT3 min, DirectX::XMFLOAT3 max)
	{
		// 行動範囲最低
		area.min.x = min.x;
		area.min.y = min.y;
		area.min.z = min.z;
		// 行動範囲最高
		area.max.x = max.x;
		area.max.y = max.y;
		area.max.z = max.z;
	};

private:
	float		moveSpeed = 5.0f;
	float		turnSpeed = 6.28f;
	float       jumpSpeed = 0;
	// 最大ジャンプ数
	int jumpCount = 0;
	int jumpCountMin = 0;

	// 加速
	float acceleration = 1.0f;

	// speed最大値
	float maxMoveSpeed = 5.0f;

	// 摩擦
	float   friction = 0.5f;

	// 傾斜率 
	float slopeRate = 1.0f;

	// 速度
	DirectX::XMFLOAT3  velocity = { 0,0,0 };

	// 移動方向Xと速度
	float moveVecX = 0.0f;

	// 移動方向Yと速度
	float moveVecZ = 0.0f;

	// 移動Yの最大値
	float posYMax = -3.620f;

	// 時間
	float flame = 60.0f;

	// 一時停止
	float stopVelocity = 0.0f;

	// 入力有無
	float isImputEmpty = 0.0f;

	// 重力
	float gravity = -1.0f;

	// 地面確認
	bool         isGround = false;

	// 腰
	float stepOffSet = 1.0f;

	// 身長メートル
	float               height = 2.0f;

	float speed = 0;

	// 着地処理
	bool onLadius = false;

	float airControl = 0.3f;

	DirectX::XMFLOAT3 positionX = { 0,0,0 };

	int butten = 0;

	// 落ちるの停止
	bool stopFall = false;

	// 移動の停止
	bool stopMove = false;

	// 行動範囲
	Area area;

	// 衝撃波か
	bool isImpactV = false;
};
