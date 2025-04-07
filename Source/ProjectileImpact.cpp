#include "ProjectileImpact.h"
#include "ProjectileManager.h"
#include "Graphics/Graphics.h"
// �R���X�g���N�^
ProjectileImpact::ProjectileImpact()
{
}
// �f�X�g���N�^
ProjectileImpact::~ProjectileImpact()
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

void ProjectileImpact::Start()
{
    // ���f����l
    model = GetActor()->GetComponent<ModelControll>()->GetModel();

    // �g�����X�t�H�[���擾
    transform = GetActor()->GetComponent<Transform>();

    // �R���W�����擾
    collision = GetActor()->GetComponent<Collision>();
    
    // �~�̓����蔻�����
    radiusInSide = 0.1f;

    // �~�̓����蔻��O��
    radiusOutSide = 1.0f;

    // �����蔻������L
    //transform->SetRadius(radius);

    // �������f
    movementCheck = true;

    scale = 10.0f;
    effectProgress->SetScale(effectProgress->GetEfeHandle(), { scale,scale,scale });
   
    if (effectProgress)
        effectProgress->Play(transform->GetPosition(), scale);
}

// �X�V����
void ProjectileImpact::Update(float elapsedTime)
{
    if (lifeTimer <= 0)
    {
        Destoroy();
    }   
    ImpactUpdate();
    transform->UpdateTransformProjectile();
    model->UpdateTransform(transform->GetTransform());
    if (effectProgress)
        effectProgress->SetPosition(effectProgress->GetEfeHandle(), transform->GetPosition());

    if (effectHit)
        effectHit->SetPosition(effectHit->GetEfeHandle(), transform->GetPosition());
    --lifeTimer;
}

// �`�揈��
void ProjectileImpact::Render(RenderContext& rc, ModelShader& shader)
{
    Graphics& graphics = Graphics::Instance();
    //Shader* shader = graphics.GetShader();
    //ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
    shader = *graphics.GetShader(ModelShaderId::Lanbert);
    shader.Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��
    shader.Draw(rc, model);
    shader.End(rc);
}

void ProjectileImpact::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // ���͉����\�����Ȃ�
        //// �Փ˔���p�̃f�o�b�O����`��
    debugRenderer->DrawSphere(transform->GetPosition(), radiusInSide, DirectX::XMFLOAT4(0, 0, 1, 1));
    debugRenderer->DrawSphere(transform->GetPosition(), radiusOutSide, DirectX::XMFLOAT4(0, 1, 0, 1));

    debugRenderer->DrawCylinder(transform->GetPosition(), 15, collision.lock()->GetHeight(), DirectX::XMFLOAT4(0, 1, 1, 1));
}

void ProjectileImpact::Destoroy()
{
    ActorManager::Instance().Remove(GetActor());
    ProjectileManager::Instance().Remove(GetActor());
}

void ProjectileImpact::ImpactUpdate()
{
    // �����蔻�葝��
    radiusInSide += 0.13f;
    
    // �����蔻�葝��
    radiusOutSide += 0.13f;
}

void ProjectileImpact::EffectProgressPlay()
{
    effectProgress->Play(transform->GetPosition(), scale);
}

void ProjectileImpact::EffectHitPlay(float elapsedTime)
{
    effectHit->Play(transform->GetPosition(), scale);
}

void ProjectileImpact::EffectProgressUpdate(float elapsedTime)
{
    effectProgress->SetPosition(effectProgress->GetEfeHandle(),
        transform->GetPosition());
}

void ProjectileImpact::EffectHitUpdate(float elapsedTime)
{
    effectHit->SetPosition(effectHit->GetEfeHandle(),
        transform->GetPosition());
}