#pragma once

#include "Graphics/Model.h"
#include "Projectile.h"
#include "Component.h"
#include "BulletFiring.h"


// 直進弾丸
class ProjectileStraight : public Component
{
public:
  /*  ProjectileStraight();*/
    ProjectileStraight();
    ~ProjectileStraight()override;

    // 名前取得
    const char* GetName() const override { return "ProjectileStraight"; }

    // 開始処理
    void Start() override;

    // 更新処理 
    void Update(float elapsedTime) override;

    // 描画処理
    //void Render(const RenderContext& rc, ModelShader* shader)override;

    //// 発射    (  どっちからはっしゃ向き、位置何処から)
    //void Lanch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);

    //// 反射弾丸更新処理
    //void UpdateReflection(float elapsedTime);

private:
    Model* model = nullptr;
    float  speed = 10.0f;// １秒間１０メートル


    // 弾丸生命時間
    float   lifeTimer = 3.0f;

    float stepOffset = 1.0f;

    std::shared_ptr<BulletFiring> bulletFiring;

    float radius = 0.3f;

};
