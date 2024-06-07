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


	// ‰Á‘¬
	float acceleration = 1.0f;
	// speedÅ‘å’l
	float maxMoveSpeed = 5.0f;

	// –€C
	float   friction = 0.5f;

	// ŒXÎ—¦ 
	float slopeRate = 1.0f;
};
