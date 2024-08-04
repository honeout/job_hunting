#pragma once

#include "Graphics//Model.h"
#include "Projectile.h"
#include "Component.h"
#include "BulletFiring.h"
#include "ModelControll.h"
#include "Transform.h"
// 追尾弾丸
class ProjectileHoming :public Component
{
public:
    ProjectileHoming();
    ~ProjectileHoming() override;

    // 名前取得
    const char* GetName() const override { return "ProjectileHoming"; }

    // 開始処理
    void Start() override;

    // 更新処理
    virtual void Update(float elapsedTime) override;

    // 描画処理
    void Render(RenderContext& rc, ModelShader& shader) override;

    void DrawDebugPrimitive();

    // 描画処理
    //virtual void Render(const RenderContext& rc, ModelShader* shader) override;

    //// 発射    (  どっちからはっしゃ向き、位置何処から)　targetに向かってホーミングスピード
    //void Lanch(const DirectX::XMFLOAT3& direction,
    //           const DirectX::XMFLOAT3& position,
    //           const DirectX::XMFLOAT3& target);

    void SetTarget(DirectX::XMFLOAT3 target) { this->target = target; }
private:
    Model* model = nullptr;
    DirectX::XMFLOAT3 target = { 0,0,0 };
    // 弾丸生命時間
    float     lifeTimer = 3.0f;
    // 進速度
    float          moveSpeed = 10.0f;
    // 回転の速度
    float         turnSpeed = DirectX::XMConvertToRadians(180);// 

    std::shared_ptr<BulletFiring> bulletFiring;
    std::shared_ptr<Transform> transform;

    float radius = 0.3f;

    

};