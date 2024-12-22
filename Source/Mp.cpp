#include <algorithm>
#include <imgui.h>
#include "Math.h"
#include "Movement.h"
#include "Mp.h"

// �R���X�g���N�^
Mp::Mp()
{
    mpEmpth = false;
}

// �f�X�g���N�^
Mp::~Mp()
{

}
#ifdef _DEBUG
// GUI�`��
void Mp::OnGUI()
{
    ImGui::SliderInt("Mp", &magic, 1, maxMagic);
}
#endif // _DEBUG
bool Mp::UpdateInbincibleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
        return false;
    }

    invincibleTimer = imvincibleTimerMax;
    return true;
}

bool Mp::ApplyConsumption(int mpConsumption)
{
    // �_���[�W���O�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
    if (mpConsumption == 0) return false;

    std::weak_ptr<Actor> actor = GetActor();

      // ���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
    if (magic <= 0) 
    {
        mpEmpth = true;
        return false;
    }
    // ���͏���܂���
    else
    {
        // �_���[�W����
        magic -= mpConsumption;
        return true;
    }
}
// �`���[�W��
bool Mp::MpCharge(float elapedTime)
{


    if (magic <= 0)
    {
        mpEmpth = true;
     
    }
    if (magic >= maxMagic)
    {
        mpEmpth = false;
    }

    if (mpEmpth && UpdateInbincibleTimer(elapedTime))
    {
        magic += 1;
        return true;
    }


    
    return false;
}



bool Mp::Consumption()
{
    //--health;
    return true;
}

bool Mp::OnDead()
{
    ActorManager::Instance().Remove(GetActor());
    return true;
}