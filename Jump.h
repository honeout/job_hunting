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

	// ���n�������ɌĂׂ��
	bool OnLanding();

	// �W�����v����
	void JumpVelocity(float speed);
	// ���n�������ɌĂׂ��
	
	//void MoveLocal(const DirectX::XMFLOAT3& direction, float elapsedTime);

	//// ����
	//void Turn(const DirectX::XMFLOAT3& direction, float elapsedTime);


	// ����

private:
	int jumpCount = 0;

	DirectX::XMFLOAT3 velocity;

	float gravity;
};
