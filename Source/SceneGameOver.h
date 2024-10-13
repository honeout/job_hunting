#pragma once

#include "Graphics/Sprite.h"
#include "Scene.h"
#include <memory>
class SceneGameOver : public Scene
{
public:
    SceneGameOver() {};
    ~SceneGameOver() override {}

    // ������
    void Initialize() override;

    // �I����
    void Finalize() override;
    // �X�V����
    void Update(float elapsedTime)override;
    // �`�揈��
    void Render() override;

private:
    std::unique_ptr<Sprite>	sprite;
    //Sprite* sprite;
};
