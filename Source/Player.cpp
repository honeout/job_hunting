#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
//#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "ProjectileRuby.h"
#include "ProjectileThrowing.h"
#include "Graphics/Model.h"
#include "EnemySlime.h"

#include "ProjectileTornade.h"

#include "StateDerived.h"

#include "TransForm2D.h"

#include "ProjectileImpact.h"
#include "ProjectileSunder.h"
#include "BulletFiring.h"

#include "UiManager.h"
#include "Ui.h"







// �f�X�g���N�^
Player::~Player()
{

    //if (movement)
    //    movement.reset();
    //if (hp)
    //    hp.reset();
    //if (transform)
    //    transform.reset();

    //if (stateMachine)
    //{
    //    delete stateMachine;
    //    stateMachine = nullptr;
    //}

    if (hitEffect != nullptr)
    {
        hitEffect->Stop(hitEffect->GetEfeHandle());
        delete hitEffect;
        hitEffect = nullptr;
    }

    if (ImpactEffect != nullptr)
    {
        ImpactEffect->Stop(ImpactEffect->GetEfeHandle());
        delete ImpactEffect;
        ImpactEffect = nullptr;
    }
    if (desEffect != nullptr)
    {
        desEffect->Stop(desEffect->GetEfeHandle());
        delete desEffect;
        desEffect = nullptr;
    }
    //if (hitFire != nullptr)
    //{
    //    hitFire->Stop(hitFire->GetEfeHandle());
    //    delete hitFire;
    //    hitFire = nullptr;
    //}

    if (cameraControlle != nullptr)
    {
        delete cameraControlle;
        cameraControlle = nullptr;
    }





   // ProjectileManager::Instance().Clear();
  
}



void Player::Start()
{


    // ���[�u�����g�֐����g����悤��
    movement = GetActor()->GetComponent<Movement>();

    // ������~
    bool stopFall = false;
    movement->SetStopFall(stopFall);

    // �ړ��̒�~
    bool stopMove = false;
    movement->SetStopMove(stopMove);

    // hp�֐����g����悤��
    hp = GetActor()->GetComponent<HP>();

    // mp�֐����g����悤��
    mp = GetActor()->GetComponent<Mp>();

    // �g�����X�t�H�[���֐����Ăяo��
    transform = GetActor()->GetComponent<Transform>();

    // �ʒu��
    position = transform->GetPosition();

    angle = transform->GetAngle();

    scale = transform->GetScale();

    // �d�͐ݒ�
    movement->SetGravity(gravity);

    // ���f���f�[�^������B
    model = GetActor()->GetComponent<ModelControll>()->GetModel();
    // �J����������
    cameraControlle = new CameraController();

    // �q�b�g�G�t�F�N�g�Ǎ� 
    hitEffect = new Effect("Data/Effect/Hit.efk");
    ImpactEffect = new Effect("Data/Effect/rehleckte.efk");
    desEffect = new Effect("Data/Effect/F.efk");

    // �G�t�F�N�g�ǂݍ���
    float effectScale = 1.0f;

    hitFire = std::make_unique<Effect>("Data/Effect/hit fire.efk");
    hitThander = std::make_unique<Effect>("Data/Effect/HitThunder.efk");
    hitIce = std::make_unique<Effect>("Data/Effect/hit Ice.efk");

    lightningAttack = std::make_unique<Effect>("Data/Effect/sunder.efk");


    // �G�t�F�N�g����
    areWork = std::make_unique<Effect>("Data/Effect/tornade.efk");

    //// �㔼�g
    //bornUpStartPoint = "mixamorig:Hips";
    //// �����g
    //bornDownerEndPoint = "mixamorig:LeftUpLeg";

     // �㔼�g�X�^�[�g�Đ��J�n�ꏊ
    bornUpStartPoint = "mixamorig:Hips";
    // �㔼�g�G���h�Đ���~�ꏊ
    bornUpEndPoint = "mixamorig:LeftUpLeg";
    // �����g�X�^�[�g�Đ��J�n�ꏊ
    bornDownerStartPoint = "mixamorig:LeftUpLeg";
    // �����g�G���h�Đ���~�ꏊ
    bornDownerEndPoint = "mixamorig:RightToe_End";

    // hp�ݒ�
    hp->SetHealth(health);
    // hp�ő�l�̐ݒ�
    hp->SetMaxHealth(maxHealth);


    // mp�ݒ�
    mp->SetMagic(magicPoint);
    // mp�ő�l
    mp->SetMaxMagic(magicPoint);

    // ���a
    transform->SetRadius(radius);
    // �g��
    transform->SetHeight(height);

    // �R�}���h����p
    selectCheck = (int)CommandAttack::Attack;

    // ���@�I��p
    selectMagicCheck = (int)CommandMagic::Normal;

    // ����U�����ߏ����l
    specialAttackCharge = 0.0f;

    // �X�e�[�g�}�V��
    //stateMachine = new StateMachine();
    stateMachine = std::make_unique<StateMachine>();

    stateMachine->RegisterState(new PlayerIdleState(GetActor()));
    stateMachine->RegisterState(new PlayerMovestate(GetActor()));
    stateMachine->RegisterState(new PlayerJumpState(GetActor()));
    stateMachine->RegisterState(new PlayerLandState(GetActor()));
    stateMachine->RegisterState(new PlayerJumpFlipState(GetActor()));
    stateMachine->RegisterState(new PlayerQuickJabState(GetActor()));
    stateMachine->RegisterState(new PlayerSideCutState(GetActor()));
    stateMachine->RegisterState(new PlayerCycloneStrikeState(GetActor()));
    stateMachine->RegisterState(new PlayerSpecialAttackState(GetActor()));
    stateMachine->RegisterState(new PlayerMagicState(GetActor()));
    stateMachine->RegisterState(new PlayerSpecialMagicState(GetActor()));
    stateMachine->RegisterState(new PlayerSpecialMagicIceState(GetActor()));
    stateMachine->RegisterState(new PlayerSpecialThanderMagicState(GetActor()));
    stateMachine->RegisterState(new PlayerDamageState(GetActor()));
    stateMachine->RegisterState(new PlayerDeathState(GetActor()));
    stateMachine->RegisterState(new PlayerReviveState(GetActor()));
    stateMachine->RegisterState(new PlayerAvoidanceState(GetActor()));
    stateMachine->RegisterState(new PlayerReflectionState(GetActor()));

    // �X�e�[�g�Z�b�g
    stateMachine->SetState(static_cast<int>(State::Idle));

    // �A�j���[�V�������[��
    updateanim = UpAnim::Normal;

    moveSpeedAnimation = 0.0f;

    // ����A�N�V�����̎��
    specialAttack.push((int)SpecialAttack::Normal);

    // ����A�N�V���������s
    specialAttackTime = false;

    // �h�ꃂ�[�h
    shakeMode = false;

    // ��]�m�F
    angleCheck = false;

    // �Ȃ��鑬�x
    turnSpeedAdd = 0;

    


}

// �X�V����
// elapsedTime(�o�ߎ���)
void Player::Update(float elapsedTime)
{

    //// �X�e�[�g���̏���
    stateMachine->Update(elapsedTime);

    // �J�����R���g���[���[�X�V����
    cameraControlle->Update(elapsedTime);

    // �G�t�F�N�g�ʒu�X�V
    if (areWork->GetEfeHandle())
    {
        areWork->SetPosition(areWork->GetEfeHandle(), position);
    }
    
    // �R�}���h����
    {
        // �s���I��
        InputSelectCheck();
        // ���@�I��
        InputSelectMagicCheck();
        // ���@�I���V���[�g�J�b�g�L�[
        InputShortCutkeyMagic();



        // ���n���ɃG�t�F�N�g�؂�
        if (movement->GetOnLadius())
        {
            areWork->Stop(areWork->GetEfeHandle());
        }

        // �U���̎��ɃX�e�[�g��ύX
        if (
            InputAttack() && GetSelectCheck() ==
            (int)Player::CommandAttack::Attack &&
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::QuickJab) &&
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::SideCut)&&
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::CycloneStrike)&&
                GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage)
            )
        {
            GetStateMachine()->ChangeState(static_cast<int>(Player::State::QuickJab));


            
            // �����n�ʂȂ牽�����Ȃ�
            bool noStart = false;
            // �G�t�F�N�g�Đ�
            areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;

            areWork->Play(position);
        }

        //�@���@���͏���
        if (InputAttack() && GetSelectCheck() == (int)Player::CommandAttack::Magic &&
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Magic) && 
            !mp->GetMpEmpth() &&
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage)
                )
        {
            GetStateMachine()->ChangeState(static_cast<int>(Player::State::Magic));


            // �����n�ʂȂ牽�����Ȃ�
            bool noStart = false;
            // �G�t�F�N�g�Đ�
            areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;

            areWork->Play(position);
        }
 

        // ����U��
        InputSpecialAttackCharge();
        //if (InputSpecialAttackCharge())
        //{
        //    
        //    //TransitionAttackState();
        //    GetStateMachine()->ChangeState(static_cast<int>(Player::State::SpecialAttack));
        //    // ����U���̔������� ����
        //    specialAttackTime = false;


        //    // �����n�ʂȂ牽�����Ȃ�
        //    bool noStart = false;
        //    // �G�t�F�N�g�Đ�
        //    areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;

        //    areWork->Play(position);
        //        
        //}
        //// ���ꖂ�@
        //if (InputSpecialShotCharge())
        //{

        //    GetStateMachine()->ChangeState(static_cast<int>(Player::State::SpecialMagic));

        //    /*InputProjectile();*/
        //    // ����U���̔������� ����
        //    specialAttackTime = false;


        //    // �����n�ʂȂ牽�����Ȃ�
        //    bool noStart = false;
        //    // �G�t�F�N�g�Đ�
        //    areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;

        //    areWork->Play(position);
        //}

    }

    // ���͏����X�V


    position = transform->GetPosition();

    angle = transform->GetAngle();

    scale = transform->GetScale();

    hp->UpdateInbincibleTimer(elapsedTime);

    // �}�W�b�N��
    mp->MpCharge(elapsedTime);

    //// ���b�N�I��
    InputRockOn();

    hitEffect->SetScale(hitEffect->GetEfeHandle(),{ 1,1,1 });
    // �����x��
    movement->UpdateVelocity(elapsedTime);
    

    // �v���C���[�ƓG�Ƃ̏Փˏ���
    //CollisionBornVsProjectile("shoulder");
    CollisionBornVsProjectile("body2");
    CollisionPlayerVsEnemies();
    // �e�ۓ����蔻��
    CollisionProjectilesVsEnemies();
    // ���r�[�����蔻��
    CollisionRubyVsEnemies();

    // �U���͈͓��Ȃ̂�UI�`��
    AttackCheckUI();
    

    // �Q�[�W�Ǘ�
    UiControlle(elapsedTime);

    // �ʒu�X�V
    transform->UpdateTransform();




    // ���[�V�����X�V����
    switch (updateanim)
    {
    case UpAnim::Stop:
    {
        break;
    }
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
        model->UpdateUpeerBodyAnimation(elapsedTime, bornUpStartPoint, bornUpEndPoint,true);
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

    case UpAnim::Reverseplayback:
    {
        // ���f���t�Đ��A�j���[�V�����X�V����
        model->ReverseplaybackAnimation(elapsedTime, true);
        break;

    }
    }


    // �ʒu�X�V
    model->UpdateTransform(transform->GetTransform());
    
}

void Player::Render(RenderContext& rc, ModelShader& shader)
{
    RockOnUI(rc.deviceContext,rc.view,rc.projection);

    rc.colorGradingData = colorGradingData;

    // �X�y�L�����[������
    rc.isSpecular = 0;

    // �e�I���I�t
    rc.isRimRightning = 1;

    Graphics& graphics = Graphics::Instance();
    shader.Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��
    
    
    shader.Draw(rc, model);
   
    shader.End(rc);


}

// �V���h�E�}�b�v
void Player::RenderShadowmap(RenderContext& rc)
{
    Graphics& graphics = Graphics::Instance();
    ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);
    shader->Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��


    shader->Draw(rc, model);

    shader->End(rc);
    
}

void Player::UpdateCameraState(float elapsedTime)
{
    CameraState oldLockonState = lockonState;
    DirectX::XMFLOAT3 oldLockonCharacter = lockonCharactor;
    lockonState = CameraState::NotLockOn;
    lockonCharactor = {0,0,0};

    
        // ���b�N�I�����[�h
    if (rockCheck)
    {
        Model::Node* PRock = model->FindNode("mixamorig:Spine1");
        //Model::Node* PRock = model->FindNode("mixamorig:Neck");
        DirectX::XMFLOAT3 pPosition =
        {
                    PRock->worldTransform._41,
                    PRock->worldTransform._42,
                    PRock->worldTransform._43
        };
        pPosition.z *= 1.1f;
        DirectX::XMVECTOR p, t, v;
        switch (oldLockonState)
        {
        case	CameraState::NotLockOn:
        {
            // ��ԋ߂������̃L�����N�^�[������
            float	length1, length2;
            int enemyCount = EnemyManager::Instance().GetEnemyCount();
            for (int ii = 0; ii < enemyCount; ++ii)
            {
                Model::Node* characterWorld = EnemyManager::Instance().GetEnemy(ii)->GetComponent<ModelControll>()->GetModel()->FindNode("body1");
                
                DirectX::XMFLOAT3 character = 
                {
                    characterWorld->worldTransform._41,
                    characterWorld->worldTransform._42,
                    characterWorld->worldTransform._43
                };

                if (lockonState != CameraState::NotLockOn)
                {
                   
                    p = DirectX::XMLoadFloat3(&pPosition);
                    t = DirectX::XMLoadFloat3(&lockonCharactor);
                    v = DirectX::XMVectorSubtract(t, p);
                    DirectX::XMStoreFloat(&length2, DirectX::XMVector3LengthSq(v));
                    p = DirectX::XMLoadFloat3(&pPosition);
                    t = DirectX::XMLoadFloat3(&character);
                    v = DirectX::XMVectorSubtract(t, p);
                    DirectX::XMStoreFloat(&length1, DirectX::XMVector3LengthSq(v));
                    if (length1 < length2)
                    {
                        lockonCharactor = character;

                        
                    }
                }
                else
                {
                    p = DirectX::XMLoadFloat3(&pPosition);
                    t = DirectX::XMLoadFloat3(&character);
                    v = DirectX::XMVectorSubtract(t, p);
                    DirectX::XMStoreFloat(&length1, DirectX::XMVector3LengthSq(v));

                    lockonCharactor = character;
                    lockonState = CameraState::LockOn;
                }
            }
            break;
        }
        case	CameraState::LockOn:
        {
            // ���b�N�I���Ώۂ����݂��Ă��邩�`�F�b�N����
            // �Ώۂ�����΃��b�N�I�����p��������
            int enemyCount = EnemyManager::Instance().GetEnemyCount();
            for (int ii = 0; ii < enemyCount; ++ii)
            {
                Model::Node* characterWorld = EnemyManager::Instance().GetEnemy(ii)->GetComponent<ModelControll>()->GetModel()->FindNode("body1");

                DirectX::XMFLOAT3 character =
                {
                    characterWorld->worldTransform._41,
                    characterWorld->worldTransform._42,
                    characterWorld->worldTransform._43
                };
                //if (character == oldLockonCharacter)
                //{
                //lockonCharactor = character;
                lockonState = CameraState::LockOn;
                p = DirectX::XMLoadFloat3(&pPosition);
                t = DirectX::XMLoadFloat3(&character);
                v = DirectX::XMVectorSubtract(t, p);

                lockonCharactor = character;
                //DirectX::XMStoreFloat3(&lockDirection, DirectX::XMVector3Normalize(v));
                //break;

            }
            // �E�X�e�B�b�N�Ń��b�N�I���Ώۂ�ύX���鏈��
            GamePad& gamePad = Input::Instance().GetGamePad();
            float ax = gamePad.GetAxisRX();	// �����̂�
            // ���������͎g��Ȃ��ł���
            lockonTargetChangeTime -= 60.0f * elapsedTime;
            if (
                lockonTargetChangeTime <= 0 &&
                ax * ax > 0.3f)
            {
                lockonTargetChangeTime = lockonTargetChangeTimeMax;
                // ���b�N�I���ΏۂƎ������g�̈ʒu����x�N�g�����Z�o
                float dx = oldLockonCharacter.x - pPosition.x;
                float dz = oldLockonCharacter.z - pPosition.z;
                float l = sqrtf(dx * dx + dz * dz);
                dx /= l;
                dz /= l;
                // �O�ς�p���č��E������s���A�p�x�I�ɍł��߂��ΏۂɃ��b�N�I����ς���
                float angleMax = FLT_MAX;
                for (int ii = 0; ii < enemyCount; ++ii)
                {
                    //std::shared_ptr<Transform> character = EnemyManager::Instance().GetEnemy(ii)->GetComponent<Transform>();
                    
                    Model::Node* characterWorld = EnemyManager::Instance().GetEnemy(ii)->GetComponent<ModelControll>()->GetModel()->FindNode("body1");

                    DirectX::XMFLOAT3 character =
                    {
                        characterWorld->worldTransform._41,
                        characterWorld->worldTransform._42,
                        characterWorld->worldTransform._43
                    };

       /*             if (character == oldLockonCharacter)
                        continue;*/
                    float ddx = character.x - pPosition.x;
                    float ddz = character.z - pPosition.z;
                    float ll = sqrtf(ddx * ddx + ddz * ddz);
                    ddx /= ll;
                    ddz /= ll;
                    float cross = dx * ddz - dz * ddx;
                    if (ax > 0 && cross < 0)
                    {
                        cross = abs(cross);
                        if (cross < angleMax)
                        {
                            angleMax = cross;
                            lockonCharactor = character;
                        }
                    }
                    else if (ax < 0 && cross > 0)
                    {
                        if (cross < angleMax)
                        {
                            angleMax = cross;
                            lockonCharactor = character;
                        }
                    }
                }
            }
            break;
        }
        }
        if (lockonState == CameraState::LockOn)
        {
            MessageData::CAMERACHANGELOCKONMODEDATA	p = { pPosition, lockonCharactor };
            Messenger::Instance().SendData(MessageData::CAMERACHANGELOCKONMODE, &p);
            //break;
        }
    }
    else if(freeCameraCheck)
    {
        Model::Node* PRock = model->FindNode("mixamorig:Spine1");
        //Model::Node* PRock = model->FindNode("mixamorig:Hips");
        //Model::Node* PRock = model->FindNode("mixamorig:Neck");
        DirectX::XMFLOAT3 pPosition =
        {
                    PRock->worldTransform._41,
                    PRock->worldTransform._42,
                    PRock->worldTransform._43
        };
        MessageData::CAMERACHANGEFREEMODEDATA	p = { pPosition };
        Messenger::Instance().SendData(MessageData::CAMERACHANGEFREEMODE, &p);
    }


    
}



// �f�o�b�O�v���~�e�B�u�`��
void Player::DrawDebugPrimitive()
{
    // �������@�̓��ق����Ȃ玩���Œǉ�
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //// �Փ˔���p�̃f�o�b�O����`��
    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    // �Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder( 
            position, radius, height, DirectX::XMFLOAT4(0, 0, 1, 1));


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
#ifdef _DEBUG
void Player::OnGUI()
{

    ImGui::InputFloat("Move Speed", &moveSpeed);
    ImGui::InputInt("selectCheck", &selectCheck);
    ImGui::InputInt("selectMagicCheck", &selectMagicCheck);
    //ImGui::InputInt("specialAttack.top", &specialAttack.top());
    ImGui::SliderFloat("specialAttackCharge", &specialAttackCharge,0,1.5f);
    ImGui::SliderFloat("specialShotCharge", &specialShotCharge,0,1.5f);

    ImGui::SliderFloat("dot", &dotfake, 0, 1);

    ImGui::SliderFloat("blend", &moveSpeedAnimation, 0.0f, 1.0f);

    
    if (ImGui::Button("RockCamera"))
    {
        SpecialCameraRock = SpecialCameraRock ? false : true;
    }

    ImGui::SliderFloat("ShakeTimer", &shakeTimer,0,10);
    ImGui::SliderFloat("shakePower", &shakePower, 0, 10);

    if (ImGui::Button("CameraShake"))
    {
        // �J�����U��
        MessageData::CAMERASHAKEDATA cameraShakeData;


        cameraShakeData = { shakeTimer , shakePower };

        Messenger::Instance().SendData(MessageData::CAMERASHAKE, &cameraShakeData);

    }


    if (SpecialCameraRock)
    {
        MessageData::CAMERACHANGEMOTIONMODEDATA	p;

        float vx = sinf(angle.y) * 6;
        float vz = cosf(angle.y) * 6;
        p.data.push_back({ 0, {position.x + vx , position.y + 3, position.z + vz }, position });
        p.data.push_back({ 90, {position.x - vx , position.y + 3, position.z - vz }, position });
        p.data.push_back({ 180, {position.x - vx , position.y + 3, position.z - vz }, position });
        

        Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

    }


    if (ImGui::Button("Samunail"))
    {

        //stateMachine->ChangeState((int)State::SpecialAttack);

        MessageData::CAMERACHANGEMOTIONMODEDATA	p;

        position = transform->GetPosition();
        angle = transform->GetAngle();



        float vx = sinf(angle.y) * 6;
        float vz = cosf(angle.y) * 6;

        float vx2 = sinf(angle.y) - 10;
        float vx3 = sinf(angle.y);
        float vz2 = cosf(angle.y) * 7;

        //float vx2 = sinf(angle.y);
        //float vz2 = cosf(angle.y);

        //DirectX::XMFLOAT3 ePos = EnemyManager::Instance().GetEnemy(0)->GetComponent<Transform>()->GetPosition();

        //DirectX::XMFLOAT3 pos;

        //pos =
        //{
        //     position.x - ePos.x,
        //     position.y - ePos.y,
        //     position.z - ePos.z,

        //};

        p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
        p.data.push_back({ 50, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
        p.data.push_back({ 90, {position.x + vx3 , position.y + 2, (position.z + 0.1f) - vz2 }, position });

        Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

        // �A�j���[�V�����Đ�
        model->PlayAnimation(
            Player::Anim_SpecialAttack, false,
            0.0f, 0.2f
        );


        angleCameraCheck = true;

       

        //MessageData::CAMERACHANGEMOTIONMODEDATA	p;

        //float vx = sinf(angle.y) * 6;
        //float vz = cosf(angle.y) * 6;
        //p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });

        //Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);



        
    }

    if (angleCameraCheck)
    {
        Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");

        DirectX::XMFLOAT3 pPosition =
        {
                    pHPosiiton->worldTransform._41,
                    pHPosiiton->worldTransform._42,
                    pHPosiiton->worldTransform._43
        };


        // �C�ӂ̃A�j���[�V�����Đ���Ԃł̂ݏՓ˔��菈��������
        float animationTime = model->GetCurrentANimationSeconds();

        if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON)
        {
            hitCheck = true;
        }

        // �A�j���[�V����
        if (animationTime >= 1.6f)
        {
            // �J�����U��
            MessageData::CAMERASHAKEDATA cameraShakeData;

            //float shakeTimer = 0.5f;
            //float shakePower = 0.8f;

            float shakeTimer2 = 0.3f;
            cameraShakeData = { shakeTimer2 , shakePower };

            Messenger::Instance().SendData(MessageData::CAMERASHAKE, &cameraShakeData);

            lightningAttack->Play(pPosition);
            angleCameraCheck = false;

            flashOn = true;

  
        }
    }

    ImGui::SliderFloat("brigthness", &colorGradingData.brigthness,0.0f ,10.0f);
    ImGui::SliderFloat("hueShift", &colorGradingData.hueShift,0.0f ,10.0f);
    ImGui::SliderFloat("saturation", &colorGradingData.saturation,0.0f ,10.0f);
}
#endif // _DEBUG

// �ړ����͏���
bool Player::InputMove(float elapsedTime)
{

    // ���͏����擾
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    // ���i�e�۔��ˁ@r�{�^������������
  
    if (ax + FLT_EPSILON != 0.0f + FLT_EPSILON &&
        GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage))
    {

        return true;
    }

    if (ay + FLT_EPSILON != 0.0f + FLT_EPSILON &&
        GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage))
    {

        return true;
    }

    //if (gamePad.GetButtonDown() & GamePad::BTN_DOWN &&
    //    GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage))
    //{

    //    return true;
    //}

    //if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT &&
    //    GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage))
    //{

    //    return true;
    //}

    //if (gamePad.GetButtonDown() & GamePad::BTN_LEFT &&
    //    GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage))
    //{

    //    return true;
    //}


    //// �i�s�x�N�g���擾
    ////DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //moveVec = GetMoveVec(elapsedTime);

    //
    //// �i�s�x�N�g�����[���x�N�g���łȂ��ꍇ�͓��͂��ꂽ
    //if(GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage))
    //return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;

    return false;
    

}
bool Player::InputRockOn()
{
   

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT_SHOULDER && !buttonRock && !rockCheck)
    {

        buttonRock = true;
        rockCheck = true;
        return true;
    }


    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT_SHOULDER && !buttonRock && rockCheck)
    {
        //cameraControlle->SetTarget(position);
        rockCheck = false;
        buttonRock = true;
        return true;
    }


    if (gamePad.GetButtonUp() & GamePad::BTN_RIGHT_SHOULDER)
        buttonRock = false;

    return false;
}
void Player::RockOnUI(ID3D11DeviceContext* dc,
    const DirectX::XMFLOAT4X4& view,
    const DirectX::XMFLOAT4X4& projection)
{
    //if (!rockCheck)return;
    // �r���[�|�[�g ��ʂ̃T�C�Y��
    // �r���[�|�[�g�Ƃ�2D�̉�ʂɕ`��͈͂̎w��(�N���b�s���O�w����o����)�ʒu���w��
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    // ���X�^���C�U�[�X�e�[�g�Ƀo�C���h����Ă���r���[�|�[�g�z����擾
    dc->RSGetViewports(&numViewports, &viewport);

    // �ϊ��s��
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
    // ���[�J�����烏�[���h�ɍs���Ƃ��ɂ���z����̃|�W�V������n���B
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

    std::shared_ptr<Ui> uiIdSight;
    std::shared_ptr<TransForm2D> uiIdSightTransform;

    std::shared_ptr<Ui> uiIdSightMove; 
    std::shared_ptr<TransForm2D> uiIdSightMoveTransform;

    int uiCount = UiManager::Instance().GetUiesCount();

    if (uiCount <= 0) return;
        uiIdSight = UiManager::Instance().GetUies((int)UiManager::UiCount::Sight)->GetComponent<Ui>();
        uiIdSightTransform = UiManager::Instance().GetUies((int)UiManager::UiCount::Sight)->GetComponent<TransForm2D>();
        uiIdSightMove = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<Ui>();
        uiIdSightMoveTransform = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<TransForm2D>();
    
    // �S�Ă̓G�̓����HP�Q�[�W��\��
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();

    for (int i = 0; i < enemyCount; ++i)
    {


        Model::Node* characterWorld = EnemyManager::Instance().GetEnemy(i)->GetComponent<ModelControll>()->GetModel()->FindNode("body1");

        //std::shared_ptr <Transform> enemy = enemyManager.GetEnemy(i)->GetComponent<Transform>();
        // �G�l�~�[����
        DirectX::XMFLOAT3 worldPosition =
        {
            
            characterWorld->worldTransform._41,
            characterWorld->worldTransform._42,
            characterWorld->worldTransform._43
        };

        // ���[���h����X�N���[��
        DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);






        // �Q�[�W�`�� // ���[���h����X�N���[��

        DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
            WorldPosition,
            viewport.TopLeftX,
            viewport.TopLeftY,
            viewport.Width,
            viewport.Height,
            viewport.MinDepth,
            viewport.MaxDepth,
            Projection,
            View,
            World

        );
        // �X�N���[�����W
        DirectX::XMFLOAT3 scereenPosition;
        DirectX::XMStoreFloat3(&scereenPosition, ScreenPosition);

        // �Q�[�W����
        //const float gaugeWidth = 30.0f;
        //const float gaugeHeight = 5.0f;

        //float healthRate = enemy->GetHealth() / static_cast<float>(enemy->GetMaxHealth());


        if (scereenPosition.z < 0.0f || scereenPosition.z > 1.0f && !rockCheck)
        {
            bool drawCheck = false;
            uiIdSight->SetDrawCheck(drawCheck);

            //uiIdSightMove->SetDrawCheck(drawCheck);

            continue;
        }

        bool drawCheck = true;
        uiIdSight->SetDrawCheck(drawCheck);

       // uiIdSightMove->SetDrawCheck(drawCheck);

        // 2D�X�v���C�g�`��
        {
            uiIdSightTransform->SetPosition(
                { 
                    scereenPosition.x,
                    scereenPosition.y
                });
            uiIdSightMoveTransform->SetPosition(
                {
                    scereenPosition.x - 34,
                    scereenPosition.y - 25
                });

            //// �`��
            //gauge->Render(dc,
            //    scereenPosition.x - gaugeWidth * 0.5f,
            //    scereenPosition.y - gaugeHeight
            //    , gaugeWidth * healthRate
            //    , gaugeHeight,
            //    0, 0,
            //    static_cast<float> (gauge->GetTextureWidth()),
            //    static_cast<float> (gauge->GetTextureHeight()),
            //    0.0f,
            //    1, 0, 0, 1);
            // {�ʒu}{�T�C�Y}{�摜�ǂ�����}{�摜�����܂�}
            // dc , �o�͈́p�o�p
        }



    }

}
// �U�����@�I��
bool Player::InputSelectCheck()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_UP && !magicAction)
    {
        ++selectCheck;
    }
    if (gamePad.GetButtonDown() & GamePad::BTN_DOWN && !magicAction)
    {
        --selectCheck;
    }
    // �R�}���h����K�w����
    if (gamePad.GetButtonDown() & GamePad::BTN_B && selectCheck == (int)CommandAttack::Magic && !magicAction)
    {
        magicAction = true;

        gamePad.SetButtonDownCountinue(true);
    }
    // ��x�����܂Ń{�^�������Ȃ�
    if (gamePad.GetButtonUp() & GamePad::BTN_B)
    {
        gamePad.SetButtonDownCountinue(false);
    }
    // �R�}���h����K�w����
    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT && selectCheck == (int)CommandAttack::Magic)
    {
        magicAction = true;
    }
    // ���[�v���� �ő�ɂ�������
    if (selectCheck > (int)CommandAttack::Magic)
    {
        selectCheck = (int)CommandAttack::Attack;
    }
    // ���[�v����@�ŏ��ɍs������
    if (selectCheck < (int)CommandAttack::Attack)
    {
        selectCheck = (int)CommandAttack::Magic;
    }

    int uiCount = UiManager::Instance().GetUiesCount();
    // ui����������
    if (uiCount <= 0) return false;

    // UI�ݒ� �K�w������O �I��
    // �U���I��
    if (selectCheck == (int)CommandAttack::Attack)
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttack)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttackCheck)->GetComponent<Ui>();

        
        uiIdAttack->SetDrawCheck(false);
        uiIdAttackCheck->SetDrawCheck(true);
    }
    // ���@�I�񂾎�
    else
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttack)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttackCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(true);
        uiIdAttackCheck->SetDrawCheck(false);
    }
    // ���@�I�񂾎�
    if (selectCheck == (int)CommandAttack::Magic)
    {
        std::shared_ptr<Ui> uiIdMagick = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagick)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdMagickCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagickCheck)->GetComponent<Ui>();


        uiIdMagick->SetDrawCheck(false);
        uiIdMagickCheck->SetDrawCheck(true);
    }
    // �U���I��
    else
    {
        std::shared_ptr<Ui> uiIdMagick = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagick)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdMagickCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagickCheck)->GetComponent<Ui>();


        uiIdMagick->SetDrawCheck(true);
        uiIdMagickCheck->SetDrawCheck(false);
    }


    return false;
}

// ���@�̃R�}���h
bool Player::InputSelectMagicCheck()
{

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_UP && magicAction)
    {
        --selectMagicCheck;
    }
    if (gamePad.GetButtonDown() & GamePad::BTN_DOWN && magicAction)
    {
        ++selectMagicCheck;
    }

    if (gamePad.GetButtonDown() & GamePad::BTN_LEFT && magicAction)
    {
        magicAction = false;
        
    }
    if (selectMagicCheck <= (int)CommandMagic::Normal && magicAction)
    {
        selectMagicCheck = (int)CommandMagic::Fire;
    }

    if (selectMagicCheck > (int)CommandMagic::Ice && magicAction)
    {
        selectMagicCheck = (int)CommandMagic::Fire;
    }

    if (selectMagicCheck < (int)CommandMagic::Fire && magicAction)
    {
        selectMagicCheck = (int)CommandMagic::Ice;
    }

    if (!magicAction)
    {
        selectMagicCheck = (int)CommandMagic::Normal;
    }

    int uiCount = UiManager::Instance().GetUiesCount();
    // ui����������
    if (uiCount <= 0) return false;

    // UI�ݒ� ��
    if (selectMagicCheck == (int)CommandMagic::Fire && magicAction)
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFire)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFireCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(false);
        uiIdAttackCheck->SetDrawCheck(true);
    }
    else
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFire)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFireCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(true);
        uiIdAttackCheck->SetDrawCheck(false);
    }
    if (!magicAction)
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFire)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFireCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(false);
        uiIdAttackCheck->SetDrawCheck(false);
    }


    // UI�ݒ� ��
    if (selectMagicCheck == (int)CommandMagic::Thander && magicAction)
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtning)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtningCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(false);
        uiIdAttackCheck->SetDrawCheck(true);
    }
    else
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtning)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtningCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(true);
        uiIdAttackCheck->SetDrawCheck(false);
    }
    if (!magicAction)
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtning)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtningCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(false);
        uiIdAttackCheck->SetDrawCheck(false);
    }


    // UI�ݒ� �X
    if (selectMagicCheck == (int)CommandMagic::Ice && magicAction)
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIce)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIceCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(false);
        uiIdAttackCheck->SetDrawCheck(true);
    }
    else
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIce)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIceCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(true);
        uiIdAttackCheck->SetDrawCheck(false);
    }
    if (!magicAction)
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIce)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIceCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(false);
        uiIdAttackCheck->SetDrawCheck(false);
    }


    return false;
}

// L1
bool Player::InputShortCutkeyMagic()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    // L1 �V���[�g�J�b�g���@
    if (gamePad.GetButtonDown() & GamePad::BTN_LEFT_SHOULDER)
    {
        selectCheck = (int)CommandAttack::Magic;
        selectMagicCheck = (int)CommandMagic::Fire;
        magicAction = true;
    }
    // �����Ă�ԑI��
    if (gamePad.GetButton() & GamePad::BTN_LEFT_SHOULDER)
    {
        magicAction = true;
    }
    // ��������
    if (gamePad.GetButtonUp() & GamePad::BTN_LEFT_SHOULDER)
    {
        selectMagicCheck = (int)CommandMagic::Normal;
        magicAction = false;
    }
    return false;
}

bool Player::InputSpecialAttackCharge()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    int uiCount = UiManager::Instance().GetUiesCount();
    // ui����������
    if (uiCount <= 0) return false;

    if (specialAttackCharge >= 1.5f)
    {
        std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdSpecialChargeSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<Ui>();

        //// �Z�m��
        //std::shared_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
        //std::shared_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();


        //// ��x��������Ə�����
        specialAttackCharge = 0.0f;
        //specialAttack.push((int)SpecialAttack::Attack);

        bool drawCheck = false;
        uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

        uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

        uiIdSpecialChargeSerde->SetDrawCheck(drawCheck);

        //for (int i = 0; i < specialAttack.size(); ++i)
        //{
        //    specialAttack;
        //}

            // ���U�������ȏ㗭�߂���
            if (attackEnergyCharge >= energyChargeMax)
            {
                // �Z�m��
                std::shared_ptr<Ui> uiIdAttackSpecial = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
                std::shared_ptr<TransForm2D> uiIdSpecialAttackTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();


                bool drawCheck = true;
                uiIdAttackSpecial->SetDrawCheck(drawCheck);


                // �K�E�Z�����o�^���ꂽ�� �a��
                specialAttack.push((int)SpecialAttack::Attack);


                DirectX::XMFLOAT2 pos;
                pos = { 94,240 };
                float add = 30;

                if (2 < (int)specialAttack.size())
                    pos.y = pos.y - (add * (float)specialAttack.size());
                uiIdSpecialAttackTransForm2D->SetPosition(pos);

                // ��x��������Ə�����
                specialAttackCharge = 0.0f;

                // �a���K�E�Z�`���[�W����
                attackEnergyCharge = false;
            }


            // �����@�����ȏ㗭�߂���
            if (fireEnergyCharge >= energyChargeMax)
            {
                // �Z�m��
                std::shared_ptr<Ui> uiIdSpecialFire = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<Ui>();
                std::shared_ptr<TransForm2D> uiIdSpecialFireTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<TransForm2D>();


                bool drawCheck = true;
                uiIdSpecialFire->SetDrawCheck(drawCheck);

                // �K�E�Z�����o�^���ꂽ�� ��
                specialAttack.push((int)SpecialAttack::MagicFire);

                DirectX::XMFLOAT2 pos;
                pos = { 94,240 };
                float add = 30;
                if (2 < (int)specialAttack.size())
                    pos.y = pos.y - (add * (float)specialAttack.size());
                uiIdSpecialFireTransForm2D->SetPosition(pos);

                // ��x��������Ə�����
                specialAttackCharge = 0.0f;


                // �ΕK�E�Z�`���[�W����
                fireEnergyCharge = false;
            }

            // �����@�����ȏ㗭�߂���
            if (ThanderEnergyCharge >= energyChargeMax)
            {


                // �Z�m��
                std::shared_ptr<Ui> uiIdSpecialThander = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulThander)->GetComponent<Ui>();
                std::shared_ptr<TransForm2D> uiIdSpecialThanderTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulThander)->GetComponent<TransForm2D>();


                bool drawCheck = true;
                uiIdSpecialThander->SetDrawCheck(drawCheck);

                // �K�E�Z�����o�^���ꂽ�� �a��
                specialAttack.push((int)SpecialAttack::MagicThander);

                DirectX::XMFLOAT2 pos;
                pos = { 94,240 };
                float add = 30;
                if (2 < (int)specialAttack.size())
                    pos.y = pos.y - (add * (float)specialAttack.size());
                uiIdSpecialThanderTransForm2D->SetPosition(pos);

                // ��x��������Ə�����
                specialAttackCharge = 0.0f;


                // ���K�E�Z�`���[�W����
                ThanderEnergyCharge = false;
            }

            // �X���@�����ȏ㗭�߂���
            if (iceEnergyCharge >= energyChargeMax)
            {
                // �Z�m��
                std::shared_ptr<Ui> uiIdSpecialIce = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulIce)->GetComponent<Ui>();
                std::shared_ptr<TransForm2D> uiIdSpecialIceTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulIce)->GetComponent<TransForm2D>();


                bool drawCheck = true;
                uiIdSpecialIce->SetDrawCheck(drawCheck);

                // �K�E�Z�����o�^���ꂽ�� �X
                specialAttack.push((int)SpecialAttack::MagicIce);

                DirectX::XMFLOAT2 pos;
                pos = { 94,240 };
                float add = 30;
                if (2 < (int)specialAttack.size())
                    pos.y = pos.y - (add * (float)specialAttack.size());
                uiIdSpecialIceTransForm2D->SetPosition(pos);


                // ��x��������Ə�����
                specialAttackCharge = 0.0f;


                // �X�K�E�Z�`���[�W����
                iceEnergyCharge = false;
            }

       

        //drawCheck = true;
        //uiIdSpecialShurashu->SetDrawCheck(drawCheck);

        //DirectX::XMFLOAT2 pos;
        //pos = { 94,240 };
        //float add = 30;
        //if (2 < (int)specialAttack.size())
        //    pos.y = pos.y - (add * (float)specialAttack.size());
        //uiIdSpecialShurashuTransForm2D->SetPosition(pos);

    }
    //if (gamePad.GetButtonDown() & GamePad::BTN_Y && specialAttack.top() == (int)SpecialAttack::Attack && !specialAttackTime)
    // �Z�����
    if (gamePad.GetButtonDown() & GamePad::BTN_Y &&  !specialAttackTime && specialAttack.size() > 0)
    {


        //// �Z�m��
        //std::shared_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
        //std::shared_ptr<Ui> uiIdSpecialShurashuPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<Ui>();
        //std::shared_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();
        //std::shared_ptr<TransForm2D> uiIdSpecialShurashuPushTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<TransForm2D>();

        //bool drawCheck = false;
        //uiIdSpecialShurashu->SetDrawCheck(drawCheck);
        //drawCheck = true;
        //uiIdSpecialShurashuPush->SetDrawCheck(drawCheck);

        //DirectX::XMFLOAT2 pos;
        //pos = { uiIdSpecialShurashuTransForm2D->GetPosition() };
        ////float add = 30;
        ////if (1 < (int)specialAttack.size())
        ////    pos.y = pos.y - (add * (float)specialAttack.size());
        //uiIdSpecialShurashuPushTransForm2D->SetPosition(pos);

        // �G�l�~�[�Ăԓz
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyManagerCount = enemyManager.GetEnemyCount();

        // ���삳���邩�ǂ���
        if (enemyManagerCount > 0)
        {

            std::shared_ptr<EnemySlime> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemySlime>();
            std::shared_ptr<Movement> enemyMove = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<Movement>();

            bool moveCheck = false;
            enemy->SetMoveCheck(moveCheck);

            // ���x��~
            bool stopVelocity = true;
            enemyMove->SetStopMove(stopVelocity);
            // ������̒�~
            bool stopFall = true;
            enemyMove->SetStopFall(stopFall);
        }

        switch (specialAttack.top())
        {
        // �a��
        case (int)SpecialAttack::Attack:
        {

            // �Z�m��
            std::shared_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
            std::shared_ptr<Ui> uiIdSpecialShurashuPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<Ui>();
            std::shared_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();
            std::shared_ptr<TransForm2D> uiIdSpecialShurashuPushTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<TransForm2D>();

            bool drawCheck = false;
            uiIdSpecialShurashu->SetDrawCheck(drawCheck);
            //drawCheck = true;
            //uiIdSpecialShurashuPush->SetDrawCheck(drawCheck);

            DirectX::XMFLOAT2 pos;
            pos = { uiIdSpecialShurashuTransForm2D->GetPosition() };
            uiIdSpecialShurashuPushTransForm2D->SetPosition(pos);

            specialAttack.pop();




            GetStateMachine()->ChangeState(static_cast<int>(Player::State::SpecialAttack));

            // �����n�ʂȂ牽�����Ȃ�
            bool noStart = false;
            // �G�t�F�N�g�Đ�
            areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;

            areWork->Play(position);

            break;
        }
        // ���@��
        case (int)SpecialAttack::MagicFire:
        {
            // �Z�m��
            std::shared_ptr<Ui> uiIdSpecialFrame = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<Ui>();
            std::shared_ptr<Ui> uiIdSpecialFramePush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<Ui>();
            std::shared_ptr<TransForm2D> uiIdSpecialFrameTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<TransForm2D>();
            std::shared_ptr<TransForm2D> uiIdSpecialFramePushTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<TransForm2D>();

            bool drawCheck = false;
            uiIdSpecialFrame->SetDrawCheck(drawCheck);
            //drawCheck = true;
            //uiIdSpecialFramePush->SetDrawCheck(drawCheck);

            DirectX::XMFLOAT2 pos;
            pos = { uiIdSpecialFrameTransForm2D->GetPosition() };
            uiIdSpecialFramePushTransForm2D->SetPosition(pos);

            specialAttack.pop();


            GetStateMachine()->ChangeState(static_cast<int>(Player::State::SpecialMagic));

            // �����n�ʂȂ牽�����Ȃ�
            bool noStart = false;
            // �G�t�F�N�g�Đ�
            areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;

            areWork->Play(position);

            break;
        }
        // ���@�X
        case (int)SpecialAttack::MagicIce:
        {
            // �Z�m��
            std::shared_ptr<Ui> uiIdSpecialIce = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulIce)->GetComponent<Ui>();
            std::shared_ptr<Ui> uiIdSpecialIceePush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<Ui>();
            std::shared_ptr<TransForm2D> uiIdSpecialIceTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulIce)->GetComponent<TransForm2D>();
            std::shared_ptr<TransForm2D> uiIdSpecialIcePushTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<TransForm2D>();

            bool drawCheck = false;
            uiIdSpecialIce->SetDrawCheck(drawCheck);
 /*           drawCheck = true;
            uiIdSpecialIceePush->SetDrawCheck(drawCheck);*/

            DirectX::XMFLOAT2 pos;
            pos = { uiIdSpecialIceTransForm2D->GetPosition() };
            uiIdSpecialIcePushTransForm2D->SetPosition(pos);


            specialAttack.pop();


            GetStateMachine()->ChangeState(static_cast<int>(Player::State::SpecialMagicIce));

            // �����n�ʂȂ牽�����Ȃ�
            bool noStart = false;
            // �G�t�F�N�g�Đ�
            areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;

            areWork->Play(position);

            break;
        }
        // ���@��
        case (int)SpecialAttack::MagicThander:
        {
            // �Z�m��
            std::shared_ptr<Ui> uiIdSpecialThander = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulThander)->GetComponent<Ui>();
            std::shared_ptr<Ui> uiIdSpecialThanderPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<Ui>();
            std::shared_ptr<TransForm2D> uiIdSpecialThanderTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulThander)->GetComponent<TransForm2D>();
            std::shared_ptr<TransForm2D> uiIdSpecialThanderPushTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<TransForm2D>();

            bool drawCheck = false;
            uiIdSpecialThander->SetDrawCheck(drawCheck);
            //drawCheck = true;
            //uiIdSpecialThanderPush->SetDrawCheck(drawCheck);

            DirectX::XMFLOAT2 pos;
            pos = { uiIdSpecialThanderTransForm2D->GetPosition() };
            uiIdSpecialThanderPushTransForm2D->SetPosition(pos);

            specialAttack.pop();


            GetStateMachine()->ChangeState(static_cast<int>(Player::State::SpecialMagic));

            // �����n�ʂȂ牽�����Ȃ�
            bool noStart = false;
            // �G�t�F�N�g�Đ�
            areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;

            areWork->Play(position);

            break;
        }
        default:
        {
            break;
        }
        }
   


        // ��x��������Ə�����
        //if (specialAttack.top() != (int)SpecialAttack::Normal)
        //    specialAttack.pop();
        specialAttackTime = true;

        return true;
    }
    else
    {
        specialAttackTime = false;

        // �Z�m��
        //std::shared_ptr<Ui> uiIdSpecialShurashuPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<Ui>();

        //bool drawCheck = false;
        //uiIdSpecialShurashuPush->SetDrawCheck(drawCheck);
    }


    // �`���[�W�����₷��
    if (specialAttackCharge >= 0.4f && specialAttackCharge < 0.8f)
    {
        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialCharge = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialCharge->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = { 110,250 };
        uiIdSpecialTransForm2D->SetPosition(pos);
    }


    // �`���[�W�����₷��
    if (specialAttackCharge >= 0.8f && specialAttackCharge < 1.2f)
    {
        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = { 47,250 };
        uiIdSpecialTransForm2DFurst->SetPosition(pos);

        uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

        pos = { 158,250 };
        uiIdSpecialTransForm2DSecond->SetPosition(pos);
    }

    // �`���[�W�����₷��
    if (specialAttackCharge >= 1.2f && specialAttackCharge < 1.5f)
    {
        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();

        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = { 0,250 };
        uiIdSpecialTransForm2DFurst->SetPosition(pos);

        uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

        pos = { 94,250 };
        uiIdSpecialTransForm2DSecond->SetPosition(pos);

        uiIdSpecialChargeSerde->SetDrawCheck(drawCheck);

        pos = { 205,250 };
        uiIdSpecialTransForm2DSerde->SetPosition(pos);
    }



    //GamePad& gamePad = Input::Instance().GetGamePad();

    //if (specialAttackCharge >= 1.5f)
    //{
    //    std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();
    //    std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();
    //    std::shared_ptr<Ui> uiIdSpecialChargeSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<Ui>();

    //    // �Z�m��
    //    std::shared_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
    //    std::shared_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();


    //    // ��x��������Ə�����
    //    specialAttackCharge = 0.0f;
    //    specialAttack.push((int)SpecialAttack::Attack);

    //    bool drawCheck = false;
    //    uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

    //    uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

    //    uiIdSpecialChargeSerde->SetDrawCheck(drawCheck);

    //    drawCheck = true;
    //    uiIdSpecialShurashu->SetDrawCheck(drawCheck);
    //    
    //    DirectX::XMFLOAT2 pos;
    //    pos = {94,240 };
    //    float add = 30;
    //    if (2 < (int)specialAttack.size())
    //    pos.y = pos.y - (add * (float)specialAttack.size());
    //    uiIdSpecialShurashuTransForm2D->SetPosition(pos);

    //}
    //// �Z�����
    //if (gamePad.GetButtonDown() & GamePad::BTN_Y && specialAttack.top() == (int)SpecialAttack::Attack && !specialAttackTime)
    //{
    // 

    //    // �Z�m��
    //    std::shared_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
    //    std::shared_ptr<Ui> uiIdSpecialShurashuPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<Ui>();
    //    std::shared_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();
    //    std::shared_ptr<TransForm2D> uiIdSpecialShurashuPushTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<TransForm2D>();

    //    bool drawCheck = false;
    //    uiIdSpecialShurashu->SetDrawCheck(drawCheck);
    //    drawCheck = true;
    //    uiIdSpecialShurashuPush->SetDrawCheck(drawCheck);

    //    DirectX::XMFLOAT2 pos;
    //    pos = { uiIdSpecialShurashuTransForm2D->GetPosition() };
    //    //float add = 30;
    //    //if (1 < (int)specialAttack.size())
    //    //    pos.y = pos.y - (add * (float)specialAttack.size());
    //    uiIdSpecialShurashuPushTransForm2D->SetPosition(pos);

    //    // ��x��������Ə�����
    //    if (specialAttack.top() != (int)SpecialAttack::Normal)
    //        specialAttack.pop();
    //    specialAttackTime = true;

    //    return true;
    //}
    //else 
    //{
    //    specialAttackTime = false;

    //    // �Z�m��
    //    std::shared_ptr<Ui> uiIdSpecialShurashuPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<Ui>();

    //    bool drawCheck = false;
    //    uiIdSpecialShurashuPush->SetDrawCheck(drawCheck);
    //}


    //// �`���[�W�����₷��
    //if (specialAttackCharge >= 0.4f && specialAttackCharge < 0.8f)
    //{
    //    std::shared_ptr<TransForm2D> uiIdSpecialTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
    //    std::shared_ptr<Ui> uiIdSpecialCharge = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

    //    bool drawCheck = true;
    //    uiIdSpecialCharge->SetDrawCheck(drawCheck);

    //    DirectX::XMFLOAT2 pos = {110,250 };
    //    uiIdSpecialTransForm2D->SetPosition(pos);
    //}


    //// �`���[�W�����₷��
    //if (specialAttackCharge >= 0.8f && specialAttackCharge < 1.2f)
    //{
    //    std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
    //    std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

    //    std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<TransForm2D>();
    //    std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();

    //    bool drawCheck = true;
    //    uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

    //    DirectX::XMFLOAT2 pos = { 47,250 };
    //    uiIdSpecialTransForm2DFurst->SetPosition(pos);

    //    uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

    //    pos = { 158,250 };
    //    uiIdSpecialTransForm2DSecond->SetPosition(pos);
    //}

    //// �`���[�W�����₷��
    //if (specialAttackCharge >= 1.2f && specialAttackCharge < 1.5f)
    //{
    //    std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
    //    std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

    //    std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<TransForm2D>();
    //    std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();

    //    std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<TransForm2D>();
    //    std::shared_ptr<Ui> uiIdSpecialChargeSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<Ui>();

    //    bool drawCheck = true;
    //    uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

    //    DirectX::XMFLOAT2 pos = { 0,250 };
    //    uiIdSpecialTransForm2DFurst->SetPosition(pos);

    //    uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

    //    pos = { 94,250 };
    //    uiIdSpecialTransForm2DSecond->SetPosition(pos);

    //    uiIdSpecialChargeSerde->SetDrawCheck(drawCheck);

    //    pos = { 205,250 };
    //    uiIdSpecialTransForm2DSerde->SetPosition(pos);
    //}

    return false;
}

bool Player::InputSpecialShotCharge()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    int uiCount = UiManager::Instance().GetUiesCount();
    // ui����������
    if (uiCount <= 0) return false;

    if (specialShotCharge >= 1.5f)
    {
        std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdSpecialChargeSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<Ui>();

        // �Z�m��
        std::shared_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<Ui>();
        std::shared_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<TransForm2D>();


        // ��x��������Ə�����
        specialShotCharge = 0.0f;
        specialAttack.push((int)SpecialAttack::MagicFire);

        bool drawCheck = false;
        uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

        uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

        uiIdSpecialChargeSerde->SetDrawCheck(drawCheck);

        drawCheck = true;
        uiIdSpecialShurashu->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos;
        pos = { 94,240 };
        float add = 30;
        if (2 < (int)specialAttack.size())
            pos.y = pos.y - (add * (float)specialAttack.size());
        uiIdSpecialShurashuTransForm2D->SetPosition(pos);


    }


    if (gamePad.GetButtonDown() & GamePad::BTN_Y && specialAttack.top() == (int)SpecialAttack::MagicFire && !specialAttackTime)
    {
        // �Z�m��
        std::shared_ptr<Ui> uiIdSpecialMagic = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdSpecialMagicPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<Ui>();
        std::shared_ptr<TransForm2D> uiIdSpecialMagicTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<TransForm2D>();
        std::shared_ptr<TransForm2D> uiIdSpecialMagicPushTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<TransForm2D>();

        bool drawCheck = false;
        uiIdSpecialMagic->SetDrawCheck(drawCheck);
        drawCheck = true;
        uiIdSpecialMagicPush->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos;
        pos = { uiIdSpecialMagicTransForm2D->GetPosition() };
        //float add = 30;
        //if (1 > (int)specialAttack.size())
        //    pos.y = pos.y - (add * (float)specialAttack.size());
        uiIdSpecialMagicPushTransForm2D->SetPosition(pos);

        // ��x��������Ə�����
        if (specialAttack.top() != (int)SpecialAttack::Normal)
            specialAttack.pop();
        specialAttackTime = true;
        return true;
    }
    else 
    {
        specialAttackTime = false;

        // �Z�m��
        std::shared_ptr<Ui> uiIdSpecialMagicPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<Ui>();

        bool drawCheck = false;
        uiIdSpecialMagicPush->SetDrawCheck(drawCheck);
    }


    // �`���[�W�����₷��
    if (specialShotCharge >= 0.4f && specialShotCharge < 0.8f)
    {
        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialCharge = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialCharge->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = { 110,250 };
        uiIdSpecialTransForm2D->SetPosition(pos);
    }


    // �`���[�W�����₷��
    if (specialShotCharge >= 0.8f && specialShotCharge < 1.2f)
    {
        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = { 47,250 };
        uiIdSpecialTransForm2DFurst->SetPosition(pos);

        uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

        pos = { 158,250 };
        uiIdSpecialTransForm2DSecond->SetPosition(pos);
    }

    // �`���[�W�����₷��
    if (specialShotCharge >= 1.2f && specialShotCharge < 1.5f)
    {
        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();

        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = { 0,250 };
        uiIdSpecialTransForm2DFurst->SetPosition(pos);

        uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

        pos = { 94,250 };
        uiIdSpecialTransForm2DSecond->SetPosition(pos);

        uiIdSpecialChargeSerde->SetDrawCheck(drawCheck);

        pos = { 205,250 };
        uiIdSpecialTransForm2DSerde->SetPosition(pos);
    }

    return false;
}

void Player::ChargeSpecialEnergyMultiple()
{

    int uiCount = UiManager::Instance().GetUiesCount();
    // ui����������
    if (uiCount <= 0) return;

    // �����@�����ȏ㗭�߂���
    if (fireEnergyCharge >= energyChargeMax)
    {
        // �Z�m��
        std::shared_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
        std::shared_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();


        bool drawCheck = true;
        uiIdSpecialShurashu->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos;
        pos = { 94,240 };
        float add = 30;
        if (2 < (int)specialAttack.size())
            pos.y = pos.y - (add * (float)specialAttack.size());
        uiIdSpecialShurashuTransForm2D->SetPosition(pos);
    }

    // �����@�����ȏ㗭�߂���
    if (ThanderEnergyCharge >= energyChargeMax)
    {

    }

    // �X���@�����ȏ㗭�߂���
    if (iceEnergyCharge >= energyChargeMax)
    {

    }


    // �����܂Œʂ����疂�@�����܂�������������̂�
    isSpecialChargeComplete = false;

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

DirectX::XMFLOAT3 Player::GetMagicMoveVec(float elapsedTime) const
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
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    // y��������炸�Ɂ@���̒������擾
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);

    // ����Y�����������Ă邩�@�E�������΂߂ł��^�������i��łق���Y���O
    //�@�ɂ��鏭���������ς�邾����P�ʃx�N�g���ɂ���P�D�O��
    if (cameraRightLength > 0.0f)
    {
        // �P�ʃx�N�g����
        // �E�����̒P�ʃx�N�g�� 
        cameraRightX = cameraRightX / cameraRightLength;
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
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);// �E����
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);// �܂�����
    // Y�������ɂ͈ړ����Ȃ�
    vec.y = 0.0f;

    if (vec.x != 0 || vec.y != 0 || vec.z != 0)
    {
        movement->Move(vec, moveSpeed, elapsedTime);
    }



    return vec;
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

        int enemyCount = enemyManager.GetEnemyCount();
        // �w��̃m�[�h�ƑS�Ă̓G�𑍓�����ŏՓˏ���
        for (int i = 0; i < enemyCount; ++i)
        {
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(i);


            DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRudius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();



            Model::Node* nodeHeart = enemy->GetComponent<ModelControll>()->GetModel()->FindNode("body2");
            Model::Node* nodeLeftArm = enemy->GetComponent<ModelControll>()->GetModel()->FindNode("boss_left_hand2");
            Model::Node* nodeRightArm = enemy->GetComponent<ModelControll>()->GetModel()->FindNode("boss_right_hand2");
            // �S���ʒu
            DirectX::XMFLOAT3 nodeHeartPosition;
            nodeHeartPosition = {
            nodeHeart->worldTransform._41,
            nodeHeart->worldTransform._42,
            nodeHeart->worldTransform._43
            };

            //  ���r�ʒu
            DirectX::XMFLOAT3 nodeLeftArmPosition;
            nodeLeftArmPosition = {
            nodeLeftArm->worldTransform._41,
            nodeLeftArm->worldTransform._42,
            nodeLeftArm->worldTransform._43
            };

            // �E�r�ʒu
            DirectX::XMFLOAT3 nodeRightArmPosition;
            nodeRightArmPosition = {
            nodeRightArm->worldTransform._41,
            nodeRightArm->worldTransform._42,
            nodeRightArm->worldTransform._43
            };
            std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
            DirectX::XMFLOAT3 projectilePosition = projectile->GetComponent<Transform>()->GetPosition();
            float projectileRadius = projectile->GetComponent<Transform>()->GetRadius();



            //// �Փˏ���
            DirectX::XMFLOAT3 outPositon;


            if (Collision::IntersectSphereVsCylinder(
                projectilePosition,
                leftHandRadius,
                {
                enemyPosition.x,
                enemyPosition.y,
                enemyPosition.z
                },
                enemyRudius,
                enemyHeight,
                outPositon) ||
                Collision::IntersectSpherVsSphere(
                    projectilePosition,
                    leftHandRadius,
                    {
                    nodeHeartPosition.x,
                    nodeHeartPosition.y,
                    nodeHeartPosition.z
                    },
                    enemyRudius,
                    outPositon) ||

                Collision::IntersectSphereVsCylinder(
                    projectilePosition,
                    leftHandRadius,
                    {
                    nodeLeftArmPosition.x,
                    nodeLeftArmPosition.y,
                    nodeLeftArmPosition.z
                    },
                    enemyRudius,
                    enemyHeight,
                    outPositon) ||
                Collision::IntersectSphereVsCylinder(
                    projectilePosition,
                    leftHandRadius,
                    {
                      nodeRightArmPosition.x,
                      nodeRightArmPosition.y,
                      nodeRightArmPosition.z
                    },
                    enemyRudius,
                    enemyHeight,
                    outPositon))
            {

                //for (int i = 0; i < projectileCount; ++i)
                //{
                //int enemyCount = EnemyManager::Instance().GetEnemyCount();
                //std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
                //    for (int j = 0; j < enemyCount; ++j)
                //    {
                //        std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(j);
                //        
                //        DirectX::XMFLOAT3 projectilePosition = projectile->GetComponent<Transform>()->GetPosition();
                //        float projectileRadius = projectile->GetComponent<Transform>()->GetRadius();
                //        
                //        DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
                //        float enemyRadius = enemy->GetComponent<Transform>()->GetRadius();
                //        float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();

                //        // �Փˏ���
                //        DirectX::XMFLOAT3 outPositon;
                //      
                //        if (!projectile->GetComponent<ProjectileHoming>() && !projectile->GetComponent<ProjectileSunder>())return;

                //        // �~���Ɖ~
                //        if (Collision::IntersectSphereVsCylinder(
                //            projectilePosition,
                //            projectileRadius,
                //            {
                //              enemyPosition.x,
                //              enemyPosition.y + enemyHeight / 2,
                //              enemyPosition.z
                //            } ,
                //            enemyRadius,
                //            enemyHeight / 2,
                //            outPositon))

                //        {
                if (!projectile->GetComponent<ProjectileHoming>() && !projectile->GetComponent<ProjectileSunder>())return;
                            // �_���[�W��^����B
                if (enemy->GetComponent<HP>()->ApplyDamage(3, 0.5f))
                {

                    if (enemy->GetComponent<EnemySlime>()->GetStateMachine()->GetStateIndex() != (int)EnemySlime::State::Wander &&
                        enemy->GetComponent<EnemySlime>()->GetStateMachine()->GetStateIndex() != (int)EnemySlime::State::Jump && 
                        enemy->GetComponent<EnemySlime>()->GetStateMachine()->GetStateIndex() != (int)EnemySlime::State::IdleBattle)
                    {
                        // �_���[�W�X�e�[�g��
                        enemy->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState((int)EnemySlime::State::Damage);
                    }


                    // �q�b�g�G�t�F�N�g�Đ�
                    {
                        //DirectX::XMFLOAT3 e = enemyPosition;
                        //e.y += enemyHeight * 0.5f;
                        if (projectile->GetComponent<ProjectileSunder>())
                        {
                            ++ThanderEnergyCharge;
                            hitThander->Play(projectilePosition);
                        }
                        else
                        {
                            switch (projectile->GetComponent<ProjectileHoming>()->GetMagicNumber())
                            {
                            case (int)ProjectileHoming::MagicNumber::Fire:
                            {
                                ++fireEnergyCharge;
                                hitFire->Play(projectilePosition);
                                break;
                            }
                            case (int)ProjectileHoming::MagicNumber::Ice:
                            {
                                ++iceEnergyCharge;
                                hitIce->Play(projectilePosition);
                                break;
                            }
                            }
                        }

                        hitEffect->Play(projectilePosition);


                    }
                    // �����������̕����I����
                    {
                        specialAttackCharge += 0.2f;
                        //specialShotCharge += 0.1f;
                        //specialShotCharge += 1.5f;
                    }
                    if (projectile->GetComponent<ProjectileHoming>())
                        // �e�۔j��
                        projectile->GetComponent<BulletFiring>()->Destroy();
                }
            }
        }
    }
}

void Player::CollisionRubyVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    projectileManager = ProjectileManager::Instance();

    // �S�Ă̓G�Ƒ�������ŏՓˏ���
    int projectileCount = projectileManager.GetProjectileCount();

    for (int i = 0; i < projectileCount; ++i)
    {
        int enemyCount = EnemyManager::Instance().GetEnemyCount();
        std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        for (int j = 0; j < enemyCount; ++j)
        {
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(j);

            DirectX::XMFLOAT3 projectilePosition = projectile->GetComponent<Transform>()->GetPosition();
            float projectileRadius = projectile->GetComponent<Transform>()->GetRadius();

            DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRadius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();

            // �Փˏ���
            DirectX::XMFLOAT3 outPositon;

            if (!projectile->GetComponent<ProjectileThrowing>() )return;

            bool counterCheck;
            counterCheck = projectile->GetComponent<ProjectileThrowing>()->GetCounterCheck();

            // �~���Ɖ~
            if (Collision::IntersectSphereVsCylinder(
                projectilePosition,
                projectileRadius,
                {
                  enemyPosition.x,
                  enemyPosition.y + enemyHeight / 2,
                  enemyPosition.z
                },
                enemyRadius,
                enemyHeight / 2,
                outPositon) && counterCheck)

            {

                // �_���[�W��^����B
                if (enemy->GetComponent<HP>()->ApplyDamage(1, 0.5f))
                {
                    // ������΂�
                    {
                      
                    }
                    // �q�b�g�G�t�F�N�g�Đ�
                    {
                        DirectX::XMFLOAT3 e = enemyPosition;
                        e.y += enemyHeight * 0.5f;


                        hitEffect->Play(e);
                    }
                    // �����������̕����I����
                    {
                        specialShotCharge += 0.1f;
                    }
                    // �e�۔j��
                    projectile->GetComponent<BulletFiring>()->Destroy();
                    enemy->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState
                    ((int)EnemySlime::State::IdleBattle);
                   
                        
                        
               
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
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(i);


            //// �Փˏ���
            DirectX::XMFLOAT3 outPositon;

            DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRadius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();


            if (Collision::IntersectCylinderVsCylinder(
                enemyPosition,
                enemyRadius,
                enemyHeight,
                position, radius, height,
                outPositon))

            {
      
      
                    DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
                    DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemyPosition);
                    DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
                    DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
                    DirectX::XMFLOAT3 normal;
                    DirectX::XMStoreFloat3(&normal, N);

                    if (normal.y > 0.8f)
                    {

                        //���W�����v
                        //Jump(jumpSpeed * 0.5f);
                       // movement->JumpVelocity(jumpSpeed * 0.5f);


                    }
                    else
                    {
                        
                        // �����o����̈ʒu�ݒ�@
                        //enemy->GetComponent<Transform>()->SetPosition(outPositon);
                        //position = outPositon;
                        //position = outPositon;
                        const float power = 1.3f;

                        //position = {
                        //    position.x * -normal.x * power,
                        //    position.y * -normal.y * power,
                        //    position.z * -normal.z * power
                        //};
                        position = outPositon;

                        transform->SetPosition(position);
                        //DirectX::XMFLOAT3 impulse;
                        //impulse.x = normal.x * power;
                        //impulse.y = power * normal.y;
                        //impulse.z = normal.z * power;
   
                        //movement->AddImpulse(impulse);
                    }
                
            }
            

        }
    
    


}

void Player::CollisionBornVsProjectile(const char* bornname)
{

    // �m�[�h�擾
    //Model::Node* nodehand = model->FindNode(bornname);

    EnemyManager& enemyManager = EnemyManager::Instance();

    // �S�Ă̓G�Ƒ�������ŏՓˏ���

    int enemyCount = enemyManager.GetEnemyCount();


    for (int i = 0; i < enemyCount; ++i)
    {
        std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(i);

        // �m�[�h�擾
        Model::Node* node = enemy->GetComponent<ModelControll>()->GetModel()->FindNode(bornname);

        // �m�[�h�ʒu�擾
        DirectX::XMFLOAT3 nodePosition;
        nodePosition = {
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        };

        //// �Փˏ���
        DirectX::XMFLOAT3 outPositon;

        DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
        float enemyRadius = enemy->GetComponent<Transform>()->GetRadius();
        float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();


        if (Collision::IntersectCylinderVsCylinder(
         
            { 
                nodePosition.x, 
                nodePosition.y - 0.1f, 
                nodePosition.z 
            }
            ,
            enemyRadius,
            enemyHeight,
            position, radius, height,
            outPositon))

        {
            

            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemyPosition);
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
            DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
            DirectX::XMFLOAT3 normal;
            DirectX::XMStoreFloat3(&normal, N);

            if (normal.y > 0.8f)
            {

                //���W�����v
                //Jump(jumpSpeed * 0.5f);
                //movement->JumpVelocity(jumpSpeed * 0.5f);


            }
            else
            {
                // �����o����̈ʒu�ݒ�@
                //enemy->GetComponent<Transform>()->SetPosition(outPositon);
                //
                //const float power = 1.3f;

                //float velocityMax = 2;

                //DirectX::XMFLOAT3 impulse;
                //impulse.x = normal.x * power;
                //impulse.y = power * normal.y;
                //impulse.z = normal.z * power;
                //if(movement->GetVelocity().x <= velocityMax)
                //movement->AddImpulse(impulse);
                position = outPositon;
                transform->SetPosition(position);


            }

        }


    }
}

// �m�[�h�ƓG�̏Փ˔���
void Player::CollisionNodeVsEnemies(
    const char* nodeName, float nodeRadius,
    const char* nodeHeartName,
    const char* nodeLeftArmName,
    const char* nodeRightArmName
)
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
        std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(i);


        DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
        float enemyRudius = enemy->GetComponent<Transform>()->GetRadius();
        float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();



        Model::Node* nodeHeart = enemy->GetComponent<ModelControll>()->GetModel()->FindNode(nodeHeartName);
        Model::Node* nodeLeftArm = enemy->GetComponent<ModelControll>()->GetModel()->FindNode(nodeLeftArmName);
        Model::Node* nodeRightArm = enemy->GetComponent<ModelControll>()->GetModel()->FindNode(nodeRightArmName);
        // �S���ʒu
        DirectX::XMFLOAT3 nodeHeartPosition;
        nodeHeartPosition = {
        nodeHeart->worldTransform._41,
        nodeHeart->worldTransform._42,
        nodeHeart->worldTransform._43
        };

        //  ���r�ʒu
        DirectX::XMFLOAT3 nodeLeftArmPosition;
        nodeLeftArmPosition = {
        nodeLeftArm->worldTransform._41,
        nodeLeftArm->worldTransform._42,
        nodeLeftArm->worldTransform._43
        };

        // �E�r�ʒu
        DirectX::XMFLOAT3 nodeRightArmPosition;
        nodeRightArmPosition = {
        nodeRightArm->worldTransform._41,
        nodeRightArm->worldTransform._42,
        nodeRightArm->worldTransform._43
        };


        //// �Փˏ���
        DirectX::XMFLOAT3 outPositon;


        if (Collision::IntersectSphereVsCylinder(
            nodePosition,
            leftHandRadius,
            {
            enemyPosition.x,
            enemyPosition.y,
            enemyPosition.z
            },
            enemyRudius,
            enemyHeight,
            outPositon) ||
            Collision::IntersectSpherVsSphere(
                nodePosition,
                leftHandRadius,
                {
                nodeHeartPosition.x,
                nodeHeartPosition.y,
                nodeHeartPosition.z
                },
                enemyRudius,
                outPositon) ||

            Collision::IntersectSphereVsCylinder(
                nodePosition,
                leftHandRadius,
                {
                nodeLeftArmPosition.x,
                nodeLeftArmPosition.y,
                nodeLeftArmPosition.z
                },
                enemyRudius,
                enemyHeight,
                outPositon) ||
            Collision::IntersectSphereVsCylinder(
                nodePosition,
                leftHandRadius,
                {
                  nodeRightArmPosition.x,
                  nodeRightArmPosition.y,
                  nodeRightArmPosition.z
                },
                enemyRudius,
                enemyHeight,
                outPositon))
        {
            if (enemy->GetComponent<HP>()->ApplyDamage(applyDamageNormal, 0.5f))
            {


                hitEffect->Play(nodePosition);

                    if (enemy->GetComponent<EnemySlime>()->GetStateMachine()->GetStateIndex() != (int)EnemySlime::State::Wander &&
                        enemy->GetComponent<EnemySlime>()->GetStateMachine()->GetStateIndex() != (int)EnemySlime::State::Jump )
                    {
                        // �_���[�W�X�e�[�g��
                        //enemy->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState((int)EnemySlime::State::Damage);

                        // �Đ����[�v
                        bool  loop = false;

                        // �Đ��J�n���� 
                        float currentAnimationStartSeconds = 1.0f;

                        // �Đ����ԉ��Z���̒l
                        float currentAnimationAddSeconds = 0.00f;

                        // �L�[�t���[���̏I��
                        float keyFrameEnd = 153.0f;

                        // �A�j���[�V�����u�����h
                        float blendSeconds = 0.35f;

                        // �ʏ�
                        enemy->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Movie, loop
                            , currentAnimationStartSeconds, blendSeconds, currentAnimationAddSeconds, keyFrameEnd);

                        

                        // ���񂾂Ƃ�
                        if (enemy->GetComponent<EnemySlime>()->GetStateMachine()->GetStateIndex() == (int)EnemySlime::State::IdleBattle)
                        {

                            // �Đ��J�n���� 
                            currentAnimationStartSeconds = 0.3f;


                            // �L�[�t���[���̏I��
                            keyFrameEnd = 55.0f;

                            enemy->GetComponent<ModelControll>()->GetModel()->PlayAnimation(EnemySlime::Animation::Anim_Die, loop
                                , currentAnimationStartSeconds, blendSeconds, currentAnimationAddSeconds, keyFrameEnd);
                        }
                     

   
        
                    }

                    // �����������̕����I����
                    specialAttackCharge += 0.1f;

                    // �a���`���[�W
                    ++attackEnergyCharge;
          
            }
        }
    }

        //// �����g
        //// �~���Ɖ~
        //if (Collision::IntersectSphereVsCylinder(
        //    nodePosition,
        //    leftHandRadius, 
        //    {
        //    enemyPosition.x,
        //    enemyPosition.y,
        //    enemyPosition.z 
        //    },
        //    enemyRudius,
        //    enemyHeight,
        //    outPositon))

        //{

        //    // �_���[�W��^����B
        //    //enemy->ApplyDamage(1);
        //// �_���[�W���ʂ����������BTRUE�ɂȂ邩��
        //    if (enemy->GetComponent<HP>()->ApplyDamage(applyDamageNormal, 0.5f))
        //    {

     
        //        // �q�b�g�G�t�F�N�g�Đ�
        //        {
      

        //            hitEffect->Play(nodePosition);

        //            // �_���[�W�X�e�[�g��
        //            enemy->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState((int)EnemySlime::State::Damage);


        //        }
        //        // �����������̕����I����
        //        {
        //            specialAttackCharge += 0.1f;
        //        }
        //   
        //    }
        //}
     
       // // ��
       // // �~���Ɖ~
       // if (Collision::IntersectSpherVsSphere(
       //     nodePosition,
       //     leftHandRadius,
       //     {
       //     nodeHeartPosition.x,
       //     nodeHeartPosition.y,
       //     nodeHeartPosition.z
       //     },
       //     enemyRudius,
       //     outPositon))

       // {

       //     // �_���[�W��^����B
       //     //enemy->ApplyDamage(1);
       // // �_���[�W���ʂ����������BTRUE�ɂȂ邩��
       //     if (enemy->GetComponent<HP>()->ApplyDamage(applyDamageNormal, 0.5f))
       //     {


       //         // �q�b�g�G�t�F�N�g�Đ�
       //         {


       //             hitEffect->Play(nodePosition);
       //             // �_���[�W�X�e�[�g��
       //             enemy->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState((int)EnemySlime::State::Damage);

       //         }
       //         // �����������̕����I����
       //         {
       //             specialAttackCharge += 0.1f;
       //         }
       //      
       //     }
       // }
       // // ���r
       // // �~���Ɖ~
       // if (Collision::IntersectSphereVsCylinder(
       //     nodePosition,
       //     leftHandRadius,
       //     {
       //     nodeLeftArmPosition.x,
       //     nodeLeftArmPosition.y,
       //     nodeLeftArmPosition.z
       //     },
       //     enemyRudius,
       //     enemyHeight,
       //     outPositon))

       // {

       //     // �_���[�W��^����B
       //     //enemy->ApplyDamage(1);
       // // �_���[�W���ʂ����������BTRUE�ɂȂ邩��
       //     if (enemy->GetComponent<HP>()->ApplyDamage(applyDamageNormal, 0.5f))
       //     {


       //         // �q�b�g�G�t�F�N�g�Đ�
       //         {



       //             hitEffect->Play(nodePosition);
       //             // �_���[�W�X�e�[�g��
       //             enemy->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState((int)EnemySlime::State::Damage);

       //         }
       //         // �����������̕����I����
       //         {
       //             specialAttackCharge += 0.1f;
       //         }
       //         
       //     }
       // }

       // // �E�r
       //// �~���Ɖ~
       // if (Collision::IntersectSphereVsCylinder(
       //     nodePosition,
       //     leftHandRadius,
       //     {
       //       nodeRightArmPosition.x,
       //       nodeRightArmPosition.y,
       //       nodeRightArmPosition.z
       //     },
       //     enemyRudius,
       //     enemyHeight,
       //     outPositon))

       // {

       //     // �_���[�W��^����B
       //     //enemy->ApplyDamage(1);
       // // �_���[�W���ʂ����������BTRUE�ɂȂ邩��
       //     if (enemy->GetComponent<HP>()->ApplyDamage(applyDamageNormal, 0.5f))
       //     {


       //         // �q�b�g�G�t�F�N�g�Đ�
       //         {


       //             hitEffect->Play(nodePosition);
       //             // �_���[�W�X�e�[�g��
       //             enemy->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState((int)EnemySlime::State::Damage);

       //         }
       //         // �����������̕����I����
       //         {
       //             specialAttackCharge += 0.1f;
       //         }
       //         
       //     }
       // }
        //// �_���[�W�m�F
        //if (enemy->GetComponent<HP>()->InvincibleTimerCheck())
        //{
        //    hitMortion = 0;
        //}
        //if (hitMortion < hitMortionMax)
        //{
        //    if (enemy->GetComponent<HP>()->DamageDrawCheck())
        //    {
        //        bool modelDrawCheck = false;
        //        enemy->GetComponent<EnemySlime>()->SetModelDrawCheck(modelDrawCheck);
        //    }
        //    else
        //    {
        //        bool modelDrawCheck = true;
        //        enemy->GetComponent<EnemySlime>()->SetModelDrawCheck(modelDrawCheck);
        //    }

        //}
        //else
        //{
        //    bool modelDrawCheck = true;
        //    enemy->GetComponent<EnemySlime>()->SetModelDrawCheck(modelDrawCheck);
        //}

    }


 


void Player::CollisionNodeVsEnemiesCounter(const char* nodeName, float nodeRadius)
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
    ProjectileManager& projectileManager = ProjectileManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();
    int projectileCount = projectileManager.GetProjectileCount();
    // �w��̃m�[�h�ƑS�Ă̓G�𑍓�����ŏՓˏ���
    for (int i = 0; i < projectileCount; ++i)
    {
        for (int j = 0; j < enemyCount; ++j)
        {
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(j);
            std::shared_ptr<Actor> projectille = projectileManager.GetProjectile(i);

            //enemy->GetComponent<ModelControll>()->GetModel()->FindNode

            if (!projectille->GetComponent<ProjectileImpact>()) return;

            DirectX::XMFLOAT3 projectilePosition = projectille->GetComponent<Transform>()->GetPosition();
            float projectileInRudius = projectille->GetComponent<ProjectileImpact>()->GetRadiusInSide();
            float projectileOutRudius = projectille->GetComponent<ProjectileImpact>()->GetRadiusOutSide();
            float projectileHeight = projectille->GetComponent<Transform>()->GetHeight();


            /*DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRudius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();*/


            //// �Փˏ���
            DirectX::XMFLOAT3 outPositon;

            std::shared_ptr<EnemySlime> enemySlime = enemy->GetComponent<EnemySlime>();


            // ���Ƌ�
            if (Collision::IntersectSpherVsSphere(
                nodePosition,
                nodeRadius,
                projectilePosition,
                projectileOutRudius,
                outPositon) && 
                !Collision::IntersectSpherVsSphere(
                    nodePosition,
                    nodeRadius,
                    projectilePosition,
                    projectileInRudius,
                    outPositon) &&projectille->GetComponent<ProjectileImpact>()) /*&&
                enemySlime->GetCounterJudgment())*/
            {
                // ���������ȉ��Ȃ�ʂ�
                if (projectilePosition.y + projectileHeight < nodePosition.y) return;


                //enemySlime->GetStateMachine()->ChangeState((int)EnemySlime::State::Idle);
                // �q�b�g�G�t�F�N�g�Đ�
                {
                    //DirectX::XMFLOAT3 e = enemyPosition;
                    //e.y += enemyHeight * 0.5f;

                    projectille->GetComponent<ProjectileImpact>()->Destoroy();

                    ImpactEffect->Play(nodePosition);

                    // �q�b�g�������X���[��
                    hitCheck = true;

                    //desEffect->Play(e);

                    //enemySlime->SetCounterJudgment(false);
                }


            }
        }


    }
}

void Player::CollisionNodeVsRubyCounter(const char* nodeName, float nodeRadius)
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
    ProjectileManager& projectileManager = ProjectileManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();
    int projectileCount = projectileManager.GetProjectileCount();
    // �w��̃m�[�h�ƑS�Ă̓G�𑍓�����ŏՓˏ���
    for (int i = 0; i < projectileCount; ++i)
    {
        for (int j = 0; j < enemyCount; ++j)
        {
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(i);
            std::shared_ptr<Actor> projectille = projectileManager.GetProjectile(i);

            //enemy->GetComponent<ModelControll>()->GetModel()->FindNode

            if (!projectille->GetComponent<ProjectileRuby>()) return;
           


            DirectX::XMFLOAT3 projectilePosition = projectille->GetComponent<Transform>()->GetPosition();
            float projectileRudius = projectille->GetComponent<Transform>()->GetRadius();
            float projectileHeight = projectille->GetComponent<Transform>()->GetHeight();


            /*DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRudius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();*/


            //// �Փˏ���
            DirectX::XMFLOAT3 outPositon;

            std::shared_ptr<EnemySlime> enemySlime = enemy->GetComponent<EnemySlime>();


            // ���Ƌ�
            if (Collision::IntersectSpherVsSphere(
                nodePosition,
                nodeRadius,
                projectilePosition,
                projectileRudius,
                outPositon)) /*&&
                enemySlime->GetCounterJudgment())*/
            {
                // ���������ȉ��Ȃ�ʂ�
                if (projectilePosition.y + projectileHeight < nodePosition.y) return;


                //enemySlime->GetStateMachine()->ChangeState((int)EnemySlime::State::Idle);
                // �q�b�g�G�t�F�N�g�Đ�
                {
                    //DirectX::XMFLOAT3 e = enemyPosition;
                    //e.y += enemyHeight * 0.5f;

                    projectille->GetComponent<BulletFiring>()->Destroy();
                    

                    ImpactEffect->Play(nodePosition);

                    // �q�b�g�������X���[��
                    hitCheck = true;

                    //desEffect->Play(e);

                    //enemySlime->SetCounterJudgment(false);
                }


            }
        }


    }
}

void Player::CollisionNodeVsRubyCounterBulletFring(const char* nodeName, float nodeRadius)
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
    ProjectileManager& projectileManager = ProjectileManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();
    int projectileCount = projectileManager.GetProjectileCount();
    // �w��̃m�[�h�ƑS�Ă̓G�𑍓�����ŏՓˏ���
    for (int j = 0; j < enemyCount; ++j)
    {
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(j);
    for (int i = 0; i < projectileCount; ++i)
    {
      
            std::shared_ptr<Actor> projectille = projectileManager.GetProjectile(i);

            if (!projectille->GetComponent<ProjectileThrowing>()) return;

            bool counterCheck = projectille->GetComponent<ProjectileThrowing>()->GetCounterCheck();


            DirectX::XMFLOAT3 projectilePosition = projectille->GetComponent<Transform>()->GetPosition();
            float projectileRudius = projectille->GetComponent<Transform>()->GetRadius();
            float projectileHeight = projectille->GetComponent<Transform>()->GetHeight();


            /*DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRudius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();*/


            //// �Փˏ���
            DirectX::XMFLOAT3 outPositon;

            std::shared_ptr<EnemySlime> enemySlime = enemy->GetComponent<EnemySlime>();


            // ���Ƌ�
            if (Collision::IntersectSpherVsSphere(
                nodePosition,
                nodeRadius,
                projectilePosition,
                projectileRudius,
                outPositon) && !counterCheck) /*&&
                enemySlime->GetCounterJudgment())*/
            {
                // ���������ȉ��Ȃ�ʂ�
               /* if (projectilePosition.y + projectileHeight < nodePosition.y) return;*/


                //enemySlime->GetStateMachine()->ChangeState((int)EnemySlime::State::Idle);
                // �q�b�g�G�t�F�N�g�Đ�
                {
                    //DirectX::XMFLOAT3 e = enemyPosition;
                    //e.y += enemyHeight * 0.5f;

                    //projectille->GetComponent<BulletFiring>()->Destroy();
                    projectille->GetComponent<ProjectileThrowing>()->SetTarget(
                        {
                            enemy->GetComponent<Transform>()->GetPosition().x,
                            enemy->GetComponent<Transform>()->GetPosition().y + enemy->GetComponent<Transform>()->GetHeight(),
                            enemy->GetComponent<Transform>()->GetPosition().z,
                        }
                    );

                    DirectX::XMFLOAT3 direction = GetForwerd(angle);

                    float lifeTimerCounter = 5.0f;
                    projectille->GetComponent<BulletFiring>()->Lanch(direction, projectilePosition, lifeTimerCounter);

                    ImpactEffect->Play(nodePosition);

                    counterCheck = true;
                    projectille->GetComponent<ProjectileThrowing>()->SetCounterCheck(counterCheck);

                    // �q�b�g�������X���[��
                    hitCheck = true;

                    //desEffect->Play(e);

                    //enemySlime->SetCounterJudgment(false);
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

            //ImGui::InputInt("jamplimit", &jumpCount);

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

void Player::Destroy()
{
    PlayerManager::Instance().Remove(GetActor());
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
        if (movement->GetJumpCount() < jumpLimit) {
           
            
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
    if (gamePad.GetButtonDown() & GamePad::BTN_B)
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
            actor->AddComponent<Transform>();
            actor->GetComponent<Transform>()->SetPosition(position);
            actor->GetComponent<Transform>()->SetAngle(angle);
            actor->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f));
            actor->AddComponent<BulletFiring>();
            actor->AddComponent<ProjectileStraight>();
            //actor->AddComponent<Collision>();
            ProjectileManager::Instance().Register(actor);
            //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
            //std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

            // ���ꂪ�QD���̊m�F
            bool check2d = false;
            actor->SetCheck2d(check2d);

            // ��Ԏ���
            float   lifeTimer = 3.0f;
            // ����
            actor->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        }

        //projectile->Lanch(dir, pos);
        return true;
    }


    // �ǔ��e�۔���
    if (gamePad.GetButtonDown() & GamePad::BTN_Y && specialAttackTime)
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
            std::shared_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // �G�̈ʒu
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetComponent<Transform>()->GetPosition());
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
                target = enemy->GetComponent<Transform>()->GetPosition();// �ʒu������
                target.y += enemy->GetComponent<Transform>()->GetHeight() * 0.5f;// �ʒu�ɐg����
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
        actor->AddComponent<Transform>();
        actor->GetComponent<Transform>()->SetPosition(position);
        actor->GetComponent<Transform>()->SetAngle(angle);
        actor->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(3.0f,3.0f,3.0f));
        actor->AddComponent<BulletFiring>();
        actor->AddComponent<ProjectileHoming>();

        // ���ꂪ�QD���̊m�F
        bool check2d = false;
        actor->SetCheck2d(check2d);

        //actor->AddComponent<Collision>();
        ProjectileManager::Instance().Register(actor);
        //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

        // ��Ԏ���
        float   lifeTimer = 3.0f;
        // ����
        projectile->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        projectile->GetComponent<ProjectileHoming>()->SetTarget(target);

        //// ����U���̔�������
        //specialAttackTime = false;
        return true;

    }
    return false;
}
// ������or�J�E���^�[
bool Player::InputAvoidance()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    // ���i�e�۔��ˁ@r�{�^������������
    //if (gamePad.GetButtonDown() & GamePad::BTN_B)
    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {

        return true;
    }


    return false;
}
// �U������
bool Player::InputAttack()
{
    // �U�����͏���

    GamePad& gamePad = Input::Instance().GetGamePad();

    // ���i�e�۔��ˁ@���{�^������������
    //if (gamePad.GetButtonDown() & GamePad::BTN_B)
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


    InputSelectCheck();

    if (InputAttack() && selectCheck == (int)CommandAttack::Attack)
    {
        //stated = state;
        TransitionAttackState();
    }





    // �e�ۓ��͏���
    else if (InputMagicframe() && selectCheck == (int)CommandAttack::Magic && selectMagicCheck == (int)CommandMagic::Fire)
    {
        InputMagicframe();
        //InputProjectile();
        //TransitionAttackState();
    }

    // ����U��
    if (InputSpecialAttackCharge())
    {
        TransitionAttackState();
    }
    // ���ꖂ�@
    if (InputSpecialShotCharge())
    {
        InputProjectile();
    }
    //currentANimationSeconds = model->GetCurrentANimationSeconds();

    // �ړ����͏���
    //InputMove(elapsedTime);


}

void Player::TransitionMoveState()
{

    afterimagemove = true;

    updateanim = UpAnim::Doble;

    //// �㔼�g
    //bornUpStartPoint = "mixamorig:Spine";
    //// �����g
    //bornDownerEndPoint = "mixamorig:Spine";


    // ����A�j���[�V�����Đ�
    //model->PlayAnimation(Anim_Running, true);

    //model->PlayUpeerBodyAnimation(Anim_Running, true);
    
    state = State::Move;
}

void Player::UpdateMoveState(float elapsedTime)
{
    // �ʒu��������B
    //position = GetActor()->GetPosition();


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

    InputSelectCheck();

    if (InputAttack() && selectCheck == (int)CommandAttack::Attack)
    {
        //stated = state;
        TransitionAttackState();
    }



    // �e�ۓ��͏���
    else if (selectCheck == (int)CommandAttack::Magic)
    {
        InputProjectile();
    }

    // ����U��
    if (InputSpecialAttackCharge())
    {
        TransitionAttackState();
    }
    // ���ꖂ�@
    if (InputSpecialShotCharge())
    {
        InputProjectile();
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
    //bornUpStartPoint = "mixamorig:Spine";

    //model->PlayUpeerBodyAnimation(Anim_Jump, false);
    //// �W�����v�A�j���[�V�����Đ�
    //model->PlayAnimation(Anim_Jump,false);

}

void Player::UpdateJumpState(float elapsedTime)
{

    //InputMove(elapsedTime);


    // �W�����v���͏���
    if (InputJump()&&movement->GetJumpCount() >= 1)
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
    //model->PlayAnimation(Anim_Landing, false);

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
    //model->PlayAnimation(Anim_Jump_Flip, false);
    //model->PlayUpeerBodyAnimation(Anim_Jump_Flip, false);
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

    //state = State::Attack;

    updateanim = UpAnim::Normal;
    // ����A�j���[�V�����Đ�
    //model->PlayAnimation(Anim_Attack, false);
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
        //bornUpStartPoint = "mixamorig:Spine";
        //// �����g
        //bornDownerEndPoint = "mixamorig:Spine";
       // model->PlayAnimation(Anim_Attack, false);
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
        //CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
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
  
        std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        float dist = FLT_MAX;// float �̍ő�lfloat�S��
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
        {
            // �G�Ƃ̋�������  �G�̐����v�� �S�Ă̓G���Ăɓ����
            std::shared_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);



            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // �G�̈ʒu
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetComponent<Transform>()->GetPosition());
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
                target = enemy->GetComponent<Transform>()->GetPosition();// �ʒu������
                target.y += enemy->GetComponent<Transform>()->GetHeight() * 0.5f;// �ʒu�ɐg����
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
    //model->PlayAnimation(Anim_GetHit1, false);
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
    //model->PlayAnimation(Anim_Death, false);
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
    //model->PlayAnimation(Anim_Revive, false);
}

void Player::UpdateReviveState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();

    }
}

bool Player::InputMagicframe()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    //if (gamePad.GetButtonDown() & GamePad::BTN_B&& magicAction && !gamePad.GetButtonDownCountinue() && !mp->GetMpEmpth())
    //if ( !mp->GetMpEmpth())
    //{
        // mp����
        mp->ApplyConsumption(magicConsumption);

        // �O���� sin�̌v�Z
        DirectX::XMFLOAT3 dir;

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
        target.x = pos.x + dir.x * 1000.0f;
        target.y = pos.y + dir.y * 1000.0f;
        target.z = pos.z + dir.z * 1000.0f;

        // ��ԋ߂��̓G���^�[�Q�b�g�ɂ���
        float dist = FLT_MAX;// float �̍ő�lfloat�S��
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
        {
            // �G�Ƃ̋�������  �G�̐����v�� �S�Ă̓G���Ăɓ����
            std::shared_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // �G�̈ʒu
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetComponent<Transform>()->GetPosition());
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
                target = enemy->GetComponent<Transform>()->GetPosition();// �ʒu������
                target.y += enemy->GetComponent<Transform>()->GetHeight() * 0.5f;// �ʒu�ɐg����
            }



        }

        // �e�ۏ�����
        const char* filename = "Data/Model/Sword/Sword.mdl";

        std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
        actor->AddComponent<ModelControll>();
        actor->GetComponent<ModelControll>()->LoadModel(filename);
        actor->SetName("ProjectileHoming");
        actor->AddComponent<Transform>();
        actor->GetComponent<Transform>()->SetPosition(position);
        actor->GetComponent<Transform>()->SetAngle(angle);
        actor->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
        actor->AddComponent<BulletFiring>();
        actor->AddComponent<ProjectileHoming>();
        const char* effectFilename = "Data/Effect/fire.efk";
        actor->GetComponent<ProjectileHoming>()->SetEffectProgress(effectFilename);
        int magicNumber = (int)ProjectileHoming::MagicNumber::Fire;
        actor->GetComponent<ProjectileHoming>()->SetMagicNumber(magicNumber);
        //actor->GetComponent<ProjectileHoming>()->EffectProgressPlay();
        // ���ꂪ�QD���̊m�F
        bool check2d = false;
        actor->SetCheck2d(check2d);

        //actor->AddComponent<Collision>();
        ProjectileManager::Instance().Register(actor);
        //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

        // ��Ԏ���
        float   lifeTimer = 4.0f;
        // ����
        projectile->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        projectile->GetComponent<ProjectileHoming>()->SetTarget(target);

        // �U������
        magicAction = false;
        selectMagicCheck = (int)CommandMagic::Normal;
     
        return true;
    //}
    //else if (gamePad.GetButtonUp() & GamePad::BTN_B)
    //{
    //    gamePad.SetButtonDownCountinue(false);
    //}
    //return false;
}

bool Player::InputMagicIce()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    //if (gamePad.GetButtonDown() & GamePad::BTN_B && magicAction && !gamePad.GetButtonDownCountinue() && !mp->GetMpEmpth())
        //if ( !mp->GetMpEmpth())
        //{
        // mp����
        mp->ApplyConsumption(magicConsumption);
        // �O���� sin�̌v�Z
        DirectX::XMFLOAT3 dir;

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
        target.x = pos.x + dir.x * 1000.0f;
        target.y = pos.y + dir.y * 1000.0f;
        target.z = pos.z + dir.z * 1000.0f;

        // ��ԋ߂��̓G���^�[�Q�b�g�ɂ���
        float dist = FLT_MAX;// float �̍ő�lfloat�S��
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
        {
            // �G�Ƃ̋�������  �G�̐����v�� �S�Ă̓G���Ăɓ����
            std::shared_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // �G�̈ʒu
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetComponent<Transform>()->GetPosition());
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
                target = enemy->GetComponent<Transform>()->GetPosition();// �ʒu������
                target.y += enemy->GetComponent<Transform>()->GetHeight() * 0.5f;// �ʒu�ɐg����
            }



        }

        // �e�ۏ�����
        const char* filename = "Data/Model/Sword/Sword.mdl";

        std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
        actor->AddComponent<ModelControll>();
        actor->GetComponent<ModelControll>()->LoadModel(filename);
        actor->SetName("ProjectileHoming");
        actor->AddComponent<Transform>();
        actor->GetComponent<Transform>()->SetPosition(position);
        actor->GetComponent<Transform>()->SetAngle(angle);
        actor->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
        actor->AddComponent<BulletFiring>();
        actor->AddComponent<ProjectileHoming>();
        const char* effectFilename = "Data/Effect/brezerd.efk";
        actor->GetComponent<ProjectileHoming>()->SetEffectProgress(effectFilename);
        //actor->GetComponent<ProjectileHoming>()->EffectProgressPlay();

        int magicNumber = (int)ProjectileHoming::MagicNumber::Ice;
        actor->GetComponent<ProjectileHoming>()->SetMagicNumber(magicNumber);

        // ���ꂪ�QD���̊m�F
        bool check2d = false;
        actor->SetCheck2d(check2d);

        //actor->AddComponent<Collision>();
        ProjectileManager::Instance().Register(actor);
        //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);
        
        // ��Ԏ���
        float   lifeTimer = 4.0f;
        // ����
        projectile->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        projectile->GetComponent<ProjectileHoming>()->SetTarget(target);

        // �U������
        magicAction = false;
        selectMagicCheck = (int)CommandMagic::Normal;

        return true;
    //}
    //else if (gamePad.GetButtonUp() & GamePad::BTN_B)
    //{
    //    gamePad.SetButtonDownCountinue(false);
    //}
    //return false;
}

bool Player::InputMagicLightning()
{
   // GamePad& gamePad = Input::Instance().GetGamePad();
   // if (gamePad.GetButtonDown() & GamePad::BTN_B && magicAction && !gamePad.GetButtonDownCountinue() && !mp->GetMpEmpth())
    //if (!mp->GetMpEmpth())
    //{
        // mp����
        mp->ApplyConsumption(magicConsumption);
        // �O���� sin�̌v�Z
        DirectX::XMFLOAT3 dir;

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
        target.x = pos.x + dir.x * 1000.0f;
        target.y = pos.y + dir.y * 1000.0f;
        target.z = pos.z + dir.z * 1000.0f;

        // ��ԋ߂��̓G���^�[�Q�b�g�ɂ���
        float dist = FLT_MAX;// float �̍ő�lfloat�S��
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
        {
            // �G�Ƃ̋�������  �G�̐����v�� �S�Ă̓G���Ăɓ����
            std::shared_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // �G�̈ʒu
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetComponent<Transform>()->GetPosition());
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
                target = enemy->GetComponent<Transform>()->GetPosition();// �ʒu������
                target.y += enemy->GetComponent<Transform>()->GetHeight() * 0.5f;// �ʒu�ɐg����
            }



        }

        // �e�ۏ�����
        const char* filename = "Data/Model/Sword/Sword.mdl";

        std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
        actor->AddComponent<ModelControll>();
        actor->GetComponent<ModelControll>()->LoadModel(filename);
        actor->SetName("ProjectileSunder");
        actor->AddComponent<Transform>();
        actor->GetComponent<Transform>()->SetPosition(pos);
        actor->GetComponent<Transform>()->SetAngle(angle);
        actor->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
        actor->AddComponent<BulletFiring>();
        actor->AddComponent<ProjectileSunder>();
        const char* effectFilename = "Data/Effect/lightningStrike.efk";
        actor->GetComponent<ProjectileSunder>()->SetEffectProgress(effectFilename);
        //actor->GetComponent<ProjectileHoming>()->EffectProgressPlay();


        // ���ꂪ�QD���̊m�F
        bool check2d = false;
        actor->SetCheck2d(check2d);

        //actor->AddComponent<Collision>();
        ProjectileManager::Instance().Register(actor);
        //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

        float   lifeTimer = 0.5f;

        // ����
        projectile->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        projectile->GetComponent<ProjectileSunder>()->SetTarget(target);

        // �U������
        magicAction = false;
        selectMagicCheck = (int)CommandMagic::Normal;

        return true;
    //}
    //else if (gamePad.GetButtonUp() & GamePad::BTN_B)
    //{
    //    gamePad.SetButtonDownCountinue(false);
    //}
    return false;
}

void Player::InputSpecialMagicframe()
{
    // mp����
    //mp->ApplyConsumption(magicConsumption);
    // �O���� sin�̌v�Z
    DirectX::XMFLOAT3 dir;

    dir = GetForwerd(angle);


    DirectX::XMFLOAT3 target;

    // ��ԋ߂��̓G���^�[�Q�b�g�ɂ���
    float dist = FLT_MAX;// float �̍ő�lfloat�S��
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
    {
        // �G�Ƃ̋�������  �G�̐����v�� �S�Ă̓G���Ăɓ����
        std::shared_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);


        target = enemy->GetComponent<Transform>()->GetPosition();// �ʒu������


    }

    // �e�ۏ�����
    const char* filename = "Data/Model/Sword/Sword.mdl";

    std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
    actor->AddComponent<ModelControll>();
    actor->GetComponent<ModelControll>()->LoadModel(filename);
    actor->SetName("MagicFrameTornade");
    actor->AddComponent<Transform>();
    actor->GetComponent<Transform>()->SetPosition(target);
    actor->GetComponent<Transform>()->SetAngle(angle);
    actor->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
    actor->AddComponent<BulletFiring>();
    actor->AddComponent<ProjectileTornade>();
    const char* effectFilename = "Data/Effect/fireTornade.efk";
    actor->GetComponent<ProjectileTornade>()->SetEffectProgress(effectFilename);
    //actor->GetComponent<ProjectileHoming>()->EffectProgressPlay();


    // ���ꂪ�QD���̊m�F
    bool check2d = false;
    actor->SetCheck2d(check2d);

    //actor->AddComponent<Collision>();
    ProjectileManager::Instance().Register(actor);
    //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
    std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

    float   lifeTimer = 0.5f;

    // ����
    projectile->GetComponent<BulletFiring>()->Lanch(dir, target, lifeTimer);
    projectile->GetComponent<ProjectileTornade>()->SetTarget(target);

    //// �U������
    //magicAction = false;
    //selectMagicCheck = (int)CommandMagic::Normal;
}

void Player::AttackCheckUI()
{
    // UI�J�[�\��
    //std::shared_ptr<Ui> uiSight = UiManager::Instance().GetUies((int)UiManager::UiCount::Sight)->GetComponent<Ui>();
    
    int uiCount = UiManager::Instance().GetUiesCount();
    // ui����������
    if (uiCount <= 0) return;

    EnemyManager& enemyManager = EnemyManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();
    switch (selectCheck)
    {
    case (int)CommandAttack::Attack:
    {
        for (int i = 0; i < enemyCount; ++i)
        {
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(i);
            ////// �Փˏ���
            //DirectX::XMFLOAT3 outPositon;

            //DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            //float enemyRadius = enemy->GetComponent<Transform>()->GetRadius();


            DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRudius = enemy->GetComponent<Transform>()->GetRadius() + 1;
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();

            //// �Փˏ���
            DirectX::XMFLOAT3 outPositon;

            // �~���Ɖ~
            if (Collision::IntersectSphereVsCylinder(
                position,
                radius,
                {
                enemyPosition.x,
                enemyPosition.y,
                enemyPosition.z
                },
                enemyRudius,
                enemyHeight,
                outPositon))

            {
                std::shared_ptr<Ui> uiSightAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<Ui>();

                bool drawCheck = true;
                uiSightAttackCheck->SetDrawCheck(drawCheck);
            }
            else
            {
                std::shared_ptr<Ui> uiSightAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<Ui>();

                bool drawCheck = false;
                uiSightAttackCheck->SetDrawCheck(drawCheck);
            }


            //if (Collision::IntersectSpherVsSphere(
            //    position, radius,
            //    enemyPosition,
            //    enemyRadius,
            //    outPositon))

            //{
            //    std::shared_ptr<Ui> uiSightAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<Ui>();

            //    bool drawCheck = true;
            //    uiSightAttackCheck->SetDrawCheck(drawCheck);
            //}
            //else
            //{
            //    std::shared_ptr<Ui> uiSightAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<Ui>();

            //    bool drawCheck = false;
            //    uiSightAttackCheck->SetDrawCheck(drawCheck);
            //}

        }


        break;
    }
    case (int)CommandAttack::Magic:
    {
        for (int i = 0; i < enemyCount; ++i)
        {
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(i);


            DirectX::XMVECTOR playerPosition =
                DirectX::XMLoadFloat3(&position);
            DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();

            DirectX::XMVECTOR enemyPositionXM =
                DirectX::XMLoadFloat3(&enemyPosition);

            DirectX::XMVECTOR LengthSq =
                DirectX::XMVectorSubtract(playerPosition,enemyPositionXM);
            LengthSq = DirectX::XMVector3LengthSq(LengthSq);
            
            float lengthSq;
            DirectX::XMStoreFloat(&lengthSq, LengthSq);
           

            if (lengthSq < magicRangeLength)
            {
                std::shared_ptr<Ui> uiSightAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<Ui>();

                bool drawCheck = true;
                uiSightAttackCheck->SetDrawCheck(drawCheck);
            }
            else
            {
                std::shared_ptr<Ui> uiSightAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<Ui>();

                bool drawCheck = false;
                uiSightAttackCheck->SetDrawCheck(drawCheck);
            }
        }
        break;
    }
    default:
        break;
    }
   

    //    DirectX::XMFLOAT3 eposition;
    //    eposition = enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition();

    //    DirectX::XMFLOAT3 dir =
    //    {
    //    eposition.x - position.x,
    //    eposition.y - position.y,
    //    eposition.z - position.z
    //    };

    //    DirectX::XMVECTOR Length;
    //    float length;

    //    DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&dir);
    //    Length = DirectX::XMVector3LengthSq(Direction);
    //    DirectX::XMStoreFloat(&length, Length);

    //    length

    //}

}

//bool Player::InputMagic()
//{
//    GamePad& gamePad = Input::Instance().GetGamePad();
//    if (gamePad.GetButtonDown() & GamePad::BTN_B)
//    {
//
//        return true;
//    }
//    return false;
//}

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

// �����̓����蔻��L��
void Player::DmageInvalidJudment(bool invalidJudgment)
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
    {
        // �G�Ƃ̋�������  �G�̐����v�� �S�Ă̓G���Ăɓ����
        std::shared_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);

        enemy->GetComponent<EnemySlime>()->SetInvalidJudgment(invalidJudgment);
    }
}

void Player::UiControlle(float elapsedTime)
{
    int uiCount = UiManager::Instance().GetUiesCount();
    // ui����������
    if (uiCount <= 0) return;

    float gaugeWidth = hp->GetMaxHealth() * hp->GetHealth() * 0.1f;
    // hp�Q�[�W
    std::shared_ptr<TransForm2D> uiHp = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerHPBar)->GetComponent<TransForm2D>();
    std::shared_ptr<TransForm2D> uiHpBar = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerHp)->GetComponent<TransForm2D>();
    DirectX::XMFLOAT2 scale = { gaugeWidth, uiHp->GetScale().y };


    uiHp->SetScale(scale);

    gaugeWidth = mp->GetMaxMagic() * mp->GetMagic() * 0.1f;
    // mp�Q�[�W
    std::shared_ptr<TransForm2D> uiMp = UiManager::Instance().GetUies((int)UiManager::UiCount::Mp)->GetComponent<TransForm2D>();
    std::shared_ptr<Ui> uiColor = UiManager::Instance().GetUies((int)UiManager::UiCount::Mp)->GetComponent<Ui>();
    scale = { gaugeWidth, uiMp->GetScale().y };

    uiMp->SetScale(scale);
    // mp�F
   mpUiColor = { 1,1,1,1 };
    if (mp->GetMpEmpth())
    {
        mpUiColor = { 1,0,0,1 };
    }
   uiColor->SetColor(mpUiColor);



    // �h��
    if (shakeMode)
    {
        
        uiHp->Shake();
        uiHpBar->Shake();
        

        

    }
    //�@������
    if (uiHp->GetShakeEnd())
    {
        float positionStandardBar = 593;
        float positionStandard = 421;
        shakeMode = false;
        uiHp->SetShakeTime(0);
        uiHpBar->SetShakeTime(0);
        uiHp->SetPosition({ uiHp->GetPosition().x ,positionStandardBar });
        uiHpBar->SetPosition({ uiHpBar->GetPosition().x ,positionStandard });
    
    }
}

void Player::MagicPointUpdate()
{
    
    
}

void Player::SpecialApplyDamageInRadius()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    
    int enemyCount = enemyManager.GetEnemyCount();
    // �w��̃m�[�h�ƑS�Ă̓G�𑍓�����ŏՓˏ���
    for (int i = 0; i < enemyCount; ++i)
    {
        std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(i);


        DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
        float enemyRudius = enemy->GetComponent<Transform>()->GetRadius();
        float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();
        ProjectileManager& projectileManager = ProjectileManager::Instance();
        for (int ii = 0; ii < projectileManager.GetProjectileCount(); ++ii)
        {
            std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(ii);
            
            if (!projectile->GetComponent<ProjectileTornade>()) return;
            
            // ���@�ʒu
            DirectX::XMFLOAT3 magicPosition = projectile->GetComponent<Transform>()->GetPosition();

            float magicRadius = projectile->GetComponent<Transform>()->GetRadius();

            float magicHeight = projectile->GetComponent<Transform>()->GetHeight();

            //// �Փˏ���
            DirectX::XMFLOAT3 outPositon;
            // �����g
            // �~���Ɖ~
            if (Collision::IntersectCylinderVsCylinder(
                magicPosition,
                magicRadius,
                magicHeight,
                enemyPosition,
                enemyRudius,
                enemyHeight,
                outPositon))

            {

                // �_���[�W��^����B
                //enemy->ApplyDamage(1);
            // �_���[�W���ʂ����������BTRUE�ɂȂ邩��
                if (enemy->GetComponent<HP>()->ApplyDamage(applyDamageSpecial, 0.5f))
                {


                    // �q�b�g�G�t�F�N�g�Đ�
                    {


                        hitEffect->Play(enemyPosition);

                        hitFire->Play(enemyPosition);


                    }
                    // �����������̕����I����
                    {
                        //specialAttackCharge += 0.1f;
                    }

                }
            }
        }
    }
}


bool Player::Ground()
{
    if (movement->GetOnLadius())
    {
        //jumpCount = 0;
        //TransitionLandState();
        return true;
    }
    return false;
    
}






//�X�V����
void PlayerManager::DeleteUpdate()
{

    //�j������
    for (std::shared_ptr<Actor> player : removes)
    {
        //std::vector�v�f���폜����ꍇ�̓C�e���[�^�ō폜����
        std::vector<std::shared_ptr<Actor>>::iterator it = std::find(players.begin(), players.end(), player);

        if (it != players.end())
        {
            players.erase(it);
        }

        //�폜
        //delete player;
        
    }
    //�j�����X�g���N���A
    removes.clear();
}

void PlayerManager::Register(std::shared_ptr<Actor> actor)
{
    players.emplace_back(actor);
}



void PlayerManager::Remove(std::shared_ptr<Actor> player)
{
    // �폜�o�^
    removes.insert(player);
}

void PlayerManager::Clear()
{
    for (std::shared_ptr<Actor>& actor : players)// 
    {
        // ���̂��������Ǘ����Ă��鐔�͂��̂܂�
       // delete projectile;
        //Remove(actor);
        actor.reset();
        //actor.reset();
    }

    players.clear();
}

