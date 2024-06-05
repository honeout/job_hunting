#pragma once

#include <DirectXMath.h>
#include "Component.h"

// �ړ�
class Jump : public Component
{
public:
	Jump();
	virtual ~Jump();

	// ���O�擾
	const char* GetName() const override { return "Movement"; }

	// GUI�`��
	void OnGUI() override;

	// �ړ�
	bool OnLanding();
	//void MoveLocal(const DirectX::XMFLOAT3& direction, float elapsedTime);

	//// ����
	//void Turn(const DirectX::XMFLOAT3& direction, float elapsedTime);

private:
	int jumpCount = 0;

	DirectX::XMFLOAT3 velocity;

	float gravity;
};
