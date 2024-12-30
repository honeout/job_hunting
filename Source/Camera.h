#pragma once

#include<DirectXMath.h>
#include "Actor.h"

// カメラ
class Camera
{
private:// シングルトンにするためにコンストラクタをプライベートに
    Camera() {}
    ~Camera() {}
public:
    // 唯一のインスタンス取得
    // ほんとはカメラは複数ある。
    static Camera& Instance()
    {
        // カメラは様々な所で取り出したいだからシングルトンにする
        static Camera camera;
        return camera;
    }

    // 指定方向を向く
    void SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up);

    // パースペクティブ設定
    void SetPerspedtiveFov(float forY, float aspect, float nearZ, float farZ);

    // ビュー行列取得
    const DirectX::XMFLOAT4X4& GetView() const { return view; }
    
    // プロジェクション行列取得
    const DirectX::XMFLOAT4X4& GetProjection() const { return projection; }

    // 視点取得
    const DirectX::XMFLOAT3& GetEye() const { return eye; }

    // 注視点取得
    const DirectX::XMFLOAT3& GetFocus() const { return focus; }

    // 上方向取得
    const DirectX::XMFLOAT3& GetUp() const { return up;  }

    // 前方向取得
    const DirectX::XMFLOAT3& GetFront() const { return front; }

    // 右方向取得
    const DirectX::XMFLOAT3& GetRight() const { return right; }


private:
    DirectX::XMFLOAT4X4     view = {
    0.0f,0.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f,0.0f };
    DirectX::XMFLOAT4X4     projection = {
    0.0f,0.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f,0.0f,
    0.0f,0.0f,0.0f,0.0f };

    DirectX::XMFLOAT3       eye = {0.0f,0.0f,0.0f};
    DirectX::XMFLOAT3       focus = {0.0f,0.0f,0.0f};

    DirectX::XMFLOAT3       up = {0.0f,0.0f,0.0f};
    DirectX::XMFLOAT3       front = {0.0f,0.0f,0.0f};
    DirectX::XMFLOAT3       right = {0.0f,0.0f,0.0f};
};
