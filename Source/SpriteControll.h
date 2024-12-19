#pragma once

#include "Component.h"
#include "Graphics\Sprite.h"

// �X�v���C�g�R���g���[���[
class SpriteControll : public Component
{
public:
    SpriteControll();
    ~SpriteControll() override;

    // ���O�擾
    const char* GetName() const override { return "SpriteControll"; }
#ifdef _DEBUG
    // GUI�`��
    void OnGUI() override;
#endif // _DEBUG

    // ���f���̓ǂݍ���
    void LoadSprite(const char* filename);

    // ���f���̎擾
    Sprite* GetSprite() const { return sprite.get(); }
private:
    std::unique_ptr<Sprite>	sprite;
};