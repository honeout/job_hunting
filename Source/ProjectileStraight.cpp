#include "ProjectileStraight.h"
#include "Collision.h"
#include "Graphics/Graphics.h"
// コンストラクタ
ProjectileStraight::ProjectileStraight()    
{
}
// デストラクタ
ProjectileStraight::~ProjectileStraight()
{
}
void ProjectileStraight::Start()
{
    // モデル一様
    model = GetActor()->GetComponent<ModelControll>()->GetModel();

    // 当たり判定を共有
    GetActor()->GetComponent<Transform>()->SetRadius(radius);
    
    // 銃移動のコンポーネント
    bulletFiring = GetActor()->GetComponent<BulletFiring>();

    // トランスフォーム取得
    transform = GetActor()->GetComponent<Transform>();
}
// 更新処理
void ProjectileStraight::Update(float elapsedTime)
{
    // 寿命まで飛ぶ
    bulletFiring->Move(speed,elapsedTime);

    transform->UpdateTransformProjectile();

    model->UpdateTransform(transform->GetTransform());
}

// 描画
void ProjectileStraight::Render(RenderContext& rc, ModelShader& shader)
{
    Graphics& graphics = Graphics::Instance();
    shader = *graphics.GetShader(ModelShaderId::Lanbert);
    shader.Begin(rc);// シェーダーにカメラの情報を渡す
    shader.Draw(rc, model);
    shader.End(rc);
}

void ProjectileStraight::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // 今は何も表示しない
        //// 衝突判定用のデバッグ球を描画
    debugRenderer->DrawSphere(transform->GetPosition(), radius, DirectX::XMFLOAT4(0, 0, 1, 1));
}
#ifdef _DEBUG
void ProjectileStraight::OnGUI()
{

}
#endif // _DEBUG



