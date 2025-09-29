#pragma once
#include <vector>
#include "Component\Actor.h"
#include <set>

namespace projectileConfig
{
    // ’eŠÛ‘å‚«‚³
    constexpr DirectX::XMFLOAT3 scale = { 0.01f,0.01f,0.01f };
    constexpr DirectX::XMFLOAT3 scaleImpact = { 3.0f,3.0f,3.0f };

    // õ–½
    constexpr float lifeTimerEImpact = 50.0f;
    // ‰Šõ–½
    constexpr float lifeTimePFire = 3.0f;
    // •Xõ–½
    constexpr float lifeTimePIce = 4.0f;
    // —‹õ–½
    constexpr float lifeTimePLightning = 0.5f;
};

//Projectil‚PŒÂ‚Ì’eŠÛ
// ’eŠÛƒ}ƒl[ƒWƒƒ[
class ProjectileManager// •¡”‚Ì’e‚ª‚ñ‹…”
{
public:
    ProjectileManager() {};
    // ’N‚ª‚Â‚©•ª‚©‚ç‚È‚¢‚©‚ç•¡”‚Ì“z‚ª•ÊX‚É‚Â‚©‚à‚µ‚ê‚È‚¢
    ~ProjectileManager() {};

    // ƒCƒ“ƒXƒ^ƒ“ƒXæ“¾
    static ProjectileManager& Instance()
    {
        static ProjectileManager instance;
        return instance;
    }

    // XVˆ—
    void DeleteUpdate(float elapsedTime) ;

    // ’eŠÛ“o˜^
    void Register(std::shared_ptr<Actor> projectile);

    // ’eŠÛ‘Síœ
    void Clear();

    // ’eŠÛ”æ“¾
    int GetProjectileCount() const { return static_cast<int> (projectiles.size()); }

    // ’eŠÛæ“¾
    std::shared_ptr<Actor> GetProjectile(int index) { return projectiles.at(index); }

    // ’eŠÛíœ
    void Remove(std::shared_ptr<Actor> projectile);
private:
    std::vector<std::shared_ptr<Actor>>   projectiles;
    // “Á’è‚Ì’eŠÛíœ‚¾‚êíœ‚·‚é
    std::set<std::shared_ptr<Actor>>       removes;
};