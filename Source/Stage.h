#pragma once

#include "Graphics/Shader.h"
#include "Collision.h"

// ステージ
class Stage
{
public:
    Stage() {}
    virtual ~Stage() {}

    // 更新処理
    virtual void Update(float elapsedTime) = 0;

    // 描画処理
    virtual void Render(const RenderContext& rc, ModelShader* shader) = 0;

    // レイキャスト　
    virtual bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
        HitResult& hit) = 0;
};
