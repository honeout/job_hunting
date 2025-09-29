#include "ProjectileImpact.h"
#include "ProjectileManager.h"
#include "Graphics/Graphics.h"
#include "MagicConfig.h"

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
    // コンポーネントを使えるように
    transform = GetActor()->GetComponent<Transform>();
    modelControll = GetActor()->GetComponent<ModelControll>();
    collision = GetActor()->GetComponent<Collision>();

    // Lockとして実体を使う
    auto transformId = transform.lock();
    auto modelControllId = modelControll.lock();
    auto collisionControllId = collision.lock();

    // 有効性チェック
    if (!transformId || !modelControllId || !collisionControllId)
        return;
    
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
        effectProgress->Play(transformId->GetPosition(), scale);
}

// 更新処理
void ProjectileImpact::Update(float elapsedTime)
{
    // コンポーネントを使えるように
    transform = GetActor()->GetComponent<Transform>();
    modelControll = GetActor()->GetComponent<ModelControll>();

    // Lockとして実体を使う
    auto transformId = transform.lock();
    auto modelControllId = modelControll.lock();

    // 有効性チェック
    if (!transformId || !modelControllId)
        return;

    if (lifeTimer <= 0)
    {
        Destoroy();
    }   
    ImpactUpdate(elapsedTime);
    transformId->UpdateTransformProjectile();
    modelControllId->GetModel()->UpdateTransform(transformId->GetTransform());
    if (effectProgress)
        effectProgress->SetPosition(effectProgress->GetEfeHandle(), transformId->GetPosition());

    if (effectHit)
        effectHit->SetPosition(effectHit->GetEfeHandle(), transformId->GetPosition());
    --lifeTimer;
}

// 描画処理
void ProjectileImpact::Render(RenderContext& rc, ModelShader& shader)
{
    // コンポーネントを使えるように
    modelControll = GetActor()->GetComponent<ModelControll>();

    // Lockとして実体を使う
    auto modelControllId = modelControll.lock();

    // 有効性チェック
    if (!modelControllId)
        return;

    Graphics& graphics = Graphics::Instance();
    shader = *graphics.GetShader(ModelShaderId::Lanbert);
    shader.Begin(rc);// シェーダーにカメラの情報を渡す
    shader.Draw(rc, modelControllId->GetModel());
    shader.End(rc);
}

void ProjectileImpact::DrawDebugPrimitive()
{
    // コンポーネントを使えるように
    transform = GetActor()->GetComponent<Transform>();

    // Lockとして実体を使う
    auto transformId = transform.lock();

    // 有効性チェック
    if (!transformId)
        return;

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // 今は何も表示しない
        //// 衝突判定用のデバッグ球を描画
    debugRenderer->DrawSphere(transformId->GetPosition(), radiusInSide, DirectX::XMFLOAT4(0, 0, 1, 1));
    debugRenderer->DrawSphere(transformId->GetPosition(), radiusOutSide, DirectX::XMFLOAT4(0, 1, 0, 1));

    debugRenderer->DrawCylinder(transformId->GetPosition(), 15, MagicConfig::kHeight, DirectX::XMFLOAT4(0, 1, 1, 1));
}

void ProjectileImpact::Destoroy()
{
    ActorManager::Instance().Remove(GetActor());
    ProjectileManager::Instance().Remove(GetActor());
}

// 当たり判定衝撃波
void ProjectileImpact::ImpactUpdate(float elapsedTime)
{
    // 当たり判定増大
    radiusInSide += impactValue * elapsedTime;

    // 当たり判定増大
    radiusOutSide += impactValue * elapsedTime;
}

void ProjectileImpact::EffectProgressPlay()
{
    // コンポーネントを使えるように
    transform = GetActor()->GetComponent<Transform>();

    // Lockとして実体を使う
    auto transformId = transform.lock();

    // 有効性チェック
    if (!transformId)
        return;

    effectProgress->Play(transformId->GetPosition(), scale);
}

void ProjectileImpact::EffectHitPlay(float elapsedTime)
{
    // コンポーネントを使えるように
    transform = GetActor()->GetComponent<Transform>();

    // Lockとして実体を使う
    auto transformId = transform.lock();

    // 有効性チェック
    if (!transformId)
        return;

    effectHit->Play(transformId->GetPosition(), scale);
}

void ProjectileImpact::EffectProgressUpdate(float elapsedTime)
{
    // コンポーネントを使えるように
    transform = GetActor()->GetComponent<Transform>();

    // Lockとして実体を使う
    auto transformId = transform.lock();

    // 有効性チェック
    if (!transformId)
        return;

    effectProgress->SetPosition(effectProgress->GetEfeHandle(),
        transformId->GetPosition());
}

void ProjectileImpact::EffectHitUpdate(float elapsedTime)
{
    // コンポーネントを使えるように
    transform = GetActor()->GetComponent<Transform>();

    // Lockとして実体を使う
    auto transformId = transform.lock();

    // 有効性チェック
    if (!transformId)
        return;

    effectHit->SetPosition(effectHit->GetEfeHandle(),
        transformId->GetPosition());
}