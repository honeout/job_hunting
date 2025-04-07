#pragma once

#include "Graphics/Model.h"
#include "Component.h"
#include "Collision.h"
#include "BulletFiring.h"
#include "ModelControll.h"
#include "Transform.h"
#include "Effect.h"

// 直進弾丸
class ProjectileStraight : public Component
{
public:
    ProjectileStraight();
    ~ProjectileStraight()override;

    // 名前取得
    const char* GetName() const override { return "ProjectileStraight"; }

    // 開始処理
    void Start() override;

    // 更新処理 
    void Update(float elapsedTime) override;

    // 描画処理
    void Render(RenderContext& rc, ModelShader& shader) override;

    // 当たり判定見る
    void DrawDebugPrimitive();

    // エフェクト更新
    void SetEffectProgress(const char* storageLocation)
    {
        effectProgress = new Effect(storageLocation);
    }

    // エフェクト発生
    void SetEffectSpawned(const char* storageLocation)
    {
        effectSpawned = new Effect(storageLocation);
    }


    void SetMovementCheck(bool movementCheck)
    {
        this->movementCheck = movementCheck;
    }
#ifdef _DEBUG
    // GUI描画
    void OnGUI() override;
#endif // _DEBUG
private:
    Model* model = nullptr;
    Effect* effectProgress = nullptr;
    // エフェクト発生時
    Effect* effectSpawned = nullptr;
    float  speed = 10.0f;// １秒間１０メートル

    // 弾丸生命時間
    float   lifeTimer = 3.0f;

    float stepOffset = 1.0f;

    std::shared_ptr<BulletFiring> bulletFiring;
    std::shared_ptr<Transform> transform;

    float radius = 0.3f;

    bool movementCheck = true;
};
