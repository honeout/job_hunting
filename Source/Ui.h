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

    // GUI�`��
    void OnGUI() override;

    // �`��m�F
    void SetDrawCheck(bool drawCheck) { this->drawCheck = drawCheck; }

    // �`��m�F
    bool GetDrawCheck() const { return drawCheck; }

private:
    Sprite* sprite = nullptr;

    // �ʒu�⓮��
    std::shared_ptr<TransForm2D> transForm2D;


    // �`��̐؂�ւ��悤
    bool       drawCheck = false;
};