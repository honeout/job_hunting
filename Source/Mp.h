#pragma once
#include <DirectXMath.h>
#include "Component.h"

// �ړ�
class Mp : public Component
{
public:
	Mp();
	virtual ~Mp();

	// ���O�擾
	const char* GetName() const override { return "Mp"; }

#ifdef _DEBUG
	// GUI�`��
	void OnGUI() override;
#endif // _DEBUG

	// ���G
	bool UpdateInbincibleTimer(float elapsedTime);

	// �_���[�W��^���� �_���[�W���󂯂��瑼�ɓn���B
	bool ApplyConsumption(int damage);

	bool MpCharge(float elapsedTime);

	// HP��Ԃ��擾
	int GetMagic() const { return magic; }

	// HP��˂�����
	void SetMagic(int magic) {
		this->magic = magic;
	}

	// �ő�HP�����o��
	int GetMaxMagic() const { return maxMagic; }

	// �ő�HP��˂�����
	void SetMaxMagic(int maxMagic) { this->maxMagic = maxMagic; }

	// ���S�������ɌĂ΂��
	bool OnDead();

	bool GetMpEmpth() const { return mpEmpth; }

	void SetMpEmpth(bool mpEmpth) { this->mpEmpth = mpEmpth; }

private:
	// �ő�lHP
	int          maxMagic = 5;

	// Mp
	int          magic = maxMagic;

	// ���G����
	float   invincibleTimer = 0.3f;
	float   invincibleTimerMin = 0.0f;

	float   imvincibleTimerMax = 0.3f;

	int     mpValueMin = 0;
	int     mpMin = 0;

	int     mpValue = 1;

	// ����
	bool mpEmpth = false;
};

