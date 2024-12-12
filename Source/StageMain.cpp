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

    //if (transformid)
    //    transformid.reset();
    

}

void StageMain::Start()
{
    // モデルデータを入れる。
    //model = std::make_unique<Model>(GetActor()->GetComponent<ModelControll>()->GetModel());
    //model = GetActor()->GetComponent<ModelControll>();
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
void StageMain::Render(RenderContext& rc, ModelShader& shader)
{
    Graphics& graphics = Graphics::Instance();
    //Shader* shader = graphics.GetShader();
    //ModelShader* shader = graphics.GetShader(  graphics.GetShader(ModelShaderId::Default);
    //ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
    
    // スペキュラー無効化
    rc.isSpecular = 0;
    
    // 解消度を上げる。
    rc.texcoordMult = 20;

    
    // 影オンオフ
    rc.isRimRightning = 0;


    shader.Begin(rc);// シェーダーにカメラの情報を渡す


    shader.Draw(rc, model);

    shader.End(rc);



}
// シャドウマップ
void StageMain::RenderShadowmap(RenderContext& rc)
{

    // シャドウマップ
    Graphics& graphics = Graphics::Instance();
    ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);
    shader->Begin(rc);// シェーダーにカメラの情報を渡す
    

    shader->Draw(rc, model);

    shader->End(rc);

}

bool StageMain::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end,
    HitResult& hit)
{
    return Collision::IntersectRayVsModel(start, end, model, hit);
}

void StageManager::Register(std::shared_ptr <Actor> actor)
{
    stages.emplace_back(actor);
}


void StageManager::Remove(std::shared_ptr<Actor> stage)
{
    // 削除登録
    removes.insert(stage);
}

// ステージ全削除
void StageManager::Clear()
{
    for (std::shared_ptr<Actor>& actor  : stages)// 
    {
        // 実体を消した管理している数はそのまま
        //Remove(actor);
        actor.reset();
    }
    // vectorの数を０に
    stages.clear();
}
