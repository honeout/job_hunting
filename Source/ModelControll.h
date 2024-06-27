#pragma once

#include "Component.h"
#include "Graphics\Model.h"

// ���f���R���g���[���[
class ModelControll : public Component
{
public:
    ModelControll();
    ~ModelControll() override;

    // ���O�擾
    const char* GetName() const override { return "Model"; }

    // �J�n����
    void Start() override;

    // �X�V����
    void Update(float elapsedTime) override;

    // GUI�\��
    void OnGUI() override;

    // ���f���̓ǂݍ���
    void LoadModel(const char* filename);

    // ���f���̎擾
    Model* GetModel() const { return model.get(); }
private:
    std::unique_ptr<Model>	model;
};