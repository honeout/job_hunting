#include "ProjectileManager.h"





//void ProjectileManager::Update(float elapsedTime)
//{
//
//    // 更新処理
//    for (Projectile* projectile : projectiles)
//    {
//        projectile->Update(elapsedTime);
//    }
//
//    // 破棄処理 毎フレームここで一気に消す。
//    for (Projectile* projectile : removes)// 殺しますよリストを殺す
//    {
//        std::vector<Projectile*>::iterator it = std::find(projectiles.begin(), projectiles.end(),
//                                                                                  projectile);
//        if (it != projectiles.end())
//        {
//            projectiles.erase(it);// 削除
//        }
//
//        // 弾丸の破棄処理
//        delete projectile;// 入れ物破棄
//
//    }
//    // 破棄リストをクリア
//    removes.clear();
//
//}

//void ProjectileManager::Render(const RenderContext& rc, ModelShader* shader)
//{
//    for (Actor* projectile : projectiles)
//    {
//        projectile->Render(rc, shader);
//    }
//}
//
//void ProjectileManager::DrawDebugPrimitive()
//{
//
//    // 全ての奴をやってくれ
//    for (Actor* projectile : projectiles)
//    {
//        projectile->DrawDebugPrimitive();
//    }
//}
// 削除関数
void ProjectileManager::DeleteUpdate(float elapsedTime)
{
        // 破棄処理 毎フレームここで一気に消す。
    for (std::shared_ptr<Actor> projectile : removes)// 殺しますよリストを殺す
    {
        std::vector<std::shared_ptr<Actor>>::iterator it = std::find(projectiles.begin(), projectiles.end(),
                                                                     projectile);
        if (it != projectiles.end())
        {
            projectiles.erase(it);// 削除
        }

        // 弾丸の破棄処理
        //delete projectile;// 入れ物破棄

    }
    // 破棄リストをクリア
    removes.clear();
}

void ProjectileManager::Register(std::shared_ptr<Actor> projectile)
{
    // 作ったら後ろから追加していく増えていくだけ
    projectiles.emplace_back(projectile);
}

void ProjectileManager::Clear()
{
        for (std::shared_ptr<Actor>  projectile : projectiles)// 
    {
        // 実体を消した管理している数はそのまま
       // delete projectile;

    }
    // vectorの数を０に
    projectiles.clear();
}

void ProjectileManager::Remove(std::shared_ptr<Actor> projectile)
{
    // 削除登録
    removes.insert(projectile);// 殺すリスト殺す為のリスト
}

//void ProjectileManager::Clear()
//{
//    for (Projectile* projectile : projectiles)// 
//    {
//        // 実体を消した管理している数はそのまま
//        delete projectile;
//
//    }
//    // vectorの数を０に
//    projectiles.clear();
//}

//// 弾丸削除 マネージャーのこの弾丸消して下さい
//void ProjectileManager::Remove(Projectile* projectile)
//{
//    removes.insert(projectile);// 殺すリスト殺す為のリスト
//}
