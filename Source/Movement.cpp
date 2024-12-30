#include <algorithm>
#include <imgui.h>
#include "Math.h"
#include "Mathf.h"
#include "Movement.h"
#include "Collision.h"
#include "StageMain.h"
#include "ModelControll.h"


//#include "StageManager.h"

// コンストラクタ
Movement::Movement()
{
    area.max.x = 0.0f;
    area.max.y = 0.0f;
    area.max.z = 0.0f;

    area.min.x = 0.0f;
    area.min.y = 0.0f;
    area.min.z = 0.0f;
}

// デストラクタ
Movement::~Movement()
{

}
#ifdef _DEBUG
// GUI描画
void Movement::OnGUI()
{
	ImGui::InputFloat("Move Speed", &moveSpeed);
	ImGui::InputFloat("Turn Speed", &turnSpeed);
    ImGui::InputInt("Jump max", &jumpCount);
    //ImGui::InputFloat("stepOffSet", &stepOffSet);
    ImGui::InputFloat3("velocity", &velocity.x);

    if (onLadius)
        ++butten;
    ImGui::InputInt("butten", &butten);

    
}
#endif // _DEBUG
// ワールド移動
void Movement::Move(const DirectX::XMFLOAT3& direction,float speed ,float elapsedTime)
{
	std::weak_ptr<Actor> actor = GetActor();
   // moveSpeed = speed * elapsedTime;
    maxMoveSpeed = speed;
	//DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
	//DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(Direction, moveSpeed);

    //collision = actor.lock()->GetComponent<Collision>();
    //transformid = actor.lock()->GetComponent<Transform>();

    //DirectX::XMStoreFloat3(&velocity, Velocity);

    moveVecX = direction.x;
    moveVecZ = direction.z;



}

// ローカル移動
void Movement::MoveLocal(const DirectX::XMFLOAT3& direction, float elapsedTime)
{
   // transformid = GetActor()->GetComponent<Transform>();
	//std::shared_ptr<Actor> actor = GetActor();
	//speed = moveSpeed * elapsedTime;
 //   //turnSpeed = moveSpeed * elapsedTime;
	//DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
	//DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(Direction, speed);
	//DirectX::XMVECTOR Rotation = DirectX::XMLoadFloat3(&GetActor()->GetComponent<Transform>()->GetAngle());
	//DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationQuaternion(Rotation);
	//DirectX::XMVECTOR Move = DirectX::XMVector3TransformNormal(Velocity, Transform);
	//DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&GetActor()->GetComponent<Transform>()->GetPosition());

	//Position = DirectX::XMVectorAdd(Position, Move);

	//DirectX::XMFLOAT3 position;
	//DirectX::XMStoreFloat3(&position, Position);
 //   GetActor()->GetComponent<Transform>()->SetPosition(position);

   // DirectX::XMStoreFloat3(&velocity, Velocity);
}
//
// 旋回
bool Movement::Turn(const DirectX::XMFLOAT3& direction,float speed, float elapsedTime)
{
    speed = speed * elapsedTime;

    std::weak_ptr<Actor> actor = GetActor();
    std::weak_ptr<Transform> transformid = actor.lock()->GetComponent<Transform>();

    DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();

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

    transformid.lock()->SetAngle(angle);

    //if ((dot + FLT_EPSILON) >= -(0.1f - FLT_EPSILON) && 
    //    dot - FLT_EPSILON <= (0.1f + FLT_EPSILON))
    //{
    //    return true;
    //}

    if (dot + FLT_EPSILON >= -(0.9f - FLT_EPSILON))
    {
        return true;
    }

    else if (dot - FLT_EPSILON <= (0.9f + FLT_EPSILON))
    {
        return true;
    }

    return false;
   
}
bool Movement::TurnCheck(const DirectX::XMFLOAT3& direction, DirectX::XMFLOAT2 angleRange, float elapsedTime)
{
    std::weak_ptr<Actor> actor = GetActor();
    std::weak_ptr<Transform> transformid = actor.lock()->GetComponent<Transform>();

    DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();

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


    // 左右判定を行う為に２つの単位ベクトルの外積を計算する
    float cross = (frontZ * vx) - (frontX * vz);

    // 2Dの外積値が正の場合か負の場合によって左右判定が行える
    // 左右判定を行うことによって左右回転を選択する
    if (cross > angleRange.x)
    {
        return true;
    }
    else if (cross < angleRange.y)
    {
        return true;
    }

    return false;
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
    std::weak_ptr<Actor> actor = GetActor();
    
    
    // 着地したからリセット
    jumpCount = 0;
    
    //// 下方向の速力が一定以上なら着地ステートへ  十分な速度で落とす重力の５倍２、３秒後に着地モーションをする。
    if (velocity.y < gravity * 5.0f)
    {
        onLadius = true;
        
        
    }
}

void Movement::JumpVelocity(float speed)
{
    std::weak_ptr<Actor> actor = GetActor();
    //DirectX::XMFLOAT3 velocity = Mathf::Scale(direction, speed);
    //DirectX::XMFLOAT3 position = actor->GetPosition();




    ++jumpCount;


    velocity.y = speed;
    //jumpSpeed = speed;

    //position.y = position.y * jumpSpeed;

   // position.y += velocity.y;

    //actor->SetPosition(position);
    //position.y = (std::min)(jumpSpeedMax, jumpSpeed);
    //actor->SetPosition(position);
}

void Movement::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}

void Movement::UpdateHorizontalVelocity( float elapsedFrame)
{
    std::weak_ptr<Actor> actor = GetActor();
    std::weak_ptr<Transform> transformid = actor.lock()->GetComponent<Transform>();
    //speed = moveSpeed * elapsedFrame;
    //DirectX::XMFLOAT3 velocity = Mathf::Scale(direction, speed);


    
    DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();


    // XZ平面の速力を減速する 速度の長さ
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if ((length + FLT_EPSILON )> (0.0f - FLT_EPSILON))
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
    std::weak_ptr<Actor> actor = GetActor();
    std::weak_ptr<Transform> transformid = actor.lock()->GetComponent<Transform>();
    //speed = moveSpeed * elapsedTime;
    //DirectX::XMFLOAT3 velocity = Mathf::Scale(direction, speed);
    DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();
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
        if (!IsOutOfAreaX(position.x))
        {
            // 移動X
            position.x += mx;
        }
        if (!IsOutOfAreaZ(position.z))
        {
            // 移動Z
            position.z += mz;
        }

        //// レイキャストによる壁判定
        //HitResult hit;



        //// モデルデータ
        //Model* stagemodel = StageManager::Instance().GetStage(StageManager::Instance().GetStageCount() - 1)->GetComponent<ModelControll>()->GetModel();
        //

        //if (collision->IntersectRayVsModel(start,end, stagemodel,hit))
        //{

        //  /*  actor2->SetName("Player");*/
        //    // 壁までのベクトル
        //    DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
        //    DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
        //    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

        //    // 壁の法線
        //    DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

        //    // 入射ベクトルを法線に射影長
        //    DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
        //    Dot = DirectX::XMVectorScale(Dot, 1.1f); // 壁にめり込まないように少しだけ補正

        //    // 補正位置の計算
        //    DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
        //    DirectX::XMFLOAT3 collectPosition;
        //    DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

        //    // 壁ずり方向へレイキャスト
        //    HitResult hit2;


        //    // モデルデータ
        //    Model* stagemodel = StageManager::Instance().GetStage(StageManager::Instance().GetStageCount() - 1)->GetComponent<ModelControll>()->GetModel();

        //    //// モデルデータを入れる。
        //    //Model* model2 = GetActor()->GetModel();
        //           
        //   // if (!StageManager::instance().RayCast(hit.position, collectPosition, hit2))
        //    //if (!stageMain->RayCast(hit.position, collectPosition,  hit))
        //    //if (!Collision::IntersectRayVsModel(hit.position, collectPosition, GetActor()->GetComponent<HP>()->GetActor()->GetModel(),  hit))
        //    if (!collision->IntersectRayVsModel(hit.position, collectPosition, stagemodel, hit2))
        //    {
        //        // 壁ずり方向で壁に当たらなかったら補正位置に移動
        //        position.x = collectPosition.x;
        //        position.z = collectPosition.z;
        //    }
        //    else
        //    {
        //        position.x = hit2.position.x;
        //        position.z = hit2.position.z;
        //    }
        //}
        //else
        //{
        //    // 移動
        //    position.x += mx;
        //    position.z += mz;
        //}

        
    }
    transformid.lock()->SetPosition(position);
}

void Movement::UpdateVerticalVelocity( float elapsedFrame)
{
    std::weak_ptr<Actor> actor = GetActor();
  

    // 重力処理
    velocity.y += gravity * elapsedFrame;

 

}

void Movement::UpdateVerticalMove( float elapsedTime)
{
    std::weak_ptr<Actor> actor = GetActor();

    std::weak_ptr<Transform> transformid = actor.lock()->GetComponent<Transform>();

    DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();

    DirectX::XMFLOAT3 angle = {transformid.lock()->GetAngle()};

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

        //

        if (position.y + FLT_EPSILON >= (area.min.y - FLT_EPSILON))
        {
            // 空中に浮いている
            position.y += my;
            isGround = false;
            onLadius = false;
        }
        else
        {
            // 着地
            if (!isGround)
            {

                OnLanding();
            }
            isGround = true;
            onLadius = true;
            velocity.y = 0.0f;

            //jumpSpeed = 0.0f;
        }


       // // レイキャストによる地面判定
       // HitResult hit;



       // // モデルデータを入れる。
       // //Model* model = GetActor()->GetModel();

       // //std::shared_ptr<Actor> actor2 = GetActor();
       // //actor2->SetName("StageMain");
       // //// モデルデータを入れる。
       // //Model* model = actor2->GetModel();
       // //Model* model = stageMain->GetModel();

       // // モデルデータ
       // Model* stagemodel = StageManager::Instance().GetStage(StageManager::Instance().GetStageCount() - 1)->GetComponent<ModelControll>()->GetModel();

       // // レイキャストを呼ぶための関数
       // //if (StageManager::instance().RayCast(start, end, hit))
       //// if (collision->IntersectRayVsModel(start, end, GetActor()->GetComponent<StageMain>()->GetActor()->GetModelSabe(), hit))
       // if (collision->IntersectRayVsModel(start, end, stagemodel, hit))
       // {
    

       //     // 法線ベクトル取得
       //     normal = hit.normal;


       //     // 地面に設置している
       //     position = hit.position;

       //     // 回転
       //     // ｙだけな理由は角度が真横になった時にクモみたいにひっつかないように
       //     angle.y += hit.rotation.y;

       //     // 傾斜率の計算 法線とXZ平面に置けるベクトルとｙ軸のベクトルで傾きが求められる。
       //     float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
       //     slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));
       //     // 着地
       //     if (!isGround)
       //     {

       //         OnLanding();
       //     }
       //     isGround = true;
       //     onLadius = true;
       //     velocity.y = 0.0f;
       //     
       //     //jumpSpeed = 0.0f;

       // }
       // else
       // {
       //     // 空中に浮いている
       //     position.y += my;
       //     isGround = false;
       //     onLadius = false;
       // }


    }
    // 上昇中
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
        onLadius = false;
    }

    // 地面の向きに沿うようにXZ軸回転
    {
        // Y軸が法線ベクトル方向に向くオイラー角回転を算出する
        float storageAngleX = atan2f(normal.z, normal.y);
        float storageAngleZ = -atan2f(normal.x, normal.y);

        // 線形補完で滑らかに回転する
        // (変化する値,最終的な値,これだけ進)
        angle.x = Mathf::Lerp(angle.x, storageAngleX, 0.2f);
        angle.z = Mathf::Lerp(angle.z, storageAngleZ, 0.2f);

    }

    transformid.lock()->SetPosition(position);
    transformid.lock()->SetAngle(angle);
}

void Movement::UpdateVelocity( float elapsedTime)
{


    // 経過フレーム
    float elapsedFrame = 60.0f * elapsedTime;

    if (!stopMove)
    {
        // 水平速力更新処理
        UpdateHorizontalVelocity(elapsedFrame);

        // 水平移動更新処理
        UpdateHorizontalMove(elapsedTime);
    }
    else
    {
        // 移動停止時後に移動を防ぐため
        velocity.x = 0;
        velocity.z = 0;
        // 落下をゆったりさせるため
        //velocity.y = 0;
    }
    if (!stopFall)
    {
        // 垂直速力更新処理
        UpdateVerticalVelocity(elapsedFrame);

        // 垂直移動更新処理
        UpdateVerticalMove(elapsedTime);
    }
    else
    {
        // 落下をゆったりさせるため
        velocity.y = 0;
    }

}

bool Movement::IsOutOfAreaX(const float& positionX)
{
    if ((positionX + velocity.x) < area.min.x || (positionX + velocity.x) > area.max.x)
    {
        velocity.x = 0;

        return true;
    }

    return false;
}

bool Movement::IsOutOfAreaZ( const float& positionZ)
{
    if ((positionZ + velocity.z) < area.min.z || (positionZ + velocity.z) > area.max.z)
    {
        velocity.z = 0;

        return true;
    }

    return false;
}

bool Movement::IsOutOfAreaY(const float& positionY)
{
    return positionY < area.min.y || positionY > area.max.y;
}
