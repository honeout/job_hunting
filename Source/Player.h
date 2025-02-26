#pragma once

#include <DirectXMath.h>
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
//#include "Character.h"
//#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileManager.h"
#include "Audio/AudioSource.h"
#include "Component.h"
#include "Movement.h"
#include "HP.h"
#include "Mp.h"
#include "Input\GamePad.h"
#include "CameraController.h"
#include "Effect.h"
#include "ModelControll.h"
#include "Transform.h"
#include "StateMachine.h"
#include "PostprocessingRenderer.h"
#include <stack>



// �v���C���[
class Player : public Component
{
public:
    Player() {};
    ~Player() override;



    //// �C���X�^���X�擾
    //static Player& Instance();


    // �A�j���[�V����
    //enum Animation
    //{
    //    Anim_Attack,
    //    Anim_Death,
    //    Anim_Falling,
    //    Anim_GetHit1,
    //    Anim_GetHit2,
    //    Anim_Idle,
    //    Anim_Jump,
    //    Anim_Jump_Flip,
    //    Anim_Landing,
    //    Anim_Revive,
    //    Anim_Running,
    //    Anim_Walking
    //};
    //enum Animation
    //{
    //    Anim_Tpose,
    //    Anim_Idle,
    //    Anim_Move,
    //    Anim_Back,
    //    Anim_Beside,
    //    Anim_BesideSrow,
    //    Anim_Jump,
    //    Anim_Slash,
    //    Anim_SlashBeside,
    //    Anim_SlashThree,
    //    Anim_Deth,
    //    Anim_Pain
    //    
    //};

    // ���O�擾
    const char* GetName() const override { return "Player"; }

    //void UpdateTransform();

    // �J�n����
    void Start() override;

    // �X�V����
    void Update(float elapsedTime) override;

    // �`�揈��
    //void Render(RenderContext rc) override;

    // �f�o�b�O�v���~�e�B�u�`��@�f�o�b�O�p
    void DrawDebugPrimitive();
#ifdef _DEBUG
    // GUI�`��
    void OnGUI() override;
#endif // _DEBUG

    // �`�揈��
    void Render(RenderContext& rc, ModelShader& shader) override;

    // �V���h�E�}�b�v����
    void RenderShadowmap(RenderContext& rc) override;

    // SE�Đ� ����
    void InputWalkSE();
    void InputStopWalkSE();

    // SE�Đ� �W�����v
    void InputJampSE();
    void InputStopJampSE();

    // SE�Đ� ��
    void InputAreWalkSE();

    // SE�Đ� �_�b�V��
    void InputDashSE();

    // SE�Đ� �a��
    void InputAttackSlashSE();

    // SE�Đ� ���@�@��
    void InputAttackFlameSE();

    // SE�Đ� ���@�@��
    void InputAttackThanderSE();
    void InputStopAttackThanderSE();

    // SE�Đ� ���@�@�X
    void InputAttackIceSE();

    // SE�Đ� ���@�@��
    void InputAttackHealeSE();


    // SE�Đ� �K�E�Z�a��
    void InputAttackSlashSpecileLightningStrikeSE();

    // SE�Đ� �K�E�Z���@�@��
    void InputAttackFlameSpecileSE();

    // �J�����؂�ւ�����
    void UpdateCameraState(float elapsedTime);

    // �e�ۂƓG�̏Փˏ���
    void CollisionProjectilesVsEnemies();

    void CollisionRubyVsEnemies();

    // �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
    void CollisionPlayerVsEnemies();

    // �{�[���ƍU���̏Փ˔���
    void CollisionBornVsProjectile(const char* bornname);

    // �f�o�b�O�pGUI�`��
    void DrawDebugGUI();

    // �j��
    void Destroy();

    // �\�[�h�g���C��
    void UpdateSwordeTraile();

    // hp�s���`
    void PinchMode(float elapsedTime);

    // HP�Ǘ�

protected:
    //// ���n�������ɌĂ΂��
    //void OnLanding() override;

    //// ���S�������ɌĂ΂��
    //void OnDead() override;

    //// �_���[�W���󂯂����ɌĂ΂��
    //void OnDamaged() override;

public:



    // �X�e�B�b�N���͒l����ړ��x�N�g�����擾 �i�s�x�N�g�������i�ނׂ�����
    DirectX::XMFLOAT3 GetMoveVec(float elapsedTime) const;

    DirectX::XMFLOAT3 GetMagicMoveVec(float elapsedTime) const;




    // �ړ����͏���
    bool InputMove(float elapsedTime);

    // ���b�N�I������
    bool InputRockOn();

    // ���b�N�I����UI���o��
    void RockOnUI(ID3D11DeviceContext* dc,
        const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection);

    // �U�����@�I��
    bool InputSelectCheck();

    // ���@�I��
    bool InputSelectMagicCheck();

    // �V���[�g�J�b�g�L�[
    bool InputShortCutkeyMagic();

    // ����U���I��
    bool InputSpecialAttackCharge();
    bool InputSpecialShotCharge();

    // UI�̉��o
    void SpecialPlayUlEffect(float elapsedTime);

    // ����Z�`���[�W�o���܂����B
    void ChargeSpecialEnergyMultiple();
 
    bool GetSpecialAttackTime() const { return specialAttackTime; }
    void SetSpecialAttackTime(bool specialAttackTime) { this->specialAttackTime = specialAttackTime; }

    // �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
    //void CollisionPlayerVsEnemies();

    // �m�[�h�ƃG�l�~�[�̏Փˏ���
    bool CollisionNodeVsEnemies(
        const char* nodeName, 
        float nodeRadius,
        const char* nodeHeartName,
        const char* nodeLeftArmName,
        const char* nodeRightArmName

    );

    // �m�[�h�ƒe�ۂ̏Փˏ���
    void CollisionNodeVsEnemiesCounter(const char* nodeName, float nodeRadius);
    
    // �m�[�h�ƃ��r�[����
    void CollisionNodeVsRubyCounter(const char* nodeName, float nodeRadius);

    // �m�[�h�ƃ��r�[���˕Ԃ�
   void CollisionNodeVsRubyCounterBulletFring(const char* nodeName, float nodeRadius);

    // �W�����v���͏���
    bool InputJump();

    // �e�ۓ���
    bool InputProjectile();

    // ���or�J�E���^�[����
    bool InputAvoidance();

    // �U������
    bool InputAttack();

    // ���@����
    bool InputMagick();


    // ���j���[�J������
    bool InputMenue();


    // �ҋ@�X�e�[�g�֑J��
    void TransitionIdleState();
    // �ҋ@�X�e�[�g�X�V
    void UpdateIdleState(float elapsedTime);

    // �ړ��X�e�[�g�֑J��    
    void TransitionMoveState();

    // �ړ��X�e�[�g�֍X�V
    void UpdateMoveState(float elapsedTime);


    // �W�����v�X�e�[�g�֑J��
    void TransitionJumpState();

    // �W�����v�X�e�[�g�X�V����
    void UpdateJumpState(float elapsedTime);

    // ���n�X�e�[�g�֑J��
    void TransitionLandState();

    // ���n�X�e�[�W�X�V����
    void UpdateLandState(float elapsedTime);

    // ��i�W�����v�X�e�[�g�֑J��
    void TransitionJumpFlipState();

    // ��i�W�����v�X�e�[�g�X�V
    void UpdatejumpFlipState(float elapsedTime);
    

    // �U���X�e�[�g�֑J��
    void TransitionAttackState();

    // �U���X�e�[�W�X�V����
    void UpdateAttackState(float elapsedTime);

    void UpdateProjectile(float elapsedTime);

    // �_���[�W�X�e�[�g�֑J��
    void TransitionDamageState();

    // �_���[�W�X�e�[�g�֍X�V����
    void UpdateDamageState(float elapsedTime);

    // ���S�X�e�[�g�֑J��
    void TransitionDeathState();

    // ���S�X�e�[�g�X�V����
    void UpdateDeathState(float elapsedTime);



    // �����X�e�[�g�֑J��
    void TransitionReviveState();

    // �����X�e�[�g�X�V����
    void UpdateReviveState(float elapsedTime);

    // ���@����
    bool InputMagicframe();
    bool InputMagicIce();
    bool InputMagicLightning();
    bool InputMagicHealing();

    void InputSpecialMagicframe();

    void AttackCheckUI();

   // void Update();

    void inFloat3(DirectX::XMFLOAT3 value, DirectX::XMFLOAT3& inValue);

    DirectX::XMFLOAT3 GetForwerd(DirectX::XMFLOAT3 angle);

    // �X�e�[�g�}�V�[���擾
    //StateMachine* GetStateMachine() { return stateMachine; }
    StateMachine* GetStateMachine() { return stateMachine.get(); }
    // �O���ŏ��������邽�߂̏��u
    void StateMachineCreate() { stateMachine = std::make_unique<StateMachine>(); }

    // �U�����@�ύX
    void SetSelectCheck(int selectCheck) { this->selectCheck = selectCheck; }
    // �U�����@�ύX
    int GetSelectCheck() const { return selectCheck; }

    // ���@�ύX
    void SetSelectMagicCheck(int selectMagicCheck) { this->selectMagicCheck = selectMagicCheck; }
    // ���@�ύX
    int GetSelectMagicCheck() const { return selectMagicCheck; }

    // ���@�I��
    void SetMagicAction(bool magicAction) { this->magicAction = magicAction; }
    
    // ���@�I��
    bool GetMagicAction() { return magicAction; }

    // ���ꖂ�@��������
    void SetSpecialAttackCharge(float specialAttackCharge) { this->specialAttackCharge = specialAttackCharge; }

    // ����A�N�V������������
    float GetSpecialAttackCharge() const { return specialAttackCharge; }

    // �U���J�n
    void SetAttackCollisionFlag(bool attackCollisionFlag) { this->attackCollisionFlag = attackCollisionFlag; }

    // �U���J�n
    bool GetAttackCollisionFlag() const { return attackCollisionFlag; }

    // ����̓����蔻��
    float GetLeftHandRadius() const { return leftHandRadius; }

    // �Q�b�g�����蔻�薳��
    bool GetInvalidJudgment() const { return invalidJudgment;}
    // �Z�b�g�����蔻�薳��
    void SetInvalidJudgment(bool invalidJudgment) { this->invalidJudgment = invalidJudgment;}

    // �����̓����蔻��L��
    void DmageInvalidJudment(bool invalidJudgment);

    //  UI����
    void UiControlle(float elapsedTime);

    // ���@�̗�
    void MagicPointUpdate();

    // �_���[�W����
    void SpecialApplyDamageInRadius();


    

public:
    // �Q�[���X�e�[�g
    enum class State
    {
        // �X�e�[�g�����̂͌�납��
        Idle,
        Move,
        Jump,
        Land,
        JumpFlip,
        QuickJab,
        SideCut,
        CycloneStrike,
        SpecialAttack,
        Magic,
        SpecialMagic,
        SpecialMagicIce,
        SpecialMagicThander,
        Damage,
        Death,
        Revive,
        Avoidance,
        Reflection,
    };


    // �^�C�g���X�e�[�g
    enum class StateTitle
    {
        // �X�e�[�g�����̂͌�납��
        Idle,
        Push,

    };

    // �N���A�X�e�[�g
    enum class StateClear
    {
        // �X�e�[�g�����̂͌�납��
        Idle,

    };

    // �Q�[���I�[�o�[�X�e�[�g
    enum class StateOver
    {
        // �X�e�[�g�����̂͌�납��
        Idle,
        Revive,

    };

    // �J�����X�e�[�g
    enum class CameraState
    {
        Normal,
        NotLockOn,
        LockOn,
        AttackLock
    };

    // �A�j��
    enum Animation
    {
        //Anim_Attack,
        //Anim_Death,
        //Anim_Falling,
        //Anim_GetHit1,
        //Anim_GetHit2,
        //Anim_Idle,
        //Anim_Jump,
        //Anim_Jump_Flip,
        //Anim_Landing,
        //Anim_Revive,
        //Anim_Running,
        //Anim_Walking

        //Anim_Tpose,
        //Anim_Idle,
        //Anim_Move,
        //Anim_Back,
        //Anim_Beside,
        //Anim_BesideSrow,
        //Anim_Jump,
        //Anim_Slash,
        //Anim_SlashBeside,
        //Anim_SlashThree,
        //Anim_Deth,
        //Anim_Pain,
        //Anim_Counter


        Anim_Tpose,
        Anim_Idle,
        Anim_Jump,
        Anim_Slash,
        Anim_SlashBeside,
        Anim_Deth,
        Anim_Pain,
        Anim_Counter,
        Anim_Running,
        Anim_SlashThree,
        Anim_Dush,
        Anim_Magic,
        Anim_MagicSeconde,
        Anim_SpecialAttack,
        Anim_Land,
        Anim_Roll

    };

    // �A�j���[�V�����Đ����@
    enum class UpAnim
    {
        Stop = -1,
        Normal,
        Doble,
        Blend,
        Reverseplayback,

    };


    enum class CommandAttack
    {
        Attack,
        Magic
    };

    enum class CommandMagic
    {
        Normal = -2,
        Fire = 0,
        Thander,
        Ice,
        Heale
    };

    enum class SpecialAttack
    {
        Normal = -1,
        Attack,
        MagicFire,
        MagicThander,
        MagicIce,
    };

private:
    // �\����
    struct SpecialMode
    {
        bool specialAttack;
        bool specialShot;
    };


public:

    bool GetAfterimageMove() const { return this->afterimagemove; }



    bool Ground();

    State GetStateTuren() const { return this->state; }

    void SetStateTuren(State state)  { this->state = state; }

    // �Đ����@
    void  SetUpdateAnim(UpAnim  updateanim) { this->updateanim = updateanim; }

    UpAnim  GetUpdateAnim() const  { return this->updateanim; }

    float GetMoveSpeedAnimation() const { return moveSpeedAnimation; }

    // UI�h�炵���
    void SetShakeMode(bool shakeMode) { this->shakeMode = shakeMode;   }

    // ��]�m�F
    void SetAngleCheck(bool angleCheck) { this->angleCheck = angleCheck; }

    //// ���ˌ�̓����蔻��G�ɔ��f
    //void SetCounterCheck(bool counterCheck) { this->counterCheck = counterCheck; }

    //// ���ˌ�̓����蔻��G�ɔ��f
    //bool GetCounterCheck() const { return counterCheck; }

    float GetGravity() const { return gravity; }

    void SetLockOnState(CameraState lockonState) { this->lockonState = lockonState; }

    CameraState GetLockOnState() const { return lockonState; }
    
    void Hit();

    // ���b�N�I�����͊m�F
    void SetRockCheck(bool rockCheck) { this->rockCheck = rockCheck; }
    bool GetRockCheck() { return this->rockCheck; }

    void SetFreeCameraCheck(bool freeCameraCheck) { this->freeCameraCheck = freeCameraCheck; }

    // �F
    void SetColorGradingData(ColorGradingData colorGradingData) { this->colorGradingData = colorGradingData; }

    // fashu
    void SetFlashOn(bool flashOn) { this->flashOn = flashOn; }
    bool GetFlashOn() { return flashOn; }

    void SetHitCheck(bool hitCheck) {  this->hitCheck = hitCheck; }
    bool GetHitCheck() {  return hitCheck; }

   // AudioSeSource* PlaySE() { return seSouce.get(); }

    void SetAttackNumberSave(int attackNumberSave) { this->attackNumberSave = attackNumberSave; }

    // UI���Ȃ��Ƃ���false
    void SetUiControlleCheck(bool uiControlleCheck) { this->uiControlleCheck = uiControlleCheck; }

    // �X�e�[�g�ϊ�
    void SetEndState(bool endState) { this->endState = endState; }
    bool GetEndState() { return endState; }

    // �K�E�Z���̃��b�N�I�t
    void SetSpecialRockOff(bool specialRockOff) { this->specialRockOff = specialRockOff; }

private:
    //std::shared_ptr<Movement>	movement;
    //std::shared_ptr<HP>	hp;
    //std::shared_ptr<Mp> mp;
    //std::shared_ptr<Transform> transform;
    std::weak_ptr<Movement>	movement;
    std::weak_ptr<HP> hp;
    std::weak_ptr<Mp> mp;
    std::weak_ptr<Transform> transform;

    
    DirectX::XMFLOAT3 velocity = { 0,0,0 };

    // �󒆍s������
    bool isAreAttack = false;

    // �󒆍s�����Ԋu
    float areAttackState = .0;
    float areAttackStateEnd = .0;
    float areAttackStateMax = 2.0f;

    // UI�������o�����`�F�b�N
    bool             uiControlleCheck = false;

    // ������
   // std::unique_ptr<AudioSeSource> seSouce;

    //std::unique_ptr<Model> model;
    Model* model = nullptr;

    GamePad        gamePad;

    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 angle = {};
    DirectX::XMFLOAT3 scale = {};

    // �g��
    float height = 1.0f;

    // �a���G�t�F�N�g�@�X�P�[��
    float slashScale = 0.4f;



    
    

    float          moveSpeed = 5.0f;

    float          turnSpeed = DirectX::XMConvertToRadians(720);

    float          turnSpeedAdd = 0.0f;

    // ��]���x�@�U����
    float          turnSpeedAttack = DirectX::XMConvertToRadians(2600);

    float          jumpSpeed = 13.0f;
    
    int                     jumpLimit = 2;

    ProjectileManager projectileManager;

    Effect* hitEffect = nullptr;
    Effect* magicFireHitEffect = nullptr;
    Effect* magicThenderHitEffect = nullptr;
    Effect* magicIceHitEffect = nullptr;
    Effect* ImpactEffect = nullptr;
    Effect* desEffect = nullptr;


    //////// ���֌W //////////
    
    // BGM
    //std::unique_ptr<AudioSource> Bgm;

    // SE
    //std::unique_ptr<AudioSeSource> SePlayer;

    //// �P�a��
    //std::unique_ptr<AudioSource> slashFustSe;
    //// �Q�a��
    //std::unique_ptr<AudioSource> slasSecondeSe;
    //// �Ō�
    //std::unique_ptr<AudioSource> buttonTherdeSe;

    //// ���s
    //std::unique_ptr<AudioSource> walkSe;

    //// �_�b�V��
    //std::unique_ptr<AudioSource> DushSe;

    //// �W�����v
    //std::unique_ptr<AudioSource> janpSe;

    //// ���n
    //std::unique_ptr<AudioSource> landSe;

    //// ��_���[�W
    //std::unique_ptr<AudioSource> damageSe;

    //// �����G
    //std::unique_ptr<AudioSource> confusionSe;

    //// ���S�����G
    //std::unique_ptr<AudioSource> deathSe;


    //// �q�b�g�X�g�b�v
    //std::unique_ptr<AudioSource> hitStop;

    //// �K�E�Z��
    //std::unique_ptr<AudioSource> flameSpecialStarteSe;
    //// �K�E�Z������
    //std::unique_ptr<AudioSource> flameSpecialSaveSe;

    //// �K�E�Z��
    //std::unique_ptr<AudioSource> lighteningStrikeSpecialSe;
    //// �K�E�Z������
    //std::unique_ptr<AudioSource> lighteningStrikeSpecialSaveSe;


    //// ������
    //std::unique_ptr<AudioSource> flameStarteSe;
    //// ������
    //std::unique_ptr<AudioSource> flameDurationSe;
    //// �����e��
    //std::unique_ptr<AudioSource> flameimpactSe;
    //

    //// �X����
    //std::unique_ptr<AudioSource> iceStarteSe;
    //// �X����
    //std::unique_ptr<AudioSource> iceDurationSe;
    //// �X���e��
    //std::unique_ptr<AudioSource> iceimpactSe;

    //// ���q�b�g��
    //std::unique_ptr<AudioSource> lightningSe;



    /////////////////////////



    std::unique_ptr<Effect> hitSlash;

    std::unique_ptr<Effect> lightningAttack;

    std::unique_ptr<Effect> hitFire;
    std::unique_ptr<Effect> hitThander;
    std::unique_ptr<Effect> hitIce;

    // �����Ƃɗ���
    std::unique_ptr<Effect> areWork;

    State                   state = State::Idle;
    State                   stated = State::Idle;
    
    CameraState            cameraState = CameraState::Normal;
    CameraState			lockonState = CameraState::NotLockOn;

    DirectX::XMFLOAT3 lockonCharactor = {0.0f,0.0f,0.0f};


    float				lockonTargetChangeTime = 0;
    float				lockonTargetChangeTimeMax = 8;

    // �d��
    float gravity =-0.5f;

    // ���n�ꏊ�܂ł̋����@ �\���ȑ��x�ŗ��Ƃ��d�͂̂T�{�Q�A�R�b��ɒ��n���[�V����������B
    float jumpfliptime = gravity * 5;

    float            leftHandRadius = 1.0f;

    bool             attackCollisionFlag = false;
  
    //int   health = 10;

    // �����Ă��锻��
    bool afterimagemove = false;

    bool blend = false;

    // �A�b�v�f�[�g�Đ����
    UpAnim  updateanim = UpAnim::Normal;

    DirectX::XMFLOAT3 moveVec = {0.0f,0.0f,0.0f};

    DirectX::XMFLOAT3 frontVec = { 0 ,0,0 };

    // �㔼�g�X�V�J�n�ʒu
    //char* bornUpStartPoint;

    //// �㔼�g�̉����܂ōX�V���邩
    //char* bornUpEndPoint = "";

    //// �����g�X�V�I���ʒu
    //char* bornDownerEndPoint = "";

    // �㔼�g�X�^�[�g�Đ��J�n�ꏊ
    char* bornUpStartPoint = "";

    // �㔼�g�G���h�Đ���~�ꏊ
    char* bornUpEndPoint = "";

    // �����g�X�^�[�g�Đ��J�n�ꏊ
    char* bornDownerStartPoint = "";

    // �����g�G���h�Đ���~�ꏊ
    char* bornDownerEndPoint = "";


    // �����蔻�蔼�a
    //float radius = 0.7f;
    float radius = 1.0f;

    // Hp
    int health = 50;

    // �ő�HP
    int maxHealth = 50;

    // Mp
    int magicPoint = 50;




    // �A�j���[�V�����̎��� 
    float currentANimationSeconds = 0.0f;




    // �e�ې�������
    //float   lifeTimer = 3.0f;

    /////////////////// �J�����֌W



    CameraController* cameraControlle = nullptr;
    //std::unique_ptr<CameraController> cameraControlle;

    DirectX::XMFLOAT3	cameraAngle = DirectX::XMFLOAT3(0, 0, 0);
    float				cameraRollSpeed = DirectX::XMConvertToRadians(90);
    float				cameraMaxPitch = DirectX::XMConvertToRadians(45);
    float				cameraMinPitch = DirectX::XMConvertToRadians(-45);
    float				cameraRange = 100.0f;
    float				characterHeight = 10.0f;



    // �U�����@�̑I��
    int selectCheck = 0;

    // ���@�I��
    int selectMagicCheck = 0;

    // ����U���o��܂�
    float specialAttackCharge = 0.0f;
    float specialAttackChargeMax = 1.5f;
    float specialShotCharge = 0.0f;

    // �U���q�b�g��
    int attackNumberSave;
    int attackNumberSaveMax = 4;

    // ����Z�̃`���[�W
    int attackEnergyCharge = 0;
    int fireEnergyCharge = 0;
    int ThanderEnergyCharge = 0;
    int iceEnergyCharge = 0;
    int energyChargeMax = 4;

    // ����Z�̃`���[�W����
    bool isSpecialChargeComplete = false;

    // ����U�������߂�z
    std::stack<int> chargedSpecialMoves;


    // �X�e�[�g�}�V���p
    //StateMachine* stateMachine = nullptr;
    std::unique_ptr<StateMachine> stateMachine;


    // �ړ��X��
    float moveSpeedAnimation = 0.0f;

    // �����蔻�薳������
    bool invalidJudgment = true;

    // ����U�������߂�z
    std::stack<int> specialAttack;
    //std::vector<int> specialAttack2;


    // ����A�N�V�����m�F
    bool specialAttackTime = false;

    // ���@�U����łV���[�g�J�b�g�p
    bool magicAction = false;

    // �h��
    bool shakeMode = false;

    // ��]�m�F
    bool angleCheck = false;

    // �p�x�m�F
    float dotfake = 0.0f;

    // ���b�N�I���`�F�b�N
    bool rockCheck = false;

    // �{�^�����������̃`�F�b�N
    bool buttonRock = false;

    // ���ˌ�̓����蔻��G�ɔ��f
    //bool counterCheck = false;

    // ���@�̎˒������鋗��
    float magicRangeLength = 470;

    // ���@�����
    int magicConsumption = 5;


    // �`��̐F���ǂ����邩
    DirectX::XMFLOAT4 mpUiColor = {1,1,1,1};


    // �_���[�W��
    int applyDamageNormal = 3;

    int applyDamageFire = 4;
    int applyDamageThander = 2;
    int applyDamageIce = 3;

    // �}�W�b�N�_���[�W
    int applyDamageMagic = 0;



    // �K�E�Z�_���[�W��
    int applyDamageSpecial = 2;

    int hitMortion = 0;
    int hitMortionMax = 180;

    // �f�o�b�O�p�K�E�Z�̃J�����A���O�����m�F
    bool SpecialCameraRock = false;


    // �f�o�b�O�p�J�����V�F�C�N�p
    float shakeTimer = 0.5f;
    float shakePower = 0.3f;


    // ���b�N�I���ȊO�̑_�����v���C���[���S���ǂ���
    bool freeCameraCheck = true;
    // �f�o�b�O
    bool angleCameraCheck = false;

    // ��ʂ̐F
    ColorGradingData       colorGradingData;
    ColorGradingData       colorGradingPostData;


    bool flashOn = false;

    bool hitCheck = false;

    static const int MAX_POLYGON = 32;
    DirectX::XMFLOAT3					trailPositions[2][MAX_POLYGON];

    int stateSize = 2;

    // uiCount�ő�l
    int uiCountMax = 3;

    // �I���^�C�~���O
    bool endState = false;

    //float uiTimeIntervalMax = 5.0f; 
    float uiShrinkValueMax = 250; 
    float uiShrinkValueMin = 100;

    // ���j���[�̃I���I�t
    bool isMenue = false;
    bool isMenueOn = true;
    bool isMenueOf = false;

    VignetteData					vignetteData;


    bool   checkVignette = false;

    bool testcolor = false;
    // �񕜒l
    int healing = 30;

    // �K�E�Z�����b�N�O��
    bool specialRockOff = false;

    // �^�[�Q�b�g�̈��ȏ㍂��
    float               topHeight = 3.0f;
    float               minHeight = 2.0f;
};

// �v���C���[�}�l�[�W���[
class PlayerManager
{
private:
    PlayerManager() {}
    ~PlayerManager() {};

public:
    // �C���X�^���X�擾
    static PlayerManager& Instance()
    {
        static PlayerManager instance;
        return instance;
    }

    // �X�V
    void DeleteUpdate();

    // �o�^
    void Register(std::shared_ptr<Actor> actor);


    // �X�e�[�W���擾
    int GetPlayerCount() const { return static_cast<int>(players.size()); }

    // �G�l�~�[�擾
    std::shared_ptr<Actor> GetPlayer(int index) { return players.at(index); }

    // �폜
    void Remove(std::shared_ptr<Actor> player);

    void Clear();
private:
    std::vector<std::shared_ptr<Actor>> players;
    // �폜�\��
    std::set<std::shared_ptr<Actor>>       removes;
};