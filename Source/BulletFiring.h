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

    // ��]
    bool Turn(float speed,const DirectX::XMFLOAT3& target, float elapedTime);

    // ������
    bool Turn2D(float speed, DirectX::XMFLOAT3& direction, float elapedTime);

    // �ǐՈړ�
    void MoveHoming(float speed,float turnSpeed,DirectX::XMFLOAT3 target, float elapsedTime);

    // �ǐՏ펞�ړ�
    void MoveFullHoming(float speed, float turnSpeed, DirectX::XMFLOAT3 target, float elapsedTime);

    // ���r�[�������яo��
    void JumpOut(float speed, float turnSpeed, DirectX::XMFLOAT3 target, float elapsedTime);
    
    // ���r�[�t�ɂ���
    void PushDown(float speed, float turnSpeed, float elapsedTime);

    // ��
    void Sunder(DirectX::XMFLOAT3 target,float elapsedTime);

    // ���I
    void Throwing(float speed, float turnSpeed, DirectX::XMFLOAT3 target, bool turnPermission, float elapsedTime);

    // ����    (  �ǂ�������͂���������A�ʒu��������)
    void Lanch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float   lifeTimer);

    // �j��
    void Destroy();

    // ���O�擾
    const char* GetName() const override { return "BulletFiring"; }
#ifdef _DEBUG
    // GUI�`��
    void OnGUI() override;
#endif // _DEBUG
private:
    // �ʒu
    DirectX::XMFLOAT3 position = { 0,0,0 };
    // ����
    DirectX::XMFLOAT3 direction = { 0,0,0 };
    // �e�ې�������
    float   lifeTimer = 3.0f;
    Model* model;
    float rangeMin = 0.01f;
    float rangeMax = 30;

    float rangeFullHoming = 10.0f;

    // �i�񂾋���
    float addLength = 0.0f;
    float lengthMax = 3.0f;
};