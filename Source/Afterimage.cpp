#include "Afterimage.h"
#include "Graphics/Graphics.h"
#include "AfterimageManager.h"



// デバッグプリミティブ描画
void Afterimage::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //// 衝突判定用のデバッグ球を描画
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    // 衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

}


// 破棄
void Afterimage::Destroy()
{
    //EnemyManager& manager = EnemyManager::Instance();
    ////// 自分自身を入れてそれで個体を消すための所に入れる。」
    ////manager->Remove(this);
    //manager.Remove(this);

    //EnemyManager::Instance().;
    AfterimageManager::Instance().Remove(this);
}