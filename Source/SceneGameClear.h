#pragma once
#include "Graphics/Sprite.h"
#include "Scene.h"
#include <memory>
class SceneGameClear : public Scene
{
public:
    SceneGameClear() {};
    ~SceneGameClear() override {}

    // ������
    void Initialize() override;

    // �I����
    void Finalize() override;
    // �X�V����
    void Update(float elapsedTime)override;
    // �`�揈��
    void Render() override;

private:
    //Sprite* sprite;
    std::unique_ptr<Sprite>	sprite;
    

};
