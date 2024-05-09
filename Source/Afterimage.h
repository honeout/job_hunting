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
    //純粋仮想関数を作ったら向こうで作らないと
    //コンパイルエラーになる
    //
    virtual void Update(float elapsedTime) = 0;

    // 描画処理　
    virtual void Render(ID3D11DeviceContext* dc, Shader* shader) = 0;

    // 破棄
    void Destroy();

    // デバッグプリミティブ描画
    virtual void DrawDebugPrimitive();


};