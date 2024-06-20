#include <imgui.h>
#include "BulletFiring.h"

void BulletFiring::Move(float speed,float elapsedTime)
{
    //�@�������� 
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)// �������s�����玩�Q
    {
        // �������폜
        Destroy();
    }
    // �ړ��@�@�b����Ȃ��ƃt���[����
    float bulletspeed = speed * elapsedTime;

    // �@�����炩���Z�Ō����ɂ��ꂾ���i������ʒu�ɓ��ꂽ
    position.x += bulletspeed * direction.x;
    position.y += bulletspeed * direction.y;
    position.z += bulletspeed * direction.z;

    GetActor()->SetPosition(position);
    

    // ����
    //UpdateReflection(elapsedTime);

    // �I�u�W�F�N�g�s����X�V�@�ς�����ʒu�̍s���
    //UpdateTransform();

    // ���f���s��X�V�@����������炱���Ɍ�����
    //model->UpdateTransform(transform);
}

void BulletFiring::Lanch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float   lifeTimer)
{
    // direction �����@���K�����ē���鍷���Ȃ���X�s�[�h�ς��
    this->direction = direction;
    this->position = position;
    this->lifeTimer = lifeTimer;
}

void BulletFiring::Destroy()
{
    ActorManager::Instance().Remove(GetActor());
}

void BulletFiring::OnGUI()
{
    ImGui::InputFloat3("Move Speed", &position.x);
    ImGui::InputFloat3("Turn Speed", &direction.x);
    ImGui::InputFloat("lifeTimer", &lifeTimer);

}
