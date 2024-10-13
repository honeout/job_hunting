#pragma once


#include "Graphics/Model.h"

#include "Component.h"
#include "Collision.h"
#include "ModelControll.h"
#include "Transform.h"

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

    // 描画処理
    void Render(RenderContext& rc, ModelShader& shader) override;

    // シャドウマップ処理
    void RenderShadowmap(RenderContext& rc) override;

    //void Render(const RenderContext& rc, ModelShader* shader) override;

    // レイキャスト
    bool RayCast(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        HitResult& hit) ;

    //// モデル取得
    //Model* GetModel() const { return model.get(); }

private:
    //Model* model;
    std::weak_ptr<ModelControll> model;
    //std::unique_ptr<Model>	model;

    std::weak_ptr<Transform> transformid;
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

    // 描画
    void Render(RenderContext& rc, ModelShader& shader);

    // 登録
    void Register(std::shared_ptr <Actor> actor);


    // 削除
    void Remove(std::shared_ptr<Actor> stage);


    void Clear();

    // ステージ数取得
    int GetStageCount() const { return static_cast<int>(stages.size()); }

    // エネミー取得
    std::shared_ptr <Actor> GetStage(int index) { return stages.at(index); }

private:
    std::vector<std::shared_ptr<Actor>>  stages;
    // 削除予約
    std::set<std::shared_ptr<Actor>>removes;
};
