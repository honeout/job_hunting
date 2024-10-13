#pragma once


#include "Component.h"

class Transform : public Component
{
public:
    Transform();
    ~Transform() override;

    // ���O�擾
    const char* GetName() const override { return "Transform"; }
    // �s��̍X�V
    void UpdateTransform();

    // �e�ۃz�[�~���O���̌v�Z�����₷�����邽��
    void UpdateTransformProjectile();

    // GUI�`��
    void OnGUI() override;

    // �ʒu�̐ݒ�
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    // �ʒu�̎擾
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    // �A���O���̐ݒ�
    void SetAngle(DirectX::XMFLOAT3 angle) { this->angle = angle; }

    // �A���O���̎擾
    DirectX::XMFLOAT3 GetAngle() { return this->angle; }

    // �X�P�[���̐ݒ�
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    // �X�P�[���̎擾
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    // �s��̎擾
    const DirectX::XMFLOAT4X4& GetTransform() const { return transform; }

    // �s��̐ݒ�
    void SetTransform(const DirectX::XMFLOAT4X4 transform) { this->transform = transform; }

    DirectX::XMFLOAT3 GetDirection() { return direction; }
    void SetDirection(DirectX::XMFLOAT3 direction) { this->direction = direction; } 

    // ���a�@
    float GetRadius() { return radius; }
    void  SetRadius(float radius) { this->radius = radius; }

    // ����
    float GetHeight() { return height; }
    void  SetHeight(float height) { this->height = height; }
private:

    // �s�񒆐g
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 angle;
    DirectX::XMFLOAT3 scale;

    // �s��
    DirectX::XMFLOAT4X4	transform = DirectX::XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

    // ����
    DirectX::XMFLOAT3 direction;

    // ���a05 
    float radius = 0.5f;

    // ����
    float height = 1;
};