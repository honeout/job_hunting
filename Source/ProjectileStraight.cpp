#include "ProjectileStraight.h"
#include "Collision.h"
#include "Graphics/Graphics.h"
#include "MagicConfig.h"

// コンストラクタ
ProjectileStraight::ProjectileStraight()    
{
}
// デストラクタ
ProjectileStraight::~ProjectileStraight()
{
    // エフェクト更新削除
    if (effectProgress)
    {
        effectProgress->Stop(effectProgress->GetEfeHandle());
        delete effectProgress;
        effectProgress = nullptr;
    }

    if (effectSpawned)
    {
        effectSpawned->Stop(effectSpawned->GetEfeHandle());
        delete effectSpawned;
        effectSpawned = nullptr;
    }
}
void ProjectileStraight::Start()
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

    if (effectSpawned)
        effectSpawned->Play(transformId->GetPosition());
}
// 更新処理
void ProjectileStraight::Update(float elapsedTime)
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

    // 寿命まで飛ぶ
    bulletFiringId->Move(speed,elapsedTime);

    transformId->UpdateTransformProjectile();

    modelControllId->GetModel()->UpdateTransform(transformId->GetTransform());


    if (effectProgress)
        effectProgress->SetPosition(effectProgress->GetEfeHandle(), transformId->GetPosition());
}

// 描画
void ProjectileStraight::Render(RenderContext& rc, ModelShader& shader)
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

void ProjectileStraight::DrawDebugPrimitive()
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
#ifdef _DEBUG
void ProjectileStraight::OnGUI()
{

}
#endif // _DEBUG



