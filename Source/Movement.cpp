#include <algorithm>
#include <imgui.h>
#include "Math.h"
#include "Mathf.h"
#include "Movement.h"

#include "StageManager.h"

// コンストラクタ
Movement::Movement()
{

}

// デストラクタ
Movement::~Movement()
{

}

// GUI描画
void Movement::OnGUI()
{
	ImGui::InputFloat("Move Speed", &moveSpeed);
	ImGui::InputFloat("Turn Speed", &turnSpeed);
    ImGui::InputInt("Jump max", &jumpCount);
    ImGui::InputFloat("stepOffSet", &stepOffSet);
}

// ワールド移動
void Movement::Move(const DirectX::XMFLOAT3& direction,float speed ,float elapsedTime)
{
	std::shared_ptr<Actor> actor = GetActor();
    moveSpeed = speed * elapsedTime;
	DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
	DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(Direction, moveSpeed);


    DirectX::XMStoreFloat3(&velocity, Velocity);

    moveVecX = velocity.x;
    moveVecZ = velocity.z;



}

// ローカル移動
void Movement::MoveLocal(const DirectX::XMFLOAT3& direction, float elapsedTime)
{
	std::shared_ptr<Actor> actor = GetActor();
	speed = moveSpeed * elapsedTime;
    //turnSpeed = moveSpeed * elapsedTime;
	DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
	DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(Direction, speed);
	DirectX::XMVECTOR Rotation = DirectX::XMLoadFloat4(&actor->GetRotation());
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationQuaternion(Rotation);
	DirectX::XMVECTOR Move = DirectX::XMVector3TransformNormal(Velocity, Transform);
	DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&actor->GetPosition());

	Position = DirectX::XMVectorAdd(Position, Move);

	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, Position);
	actor->SetPosition(position);

   // DirectX::XMStoreFloat3(&velocity, Velocity);
}
//
// 旋回
void Movement::Turn(const DirectX::XMFLOAT3& direction, float elapsedTime)
{/*
	std::shared_ptr<Actor> actor = GetActor();
	speed = turnSpeed * elapsedTime;
	DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
	DirectX::XMVECTOR Rotation = DirectX::XMLoadFloat4(&actor->GetRotation());
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationQuaternion(Rotation);
	DirectX::XMVECTOR OneZ = DirectX::XMVectorSet(0, 0, 1, 0);
	DirectX::XMVECTOR Front = DirectX::XMVector3TransformNormal(OneZ, Transform);

    if (direction.x == 0 && direction.z == 0)return;

	Direction = DirectX::XMVector3Normalize(Direction);
	DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Front, Direction);
	if (DirectX::XMVector3Equal(Axis, DirectX::XMVectorZero()))
	{
		return;
	}

	DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Front, Direction);

	float dot;
	DirectX::XMStoreFloat(&dot, Dot);
    speed = (std::min)(1.0f - dot, speed);

	DirectX::XMVECTOR Turn = DirectX::XMQuaternionRotationAxis(Axis, speed);
	Rotation = DirectX::XMQuaternionMultiply(Rotation, Turn);

	DirectX::XMFLOAT4 rotation;
	DirectX::XMStoreFloat4(&rotation, Rotation);
	actor->SetRotation(rotation);*/



     //1フレームでどれだけ移動
    speed = turnSpeed * elapsedTime;
    //speed *= elapsedTime;

    std::shared_ptr<Actor> actor = GetActor();

    DirectX::XMFLOAT4 rotate = actor->GetRotation();

    // 進行ベクトルが0ベクトルの場合は処理する必要なし
    if (direction.x == 0 && direction.z == 0)return;

    float vx = direction.x;
    float vz = direction.z;

    float length = sqrtf(direction.x * direction.x + direction.z * direction.z);
    if (length < 0.001f) return;

    vx /= length;
    vz /= length;

    // 自身の回転値から前方向を求める
    float frontX = sinf(rotate.y); // 左右を見る為
    float frontZ = cosf(rotate.y); // 前後判定のため

    //回転角を求めるため、２つのベクトルの内積を計算する
    float dot = (frontX * vx) + (frontZ * vz);// 内積

    // 内積地は-1~1で表現されていて２つの単位ベクトルの角度が
    // 小さいほど１．０にちがづくという性質を利用して回転速度を調整
    float rot = 1.0 - dot;
    if (rot > speed) rot = speed;


    // 左右判定を行う為に２つの単位ベクトルの外積を計算する
    float cross = (frontZ * vx) - (frontX * vz);

    // 2Dの外積値が正の場合か負の場合によって左右判定が行える
    // 左右判定を行うことによって左右回転を選択する
    if (cross < 0.0f)
    {
        rotate.y -= rot;
    }
    else
    {
        rotate.y += rot;
    }

    actor->SetRotation(rotate);
    
}
//
////旋回処理
//void Movement::Turn(float elapsedTime, float vx, float vz, float speed)
//{
//    // 1フレームでどれだけ移動
//    speed *= elapsedTime;
//
//    std::shared_ptr<Actor> actor = GetActor();
//
//    DirectX::XMFLOAT3 angle = 
//    {
//        actor->GetRotation().x,
//        actor->GetRotation().y,
//        actor->GetRotation().z
//    };
//
//    // 進行ベクトルが0ベクトルの場合は処理する必要なし
//    if (vx == 0 && vz == 0)return;
//
//    float length = sqrtf(vx * vx + vz * vz);
//    if (length < 0.001f) return;
//
//    vx /= length;
//    vz /= length;
//
//    // 自身の回転値から前方向を求める
//    float frontX = sinf(angle.y); // 左右を見る為
//    float frontZ = cosf(angle.y); // 前後判定のため
//
//    //回転角を求めるため、２つのベクトルの内積を計算する
//    float dot = (frontX * vx) + (frontZ * vz);// 内積
//
//    // 内積地は-1~1で表現されていて２つの単位ベクトルの角度が
//    // 小さいほど１．０にちがづくという性質を利用して回転速度を調整
//    float rot = 1.0 - dot;
//    if (rot > speed) rot = speed;
//
//
//    // 左右判定を行う為に２つの単位ベクトルの外積を計算する
//    float cross = (frontZ * vx) - (frontX * vz);
//
//    // 2Dの外積値が正の場合か負の場合によって左右判定が行える
//    // 左右判定を行うことによって左右回転を選択する
//    if (cross < 0.0f)
//    {
//        angle.y -= rot;
//    }
//    else
//    {
//        angle.y += rot;
//    }
//
//    actor->SetRotation(
//        { angle.x,angle.y,angle.z,0 });
//}


// ワールド移動
void Movement::OnLanding()
{
    std::shared_ptr<Actor> actor = GetActor();
    
    
    // 着地したからリセット
    jumpCount = 0;
    
    //// 下方向の速力が一定以上なら着地ステートへ  十分な速度で落とす重力の５倍２、３秒後に着地モーションをする。
    if (velocity.y < gravity * 5.0f)
    {
        onLadius = true;
        
        
    }
}

void Movement::JumpVelocity( float speed)
{
    std::shared_ptr<Actor> actor = GetActor();
    //DirectX::XMFLOAT3 velocity = Mathf::Scale(direction, speed);
    DirectX::XMFLOAT3 position = actor->GetPosition();




    


    velocity.y = speed;
    //jumpSpeed = speed;

    //position.y = position.y * jumpSpeed;

   // position.y += velocity.y;

    //actor->SetPosition(position);
    //position.y = (std::min)(jumpSpeedMax, jumpSpeed);
    actor->SetPosition(position);
}

void Movement::UpdateHorizontalVelocity( float elapsedFrame)
{
    std::shared_ptr<Actor> actor = GetActor();
    //speed = moveSpeed * elapsedFrame;
    //DirectX::XMFLOAT3 velocity = Mathf::Scale(direction, speed);



    DirectX::XMFLOAT3 position = actor->GetPosition();


    // XZ平面の速力を減速する 速度の長さ
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (length > 0.0f)
    {
        // 摩擦力
        float friction = this->friction * elapsedFrame;

        // 空中にいるときは摩擦力を減らす
        if (!isGround) friction *= airControl;// 元々の摩擦の計数を減らす為

        // 摩擦による横方向の減速処理
        if (length > friction)
        {
            // 単位ベクトル化する。
            float vx = velocity.x / length;
            float vz = velocity.z / length;

            // 減速抵抗力の分だけ
            velocity.x -= vx * friction;
            velocity.z -= vz * friction;
        }
        // 横方向の速力が摩擦力以下になったので速力を無効化
        else
        {
            velocity.x = 0;
            velocity.z = 0;

        }
    }

    // XZ平面の速力を加速する
    if (length <= maxMoveSpeed)
    {
        // 移動ベクトルが0ベクトルでないなら加速する
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
        if (moveVecLength > 0.0f)
        {
            // 加速力
            float accelration = this->acceleration * elapsedFrame;

            // 空中にいるときは摩擦力を減らす
            if (!isGround) accelration *= isGround;// 鈍くする。30パーセント

            // 移動ベクトルによる加速処理 また速度をいじったから
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            // 最大速度制御
            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;

                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }
            // 下り坂でガタガタしないようにする
            if (isGround && slopeRate > 0.0f)
            {
                velocity.y -= length * slopeRate * elapsedFrame;
            }
        }
    }
    // 移動ベクトルをリセット アクセル踏んでる状態を解除
    moveVecX = 0.0f;
    moveVecZ = 0.0f;

}

void Movement::UpdateHorizontalMove( float elapsedTime)
{
    std::shared_ptr<Actor> actor = GetActor();
    //speed = moveSpeed * elapsedTime;
    //DirectX::XMFLOAT3 velocity = Mathf::Scale(direction, speed);
    DirectX::XMFLOAT3 position = actor->GetPosition();
    //float stepOffSet = actor->GetStepOffset();


    // 水平速力量計算
    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (velocityLengthXZ > 0.0f)
    {
        // 水平移動値
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        // レイの開始位置と終点位置
        // 段差分高く
        DirectX::XMFLOAT3 start = { position.x, position.y + stepOffSet, position.z };
        // 移動方向分足した奴
        DirectX::XMFLOAT3 end = { position.x + mx, position.y + stepOffSet, position.z + mz };

        // レイキャストによる壁判定
        HitResult hit;
        if (StageManager::instance().RayCast(start, end, hit))
        {
            // 壁までのベクトル
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

            // 壁の法線
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

            // 入射ベクトルを法線に射影長
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
            Dot = DirectX::XMVectorScale(Dot, 1.1f); // 壁にめり込まないように少しだけ補正

            // 補正位置の計算
            DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
            DirectX::XMFLOAT3 collectPosition;
            DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

            // 壁ずり方向へレイキャスト
            HitResult hit2;
            if (!StageManager::instance().RayCast(hit.position, collectPosition, hit2))
            {
                // 壁ずり方向で壁に当たらなかったら補正位置に移動
                position.x = collectPosition.x;
                position.z = collectPosition.z;
            }
            else
            {
                position.x = hit2.position.x;
                position.z = hit2.position.z;
            }
        }
        else
        {
            // 移動
            position.x += mx;
            position.z += mz;
        }

        actor->SetPosition(position);
        
    }
}

void Movement::UpdateVerticalVelocity( float elapsedFrame)
{
    std::shared_ptr<Actor> actor = GetActor();
    //speed = moveSpeed * elapsedFrame;
    //DirectX::XMFLOAT3 velocity = Mathf::Scale(direction, speed);
    //DirectX::XMFLOAT3 position = actor->GetPosition();
    //float stepOffSet = actor->GetStepOffset();

    //DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
    //DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(Direction, speed);

   // DirectX::XMStoreFloat3(&velocity, Velocity);

    // 重力処理
    velocity.y += gravity * elapsedFrame;

    //actor->SetPosition(position);

}

void Movement::UpdateVerticalMove( float elapsedTime)
{
    std::shared_ptr<Actor> actor = GetActor();

    DirectX::XMFLOAT3 position = actor->GetPosition();

    DirectX::XMFLOAT4 rotation = { actor->GetRotation()};

    // 垂直方向の移動量
    float my = velocity.y * elapsedTime;

    slopeRate = 0.0f; // 傾斜率

    

    // キャラクターのY軸方向となる法線ベクトル
    DirectX::XMFLOAT3 normal = { 0,1,0 };

    // 落下中
    if (my <= 0.0f)
    {
        // レイの開始位置は足元より少し上
        DirectX::XMFLOAT3 start = { position.x, position.y + stepOffSet, position.z };
        // レイの終点位置は移動後の位置
        DirectX::XMFLOAT3 end = { position.x, position.y + my , position.z };

        // レイキャストによる地面判定
        HitResult hit;
        // レイキャストを呼ぶための関数
        //if (StageManager::instance().RayCast(start, end, hit))
        if (StageManager::instance().RayCast(start, end, hit))
        {

            // 法線ベクトル取得
            normal = hit.normal;


            // 地面に設置している
            position = hit.position;

            // 回転
            // ｙだけな理由は角度が真横になった時にクモみたいにひっつかないように
            rotation.y += hit.rotation.y;

            // 傾斜率の計算 法線とXZ平面に置けるベクトルとｙ軸のベクトルで傾きが求められる。
            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));
            // 着地
            if (!isGround)
            {

                OnLanding();
            }
            isGround = true;
            velocity.y = 0.0f;
            
            //jumpSpeed = 0.0f;

        }
        else
        {
            // 空中に浮いている
            position.y += my;
            isGround = false;
            onLadius = false;
        }

    }
    // 上昇中
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }

    // 地面の向きに沿うようにXZ軸回転
    {
        // Y軸が法線ベクトル方向に向くオイラー角回転を算出する
        float storageAngleX = atan2f(normal.z, normal.y);
        float storageAngleZ = -atan2f(normal.x, normal.y);

        // 線形補完で滑らかに回転する
        // (変化する値,最終的な値,これだけ進)
        rotation.x = Mathf::Lerp(rotation.x, storageAngleX, 0.2f);
        rotation.z = Mathf::Lerp(rotation.z, storageAngleZ, 0.2f);

    }

    actor->SetPosition(position);
    actor->SetRotation(rotation);
}

void Movement::UpdateVelocity( float elapsedTime)
{
    //DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
    //DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(Direction, speed);

  /*  DirectX::XMStoreFloat3(&velocity, Velocity);*/


    // 経過フレーム
    float elapsedFrame = 60.0f * elapsedTime;

    // 垂直速力更新処理
    UpdateVerticalVelocity(elapsedFrame);

    // 水平速力更新処理
    UpdateHorizontalVelocity( elapsedFrame);

    // 水平移動更新処理
    UpdateHorizontalMove(elapsedFrame);

    // 垂直移動更新処理
    UpdateVerticalMove(elapsedTime);
}
