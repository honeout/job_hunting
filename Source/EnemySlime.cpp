#include <imgui.h>
#include "EnemySlime.h"
//#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
//#include "Collision.h"
#include "StateDerived.h"


// �f�X�g���N�^
EnemySlime::~EnemySlime()
{
   // delete model;
    if (stateMachine != nullptr)
    {
        delete stateMachine;
        stateMachine = nullptr;
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
    model = GetActor()->GetComponent<ModelControll>()->GetModel();
    

    hp->SetHealth(health);

    hp->SetMaxHealth(maxHealth);

    transform->SetRadius(radius);

    transform->SetHeight(height);

    // �p�j�X�e�[�g�֑J��
    //TransitionWanderState();

    //SetTerritory(position, territoryarea);

    //stateMachine = std::make_unique<StateMachine>();
    stateMachine = new StateMachine();

    // �X�e�[�g�}�V���ɃX�e�[�g�o�^
    stateMachine->RegisterState(new WanderState(GetActor().get()));
    stateMachine->RegisterState(new IdleState(GetActor().get()));
    stateMachine->RegisterState(new PursuitState(GetActor().get()));
    stateMachine->RegisterState(new AttackState(GetActor().get()));

    // �X�e�[�g�Z�b�g
    stateMachine->SetState(static_cast<int>(State::Idle));

}

// �X�V����
void EnemySlime::Update(float elapsedTime)
{
    // �X�e�[�g���̏���
   /* switch (state)
    {
    case State::Wander:
        UpdateWanderState(elapsedTime);
        break;
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Pursuit:
        UpdatePursuitState(elapsedTime);
        break;
    case State::Attack:
        UpdateAttackState(elapsedTime);
        break;
    case State::IdleBattle:
        UpdateIdleBattleState(elapsedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;
    }*/

    stateMachine->Update(elapsedTime);

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

    DrawDebugPrimitive();

    transform->UpdateTransform();

    model->UpdateAnimation(elapsedTime,true);

    //model->UpdateAnimation(elapsedTime, true);

    model->UpdateTransform(transform->GetTransform());
    //GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());
    
   // UpdateVelocity(elapsedTime);

    //UpdateInbincibleTimer(elapsedTime);
    // �I�u�W�F�N�g�s����X�V
    //UpdateTransform();

    //// ���f���A�j���[�V�����X�V
    //model->UpdateAnimation(elapsedTime);

    //// ���f���s��X�V
    //model->UpdateTransform(transform);
}

// �`�揈��
//void EnemySlime::Render(const RenderContext& rc, ModelShader* shader)
//{
//    shader->Draw(rc, model);
//}

void EnemySlime::Render(RenderContext& rc)
{
    Graphics& graphics = Graphics::Instance();
    //Shader* shader = graphics.GetShader();
<<<<<<< HEAD
<<<<<<< HEAD
    ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);
=======
    ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
>>>>>>> parent of 8a0ff20 (とりあえずシェーダー綺麗な奴入った)
=======
    ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
    shader->Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��


    shader->Draw(rc, model);

    shader->End(rc);

}
// �V���h�E�}�b�v
void EnemySlime::RenderShadowmap(RenderContext& rc)
{
    Graphics& graphics = Graphics::Instance();
    ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);
>>>>>>> parent of 0db33c3 (一度保存ワーニング大量)
    shader->Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��


    shader->Draw(rc, model);

    shader->End(rc);
}

// �f�o�b�O�v���~�e�B�u�`��
void EnemySlime::DrawDebugPrimitive()
{
    // ���N���X�̃f�o�b�O�v���~�e�B�u�`��
    //Enemy::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // �꒣��͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f,
        DirectX::XMFLOAT4(0, 1, 0, 1));

    // �^�[�Q�b�g�ʒu���f�o�b�O���`��
    debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1, 1, 0, 1));
    
    // ���G�͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0, 0, 1, 1));

    // �U���͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(position, attackRange, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
}

// �꒣��ݒ�
void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;

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

// �p�j�X�e�[�g�֑J��
void EnemySlime::TransitionWanderState()
{
    state = State::Wander;

    // �ڕW�n�_�ݒ�
    SetRandomTargetPosition();

    // �����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_WalkFWD, true);
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
    if (node != nullptr)
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
    model->PlayAnimation(Anim_IdleNormal, true);
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
    model->PlayAnimation(Anim_RunFWD, true);
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
    model->PlayAnimation(Anim_Attack1, false);
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
    model->PlayAnimation(Anim_IdleBattle, true);
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
    model->PlayAnimation(Anim_GetHit, false);
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
    //Actor* enemyId = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1);
    EnemyManager::Instance().Remove(GetActor());
    //ActorManager::Instance().Remove();
}

//// ���S�������ɌĂ΂��
//void EnemySlime::OnDead()
//{
//    // ���S�X�e�[�g�֑J��
//    TransitionDeathState();
//}
//
//void EnemySlime::OnDamaged()
//{
//    // �_���[�W�X�e�[�g�֑J��
//    TransitionDamageState();
//}



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

void EnemyManager::Remove(std::shared_ptr<Actor> actor)
{
    // �폜�o�^
    removes.insert(actor);
}
