#include "StageMain.h"
#include "Graphics/Graphics.h"
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
    model = GetActor()->GetComponent<ModelControll>()->GetModel();

    transformid = GetActor()->GetComponent<Transform>();
}



// 更新処理
void StageMain::Update(float elasedTime)
{
    // 今は特にやることなし
    transformid->UpdateTransform();

    model->UpdateTransform(transformid->GetTransform());
    //GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());
}

//// 描画処理
void StageMain::Render(RenderContext& rc)
{
    Graphics& graphics = Graphics::Instance();
    //Shader* shader = graphics.GetShader();
    ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);
    shader->Begin(rc);// シェーダーにカメラの情報を渡す


    shader->Draw(rc, model);

    shader->End(rc);
}

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
