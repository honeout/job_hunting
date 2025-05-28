#include <imgui.h>
#include "EnemyBoss.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "StateDerived.h"
#include "Audio/Audio.h"
#include "TransForm2D.h"
#include "ProjectileImpact.h"
#include "UiManager.h"
#include "Ui.h"

//

// �f�X�g���N�^
EnemyBoss::~EnemyBoss()
{
   
}

void EnemyBoss::Start()
{
    // ���S�`�F�b�N
    auto sharedId = GetActor();
    if (!sharedId)
        return;

    // �R���|�[�l���g���g����悤��
    std::shared_ptr movementId = sharedId->GetComponent<Movement>();
    std::shared_ptr hpId = sharedId->GetComponent<HP>();
    std::shared_ptr transformId = sharedId->GetComponent<Transform>();
    std::shared_ptr collisionId = sharedId->GetComponent<Collision>();

    // ���f���f�[�^������B
    model = GetActor()->GetComponent<ModelControll>()->GetModel();
    hpId->SetHealth(health);
    hpId->SetMaxHealth(maxHealth);
    collisionId->SetRadius(radius);
    collisionId->SetPartRadius(partRadius);
    collisionId->SetHeight(height);
    collisionId->SetSecondesHeight(confusionHeight);

    // �G�t�F�N�g
    moveAttackEffect = std::make_unique<Effect>("Data/Effect/enemyMoveAttackHit.efk");
    awakeEffect = std::make_unique<Effect>("Data/Effect/awake.efk");
    inpactEffect = std::make_unique<Effect>("Data/Effect/hit fire.efk");

    // �A�j���[�V�������[��
    updateanim = UpAnim::Normal;
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
    attackRightFootRange = 1.5f;

    // ����`�F�b�N
    moveCheck = true;

    // �Ռ��g�N����͈͊O��
    radiusInpactOutSide = 0.3f;

    // �Ռ��g�N����͈͓���
    radiusInpactInSide = 0.3f;

    // �Ռ��g����
    radiusInpactHeight = 0.3f;
}

// �X�V����
void EnemyBoss::Update(float elapsedTime)
{
    // ���S�`�F�b�N
    auto sharedId = GetActor();
    if (!sharedId)
        return;

    // �R���|�[�l���g���g����悤��
    std::shared_ptr transformId = sharedId->GetComponent<Transform>();
    std::shared_ptr movementId = sharedId->GetComponent<Movement>();
    std::shared_ptr hpId = sharedId->GetComponent<HP>();

    // ���삷�邩�ǂ���
    if (moveCheck)
    // �X�e�[�g���̏���
    stateMachine->Update(elapsedTime);

    // �G�o���Ǘ�
    ManageAwakeTime(elapsedTime);

    // �ʒu
    position = transformId->GetPosition();
    // ����
    angle = transformId->GetAngle();
    // �傫��
    scale = transformId->GetScale();

    // ���͏����X�V
    movementId->UpdateVelocity(elapsedTime);

    // ���G���ԍX�V
    hpId->UpdateInbincibleTimer(elapsedTime);

    // �����蔻��Ռ��g�ƃv���C���[
    CollisionImpactVsPlayer();

    // �폜
    ProjectileManager::Instance().DeleteUpdate(elapsedTime);//todo ����́H

    // ���[���h�ʒu
    transformId->UpdateTransform();

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

    // �p��
    model->UpdateTransform(transformId->GetTransform());
    
    // �Q�[�W�Ǘ�
    UiControlle(elapsedTime);
}

void EnemyBoss::Render(RenderContext& rc, ModelShader& shader)
{
        Graphics& graphics = Graphics::Instance();
        // �ԐF
        rc.color = colorGB;
        // �X�y�L�����[������
        rc.isSpecular = isSpecular;
        // �e�I���I�t
        rc.isRimRightning = isRimRightning;
        // �`��I���I�t
        rc.StencilRef = StencilRef;
        shader.Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��
        shader.Draw(rc, model);
        shader.End(rc);
}
#ifdef _DEBUG
void EnemyBoss::OnGUI()
{
    // ���S�`�F�b�N
    auto sharedId = GetActor();
    if (!sharedId)
        return;

    std::shared_ptr hpId = sharedId->GetComponent<HP>();

    if(ImGui::Button("drawtrue"))
    {
        modelDrawCheck = true;
    }
    if (ImGui::Button("drawfalse"))
    {
        modelDrawCheck = false;
    }
    if (ImGui::Button("isEnemyAwakened"))
    {
        ResetAwakeTime();
        bool check = isEnemyAwakened ? true : false;
        // �ϋv�͒ǉ�
        hpId->SetIsBonusHpActive(check);
    }
    if (ImGui::Button("StartAwake"))
    {
        stateMachine->ChangeState(static_cast<int>(State::AwakeStart));
    }
    if (ImGui::Button("stateStop"))
    {
        moveCheck = !moveCheck;
    }
}
#endif // _DEBUG

// �f�o�b�O�v���~�e�B�u�`��
void EnemyBoss::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // �꒣��͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f,
        DirectX::XMFLOAT4(0, 1, 0, 1));

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
// se�Đ�
void EnemyBoss::InputSe(AudioParam param)
{
    Audio& Se = Audio::Instance();
    Se.Play(param);
}
void EnemyBoss::PlaySe(const std::string& filename)
{
    Audio& Se = Audio::Instance();
    AudioParam audioParam;
    audioParam.filename = filename;
    audioParam.loop = isLoopDisabled;
    audioParam.volume = seVolume;
    Se.Play(audioParam);
}
// se��~
void EnemyBoss::StopSe(const std::string& filename)
{
    Audio& Se = Audio::Instance();
    // ��ޒ�~
    Se.Stop(filename);
}

// ��ύXCollision
// ������(�Ռ��g)�̓����蔻��
void EnemyBoss::CollisionImpactVsPlayer()
{
    // ���S�`�F�b�N
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    std::shared_ptr collisionId = sharedId->GetComponent<Collision>();
    PlayerManager& playerManager = PlayerManager::Instance();

    ProjectileManager& projectileManager = ProjectileManager::Instance();

    // �S�Ă̓G�Ƒ�������ŏՓˏ���
    int playerCount = playerManager.GetPlayerCount();//todo �O
    int projectileCount = projectileManager.GetProjectileCount();

    // player��������
    if (playerCount <= 0) return;

    // ���S�`�F�b�N
    std::shared_ptr<Actor> player = playerManager.GetPlayer((int)PlayerManager::PlayerType::Main);
    if (!player)
        return;
    // player�֌W
    std::shared_ptr<Player> playerMain = player->GetComponent<Player>();
    std::shared_ptr<Transform> playerTransform = player->GetComponent<Transform>();
    std::shared_ptr<Movement> playerMovement = player->GetComponent<Movement>();
    std::shared_ptr<Collision> playerCollision = player->GetComponent<Collision>();
    std::shared_ptr<HP> playerHp = player->GetComponent<HP>();
    // �ʒu�A���a�A����
    DirectX::XMFLOAT3 playerPosition = playerTransform->GetPosition();
    float playerRadius = playerCollision->GetRadius();
    float playerHeight = playerCollision->GetHeight();

    for (int i = 0; i < projectileCount; ++i)
    {
        std::weak_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        if (!projectile.lock()->GetComponent<ProjectileImpact>()) return;

        DirectX::XMFLOAT3 projectilePosition =
            projectile.lock()->GetComponent<Transform>()->GetPosition();

        // �g��
        float height = 1.0f;
        projectile.lock()->GetComponent<Collision>()->SetHeight(height);
        float projectileHeight = projectile.lock()->GetComponent<Collision>()->GetHeight();
        float projectileRadiusOutLine = projectile.lock()->GetComponent<ProjectileImpact>()->GetRadiusOutSide();
        float projectileRadiusInLine = projectile.lock()->GetComponent<ProjectileImpact>()->GetRadiusInSide();
        // �Փˏ���
        DirectX::XMFLOAT3 outPositon;

        // �C���p�N�g�̋^���I����
        // �~���Ɖ~
        if (collisionId->IntersectSphereVsCylinder(
            projectilePosition,
            projectileRadiusOutLine,
            playerPosition,
            playerRadius,
            playerHeight,
            outPositon) &&
            !collisionId->IntersectSphereVsCylinder(
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
            if (!playerHp->ApplyDamage(applyDamageImpact, impactInvincibleTime)) return;

            playerMain->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Damage));

            // �Ռ��g��
            PlaySe("Data/Audio/SE/�Ռ��g�q�b�g.wav");

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

                playerMovement->AddImpulse(impulse);
            }
            // �q�b�g�G�t�F�N�g�Đ�
            {
                playerPosition.y += playerHeight * 0.5f;

                bool loopSe = false;
            }
            // UI�h��
            playerMain->SetShakeMode(true);

            // �U��
            StartDamageShake();
        }
    }
}

// ��ύXCollision
// �Ռ��g
void EnemyBoss::CollisionInpact()
{
    // ���S�`�F�b�N
    auto sharedId = GetActor();
    if (!sharedId)
        return;

    std::shared_ptr collisionId = sharedId->GetComponent<Collision>();

    // �Ռ��g�̗L��
    if (!IsInpact) return;

    ProjectileManager& projectileManager = ProjectileManager::Instance();
    // �����̃{�[����
    Model::Node* bossLeftFoot = model->FindNode("boss_left_foot1");
    // �m�[�h�ʒu�擾
    // ����
    DirectX::XMFLOAT3 bossLeftFootPosition;
    bossLeftFootPosition = model->ConvertLocalToWorld(bossLeftFoot);

    DetectHitByBodyPart(bossLeftFootPosition, applyDamageStamp);

    // �����蔻�葝��
    radiusInpactInSide += 0.3f;

    // �����蔻�葝��
    radiusInpactOutSide += 0.3f;

    // �����蔻�葝�卂��
    radiusInpactHeight += 0.3f;

    PlayerManager& playerManager = PlayerManager::Instance();

    // �S�Ă̓G�Ƒ�������ŏՓˏ���
    int playerCount = playerManager.GetPlayerCount();

    // player��������
    if (playerCount <= 0) return;

    // ���S�`�F�b�N
    std::shared_ptr<Actor> player = playerManager.GetPlayer((int)PlayerManager::PlayerType::Main);
    if (!player)
        return;
    // player�֌W
    std::shared_ptr<Player> playerMain = player->GetComponent<Player>();
    std::shared_ptr<Transform> playerTransform = player->GetComponent<Transform>();
    std::shared_ptr<Movement> playerMovement = player->GetComponent<Movement>();
    std::shared_ptr<Collision> playerCollision = player->GetComponent<Collision>();
    std::shared_ptr<HP> playerHp = player->GetComponent<HP>();
    // �ʒu�A���a�A����
    DirectX::XMFLOAT3 playerPosition = playerTransform->GetPosition();
    float playerRadius = playerCollision->GetRadius();
    float playerHeight = playerCollision->GetHeight();

    // �Փˏ���
    DirectX::XMFLOAT3 outPositon;
    // �~���Ɖ~
    if (collisionId->IntersectSphereVsCylinder(
        bossLeftFootPosition,
        radiusInpactOutSide,
        playerPosition,
        playerRadius,
        playerHeight,
        outPositon) &&
        !collisionId->IntersectSphereVsCylinder(
            bossLeftFootPosition,
            radiusInpactInSide,
            playerPosition,
            playerRadius,
            playerHeight,
            outPositon))

    {
        // ���������ȉ��Ȃ�ʂ�
        if (bossLeftFootPosition.y + radiusInpactHeight < playerPosition.y) return;
        // �_���[�W��^����B
        if (!playerHp->ApplyDamage(applyDamageImpact, impactInvincibleTime)) return;

        playerMain->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Damage));

        // ������΂�
        {
            // �Փ�
            DirectX::XMFLOAT3 impulse;
            // �Ռ�
            const float power = 10.0f;
            float vx = playerPosition.x - bossLeftFootPosition.x;
            float vz = playerPosition.z - bossLeftFootPosition.z;
            float lengthXZ = sqrtf(vx * vx + vz * vz);
            vx /= lengthXZ;
            vz /= lengthXZ;
            impulse.x = vx * power;
            impulse.y = power * 0.5f;
            impulse.z = vz * power;
            playerMovement->AddImpulse(impulse);
        }
        // �q�b�g�G�t�F�N�g�Đ�
        {
            playerPosition.y += playerHeight * 0.5f;
        }
    }
}

// ��ύX���邩�ǂ���
// �G�o���Ǘ�
void EnemyBoss::ManageAwakeTime(float elapsedTime)
{
    // �o����
    if (enemyAwakeningDuration >= enemyAwakeningDurationEnd)
    {
        enemyAwakeningDuration -= elapsedTime;
        // �\�����
        isEnemyAwakened = true;
    }
    // �o���I��
    else
    {
        // �\�����
        isEnemyAwakened = false;
    }
    // �o���G�t�F�N�g�ʒu�X�V
    if (awakeEffect->GetEfeHandle())
    {
        awakeEffect->SetPosition(awakeEffect->GetEfeHandle(), position);
    }
}

// ��ύX���邩�ǂ���
// �G�o�����ԏ�����
void EnemyBoss::ResetAwakeTime()
{
    // ���ԏ�����
    enemyAwakeningDuration = enemyAwakeningDurationMax;
    // �o�����̃G�t�F�N�g
    awakeEffect->Play(position);
}

// ��ύXCollision
// �p�[�c���Ƃ̓����蔻��
void EnemyBoss::DetectHitByBodyPart(DirectX::XMFLOAT3 partBodyPosition, int applyDamage)
{
    // ���S�`�F�b�N
    auto sharedId = GetActor();
    if (!sharedId)
        return;

    std::shared_ptr collisionId = sharedId->GetComponent<Collision>();

    PlayerManager& playerManager = PlayerManager::Instance();

    // �S�Ă̓G�Ƒ�������ŏՓˏ���
    int playerCount = playerManager.GetPlayerCount();

    // ���S�`�F�b�N
    std::shared_ptr<Actor> player = playerManager.GetPlayer((int)PlayerManager::PlayerType::Main);
    if (!player)
        return;
    // player�֌W
    std::shared_ptr<Player> playerMain = player->GetComponent<Player>();
    std::shared_ptr<Transform> playerTransform = player->GetComponent<Transform>();
    std::shared_ptr<Movement> playerMovement = player->GetComponent<Movement>();
    std::shared_ptr<Collision> playerCollision = player->GetComponent<Collision>();
    std::shared_ptr<HP> playerHp = player->GetComponent<HP>();
    // �ʒu�A���a�A����
    DirectX::XMFLOAT3 playerPosition = playerTransform->GetPosition();
    float playerRadius = playerCollision->GetRadius();
    float playerHeight = playerCollision->GetHeight();
    // �Փˏ���
    DirectX::XMFLOAT3 outPositon;
    // ���Ƌ�
    if (collisionId->IntersectSphereVsCylinder(
        partBodyPosition,
        attackRightFootRange,
        playerPosition,
        playerRadius,
        playerHeight,
        outPositon))
    {
        // �_���[�W��^����B
        if (!playerHp->ApplyDamage(applyDamage, nuckleInvincibleTime)) return;

        // �a����
        PlaySe("Data/Audio/SE/�X���b�V���Q���.wav");

        // se�Đ�
        AudioParam audioParam;
        audioParam.filename = "Data/Audio/SE/�X���b�V���Q���.wav";
        audioParam.loop = isLoopDisabled;
        audioParam.volume = seVolume;
        InputSe(audioParam);


        DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&playerPosition);
        DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
        DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
        DirectX::XMFLOAT3 normal;
        DirectX::XMStoreFloat3(&normal, N);
        float jumpSpeed = 5.0f;
        //// ������΂�
        {
            // �Փ�
            DirectX::XMFLOAT3 impulse;
            // �Ռ�
            const float power = 10.0f;
            float vx = outPositon.x - playerPosition.x;
            float vz = outPositon.z - playerPosition.z;
            float lengthXZ = sqrtf(vx * vx + vz * vz);
            vx /= lengthXZ;
            vz /= lengthXZ;
            impulse.x = vx * power;
            impulse.y = power * 0.5f;
            impulse.z = vz * power;
            playerMain->GetStateMachine()->ChangeState((int)Player::State::Damage);
            playerMovement->AddImpulse(impulse);
            // �G�t�F�N�g�����ʒu
            DirectX::XMFLOAT3 efcPos = playerPosition;
            efcPos.y += playerCollision->GetHeight();
            // �q�b�g�G�t�F�N�g�Đ�
            moveAttackEffect->Play(playerPosition);
            //SE
            bool loopSe = false;
            // UI�h��
            playerMain->SetShakeMode(true);
            // �U��
            StartDamageShake();
        }
    }
}

void EnemyBoss::DetectHitByBodyAllPart(int applyDamage)
{
    // ���S�`�F�b�N
    auto sharedId = GetActor();
    if (!sharedId)
        return;

    std::shared_ptr collisionId = sharedId->GetComponent<Collision>();
    std::shared_ptr modelId = sharedId->GetComponent<ModelControll>();

    PlayerManager& playerManager = PlayerManager::Instance();

    // �S�Ă̓G�Ƒ�������ŏՓˏ���
    int playerCount = playerManager.GetPlayerCount();

    // ���S�`�F�b�N
    std::shared_ptr<Actor> player = playerManager.GetPlayer((int)PlayerManager::PlayerType::Main);
    if (!player)
        return;
    // player�֌W
    std::shared_ptr<Player> playerMain = player->GetComponent<Player>();
    std::shared_ptr<Transform> playerTransform = player->GetComponent<Transform>();
    std::shared_ptr<Movement> playerMovement = player->GetComponent<Movement>();
    std::shared_ptr<Collision> playerCollision = player->GetComponent<Collision>();
    std::shared_ptr<HP> playerHp = player->GetComponent<HP>();
    // �ʒu�A���a�A����
    DirectX::XMFLOAT3 playerPosition = playerTransform->GetPosition();
    float playerRadius = playerCollision->GetRadius();
    float playerHeight = playerCollision->GetHeight();

    // �p�[�c�̎��
    Model::Node* nodePart;

    // �p�[�c�S�m�F
    for (auto& part : hitSizes)
    {
        // �p�[�c�̎��
        nodePart = modelId->GetModel()->FindNode(bornPart.at(part.first));

        // �ʒu
        DirectX::XMFLOAT3 nodePosition;
        nodePosition = modelId->GetModel()->ConvertLocalToWorld(nodePart);

        // �Փˏ���
        DirectX::XMFLOAT3 outPositon;
        // ���Ƌ�
        if (!collisionId->IntersectSphereVsCylinder(
            nodePosition,
            hitSizes.at(part.first),
            playerPosition,
            playerRadius,
            playerHeight,
            outPositon)) return;

        // �_���[�W��^����B
        if (!playerHp->ApplyDamage(applyDamage, nuckleInvincibleTime)) return;

        // �a����
        PlaySe("Data/Audio/SE/�X���b�V���Q���.wav");
        DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&playerPosition);
        DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
        DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
        DirectX::XMFLOAT3 normal;
        DirectX::XMStoreFloat3(&normal, N);
        float jumpSpeed = 5.0f;
        //// ������΂�
        {
            // �Փ�
            DirectX::XMFLOAT3 impulse;
            // �Ռ�
            const float power = 10.0f;
            float vx = outPositon.x - playerPosition.x;
            float vz = outPositon.z - playerPosition.z;
            float lengthXZ = sqrtf(vx * vx + vz * vz);
            vx /= lengthXZ;
            vz /= lengthXZ;
            impulse.x = vx * power;
            impulse.y = power * 0.5f;
            impulse.z = vz * power;
            playerMain->GetStateMachine()->ChangeState((int)Player::State::Damage);
            playerMovement->AddImpulse(impulse);
            // �G�t�F�N�g�����ʒu
            DirectX::XMFLOAT3 efcPos = playerPosition;
            efcPos.y += playerCollision->GetHeight();
            // �q�b�g�G�t�F�N�g�Đ�
            moveAttackEffect->Play(playerPosition);
            //SE
            bool loopSe = false;
            // UI�h��
            playerMain->SetShakeMode(true);
            // �U��
            StartDamageShake();
        }
    }
}

// ��ύX�Ռ��g��΂�
// �Ռ��g����
void EnemyBoss::InputImpact(DirectX::XMFLOAT3 pos)
{
    // �e�ۏ�����
    const char* filename = "Data/Model/SpikeBall/SpikeBall.mdl";
    std::weak_ptr<Actor> actor = ActorManager::Instance().Create();
    actor.lock()->AddComponent<ModelControll>();
    actor.lock()->GetComponent<ModelControll>()->LoadModel(filename);
    actor.lock()->SetName("ProjectileImpact");
    actor.lock()->AddComponent<Transform>();
    actor.lock()->GetComponent<Transform>()->SetPosition(pos);
    actor.lock()->GetComponent<Transform>()->SetAngle(angle);
    actor.lock()->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f));
    actor.lock()->AddComponent<Collision>();
    actor.lock()->AddComponent<ProjectileImpact>();
    const char* effectFilename = "Data/Effect/inpact.efk";
    actor.lock()->GetComponent<ProjectileImpact>()->SetEffectProgress(effectFilename);
    // ��������
    float lifeTimer = 50.0f;
    actor.lock()->GetComponent<ProjectileImpact>()->SetLifeTimer(lifeTimer);
    // ���ꂪ�QD���̊m�F
    bool check2d = false;
    actor.lock()->SetCheck2d(check2d);
    ProjectileManager::Instance().Register(actor.lock());
}

// ��ύX �꒣��ݒ�
// �꒣��ݒ�
void EnemyBoss::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;
}

// ��ύX UIControlle
// �GHP��UI
void EnemyBoss::UiControlle(float elapsedTime)
{
    // ���S�`�F�b�N
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // �R���|�[�l���g���g����悤��
    std::shared_ptr hpId = sharedId->GetComponent<HP>();

    if (UiManager::Instance().GetUiesCount() <= uiCountMax)return;
    float gaugeWidth = hpId->GetMaxHealth() * hpId->GetHealth() * 0.12f;
    std::weak_ptr<TransForm2D> uiHp = UiManager::Instance().GetUies((int)UiManager::UiCount::EnemyHPBar)->GetComponent<TransForm2D>();
    DirectX::XMFLOAT2 scale = { gaugeWidth, uiHp.lock()->GetScale().y };
    uiHp.lock()->SetScale(scale);
    std::weak_ptr<Ui> uiHpLife1 = UiManager::Instance().GetUies((int)UiManager::UiCount::EnemyHPLife01)->GetComponent<Ui>();
    std::weak_ptr<Ui> uiHpLife2 = UiManager::Instance().GetUies((int)UiManager::UiCount::EnemyHPLife02)->GetComponent<Ui>();
    bool checkDraw = false;
    switch (hpId->GetLife())
    {
    case 1:
    {
        uiHpLife2.lock()->SetDrawCheck(checkDraw);
        break;
    }
    case 0:
    {
        uiHpLife1.lock()->SetDrawCheck(checkDraw);
        break;
    }
    default:
        break;
    }
}

// ��ύX �G�l�~�[�_��
// �G�l�~�[�_��
void EnemyBoss::OnHit(float elapsedTime)
{
    // ���S�`�F�b�N
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // �R���|�[�l���g���g����悤��
    std::shared_ptr hpId = sharedId->GetComponent<HP>();

    // �_�Ŏ���
    if (hpId->FlashTime(elapsedTime))
    {
        ++damageStateTime;
        if (damageStateTime >= damageStateTimeMax)
        {
            damageStateCheck = damageStateCheck ? false : true;
            damageStateTime = 0;
        }
        // �ʏ�F
        if (damageStateCheck)
        {
            bool onHit = false;
            modelDrawCheck = onHit;
            colorGB.x += 0.1f;
            colorGB.y += 0.1f;
        }
        // ��
        else
        {
            bool onHit = true;
            modelDrawCheck = onHit;
            colorGB.x -= 0.1f;
            colorGB.y -= 0.1f;
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

// �ړ��ʒu
void EnemyBoss::SetRandomTargetPosition()
{
    float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
    float range = Mathf::RandomRange(territoryRangeMin, territoryRange);
    targetPosition.x = territoryOrigin.x + sinf(theta)*range;
    targetPosition.y = territoryOrigin.y;
    targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

// ��ύX�@�^�[�Q�b�g�ʒu�܂ňړ�
// �^�[�Q�b�g�ʒu�܂ł̈ړ�
void EnemyBoss::MoveToTarget(float elapsedTime, float speedRate)
{
    // ���S�`�F�b�N
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // �R���|�[�l���g���g����悤��
    std::shared_ptr movementId = sharedId->GetComponent<Movement>();

    // �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
    float vx = targetPosition.x - position.x;
    float vy = 0.0f;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;
    // �ړ�����
    movementId->Move({ vx,vy ,vz }, moveSpeed * speedRate, elapsedTime);
    movementId->Turn({ vx,vy ,vz } ,turnSpeed,elapsedTime);
}

// ��ύX�@��]
// �ړI�����ւ̉�]
void EnemyBoss::TurnToTarget(float elapsedTime, float speedRate)
{
    // ���S�`�F�b�N
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // �R���|�[�l���g���g����悤��
    std::shared_ptr movementId = sharedId->GetComponent<Movement>();

    float vx = targetPosition.x - position.x;
    float vy = 0.0f;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;
    // ��]
    movementId->Turn({ vx,vy ,vz }, turnSpeed, elapsedTime);
}

// �W�����v
void EnemyBoss::InputJump()
{
    // ���S�`�F�b�N
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // �R���|�[�l���g���g����悤��
    std::shared_ptr movementId = sharedId->GetComponent<Movement>();

    // ������~
    if (position.y >= jumpLimit)
    {
        jumpSpeed = 0;
        movementId->JumpVelocity(jumpSpeed);
    }

    if (movementId->GetOnLadius())
    {
        movementId->JumpVelocity(jumpSpeedMin);
    }
}

// �v���C���[�̈ʒu��T���B
bool EnemyBoss::SearchPlayer()
{
    // �v���C���[�擾
    std::weak_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount()-1);
    //�@�g�����X�t�H�[������
    DirectX::XMFLOAT3 playerPosition = playerid.lock()->GetComponent<Transform>()->GetPosition();
    float playerRadius = playerid.lock()->GetComponent<Collision>()->GetRadius();
    float playerHeight = playerid.lock()->GetComponent<Collision>()->GetRadius();
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

// �G�l�~�[�폜
void EnemyBoss::Destroy()
{
    EnemyManager::Instance().Remove(GetActor());
}

// ��ʂ�h�炷
void EnemyBoss::StartDamageShake()
{
    // �V�F�C�N���� �p���[
    MessageData::CAMERASHAKEDATA p;
    p.shakePower = shakePower;
    p.shakeTimer = shakeTimer;
    Messenger::Instance().SendData(MessageData::CAMERASHAKE, &p);
    PostprocessingRenderer postprocessingRenderer;
    damageDistortion.radius = 300.0f;
    damageDistortion.mask_radius = 200.0f;
    postprocessingRenderer.SetRadialBlurMaxData(damageDistortion);
}

// �폜�X�V
void EnemyManager::DeleteUpdate(float elapsedTime)
{
    // �j������ ���t���[�������ň�C�ɏ����B
    for (std::shared_ptr<Actor> enemy : removes)// ���X�g������
    {
        std::vector<std::shared_ptr<Actor>>::iterator it = std::find(enemies.begin(), enemies.end(),
            enemy);
        if (it != enemies.end())
        {
            enemies.erase(it);// �폜
        }
    }
    // �j�����X�g���N���A
    removes.clear();
}

// �G�l�~�[�쐬
void EnemyManager::Register(std::shared_ptr<Actor> actor)
{
    enemies.emplace_back(actor);
}

// �S�폜
void EnemyManager::Clear()
{
    for (std::shared_ptr<Actor>& actor : enemies)
    {
        // ���̂��������Ǘ����Ă��鐔�͂��̂܂�
        actor.reset();
    }
    enemies.clear();
}

// �폜�o�^
void EnemyManager::Remove(std::shared_ptr<Actor> actor)
{
    // �폜�o�^
    removes.insert(actor);
}
