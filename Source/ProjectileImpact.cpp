#include "ProjectileImpact.h"
#include "ProjectileManager.h"
#include "Graphics/Graphics.h"
// コンストラクタ
ProjectileImpact::ProjectileImpact()
{
    //model = new Model("Data/Model/Sword/Sword.mdl");

    //// モデルが小さいのでスケーリング
    //scale.x = scale.y = scale.z = 3.0f;
}
// デストラクタ
ProjectileImpact::~ProjectileImpact()
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

void ProjectileImpact::Start()
{
    // モデル一様
    model = GetActor()->GetComponent<ModelControll>()->GetModel();


    // 銃移動のコンポーネント
    //bulletFiring = GetActor()->GetComponent<BulletFiring>();

    // トランスフォーム取得
    transform = GetActor()->GetComponent<Transform>();
    
    // 円の当たり判定内側
    radiusInSide = 0.1f;

    // 円の当たり判定外側
    radiusOutSide = 1.0f;

    // 当たり判定を共有
    //transform->SetRadius(radius);

    // 動き反映
    movementCheck = true;

    if (effectProgress)
        effectProgress->Play(transform->GetPosition(), 10.0f);


}

// 更新処理
void ProjectileImpact::Update(float elapsedTime)
{
    if (lifeTimer <= 0)
    {
        Destoroy();
    }

    if (movementCheck)
    {
        //bulletFiring->MoveHoming(moveSpeed, turnSpeed, target, elapsedTime);
        ImpactUpdate();
    }
    transform->UpdateTransformProjectile();

    model->UpdateTransform(transform->GetTransform());

    if (effectProgress)
        effectProgress->SetPosition(effectProgress->GetEfeHandle(), transform->GetPosition());

    if (effectHit)
        effectHit->SetPosition(effectHit->GetEfeHandle(), transform->GetPosition());

    //DrawDebugPrimitive();

    --lifeTimer;

    //// 寿命処理
    //lifeTimer -= elapsedTime;
    //if (lifeTimer <= 0.0f)// 寿命が尽きたら自害
    //{
    //    // 自分を削除
    //    Destroy();
    //}

    //// 移動
    //{
    //    float moveSpeed = this->moveSpeed * elapsedTime;
    //    // 　だからかけ算で向きにこれだけ進だから位置に入れた
    //    // 伸ばした分だけ＋＝で進
    //    position.x += direction.x*moveSpeed;
    //    position.y += direction.y*moveSpeed;
    //    position.z += direction.z*moveSpeed;
    //}

    //// 旋回
    //{
    //    float turnSpeed = this->turnSpeed * elapsedTime;

    //    // ターゲットまでのベクトルを算出
    //    DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
    //    DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
    //    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);

    //    // ゼロベクトルでないなら回転処理　ピッタリ同じなら回転できるから確認
    //    DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
    //    float lengthSq;
    //    DirectX::XMStoreFloat(&lengthSq, LengthSq);
    //   
    //    if (lengthSq > 0.00001f)
    //    {

    //        // ターゲットまでのベクトルを単位ベクトル化
    //        Vec = DirectX::XMVector3Normalize(Vec);

    //        // 向いてる方向ベクトルを算出　direction単位ベクトル前提
    //        DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);

    //        // 前方方向ベクトルとターゲットまでのベクトルの内積（角度）を算出
    //        DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);
    //  


    //        float dot;
    //        DirectX::XMStoreFloat(&dot, Dot);

    //        // 回転速度調整最後の微調整行き過ぎないように少しずつ小さく出来る。
    //        // アークコサインでも出来るしかも一瞬でその値を入れる。
    //        // 2つの単位ベクトルの角度が小さい程
    //        // 1.0に近づくという性質を利用して回転速度を調整する
    //        //if (dot )
    //        float rot;
    //        // 1.0f　dotは０に近づく程１になるからーくとこっちも０になる
    //        rot = 1.0f - dot;
    //        // だから１のほうがでかければスピードを入れる
    //        // ターンスピードよりロットの方が小さい時に
    //        if (rot > turnSpeed)
    //        {
    //            rot = turnSpeed;
    //        }
    //       

    //        // 回転角度があるなら回転処理する　ここで０を取ってないと外積が全く同じになって計算出来ない
    //        if (fabsf(rot) >= 0.0001)
    //        {
    //            // 回転軸を算出  外積  向かせたい方を先に 
    //            DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction, Vec);
    //            // 誤差防止の為に単位ベクトルした方が安全
    //            Axis =  DirectX::XMVector3Normalize(Axis);
    //            // 回転軸と回転量から回転行列を算出 回転量を求めている。
    //            DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);
    //            // 現在の行列を回転させる　自分自身の姿勢
    //            DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
    //            Transform = DirectX::XMMatrixMultiply(Transform,Rotation) ; // 同じだからただ×だけ  Transform*Rotation
    //            // DirectX::XMMatrixMultrixMultiply
    //            // 回転後の前方方向を取り出し、単位ベクトル化する
    //            Direction = DirectX::XMVector3Normalize(Transform.r[2]);// row
    //            DirectX::XMStoreFloat3(&direction,Direction);

    //        }
    //    }
    //}

    //        // オブジェクト行列を更新
    //        UpdateTransform();

    //        // モデル行列更新
    //        model->UpdateTransform(transform);
    //        

    //        


}

// 描画処理
void ProjectileImpact::Render(RenderContext& rc, ModelShader& shader)
{
    //Graphics& graphics = Graphics::Instance();
    ////Shader* shader = graphics.GetShader();
    ////ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
    //shader = *graphics.GetShader(ModelShaderId::Lanbert);
    //shader.Begin(rc);// シェーダーにカメラの情報を渡す


    //shader.Draw(rc, model);

    //shader.End(rc);
}

void ProjectileImpact::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // 今は何も表示しない
        //// 衝突判定用のデバッグ球を描画
    debugRenderer->DrawSphere(transform->GetPosition(), radiusInSide, DirectX::XMFLOAT4(0, 0, 1, 1));
    debugRenderer->DrawSphere(transform->GetPosition(), radiusOutSide, DirectX::XMFLOAT4(0, 1, 0, 1));

    debugRenderer->DrawCylinder(transform->GetPosition(), 15, transform->GetHeight(), DirectX::XMFLOAT4(0, 1, 1, 1));
}

void ProjectileImpact::Destoroy()
{
    ActorManager::Instance().Remove(GetActor());
    ProjectileManager::Instance().Remove(GetActor());
}

void ProjectileImpact::ImpactUpdate()
{


    // 当たり判定増大
    radiusInSide += 0.3f;
    
    // 当たり判定増大
    radiusOutSide += 0.3f;


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