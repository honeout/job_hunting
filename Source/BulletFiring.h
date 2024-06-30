#pragma once
#include <DirectXMath.h>
#include "Collision.h"
#include "Transform.h"
// ˆÚ“®
class BulletFiring : public Component
{
public:
    // ˆÚ“®
    void Move(float speed, float elapsedTime);

    // ’ÇÕˆÚ“®
    void MoveHoming(float speed,float turnSpeed,DirectX::XMFLOAT3 target, float elapsedTime);
    // ”­Ë    (  ‚Ç‚Á‚¿‚©‚ç‚Í‚Á‚µ‚áŒü‚«AˆÊ’u‰½ˆ‚©‚ç)
    void Lanch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float   lifeTimer);

    // ”jŠü
    void Destroy();

    // –¼‘Oæ“¾
    const char* GetName() const override { return "BulletFiring"; }

    // GUI•`‰æ
    void OnGUI() override;

private:
    std::shared_ptr<Transform> transformid;
    // ˆÊ’u
    DirectX::XMFLOAT3 position = { 0,0,0 };
    // Œü‚«
    DirectX::XMFLOAT3 direction = { 0,0,0 };
    // ’eŠÛ¶–½ŠÔ
    float   lifeTimer = 3.0f;

    Model* model;
};