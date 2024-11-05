#include "ProjectileManager.h"




// íœŠÖ”
void ProjectileManager::DeleteUpdate(float elapsedTime)
{
        // ”jŠüˆ— –ˆƒtƒŒ[ƒ€‚±‚±‚Åˆê‹C‚ÉÁ‚·B
    for (std::shared_ptr<Actor> projectile : removes)// E‚µ‚Ü‚·‚æƒŠƒXƒg‚ğE‚·
    {
        std::vector<std::shared_ptr<Actor>>::iterator it = std::find(projectiles.begin(), projectiles.end(),
                                                                     projectile);
        if (it != projectiles.end())
        {
            projectiles.erase(it);// íœ
        }

        // ’eŠÛ‚Ì”jŠüˆ—
        //delete projectile;// “ü‚ê•¨”jŠü

    }
    // ”jŠüƒŠƒXƒg‚ğƒNƒŠƒA
    removes.clear();
}

void ProjectileManager::Register(std::shared_ptr<Actor> projectile)
{
    // ì‚Á‚½‚çŒã‚ë‚©‚ç’Ç‰Á‚µ‚Ä‚¢‚­‘‚¦‚Ä‚¢‚­‚¾‚¯
    projectiles.emplace_back(projectile);
}

void ProjectileManager::Clear()
{
        for (std::shared_ptr<Actor>  projectile : projectiles)// 
    {
        // À‘Ì‚ğÁ‚µ‚½ŠÇ—‚µ‚Ä‚¢‚é”‚Í‚»‚Ì‚Ü‚Ü
    
            projectile.reset();
    }
    // vector‚Ì”‚ğ‚O‚É
    projectiles.clear();
}

void ProjectileManager::Remove(std::shared_ptr<Actor> projectile)
{
    // íœ“o˜^
    removes.insert(projectile);// E‚·ƒŠƒXƒgE‚·ˆ×‚ÌƒŠƒXƒg
}
