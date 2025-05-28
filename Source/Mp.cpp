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
    if (invincibleTimer > invincibleTimerMin)
    {
        invincibleTimer -= elapsedTime;
        return false;
    }
    invincibleTimer = imvincibleTimerMax;
    return true;
}

// mp����
bool Mp::ApplyConsumption(int mpConsumption)
{
    // ����ĂȂ�
    isConsumption = false;
    // mp����O�̏ꍇ�͕ύX����K�v���Ȃ�
    if (mpConsumption == mpValueMin) return false;
    std::weak_ptr<Actor> actor = GetActor();
      // �������Ȃ��ꍇ�͕ύX���Ȃ�
    if (magic <= mpMin)
    {
        mpEmpth = true;
        return false;
    }
    // ���͏���܂���
    else
    {
        magic -= mpConsumption;
        magic = magic <= mpMin ? mpMin : magic;
        mpEmpth = magic <= mpMin ? true : false;
        isConsumption = true;
        return true;
    }
}
// �`���[�W��
bool Mp::MpCharge(float elapedTime)
{
    // mp���Œ�l�ȉ��Ȃ�
    if (magic <= mpMin)
    {
        mpEmpth = true;
     
    }
    // mp�����܂���
    if (magic >= maxMagic)
    {
        mpEmpth = false;
    }
    if (mpEmpth && UpdateInbincibleTimer(elapedTime))
    {
        magic += mpValue;
        return true;
    }
    return false;
}

bool Mp::OnDead()
{
    ActorManager::Instance().Remove(GetActor());
    return true;
}