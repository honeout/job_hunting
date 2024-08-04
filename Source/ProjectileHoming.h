#pragma once

#include "Graphics//Model.h"
#include "Projectile.h"
#include "Component.h"
#include "BulletFiring.h"
#include "ModelControll.h"
#include "Transform.h"
// �ǔ��e��
class ProjectileHoming :public Component
{
public:
    ProjectileHoming();
    ~ProjectileHoming() override;

    // ���O�擾
    const char* GetName() const override { return "ProjectileHoming"; }

    // �J�n����
    void Start() override;

    // �X�V����
    virtual void Update(float elapsedTime) override;

    // �`�揈��
    void Render(RenderContext& rc, ModelShader& shader) override;

    void DrawDebugPrimitive();

    // �`�揈��
    //virtual void Render(const RenderContext& rc, ModelShader* shader) override;

    //// ����    (  �ǂ�������͂���������A�ʒu��������)�@target�Ɍ������ăz�[�~���O�X�s�[�h
    //void Lanch(const DirectX::XMFLOAT3& direction,
    //           const DirectX::XMFLOAT3& position,
    //           const DirectX::XMFLOAT3& target);

    void SetTarget(DirectX::XMFLOAT3 target) { this->target = target; }
private:
    Model* model = nullptr;
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

    

};