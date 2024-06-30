#pragma once

#include "Component.h"
#include "Graphics\Sprite.h"

// モデルコントローラー
class SpriteControll : public Component
{
public:
    SpriteControll();
    ~SpriteControll() override;

    // 名前取得
    const char* GetName() const override { return "Sprite"; }

    //// 開始処理
    //void Start() override;

    //// 更新処理
    //void Update(float elapsedTime) override;

    //// GUI表示
    //void OnGUI() override;

    // モデルの読み込み
    void LoadSprite(const char* filename);

    // モデルの取得
    Sprite* GetSprite() const { return sprite.get(); }
private:
    std::unique_ptr<Sprite>	sprite;
};