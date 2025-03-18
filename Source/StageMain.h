#pragma once
#include "Graphics/Model.h"
#include "Collision.h"
#include "Component.h"
#include "ModelControll.h"
#include "Transform.h"

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

    // �`�揈��
    void Render(RenderContext& rc, ModelShader& shader) override;

    // �V���h�E�}�b�v����
    void RenderShadowmap(RenderContext& rc) override;

    // ���C�L���X�g
    bool RayCast(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        HitResult& hit) ;
    // �X�y�L�����[
    void SetIsSpecular(int isSpecular) { this->isSpecular = isSpecular; }
    // �����x���グ��
    void SetTexcoordMult(int texcoordMult) { this->texcoordMult = texcoordMult; }
    // �������C�g
    void SetIsRimRightning(int isRimRightning) { this->isRimRightning = isRimRightning; }

private:
    Model* model = nullptr;
    std::shared_ptr<Transform> transformid;
    // �X�y�L�����[������
    int isSpecular = 0;
    // �����x���グ��B
    int texcoordMult = 0;
    // �������C�g
    int isRimRightning = 1;
};

// �X�e�[�W�}�l�[�W���[
class StageManager
{
private:
    StageManager() {}
    ~StageManager() {};

public:
    // �C���X�^���X�擾
    static StageManager& Instance()
    {
        static StageManager instance;
        return instance;
    }

    // �o�^
    void Register(std::shared_ptr <Actor> actor);

    // �폜
    void Remove(std::shared_ptr<Actor> stage);

    void Clear();

    // �X�e�[�W���擾
    int GetStageCount() const { return static_cast<int>(stages.size()); }

    // �G�l�~�[�擾
    std::shared_ptr <Actor> GetStage(int index) { return stages.at(index); }

private:
    std::vector<std::shared_ptr<Actor>>  stages;
    // �폜�\��
    std::set<std::shared_ptr<Actor>>removes;
};
