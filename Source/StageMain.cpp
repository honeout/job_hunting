#include "StageMain.h"

// コントラスト
StageMain::StageMain()
{
    // ステージモデルを読み込み
    //model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
}




StageMain::~StageMain()
{
    // ステージモデルを破棄
    //delete model;


}

void StageMain::Start()
{
    // モデルデータを入れる。
    model = GetActor()->GetModel();
}



// 更新処理
void StageMain::Update(float elasedTime)
{
    // 今は特にやることなし
    GetActor()->UpdateTransform();
    //GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());
}

//// 描画処理
//void StageMain::Render(const RenderContext& rc, ModelShader* shader)
//{
//// シェーダーにモデルを描画してもらう
//    shader->Draw(rc, model);
//
//}

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
    HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

void StageManager::Register(Actor* actor)
{
    stages.emplace_back(actor);
}


// ステージ全削除
void StageManager::Clear()
{
    for (Actor* stage : stages)// 
    {
        // 実体を消した管理している数はそのまま
        delete stage;

    }
    // vectorの数を０に
    stages.clear();
}
