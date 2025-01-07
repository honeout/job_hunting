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


#ifdef _DEBUG
	// GUI�`��
	void OnGUI() override;
#endif // _DEBUG

	// ���G
	void UpdateInbincibleTimer(float elapsedTime);


	// �_���[�W��^���� �_���[�W���󂯂��瑼�ɓn���B
	bool ApplyDamage(int damage, float invincibleTime);


	bool DamageDrawCheck();



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

	bool GetDead() const { return dead; }

	void SetDead(bool dead) { this->dead = dead; }

	// �c�@
	void SetLife(int life) { this->life = life; }
	// �c�@
	int GetLife() const { return life; }

	bool HealthPinch();

	// ���G���Ԓ�
	bool InvincibleTimerCheck();

	// �_���[�W�����擾
	bool FlashTime(float elapsedTime);

	// ���G����
	void SetInvincibleTimer(float invincibleTimer) { this->invincibleTimer = invincibleTimer; }

	// �����ԂȂ��Ƀ_���[�W����萔�ȏ�H�������
	bool CheckDamageThresholdWithinTime(float elapsedTime,float damageThreshold, float timeLimit);

	void ResetOnDamageThresholdTime();

	//  �_���[�W���ݒ肳�ꂽ���E�𒴂�������
	bool GetIsOverDamageRule() { return isOverDamageRule; }
	void SetIsOverDamageRule(bool isOverDamageRule) { this->isOverDamageRule = isOverDamageRule; }

	// �ǉ�HP�Ȃ��Ă邩�ǂ���
	//void SetIsBonusHpActive(bool isBonusHpActive) { this->isBonusHpActive = isBonusHpActive; }

	// �ϋv�ǉ�
	void SetIsBonusHpActive(bool isBonusHpActive);

	// �ϋv�͒ǉ�����Ă邩�ǂ���
	bool GetIsBonusHpActive() { return isBonusHpActive; }

private:
	// �ő�lHP
	int          maxHealth = 5;

	// HP
	int          health = maxHealth;

	// ���G����
	float   invincibleTimer = 0.0f;

	// ����
	bool dead = false;

	// �c�@
	int life = 0;

	// �ǉ��̑ϋv��
	int bonusHp = 0;
	int bonusHpEnd = 0;
	bool isBonusHpActive = false;

	// �_���[�W����
	bool onDamage = false;

	// �o�ߎ���
	float stateTimerMax = 30.0f;
	float stateTimer = stateTimerMax;

	// ���]�p
	int blinkingTimeMax = 60;
	int blinkingTime = blinkingTimeMax;

	// �_���[�W�A�����ȏ�� ����
	float escapeOnFixedDamageStart = 0.0f;
	float escapeOnFixedDamage = escapeOnFixedDamageStart;
	float escapeOnFixedDamageMax = 10;

	// �_���[�W���̌o�ߎ���
	float damageThresholdStartTime = 0.0f;
	float damageThresholdTime = damageThresholdStartTime;

	// �_���[�W���ݒ肳�ꂽ���E�𒴂�������
	bool isOverDamageRule = false;

};
