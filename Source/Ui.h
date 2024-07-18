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
    void Render(RenderContext& rc, ModelShader& shader) override;

    // GUI�`��
    void OnGUI() override;
private:
    Sprite* sprite = nullptr;

    // �ʒu�⓮��
    std::shared_ptr<TransForm2D> transForm2D;
};