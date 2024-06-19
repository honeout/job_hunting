#pragma once


#include "Graphics/Model.h"

#include "Stage.h"

#include "Component.h"


// �X�e�[�W
class StageMain : public Component
{
public:

    StageMain();
    ~StageMain() override;

    // ���O�擾
    const char* GetName() const override { return "StageMain"; }

    // �J�n����
    void Start() override;

    // �X�V����
    void Update(float elapsedTime) override;

    //// �`�揈��
    //void Render(const RenderContext& rc, ModelShader* shader) override;

    // ���C�L���X�g
    bool RayCast(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        HitResult& hit) ;

    //// ���f���擾
    //Model* GetModel() const { return model.get(); }

private:
    Model*	model;
};
