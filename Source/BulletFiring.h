#pragma once
#include <DirectXMath.h>
#include "Collision.h"

// �ړ�
class BulletFiring : public Component
{
public:
    // �ړ�
    void Move(float speed, float elapsedTime);
    // ����    (  �ǂ�������͂���������A�ʒu��������)
    void Lanch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float   lifeTimer);

    // �j��
    void Destroy();

    // ���O�擾
    const char* GetName() const override { return "BulletFiring"; }

    // GUI�`��
    void OnGUI() override;

private:
    // �ʒu
    DirectX::XMFLOAT3 position = { 0,0,0 };
    // ����
    DirectX::XMFLOAT3 direction = { 0,0,0 };
    // �e�ې�������
    float   lifeTimer = 3.0f;

};