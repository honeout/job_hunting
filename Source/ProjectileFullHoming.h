#pragma once
#pragma once
#include "Graphics//Model.h"
#include "Component.h"
#include "BulletFiring.h"
#include "ModelControll.h"
#include "Transform.h"
#include "Effect.h"


// 追尾弾丸
class ProjectileFullHoming :public Component
{
public:
    ProjectileFullHoming();
    ~ProjectileFullHoming() override;

    // 名前取得
    const char* GetName() const override { return "ProjectileFullHoming"; }

    // 開始処理
    void Start() override;

    // 更新処理
    virtual void Update(float elapsedTime) override;

    // 描画処理
    void Render(RenderContext& rc, ModelShader& shader) override;

    void DrawDebugPrimitive();

    void SetTarget(DirectX::XMFLOAT3 target) { this->target = target; }

    // 回転速度
    void SetTurnSpeed(float turnSpeed) { this->turnSpeed = turnSpeed; }

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
    }

    void SetMagicNumber(int magicNumber) { this->magicNumber = magicNumber; }

    int GetMagicNumber() { return magicNumber; }

public:
    enum class MagicNumber
    {
        Unnorne = -1,
        Fire,
        Thander,
        Ice,
    };

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
    //float         turnSpeed = DirectX::XMConvertToRadians(1);// 
    float         turnSpeed = DirectX::XMConvertToRadians(180);// 

    std::shared_ptr<BulletFiring> bulletFiring;
    std::shared_ptr<Transform> transform;

    float radius = 0.3f;

    float scale = 1.0f;

    bool movementCheck = true;

    int magicNumber = (int)MagicNumber::Thander;
};