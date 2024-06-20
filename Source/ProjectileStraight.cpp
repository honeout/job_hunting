#include "ProjectileStraight.h"
#include "Collision.h"
//#include "StageManager.h"
// コンストラクタ
ProjectileStraight::ProjectileStraight()
                   
{
    //model = new Model("Data/Model/Sword/Sword.mdl");

    //// 表示サイズを調整
    //scale.x = scale.y = scale.z = 3.0f;
}
// デストラクタ
ProjectileStraight::~ProjectileStraight()
{

    //delete model;
}
void ProjectileStraight::Start()
{
    // モデル一様
    model = GetActor()->GetModel();

    
    // 銃移動のコンポーネント
    bulletFiring = GetActor()->GetComponent<BulletFiring>();

    
}
// 更新処理
void ProjectileStraight::Update(float elapsedTime)
{
    // 寿命まで飛ぶ
    bulletFiring->Move(speed,elapsedTime);
    ////　寿命処理 

    //lifeTimer -= elapsedTime;
    //if (lifeTimer <= 0.0f)// 寿命が尽きたら自害
    //{
    //    // 自分を削除
    //    Destroy();
    //}
    //// 移動　　秒じゃなくとフレームに
    //float speed = this->speed * elapsedTime;

    //// 　だからかけ算で向きにこれだけ進だから位置に入れた
    //position.x += speed * direction.x;
    //position.y += speed * direction.y;
    //position.z += speed * direction.z;

    //UpdateReflection(elapsedTime);

    //// オブジェクト行列を更新　変わった位置の行列を
    //UpdateTransform();

    //// モデル行列更新　かわったからここに言って
    //model->UpdateTransform(transform);
}
//// 描画処理
//void ProjectileStraight::Render(const RenderContext& rc, ModelShader* shader)
//{
//    shader->Draw(rc, model);
//}
//// 発射
//void ProjectileStraight::Lanch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3&position)
//{
//    // direction 方向　正規化して入れる差もなきゃスピード変わる
//    this->direction = direction;
//    this->position = position;
//}
//
//void ProjectileStraight::UpdateReflection(float elapsedTime)
//{
//    DirectX::XMFLOAT2 velocity = {  this->direction.x * this->speed,   this->direction.z * this->speed };
//    // 水平速力量計算
//    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
//    
//    if (velocityLengthXZ > 0.0f)
//    {
//        // 水平移動値
//        float mx = velocity.x * elapsedTime;
//        float mz = velocity.y * elapsedTime;
//
//        // レイの開始位置と終点位置
//        // 段差分高く
//        DirectX::XMFLOAT3 start = { position.x,position.y, position.z };
//        // 移動方向分足した奴
//        DirectX::XMFLOAT3 end = { position.x + mx,position.y, position.z + mz };
//
//        //////////////////////////////////
//
//    //    // レイキャストによる壁判定
//    //    HitResult hit;
//    //    if (StageManager::instance().RayCast(start, end, hit))
//    //    {
//    //           
//    //        // 壁までのベクトル
//    //        DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
//    //        DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
//    //        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
//
//    //        // 壁の法線
//    //        DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);
//
//    //        // 入射ベクトルを法線に射影長 a
//    //        DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
//    //        Dot = DirectX::XMVectorScale(DirectX::XMVectorScale(Dot, 2.0f), 1.1f); // 壁にめり込まないように少しだけ補正
//
//    //       /* DirectX::XMVECTOR Rpositon;*/
//    //        //        // 壁反射
//    //        //Dot = DirectX::XMVectorAdd(End, Dot);
//
//    //        // 補正位置の計算
//    //        DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
//    //        DirectX::XMFLOAT3 collectPosition;
//    //        DirectX::XMStoreFloat3(&collectPosition, CollectPosition);
//
//    //        DirectX::XMVECTOR R = DirectX::XMVectorSubtract(CollectPosition, Start);
//    //        DirectX::XMFLOAT3 r; 
//    //        R = DirectX::XMVector3Normalize(R);
//    //        DirectX::XMStoreFloat3(&r, R);
//
//
//    //        direction.x = r.x;
//    //        direction.z = r.z;
//
//    //        // 壁ずり方向へレイキャスト
//    //        //HitResult hit2;
//    //        //if (!StageManager::instance().RayCast(hit.position, collectPosition, hit2))
//    //        {
//    //            // 壁ずり方向で壁に当たらなかったら補正位置に移動
//    //            position.x = collectPosition.x;
//    //            position.z = collectPosition.z;
//
//
//
//    //        }
//    //        //else
//    //        //{
//    //        //    position.x = hit2.position.x;
//    //        //    position.z = hit2.position.z;
//
//    //        //       
//    //        //      
//    //        //}
//    //    }
//    //    else
//    //    {
//    //        // 移動
//    //        position.x += mx;
//    //        position.z += mz;
//    //    }
//    //}
//        /////////////////////////////////
//    }
//    //// レイキャストによる壁判定
//    //HitResult hit;
//    //if (StageManager::instance().RayCast(start, end, hit))
//    //{
//    //    // 壁までのベクトル
//    //    DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
//    //    DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
//    //    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
//
//    //    // 壁の法線
//    //    DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);
//
//    //    // 入射ベクトルを法線に射影長 a
//    //    DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
//    //    Dot = DirectX::XMVectorScale(DirectX::XMVectorScale(Dot, 2.0f), 1.1f); // 壁にめり込まないように少しだけ補正
//
//    //    // 壁反射
//    //    Dot = DirectX::XMVectorAdd(End, Dot);
//
//
//    //    // 補正位置の計算
//    //    DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
//    //    DirectX::XMFLOAT3 collectPosition;
//    //    DirectX::XMStoreFloat3(&collectPosition, CollectPosition);
//
//
// //           // 壁ずり方向へレイキャスト
// //           HitResult hit2;
// //           if (!StageManager::instance().RayCast( hit.position, collectPosition, hit2))
// //           {
// //               // 壁ずり方向で壁に当たらなかったら補正位置に移動
// ///*               position.x = collectPosition.x;
// //               position.z = collectPosition.z;*/
// //              /* DirectX::XMStoreFloat3(&direction,Dot);*/
// //               
// //               //position.x = hit.position.x;
// //               //position.z = hit.position.z;
// //               //direction.x = sinf(hit2.rotation.y);
// //               //direction.z = cosf(hit2.rotation.y);
//
//
//
// //           }
// //           else
// //           {
// //               //DirectX::XMVECTOR Start2 = DirectX::XMLoadFloat3(&hit.position);
// //               //DirectX::XMVECTOR End2 = DirectX::XMLoadFloat3(&collectPosition);
// //               //DirectX::XMVECTOR Vector2 = DirectX::XMVectorSubtract(End2, Start2);
// //               //Vector2 = DirectX::XMVector3Normalize(Vector2);
//
// //               //DirectX::XMFLOAT3 vector2;
// //               //DirectX::XMStoreFloat3(&vector2,Vector2);
//
// //               //position.x = hit.position.x;
// //               //position.z = hit.position.z;
//
// //               direction.x = 0;
// //               direction.z = 0;
//
// //           }
// //       }
// //       else
// //       {
// //           // 移動
// //           position.x += mx;
// //           position.z += mz;
// //       }
// //   }
//}



