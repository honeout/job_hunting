#include <algorithm>
#include <imgui.h>
#include "Math.h"
#include "Movement.h"
#include "HP.h"

// �R���X�g���N�^
HP::HP()
{
    dead = false;
}

// �f�X�g���N�^
HP::~HP()
{
}
#ifdef _DEBUG
// GUI�`��
void HP::OnGUI()
{
	ImGui::SliderInt("Hp", &health,1,maxHealth);
	ImGui::SliderInt("bonusHp", &bonusHp,0,maxHealth);
}
#endif // _DEBUG
void HP::UpdateInbincibleTimer(float elapsedTime)
{
    if (invincibleTimer > invincibleTimerMin)
    {
        invincibleTimer -= elapsedTime;
    }
}

bool HP::ApplyDamage(int damage, float invincibleTime)
{
    // �q�b�g�������ǂ���
    onDamage = false;
    // �_���[�W���O�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
    if (damage == damageEmpty) return false;
    std::weak_ptr<Actor> actor = GetActor();
    // ���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
    if (health <= healthEmpty)return false;
    if (invincibleTimer > invincibleTimerMin)return false;
    // ���b���G
    invincibleTimer = invincibleTime;
    // �ʏ��_��
    if(!isBonusHpActive)
    // �_���[�W����
    health -= damage;
    // �ϋv�̓_��
    else
    {
        bonusHp -= damage;
    }
    // ���_���[�W��
    escapeOnFixedDamage += damage;

    // ���C�t�ő�l��������B
    if (health <= healthEmpty)
    {
        --life;
    }
    // ���S�ʒm
    if (life <= lifeEmpth)
    {
        //OnDead();
        dead = true;
        return true;
    }
    // �_���[�W�ʒm
    else
    {
        onDamage = true;
        return true;
    }
    // �ǉ�HP
    if (bonusHp <= bonusHpEnd)
    {
        isBonusHpActive = false;
    }
    
    // ���N��Ԃ��ύX�����ꍇ��true��Ԃ�
    return false;
}


// �ϋv�ǉ�
void HP::SetIsBonusHpActive(bool isBonusHpActive)
{
    this->isBonusHpActive = isBonusHpActive;
    if (this->isBonusHpActive)
    {
        // �ǉ�HP
        bonusHp = maxHealth;
    }
    else
    {
        bonusHp = bonusHpEnd;
    }
}
void HP::AddHealth(int health)
{
    this->health += health;
    if (this->health >= maxHealth)
    {
        this->health = maxHealth;
    }
}

bool HP::OnDamaged()
{
    return onDamage;
}

bool HP::OnDead()
{
    ActorManager::Instance().Remove(GetActor());
    return true;
}

bool HP::HealthPinch()
{
    if (health <= (maxHealth / 4))
        return true;
    
    return false;
}

bool HP::InvincibleTimerCheck()
{
    if (invincibleTimer > invincibleTimerMin)
        return true;

        return false;
}

bool HP::FlashTime(float elapsedTime)
{
    if (onDamage)
    {
        blinkingTime = blinkingTimeMin;
    }
    onDamage = false;
    // �`�F���W����
    if (blinkingTime < blinkingTimeMax)
    {
        ++blinkingTime;
        return true;
    }
    return false;
}
// ��_����萔
bool HP::CheckDamageThresholdWithinTime(float elapsedTime,  float damageThreshold, float timeLimit)
{
    damageThresholdTime += elapsedTime;
    // �ő�l
    escapeOnFixedDamageMax = damageThreshold;
    // �o�ߎ��Ԉ�莞�ԉz��
    if (damageThresholdTime > timeLimit)
    {
        damageThresholdTime = damageThresholdStartTime;
        return false;
    }
    // �����B��
    if (escapeOnFixedDamage > escapeOnFixedDamageMax)
    {
        // �_���[�W������
        escapeOnFixedDamage = escapeOnFixedDamageStart;

        // ���ԏ�����
        damageThresholdTime = damageThresholdStartTime;

        // �_���[�W�����ȏ�m�F
        isOverDamageRule = true;
        return true;
    }
    return false;
}
// ��_�����
void HP::ResetOnDamageThresholdTime()
{
    // ��_���`���[�W������
    escapeOnFixedDamage = escapeOnFixedDamageStart;
    // �o�ߎ��ԋ����I��
    damageThresholdTime = damageThresholdStartTime;
}
