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
  /*  int health = actor->GetHealth();*/

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
        return true;
    }
    // �_���[�W�ʒm
    else
    {
        OnDamaged();
        return true;
    }

    //actor->SetHealth(health);

    // ���N��Ԃ��ύX�����ꍇ��true��Ԃ�
    return false;
}



bool HP::OnDamaged()
{
    --health;
    return true;
}

bool HP::OnDead()
{
    ActorManager::Instance().Remove(GetActor());
    return true;
}
