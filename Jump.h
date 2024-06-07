#pragma once

#include <DirectXMath.h>
#include "Component.h"

// 移動
class Jump : public Component
{
public:
	Jump();
	virtual ~Jump();

	// 名前取得
	const char* GetName() const override { return "Movement"; }

	// GUI描画
	void OnGUI() override;

	// 着地した時に呼べれる
	bool OnLanding();

	// ジャンプ処理
	void JumpVelocity(float speed);
	// 着地した時に呼べれる
	
	//void MoveLocal(const DirectX::XMFLOAT3& direction, float elapsedTime);

	//// 旋回
	//void Turn(const DirectX::XMFLOAT3& direction, float elapsedTime);


	// 水平

private:
	int jumpCount = 0;

	DirectX::XMFLOAT3 velocity;

	float gravity;
};
