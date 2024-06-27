#include <imgui.h>
#include "BulletFiring.h"
#include "ModelControll.h"

void BulletFiring::Move(float speed,float elapsedTime)
{
    //　寿命処理 
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)// 寿命が尽きたら自害
    {
        // 自分を削除
        Destroy();
    }
    // 移動　　秒じゃなくとフレームに
    float bulletspeed = speed * elapsedTime;

    // 　だからかけ算で向きにこれだけ進だから位置に入れた
    position.x += bulletspeed * direction.x;
    position.y += bulletspeed * direction.y;
    position.z += bulletspeed * direction.z;

    GetActor()->SetPosition(position);
    
    GetActor()->UpdateTransform();

    //GetActor()->GetModel()->UpdateAnimation(elapsedTime, true);


    model->UpdateTransform(GetActor()->GetTransform());
    //GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());
    // 反射
    //UpdateReflection(elapsedTime);

    // オブジェクト行列を更新　変わった位置の行列を
    //UpdateTransform();

    // モデル行列更新　かわったからここに言って
    //model->UpdateTransform(transform);
}

void BulletFiring::MoveHoming(float speed, float turnSpeed, DirectX::XMFLOAT3 target, float elapsedTime)
{
    //　寿命処理 
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)// 寿命が尽きたら自害
    {
        // 自分を削除
        Destroy();
    }
    // 移動　　秒じゃなくとフレームに
    float bulletspeed = speed * elapsedTime;

    // 　だからかけ算で向きにこれだけ進だから位置に入れた
    position.x += bulletspeed * direction.x;
    position.y += bulletspeed * direction.y;
    position.z += bulletspeed * direction.z;


    // 旋回
    {
        float bulletTurnSpeed = turnSpeed * elapsedTime;

        // ターゲットまでのベクトルを算出
        DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);

        // ゼロベクトルでないなら回転処理　ピッタリ同じなら回転できるから確認
        DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
        float lengthSq;
        DirectX::XMStoreFloat(&lengthSq, LengthSq);

      

        if (lengthSq > 0.00001f)
        {

            // ターゲットまでのベクトルを単位ベクトル化
            Vec = DirectX::XMVector3Normalize(Vec);

            // 向いてる方向ベクトルを算出　direction単位ベクトル前提
            DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);


            // 前方方向ベクトルとターゲットまでのベクトルの内積（角度）を算出
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);



            float dot;
            DirectX::XMStoreFloat(&dot, Dot);

            // 回転速度調整最後の微調整行き過ぎないように少しずつ小さく出来る。
            // アークコサインでも出来るしかも一瞬でその値を入れる。
            // 2つの単位ベクトルの角度が小さい程
            // 1.0に近づくという性質を利用して回転速度を調整する
            //if (dot )
            float rot;
            // 1.0f　dotは０に近づく程１になるからーくとこっちも０になる
            rot = 1.0f - dot;
            // だから１のほうがでかければスピードを入れる
            // ターンスピードよりロットの方が小さい時に
            if (rot > bulletTurnSpeed)
            {
                rot = bulletTurnSpeed;
            }


            // 回転角度があるなら回転処理する　ここで０を取ってないと外積が全く同じになって計算出来ない
            if (fabsf(rot) >= 0.0001)
            {
                // 回転軸を算出  外積  向かせたい方を先に 
                DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction, Vec);
                // 誤差防止の為に単位ベクトルした方が安全
                Axis = DirectX::XMVector3Normalize(Axis);
                // 回転軸と回転量から回転行列を算出 回転量を求めている。
                DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);


                DirectX::XMFLOAT4X4 transform = GetActor()->GetTransform();

                // 現在の行列を回転させる　自分自身の姿勢
                DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
                Transform = DirectX::XMMatrixMultiply(Transform, Rotation); // 同じだからただ×だけ  Transform*Rotation
                // DirectX::XMMatrixMultrixMultiply
                // 回転後の前方方向を取り出し、単位ベクトル化する
                Direction = DirectX::XMVector3Normalize(Transform.r[2]);// row
                DirectX::XMStoreFloat3(&direction, Direction);


                //DirectX::XMStoreFloat4x4(&transform, Transform);


               

            }


              
            
        }
    }

    GetActor()->SetDirection(direction);
    GetActor()->SetPosition(position);

    GetActor()->UpdateTransformProjectile();

    //GetActor()->GetModel()->UpdateAnimation(elapsedTime, true);



    model->UpdateTransform(GetActor()->GetTransform());
            //GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());
          
}

void BulletFiring::Lanch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float   lifeTimer)
{
    // direction 方向　正規化して入れる差もなきゃスピード変わる
    this->direction = direction;
    this->position = position;
    this->lifeTimer = lifeTimer;

    model = GetActor()->GetComponent<ModelControll>()->GetModel();


    GetActor()->SetDirection(direction);
}

void BulletFiring::Destroy()
{
    ActorManager::Instance().Remove(GetActor());
}

void BulletFiring::OnGUI()
{
    ImGui::InputFloat3("Move Speed", &position.x);
    ImGui::InputFloat3("direction", &direction.x);
    ImGui::InputFloat("lifeTimer", &lifeTimer);

}
