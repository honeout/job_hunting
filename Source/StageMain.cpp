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


}

void StageMain::Start()
{
    // ���f���f�[�^������B
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
void StageMain::Render(RenderContext& rc)
{
    Graphics& graphics = Graphics::Instance();
    //Shader* shader = graphics.GetShader();
    ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);
    shader->Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��


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


// �X�e�[�W�S�폜
void StageManager::Clear()
{
    for (Actor* stage : stages)// 
    {
        // ���̂��������Ǘ����Ă��鐔�͂��̂܂�
        delete stage;

    }
    // vector�̐����O��
    stages.clear();
}
