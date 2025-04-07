#include "ProjectileImpact.h"
#include "ProjectileManager.h"
#include "Graphics/Graphics.h"
// コンストラクタ
ProjectileImpact::ProjectileImpact()
{
}
// デストラクタ
ProjectileImpact::~ProjectileImpact()
{
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

void ProjectileImpact::Start()
{
    // モデル一様
    model = GetActor()->GetComponent<ModelControll>()->GetModel();

    // トランスフォーム取得
    transform = GetActor()->GetComponent<Transform>();

    // コリジョン取得
    collision = GetActor()->GetComponent<Collision>();
    
    // 円の当たり判定内側
    radiusInSide = 0.1f;

    // 円の当たり判定外側
    radiusOutSide = 1.0f;

    // 当たり判定を共有
    //transform->SetRadius(radius);

    // 動き反映
    movementCheck = true;

    scale = 10.0f;
    effectProgress->SetScale(effectProgress->GetEfeHandle(), { scale,scale,scale });
   
    if (effectProgress)
        effectProgress->Play(transform->GetPosition(), scale);
}

// 更新処理
void ProjectileImpact::Update(float elapsedTime)
{
    if (lifeTimer <= 0)
    {
        Destoroy();
    }   
    ImpactUpdate();
    transform->UpdateTransformProjectile();
    model->UpdateTransform(transform->GetTransform());
    if (effectProgress)
        effectProgress->SetPosition(effectProgress->GetEfeHandle(), transform->GetPosition());

    if (effectHit)
        effectHit->SetPosition(effectHit->GetEfeHandle(), transform->GetPosition());
    --lifeTimer;
}

// 描画処理
void ProjectileImpact::Render(RenderContext& rc, ModelShader& shader)
{
    Graphics& graphics = Graphics::Instance();
    //Shader* shader = graphics.GetShader();
    //ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
    shader = *graphics.GetShader(ModelShaderId::Lanbert);
    shader.Begin(rc);// シェーダーにカメラの情報を渡す
    shader.Draw(rc, model);
    shader.End(rc);
}

void ProjectileImpact::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // 今は何も表示しない
        //// 衝突判定用のデバッグ球を描画
    debugRenderer->DrawSphere(transform->GetPosition(), radiusInSide, DirectX::XMFLOAT4(0, 0, 1, 1));
    debugRenderer->DrawSphere(transform->GetPosition(), radiusOutSide, DirectX::XMFLOAT4(0, 1, 0, 1));

    debugRenderer->DrawCylinder(transform->GetPosition(), 15, collision.lock()->GetHeight(), DirectX::XMFLOAT4(0, 1, 1, 1));
}

void ProjectileImpact::Destoroy()
{
    ActorManager::Instance().Remove(GetActor());
    ProjectileManager::Instance().Remove(GetActor());
}

void ProjectileImpact::ImpactUpdate()
{
    // 当たり判定増大
    radiusInSide += 0.13f;
    
    // 当たり判定増大
    radiusOutSide += 0.13f;
}

void ProjectileImpact::EffectProgressPlay()
{
    effectProgress->Play(transform->GetPosition(), scale);
}

void ProjectileImpact::EffectHitPlay(float elapsedTime)
{
    effectHit->Play(transform->GetPosition(), scale);
}

void ProjectileImpact::EffectProgressUpdate(float elapsedTime)
{
    effectProgress->SetPosition(effectProgress->GetEfeHandle(),
        transform->GetPosition());
}

void ProjectileImpact::EffectHitUpdate(float elapsedTime)
{
    effectHit->SetPosition(effectHit->GetEfeHandle(),
        transform->GetPosition());
}