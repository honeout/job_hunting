#pragma once

#include <DirectXMath.h>
#include "Component.h"

// 移動
class Movement : public Component
{
public:
	Movement();
	virtual ~Movement();

	// 名前取得
	const char* GetName() const override { return "Movement"; }

	// GUI描画
	void OnGUI() override;

	// 移動
	void Move(const DirectX::XMFLOAT3& direction, float elapsedTime);
	void MoveLocal(const DirectX::XMFLOAT3& direction, float elapsedTime);

	// 旋回
	void Turn(const DirectX::XMFLOAT3& direction, float elapsedTime);

	// 着地した時に呼べれる
	bool OnLanding();

	// ジャンプ処理
	void JumpVelocity( float speed);

	// 水平速力更新処理
	void UpdateHorizontalVelocity( float elapsedFrame);
	// 水平移動更新処理
	void UpdateHorizontalMove( float elapsedTime);


	// 垂直速力更新処理
	void UpdateVerticalVelocity( float elapsedFrame);

	// 垂直移動更新処理
	void UpdateVerticalMove( float elapsedTime);
	// 速力処理更新
	void UpdateVelocity(const DirectX::XMFLOAT3& direction, float elapsedTime);



private:
	float		moveSpeed = 5.0f;
	float		turnSpeed = 6.28f;

	// 最大ジャンプ数
	int jumpCount = 0;

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

	// 重力
	float gravity;


	// 地面確認
	bool         isGround = false;


};
