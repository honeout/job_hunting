#include <imgui.h>
#include "AfterimageManager.h"
#include "Collision.h"




// 更新処理
void AfterimageManager::Update(float elapsedTime)
{
    for (Afterimage* after : afterimages)
    {
        after->Update(elapsedTime);
    }


    // 破棄処理 毎フレームここで一気に消す。
    for (Afterimage* after : removes)
    {
        std::vector<Afterimage*>::iterator it = std::find(afterimages.begin(), afterimages.end(),
            after);

        // 見つかったら
        if (it != afterimages.end())
        {
            // 削除登録
            afterimages.erase(it);

        }

        // 消えた訳じゃないので
        // 敵の破棄処理
        delete after;// 入れ物破棄

    }
    // 破棄リストをクリア
    removes.clear();

    //// 敵同士の衝突処理
    //CollisiionEnemyVsEnemies();
}

// 当たり判定描画
void AfterimageManager::DrawDebugPrimitive()
{
   
    for (Afterimage* after : afterimages)
    {
        after->DrawDebugPrimitive();
    }
}


// 描画処理
void AfterimageManager::Render(RenderContext& rc, ModelShader* shader)
{
    for (Afterimage* after : afterimages)
    {
        after->Render(rc, shader);
    }
}

void AfterimageManager::Remove(Afterimage* after)
{
    // 削除設定
    removes.insert(after);// いきなり消すとバグるため
  
}




void AfterimageManager::Register(Afterimage* after)
{
    // 作ったら後ろから追加していく増えていくだけ
    afterimages.emplace_back(after);
}
//
//void AfterimageManager::CollisiionEnemyVsEnemies()
//{
//    // 最大数
//    size_t playerafterimageCount = playerafterimages.size();
//    for (int i = 0; i < playerafterimageCount; ++i) {
//        // i番目の位置
//        Enemy* enemyA = enemies.at(i);
//        for (int j = i + 1; j < enemyCount; ++j)
//        {
//            Enemy* enemyB = enemies.at(j);
//            DirectX::XMFLOAT3 outPosition;
//            if (Collision::IntersectSpherVsSphere(
//                enemyA->GetPosition(),
//                enemyA->GetRadius(),
//                enemyB->GetPosition(),
//                enemyB->GetRadius(),
//                outPosition))
//            {
//                enemyB->SetPosition(outPosition);
//            }
//        }
//
//    }
//
//}



void AfterimageManager::DrawDebugGUI()
{
}

// エネミー全削除
void AfterimageManager::Clear()
{
    for (Afterimage* after : afterimages)
    {
        // 実体を消した管理している数はそのまま
        delete after;

    }
    // vectorの数を０に
    afterimages.clear();
}
