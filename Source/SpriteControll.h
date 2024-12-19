#pragma once

#include "Component.h"
#include "Graphics\Sprite.h"

// スプライトコントローラー
class SpriteControll : public Component
{
public:
    SpriteControll();
    ~SpriteControll() override;

    // 名前取得
    const char* GetName() const override { return "SpriteControll"; }
#ifdef _DEBUG
    // GUI描画
    void OnGUI() override;
#endif // _DEBUG

    // モデルの読み込み
    void LoadSprite(const char* filename);

    // モデルの取得
    Sprite* GetSprite() const { return sprite.get(); }
private:
    std::unique_ptr<Sprite>	sprite;
};