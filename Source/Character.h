#pragma once

#include <DirectXMath.h>
#include "Effect.h"
// キャラクター
class Character
{
public:
    Character() = default;
    virtual ~Character() = default;


    // 行列更新処理
    void UpdateTransform();

    // 位置取得
    const DirectX::XMFLOAT3& GetPosition() const { return position; }
    // 位置設定
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    // 回転取得
    const DirectX::XMFLOAT3& GetAngle() const { return angle; }

    // 回転設定
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    // スケール取得
    const DirectX::XMFLOAT3& GetSclae() const { return scale; }
    // スケール取得
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    //  はじき処理
    void AddImpulse(const DirectX::XMFLOAT3& impulse);

    // 半径取得 05
    float GetRadius() const { return radius; }

    // 地面に設置しているか エフェクト等も
    bool IsGround() const { return isGround; }

    // 高さ取得
    float GetHeight() const { return height; }

    // ダメージを与える ダメージを受けたら他に渡す。
    bool ApplyDamage(int damage, float invincibleTime);

    // HP状態を取得
    int GetHealth() const { return health; }

    // 最大HP状態を取得
    int GetMaxHealth() const { return maxHealth; }

    // アニメーションの時間
    float GetCurrentANimationSeconds() const { return currentANimationSeconds; }
    // アニメーションの時間
    void SetCurrentAnimationSeconds(const float currentANimationSeconds) { this->currentANimationSeconds = currentANimationSeconds; }

    // 行列の姿勢
    DirectX::XMFLOAT4X4 GetTransform() { return this->transform; }

    // 行列の姿勢をセット
    void SetTransform(DirectX::XMFLOAT4X4 transform) { this->transform = transform; }

    // 生存時間のセット
    void SetSurvivalTime(float survivaltime) {this->survivaltime = survivaltime;}

    // 減らす時間　生存時間じ
    void SetReduce(float reduce) {this->reduce = reduce;}

protected:
    // 移動処理
    void Move(float vx, float vz, float speed);

    // 旋回処理
    void Turn(float elapsedTime, float vx, float vz, float speed);

    // ジャンプ処理
    void Jump(float speed);

    // 速力処理更新
    void UpdateVelocity(float elapsedTime);
    void UpdateInbincibleTimer(float elapsedTime);

    // 着地した時に呼べれる
    virtual void OnLanding() {};

    // ダメージを受けた時に呼ばれる
    virtual void OnDamaged() {};
    // 死亡した時に呼ばれる
    virtual void OnDead() {};
private:
    // 垂直速力更新処理
    void UpdateVerticalVelocity(float elapsedFrame);

    // 垂直移動更新処理
    void UpdateVerticalMove(float elapsedTime);

    // 水平速力更新処理
    void UpdateHorizontalVelocity(float elapsedFrame);
    // 水平移動更新処理
    void UpdateHorizontalMove(float elapsedTime);

    // 反射弾丸更新処理
    void UpdateReflection(float elapsedTime);


protected:// 継承先でも使える
    DirectX::XMFLOAT3             position = { 0, 0, 0 };
    DirectX::XMFLOAT3             angle = { 0, 0, 0 };
    DirectX::XMFLOAT3             scale = { 1, 1, 1 };
    DirectX::XMFLOAT4X4 transform = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
    // 半径05
    float radius = 0.5f;

    // 重力
    float              grabity = -1.0f;
    // 速度
    DirectX::XMFLOAT3  velocity = { 0,0,0 };

    // 地面確認
    bool         isGround = false;
    // 身長メートル
    float               height = 2.0f;

    // HP
    int          health = 5;
    // 最大値HP
    int          maxHealth = 5;
    // 無敵時間
    float   invincibleTimer = 1.0f;
    // 摩擦
    float   friction = 0.5f;

    // 加速
    float acceleration = 1.0f;
    // speed最大値
    float maxMoveSpeed = 5.0f;
    // 移動方向Xと速度
    float moveVecX = 0.0f;
    // 移動方向Yと速度
    float moveVecZ = 0.0f;

    // 空中での移動速度減少
    float airControl = 0.3f;


    // 腰
    float stepOffset = 1.0f;
    // 傾斜率
    float slopeRate = 1.0f;

    // アニメーションの時間
    float currentANimationSeconds = 0.0f;

    // 生存時間
    float survivaltime = 0.0f;

    // 減らす秒数　生存時間時
    float reduce = 0;

    float alpha = 1;
};