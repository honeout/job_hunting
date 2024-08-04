#include "Projectile.h"
#include "ProjectileManager.h"
#include "Graphics/Graphics.h"
Projectile::Projectile()
  
{
    // 作った奴を登録
   /* manager->Register(this);*/
}

//void Projectile::DrawDebugPrimitive()
//{
//    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
//
//    // 今は何も表示しない
//        //// 衝突判定用のデバッグ球を描画
//    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 1, 1));
//
//}




void Projectile::UpdateTransform()
{

    // 何か
    DirectX::XMVECTOR Front, Up, Right;
    // 前ベクトルを算出　まず向きを手に入れる
    Front = DirectX::XMLoadFloat3(&direction);
    Front = DirectX::XMVector3Normalize(Front);
    // 仮の上ベクトルを算出　少しおかしい値を入れておく真上に打った時に
    // ここと同じ値だとバグるから
    Up = DirectX::XMVectorSet(0.001f, 1, 0, 0);
    Up = DirectX::XMVector3Normalize(Up);

    // 右ベクトル算出　上二つを使い一つの新しい軸を作る
    Right = DirectX::XMVector3Cross(Up, Front);// ２つで１つの奴を出す

    Right = DirectX::XMVector3Normalize(Right);
    // 上ベクトル算出 さらに作った二つでさらに新しいひとつを作る。
    Up = DirectX::XMVector3Cross(Front, Right);// 斜め上の奴出来る



    // 計算結果を取り出し
    DirectX::XMFLOAT3 right, up, front;
    DirectX::XMStoreFloat3(&right, Right);
    DirectX::XMStoreFloat3(&up, Up);
    DirectX::XMStoreFloat3(&front, Front);

    // 算出した軸ベクトルから行列を作成 回転あり
    transform._11 = right.x * scale.x; // 大きさだけやった奴０．１倍ならそうなる
    transform._12 = right.y * scale.x;// 角度
    transform._13 = right.z * scale.x;
    transform._14 = 0.0f;
    transform._21 = up.x * scale.y;
    transform._22 = up.y * scale.y;// 裏返る
    transform._23 = up.z * scale.y;
    transform._24 = 0.0f;
    transform._31 = front.x* scale.z;
    transform._32 = front.y * scale.z;
    transform._33 = front.z * scale.z;// 向き
    transform._34 = 0.0f;
    transform._41 = position.x;
    transform._42 = position.y;
    transform._43 = position.z;
    transform._44 = 1.0f;

    this->direction = front;// 向きをこっちに入れるこれでむきの計算
    // を外に
}

void Projectile::Destroy()
{
    // 弾丸から自分を削除つまりマネージャーから　 管理している奴をからも消す
    //manager->Remove(this);
}
