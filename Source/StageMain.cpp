#include "StageMain.h"

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
    model = GetActor()->GetModel();
}



// �X�V����
void StageMain::Update(float elasedTime)
{
    // ���͓��ɂ�邱�ƂȂ�
    GetActor()->UpdateTransform();
    //GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());
}

//// �`�揈��
//void StageMain::Render(const RenderContext& rc, ModelShader* shader)
//{
//// �V�F�[�_�[�Ƀ��f����`�悵�Ă��炤
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
