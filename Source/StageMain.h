#pragma once


#include "Graphics/Model.h"

#include "Stage.h"

#include "Component.h"


// ステージ
class StageMain : public Component
{
public:

    StageMain();
    ~StageMain() override;

    // 名前取得
    const char* GetName() const override { return "StageMain"; }

    // 開始処理
    void Start() override;

    // 更新処理
    void Update(float elapsedTime) override;

    //// 描画処理
    //void Render(const RenderContext& rc, ModelShader* shader) override;

    // レイキャスト
    bool RayCast(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        HitResult& hit) ;

    //// モデル取得
    //Model* GetModel() const { return model.get(); }

private:
    Model*	model;
};
