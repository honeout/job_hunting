#pragma once

#include <vector>
#include "Actor.h"
#include <set>

//Projectil１個の弾丸
// 弾丸マネージャー
class ProjectileManager// 複数の弾がん球数
{
public:

    ProjectileManager() {};
    // 誰が持つか分からないから複数の奴が別々に持つかもしれない
    ~ProjectileManager() {};

    // インスタンス取得
    static ProjectileManager& Instance()
    {
        static ProjectileManager instance;
        return instance;
    }




    // 更新処理
    void DeleteUpdate(float elapsedTime) ;

    // 描画処理
    //void Render(const RenderContext& rc, ModelShader* shader);

    // デバッグプリミティブ描画
    void DrawDebugPrimitive();

    // 弾丸登録
    void Register(std::shared_ptr<Actor> projectile);

    // 弾丸全削除
    void Clear();

    // 弾丸数取得
    int GetProjectileCount() const { return static_cast<int> (projectiles.size()); }


    // 弾丸取得
    std::shared_ptr<Actor> GetProjectile(int index) { return projectiles.at(index); }

    // 弾丸削除
    void Remove(std::shared_ptr<Actor> projectile);

private:
    std::vector<std::shared_ptr<Actor>>   projectiles;
    // 特定の弾丸削除だれ削除する
    std::set<std::shared_ptr<Actor>>       removes;
};