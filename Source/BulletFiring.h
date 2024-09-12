#pragma once
#include <DirectXMath.h>
#include "Collision.h"
#include "Transform.h"
// 移動
class BulletFiring : public Component
{
public:
    // 移動
    void Move(float speed, float elapsedTime);

    // 追跡移動
    void MoveHoming(float speed,float turnSpeed,DirectX::XMFLOAT3 target, float elapsedTime);
    
    // ルビー下から飛び出し
    void JumpOut(float speed, float turnSpeed, DirectX::XMFLOAT3 target, float elapsedTime);
    
    // 発射    (  どっちからはっしゃ向き、位置何処から)
    void Lanch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float   lifeTimer);

    // 破棄
    void Destroy();

    // 名前取得
    const char* GetName() const override { return "BulletFiring"; }

    // GUI描画
    void OnGUI() override;

private:
    std::shared_ptr<Transform> transformid;
    // 位置
    DirectX::XMFLOAT3 position = { 0,0,0 };
    // 向き
    DirectX::XMFLOAT3 direction = { 0,0,0 };
    // 弾丸生命時間
    float   lifeTimer = 3.0f;

    Model* model;
};