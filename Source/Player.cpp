#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
//#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "Graphics/Model.h"
#include "EnemySlime.h"


#include "AfterimageManager.h"


static Player* instance = nullptr;

// �C���X�^���X�擾
Player& Player::Instance()
{
    return *instance;
}



// �R���X�g���N�^
Player::Player()
{
    // �C���X�^���X�|�C���^�ݒ�
    //instance = this;

    //model = new Model("Data/Model/Jammo/Jammo.mdl");
    //model = new Model("Data/Model/AimTest/AimTest.mdl");

    // ���f�������������̂ŃX�P�[�����O
    // �L�����N�^�[��1.1�{
    //scale.x = scale.y = scale.z = 0.01f;

    //// �q�b�g�G�t�F�N�g�Ǎ� 
    //hitEffect = new Effect("Data/Effect/sunder.efk");
    //desEffect = new Effect("Data/Effect/F.efk");

    //// �㉺�ʃA�j���[�V�����p
    //state = State::Idle;

    //// �㔼�g
    //bornUpStartPoint = "mixamorig:Spine";
    //// �����g
    //bornDownerEndPoint = "mixamorig:Spine";

    //// �ҋ@�X�e�[�g�֑J��
    //TransitionIdleState();

}

// �f�X�g���N�^
Player::~Player()
{
    //if (hitEffect == nullptr)
    //delete hitEffect;
    //if (desEffect == nullptr)
    //delete desEffect;
    //if (model == nullptr)
    //delete model;
    if (hitEffect != nullptr)
    {
        delete hitEffect;
        hitEffect = nullptr;
    }

    if (desEffect != nullptr)
    {
        delete desEffect;
        desEffect = nullptr;
    }

    if (cameraControlle != nullptr)
    {
        delete cameraControlle;
        cameraControlle = nullptr;
    }

    //if (model != nullptr)
    //{
    //    delete model;
    //    //model = nullptr;
    //}
}




//void Player::UpdateTransform()
//{
//
//
//
//    
//}

void Player::Start()
{
    // �C���X�^���X��
    instance = this;

    // ���[�u�����g�֐����g����悤��
    movement = GetActor()->GetComponent<Movement>();

    // hp�֐����g����悤��
    hp = GetActor()->GetComponent<HP>();

    // projectileStraight�֐����g����悤��
    //projectileStraight = GetActor()->GetComponent<>;
    
    // ���f���f�[�^������B
    model = GetActor()->GetComponent<ModelControll>()->GetModel();
    
    cameraControlle = new CameraController();

    // �q�b�g�G�t�F�N�g�Ǎ� 
    hitEffect = new Effect("Data/Effect/sunder.efk");
    desEffect = new Effect("Data/Effect/F.efk");


    // �㔼�g
    bornUpStartPoint = "mixamorig:Spine";
    // �����g
    bornDownerEndPoint = "mixamorig:Spine";

   

    hp->SetHealth(health);

    hp->SetMaxHealth(maxHealth);

    GetActor()->SetRadius(radius);
    GetActor()->SetHeight(height);
    // �ҋ@�X�e�[�g�֑J��
    TransitionIdleState();

}

// �X�V����
// elapsedTime(�o�ߎ���)
void Player::Update(float elapsedTime)
{
    // �X�e�[�g���̏���
    switch (state)
    {
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Move:
        UpdateMoveState(elapsedTime);
        break;

    case State::Jump:
        UpdateJumpState(elapsedTime);
        break;
    case State::Land:
        UpdateLandState(elapsedTime);
        break;

    case State::JumpFlip:
        UpdatejumpFlipState(elapsedTime);
        break;

    case State::Attack:
        UpdateAttackState(elapsedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;
    case State::Revive:
        UpdateReviveState(elapsedTime);
        break;
    }
    // ���͏����X�V

    position = GetActor()->GetPosition();

    angle = { 
        GetActor()->GetRotation().x,
        GetActor()->GetRotation().y,
        GetActor()->GetRotation().z
    };

    //velocity = movement->GetVelocity();

    //movement->SetVelocity(velocity);

    //velocity = movement->GetVelocity();

    hp->UpdateInbincibleTimer(elapsedTime);

    cameraControlle->Update(elapsedTime);
    cameraControlle->SetTarget(position);

    hitEffect->SetScale(hitEffect->GetEfeHandle(),{ 1,1,1 });

    movement->UpdateVelocity(elapsedTime);



    //UpdateTransform();
    //CameraControl(elapsedTime);
   //CharacterControl(elapsedTime);

    // �v���C���[�ƓG�Ƃ̏Փˏ���
    CollisionPlayerVsEnemies();

    CollisionProjectilesVsEnemies();

    UpdateProjectile(elapsedTime);

    // �e�ۍX�V����
    //projectileManager.Update(elapsedTime);

    // �I�u�W�F�N�g�s����X�V
    //UpdateTransform();

    // ���f���A�j���[�V�����X�V����
    //model->UpdateAnimation(elapsedTime, true);

    //switch (updateanim)
    //{
    //case UpAnim::Normal:
    //{
    //    // ���f���A�j���[�V�����X�V����
    //    //model->UpdateAnimation(elapsedTime, true);
    //    break;
    //}
    //case UpAnim::Doble:
    //{
    //    // ���f���A�j���[�V�����X�V����
    //    //model->UpdateUpeerBodyAnimation(elapsedTime, bornUpStartPoint,bornUpEndPoint, true);
    //    //model->UpdateLowerBodyAnimation(elapsedTime, bornDownerEndPoint, true);
    //    break;
    //}
    //}
    //model->Update_blend_animations(0.675f, frontVec.x,1.582f);
    //model->Update_blend_animations(elapsedTime, frontVec.x,36,60, true);
    //model->Update_blend_animations(elapsedTime, frontVec.y,40,80, true);
    // �ʒu�X�V
    GetActor()->UpdateTransform();
    // �A�j���[�V�����Đ�
    model->UpdateAnimation(elapsedTime, true);
    // �ʒu�X�V
    model->UpdateTransform(GetActor()->GetTransform());
    //GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());

    // ���f���s��X�V
    //model->UpdateTransform(transform);
}

void Player::Render(RenderContext rc)
{
    Graphics& graphics = Graphics::Instance();
    //Shader* shader = graphics.GetShader();
    ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
    shader->Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��


    shader->Draw(rc, model);
   
    shader->End(rc);
}



// �f�o�b�O�v���~�e�B�u�`��
void Player::DrawDebugPrimitive()
{
    // �������@�̓��ق����Ȃ玩���Œǉ�
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //// �Փ˔���p�̃f�o�b�O����`��
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    // �Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    // �e�ۃf�o�b�O�v���~�e�B�u�`��
    //projectileManager.DrawDebugPrimitive();

    if (attackCollisionFlag)
    {


        // �U���Փ˗p�̍���m�[�h�f�o�b�O����`��
        Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            leftHandBone->worldTransform._41,
            leftHandBone->worldTransform._42,
            leftHandBone->worldTransform._43),
            leftHandRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }
}

void Player::OnGUI()
{
    ImGui::InputFloat("Move Speed", &moveSpeed);
    ImGui::InputInt("Jump max", &jumpCount);
}


// �ړ����͏���
bool Player::InputMove(float elapsedTime)
{
    // �i�s�x�N�g���擾
    //DirectX::XMFLOAT3 moveVec = GetMoveVec();
    moveVec = GetMoveVec(elapsedTime);

    

    // �i�s�x�N�g�����[���x�N�g���łȂ��ꍇ�͓��͂��ꂽ
    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}


void Player::CameraControl(float elapsedTime)
{
  /*  float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();

    float lengthSq = ax * ax + ay * ay;
    if (lengthSq > 0.1f)
    {
        float speed = cameraRollSpeed * elapsedTime;

        cameraAngle.x += ay * speed;
        cameraAngle.y += ax * speed;

        if (cameraAngle.x < cameraMinPitch)
        {
            cameraAngle.x = cameraMinPitch;
        }
        if (cameraAngle.x > cameraMaxPitch)
        {
            cameraAngle.x = cameraMaxPitch;
        }
        if (cameraAngle.y < -DirectX::XM_PI)
        {
            cameraAngle.y += DirectX::XM_2PI;
        }
        if (cameraAngle.y > DirectX::XM_PI)
        {
            cameraAngle.y -= DirectX::XM_2PI;
        }
    }

    DirectX::XMVECTOR Offset = DirectX::XMVectorSet(0.0f, characterHeight, 0.0f, 0.0f);
    DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&GetActor()->GetPosition());
    DirectX::XMVECTOR Focus = DirectX::XMVectorAdd(Target, Offset);
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(cameraAngle.x, cameraAngle.y, cameraAngle.z);
    DirectX::XMVECTOR Range = DirectX::XMVectorSet(0.0f, 0.0f, -cameraRange, 0.0f);
    DirectX::XMVECTOR Vec = DirectX::XMVector3TransformCoord(Range, Transform);
    DirectX::XMVECTOR Eye = DirectX::XMVectorAdd(Focus, Vec);

    DirectX::XMFLOAT3 eye, focus;
    DirectX::XMStoreFloat3(&eye, Eye);
    DirectX::XMStoreFloat3(&focus, Focus);

    Camera& camera = Camera::Instance();
    camera.SetLookAt(eye, focus, DirectX::XMFLOAT3(0, 1, 0));*/
}

DirectX::XMFLOAT3 Player::GetMoveVec(float elapsedTime) const
{
    // ���͏����擾
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    // �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    // �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

    // �J�����E�����x�N�g���͂w�y���ʂɐ����ȃx�N�g���ɕϊ�
    float cameraRightX =  cameraRight.x;
    float cameraRightZ =  cameraRight.z;
    // y��������炸�Ɂ@���̒������擾
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);

    // ����Y�����������Ă邩�@�E�������΂߂ł��^�������i��łق���Y���O
    //�@�ɂ��鏭���������ς�邾����P�ʃx�N�g���ɂ���P�D�O��
    if (cameraRightLength > 0.0f)
    {
        // �P�ʃx�N�g����
        // �E�����̒P�ʃx�N�g�� 
        cameraRightX = cameraRightX/cameraRightLength ;
        cameraRightZ = cameraRightZ / cameraRightLength;
        
    }



    // �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        // �P�ʃx�N�g����
        cameraFrontX = cameraFrontX / cameraFrontLength;
        cameraFrontZ = cameraFrontZ / cameraFrontLength;
    
    }

    // �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
    // �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
    // �i�s�x�N�g�����v�Z����
    DirectX::XMFLOAT3 vec;// �ړ������i�ނׂ������i�s�x�N�g��
    vec.x = (cameraRightX* ax) + (cameraFrontX * ay);// �E����
    vec.z = (cameraRightZ *ax) + (cameraFrontZ * ay);// �܂�����
    // Y�������ɂ͈ړ����Ȃ�
    vec.y = 0.0f;

    if (vec.x != 0 || vec.y != 0 || vec.z != 0)
    {
        movement->Move(vec,moveSpeed, elapsedTime);
        movement->Turn( vec ,turnSpeed, elapsedTime);
    }

   

    return vec;

}

void Player::CharacterControl(float elapsedTime)
{
  /*  float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    float lengthSq = ax * ax + ay * ay;
    if (lengthSq > 0.1f)
    {
        Camera& camera = Camera::Instance();
        const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
        const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

        DirectX::XMFLOAT3 direction;
        direction.x = (cameraRight.x * ax) + (cameraFront.x * ay);
        direction.y = 0.0f;
        direction.z = (cameraRight.z * ax) + (cameraFront.z * ay);

        float length = sqrtf(direction.x * direction.x + direction.z * direction.z);
        direction.x /= length;
        direction.z /= length;

        movement->Turn(direction, elapsedTime);
        movement->Move(direction,5 ,elapsedTime);
    }*/
}



// �`�揈��
void Player::Render(const RenderContext& rc, ModelShader* shader)
{
    // �`��
    shader->Draw(rc, model);
    // �e�ە`�揈��
    //projectileManager.Render(rc, shader);
}





// �e�ۂƓG�̏Փˏ���
void Player::CollisionProjectilesVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
   // Actor* enemyManager = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1);
   // EnemyManager& enemyManager = EnemyManager::Instance();
   
    projectileManager = ProjectileManager::Instance();

    // �S�Ă̓G�Ƒ�������ŏՓˏ���
    int projectileCount = projectileManager.GetProjectileCount();
    
    for (int i = 0; i < projectileCount; ++i)
    {
    int enemyCount = EnemyManager::Instance().GetEnemyCount();
        Actor* projectile = projectileManager.GetProjectile(i);
        for (int j = 0; j < enemyCount; ++j)
        {
            Actor* enemy = enemyManager.GetEnemy(j);

            
            // �Փˏ���
            DirectX::XMFLOAT3 outPositon;
            // �~���Ɖ~
            if (Collision::IntersectSphereVsCylinder(
                projectile->GetPosition(),
                projectile->GetRadius(),
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPositon))

            {

                // �_���[�W��^����B
                if (enemy->GetComponent<HP>()->ApplyDamage(1, 0.5f))
                {
                    // ������΂�
                    {
                        // �Փ�
                        DirectX::XMFLOAT3 impulse;
                        const float power = 10.0f;
                        const DirectX::XMFLOAT3& e = enemy->GetPosition();
                        const DirectX::XMFLOAT3& p = projectile->GetPosition();
                        float vx = e.x - p.x;
                        float vz = e.z - p.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;

                        impulse.x = vx * power;
                        impulse.y = power * 0.5f;
                        impulse.z = vz * power;

                        enemy->GetComponent<Movement>()->AddImpulse(impulse);
                    }
                    // �q�b�g�G�t�F�N�g�Đ�
                    {
                        DirectX::XMFLOAT3 e = enemy->GetPosition();
                        e.y += enemy->GetHeight() * 0.5f;


                        hitEffect->Play(e);
                    }
                    // �e�۔j��
                    //projectile->;
                    //ActorManager::Instance().Remove();
                    //if (projectile->GetName() == "ProjectileStraight")
                    //ActorManager::Instance().Remove(projectile->GetComponent<ProjectileStraight>()->GetActor());

                    //if (projectile->GetName() == "ProjectileHoming")
                    //    ActorManager::Instance().Remove(projectile->GetComponent<ProjectileHoming>()->GetActor());


                    projectile->GetComponent<BulletFiring>()->Destroy();
                }
            }
        }
    }
}

// �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    // �S�Ă̓G�Ƒ�������ŏՓˏ���

    int enemyCount = enemyManager.GetEnemyCount();


        for (int i = 0; i < enemyCount; ++i)
        {
            Actor* enemy = enemyManager.GetEnemy(i);


            //// �Փˏ���
            DirectX::XMFLOAT3 outPositon;



            if (Collision::IntersectCylinderVsCylinder(
                position, radius, height,
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPositon))

            {


                    DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
                    DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
                    DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
                    DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
                    DirectX::XMFLOAT3 normal;
                    DirectX::XMStoreFloat3(&normal, N);

                    if (normal.y > 0.8f)
                    {
                        if (enemy->GetComponent<HP>()->ApplyDamage(1, 0.1f))
                        {
                            //���W�����v
                            //Jump(jumpSpeed * 0.5f);
                            movement->JumpVelocity(jumpSpeed*0.5f);
                        }

                    }
                    else
                    {
                        // �����o����̈ʒu�ݒ�@
                        enemy->SetPosition(outPositon);
                    }
                
            }
            

        }
    
    


}

// �m�[�h�ƓG�̏Փ˔���
void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{

    // �m�[�h�擾
    Model::Node* node = model->FindNode(nodeName);
    //worldTransform
    //localTransform
    // �m�[�h�ʒu�擾
    DirectX::XMFLOAT3 nodePosition;
    nodePosition = {
        node->worldTransform._41,
        node->worldTransform._42,
        node->worldTransform._43
    };
    // �}�l�[�W���[�擾
    EnemyManager& enemyManager = EnemyManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();
    // �w��̃m�[�h�ƑS�Ă̓G�𑍓�����ŏՓˏ���
    for (int i = 0; i < enemyCount; ++i)
    {
        Actor* enemy = enemyManager.GetEnemy(i);


        //// �Փˏ���
        DirectX::XMFLOAT3 outPositon;

        // �~���Ɖ~
        if (Collision::IntersectSphereVsCylinder(
            nodePosition,
            nodeRadius,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPositon))

        {

            // �_���[�W��^����B
            //enemy->ApplyDamage(1);
        // �_���[�W���ʂ����������BTRUE�ɂȂ邩��
            if (enemy->GetComponent<HP>()->ApplyDamage(1, 0.5f))
            {

                // ������΂�
                {
                    // �Փ�
                    DirectX::XMFLOAT3 impulse;
                    const float power = 10.0f;
                    const DirectX::XMFLOAT3& e = enemy->GetPosition();
                    const DirectX::XMFLOAT3& p = nodePosition;
                    float vx = e.x - p.x;
                    float vz = e.z - p.z;
                    float lengthXZ = sqrtf(vx * vx + vz * vz);
                    vx /= lengthXZ;
                    vz /= lengthXZ;

                    impulse.x = vx * power;
                    impulse.y = power * 0.5f;
                    impulse.z = vz * power;

                    enemy->GetComponent<Movement>()->AddImpulse(impulse);
                }
                // �q�b�g�G�t�F�N�g�Đ�
                {
                    DirectX::XMFLOAT3 e = enemy->GetPosition();
                    e.y += enemy->GetHeight() * 0.5f;


                    hitEffect->Play(e);


                    //desEffect->Play(e);

                }

            }
        }



    }
 
}



// �f�o�b�O�pGUI�`��
void Player::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
    // begin����end�܂ł̓��e���o����
    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        // �g�����X�t�H�[��
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // �ʒu�@// ���l��������
            ImGui::InputFloat3("Position", &position.x);

            //ImGui::SliderFloat3("Velocity", &velocity.x,5,0);

            ImGui::InputInt("jamplimit", &jumpCount);

            //ImGui::InputInt("hp", &health);

            ImGui::SliderFloat("frontVec", &frontVec.x,-0.37f,1.0);
            ImGui::SliderFloat("frontVecY", &frontVec.y,-0.37f,1.0);
            //ImGui::SliderFloat("frontVecX", &frontVec.y,0.0f,1.5);
        // ��]
        DirectX::XMFLOAT3 a;
        // XMConvertToDegrees���ʂ̐��������x�ɂ���
        a.x = DirectX::XMConvertToDegrees(angle.x);
        a.y = DirectX::XMConvertToDegrees(angle.y);
        a.z = DirectX::XMConvertToDegrees(angle.z);
        ImGui::InputFloat3("Angle", &a.x);
        angle.x = DirectX::XMConvertToRadians(a.x);
        angle.y = DirectX::XMConvertToRadians(a.y);
        angle.z = DirectX::XMConvertToRadians(a.z);
        // �X�P�[��
        //ImGui::InputFloat3("Scale", &scale.x);

       

    }
    }
    ImGui::End();
}

//// ���n�������ɌĂ΂��
//void Player::OnLanding()
//{
//    // ���n�������烊�Z�b�g
//    jumpCount = 0;
//
//
//
//    //// �������̑��͂����ȏ�Ȃ璅�n�X�e�[�g��  �\���ȑ��x�ŗ��Ƃ��d�͂̂T�{�Q�A�R�b��ɒ��n���[�V����������B
//    if (velocity.y < gravity * 5.0f)
//    {
//        if (state != State::Damage && state != State::Death)
//        {
//            // ���n�X�e�[�g�֑J��
//            TransitionLandState();
//        }
//    }
//}
//
//void Player::OnDead()
//{
//
//
//    // ���S�X�e�[�g�J��
//    TransitionDeathState();
//}
//
//void Player::OnDamaged()
//{
//    
//        // �_���[�W�X�e�[�g�֑J��
//    TransitionDamageState();
//}

bool Player::InputJump()
{
    // �{�^���œ��͂ŃW�����v�i�W�����v�񐔐������j
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        // �l������������
        if (jumpCount < jumpLimit) {
            // �W�����v�̍ő�l
            ++jumpCount;
            movement->JumpVelocity(jumpSpeed);

            // �W�����v���͂���
           return true;
        }
    }

    return false;
}

bool Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();




    // ���i�e�۔��ˁ@c�{�^������������
    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        // �O���� sin�̌v�Z �p�x�̌v�Z
        DirectX::XMFLOAT3 dir;
        dir = GetForwerd(angle);
        
        // ���ˈʒu�i�v���C���[�̍�������)
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height*0.5f;// �g�����ʒu�̂�
        pos.z = position.z;
        // ���ˁ@�X�g���[�g�e�ۂ�p��
        {
            // �e�ۏ�����
            const char* filename = "Data/Model/Sword/Sword.mdl";

            std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
            actor->AddComponent<ModelControll>();
            actor->GetComponent<ModelControll>()->LoadModel(filename);
            actor->SetName("ProjectileStraight");
            actor->SetPosition(position);
            actor->SetRotation(GetActor()->GetRotation());
            actor->SetScale(DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f));
            actor->AddComponent<BulletFiring>();
            actor->AddComponent<ProjectileStraight>();
            //actor->AddComponent<Collision>();
            ProjectileManager::Instance().Register(actor.get());
            //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
            Actor* projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

            // ����
            actor->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        }

        //projectile->Lanch(dir, pos);
        return true;
    }

    // �ǔ��e�۔���
    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        // �O���� sin�̌v�Z
        DirectX::XMFLOAT3 dir;
        //dir.x = sinf(angle.y);// �O�p���΂߂ɂ��Ĉʒu��ς���
        //dir.y = 0;
        //dir.z = cosf(angle.y);

        dir = GetForwerd(angle);

        //sinf0�x�O�@cosf0�͂P�x
        //�X�Osin1,cos0�Ԃ��Ă��鉡
        //�S�Tsin0.5,cos0.5�΂�
        // 360�x����肭�\���o����B2d�ł��s����B


        // ���ˈʒu�i�v���C���[�̍�������)
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;// �g�����ʒu�̂�
        pos.z = position.z;
        //�^�[�Q�b�g�i�f�t�H���g�ł̓v���C���[�̑O���j
        DirectX::XMFLOAT3 target;
        // �G�����Ȃ��������̂��߂Ɂ@1000��܂Ŕ��ł���
        target.x = pos.x+dir.x * 1000.0f;
        target.y = pos.y+dir.y * 1000.0f;
        target.z = pos.z+dir.z * 1000.0f;

        // ��ԋ߂��̓G���^�[�Q�b�g�ɂ���
        float dist = FLT_MAX;// float �̍ő�lfloat�S��
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
        {
            // �G�Ƃ̋�������  �G�̐����v�� �S�Ă̓G���Ăɓ����
            Actor* enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // �G�̈ʒu
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            // ��������G�܂ł̈ʒu���v��
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            // �x�N�g���̂Ȃ������Q�悷��B����Ă��Ȃ��z
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                // �������G�̂��̂����鏭�Ȃ�����R�O�Ȃ�R�O�A�P�O�O�Ȃ�P�O�O�����
                dist = d;
                target = enemy->GetPosition();// �ʒu������
                target.y += enemy->GetHeight() * 0.5f;// �ʒu�ɐg����
            }
            
            

        }


        // ���ˁ@�X�g���[�g�e�ۂ�p��
        //ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
        //projectile->Lanch(dir, pos,target);

        // �e�ۏ�����
        const char* filename = "Data/Model/Sword/Sword.mdl";

        std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
        actor->AddComponent<ModelControll>();
        actor->GetComponent<ModelControll>()->LoadModel(filename);
        actor->SetName("ProjectileHoming");
        actor->SetPosition(position);
        actor->SetRotation(GetActor()->GetRotation());
        actor->SetScale(DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f));
        actor->AddComponent<BulletFiring>();
        actor->AddComponent<ProjectileHoming>();
        //actor->AddComponent<Collision>();
        ProjectileManager::Instance().Register(actor.get());
        //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        Actor* projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

        // ����
        projectile->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        projectile->GetComponent<ProjectileHoming>()->SetTarget(target);


        return true;

    }
    return false;
}

bool Player::InputAttack()
{
    // �U�����͏���

    GamePad& gamePad = Input::Instance().GetGamePad();

    // ���i�e�۔��ˁ@x�{�^������������
    if (gamePad.GetButtonDown() & GamePad::BTN_B)
    {
  
        return true;
    }

    return false;
}

// �ҋ@�X�e�[�g�֑J��
void Player::TransitionIdleState()
{
    state = State::Idle;

    afterimagemove = false;

    updateanim = UpAnim::Normal;

    // �ҋ@�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Idle, true);

    
}

// �ҋ@�X�e�[�g�X�V����
void Player::UpdateIdleState(float elapsedTime)
{
    // �ړ����͏���
    if (InputMove(elapsedTime))
    {
       
        TransitionMoveState();
    }

    // �W�����v���͏���
    if (InputJump())
    {
        
        TransitionJumpState();
    }

    if (InputAttack())
    {
        //stated = state;
        TransitionAttackState();
    }

    // �e�ۓ��͏���
    if (InputProjectile())
    {
        TransitionAttackState();
    }
    //currentANimationSeconds = model->GetCurrentANimationSeconds();

    // �ړ����͏���
    //InputMove(elapsedTime);


}

void Player::TransitionMoveState()
{

    afterimagemove = true;

    updateanim = UpAnim::Doble;

    // �㔼�g
    bornUpStartPoint = "mixamorig:Spine";
    // �����g
    bornDownerEndPoint = "mixamorig:Spine";


    // ����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Running, true);

    model->PlayUpeerBodyAnimation(Anim_Running, true);
    
    state = State::Move;
}

void Player::UpdateMoveState(float elapsedTime)
{
    // �ʒu��������B
    position = GetActor()->GetPosition();


    // �ړ����͏���
    if (!InputMove(elapsedTime))
    {
        
        TransitionIdleState();
    }

 

    // �W�����v���͏���
    if (InputJump())
    {
        
        TransitionJumpState();
    }

    if (InputAttack())
    {
        //stated = state;
        TransitionAttackState();
    }

    

    // �e�ۓ��͏���
    if (InputProjectile())
    {
        TransitionAttackState();
    }
    // �������n
    Ground();

    // �c���p���p
    //currentANimationSeconds = model->GetCurrentANimationSeconds();



}




void Player::TransitionJumpState()
{

    state = State::Jump;

    // �㔼�g
    bornUpStartPoint = "mixamorig:Spine";

    model->PlayUpeerBodyAnimation(Anim_Jump, false);
    // �W�����v�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Jump,false);

}

void Player::UpdateJumpState(float elapsedTime)
{

    //InputMove(elapsedTime);


    // �W�����v���͏���
    if (InputJump()&&jumpCount >= 1)
    {
        TransitionJumpFlipState();
    }

    // �e�ۓ��͏���
    if (InputProjectile())
    {
        TransitionAttackState();
    }


    // �������n
    Ground();

    //if (movement->GetStepOffSet())
    //{
    //    TransitionLandState();
    //   
    //}

    


    //currentANimationSeconds = model->GetCurrentANimationSeconds();

}

void Player::TransitionLandState()
{
    state = State::Land;

    updateanim = UpAnim::Normal;
    // ���n�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Landing, false);

    movement->SetOnLadius(false);

    //movement->SetStepOffSet(false);
    
}

void Player::UpdateLandState(float elapsedTime)
{
   

    // �����I�������ҋ@�ɕύX
    if (!model->IsPlayAnimation())
    {
        movement->SetOnLadius(false);
        TransitionIdleState();
    }

    //currentANimationSeconds = model->GetCurrentANimationSeconds();
 
}





void Player::TransitionJumpFlipState()
{


    state = State::JumpFlip;

    // ����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Jump_Flip, false);
    model->PlayUpeerBodyAnimation(Anim_Jump_Flip, false);
}

void Player::UpdatejumpFlipState(float elapsedTime)
{


    if (InputMove(elapsedTime))
        bool afterimagemove = true;
    // �W�����v���͏���
    if (!model->IsPlayAnimation())
    {
        TransitionJumpState();
    }

    //currentANimationSeconds = model->GetCurrentANimationSeconds();
}

void Player::TransitionAttackState()
{
    //if (updateanim == UpAnim::Doble)
    //{
    //    state = State::Attack;
    //    //updateanim = UpAnim::Doble;
    //    //�㔼�g
    //    bornUpStartPoint = "mixamorig:Spine2";

    //   /* bornUpEndPoint = "mixamorig:Neck";*/
    //    // �����g
    //    bornDownerEndPoint = "mixamorig:Spine";

    //    model->PlayUpeerBodyAnimation(Anim_Attack, false);
    //}
    //else
    //{
    //    state = State::Attack;

    //    updateanim = UpAnim::Normal;
    //    // ����A�j���[�V�����Đ�
    //    model->PlayAnimation(Anim_Attack, false);
    //}

    state = State::Attack;

    updateanim = UpAnim::Normal;
    // ����A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Attack, false);
}

void Player::UpdateAttackState(float elapsedTime)
{

    // �����I�������ҋ@�ɕύX
    if (updateanim == UpAnim::Doble && !model->IsPlayUpeerBodyAnimation())
    {
        attackCollisionFlag = false;
        TransitionMoveState();

       
    }
    else if (updateanim == UpAnim::Normal && !model->IsPlayAnimation())
    {
        attackCollisionFlag = false;
        TransitionMoveState();
    }
    if (updateanim == UpAnim::Doble && !InputMove(elapsedTime))
    {
        updateanim = UpAnim::Doble;
        //�㔼�g
        bornUpStartPoint = "mixamorig:Spine";
        // �����g
        bornDownerEndPoint = "mixamorig:Spine";
        model->PlayAnimation(Anim_Attack, false);
    }

        // �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
        float animationTime = model->GetCurrentANimationSeconds();
        // ��肭�s���ΓG������s��������Ă����s����p�ӏo����B
        attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
        if (updateanim == UpAnim::Doble)
        {
            // �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
            float animationTime = model->GetCurrentAnimationSecondsUpeer();
            // ��肭�s���ΓG������s��������Ă����s����p�ӏo����B
            attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
        }
    if (attackCollisionFlag)
    {
        // ����m�[�h�ƃG�l�~�[�̏Փˏ���
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    }

    Ground();
    
}

void Player::UpdateProjectile(float elapsedTime)
{
    DirectX::XMFLOAT3 target;
    projectileManager = ProjectileManager::Instance();

    // �S�Ă̓G�Ƒ�������ŏՓˏ���
    int projectileCount = projectileManager.GetProjectileCount();

    for (int i = 0; i < projectileCount; ++i)
    {
  
        Actor* projectile = projectileManager.GetProjectile(i);
        float dist = FLT_MAX;// float �̍ő�lfloat�S��
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
        {
            // �G�Ƃ̋�������  �G�̐����v�� �S�Ă̓G���Ăɓ����
            Actor* enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // �G�̈ʒu
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
            // ��������G�܂ł̈ʒu���v��
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            // �x�N�g���̂Ȃ������Q�悷��B����Ă��Ȃ��z
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                // �������G�̂��̂����鏭�Ȃ�����R�O�Ȃ�R�O�A�P�O�O�Ȃ�P�O�O�����
                dist = d;
                target = enemy->GetPosition();// �ʒu������
                target.y += enemy->GetHeight() * 0.5f;// �ʒu�ɐg����
                //if (projectile->GetModel())
                //    projectile->GetComponent<ProjectileHoming>()->SetTarget(target);
            }


        }
    }

}

void Player::TransitionDamageState()
{
    state = State::Damage;

    // �_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_GetHit1, false);
}

void Player::UpdateDamageState(float elapsedTime)
{
    // �_���[�W�A�j���[�V�����I�������ҋ@�X�e�[�g��
    if (!model->IsPlayAnimation())
    {

            TransitionIdleState();

    }
}

void Player::TransitionDeathState()
{
    state = State::Death;

    // �_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Death, false);
}

void Player::UpdateDeathState(float elapsedTime)
{   
    
    if (!model->IsPlayAnimation())
    {

       // �{�^�����������畜���X�e�[�g�֑J��
        GamePad& gamePad = Input::Instance().GetGamePad();
        if (gamePad.GetButtonDown() & GamePad::BTN_A)
        {
            TransitionReviveState();
        }


    }

}
// �����X�e�[�g�J��
void Player::TransitionReviveState()
{
    state = State::Revive;

    // �̗͉�
    health = maxHealth;


    // �_���[�W�A�j���[�V�����Đ�
    model->PlayAnimation(Anim_Revive, false);
}

void Player::UpdateReviveState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();

    }
}

//void Player::UpdateTransform()
//{
//    // �ʒu���
//    position = GetActor()->GetPosition();
//
//    // �������
//    inFloat3({
//        GetActor()->GetRotation().x,
//        GetActor()->GetRotation().y,
//        GetActor()->GetRotation().z
//        }, angle);
//    
//}

void Player::inFloat3(DirectX::XMFLOAT3 value, DirectX::XMFLOAT3 &inValue)
{
   inValue =
    {
         value.x,
         value.y,
         value.z
    };

    
}

DirectX::XMFLOAT3 Player::GetForwerd(DirectX::XMFLOAT3 angle)
{
    DirectX::XMFLOAT3 dir;
    dir.x = sinf(angle.y);// �O�p���΂߂ɂ��Ĉʒu��ς���
    dir.y = 0;
    dir.z = cosf(angle.y);

    return dir;
}


void Player::Ground()
{
    if (movement->GetOnLadius())
    {
        jumpCount = 0;
        TransitionLandState();
    }
    
}

void PlayerManager::Register(Actor* actor)
{
    players.emplace_back(actor);
}
