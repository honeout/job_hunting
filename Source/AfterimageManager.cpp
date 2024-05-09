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
    // 注意projectilesの範囲for文中でerase()すると不具合が発生してしまうため、
    //  更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する。
    for (Afterimage* after : removes)// 殺しますよリストを殺す
    {
        // std::vectorから要素を削除する場合はイテレーターで削除しなければならない
        //      イテレーターの為にstd::findを使う              (enemiesの最初から最後まで調べてenemyがあったら)
        std::vector<Afterimage*>::iterator it = std::find(afterimages.begin(), afterimages.end(),
            after);//vectorからenemy探して出す
        if (it != afterimages.end())// 見つかったら
        {
            //erase関数でvector要素を削除、破棄リストのポインタからイテレーターを検索し、関数に渡す
            afterimages.erase(it);// 削除
            // 管理課から消えただけ
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

void AfterimageManager::DrawDebugPrimitive()
{
    // 全ての奴をやってくれ
    for (Afterimage* after : afterimages)
    {
        after->DrawDebugPrimitive();
    }
}


// 描画処理
void AfterimageManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Afterimage* after : afterimages)
    {
        after->Render(context, shader);
    }
}

void AfterimageManager::Remove(Afterimage* after)
{
    // vectorとは違うsetは削除これ
    // 破棄リストに追加 直接enemies要素を消すと不具合が起こるから破棄リストに登録
    removes.insert(after);// 殺すリスト殺す為のリスト
    // 何故ここで消さないここで消したらfor中にやってしまうから
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

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        // トランスフォーム
        if (ImGui::CollapsingHeader("Enemy", ImGuiTreeNodeFlags_DefaultOpen))
        {

            //  ImGui::InputFloat3("Position", &);


            ImGui::Button("Add");

            //for (Enemy* enemy : enemies)
            //{
            //    enemies.emplace_back(enemy);
            //}





        }
    }
    ImGui::End();
}

// エネミー全削除
void AfterimageManager::Clear()
{
    for (Afterimage* after : afterimages)// 
    {
        // 実体を消した管理している数はそのまま
        delete after;

    }
    // vectorの数を０に
    afterimages.clear();
}
