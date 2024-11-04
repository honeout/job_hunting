#pragma once
#include "Component.h"

class TransForm2D : public Component
{
public:
    TransForm2D() {};
    ~TransForm2D() override;

    // ���O�擾
    const char* GetName() const override { return "TransForm2D"; }
#ifdef _DEBUG
    // GUI�`��
    void OnGUI() override;
#endif // _DEBUG

    // �h��
    void Shake();
    // �h�炷�͈͍ő�
    void SetUiMax(int max) { this->max = max; }
    // �h�炷�͈͍ŏ�
    void SetUiMin(int min) { this->min = min; }

    // �ʒu�̐ݒ�
    void SetPosition(const DirectX::XMFLOAT2& position) { this->position = position; }

    // ���ʒu�̐ݒ�
    void SetTexPosition(const DirectX::XMFLOAT2& texPosition) { this->texPosition = texPosition; }

    // �ʒu�̎擾
    const DirectX::XMFLOAT2& GetPosition() const { return position; }

    // ���ʒu�̎擾
    const DirectX::XMFLOAT2& GetTexPosition() const { return texPosition; }

    // �p�x�̐ݒ�
    void SetAngle(const float& angle) {  this->angle = angle; }

    // �p�x�̐ݒ�
    const float& GetAngle() const { return angle; }

    // �X�P�[���̐ݒ�
    void SetScale(const DirectX::XMFLOAT2& scale) { this->scale = scale; }

    // ���X�P�[���̐ݒ�
    void SetTexScale(const DirectX::XMFLOAT2& texScale) { this->texScale = texScale; }

    // �X�P�[���̎擾
    const DirectX::XMFLOAT2& GetScale() const { return scale; }

    // ���X�P�[���̎擾
    const DirectX::XMFLOAT2& GetTexScale() const { return texScale; }

    // �h�ꎞ�ԍő�
    void SetShakeTimeMax(int shakeTimeUiMax) { this->shakeTimeUiMax = shakeTimeUiMax; }
   
    // �h��o�ߎ��ԕύX
    void SetShakeTime(int shakeTimeUi) { this->shakeTimeUi = shakeTimeUi; }

    // ���ԏI�����ǂ���
    bool GetShakeEnd() { return shakeTimeUi >= shakeTimeUiMax ? true : false; }

private:

    // �摜�̈ʒu
    DirectX::XMFLOAT2 position = {0,0};

    // �摜�̌��ʒu
    DirectX::XMFLOAT2 texPosition = { 0,0 };

    // �p�x
    float angle = 0;
    // �摜�̑傫��
    DirectX::XMFLOAT2 scale = {0,0};

    // �摜�̌��̑傫��
    DirectX::XMFLOAT2 texScale = { 0,0 };

    bool checkPositionY = false;

    // UI�h�炷�͈�
    int max = 0;
    int min = 0;

    // UI��h�炷���߂̎���
    int shakeTimeUi = 0;
    // UI��h�炷���ԍő�
    int shakeTimeUiMax = 0;

    

};