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

	// HP��Ԃ��擾
	int GetHealth() const { return health; }

	// HP��˂�����
	void SetHealth(int health) {
		this->health = health;
	}

	// HP��˂�����
	void AddHealth(int health);

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
	void SetLife(int life) { this->life += life; }

	// �c�@
	int GetLife() const { return life; }

	// HP��4/1�Ȃ�
	bool HealthPinch();

	// ���G���Ԓ�
	bool InvincibleTimerCheck();

	// �_���[�W�����擾
	bool FlashTime(float elapsedTime);

	// ���G����
	void SetInvincibleTimer(float invincibleTimer) { this->invincibleTimer = invincibleTimer; }

	// ��_����萔
	bool CheckDamageThresholdWithinTime(float elapsedTime,float damageThreshold, float timeLimit);

	// �_���[�W��萔���
	void ResetOnDamageThresholdTime();

	//  �_���[�W���ݒ肳�ꂽ���E�𒴂�������
	bool GetIsOverDamageRule() { return isOverDamageRule; }
	void SetIsOverDamageRule(bool isOverDamageRule) { this->isOverDamageRule = isOverDamageRule; }

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
	float   invincibleTimerMin = 0.0f;
	// ����
	bool dead = false;

	// �_���[�W����
	int damageEmpty = 0;
	int healthEmpty = 0;

	// �c�@
	int life = 0;
	int lifeEmpth = -1;

	// �ǉ��̑ϋv��
	int bonusHp = 0;
	// �ϋv����
	int bonusHpEnd = 0;
	// �ϋv�͂̒ǉ��A����
	bool isBonusHpActive = false;

	// �_���[�W����
	bool onDamage = false;

	// �o�ߎ���
	float stateTimerMax = 30.0f;
	float stateTimer = stateTimerMax;

	// ���]�p
	int blinkingTimeMax = 60;
	int blinkingTime = blinkingTimeMax;
	int blinkingTimeMin = 0;

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
