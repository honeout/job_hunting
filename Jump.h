#pragma once

#include <DirectXMath.h>
#include "Component.h"

// ˆÚ“®
class Jump : public Component
{
public:
	Jump();
	virtual ~Jump();

	// –¼‘Oæ“¾
	const char* GetName() const override { return "Movement"; }

	// GUI•`‰æ
	void OnGUI() override;

	// ˆÚ“®
	bool OnLanding();
	//void MoveLocal(const DirectX::XMFLOAT3& direction, float elapsedTime);

	//// ù‰ñ
	//void Turn(const DirectX::XMFLOAT3& direction, float elapsedTime);

private:
	int jumpCount = 0;

	DirectX::XMFLOAT3 velocity;

	float gravity;
};
