#include <imgui.h>
#include "EnemySlime.h"
//#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
//#include "Collision.h"
#include "StateDerived.h"

#include "TransForm2D.h"

#include "ProjectileImpact.h"
#include "ProjectileRuby.h"
#include "ProjectileThrowing.h"


#include "UiManager.h"
#include "Ui.h"


// �f�X�g���N�^
EnemySlime::~EnemySlime()
{
   // delete model;
    //if (stateMachine != nullptr)
    //{
    //    delete stateMachine;
    //    stateMachine = nullptr;
    //}
    //if (movement)
    //movement.reset();
    //if (hp)
    //hp.reset();
    //if (transform)
    //transform.reset();
    if (stateMachine)
    {
        stateMachine;
    }

}

void EnemySlime::Start()
{
    // ���[�u�����g�֐����g����悤��
    movement = GetActor()->GetComponent<Movement>();

    // hp�֐����g����悤��
    hp = GetActor()->GetComponent<HP>();

    // transform�֐����g����悤��
    transform = GetActor()->GetComponent<Transform>();

    // ���f���f�[�^������B
    //model = std::make_unique<Model>(GetActor()->GetComponent<ModelControll>()->GetModel());
    model = GetActor()->GetComponent<ModelControll>()->GetModel();
    

    hp->SetHealth(health);

    hp->SetMaxHealth(maxHealth);

    transform->SetRadius(radius);

    transform->SetHeight(height);

    // �p�j�X�e�[�g�֑J��
    //TransitionWanderState();

    //SetTerritory(position, territoryarea);

    stateMachine = std::make_shared<StateMachine>();



    //stateMachine = new StateMachine();

    // �X�e�[�g�}�V���ɃX�e�[�g�o�^
    stateMachine->RegisterState(new WanderState(GetActor().get()));
    stateMachine->RegisterState(new IdleState(GetActor().get()));
    stateMachine->RegisterState(new PursuitState(GetActor().get()));
    stateMachine->RegisterState(new JumpState(GetActor().get()));
    stateMachine->RegisterState(new AttackState(GetActor().get()));
    stateMachine->RegisterState(new AttackShotState(GetActor().get()));
    stateMachine->RegisterState(new AttackShotThrowingState(GetActor().get()));
    stateMachine->RegisterState(new ConfusionState(GetActor().get()));
    stateMachine->RegisterState(new DamageState(GetActor().get()));
    stateMachine->RegisterState(new DeathState(GetActor().get()));
    

    // �X�e�[�g�Z�b�g
    stateMachine->SetState(static_cast<int>(State::Idle));


    // �A�j���[�V�������[��
    updateanim = UpAnim::Normal;

    //// �㔼�g
    //bornUpStartPoint = "mixamorig:Spine";
    //// �����g
    //bornDownerEndPoint = "mixamorig:Spine";

    // �㔼�g�X�^�[�g�Đ��J�n�ꏊ
    bornUpStartPoint = "body3";
    // �㔼�g�G���h�Đ���~�ꏊ
    bornUpEndPoint = "body2";
    // �����g�X�^�[�g�Đ��J�n�ꏊ
    bornDownerStartPoint = "body2";
    // �����g�G���h�Đ���~�ꏊ
    bornDownerEndPoint = "boss_left_eye";


    // �����蔻�薳������
    invalidJudgment = true;

    // player�J�E���^�[�p
    counterJudgment = false;

    // �U���E�����邩�ǂ���
    attackRightFootRange = 10.5f;

    // ����`�F�b�N
    moveCheck = true;
}

// �X�V����
void EnemySlime::Update(float elapsedTime)
{

    //// ���삷�邩�ǂ���
    //if (moveCheck)
    ////// �X�e�[�g���̏���
    //stateMachine->Update(elapsedTime);

    // �ʒu
    position = transform->GetPosition();
    // ����
    angle = transform->GetAngle();
    // �傫��
    scale = transform->GetScale();

    // ���͏����X�V
    movement->UpdateVelocity(elapsedTime);
    // ���G���ԍX�V
    hp->UpdateInbincibleTimer(elapsedTime);

    //DrawDebugPrimitive();

    // �����蔻��Ռ��g�ƃv���C���[
    CollisionImpactVsPlayer();

    // �����蔻�胋�r�[�ƃv���C���[
    CollisionRubyVsPlayer();

    // �����蔻�艡
    CollisionRubyWidthVsPlayer();

    //// �n�ʂƒe�ۂ̓����蔻��
    //if (pushuThrow)
    //{
    //    CollisionRubyWidthVsOnGraound();
    //}

    // �폜
    ProjectileManager::Instance().DeleteUpdate(elapsedTime);//todo ����́H

    transform->UpdateTransform();

    // �_���[�W�_��
    OnHit(elapsedTime);

    // ���삷�邩�ǂ���
    if (moveCheck)
    // ���[�V�����X�V����
    switch (updateanim)
    {
            // �ʏ�A�j���[�V����
        case UpAnim::Normal:
        {
            // �A�j���[�V�����Đ�
            model->UpdateAnimation(elapsedTime, true);
            break;
        }
        // �����Đ�
        case UpAnim::Doble:
        {
            // ���f�������A�j���[�V�����X�V����
            model->UpdateUpeerBodyAnimation(elapsedTime, bornUpStartPoint, bornUpEndPoint, true);
            model->UpdateLowerBodyAnimation(elapsedTime, bornDownerStartPoint, bornDownerEndPoint, true);
            break;
        }
        // �����u�����h�Đ�
        case UpAnim::Blend:
        {
            // ���f�������u�����h�A�j���[�V�����X�V����
            model->Update_blend_animations(elapsedTime, true);
            break;
        }
        // �t�Đ�
        case UpAnim::Reverseplayback:
        {
            model->ReverseplaybackAnimation(elapsedTime, true);
            break;
        }
    }


    model->UpdateTransform(transform->GetTransform());
    

    // �Q�[�W�Ǘ�
    UiControlle(elapsedTime);


}



void EnemySlime::Render(RenderContext& rc, ModelShader& shader)
{
 
    
        Graphics& graphics = Graphics::Instance();
        //Shader* shader = graphics.GetShader();
        //ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
        // �ԐF
        rc.color = colorGB;

        // �X�y�L�����[������
        rc.isSpecular = 0;
        
        shader.Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��

       // if (modelDrawCheck)
    
        shader.Draw(rc, model);

        shader.End(rc);
    

}
#ifdef _DEBUG
void EnemySlime::OnGUI()
{
    if(ImGui::Button("drawtrue"))
    {
        modelDrawCheck = true;
    }

    if (ImGui::Button("drawfalse"))
    {
        modelDrawCheck = false;
    }


    //ImGui::InputFloat("Bullet", &moveSpeed);
}
#endif // _DEBUG


// �f�o�b�O�v���~�e�B�u�`��
void EnemySlime::DrawDebugPrimitive()
{
    // ���N���X�̃f�o�b�O�v���~�e�B�u�`��
    //Enemy::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // �꒣��͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f,
        DirectX::XMFLOAT4(0, 1, 0, 1));

    // �w�m�F
    //debugRenderer->DrawCylinder(position, 1.0f, height,
    //    DirectX::XMFLOAT4(1, 1, 0, 1));

    debugRenderer->DrawCylinder(
        {
            position.x,
            position.y + height / 2,
            position.z,
        }, radius, height / 2, DirectX::XMFLOAT4(1, 0, 0, 1));


    // �^�[�Q�b�g�ʒu���f�o�b�O���`��
    debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1, 1, 0, 1));
    
    // �U���E�����邩�ǂ���
    debugRenderer->DrawSphere(position, attackRightFootRange, DirectX::XMFLOAT4(1, 0, 1, 1));
    
    // ���G�͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0, 0, 1, 1));

    // �U���͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(position, attackRange, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));

    debugRenderer->DrawSphere(position, 10, DirectX::XMFLOAT4(0, 1, 0, 1));
    debugRenderer->DrawSphere(position, 3, DirectX::XMFLOAT4(0, 1, 0, 1));

    debugRenderer->DrawCylinder(targetPosition, radius, height, DirectX::XMFLOAT4(1, 1, 0, 1));
}

// ������(�Ռ��g)�̓����蔻��
void EnemySlime::CollisionImpactVsPlayer()
{
    PlayerManager& playerManager = PlayerManager::Instance();

    ProjectileManager& projectileManager = ProjectileManager::Instance();

    // �S�Ă̓G�Ƒ�������ŏՓˏ���
    int playerCount = PlayerManager::Instance().GetPlayerCount();//todo �O
    int projectileCount = projectileManager.GetProjectileCount();
    for (int j = 0; j < playerCount; ++j)
    {
        std::shared_ptr<Actor> player = playerManager.GetPlayer(j);//�O���[�v�̕����y��

    for (int i = 0; i < projectileCount; ++i)
    {
        std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);

            if (projectile->GetComponent<ProjectileImpact>())
            {

                DirectX::XMFLOAT3 projectilePosition = projectile->GetComponent<Transform>()->GetPosition();
                
                // �g��
                float height = 1.0f;
                projectile->GetComponent<Transform>()->SetHeight(height);
                float projectileHeight = projectile->GetComponent<Transform>()->GetHeight();

                float projectileRadiusOutLine = projectile->GetComponent<ProjectileImpact>()->GetRadiusOutSide();
                float projectileRadiusInLine = projectile->GetComponent<ProjectileImpact>()->GetRadiusInSide();

                DirectX::XMFLOAT3 playerPosition = player->GetComponent<Transform>()->GetPosition();
                float playerRadius = player->GetComponent<Transform>()->GetRadius();
                float playerHeight = player->GetComponent<Transform>()->GetHeight();

                // �Փˏ���
                DirectX::XMFLOAT3 outPositon;
                // �~���Ɖ~
                if (Collision::IntersectSphereVsCylinder(
                    projectilePosition,
                    projectileRadiusOutLine,
                    playerPosition,
                    playerRadius,
                    playerHeight,
                    outPositon) &&
                    !Collision::IntersectSphereVsCylinder(
                        projectilePosition,
                        projectileRadiusInLine,
                        playerPosition,
                        playerRadius,
                        playerHeight,
                        outPositon))

                {
                    // ���������ȉ��Ȃ�ʂ�
                    if (projectilePosition.y + projectileHeight < playerPosition.y) return;
                    // �_���[�W��^����B
                    if (player->GetComponent<HP>()->ApplyDamage(3, 0.5f))
                    {
                        player->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Damage));

                        // ������΂�
                        {
                            // �Փ�
                            DirectX::XMFLOAT3 impulse;
                            // �Ռ�
                            const float power = 10.0f;

                            float vx = playerPosition.x - projectilePosition.x;
                            float vz = playerPosition.z - projectilePosition.z;
                            float lengthXZ = sqrtf(vx * vx + vz * vz);
                            vx /= lengthXZ;
                            vz /= lengthXZ;

                            impulse.x = vx * power;
                            impulse.y = power * 0.5f;
                            impulse.z = vz * power;

                            player->GetComponent<Movement>()->AddImpulse(impulse);
                        }
                        // �q�b�g�G�t�F�N�g�Đ�
                        {
                            playerPosition.y += playerHeight * 0.5f;


                            //hitEffect->Play(e);
                        }

                        // �e�۔j��
                        //projectile->GetComponent<ProjectileImpact>()->Destoroy();
                        
                    }

                }
            }
        }
    }
}

void EnemySlime::CollisionRubyVsPlayer()
{
    PlayerManager& playerManager = PlayerManager::Instance();

    ProjectileManager& projectileManager = ProjectileManager::Instance();

    // �S�Ă̓G�Ƒ�������ŏՓˏ���
    int projectileCount = projectileManager.GetProjectileCount();

    for (int i = 0; i < projectileCount; ++i)
    {
        int playerCount = PlayerManager::Instance().GetPlayerCount();
        std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        for (int j = 0; j < playerCount; ++j)
        {
            if (projectile->GetComponent<ProjectileRuby>())
            {
                std::shared_ptr<Actor> player = playerManager.GetPlayer(j);

                DirectX::XMFLOAT3 projectilePosition = projectile->GetComponent<Transform>()->GetPosition();
                float projectileRadius = projectile->GetComponent<Transform>()->GetRadius();
                float projectileHeight = projectile->GetComponent<Transform>()->GetHeight();

                DirectX::XMFLOAT3 playerPosition = player->GetComponent<Transform>()->GetPosition();
                float playerRadius = player->GetComponent<Transform>()->GetRadius();
                float playerHeight = player->GetComponent<Transform>()->GetHeight();

                // �Փˏ���
                DirectX::XMFLOAT3 outPositon;
                // �~���Ɖ~��
                if (Collision::IntersectCylinderVsCylinder(
                    projectilePosition,
                    projectileRadius,
                    projectileHeight,
                    playerPosition,
                    playerRadius,
                    playerHeight,
                    outPositon))

                {
                    // �_���[�W��^����B
                    if (player->GetComponent<HP>()->ApplyDamage(3, 0.5f))
                    {
                        player->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Damage));
                        
                        DirectX::XMVECTOR ProjectileP = DirectX::XMLoadFloat3(&projectilePosition);
                        DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&playerPosition);
                        DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P , ProjectileP);
                        DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
                        DirectX::XMFLOAT3 normal;
                        DirectX::XMStoreFloat3(&normal, N);

                        if (normal.y > 0.8f)
                        {
                            float jumpSpeed = 20.0f;
                            // �Ռ�
                            const float power = 10.0f;
                            DirectX::XMFLOAT3 impulse;
                            impulse.y = power * 0.5f;
                            player->GetComponent<Movement>()->JumpVelocity(jumpSpeed);

                        }
                        //// ������΂�
                        else
                        {
                            // �Փ�
                            DirectX::XMFLOAT3 impulse;
                            // �Ռ�
                            const float power = 10.0f;

                            float vx = playerPosition.x - projectilePosition.x;
                            float vz = playerPosition.z - projectilePosition.z;
                            float lengthXZ = sqrtf(vx * vx + vz * vz);
                            vx /= lengthXZ;
                            vz /= lengthXZ;

                            impulse.x = vx * power;
                            impulse.y = power * 0.5f;
                            impulse.z = vz * power;

                            player->GetComponent<Movement>()->AddImpulse(impulse);
                            // �q�b�g�G�t�F�N�g�Đ�

                            playerPosition.y += playerHeight * 0.5f;

                            //hitEffect->Play(e);

                        }
                      



                        // �e�۔j��
                        //projectile->GetComponent<ProjectileImpact>()->Destoroy();

                    }
                    else
                    {
                        DirectX::XMVECTOR ProjectileP = DirectX::XMLoadFloat3(&projectilePosition);
                        DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&playerPosition);
                        DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, ProjectileP);
                        DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
                        DirectX::XMFLOAT3 normal;
                        DirectX::XMStoreFloat3(&normal, N);

                        if (normal.y > 0.8f)
                        {
                            float jumpSpeed = 20.0f;
                            // �Ռ�
                            const float power = 10.0f;
                            DirectX::XMFLOAT3 impulse;
                            impulse.y = power * 0.5f;
                            player->GetComponent<Movement>()->JumpVelocity(jumpSpeed);

                        }
                        //// ������΂�
                        else
                        {
                            // �Փ�
                            DirectX::XMFLOAT3 impulse;
                            // �Ռ�
                            const float power = 10.0f;

                            float vx = playerPosition.x - projectilePosition.x;
                            float vz = playerPosition.z - projectilePosition.z;
                            float lengthXZ = sqrtf(vx * vx + vz * vz);
                            vx /= lengthXZ;
                            vz /= lengthXZ;

                            impulse.x = vx * power;
                            impulse.y = power * 0.5f;
                            impulse.z = vz * power;

                            player->GetComponent<Movement>()->AddImpulse(impulse);
                            // �q�b�g�G�t�F�N�g�Đ�

                            playerPosition.y += playerHeight * 0.5f;

                            //hitEffect->Play(e);

                        }
                    }

                }
            }
        }
    }
}
// ���r�[�ƃv���C���[�̓����蔻��
void EnemySlime::CollisionRubyWidthVsPlayer()
{
    PlayerManager& playerManager = PlayerManager::Instance();

    ProjectileManager& projectileManager = ProjectileManager::Instance();

    // �S�Ă̓G�Ƒ�������ŏՓˏ���
    int projectileCount = projectileManager.GetProjectileCount();

    for (int i = 0; i < projectileCount; ++i)
    {
        int playerCount = PlayerManager::Instance().GetPlayerCount();
        std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        for (int j = 0; j < playerCount; ++j)
        {
            std::shared_ptr<Actor> player = playerManager.GetPlayer(j);
            if (!projectile->GetComponent<ProjectileThrowing>())return;


            DirectX::XMFLOAT3 projectilePosition = projectile->GetComponent<Transform>()->GetPosition();
            float projectileRadius = projectile->GetComponent<Transform>()->GetRadius();
            float projectileHeight = projectile->GetComponent<Transform>()->GetHeight();

            DirectX::XMFLOAT3 playerPosition = player->GetComponent<Transform>()->GetPosition();
            float playerRadius = player->GetComponent<Transform>()->GetRadius();
            float playerHeight = player->GetComponent<Transform>()->GetHeight();

            bool counterCheck = projectile->GetComponent<ProjectileThrowing>()->GetCounterCheck();

            // �Փˏ���
            DirectX::XMFLOAT3 outPositon;
            // ���Ƌ�
            if (Collision::IntersectSpherVsSphere(
                projectilePosition,
                projectileRadius,
                playerPosition,
                playerRadius,
                outPositon) && !counterCheck)

            {
                // �_���[�W��^����B
                if (player->GetComponent<HP>()->ApplyDamage(3, 0.5f))
                {

                    DirectX::XMVECTOR ProjectileP = DirectX::XMLoadFloat3(&projectilePosition);
                    DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&playerPosition);
                    DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, ProjectileP);
                    DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
                    DirectX::XMFLOAT3 normal;
                    DirectX::XMStoreFloat3(&normal, N);

                    if (normal.y > 0.8f)
                    {
                        float jumpSpeed = 5.0f;
                        // �Ռ�
                        const float power = 10.0f;
                        DirectX::XMFLOAT3 impulse;
                        impulse.y = power * 0.5f;
                        player->GetComponent<Movement>()->JumpVelocity(jumpSpeed);

                    }
                    //// ������΂�
                    else
                    {
                        // �Փ�
                        DirectX::XMFLOAT3 impulse;
                        // �Ռ�
                        const float power = 5.0f;

                        float vx = playerPosition.x - projectilePosition.x;
                        float vz = playerPosition.z - projectilePosition.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;

                        impulse.x = vx * power;
                        impulse.y = power * 0.5f;
                        impulse.z = vz * power;

                        player->GetComponent<Movement>()->AddImpulse(impulse);
                        // �q�b�g�G�t�F�N�g�Đ�

                        playerPosition.y += playerHeight * 0.5f;

                        //hitEffect->Play(e);

                    }



                    // �n�ʔ����x����
                    pushuThrow = false;
                    // �e�۔j��
                    projectile->GetComponent<BulletFiring>()->Destroy();

                }


            }
        }
    }
}

// ���r�B�ƒn��
void EnemySlime::CollisionRubyWidthVsOnGraound()
{
    ProjectileManager& projectileManager = ProjectileManager::Instance();

    // �S�Ă̓G�Ƒ�������ŏՓˏ���
    int projectileCount = projectileManager.GetProjectileCount();

    // ���f���f�[�^
    Model* stagemodel = StageManager::Instance().GetStage(StageManager::Instance().GetStageCount() - 1)->GetComponent<ModelControll>()->GetModel();

    // ���C�L���X�g�ɂ��ǔ���
    HitResult hit;

    for (int i = 0; i < projectileCount; ++i)
    {
        int playerCount = PlayerManager::Instance().GetPlayerCount();
        std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        for (int j = 0; j < playerCount; ++j)
        {
            if (!projectile->GetComponent<ProjectileThrowing>())return;


            DirectX::XMFLOAT3 projectilePosition = projectile->GetComponent<Transform>()->GetPosition();
            float projectileRadius = projectile->GetComponent<Transform>()->GetRadius();
            float projectileHeight = projectile->GetComponent<Transform>()->GetHeight();

            bool counterCheck = projectile->GetComponent<ProjectileThrowing>()->GetCounterCheck();



            //// �Փˏ���
            //DirectX::XMFLOAT3 outPositon;
            //if (Collision::IntersectRayVsModel(
            //    { projectilePosition.x, projectilePosition.y + projectileRadius ,projectilePosition.z },
            //    { projectilePosition.x, projectilePosition.y - projectileRadius ,projectilePosition.z },
            //    stagemodel,
            //    hit))
            //{
            //    projectile->GetComponent<BulletFiring>()->Destroy();
            //    // �����蔻������B
            //    pushuThrow = false;
            //}





        }
    }
}

bool EnemySlime::CollisionPlayerWithRushEnemy()
{


        int playerCount = PlayerManager::Instance().GetPlayerCount();
        for (int j = 0; j < playerCount; ++j)
        {
            std::shared_ptr<Actor> player = PlayerManager::Instance().GetPlayer(j);


            DirectX::XMFLOAT3 playerPosition = player->GetComponent<Transform>()->GetPosition();
            float playerRadius = player->GetComponent<Transform>()->GetRadius();
            float playerHeight = player->GetComponent<Transform>()->GetHeight();

            // �Փˏ���
            DirectX::XMFLOAT3 outPositon;
            // ���Ƌ�
            if (Collision::IntersectCylinderVsCylinder(
                position,
                attackRange,
                height,
                {
                playerPosition.x,
                playerPosition.y,
                playerPosition.z,
                },
                playerRadius ,
                playerHeight,
                outPositon))

            {
                // �_���[�W��^����B
                if (player->GetComponent<HP>()->ApplyDamage(3, 1.0f))
                {

                    DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&position);
                    DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&playerPosition);
                    DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
                    DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
                    DirectX::XMFLOAT3 normal;
                    DirectX::XMStoreFloat3(&normal, N);

                    float jumpSpeed = 5.0f;
                    // �Ռ�
                    const float power = 10.0f;
                    DirectX::XMFLOAT3 impulse;
                    impulse.y = power * 0.5f;
                    player->GetComponent<Movement>()->JumpVelocity(jumpSpeed);


                    //// ������΂�
                    {
                        // �Փ�
                        DirectX::XMFLOAT3 impulse;
                        // �Ռ�
                        const float power = 20.0f;

                        float vx = playerPosition.x - position.x;
                        float vz = playerPosition.z - position.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;

                        impulse.x = vx * power;
                        impulse.y = power * 0.5f;
                        impulse.z = vz * power;

                        player->GetComponent<Movement>()->AddImpulse(impulse);
                        // �q�b�g�G�t�F�N�g�Đ�

                        playerPosition.y += playerHeight * 0.5f;

                        player->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Damage);

                        //hitEffect->Play(e);

                    }

                }


            }
        }

    return false;
}

void EnemySlime::DetectHitByBodyPart(DirectX::XMFLOAT3 partBodyPosition)
{

    int playerCount = PlayerManager::Instance().GetPlayerCount();
    for (int j = 0; j < playerCount; ++j)
    {
        std::shared_ptr<Actor> player = PlayerManager::Instance().GetPlayer(j);


        DirectX::XMFLOAT3 playerPosition = player->GetComponent<Transform>()->GetPosition();
        float playerRadius = player->GetComponent<Transform>()->GetRadius();
        float playerHeight = player->GetComponent<Transform>()->GetHeight();

        // �Փˏ���
        DirectX::XMFLOAT3 outPositon;
        // ���Ƌ�
        if (Collision::IntersectSphereVsCylinder(
            partBodyPosition,
            radius,
            {
            playerPosition.x,
            playerPosition.y,
            playerPosition.z,
            },
            playerRadius,
            playerHeight,
            outPositon))

        {
            // �_���[�W��^����B
            if (player->GetComponent<HP>()->ApplyDamage(3, 1.0f))
            {

                DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&position);
                DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&playerPosition);
                DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
                DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
                DirectX::XMFLOAT3 normal;
                DirectX::XMStoreFloat3(&normal, N);

                float jumpSpeed = 5.0f;
                // �Ռ�
                const float power = 10.0f;
                DirectX::XMFLOAT3 impulse;
                impulse.y = power * 0.5f;
                player->GetComponent<Movement>()->JumpVelocity(jumpSpeed);


                //// ������΂�
                {
                    // �Փ�
                    DirectX::XMFLOAT3 impulse;
                    // �Ռ�
                    const float power = 20.0f;

                    float vx = playerPosition.x - position.x;
                    float vz = playerPosition.z - position.z;
                    float lengthXZ = sqrtf(vx * vx + vz * vz);
                    vx /= lengthXZ;
                    vz /= lengthXZ;

                    impulse.x = vx * power;
                    impulse.y = power * 0.5f;
                    impulse.z = vz * power;

                    player->GetComponent<Movement>()->AddImpulse(impulse);
                    // �q�b�g�G�t�F�N�g�Đ�

                    playerPosition.y += playerHeight * 0.5f;

                    player->GetComponent<Player>()->GetStateMachine()->ChangeState((int)Player::State::Damage);

                    //hitEffect->Play(e);

                }

            }


        }
    }
}

void EnemySlime::InputImpact(DirectX::XMFLOAT3 pos)
{
    {


        // �e�ۏ�����
        const char* filename = "Data/Model/Sword/Sword.mdl";

        std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
        actor->AddComponent<ModelControll>();
        actor->GetComponent<ModelControll>()->LoadModel(filename);
        actor->SetName("ProjectileImpact");
        actor->AddComponent<Transform>();
        actor->GetComponent<Transform>()->SetPosition(pos);
        actor->GetComponent<Transform>()->SetAngle(angle);
        actor->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f));



        //actor->AddComponent<BulletFiring>();
        actor->AddComponent<ProjectileImpact>();
        const char* effectFilename = "Data/Effect/inpact.efk";
        actor->GetComponent<ProjectileImpact>()->SetEffectProgress(effectFilename);
        // ��������
        float lifeTimer = 50.0f;
        actor->GetComponent<ProjectileImpact>()->SetLifeTimer(lifeTimer);

        
        //actor->GetComponent<ProjectileImpact>()->EffectProgressPlay();
        // ���ꂪ�QD���̊m�F
        bool check2d = false;
        actor->SetCheck2d(check2d);

        //actor->AddComponent<Collision>();
        ProjectileManager::Instance().Register(actor);
        //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        //std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

        // ����
        //projectile->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        //projectile->GetComponent<ProjectileImpact>()->SetTarget(target);

        // �U������
        //magicAction = false;
        //selectMagicCheck = (int)CommandMagic::Normal;

       
    }

}

void EnemySlime::InputProjectile()
{
    // �O���� sin�̌v�Z
    DirectX::XMFLOAT3 dir;


    dir.x = 0;// �O�p���΂߂ɂ��Ĉʒu��ς���
    dir.y = cosf(angle.x);
    dir.z = 0;

    DirectX::XMFLOAT3 pos;
    PlayerManager& playerManager = PlayerManager::Instance();
    int playerCount = playerManager.GetPlayerCount();
    for (int i = 0; i < playerCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
    {
        std::shared_ptr<Actor> player = PlayerManager::Instance().GetPlayer(i);
        // �ʒu

        pos = player->GetComponent<Transform>()->GetPosition();
        pos.y -= 15;

    }

    // �e�ۏ�����
    const char* filename = "Data/Model/Bullet/Bullet.mdl";
    //const char* filename = "Data/Model/Sword/Sword.mdl";

    std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
    actor->AddComponent<ModelControll>();
    actor->GetComponent<ModelControll>()->LoadModel(filename);
    actor->SetName("ProjectileRuby");
    actor->AddComponent<Transform>();
    actor->GetComponent<Transform>()->SetPosition(pos);
    actor->GetComponent<Transform>()->SetAngle(angle);
    actor->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

    float height = 5;
    actor->GetComponent<Transform>()->SetHeight(height);
    float lifetimer = 3;

    actor->AddComponent<BulletFiring>();
    actor->GetComponent<BulletFiring>()->Lanch(dir, pos, lifetimer);


    actor->AddComponent<ProjectileRuby>();
    const char* effectFilename = "Data/Effect/smorker.efk";
    actor->GetComponent<ProjectileRuby>()->SetEffectProgress(effectFilename);
    //actor->GetComponent<ProjectileHoming>()->EffectProgressPlay();
    // ���ꂪ�QD���̊m�F
    bool check2d = false;
    actor->SetCheck2d(check2d);
    // �ړ������̗L��
    bool movementCheck = true;
    actor->GetComponent<ProjectileRuby>()->SetMovementCheck(movementCheck);
    // ��������
    //float lifeTimer = 50.0f;
    //actor->GetComponent<ProjectileHoming>()->SetLifeTimer(lifeTimer);

    //actor->AddComponent<Collision>();
    ProjectileManager::Instance().Register(actor);
    //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
    //std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

    // ����
    //projectile->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
    //projectile->GetComponent<ProjectileImpact>()->SetTarget(target);

    // �U������
    //magicAction = false;
    //selectMagicCheck = (int)CommandMagic::Normal;



}

void EnemySlime::InputThrowingRuby(DirectX::XMFLOAT3 target)
{
    //// �O���� sin�̌v�Z
    DirectX::XMFLOAT3 direction;


    direction.x = sinf(angle.y);// �O�p���΂߂ɂ��Ĉʒu��ς���
    direction.y = cosf(angle.x);
    direction.z = cosf(angle.y);

    DirectX::XMFLOAT3 pos;
    //PlayerManager& playerManager = PlayerManager::Instance();
    //int playerCount = playerManager.GetPlayerCount();
    //for (int i = 0; i < playerCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
    //{
    //    std::shared_ptr<Actor> player = PlayerManager::Instance().GetPlayer(i);
    //    // �ʒu

    //    pos = player->GetComponent<Transform>()->GetPosition();
    //    pos.y -= 15;

    //}


    pos = position;
    pos.y = 0;

    // �e�ۏ�����
    const char* filename = "Data/Model/Bullet/Bullet.mdl";
    //const char* filename = "Data/Model/Sword/Sword.mdl";

    std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
    actor->AddComponent<ModelControll>();
    actor->GetComponent<ModelControll>()->LoadModel(filename);
    actor->SetName("ProjectileRubyThrowing");
    actor->AddComponent<Transform>();
    actor->GetComponent<Transform>()->SetPosition(pos);
    actor->GetComponent<Transform>()->SetAngle(angle);
    actor->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));


    float height = 5;
    actor->GetComponent<Transform>()->SetHeight(height);

    float radius = 6;
    actor->GetComponent<Transform>()->SetRadius(radius);

    float lifetimer = 10;

    actor->AddComponent<BulletFiring>();
    actor->GetComponent<BulletFiring>()->Lanch(direction, pos, lifetimer);

    


    actor->AddComponent<ProjectileThrowing>();
    const char* effectFilename = "Data/Effect/smorker.efk";
    actor->GetComponent<ProjectileThrowing>()->SetEffectProgress(effectFilename);

    actor->GetComponent<ProjectileThrowing>()->SetTarget(target);
    //actor->GetComponent<ProjectileHoming>()->EffectProgressPlay();
    // ���ꂪ�QD���̊m�F
    bool check2d = false;
    actor->SetCheck2d(check2d);
    // �ړ������̗L��
    bool movementCheck = false;
    actor->GetComponent<ProjectileThrowing>()->SetMovementCheck(movementCheck);


    // ��������
    //float lifeTimer = 50.0f;
    //actor->GetComponent<ProjectileHoming>()->SetLifeTimer(lifeTimer);

    //actor->AddComponent<Collision>();
    ProjectileManager::Instance().Register(actor);
    //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
    //std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

    // ����
    //projectile->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
    //projectile->GetComponent<ProjectileImpact>()->SetTarget(target);

    // �U������
    //magicAction = false;
    //selectMagicCheck = (int)CommandMagic::Normal;


}

// �꒣��ݒ�
void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;

}

void EnemySlime::UiControlle(float elapsedTime)
{
    if (UiManager::Instance().GetUiesCount() <= 0)return;
    float gaugeWidth = hp->GetMaxHealth() * hp->GetHealth() * 0.12f;
    std::shared_ptr<TransForm2D> uiHp = UiManager::Instance().GetUies((int)UiManager::UiCount::EnemyHPBar)->GetComponent<TransForm2D>();
    DirectX::XMFLOAT2 scale = { gaugeWidth, uiHp->GetScale().y };

    uiHp->SetScale(scale);

    std::shared_ptr<Ui> uiHpLife1 = UiManager::Instance().GetUies((int)UiManager::UiCount::EnemyHPLife01)->GetComponent<Ui>();
    std::shared_ptr<Ui> uiHpLife2 = UiManager::Instance().GetUies((int)UiManager::UiCount::EnemyHPLife02)->GetComponent<Ui>();

    bool checkDraw = false;
    switch (hp->GetLife())
    {
    case 1:
    {
        uiHpLife2->SetDrawCheck(checkDraw);
        break;
    }
    case 0:
    {
        uiHpLife1->SetDrawCheck(checkDraw);
        break;
    }
    default:
        break;
    }
    
    

}

void EnemySlime::OnHit(float elapsedTime)
{
    if (hp->FlashTime(elapsedTime))
    {
        ++damageStateTime;
        if (damageStateTime >= damageStateTimeMax)
        {
            damageStateCheck = damageStateCheck ? false : true;
            damageStateTime = 0;
        }

        if (damageStateCheck)
        {
            bool onHit = false;
            modelDrawCheck = onHit;

            colorGB = {1,1};
     
        }
        else
        {
            bool onHit = true;
            modelDrawCheck = onHit;

            colorGB = {0.2f,0.2f};
        }
    }
    else
    {
        damageStateTime = 0;

        bool onHit = true;
        modelDrawCheck = onHit;

        colorGB = { 1,1 };
    }
}

void EnemySlime::SetRandomTargetPosition()
{

    float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
    float range = Mathf::RandomRange(0.0f, territoryRange);
    targetPosition.x = territoryOrigin.x + sinf(theta)*range;
    targetPosition.y = territoryOrigin.y;
    targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

void EnemySlime::MoveToTarget(float elapsedTime, float speedRate)
{
    // �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;

    // �ړ�����
    movement->Move({ vx,0.0f ,vz }, moveSpeed * speedRate, elapsedTime);
    movement->Turn({ vx,0.0f ,vz } ,turnSpeed,elapsedTime);
   // Move( vx,vz  ,moveSpeed * speedRate);
    //Turn(elapsedTime, vx, vz ,turnSpeed * speedRate);
    //movement->Turn({ vx,0.0f ,vz } ,turnSpeed * speedRate,elapsedTime);
}
// �ړI�����ւ̉�]
void EnemySlime::TurnToTarget(float elapsedTime, float speedRate)
{

    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;
    // ��]
    movement->Turn({ vx,0.0f ,vz }, turnSpeed, elapsedTime);
}

void EnemySlime::InputJump()
{

    //jumpSpeed += jumpSpeedMin;

    // ������~
    if (position.y >= 1)
    {
        jumpSpeed = 0;
        movement->JumpVelocity(jumpSpeed);
    }


    if (movement->GetOnLadius())
    {
        movement->JumpVelocity(jumpSpeedMin);
    }

}

// �p�j�X�e�[�g�֑J��
void EnemySlime::TransitionWanderState()
{
    state = State::Wander;

    // �ڕW�n�_�ݒ�
    SetRandomTargetPosition();

    // �����A�j���[�V�����Đ�
    model->PlayAnimation(EnemySlime::Anim_Walk, true);
}

// �p�j�X�e�[�g�X�V����
void EnemySlime::UpdateWanderState(float elapsedTime)
{
    // �ڕW�n�_�܂�XZ���ʂł̋�������
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z- position.z;
    float distSq = vx * vx + vz * vz;
    if (distSq < radius * radius)
    {

        //// �ҋ@�X�e�[�g��
        TransitionIdleState();
    }

    // �ڕW�n�_�ֈړ�
    MoveToTarget(elapsedTime, 0.5f);

    // �v���C���[���G
    if (SearchPlayer())
    {
        // ����������
        TransitionPursuitState();
    }
}

void EnemySlime::CollisitionNodeVsPlayer(const char* nodeName, float nodeRadius)
{
    // �m�[�h�ʒu�Ɠ����蔻����s��
    Model::Node* node = model->FindNode(nodeName);


    if (node != nullptr && invalidJudgment)
    {
        // �m�[�h�̃��[���h���W
        DirectX::XMFLOAT3 nodePosition(
        node->worldTransform._41,
        node->worldTransform._42,
        node->worldTransform._43
            );

        // �����蔻��\��
        Graphics::Instance().GetDebugRenderer()->DrawSphere(
            nodePosition, nodeRadius, DirectX::XMFLOAT4(0, 0, 1, 1)
        );

        // �v���C���[�Ɠ����蔻��
        //Player& player = Player::Instance();

        int playercount = PlayerManager::Instance().GetPlayerCount();
        
        for (int i = 0; i < playercount; ++i)
        {
            // �v���C���[�擾
            std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(i);

            //�@�g�����X�t�H�[������
            DirectX::XMFLOAT3 playerPosition = playerid->GetComponent<Transform>()->GetPosition();
            float playerRadius = playerid->GetComponent<Transform>()->GetRadius();
            float playerHeight = playerid->GetComponent<Transform>()->GetRadius();


            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                nodePosition,
                nodeRadius,
                playerPosition,
                playerRadius,
                playerHeight,
                outPosition))
            {
                // �_���[�W��^����
                if (playerid->GetComponent<HP>()->ApplyDamage(1, 0.5f))
                {

                    // �Փ�
                    DirectX::XMFLOAT3 impulse;
                    const float power = 10.0f;
                    const DirectX::XMFLOAT3& p = playerPosition;
                    const DirectX::XMFLOAT3& e = nodePosition;
                    float vx = p.x - e.x;
                    float vz = p.z - e.z;
                    float lengthXZ = sqrtf(vx * vx + vz * vz);
                    vx /= lengthXZ;
                    vz /= lengthXZ;

                    impulse.x = vx * power;
                    impulse.y = power * 0.5f;
                    impulse.z = vz * power;



                    // ������΂�
                    playerid->GetComponent<Movement>()->AddImpulse(impulse);

                    // Ui �h�炵�J�n
                    playerid->GetComponent<Player>()->SetShakeMode(true);
                }
            }
        }
    }
}

bool EnemySlime::SearchPlayer()
{
    // �v���C���[�擾
    std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount()-1);
    
    //�@�g�����X�t�H�[������
    DirectX::XMFLOAT3 playerPosition = playerid->GetComponent<Transform>()->GetPosition();
    float playerRadius = playerid->GetComponent<Transform>()->GetRadius();
    float playerHeight = playerid->GetComponent<Transform>()->GetRadius();


    
    // �v���C���[�ƍ��፷���l�����ĂRD�ł̋������������
    //const DirectX::XMFLOAT3& playerPosition = playerid->GetPosition();
    //const DirectX::XMFLOAT3& playerPosition = DirectX::XMFLOAT3{0, 0, 0};
    float vx = playerPosition.x - position.x;
    float vy = playerPosition.y - position.y;
    float vz = playerPosition.z - position.z;
    // ���[�g
    float dist = sqrtf(vx * vx + vz * vz);
    if (dist < searchRange)
    {
        float distXZ = sqrtf(vx * vx + vz * vz);
        // �P�ʃx�N�g����
        vx /= distXZ;
        vz /= distXZ;
        // �O���x�N�g��
        float frontX = sinf(angle.y);
        float frontZ = cosf(angle.y);
        // �Q�̃x�N�g���̓��ϒl�őO�㔻��
        float dot = (frontX * vx) + (frontZ * vz);
        //0.070�G90�x
        //0.0f�G�����ȏ�
        if (dot > 0.0f)
        {
            return true;
        }
    }
    return false;
}

void EnemySlime::TransitionIdleState()
{
    state = State::Idle;

    // �^�C�}�[�������_����ݒ�@�ҋ@����
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    // �ҋ@�A�j���[�V�����Đ�
   // model->PlayAnimation(Anim_IdleNormal, true);
}

void EnemySlime::UpdateIdleState(float elapsedTime)
{

    // �^�C�}�[���� �ҋ@����
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        // �p�j�X�e�[�g�֑J��
        TransitionWanderState();
    }
}

void EnemySlime::TransitionPursuitState()
{
    state = State::Pursuit;

    // ���b�Ԓǔ�����^�C�}�[�������_���ݒ�
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    // �����A�j���[�V�����Đ�
    // model->PlayAnimation(Anim_RunFWD, true);
}

// �ǔ��X�e�[�g�X�V����
void EnemySlime::UpdatePursuitState(float elapsedTime)
{
    // �v���C���[id
    std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
    // �ڕW�n�_�����v���C���[�ʒu�ɐݒ�
    targetPosition = playerid->GetComponent<Transform>()->GetPosition();

    // �ڕW�n�_�ֈړ�
    MoveToTarget(elapsedTime, 1.0f);

    // �^�C�}�[����
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        // �ҋ@�X�e�[�g�֑J��
        TransitionIdleState();
    }

    // �v���C���[�̋߂Â��ƍU���X�e�[�g�֑J��
    float vx = targetPosition.x - position.x;
    float vy = targetPosition.y - position.y;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);
    if (dist < attackRange)
    {
        // �U���X�e�[�g�֑J��
        TransitionAttackState();
    }



}

void EnemySlime::TransitionAttackState()
{
    state = State::Attack;

    // �U���A�j���[�V�����Đ�
    //model->PlayAnimation(Anim_Attack1, false);
}

void EnemySlime::UpdateAttackState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
    // �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
    float animationTime = model->GetCurrentANimationSeconds();
    if (animationTime >= 0.2f && animationTime <= 0.35f)
    {
        // �ڋʃm�[�h�ƃv���C���[�̏Փˏ���
        CollisitionNodeVsPlayer("EyeBall", 0.2f);
    }
}

void EnemySlime::TransitionIdleBattleState()
{
    state = State::IdleBattle;

    // �U���A�j���[�V�����Đ�
    //model->PlayAnimation(Anim_IdleBattle, true);
}

void EnemySlime::UpdateIdleBattleState(float elapsedTime)
{
    // �v���C���[id
    std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
    // �ڕW�n�_�����v���C���[�ʒu�ɐݒ�
    targetPosition = playerid->GetComponent<Transform>()->GetPosition();

    // �^�C�}�[����
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        // �v���C���[���U���͈͂ɂ����ꍇ�͍U���X�e�[�g�֑J��
        float vx = targetPosition.x - position.x;
        float vy = targetPosition.y - position.y;
        float vz = targetPosition.z - position.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);
        if (dist < attackRange)
        {
            // �U���X�e�[�g�֑J��
            TransitionAttackState();
        }
        else
        {
            // �p�j�X�e�[�g�֑J��
            TransitionWanderState();
        }
    }
    MoveToTarget(elapsedTime, 0.0f);
}
// �_���[�W�X�e�[�g�֑J��
void EnemySlime::TransitionDamageState()
{
    state = State::Damage;

    // �_���[�W�A�j���[�V�����Đ�
    //model->PlayAnimation(Anim_GetHit, false);
}

void EnemySlime::UpdateDamageState(float elapsedTime)
{
    // �_���[�W�A�j���[�V�������I�������퓬�ҋ@�X�e�[�g�֑J��
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

// ���S�X�e�[�g�֑J��
void EnemySlime::TransitionDeathState()
{
    state = State::Death;

    // �_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Die, false);
}

void EnemySlime::UpdateDeathState(float elapsedTime)
{
    // �_���[�W�A�j���[�V�������I������玩����j��
    if (!model->IsPlayAnimation())
    {
        hp->OnDead();
        EnemyManager::Instance().Remove(GetActor());
        EnemyManager::Instance().DeleteUpdate(elapsedTime);
    }
}

void EnemySlime::Destroy()
{
    EnemyManager::Instance().Remove(GetActor());

}



void EnemyManager::DeleteUpdate(float elapsedTime)
{
    // �j������ ���t���[�������ň�C�ɏ����B
    for (std::shared_ptr<Actor> enemy : removes)// �E���܂��惊�X�g���E��
    {
        std::vector<std::shared_ptr<Actor>>::iterator it = std::find(enemies.begin(), enemies.end(),
            enemy);
        if (it != enemies.end())
        {
            enemies.erase(it);// �폜
        }

        // �e�ۂ̔j������
        //delete projectile;// ���ꕨ�j��

    }
    // �j�����X�g���N���A
    removes.clear();
}

void EnemyManager::Register(std::shared_ptr<Actor> actor)
{
    enemies.emplace_back(actor);
}

void EnemyManager::Clear()
{
    for (std::shared_ptr<Actor>& actor : enemies)// 
    {
        // ���̂��������Ǘ����Ă��鐔�͂��̂܂�
        actor.reset();
    }
    enemies.clear();

   
}

void EnemyManager::Remove(std::shared_ptr<Actor> actor)
{
    // �폜�o�^
    removes.insert(actor);
}
