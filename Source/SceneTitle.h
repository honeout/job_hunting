#pragma once
#include "Graphics/Sprite.h"
#include "Scene.h"
class SceneTitle : public Scene
{
public:
    SceneTitle() {};
    ~SceneTitle() override {}

    // ������
    void Initialize() override;

    // �I����
    void Finalize() override;
    // �X�V����
    void Update(float elapsedTime)override;
    // �`�揈��
    void Render() override;

private:

    std::unique_ptr<Sprite> sprite;
    std::unique_ptr<Sprite> spritePush;


    DirectX::XMFLOAT2 position = {500.0f,300.0f};
    DirectX::XMFLOAT2 scale = {0.0f,0.0f};




};