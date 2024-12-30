#pragma once
#include "Graphics/Sprite.h"
#include "Scene.h"
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
    std::unique_ptr<Sprite> spritePush;


    DirectX::XMFLOAT2 position = {500.0f,300.0f};
    DirectX::XMFLOAT2 scale = {0.0f,0.0f};




};