#include "Camera.h"

// 指定方向を向く

void Camera::SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up)
{
    // 視点、　注視点、上方向からビュー行列を作成
    DirectX::XMVECTOR Eye = {eye.x,eye.y,eye.z};
    DirectX::XMVECTOR Focus = { focus.x,focus.y,focus.z};
    DirectX::XMVECTOR Up = { up.x,up.y,up.z};
    DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
    DirectX::XMStoreFloat4x4(&view, View);
    // まとめる

    // ビューを逆行列化し、ワールド行列に戻す  (nullptrほぼ絶対あるけど使わん,View)
    DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
    DirectX::XMFLOAT4X4 world;
    DirectX::XMStoreFloat4x4(&world, World);

    // カメラの方向を取り出す
    this->right.x = world._11;
    this->right.y = world._12;
    this->right.z = world._13;

    this->up.x = world._21;
    this->up.y = world._22;
    this->up.z = world._23;

    this->front.x = world._31;
    this->front.y = world._32;
    this->front.z = world._33;
    
    // 視点、注視点を保存
    this->eye = eye;
    this->focus = focus;
}
// ハースペクティブ設定
void Camera::SetPerspedtiveFov(float forY, float aspect, float nearZ, float farZ)
{
    // 画角、画面比率、クリップ距離からプロジェクト行列を作成
    DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(forY, aspect, nearZ, farZ);
    DirectX::XMStoreFloat4x4(&projection, Projection);
    //いつでもview projectionが使えるのが肝
}
// 角度を瞬時に変える
float Camera::MoveTowardsTargetRadian(float currentRadian, float targetRadian)
{
    // 差分計算
    float diff = targetRadian - currentRadian;

    // ラジアンを[-π、π]の範囲に正規化
    for (; diff > DirectX::XM_PI;)diff -= DirectX::XM_2PI;
    for (; diff < -DirectX::XM_PI;)diff += DirectX::XM_2PI;

    // 今回は一気に向かせる
    currentRadian += diff;

    // 2π以内に納める
    if (currentRadian < 0.0f)currentRadian += DirectX::XM_2PI;
    if (currentRadian >= DirectX::XM_PI)currentRadian -= DirectX::XM_2PI;

    return sinf(currentRadian);
}


