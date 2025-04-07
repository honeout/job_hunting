#pragma once

#include "Graphics/Model.h"
#include "Component.h"
#include "Collision.h"
#include "BulletFiring.h"
#include "ModelControll.h"
#include "Transform.h"
#include "Effect.h"

// ���i�e��
class ProjectileStraight : public Component
{
public:
    ProjectileStraight();
    ~ProjectileStraight()override;

    // ���O�擾
    const char* GetName() const override { return "ProjectileStraight"; }

    // �J�n����
    void Start() override;

    // �X�V���� 
    void Update(float elapsedTime) override;

    // �`�揈��
    void Render(RenderContext& rc, ModelShader& shader) override;

    // �����蔻�茩��
    void DrawDebugPrimitive();

    // �G�t�F�N�g�X�V
    void SetEffectProgress(const char* storageLocation)
    {
        effectProgress = new Effect(storageLocation);
    }

    // �G�t�F�N�g����
    void SetEffectSpawned(const char* storageLocation)
    {
        effectSpawned = new Effect(storageLocation);
    }


    void SetMovementCheck(bool movementCheck)
    {
        this->movementCheck = movementCheck;
    }
#ifdef _DEBUG
    // GUI�`��
    void OnGUI() override;
#endif // _DEBUG
private:
    Model* model = nullptr;
    Effect* effectProgress = nullptr;
    // �G�t�F�N�g������
    Effect* effectSpawned = nullptr;
    float  speed = 10.0f;// �P�b�ԂP�O���[�g��

    // �e�ې�������
    float   lifeTimer = 3.0f;

    float stepOffset = 1.0f;

    std::shared_ptr<BulletFiring> bulletFiring;
    std::shared_ptr<Transform> transform;

    float radius = 0.3f;

    bool movementCheck = true;
};
