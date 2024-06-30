#pragma once

#include "Graphics/Model.h"
#include "Projectile.h"
#include "Component.h"
#include "BulletFiring.h"
#include "ModelControll.h"
#include "Transform.h"

// ���i�e��
class ProjectileStraight : public Component
{
public:
  /*  ProjectileStraight();*/
    ProjectileStraight();
    ~ProjectileStraight()override;

    // ���O�擾
    const char* GetName() const override { return "ProjectileStraight"; }

    // �J�n����
    void Start() override;

    // �X�V���� 
    void Update(float elapsedTime) override;

    // �`�揈��
    void Render(RenderContext rc) override;

    void OnGUI() override;

    // �`�揈��
    //void Render(const RenderContext& rc, ModelShader* shader)override;

    //// ����    (  �ǂ�������͂���������A�ʒu��������)
    //void Lanch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

    //// ���˒e�ۍX�V����
    //void UpdateReflection(float elapsedTime);

private:
    Model* model = nullptr;
    float  speed = 10.0f;// �P�b�ԂP�O���[�g��


    // �e�ې�������
    float   lifeTimer = 3.0f;

    float stepOffset = 1.0f;

    std::shared_ptr<BulletFiring> bulletFiring;
    std::shared_ptr<Transform> transform;

    float radius = 0.3f;

};
