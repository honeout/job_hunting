#pragma once
#include "Graphics/Sprite.h"
#include "Scene.h"
#include <memory>
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
    //Sprite* sprite = nullptr;
};