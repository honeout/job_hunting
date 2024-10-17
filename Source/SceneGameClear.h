#pragma once
#include "Graphics/Sprite.h"
#include "Scene.h"
class SceneGameClear : public Scene
{
public:
    SceneGameClear() {};
    ~SceneGameClear() override {}

    // ‰Šú‰»
    void Initialize() override;

    // I—¹‰»
    void Finalize() override;
    // XVˆ—
    void Update(float elapsedTime)override;
    // •`‰æˆ—
    void Render() override;

private:
    Sprite* sprite = nullptr;

};
