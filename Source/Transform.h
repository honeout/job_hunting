#pragma once


#include "Component.h"

class Transform : public Component
{
public:
    Transform();
    ~Transform() override;

    // 名前取得
    const char* GetName() const override { return "Transform"; }
    // 行列の更新
    void UpdateTransform();

    // 弾丸ホーミング時の計算をやりやすくするため
    void UpdateTransformProjectile();

    // GUI描画
    void OnGUI() override;

    // 位置の設定
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    // 位置の取得
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    // アングルの設定
    void SetAngle(DirectX::XMFLOAT3 angle) { this->angle = angle; }

    // アングルの取得
    DirectX::XMFLOAT3 GetAngle() { return this->angle; }

    // スケールの設定
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    // スケールの取得
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    // 行列の取得
    const DirectX::XMFLOAT4X4& GetTransform() const { return transform; }

    // 行列の設定
    void SetTransform(const DirectX::XMFLOAT4X4 transform) { this->transform = transform; }

    DirectX::XMFLOAT3 GetDirection() { return direction; }
    void SetDirection(DirectX::XMFLOAT3 direction) { this->direction = direction; } 

    // 半径　
    float GetRadius() { return radius; }
    void  SetRadius(float radius) { this->radius = radius; }

    // 高さ
    float GetHeight() { return height; }
    void  SetHeight(float height) { this->height = height; }
private:

    // 行列中身
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 angle;
    DirectX::XMFLOAT3 scale;

    // 行列
    DirectX::XMFLOAT4X4	transform = DirectX::XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

    // 向き
    DirectX::XMFLOAT3 direction;

    // 半径05 
    float radius = 0.5f;

    // 高さ
    float height = 1;
};