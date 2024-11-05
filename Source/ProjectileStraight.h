#pragma once

#include "Graphics/Model.h"
#include "Projectile.h"
#include "Component.h"
#include "BulletFiring.h"
#include "ModelControll.h"
#include "Transform.h"

// ’¼i’eŠÛ
class ProjectileStraight : public Component
{
public:
    ProjectileStraight();
    ~ProjectileStraight()override;

    // –¼‘Oæ“¾
    const char* GetName() const override { return "ProjectileStraight"; }

    // ŠJnˆ—
    void Start() override;

    // XVˆ— 
    void Update(float elapsedTime) override;

    // •`‰æˆ—
    void Render(RenderContext& rc, ModelShader& shader) override;

    // “–‚½‚è”»’èŒ©‚é
    void DrawDebugPrimitive();

#ifdef _DEBUG
    // GUI•`‰æ
    void OnGUI() override;
#endif // _DEBUG


private:
    Model* model = nullptr;
    float  speed = 10.0f;// ‚P•bŠÔ‚P‚Oƒ[ƒgƒ‹


    // ’eŠÛ¶–½ŠÔ
    float   lifeTimer = 3.0f;

    float stepOffset = 1.0f;

    std::shared_ptr<BulletFiring> bulletFiring;
    std::shared_ptr<Transform> transform;

    float radius = 0.3f;

};
