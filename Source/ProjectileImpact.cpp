#include "ProjectileImpact.h"
#include "ProjectileManager.h"
#include "Graphics/Graphics.h"
// �R���X�g���N�^
ProjectileImpact::ProjectileImpact()
{
    //model = new Model("Data/Model/Sword/Sword.mdl");

    //// ���f�����������̂ŃX�P�[�����O
    //scale.x = scale.y = scale.z = 3.0f;
}
// �f�X�g���N�^
ProjectileImpact::~ProjectileImpact()
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
}

void ProjectileImpact::Start()
{
    // ���f����l
    model = GetActor()->GetComponent<ModelControll>()->GetModel();


    // �e�ړ��̃R���|�[�l���g
    //bulletFiring = GetActor()->GetComponent<BulletFiring>();

    // �g�����X�t�H�[���擾
    transform = GetActor()->GetComponent<Transform>();
    
    // �~�̓����蔻�����
    radiusInSide = 0.1f;

    // �~�̓����蔻��O��
    radiusOutSide = 1.0f;

    // �����蔻������L
    //transform->SetRadius(radius);

    // �������f
    movementCheck = true;

    if (effectProgress)
        effectProgress->Play(transform->GetPosition(), 10.0f);


}

// �X�V����
void ProjectileImpact::Update(float elapsedTime)
{
    if (lifeTimer <= 0)
    {
        Destoroy();
    }

    if (movementCheck)
    {
        //bulletFiring->MoveHoming(moveSpeed, turnSpeed, target, elapsedTime);
        ImpactUpdate();
    }
    transform->UpdateTransformProjectile();

    model->UpdateTransform(transform->GetTransform());

    if (effectProgress)
        effectProgress->SetPosition(effectProgress->GetEfeHandle(), transform->GetPosition());

    if (effectHit)
        effectHit->SetPosition(effectHit->GetEfeHandle(), transform->GetPosition());

    //DrawDebugPrimitive();

    --lifeTimer;

    //// ��������
    //lifeTimer -= elapsedTime;
    //if (lifeTimer <= 0.0f)// �������s�����玩�Q
    //{
    //    // �������폜
    //    Destroy();
    //}

    //// �ړ�
    //{
    //    float moveSpeed = this->moveSpeed * elapsedTime;
    //    // �@�����炩���Z�Ō����ɂ��ꂾ���i������ʒu�ɓ��ꂽ
    //    // �L�΂����������{���Ői
    //    position.x += direction.x*moveSpeed;
    //    position.y += direction.y*moveSpeed;
    //    position.z += direction.z*moveSpeed;
    //}

    //// ����
    //{
    //    float turnSpeed = this->turnSpeed * elapsedTime;

    //    // �^�[�Q�b�g�܂ł̃x�N�g�����Z�o
    //    DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
    //    DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
    //    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);

    //    // �[���x�N�g���łȂ��Ȃ��]�����@�s�b�^�������Ȃ��]�ł��邩��m�F
    //    DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
    //    float lengthSq;
    //    DirectX::XMStoreFloat(&lengthSq, LengthSq);
    //   
    //    if (lengthSq > 0.00001f)
    //    {

    //        // �^�[�Q�b�g�܂ł̃x�N�g����P�ʃx�N�g����
    //        Vec = DirectX::XMVector3Normalize(Vec);

    //        // �����Ă�����x�N�g�����Z�o�@direction�P�ʃx�N�g���O��
    //        DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);

    //        // �O�������x�N�g���ƃ^�[�Q�b�g�܂ł̃x�N�g���̓��ρi�p�x�j���Z�o
    //        DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);
    //  


    //        float dot;
    //        DirectX::XMStoreFloat(&dot, Dot);

    //        // ��]���x�����Ō�̔������s���߂��Ȃ��悤�ɏ������������o����B
    //        // �A�[�N�R�T�C���ł��o���邵������u�ł��̒l������B
    //        // 2�̒P�ʃx�N�g���̊p�x����������
    //        // 1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
    //        //if (dot )
    //        float rot;
    //        // 1.0f�@dot�͂O�ɋ߂Â����P�ɂȂ邩��[���Ƃ��������O�ɂȂ�
    //        rot = 1.0f - dot;
    //        // ������P�̂ق����ł�����΃X�s�[�h������
    //        // �^�[���X�s�[�h��胍�b�g�̕�������������
    //        if (rot > turnSpeed)
    //        {
    //            rot = turnSpeed;
    //        }
    //       

    //        // ��]�p�x������Ȃ��]��������@�����łO������ĂȂ��ƊO�ς��S�������ɂȂ��Čv�Z�o���Ȃ�
    //        if (fabsf(rot) >= 0.0001)
    //        {
    //            // ��]�����Z�o  �O��  ���������������� 
    //            DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction, Vec);
    //            // �덷�h�~�ׂ̈ɒP�ʃx�N�g�������������S
    //            Axis =  DirectX::XMVector3Normalize(Axis);
    //            // ��]���Ɖ�]�ʂ����]�s����Z�o ��]�ʂ����߂Ă���B
    //            DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);
    //            // ���݂̍s�����]������@�������g�̎p��
    //            DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
    //            Transform = DirectX::XMMatrixMultiply(Transform,Rotation) ; // ���������炽���~����  Transform*Rotation
    //            // DirectX::XMMatrixMultrixMultiply
    //            // ��]��̑O�����������o���A�P�ʃx�N�g��������
    //            Direction = DirectX::XMVector3Normalize(Transform.r[2]);// row
    //            DirectX::XMStoreFloat3(&direction,Direction);

    //        }
    //    }
    //}

    //        // �I�u�W�F�N�g�s����X�V
    //        UpdateTransform();

    //        // ���f���s��X�V
    //        model->UpdateTransform(transform);
    //        

    //        


}

// �`�揈��
void ProjectileImpact::Render(RenderContext& rc, ModelShader& shader)
{
    //Graphics& graphics = Graphics::Instance();
    ////Shader* shader = graphics.GetShader();
    ////ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
    //shader = *graphics.GetShader(ModelShaderId::Lanbert);
    //shader.Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��


    //shader.Draw(rc, model);

    //shader.End(rc);
}

void ProjectileImpact::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // ���͉����\�����Ȃ�
        //// �Փ˔���p�̃f�o�b�O����`��
    debugRenderer->DrawSphere(transform->GetPosition(), radiusInSide, DirectX::XMFLOAT4(0, 0, 1, 1));
    debugRenderer->DrawSphere(transform->GetPosition(), radiusOutSide, DirectX::XMFLOAT4(0, 1, 0, 1));

    debugRenderer->DrawCylinder(transform->GetPosition(), 15, transform->GetHeight(), DirectX::XMFLOAT4(0, 1, 1, 1));
}

void ProjectileImpact::Destoroy()
{
    ActorManager::Instance().Remove(GetActor());
    ProjectileManager::Instance().Remove(GetActor());
}

void ProjectileImpact::ImpactUpdate()
{


    // �����蔻�葝��
    radiusInSide += 0.3f;
    
    // �����蔻�葝��
    radiusOutSide += 0.3f;


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