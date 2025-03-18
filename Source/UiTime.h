#pragma once
#include "Component.h"
#include "SpriteControll.h"
#include "TransForm2D.h"

#define NUMBER_WIDTH    (64)
#define NUMBER_HEIGHT   (96)

class UiTime : public Component
{
public:
    UiTime() {};
    ~UiTime() override;

    // ���O�擾
    const char* GetName() const override { return "UiTime"; }

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

    void SetDigit(int digit) { this->digit = digit; }

    void SetScore(int score) { this->score = score; }

    void SetTimeUp(bool timeUp) { this->timeUp = timeUp; }

    bool GetTimeUp() { return this->timeUp; }

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

    // ��
    int        digit = 0;

    // ����
    int        score = 0;

    // �I���^�C�~���O
    bool       timeUp = false;
};
