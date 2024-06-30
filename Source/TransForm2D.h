#pragma once
#include "Component.h"

class TransForm2D : public Component
{
public:
    TransForm2D() {};
    ~TransForm2D() override;

    // 名前取得
    const char* GetName() const override { return "TransForm2D"; }

    // GUI描画
    void OnGUI() override;

    // 位置の設定
    void SetPosition(const DirectX::XMFLOAT2& position) { this->position = position; }

    // 位置の取得
    const DirectX::XMFLOAT2& GetPosition() const { return position; }

    // 角度の設定
    void SetAngle(const float& angle) {  this->angle = angle; }

    // 角度の設定
    const float& GetAngle() const { return angle; }

    // スケールの設定
    void SetScale(const DirectX::XMFLOAT2& scale) { this->scale = scale; }

    // スケールの取得
    const DirectX::XMFLOAT2& GetScale() const { return scale; }

private:

    // 画像の位置
    DirectX::XMFLOAT2 position = {0,0};
    // 角度
    float angle = 0;
    // 画像の大きさ
    DirectX::XMFLOAT2 scale = {0,0};



};