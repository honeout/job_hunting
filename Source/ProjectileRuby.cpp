#include "ProjectileRuby.h"
#include "Graphics/Graphics.h"
// コンストラクタ
ProjectileRuby::ProjectileRuby()
{
    //model = new Model("Data/Model/Sword/Sword.mdl");

    //// モデルが小さいのでスケーリング
    //scale.x = scale.y = scale.z = 3.0f;
}
// デストラクタ
ProjectileRuby::~ProjectileRuby()
{
    //delete model;

    // エフェクト更新削除
    if (effectProgress)
    {
        effectProgress->Stop(effectProgress->GetEfeHandle());
        delete effectProgress;
        effectProgress = nullptr;
    }

    // エフェクトヒット削除
    if (effectHit)
    {
        effectHit->Stop(effectHit->GetEfeHandle());
        delete effectHit;
        effectHit = nullptr;
    }
}

void ProjectileRuby::Start()
{
    // モデル一様
    model = GetActor()->GetComponent<ModelControll>()->GetModel();

    // 当たり判定を共有
    GetActor()->GetComponent<Transform>()->SetRadius(radius);

    // 銃移動のコンポーネント
    bulletFiring = GetActor()->GetComponent<BulletFiring>();

    // トランスフォーム取得
    transform = GetActor()->GetComponent<Transform>();

    // 動き反映
    //movementCheck = true;

    if (effectProgress)
        effectProgress->Play(transform->GetPosition(), 1);
}

// 更新処理
void ProjectileRuby::Update(float elapsedTime)
{
    if (movementCheck)
        bulletFiring->JumpOut(moveSpeed, turnSpeed, target, elapsedTime);

    transform->UpdateTransformProjectile();

    model->UpdateTransform(transform->GetTransform());

    DrawDebugPrimitive();

    if (effectProgress)
        effectProgress->SetPosition(effectProgress->GetEfeHandle(), transform->GetPosition());

    if (effectHit)
        effectHit->SetPosition(effectHit->GetEfeHandle(), transform->GetPosition());

}

// 描画処理
void ProjectileRuby::Render(RenderContext& rc, ModelShader& shader)
{
    Graphics& graphics = Graphics::Instance();
    //Shader* shader = graphics.GetShader();
    //ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
    shader = *graphics.GetShader(ModelShaderId::Lanbert);
    shader.Begin(rc);// シェーダーにカメラの情報を渡す


    shader.Draw(rc, model);

    shader.End(rc);
}

void ProjectileRuby::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // 今は何も表示しない
        //// 衝突判定用のデバッグ球を描画
    //debugRenderer->DrawSphere(transform->GetPosition(), radius, DirectX::XMFLOAT4(0, 0, 1, 1));
    debugRenderer->DrawCylinder(transform->GetPosition(), radius, transform->GetHeight(), DirectX::XMFLOAT4(0, 0, 1, 1));
}

void ProjectileRuby::EffectProgressPlay()
{
    effectProgress->Play(transform->GetPosition(), scale);
}

void ProjectileRuby::EffectHitPlay(float elapsedTime)
{
    effectHit->Play(transform->GetPosition(), scale);
}

void ProjectileRuby::EffectProgressUpdate(float elapsedTime)
{
    effectProgress->SetPosition(effectProgress->GetEfeHandle(),
        transform->GetPosition());
}

void ProjectileRuby::EffectHitUpdate(float elapsedTime)
{
    effectHit->SetPosition(effectHit->GetEfeHandle(),
        transform->GetPosition());
}

