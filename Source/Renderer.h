#pragma once
#pragma once

#include <DirectXMath.h>
#include "Component.h"

// �ړ�
class Rendere : public Component
{
public:
	Rendere();
	virtual ~Rendere();

	// ���O�擾
	const char* GetName() const override { return "Render"; }

	// GUI�`��
	void OnGUI() override;

	// ���a�擾 05
	float GetRadius() const { return radius; }

	// �_���[�W��^���� �_���[�W���󂯂��瑼�ɓn���B
	bool DrawDebugPrimitive();

private:
	// ���a05
	float radius = 0.5f;

};
