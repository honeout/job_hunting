#pragma once

#include "Graphics//Model.h"
#include "Component.h"
#include "BulletFiring.h"
#include "ModelControll.h"
#include "Transform.h"
#include "Effect.h"
// �ǔ��e��
class ProjectileSunder :public Component
{
public:
    ProjectileSunder();
    ~ProjectileSunder() override;

    // ���O�擾
    const char* GetName() const override { return "ProjectileHoming"; }

    // �J�n����
    void Start() override;

    // �X�V����
    virtual void Update(float elapsedTime) override;

    // �`�揈��
    void Render(RenderContext& rc, ModelShader& shader) override;

    void DrawDebugPrimitive();

    void SetTarget(DirectX::XMFLOAT3 target) { this->target = target; }

    // �G�t�F�N�g�X�V
    void SetEffectProgress(const char* storageLocation)
    {
        effectProgress = new Effect(storageLocation);
    }

    // �G�t�F�N�g�I����
    void SetEffectHit(const char* storageLocation)
    {
        effectHit = new Effect(storageLocation);
    }

    // �G�t�F�N�g�J�n
    void EffectProgressPlay();

    // �G�t�F�N�g�I���J�n
    void EffectHitPlay(float elapsedTime);

    // �G�t�F�N�g�X�V
    void EffectProgressUpdate(float elapsedTime);

    // �G�t�F�N�g�I���X�V
    void EffectHitUpdate(float elapsedTime);

    void SetMovementCheck(bool movementCheck)
    {
        this->movementCheck = movementCheck;
    };

private:
    Model* model = nullptr;
    Effect* effectProgress = nullptr;
    Effect* effectHit = nullptr;

    DirectX::XMFLOAT3 target = { 0,0,0 };
    // �e�ې�������
    float     lifeTimer = 3.0f;
    // �i���x
    float          moveSpeed = 10.0f;
    // ��]�̑��x
    float         turnSpeed = DirectX::XMConvertToRadians(180);// 

    std::shared_ptr<BulletFiring> bulletFiring;
    std::shared_ptr<Transform> transform;

    float radius = 0.3f;

    float scale = 1.0f;

    bool movementCheck = true;
};