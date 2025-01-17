#pragma once
#include "Graphics/Sprite.h"
#include "CameraController.h"
#include "Scene.h"
class SceneGameOver : public Scene
{
public:
    SceneGameOver() {};
    ~SceneGameOver() override {}

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


    CameraController* cameraControlle = nullptr;

};
