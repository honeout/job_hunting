#pragma once

#include "Component.h"
#include "Graphics\Model.h"

// モデルコントローラー
class ModelControll : public Component
{
public:
    ModelControll();
    ~ModelControll() override;

    // 名前取得
    const char* GetName() const override { return "Model"; }

    // 開始処理
    void Start() override;

    // 更新処理
    void Update(float elapsedTime) override;

    // GUI表示
    void OnGUI() override;

    // モデルの読み込み
    void LoadModel(const char* filename);

    // モデルの取得
    Model* GetModel() const { return model.get(); }
private:
    std::unique_ptr<Model>	model;
};