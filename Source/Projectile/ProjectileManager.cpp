#include "ProjectileManager.h"

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
    
            projectile.reset();
    }
    // vectorの数を０に
    projectiles.clear();
}

void ProjectileManager::Remove(std::shared_ptr<Actor> projectile)
{
    // 削除登録
    removes.insert(projectile);// 殺すリスト殺す為のリスト
}
