#pragma once

#include <DirectXMath.h>
#include "Component.h"

// ˆÚ“®
class Movement : public Component
{
public:
	Movement();
	virtual ~Movement();

	// –¼‘Oæ“¾
	const char* GetName() const override { return "Movement"; }

	// GUI•`‰æ
	void OnGUI() override;

	// ˆÚ“®
	void Move(const DirectX::XMFLOAT3& direction, float elapsedTime);
	void MoveLocal(const DirectX::XMFLOAT3& direction, float elapsedTime);

	// ù‰ñ
	void Turn(const DirectX::XMFLOAT3& direction, float elapsedTime);

private:
	float		moveSpeed = 5.0f;
	float		turnSpeed = 6.28f;
};
