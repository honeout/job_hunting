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

#include "StateDerived.h"

#include "AfterimageManager.h"

#include "TransForm2D.h"

#include "ProjectileImpact.h"
#include "ProjectileSunder.h"
#include "BulletFiring.h"

#include "UiManager.h"
#include "Ui.h"







// �f�X�g���N�^
Player::~Player()
{

    if (movement)
        movement.reset();
    if (hp)
        hp.reset();
    if (transform)
        transform.reset();

    if (stateMachine)
    {
        delete stateMachine;
        stateMachine = nullptr;
    }

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
    if (fire != nullptr)
    {
        fire->Stop(fire->GetEfeHandle());
        delete fire;
        fire = nullptr;
    }

    if (cameraControlle != nullptr)
    {
        delete cameraControlle;
        cameraControlle = nullptr;
    }





    ProjectileManager::Instance().Clear();
  
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
    hitEffect = new Effect("Data/Effect/sunder.efk");
    ImpactEffect = new Effect("Data/Effect/rehleckte.efk");
    desEffect = new Effect("Data/Effect/F.efk");

    // �G�t�F�N�g�ǂݍ���
    fire = new Effect("Data/Effect/fire.efk");

    // �㔼�g
    bornUpStartPoint = "mixamorig:Spine";
    // �����g
    bornDownerEndPoint = "mixamorig:Spine";

   
    // hp�ݒ�
    hp->SetHealth(health);
    // hp�ő�l�̐ݒ�
    hp->SetMaxHealth(maxHealth);
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
    stateMachine = new StateMachine();

    stateMachine->RegisterState(new PlayerIdleState(GetActor().get()));
    stateMachine->RegisterState(new PlayerMovestate(GetActor().get()));
    stateMachine->RegisterState(new PlayerJumpState(GetActor().get()));
    stateMachine->RegisterState(new PlayerLandState(GetActor().get()));
    stateMachine->RegisterState(new PlayerJumpFlipState(GetActor().get()));
    stateMachine->RegisterState(new PlayerAttackState(GetActor().get()));
    stateMachine->RegisterState(new PlayerDamageState(GetActor().get()));
    stateMachine->RegisterState(new PlayerDeathState(GetActor().get()));
    stateMachine->RegisterState(new PlayerReviveState(GetActor().get()));
    stateMachine->RegisterState(new PlayerAvoidanceState(GetActor().get()));
    stateMachine->RegisterState(new PlayerReflectionState(GetActor().get()));

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



    // �R�}���h����
    {
        // �s���I��
        InputSelectCheck();
        // ���@�I��
        InputSelectMagicCheck();
        // ���@�I���V���[�g�J�b�g�L�[
        InputShortCutkeyMagic();




        // �U���̎��ɃX�e�[�g��ύX
        if (
            InputAttack() && GetSelectCheck() ==
            (int)Player::CommandAttack::Attack && 
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Attack)
            )
        {
            GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));

        }

        // �e�ۓ��͏���
        // ������
        if (selectMagicCheck == (int)Player::CommandMagic::Fire && GetSelectCheck() == (int)Player::CommandAttack::Magic)
        {
            // ������
            InputMagicframe();
            //TransitionAttackState();
        }
        // ��
        if (selectMagicCheck == (int)Player::CommandMagic::Thander && GetSelectCheck() == (int)Player::CommandAttack::Magic)
        {
            // ������
            InputMagicLightning();
            //TransitionAttackState();
        }
        // �X
        if (selectMagicCheck == (int)Player::CommandMagic::Ice && GetSelectCheck() == (int)Player::CommandAttack::Magic)
        {
            // �X����
            InputMagicIce();
            //TransitionAttackState();
        }

        // ����U��
        if (InputSpecialAttackCharge())
        {
            
            //TransitionAttackState();
            GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
            // ����U���̔������� ����
            specialAttackTime = false;
                
        }
        // ���ꖂ�@
        if (InputSpecialShotCharge())
        {
            InputProjectile();
            // ����U���̔������� ����
            specialAttackTime = false;
        }

    }

    // ���͏����X�V


    position = transform->GetPosition();

    angle = transform->GetAngle();

    scale = transform->GetScale();

    hp->UpdateInbincibleTimer(elapsedTime);

    // ���b�N�I��
    InputRockOn();

    // �J�����ݒ�
    if (rockCheck)
    {
        cameraControlle->RockUpdate(elapsedTime, position);

    }
    else
    {
        cameraControlle->SetTarget(
            { 
                position.x,
                position.y + height,
                position.z
            });

        cameraControlle->Update(elapsedTime);
    }

    // �J�����ݒ�
    {


 //       cameraControlle->RockUpdate(elapsedTime);

 //       DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);

 //       DirectX::XMVECTOR EPosition =  DirectX::XMLoadFloat3(&EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<Transform>()->GetPosition());

 //       DirectX::XMVECTOR Pos = DirectX::XMVectorSubtract(EPosition, Position);

 //     

 //       Pos = DirectX::XMVector3Normalize(Pos);

 ///*       Pos = DirectX::XMVectorScale(Pos, 5.0f);

 //       Pos = DirectX::XMVectorAdd(Position, Pos);*/

 //       DirectX::XMFLOAT3 pos;

 //       DirectX::XMStoreFloat3(&pos, Pos);

 //     
 //       
 //       pos = { position.x /2, 0, position.z /2};


 //       cameraControlle->SetTarget(pos);
    }

    hitEffect->SetScale(hitEffect->GetEfeHandle(),{ 1,1,1 });
    // �����x��
    movement->UpdateVelocity(elapsedTime);
    

    // �v���C���[�ƓG�Ƃ̏Փˏ���
    CollisionBornVsProjectile("shoulder");
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
        model->UpdateUpeerBodyAnimation(elapsedTime, bornUpStartPoint,true);
        model->UpdateLowerBodyAnimation(elapsedTime, bornDownerEndPoint, true);
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
    //GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());

    // ���f���s��X�V
    //model->UpdateTransform(transform);
}

void Player::Render(RenderContext& rc, ModelShader& shader)
{
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


    //// �e�ۃf�o�b�O�v���~�e�B�u�`��
    //projectileManager.DrawDebugPrimitive();
    //for (int i = 0; i < projectileManager.GetProjectileCount(); ++i)
    //{

    //    projectileManager.GetProjectile(i)->GetComponent<ProjectileStraight>()->DrawDebugPrimitive();

    //}

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
    ImGui::InputInt("selectCheck", &selectCheck);
    ImGui::InputInt("selectMagicCheck", &selectMagicCheck);
    ImGui::InputInt("specialAttack.top", &specialAttack.top());
    ImGui::SliderFloat("specialAttackCharge", &specialAttackCharge,0,1.5f);
    ImGui::SliderFloat("specialShotCharge", &specialShotCharge,0,1.5f);

    ImGui::SliderFloat("dot", &dotfake, 0, 1);

    ImGui::SliderFloat("blend", &moveSpeedAnimation, 0.0f, 1.0f);

    

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
bool Player::InputRockOn()
{
   

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT_SHOULDER && !buttonRock && !rockCheck)
    {

        buttonRock = true;
        rockCheck = true;
        return true;
    }

    if (rockCheck)
    {
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        DirectX::XMFLOAT3 epos = enemyManager.GetEnemy(0)->GetComponent<Transform>()->GetPosition();
        float eheight = enemyManager.GetEnemy(0)->GetComponent<Transform>()->GetHeight();


        DirectX::XMFLOAT3 pos;
        pos =
        {
            position.x - epos.x,
            position.y - epos.y,
            position.z - epos.z
        };
        //{
        //   epos.x - position.x,
        //   epos.y - position.y,
        //   epos.z - position.z,
        //};

        cameraControlle->SetTarget(
            {
                epos.x, 
                epos.y + eheight, 
                epos.z 
            });
        //cameraControlle->SetTarget(position);
        cameraControlle->SetFrontAngle(
            { 
                pos.x,
                pos.y,
                pos.z
            }
            //{
            //position.x,
            //-position.y,
            //position.z
            //}
        );
    }

    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT_SHOULDER && !buttonRock && rockCheck)
    {
        cameraControlle->SetTarget(position);
        rockCheck = false;
        buttonRock = true;
        return true;
    }


    if (gamePad.GetButtonUp() & GamePad::BTN_RIGHT_SHOULDER)
        buttonRock = false;

    return false;
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

    if (specialAttackCharge >= 1.5f)
    {
        std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdSpecialChargeSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<Ui>();

        // �Z�m��
        std::shared_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
        std::shared_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();


        // ��x��������Ə�����
        specialAttackCharge = 0.0f;
        specialAttack.push((int)SpecialAttack::Attack);

        bool drawCheck = false;
        uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

        uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

        uiIdSpecialChargeSerde->SetDrawCheck(drawCheck);

        drawCheck = true;
        uiIdSpecialShurashu->SetDrawCheck(drawCheck);
        
        DirectX::XMFLOAT2 pos;
        pos = {94,240 };
        float add = 30;
        if (2 < (int)specialAttack.size())
        pos.y = pos.y - (add * (float)specialAttack.size());
        uiIdSpecialShurashuTransForm2D->SetPosition(pos);

    }

    if (gamePad.GetButtonDown() & GamePad::BTN_Y && specialAttack.top() == (int)SpecialAttack::Attack && !specialAttackTime)
    {
        //EnemyManager& enemyManager = EnemyManager::Instance();
        //int enemyCount = enemyManager.GetEnemyCount();
        //for (int i = 0; i < enemyCount; ++i)//float �ő�l�Ȃ��ɂ���G�Ɍ�����
        //{
        //    // �^�[�Q�b�g�܂ł̃x�N�g�����Z�o
        //    DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);

        //    DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition());
        //    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Position, Target);

        //    DirectX::XMFLOAT3 vec;
        //    DirectX::XMStoreFloat3(&vec, Vec);

        //    DirectX::XMFLOAT3 direction = GetForwerd(angle);

        //    // �J�����֌W
        //    cameraControlle->SetAngle(vec);
        //}

        // �Z�m��
        std::shared_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdSpecialShurashuPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<Ui>();
        std::shared_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();
        std::shared_ptr<TransForm2D> uiIdSpecialShurashuPushTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<TransForm2D>();

        bool drawCheck = false;
        uiIdSpecialShurashu->SetDrawCheck(drawCheck);
        drawCheck = true;
        uiIdSpecialShurashuPush->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos;
        pos = { uiIdSpecialShurashuTransForm2D->GetPosition() };
        //float add = 30;
        //if (1 < (int)specialAttack.size())
        //    pos.y = pos.y - (add * (float)specialAttack.size());
        uiIdSpecialShurashuPushTransForm2D->SetPosition(pos);

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
        std::shared_ptr<Ui> uiIdSpecialShurashuPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<Ui>();

        bool drawCheck = false;
        uiIdSpecialShurashuPush->SetDrawCheck(drawCheck);
    }


    // �`���[�W�����₷��
    if (specialAttackCharge >= 0.4f && specialAttackCharge < 0.8f)
    {
        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialCharge = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialCharge->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = {110,250 };
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

    return false;
}

bool Player::InputSpecialShotCharge()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

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

    //if (gamePad.GetButtonDown() & GamePad::BTN_Y && specialAttack.top() == (int)SpecialAttack::MagicFire && !specialAttackTime)
    //{
    //    // ��x��������Ə�����
    //    if (specialAttack.top() != (int)SpecialAttack::Normal)
    //        specialAttack.pop();
    //    specialAttackTime = true;
    //    return true;
    //}

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


void Player::CameraControl(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    cameraControlle->SetAngleY(-angle.y);
    
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



//// �`�揈��
//void Player::Render(RenderContext& rc, ModelShader* shader)
//{
//
//
//    shader->Begin(rc);
//    // �`��
//    shader->Draw(rc, model);
//
//    shader->End(rc);
//    // �e�ە`�揈��
//    //projectileManager.Render(rc, shader);
//}





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
          
            if (!projectile->GetComponent<ProjectileHoming>() && !projectile->GetComponent<ProjectileSunder>())return;

            // �~���Ɖ~
            if (Collision::IntersectSphereVsCylinder(
                projectilePosition,
                projectileRadius,
                {
                  enemyPosition.x,
                  enemyPosition.y + enemyHeight / 2,
                  enemyPosition.z
                } ,
                enemyRadius,
                enemyHeight / 2,
                outPositon))

            {

                // �_���[�W��^����B
                if (enemy->GetComponent<HP>()->ApplyDamage(3, 0.5f))
                {
                    // ������΂�
                    {
                        // �Փ�
        /*                DirectX::XMFLOAT3 impulse;
                        const float power = 10.0f;
                        const DirectX::XMFLOAT3& e = enemyPosition;
                        const DirectX::XMFLOAT3& p = projectilePosition;
                        float vx = e.x - p.x;
                        float vz = e.z - p.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;*/

                       /* impulse.x = vx * power;
                        impulse.y = power * 0.5f;
                        impulse.z = vz * power;

                        enemy->GetComponent<Movement>()->AddImpulse(impulse);*/
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
                        //// �Փ�
                        //DirectX::XMFLOAT3 impulse;
                        //const float power = 10.0f;
                        //const DirectX::XMFLOAT3& e = enemyPosition;
                        //const DirectX::XMFLOAT3& p = projectilePosition;
                        //float vx = e.x - p.x;
                        //float vz = e.z - p.z;
                        //float lengthXZ = sqrtf(vx * vx + vz * vz);
                        //vx /= lengthXZ;
                        //vz /= lengthXZ;

                        //impulse.x = vx * power;
                        //impulse.y = power * 0.5f;
                        //impulse.z = vz * power;

                        //enemy->GetComponent<Movement>()->AddImpulse(impulse);
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
                position, radius, height,
                enemyPosition,
                enemyRadius,
                enemyHeight/2,
                
                outPositon))

            {
                //// ������΂�
                //{
                //    // �Փ�
                //    DirectX::XMFLOAT3 impulse;
                //    const float power = 10.0f;
                //    const DirectX::XMFLOAT3& e = enemyPosition;
                //    const DirectX::XMFLOAT3& p = projectilePosition;
                //    float vx = e.x - p.x;
                //    float vz = e.z - p.z;
                //    float lengthXZ = sqrtf(vx * vx + vz * vz);
                //    vx /= lengthXZ;
                //    vz /= lengthXZ;

                //    impulse.x = vx * power;
                //    impulse.y = power * 0.5f;
                //    impulse.z = vz * power;
                //}

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
                        movement->JumpVelocity(jumpSpeed * 0.5f);


                    }
                    else
                    {
                        
                        // �����o����̈ʒu�ݒ�@
                        //enemy->GetComponent<Transform>()->SetPosition(outPositon);
                        //position = outPositon;
                        const float power = 2.3f;

                        DirectX::XMFLOAT3 impulse;
                        impulse.x = normal.x * power;
                        impulse.y = power * normal.y;
                        impulse.z = normal.z * power;

                        movement->AddImpulse(impulse);
                    }
                
            }
            

        }
    
    


}

void Player::CollisionBornVsProjectile(const char* bornname)
{

    // �m�[�h�擾
    Model::Node* nodehand = model->FindNode(bornname);

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
        float enemyHeight = enemy->GetComponent<Transform>()->GetRadius();


        if (Collision::IntersectCylinderVsCylinder(
            position, radius, height,
            nodePosition,
            enemyRadius,
            enemyHeight,
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
                movement->JumpVelocity(jumpSpeed * 0.5f);


            }
            else
            {
                // �����o����̈ʒu�ݒ�@
                //enemy->GetComponent<Transform>()->SetPosition(outPositon);
                
                const float power = 2.3f;

                float velocityMax = 2;

                DirectX::XMFLOAT3 impulse;
                impulse.x = normal.x * power;
                impulse.y = power * normal.y;
                impulse.z = normal.z * power;
                if(movement->GetVelocity().x <= velocityMax)
                movement->AddImpulse(impulse);


            }

        }


    }
}

// �m�[�h�ƓG�̏Փ˔���
void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius, const char* nodeNameSeconds)
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

        //// �Փˏ���
        DirectX::XMFLOAT3 outPositon;

        // �~���Ɖ~
        if (Collision::IntersectSphereVsCylinder(
            nodePosition,
            nodeRadius, 
            {
            enemyPosition.x,
            enemyPosition.y + enemyHeight / 2,
            enemyPosition.z 
            },
            enemyRudius,
            enemyHeight / 2,
            outPositon))

        {

            // �_���[�W��^����B
            //enemy->ApplyDamage(1);
        // �_���[�W���ʂ����������BTRUE�ɂȂ邩��
            if (enemy->GetComponent<HP>()->ApplyDamage(5, 0.5f))
            {

                //// ������΂�
                //{
                //    // �Փ�
                //    DirectX::XMFLOAT3 impulse;
                //    const float power = 10.0f;
                //    const DirectX::XMFLOAT3& e = enemyPosition;
                //    const DirectX::XMFLOAT3& p = nodePosition;
                //    float vx = e.x - p.x;
                //    float vz = e.z - p.z;
                //    float lengthXZ = sqrtf(vx * vx + vz * vz);
                //    vx /= lengthXZ;
                //    vz /= lengthXZ;

                //    impulse.x = vx * power;
                //    impulse.y = power * 0.5f;
                //    impulse.z = vz * power;

                //    enemy->GetComponent<Movement>()->AddImpulse(impulse);
                //}
                // �q�b�g�G�t�F�N�g�Đ�
                {
                    DirectX::XMFLOAT3 e = enemyPosition;
                    e.y += enemyHeight * 0.5f;


                    hitEffect->Play(e);


                    //desEffect->Play(e);

                }
                // �����������̕����I����
                {
                    specialAttackCharge += 0.1f;
                }

            }
        }



    }
 
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
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(i);
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

    // �㔼�g
    bornUpStartPoint = "mixamorig:Spine";
    // �����g
    bornDownerEndPoint = "mixamorig:Spine";


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
    bornUpStartPoint = "mixamorig:Spine";

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

    state = State::Attack;

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
        bornUpStartPoint = "mixamorig:Spine";
        // �����g
        bornDownerEndPoint = "mixamorig:Spine";
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
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius, "shoulder");
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
    if (gamePad.GetButtonDown() & GamePad::BTN_B&& magicAction && !gamePad.GetButtonDownCountinue())
    {

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
    }
    else if (gamePad.GetButtonUp() & GamePad::BTN_B)
    {
        gamePad.SetButtonDownCountinue(false);
    }
    return false;
}

bool Player::InputMagicIce()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_B && magicAction && !gamePad.GetButtonDownCountinue())
    {

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
    }
    else if (gamePad.GetButtonUp() & GamePad::BTN_B)
    {
        gamePad.SetButtonDownCountinue(false);
    }
    return false;
}

bool Player::InputMagicLightning()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_B && magicAction && !gamePad.GetButtonDownCountinue())
    {

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
    }
    else if (gamePad.GetButtonUp() & GamePad::BTN_B)
    {
        gamePad.SetButtonDownCountinue(false);
    }
    return false;
}

void Player::AttackCheckUI()
{
    // UI�J�[�\��
    //std::shared_ptr<Ui> uiSight = UiManager::Instance().GetUies((int)UiManager::UiCount::Sight)->GetComponent<Ui>();
    

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
            float enemyRudius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();

            //// �Փˏ���
            DirectX::XMFLOAT3 outPositon;

            // �~���Ɖ~
            if (Collision::IntersectSphereVsCylinder(
                position,
                radius,
                {
                enemyPosition.x,
                enemyPosition.y + enemyHeight / 2,
                enemyPosition.z
                },
                enemyRudius,
                enemyHeight / 2,
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

    float gaugeWidth = hp->GetMaxHealth() * hp->GetHealth() * 0.1f;
    // hp�Q�[�W
    std::shared_ptr<TransForm2D> uiHp = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerHPBar)->GetComponent<TransForm2D>();
    std::shared_ptr<TransForm2D> uiHpBar = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerHp)->GetComponent<TransForm2D>();
    DirectX::XMFLOAT2 scale = { gaugeWidth, uiHp->GetScale().y };

    uiHp->SetScale(scale);

    
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

