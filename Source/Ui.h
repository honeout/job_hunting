#pragma once
#include "Component.h"
#include "SpriteControll.h"
#include "TransForm2D.h"

class Ui : public Component
{
public:
    Ui() {};
    ~Ui() override;

    // ���O�擾
    const char* GetName() const override { return "Ui"; }

    // �J�n����
    void Start() override;

    // �X�V����
    void Update(float elapsedTime) override;

    // �`�揈��
    void Render2D(RenderContext& rc, SpriteShader& shader) override;
#ifdef _DEBUG
    // GUI�`��
    void OnGUI() override;
#endif // _DEBUG

    // �㏸
    void IncrementToAlpha(float increment);

    // ����
    void UiTimeUpdate();

    // �`��m�F
    void SetDrawCheck(bool drawCheck) { this->drawCheck = drawCheck; }

    // �`��m�F
    bool GetDrawCheck() const { return drawCheck; }

    // �Z�b�g���ԕb�ǂ�
    void SetTimeMax(int timeMax) { this->timeMax = timeMax; }

    // �Q�b�g���ԕb�ǂ�
    int GetTimeMax() const { return timeMax; }

    // �Q�b�g���ԂP�t���[��
    int GetTime() const { return time; }

    void SetCountDown(int countDown) { this->countDown = countDown; }

    void SetColor(DirectX::XMFLOAT4 color) { this->color = color; }
    DirectX::XMFLOAT4 GetColor() { return color; }

private:
    Sprite* sprite = nullptr;

    // �ʒu�⓮��
    std::shared_ptr<TransForm2D> transForm2D;


    // �`��̐؂�ւ��悤
    bool       drawCheck = false;

    // �P�b
    int        time = 60;
    // �������ԕb�ǂ�
    int        timeMax = 0;

    // �J�E���g�_�E��
    int        countDown = 0;

    DirectX::XMFLOAT4 color = { 1,1,1,1 };

    float alphaMax = 1.0f;
};