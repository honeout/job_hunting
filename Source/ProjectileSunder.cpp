#include "ProjectileSunder.h"
#include "Graphics/Graphics.h"
#include "MagicConfig.h"

// コンストラクタ
ProjectileSunder::ProjectileSunder()
{
}
// デストラクタ
ProjectileSunder::~ProjectileSunder()
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

void ProjectileSunder::Start()
{
    // コンポーネントを使えるように
    bulletFiring = GetActor()->GetComponent<BulletFiring>();
    transform = GetActor()->GetComponent<Transform>();
    modelControll = GetActor()->GetComponent<ModelControll>();

    // Lockとして実体を使う
    auto bulletFiringId = bulletFiring.lock();
    auto transformId = transform.lock();
    auto modelControllId = modelControll.lock();


    // 有効性チェック
    if (!bulletFiringId || !transformId || !modelControllId)
        return;

    // 当たり判定を共有
    radius = MagicConfig::kRadius;

    if (effectProgress)
        effectProgress->Play(transformId->GetPosition(), 1);
}

// 更新処理
void ProjectileSunder::Update(float elapsedTime)
{
    // コンポーネントを使えるように
    bulletFiring = GetActor()->GetComponent<BulletFiring>();
    transform = GetActor()->GetComponent<Transform>();
    modelControll = GetActor()->GetComponent<ModelControll>();

    // Lockとして実体を使う
    auto bulletFiringId = bulletFiring.lock();
    auto transformId = transform.lock();
    auto modelControllId = modelControll.lock();

    // 有効性チェック
    if (!bulletFiringId || !transformId || !modelControllId)
        return;

    if (movementCheck)
        bulletFiringId->Sunder( target, elapsedTime);

    transformId->UpdateTransformProjectile();

    modelControllId->GetModel()->UpdateTransform(transformId->GetTransform());

    if (effectProgress)
        effectProgress->SetPosition(effectProgress->GetEfeHandle(), transformId->GetPosition());

    if (effectHit)
        effectHit->SetPosition(effectHit->GetEfeHandle(), transformId->GetPosition());
}

// 描画処理
void ProjectileSunder::Render(RenderContext& rc, ModelShader& shader)
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

void ProjectileSunder::DrawDebugPrimitive()
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
    debugRenderer->DrawSphere(transformId->GetPosition(), radius, DirectX::XMFLOAT4(0, 0, 1, 1));
}

void ProjectileSunder::EffectProgressPlay()
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

void ProjectileSunder::EffectHitPlay(float elapsedTime)
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

void ProjectileSunder::EffectProgressUpdate(float elapsedTime)
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

void ProjectileSunder::EffectHitUpdate(float elapsedTime)
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
