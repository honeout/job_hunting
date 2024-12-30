#include "ProjectileTornade.h"
#include "Graphics/Graphics.h"


// �R���X�g���N�^
ProjectileTornade::ProjectileTornade()
{
    height = 0.0f;
}
// �f�X�g���N�^
ProjectileTornade::~ProjectileTornade()
{

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
}

void ProjectileTornade::Start()
{
    // ���f����l
    model = GetActor()->GetComponent<ModelControll>()->GetModel();

    // �����蔻������L
    GetActor()->GetComponent<Transform>()->SetRadius(radius);

    // �e�ړ��̃R���|�[�l���g
    bulletFiring = GetActor()->GetComponent<BulletFiring>();

    // �g�����X�t�H�[���擾
    transform = GetActor()->GetComponent<Transform>();


    if (effectProgress)
        effectProgress->Play(transform->GetPosition(), 1);

    
}

// �X�V����
void ProjectileTornade::Update(float elapsedTime)
{

    transform->UpdateTransformProjectile();

    model->UpdateTransform(transform->GetTransform());

    //DrawDebugPrimitive();

    if (effectProgress)
        effectProgress->SetPosition(effectProgress->GetEfeHandle(), transform->GetPosition());

    if (effectHit)
        effectHit->SetPosition(effectHit->GetEfeHandle(), transform->GetPosition());

    
}

// �`�揈��
void ProjectileTornade::Render(RenderContext& rc, ModelShader& shader)
{
    Graphics& graphics = Graphics::Instance();
    //Shader* shader = graphics.GetShader();
    //ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
    shader = *graphics.GetShader(ModelShaderId::Lanbert);
    shader.Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��


    shader.Draw(rc, model);

    shader.End(rc);
}

void ProjectileTornade::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // ���͉����\�����Ȃ�
        //// �Փ˔���p�̃f�o�b�O����`��

    debugRenderer->DrawCylinder(transform->GetPosition(), radius, transform->GetHeight(), DirectX::XMFLOAT4(0, 0, 1, 1));
}

void ProjectileTornade::EffectProgressPlay()
{
    effectProgress->Play(transform->GetPosition(), scale);
}

void ProjectileTornade::EffectHitPlay(float elapsedTime)
{
    effectHit->Play(transform->GetPosition(), scale);
}

void ProjectileTornade::EffectProgressUpdate(float elapsedTime)
{
    effectProgress->SetPosition(effectProgress->GetEfeHandle(),
        transform->GetPosition());
}

void ProjectileTornade::EffectHitUpdate(float elapsedTime)
{
    effectHit->SetPosition(effectHit->GetEfeHandle(),
        transform->GetPosition());
}
