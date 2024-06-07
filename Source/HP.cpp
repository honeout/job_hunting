#include <algorithm>
#include <imgui.h>
#include "Math.h"
#include "Movement.h"
#include "HP.h"

// �R���X�g���N�^
HP::HP()
{

}

// �f�X�g���N�^
HP::~HP()
{

}

// GUI�`��
void HP::OnGUI()
{
	ImGui::InputInt("Hp", &health);
}

void HP::UpdateInbincibleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}

bool HP::ApplyDamage(int damage, float invincibleTime)
{
    // �_���[�W���O�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
    if (damage == 0) return false;

    std::shared_ptr<Actor> actor = GetActor();
    int health = actor->GetHealth();

    // ���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
    if (health <= 0)return false;

    if (this->invincibleTimer >= 0.0f)return false;

    // ���b���G
    this->invincibleTimer = invincibleTime;

    // �_���[�W����
    health -= damage;


    // ���S�ʒm
    if (health <= 0)
    {
        OnDead();
    }
    // �_���[�W�ʒm
    else
    {
        OnDamaged();
    }

    actor->SetHealth(health);

    // ���N��Ԃ��ύX�����ꍇ��true��Ԃ�
    return true;
}

bool HP::OnDamaged()
{
    return true;
}

bool HP::OnDead()
{
    --health;
    return true;
}
