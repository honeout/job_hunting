#pragma once

#include "Component.h"
#include "Graphics\Sprite.h"

// ���f���R���g���[���[
class SpriteControll : public Component
{
public:
    SpriteControll();
    ~SpriteControll() override;

    // ���O�擾
    const char* GetName() const override { return "SpriteControll"; }

    //// GUI�\��
    void OnGUI() override;

    // ���f���̓ǂݍ���
    void LoadSprite(const char* filename);

    // ���f���̎擾
    Sprite* GetSprite() const { return sprite.get(); }
private:
    std::unique_ptr<Sprite>	sprite;
};