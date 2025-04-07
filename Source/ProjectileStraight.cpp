#include "ProjectileStraight.h"
#include "Collision.h"
#include "Graphics/Graphics.h"
// �R���X�g���N�^
ProjectileStraight::ProjectileStraight()    
{
}
// �f�X�g���N�^
ProjectileStraight::~ProjectileStraight()
{
    // �G�t�F�N�g�X�V�폜
    if (effectProgress)
    {
        effectProgress->Stop(effectProgress->GetEfeHandle());
        delete effectProgress;
        effectProgress = nullptr;
    }

    if (effectSpawned)
    {
        effectSpawned->Stop(effectSpawned->GetEfeHandle());
        delete effectSpawned;
        effectSpawned = nullptr;
    }
}
void ProjectileStraight::Start()
{
    // ���f����l
    model = GetActor()->GetComponent<ModelControll>()->GetModel();

    //// �����蔻������L
    //GetActor()->GetComponent<Transform>()->SetRadius(radius);

    // �����蔻������L
    radius = GetActor()->GetComponent<Collision>()->GetRadius();
    
    // �e�ړ��̃R���|�[�l���g
    bulletFiring = GetActor()->GetComponent<BulletFiring>();

    // �g�����X�t�H�[���擾
    transform = GetActor()->GetComponent<Transform>();

    if (effectProgress)
        effectProgress->Play(transform->GetPosition(), 1);

    if (effectSpawned)
        effectSpawned->Play(transform->GetPosition());
}
// �X�V����
void ProjectileStraight::Update(float elapsedTime)
{
    // �����܂Ŕ��
    bulletFiring->Move(speed,elapsedTime);

    transform->UpdateTransformProjectile();

    model->UpdateTransform(transform->GetTransform());


    if (effectProgress)
        effectProgress->SetPosition(effectProgress->GetEfeHandle(), transform->GetPosition());
}

// �`��
void ProjectileStraight::Render(RenderContext& rc, ModelShader& shader)
{
    Graphics& graphics = Graphics::Instance();
    shader = *graphics.GetShader(ModelShaderId::Lanbert);
    shader.Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��
    shader.Draw(rc, model);
    shader.End(rc);
}

void ProjectileStraight::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // ���͉����\�����Ȃ�
        //// �Փ˔���p�̃f�o�b�O����`��
    debugRenderer->DrawSphere(transform->GetPosition(), radius, DirectX::XMFLOAT4(0, 0, 1, 1));
}
#ifdef _DEBUG
void ProjectileStraight::OnGUI()
{

}
#endif // _DEBUG



