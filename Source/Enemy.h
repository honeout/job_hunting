#pragma once

#include "Graphics/Shader.h"
#include "Character.h"
#include "Movement.h"
#include "HP.h"
#include "Actor.h"
#include "Component.h"

// エネミー
class Enemy : public Character
{
public:
    Enemy() {}
    ~Enemy() override {}


    virtual void Start() = 0;
    // 更新処理
    //純粋仮想関数を作ったら向こうで作らないと
    //コンパイルエラーになる
    //
    virtual void Update(float elapsedTime) = 0;

    // 描画処理　
    virtual void Render(const RenderContext& rc, ModelShader* shader) = 0;

    // 破棄
    void Destroy();

    // デバッグプリミティブ描画
    virtual void DrawDebugPrimitive();

private:

};