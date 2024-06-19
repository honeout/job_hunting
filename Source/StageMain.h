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

// ステージマネージャー
class StageManager
{
private:
    StageManager() {}
    ~StageManager() {};

public:
    // インスタンス取得
    static StageManager& Instance()
    {
        static StageManager instance;
        return instance;
    }


    // 削除
    void Remove(std::shared_ptr<Actor> actor);

    // 更新
    void Update(float elapsedTime);

    // 行列更新
    void UpdateTransform();

    // 描画
    void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

    // 登録
    void Register(Actor* actor);

    void Clear();

    // ステージ数取得
    int GetStageCount() const { return static_cast<int>(stages.size()); }

    // エネミー取得
    Actor* GetStage(int index) { return stages.at(index); }

private:
    std::vector<Actor*> stages;
};
