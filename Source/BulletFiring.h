#pragma once
#include <DirectXMath.h>
#include "Collision.h"

// 移動
class BulletFiring : public Component
{
public:
    // 移動
    void Move(float speed, float elapsedTime);
    // 発射    (  どっちからはっしゃ向き、位置何処から)
    void Lanch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float   lifeTimer);

    // 破棄
    void Destroy();

    // 名前取得
    const char* GetName() const override { return "BulletFiring"; }

    // GUI描画
    void OnGUI() override;

private:
    // 位置
    DirectX::XMFLOAT3 position = { 0,0,0 };
    // 向き
    DirectX::XMFLOAT3 direction = { 0,0,0 };
    // 弾丸生命時間
    float   lifeTimer = 3.0f;

};