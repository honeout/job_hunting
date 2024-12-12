#include "StageMain.h"
#include "Graphics/Graphics.h"
// �R���g���X�g
StageMain::StageMain()
{
    // �X�e�[�W���f����ǂݍ���
    //model = new Model("Data/Model/ExampleStage/ExampleStage.mdl");
}




StageMain::~StageMain()
{
    // �X�e�[�W���f����j��
    //delete model;

    //if (transformid)
    //    transformid.reset();
    

}

void StageMain::Start()
{
    // ���f���f�[�^������B
    //model = std::make_unique<Model>(GetActor()->GetComponent<ModelControll>()->GetModel());
    //model = GetActor()->GetComponent<ModelControll>();
    model = GetActor()->GetComponent<ModelControll>()->GetModel();

    transformid = GetActor()->GetComponent<Transform>();
}



// �X�V����
void StageMain::Update(float elasedTime)
{
   
    // ���͓��ɂ�邱�ƂȂ�
    transformid->UpdateTransform();

    model->UpdateTransform(transformid->GetTransform());
    //GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());
   
}

//// �`�揈��
void StageMain::Render(RenderContext& rc, ModelShader& shader)
{
    Graphics& graphics = Graphics::Instance();
    //Shader* shader = graphics.GetShader();
    //ModelShader* shader = graphics.GetShader(  graphics.GetShader(ModelShaderId::Default);
    //ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
    
    // �X�y�L�����[������
    rc.isSpecular = 0;
    
    // �����x���グ��B
    rc.texcoordMult = 20;

    
    // �e�I���I�t
    rc.isRimRightning = 0;


    shader.Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��


    shader.Draw(rc, model);

    shader.End(rc);



}
// �V���h�E�}�b�v
void StageMain::RenderShadowmap(RenderContext& rc)
{

    // �V���h�E�}�b�v
    Graphics& graphics = Graphics::Instance();
    ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);
    shader->Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��
    

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
    // �폜�o�^
    removes.insert(stage);
}

// �X�e�[�W�S�폜
void StageManager::Clear()
{
    for (std::shared_ptr<Actor>& actor  : stages)// 
    {
        // ���̂��������Ǘ����Ă��鐔�͂��̂܂�
        //Remove(actor);
        actor.reset();
    }
    // vector�̐����O��
    stages.clear();
}
