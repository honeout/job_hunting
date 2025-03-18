#include <imgui.h>
#include "BulletFiring.h"
#include "ModelControll.h"
#include "ProjectileManager.h"

void BulletFiring::Move(float speed,float elapsedTime)
{
    //　寿命処理 
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)// 寿命が尽きたら自害
    {
        // 自分を削除
        Destroy();
    }
    ProjectileManager::Instance().DeleteUpdate(elapsedTime);
    // 移動　　秒じゃなくとフレームに
    float bulletspeed = speed * elapsedTime;
    // 　だからかけ算で向きにこれだけ進だから位置に入れた
    position.x += bulletspeed * direction.x;
    position.y += bulletspeed * direction.y;
    position.z += bulletspeed * direction.z;
    GetActor()->GetComponent<Transform>()->SetPosition(position);
}
// 回転
bool BulletFiring::Turn(float turnspeed,const DirectX::XMFLOAT3 &target, float elapedTime)
{
   std::weak_ptr<Actor> actor = GetActor();
   std::weak_ptr<Transform> transformid = GetActor()->GetComponent<Transform>();
    DirectX::XMFLOAT3 angle = actor.lock()->GetComponent<Transform>()->GetAngle();
    //// 正面の向きベクトル
    direction.x = sinf(angle.y);// 三角を斜めにして位置を変えた
    direction.y = sinf(angle.x);
    direction.z = cosf(angle.y);
    float bulletTurnSpeed = turnspeed * elapedTime;
    // ターゲットまでのベクトルを算出
    DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);
    // ゼロベクトルでないなら回転処理　ピッタリ同じなら回転できるから確認
    DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);
    if (lengthSq > rangeMin && lengthSq <= rangeMax)
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
            // 現在の行列を回転させる　自分自身の姿勢
            DirectX::XMMATRIX Transformmid = DirectX::XMLoadFloat4x4(&transformid.lock()->GetTransform());
            Transformmid = DirectX::XMMatrixMultiply(Transformmid, Rotation); // 同じだからただ×だけ  Transform*Rotation
            //// DirectX::XMMatrixMultrixMultiply
            //// 回転後の前方方向を取り出し、単位ベクトル化する
            Direction = DirectX::XMVector3Normalize(Transformmid.r[2]);// row
            DirectX::XMStoreFloat3(&direction, Direction);
            actor.lock()->GetComponent<Transform>()->SetDirection(direction);
            return true;
        }
    }
    return false;
}

bool BulletFiring::Turn2D(float speed, DirectX::XMFLOAT3& direction, float elapedTime)
{
    std::shared_ptr<Actor> actor = GetActor();
    DirectX::XMFLOAT3 angle = GetActor()->GetComponent<Transform>()->GetAngle();
    // 進行ベクトルが0ベクトルの場合は処理する必要なし
    if (direction.x == 0 && direction.z == 0)
        return true;
    float vx = direction.x;
    float vz = direction.z;
    float length = sqrtf(direction.x * direction.x + direction.z * direction.z);
    if (length < 0.001f) return true;
    vx /= length;
    vz /= length;
    // 自身の回転値から前方向を求める
    float frontX = sinf(angle.y); // 左右を見る為
    float frontZ = cosf(angle.y); // 前後判定のため
    //回転角を求めるため、２つのベクトルの内積を計算する
    float dot = (frontX * vx) + (frontZ * vz);// 内積
    // 内積地は-1~1で表現されていて２つの単位ベクトルの角度が
    // 小さいほど１．０にちがづくという性質を利用して回転速度を調整
    float rot = 1.0f - dot;
    if (rot > speed) rot = speed;
    // 左右判定を行う為に２つの単位ベクトルの外積を計算する
    float cross = (frontZ * vx) - (frontX * vz);
    // 2Dの外積値が正の場合か負の場合によって左右判定が行える
    // 左右判定を行うことによって左右回転を選択する
    if (cross < 0.0f)
    {
        angle.y -= rot;
    }
    else
    {
        angle.y += rot;
    }
    //// 正面の向きベクトル
    this->direction.x = sinf(angle.y);// 三角を斜めにして位置を変えた
    //this->direction.y = get;
    this->direction.z = cosf(angle.y);
    GetActor()->GetComponent<Transform>()->SetDirection(this->direction);
    if (dot >= 0.93f)
    {
        return true;
    }
    else if (dot <= 0.1f)
    {
        return true;
    }
    return false;
}

void BulletFiring::MoveHoming(float speed, float turnSpeed, DirectX::XMFLOAT3 target, float elapsedTime)
{
    std::weak_ptr<Transform> transformid = GetActor()->GetComponent<Transform>();
    //　寿命処理 
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)// 寿命が尽きたら自害
    {
        // 自分を削除
        Destroy();
    }
    ProjectileManager::Instance().DeleteUpdate(elapsedTime);
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
        if (lengthSq > rangeMin && lengthSq <= rangeMax)
        {
            // ターゲットまでのベクトルを単位ベクトル化
            Vec = DirectX::XMVector3Normalize(Vec);
            // 向いてる方向ベクトルを算出　direction単位ベクトル前提
            DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
            // 前方方向ベクトルとターゲットまでのベクトルの内積（角度）を算出
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);
            float dot;
            DirectX::XMStoreFloat(&dot, Dot);
            float rot;
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
                // 現在の行列を回転させる　自分自身の姿勢
                DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transformid.lock()->GetTransform());
                Transform = DirectX::XMMatrixMultiply(Transform, Rotation); // 同じだからただ×だけ  Transform*Rotation
                // 回転後の前方方向を取り出し、単位ベクトル化する
                Direction = DirectX::XMVector3Normalize(Transform.r[2]);// row
                DirectX::XMStoreFloat3(&direction, Direction);
            }
        }
    }
    transformid.lock()->SetDirection(direction);
    transformid.lock()->SetPosition(position);
}

void BulletFiring::JumpOut(float speed, float turnSpeed, DirectX::XMFLOAT3 target, float elapsedTime)
{
    std::weak_ptr<Transform> transformid = GetActor()->GetComponent<Transform>();
    //　寿命処理 
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)// 寿命が尽きたら自害
    {
        Destroy();
    }
    else
    {
        ProjectileManager::Instance().DeleteUpdate(elapsedTime);
        // 移動　　秒じゃなくとフレームに
        float bulletspeed = speed * elapsedTime;
        // 　だからかけ算で向きにこれだけ進だから位置に入れた
        position.x += bulletspeed * direction.x;
        position.y += bulletspeed * direction.y;
        position.z += bulletspeed * direction.z;
        if (position.y <= -3.5f)
            transformid.lock()->SetPosition(position);
    }
}

void BulletFiring::PushDown(float speed, float turnSpeed,  float elapsedTime)
{
    std::weak_ptr<Transform> transformid = GetActor()->GetComponent<Transform>();
    ProjectileManager::Instance().DeleteUpdate(elapsedTime);
    // 移動　　秒じゃなくとフレームに
    float bulletspeed = speed * elapsedTime;
    position.x -= bulletspeed * direction.x;
    position.y -= bulletspeed * direction.y;
    position.z -= bulletspeed * direction.z;
    transformid.lock()->SetPosition(position);
        if (position.y <= -5.5f)
            Destroy();
}

void BulletFiring::Sunder(DirectX::XMFLOAT3 target, float elapsedTime)
{
    std::weak_ptr<Transform> transformid = GetActor()->GetComponent<Transform>();
    //　寿命処理 
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)// 寿命が尽きたら自害
    {
        // 自分を削除
        Destroy();
    }
    DirectX::XMFLOAT3 pos;
    pos =
    {
        target.x,
        target.y,
        target.z

    };
    transformid.lock()->SetPosition(pos);
}

void BulletFiring::Throwing(float speed, float turnSpeed, DirectX::XMFLOAT3 target, bool turnPermission,float elapsedTime)
{
    std::weak_ptr<Transform> transformid = GetActor()->GetComponent<Transform>();
    //　寿命処理 
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)// 寿命が尽きたら自害
    {
        // 自分を削除
        Destroy();
    }
    ProjectileManager::Instance().DeleteUpdate(elapsedTime);
    // 移動　　秒じゃなくとフレームに
    float bulletspeed = speed * elapsedTime;
    // 旋回
    if (turnPermission)
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
                // 現在の行列を回転させる　自分自身の姿勢
                DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transformid.lock()->GetTransform());
                Transform = DirectX::XMMatrixMultiply(Transform, Rotation); // 同じだからただ×だけ  Transform*Rotation
                // DirectX::XMMatrixMultrixMultiply
                // 回転後の前方方向を取り出し、単位ベクトル化する
                Direction = DirectX::XMVector3Normalize(Transform.r[2]);// row
                DirectX::XMStoreFloat3(&direction, Direction);
            }
        }

    }
    float dirX = 0;
    if (position.y <= -1)
        direction.x += 0.0001f;
    if (position.y >= 0)
        direction.x += -0.01f;
    transformid.lock()->SetDirection(direction);
    position.x += bulletspeed * direction.x;
    position.y += bulletspeed * direction.y;
    position.z += bulletspeed * direction.z;
    // if (position.y <= 0)
    transformid.lock()->SetPosition(position);
}

void BulletFiring::Lanch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float   lifeTimer)
{
    // トランスフォーム取得
    std::weak_ptr<Transform> transformid = GetActor()->GetComponent<Transform>();
    // direction 方向　正規化して入れる差もなきゃスピード変わる
    this->direction = direction;
    this->position = position;
    this->lifeTimer = lifeTimer;
    model = GetActor()->GetComponent<ModelControll>()->GetModel();
    transformid.lock()->SetDirection(direction);
}

void BulletFiring::Destroy()
{
    ActorManager::Instance().Remove(GetActor());
    ProjectileManager::Instance().Remove(GetActor());
   
}
#ifdef _DEBUG
void BulletFiring::OnGUI()
{
    ImGui::InputFloat3("Move Speed", &position.x);
    ImGui::InputFloat3("direction", &direction.x);
    ImGui::InputFloat("lifeTimer", &lifeTimer);
    

}
#endif // _DEBUG