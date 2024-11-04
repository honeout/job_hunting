#include "ProjectileThrowing.h"
#include "Graphics/Graphics.h"
// コンストラクタ
ProjectileThrowing::ProjectileThrowing()
{
    //model = new Model("Data/Model/Sword/Sword.mdl");

    //// モデルが小さいのでスケーリング
    //scale.x = scale.y = scale.z = 3.0f;
}
// デストラクタ
ProjectileThrowing::~ProjectileThrowing()
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
    counterCheck = false;
}

void ProjectileThrowing::Start()
{
    // モデル一様
    model = GetActor()->GetComponent<ModelControll>()->GetModel();

    //// 当たり判定を共有
    //GetActor()->GetComponent<Transform>()->SetRadius(radius);

    // 銃移動のコンポーネント
    bulletFiring = GetActor()->GetComponent<BulletFiring>();

    // トランスフォーム取得
    transform = GetActor()->GetComponent<Transform>();

    // 動き反映
    //movementCheck = true;

    if (effectProgress)
        effectProgress->Play(transform->GetPosition(), 1);

    radius = transform->GetRadius();
}

// 更新処理
void ProjectileThrowing::Update(float elapsedTime)
{
    if (movementCheck)
        bulletFiring->Move(moveSpeed,elapsedTime);
        //bulletFiring->MoveHoming(moveSpeed, turnSpeed, target, elapsedTime);

    transform->UpdateTransformProjectile();

    model->UpdateTransform(transform->GetTransform());

    if (effectProgress)
        effectProgress->SetPosition(effectProgress->GetEfeHandle(), transform->GetPosition());

    if (effectHit)
        effectHit->SetPosition(effectHit->GetEfeHandle(), transform->GetPosition());

    //DrawDebugPrimitive();
}

// 描画処理
void ProjectileThrowing::Render(RenderContext& rc, ModelShader& shader)
{
    Graphics& graphics = Graphics::Instance();
    //Shader* shader = graphics.GetShader();
    //ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
    shader = *graphics.GetShader(ModelShaderId::Lanbert);
    shader.Begin(rc);// シェーダーにカメラの情報を渡す


    shader.Draw(rc, model);

    shader.End(rc);
}

void ProjectileThrowing::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // 今は何も表示しない
        //// 衝突判定用のデバッグ球を描画
    debugRenderer->DrawSphere(transform->GetPosition(), radius, DirectX::XMFLOAT4(0, 1, 1, 1));
    /*debugRenderer->DrawSphere(
        { 
            transform->GetPosition().x,
            transform->GetPosition().y,
            transform->GetPosition().z + transform->GetHeight()
        },
        radius, DirectX::XMFLOAT4(0, 1, 1, 1));

    debugRenderer->DrawSphere(
        {
            transform->GetPosition().x,
            transform->GetPosition().y,
            transform->GetPosition().z - transform->GetHeight()
        },
        radius, DirectX::XMFLOAT4(0, 1, 1, 1));*/
    //debugRenderer->DrawCylinder(transform->GetPosition(), radius, transform->GetHeight(), DirectX::XMFLOAT4(1, 0, 0, 1));
}
#ifdef _DEBUG
void ProjectileThrowing::OnGUI()
{
    ImGui::InputFloat("Radius", &radius);
}
#endif // _DEBUG
void ProjectileThrowing::EffectProgressPlay()
{
    effectProgress->Play(transform->GetPosition(), scale);
}

void ProjectileThrowing::EffectHitPlay(float elapsedTime)
{
    effectHit->Play(transform->GetPosition(), scale);
}

void ProjectileThrowing::EffectProgressUpdate(float elapsedTime)
{
    effectProgress->SetPosition(effectProgress->GetEfeHandle(),
        transform->GetPosition());
}

void ProjectileThrowing::EffectHitUpdate(float elapsedTime)
{
    effectHit->SetPosition(effectHit->GetEfeHandle(),
        transform->GetPosition());
}
//
//void ProjectileThrowing::Lanch(const DirectX::XMFLOAT3& direction,
//    const DirectX::XMFLOAT3& position,
//    const DirectX::XMFLOAT3& target)
//{
//    this->direction = direction;
//    this->position = position;
//    this->target = target;
//
//    //UpdateTransform();
//}
