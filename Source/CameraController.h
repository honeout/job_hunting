#pragma once

#include <DirectXMath.h>

// カメラコントローラー
class CameraController
{
public:
    CameraController() {}
    ~CameraController() {}

    // 更新処理
    void Update(float elasedTime);

    void RockUpdate(float elapsedTime, const DirectX::XMFLOAT3& cameraPosition);

    // ターゲット位置設定 ちゃんと変数を入れる
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

    // デバッグ用GUI描画
    void DrawDebugGUI();

    // 角度セット
    void SetAngle(DirectX::XMFLOAT3 angle) 
    {
        this->angle = angle;
    }

    // 角度Y軸セット
    void SetAngleY(float angleY)
    {
        this->angle.y = angleY;
    }

    void SetFrontAngle(DirectX::XMFLOAT3 frontAngle)
    {
        this->frontAngle = frontAngle;
    }

private:
    DirectX::XMFLOAT3       target = { 0, 0, 0 };
    DirectX::XMFLOAT3       angle = { 0, 0, 0 };
    float                   rollSpeed = DirectX::XMConvertToRadians(90);
    // 範囲
    float                   range = 10.0f;
    float                   maxAngleX = DirectX::XMConvertToRadians(35);
    float                   minAngleX = DirectX::XMConvertToRadians(6);

    DirectX::XMFLOAT3 frontAngle;
};

