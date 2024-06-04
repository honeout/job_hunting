#pragma once

#include <DirectXMath.h>
#include "Component.h"

// �ړ�
class HP : public Component
{
public:
	HP();
	virtual ~HP();

	// ���O�擾
	const char* GetName() const override { return "HP"; }

	// GUI�`��
	void OnGUI() override;


	// �_���[�W��^���� �_���[�W���󂯂��瑼�ɓn���B
	bool ApplyDamage(int damage, float invincibleTime);

	//// HP��Ԃ��擾
	//int GetHealth() const { return health; }

	// �_���[�W���󂯂����ɌĂ΂��
	bool OnDamaged();
	// ���S�������ɌĂ΂��
	bool OnDead();
private:
	// HP
	int          health = 5;

	// �ő�lHP
	//int          maxHealth = 5;
	// ���G����
	float   invincibleTimer = 1.0f;
};
