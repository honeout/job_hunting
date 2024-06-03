#pragma once

#include "Graphics/Shader.h"
#include "Character.h"

// エネミー
class Afterimage : public Character
{
public:
    Afterimage() {}
    ~Afterimage() override {}

    // 更新処理
    virtual void Update(float elapsedTime) = 0;

    // 描画処理　
    virtual void Render(RenderContext& context, ModelShader* shade) = 0;

    // 破棄
    void Destroy();

    // デバッグプリミティブ描画
    virtual void DrawDebugPrimitive();


};