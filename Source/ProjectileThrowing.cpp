#include "ProjectileThrowing.h"
#include "Graphics/Graphics.h"
// �R���X�g���N�^
ProjectileThrowing::ProjectileThrowing()
{
    //model = new Model("Data/Model/Sword/Sword.mdl");

    //// ���f�����������̂ŃX�P�[�����O
    //scale.x = scale.y = scale.z = 3.0f;
}
// �f�X�g���N�^
ProjectileThrowing::~ProjectileThrowing()
{
    //delete model;

    // �G�t�F�N�g�X�V�폜
    if (effectProgress)
    {
        effectProgress->Stop(effectProgress->GetEfeHandle());
        delete effectProgress;
        effectProgress = nullptr;
    }

    // �G�t�F�N�g�q�b�g�폜
    if (effectHit)
    {
        effectHit->Stop(effectHit->GetEfeHandle());
        delete effectHit;
        effectHit = nullptr;
    }
    counterCheck = false;
}

void ProjectileThrowing::Start()
{
    // ���f����l
    model = GetActor()->GetComponent<ModelControll>()->GetModel();

    //// �����蔻������L
    //GetActor()->GetComponent<Transform>()->SetRadius(radius);

    // �e�ړ��̃R���|�[�l���g
    bulletFiring = GetActor()->GetComponent<BulletFiring>();

    // �g�����X�t�H�[���擾
    transform = GetActor()->GetComponent<Transform>();

    // �������f
    //movementCheck = true;

    if (effectProgress)
        effectProgress->Play(transform->GetPosition(), 1);

    radius = transform->GetRadius();
}

// �X�V����
void ProjectileThrowing::Update(float elapsedTime)
{
    if (movementCheck)
        bulletFiring->Move(moveSpeed,elapsedTime);
        //bulletFiring->MoveHoming(moveSpeed, turnSpeed, target, elapsedTime);

    transform->UpdateTransformProjectile();

    model->UpdateTransform(transform->GetTransform());

    if (effectProgress)
        effectProgress->SetPosition(effectProgress->GetEfeHandle(), transform->GetPosition());

    if (effectHit)
        effectHit->SetPosition(effectHit->GetEfeHandle(), transform->GetPosition());

    //DrawDebugPrimitive();
}

// �`�揈��
void ProjectileThrowing::Render(RenderContext& rc, ModelShader& shader)
{
    Graphics& graphics = Graphics::Instance();
    //Shader* shader = graphics.GetShader();
    //ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
    shader = *graphics.GetShader(ModelShaderId::Lanbert);
    shader.Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��


    shader.Draw(rc, model);

    shader.End(rc);
}

void ProjectileThrowing::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // ���͉����\�����Ȃ�
        //// �Փ˔���p�̃f�o�b�O����`��
    debugRenderer->DrawSphere(transform->GetPosition(), radius, DirectX::XMFLOAT4(0, 1, 1, 1));
    /*debugRenderer->DrawSphere(
        { 
            transform->GetPosition().x,
            transform->GetPosition().y,
            transform->GetPosition().z + transform->GetHeight()
        },
        radius, DirectX::XMFLOAT4(0, 1, 1, 1));

    debugRenderer->DrawSphere(
        {
            transform->GetPosition().x,
            transform->GetPosition().y,
            transform->GetPosition().z - transform->GetHeight()
        },
        radius, DirectX::XMFLOAT4(0, 1, 1, 1));*/
    //debugRenderer->DrawCylinder(transform->GetPosition(), radius, transform->GetHeight(), DirectX::XMFLOAT4(1, 0, 0, 1));
}
#ifdef _DEBUG
void ProjectileThrowing::OnGUI()
{
    ImGui::InputFloat("Radius", &radius);
}
#endif // _DEBUG
void ProjectileThrowing::EffectProgressPlay()
{
    effectProgress->Play(transform->GetPosition(), scale);
}

void ProjectileThrowing::EffectHitPlay(float elapsedTime)
{
    effectHit->Play(transform->GetPosition(), scale);
}

void ProjectileThrowing::EffectProgressUpdate(float elapsedTime)
{
    effectProgress->SetPosition(effectProgress->GetEfeHandle(),
        transform->GetPosition());
}

void ProjectileThrowing::EffectHitUpdate(float elapsedTime)
{
    effectHit->SetPosition(effectHit->GetEfeHandle(),
        transform->GetPosition());
}
//
//void ProjectileThrowing::Lanch(const DirectX::XMFLOAT3& direction,
//    const DirectX::XMFLOAT3& position,
//    const DirectX::XMFLOAT3& target)
//{
//    this->direction = direction;
//    this->position = position;
//    this->target = target;
//
//    //UpdateTransform();
//}
