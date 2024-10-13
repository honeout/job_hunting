#pragma once
#include "Graphics/Sprite.h"
#include "Scene.h"
#include <memory>
class SceneTitle : public Scene
{
public:
    SceneTitle() {};
    ~SceneTitle() override {}

    // ‰Šú‰»
    void Initialize() override;

    // I—¹‰»
    void Finalize() override;
    // XVˆ—
    void Update(float elapsedTime)override;
    // •`‰æˆ—
    void Render() override;

private:
    std::unique_ptr<Sprite> sprite;
    //Sprite* sprite = nullptr;
};