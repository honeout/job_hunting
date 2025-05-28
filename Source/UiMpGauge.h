#pragma once
#include "Component.h"
#include "SpriteControll.h"
#include "TransForm2D.h"

class UiMpGauge : public Component
{
public:
    UiMpGauge() {};
    ~UiMpGauge() override;

    // ���O�擾
    const char* GetName() const override { return "UiHp"; }

    // �J�n����
    void Start() override;

    // �X�V����
    void Update(float elapsedTime) override;

    // �`�揈��
    void Render2D(RenderContext& rc, SpriteShader& shader) override;

    // hp�Q�[�W����
    void MpGaugeUpdate();
#ifdef _DEBUG
    // GUI�`��
    void OnGUI() override;
#endif // _DEBUG

    void SetColor(DirectX::XMFLOAT4 color) { this->color = color; }
    DirectX::XMFLOAT4 GetColor() { return color; }

private:
    // �`��̐؂�ւ��悤
    bool       drawCheck = false;

    // �P�b
    int        time = 60;
    // �������ԕb�ǂ�
    int        timeMax = 0;

    // �J�E���g�_�E��
    int        countDown = 0;

    DirectX::XMFLOAT4 color = { 1,1,1,1 };

    DirectX::XMFLOAT2 scale = { 0,0 };

    float alphaMax = 1.0f;
};
