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

	// ���G
	void UpdateInbincibleTimer(float elapsedTime);


	// �_���[�W��^���� �_���[�W���󂯂��瑼�ɓn���B
	bool ApplyDamage(int damage, float invincibleTime);

	// HP��Ԃ��擾
	int GetHealth() const { return health; }

	// HP��˂�����
	void SetHealth(int health) {
		this->health = health;
	}

	// �ő�HP�����o��
	int GetMaxHealth() const { return maxHealth; }

	// �ő�HP��˂�����
	void SetMaxHealth(int maxHealth)  { this->maxHealth = maxHealth; }

	// �_���[�W���󂯂����ɌĂ΂��
	bool OnDamaged();
	// ���S�������ɌĂ΂��
	bool OnDead();
private:
	// �ő�lHP
	int          maxHealth = 5;

	// HP
	int          health = maxHealth;

	// ���G����
	float   invincibleTimer = 1.0f;
};
