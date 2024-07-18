#pragma once

#include "Component.h"
#include "SpriteControll.h"
#include "TransForm2D.h"

class Ui : public Component
{
public:
    Ui() {};
    ~Ui() override;

    // 名前取得
    const char* GetName() const override { return "Ui"; }

    // 開始処理
    void Start() override;

    // 更新処理
    void Update(float elapsedTime) override;

    // 描画処理
    void Render(RenderContext& rc, ModelShader& shader) override;

    // GUI描画
    void OnGUI() override;
private:
    Sprite* sprite = nullptr;

    // 位置や動き
    std::shared_ptr<TransForm2D> transForm2D;
};