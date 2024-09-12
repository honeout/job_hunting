#pragma once
#include <DirectXMath.h>
#include "Collision.h"
#include "Transform.h"
// �ړ�
class BulletFiring : public Component
{
public:
    // �ړ�
    void Move(float speed, float elapsedTime);

    // �ǐՈړ�
    void MoveHoming(float speed,float turnSpeed,DirectX::XMFLOAT3 target, float elapsedTime);
    
    // ���r�[�������яo��
    void JumpOut(float speed, float turnSpeed, DirectX::XMFLOAT3 target, float elapsedTime);
    
    // ����    (  �ǂ�������͂���������A�ʒu��������)
    void Lanch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float   lifeTimer);

    // �j��
    void Destroy();

    // ���O�擾
    const char* GetName() const override { return "BulletFiring"; }

    // GUI�`��
    void OnGUI() override;

private:
    std::shared_ptr<Transform> transformid;
    // �ʒu
    DirectX::XMFLOAT3 position = { 0,0,0 };
    // ����
    DirectX::XMFLOAT3 direction = { 0,0,0 };
    // �e�ې�������
    float   lifeTimer = 3.0f;

    Model* model;
};