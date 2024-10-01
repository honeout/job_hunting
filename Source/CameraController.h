#pragma once

#include <DirectXMath.h>

// �J�����R���g���[���[
class CameraController
{
public:
    CameraController() {}
    ~CameraController() {}

    // �X�V����
    void Update(float elasedTime);

    void RockUpdate(float elapsedTime, const DirectX::XMFLOAT3& cameraPosition);

    // �^�[�Q�b�g�ʒu�ݒ� �����ƕϐ�������
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    // �f�o�b�O�pGUI�`��
    void DrawDebugGUI();

    // �p�x�Z�b�g
    void SetAngle(DirectX::XMFLOAT3 angle) 
    {
        this->angle = angle;
    }

    // �p�xY���Z�b�g
    void SetAngleY(float angleY)
    {
        this->angle.y = angleY;
    }

    void SetFrontAngle(DirectX::XMFLOAT3 frontAngle)
    {
        this->frontAngle = frontAngle;
    }

private:
    DirectX::XMFLOAT3       target = { 0, 0, 0 };
    DirectX::XMFLOAT3       angle = { 0, 0, 0 };
    float                   rollSpeed = DirectX::XMConvertToRadians(90);
    // �͈�
    float                   range = 10.0f;
    float                   maxAngleX = DirectX::XMConvertToRadians(35);
    float                   minAngleX = DirectX::XMConvertToRadians(6);

    DirectX::XMFLOAT3 frontAngle;
};

