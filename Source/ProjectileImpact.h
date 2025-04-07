#pragma once

#include "Graphics//Model.h"
#include "Component.h"
#include "BulletFiring.h"
#include "ModelControll.h"
#include "Transform.h"
#include "Effect.h"
// 追尾弾丸
class ProjectileImpact :public Component
{
public:
    ProjectileImpact();
    ~ProjectileImpact() override;

    // 名前取得
    const char* GetName() const override { return "ProjectileHoming"; }

    // 開始処理
    void Start() override;

    // 更新処理
    virtual void Update(float elapsedTime) override;

    // 描画処理
    void Render(RenderContext& rc, ModelShader& shader) override;

    void DrawDebugPrimitive();

    void Destoroy();

    // 当たり判定衝撃波
    void ImpactUpdate();

    void SetTarget(DirectX::XMFLOAT3 target) { this->target = target; }

    void SetLifeTimer(float lifeTimer) { this->lifeTimer = lifeTimer; }

    // エフェクト更新
    void SetEffectProgress(const char* storageLocation)
    {
        effectProgress = new Effect(storageLocation);
    }

    // エフェクト終了時
    void SetEffectHit(const char* storageLocation)
    {
        effectHit = new Effect(storageLocation);
    }

    // エフェクト開始
    void EffectProgressPlay();

    // エフェクト終了開始
    void EffectHitPlay(float elapsedTime);

    // エフェクト更新
    void EffectProgressUpdate(float elapsedTime);

    // エフェクト終了更新
    void EffectHitUpdate(float elapsedTime);

    void SetMovementCheck(bool movementCheck)
    {
        this->movementCheck = movementCheck;
    };

    float GetRadiusOutSide() const { return radiusOutSide; }
    float GetRadiusInSide() const { return radiusInSide; }

private:
    Model* model = nullptr;
    Effect* effectProgress = nullptr;
    Effect* effectHit = nullptr;

    DirectX::XMFLOAT3 target = { 0,0,0 };
    // 弾丸生命時間
    float     lifeTimer = 3.0f;
    // 進速度
    float          moveSpeed = 10.0f;
    // 回転の速度
    float         turnSpeed = DirectX::XMConvertToRadians(180);// 

    std::shared_ptr<BulletFiring> bulletFiring;
    std::shared_ptr<Transform> transform;
    std::weak_ptr<Collision> collision;

    // 衝撃波起こる範囲外側
    float radiusOutSide = 0.3f;

    // 衝撃波起こる範囲内側
    float radiusInSide = 0.3f;

    float scale = 10.0f;

    bool movementCheck = false;
};