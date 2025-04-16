#pragma once
#include <DirectXMath.h>
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Collision.h"
#include "ProjectileManager.h"
#include "Audio/AudioSource.h"
#include "Component.h"
#include "Collision.h"
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

    // ���O�擾
    const char* GetName() const override { return "Player"; }
    // �J�n����
    void Start() override;
    // �X�V����
    void Update(float elapsedTime) override;
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
    void CollisionMagicVsEnemies();
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
public:
    // �ȍĐ�
    void PlayPintchSe();
    void StopPintchSe();
    // �K�E�Z���ߏI��
    void PlaySpecialChargeCompleteSe();

    // �e���|�[�g��
    void PlayTellePortSe();

    // �X�e�B�b�N���͒l����ړ��x�N�g�����擾 �i�s�x�N�g�������i�ނׂ�����
    DirectX::XMFLOAT3 GetMoveVec(float elapsedTime) const;

    DirectX::XMFLOAT3 GetMagicMoveVec(float elapsedTime) const;

    // �ړ����͏���
    bool InputMove();

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

    // ���@�I��UI����
    void RemoveUIMagic();

    // ����U���I��
    bool InputSpecialAttackCharge();

    // ����Z���Ԍ��
    void InputSpecialAttackChange();

    // UI�̉��o
    void SpecialPlayUlEffect(float elapsedTime);

    bool GetSpecialAttackTime() const { return specialAttackTime; }
    void SetSpecialAttackTime(bool specialAttackTime) { this->specialAttackTime = specialAttackTime; }

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

    // �W�����v���͏���
    bool InputJump();

    // ���or�J�E���^�[����
    bool InputAvoidance();

    // �U������
    bool InputAttack();

    // ���@����
    bool InputMagick();

    // ���j���[�J������
    bool InputMenue();

    // �f�o�b�O�p����
    void DebugLength();

    // ���@����
    bool InputMagicframe();
    bool InputMagicIce();
    bool InputMagicLightning();
    bool InputMagicHealing();

    // ���@������
    void PushMagicFrame(DirectX::XMFLOAT3 angle);
    void PushMagicIce(DirectX::XMFLOAT3 angle);

    void InputSpecialMagicframe();

    void AttackCheckUI();

    void inFloat3(DirectX::XMFLOAT3 value, DirectX::XMFLOAT3& inValue);

    DirectX::XMFLOAT3 GetForwerd(DirectX::XMFLOAT3 angle);

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
    bool GetInvalidJudgment() const { return invalidJudgment; }
    // �Z�b�g�����蔻�薳��
    void SetInvalidJudgment(bool invalidJudgment) { this->invalidJudgment = invalidJudgment; }

    // �����̓����蔻��L��
    void DmageInvalidJudment(bool invalidJudgment);

    //  UI����
    void UiControlle(float elapsedTime);

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
        Anim_Roll,
        Anim_MagicArch
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
        Magic,
        Special
    };

    enum class CommandMagic
    {
        Normal = -2,
        Fire = 0,
        Thander,
        Ice,
        Heale
    };

    enum class SpecialAttackType
    {
        Normal = -1,
        Attack,
        MagicFire,
        MagicThander,
        MagicIce,
    };

private:
        //// �\����
        //struct SpecialMode
        //{
        //    bool specialAttack;
        //    bool specialShot;
        //};
public:
    bool GetAfterimageMove() const { return this->afterimagemove; }

    bool Ground();

    State GetStateTuren() const { return this->state; }

    void SetStateTuren(State state) { this->state = state; }

    // �Đ����@
    void  SetUpdateAnim(UpAnim  updateanim) { this->updateanim = updateanim; }

    UpAnim  GetUpdateAnim() const { return this->updateanim; }

    float GetMoveSpeedAnimation() const { return moveSpeedAnimation; }

    // UI�h�炵���
    void SetShakeMode(bool shakeMode) { this->shakeMode = shakeMode; }

    // ��]�m�F
    void SetAngleCheck(bool angleCheck) { this->angleCheck = angleCheck; }

    float GetGravity() const { return gravity; }

    void SetLockOnState(CameraState lockonState) { this->lockonState = lockonState; }

    CameraState GetLockOnState() const { return lockonState; }

    // ���b�N�I�����͊m�F
    void SetRockCheck(bool rockCheck) { this->rockCheck = rockCheck; }
    bool GetRockCheck() { return this->rockCheck; }

    void SetFreeCameraCheck(bool freeCameraCheck) { this->freeCameraCheck = freeCameraCheck; }

    // �F
    void SetColorGradingData(ColorGradingData colorGradingData) { this->colorGradingData = colorGradingData; }

    // fashu
    void SetFlashOn(bool flashOn) { this->flashOn = flashOn; }
    bool GetFlashOn() { return flashOn; }

    void SetHitCheck(bool hitCheck) { this->hitCheck = hitCheck; }
    bool GetHitCheck() { return hitCheck; }

    void SetAttackNumberSave(int attackNumberSave) { this->attackNumberSave = attackNumberSave; }

    // UI���Ȃ��Ƃ���false
    void SetUiControlleCheck(bool uiControlleCheck) { this->uiControlleCheck = uiControlleCheck; }

    // �X�e�[�g�ϊ�
    void SetEndState(bool endState) { this->endState = endState; }
    bool GetEndState() { return endState; }

    // �K�E�Z���̃��b�N�I�t
    void SetSpecialRockOff(bool specialRockOff) { this->specialRockOff = specialRockOff; }

    // �󒆍s�����m�F
    void SetAreAttackState(int areAttackState) { this->areAttackState = areAttackState; }

    void AreAttackDecreaseAmount();

    // �o�ߎ���
    bool UpdateElapsedTime(float timeMax, float elapsedTime);

    // �`�悷�邩�ǂ���
    void SetPlayeDrawCheck(int isPlayerDrawCheck) { this->isPlayerDrawCheck = isPlayerDrawCheck; }

private:
    // �\����
    struct SpecialAttack
    {
        int id;
        bool hasSkill;
    };

private:
    std::weak_ptr<Movement>	movement;
    std::weak_ptr<HP> hp;
    std::weak_ptr<Collision> collision;
    std::weak_ptr<Mp> mp;
    std::weak_ptr<Transform> transform;

    DirectX::XMFLOAT3 velocity = { 0,0,0 };

    // �󒆍s������
    bool isAreAttack = false;

    // �󒆍s�����Ԋu
    int areAttackState = 0;
    int areAttackStateEnd = 0;
    int areAttackStateMax = 1;

    // �󒆍s���Ԋu����
    float areAttackTime = 0.0f;
    float areAttackTimeEnd = 0.0f;
    float areAttackTimeMax = 1.0f;
    float areAttackTimeValue = 0.1f;

    // �o�ߎ��Ԃ𑪂�
    float timeElapsed = 0.0f;
    float timeElapsedMin = 0.0f;
    float timeElapsedHintMax = 1.0f;

    // UI�������o�����`�F�b�N
    bool             uiControlleCheck = false;

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
    std::unique_ptr<Effect> hitSlash;
    std::unique_ptr<Effect> lightningAttack;
    std::unique_ptr<Effect> hitFire;
    std::unique_ptr<Effect> hitThander;
    std::unique_ptr<Effect> hitIce;
    // �����Ƃɗ���
    std::unique_ptr<Effect> areWork;
    std::unique_ptr<Effect> effectFocus2D;


    State                   state = State::Idle;
    State                   stated = State::Idle;

    CameraState            cameraState = CameraState::Normal;
    CameraState			lockonState = CameraState::NotLockOn;

    DirectX::XMFLOAT3 lockonCharactor = { 0.0f,0.0f,0.0f };

    float				lockonTargetChangeTime = 0;
    float				lockonTargetChangeTimeMax = 8;

    // �ʒu���̃����_��
    DirectX::XMFLOAT3 randomPosMax = {1.5f,3.0f,1.5f};
    DirectX::XMFLOAT3 randomPosMin = {1.0f,0.1f,1.0f};

    // �d��
    float gravity = -0.5f;

    // ���n�ꏊ�܂ł̋����@ �\���ȑ��x�ŗ��Ƃ��d�͂̂T�{�Q�A�R�b��ɒ��n���[�V����������B
    float jumpfliptime = gravity * 5;

    float            leftHandRadius = 1.0f;

    bool             attackCollisionFlag = false;

    // �����Ă��锻��
    bool afterimagemove = false;

    bool blend = false;

    // �A�b�v�f�[�g�Đ����
    UpAnim  updateanim = UpAnim::Normal;

    DirectX::XMFLOAT3 moveVec = { 0.0f,0.0f,0.0f };

    DirectX::XMFLOAT3 frontVec = { 0 ,0,0 };

    // �㔼�g�X�^�[�g�Đ��J�n�ꏊ
    char* bornUpStartPoint = "";

    // �㔼�g�G���h�Đ���~�ꏊ
    char* bornUpEndPoint = "";

    // �����g�X�^�[�g�Đ��J�n�ꏊ
    char* bornDownerStartPoint = "";

    // �����g�G���h�Đ���~�ꏊ
    char* bornDownerEndPoint = "";

    // �����蔻�蔼�a
    float radius = 1.0f;

    // Hp
    int health = 50;

    // �ő�HP
    int maxHealth = 50;

    // Mp
    int magicPoint = 50;

    // �A�j���[�V�����̎��� 
    float currentANimationSeconds = 0.0f;

    CameraController* cameraControlle = nullptr;

    DirectX::XMFLOAT3	cameraAngle = DirectX::XMFLOAT3(0, 0, 0);
    float				cameraRollSpeed = DirectX::XMConvertToRadians(90);
    float				cameraMaxPitch = DirectX::XMConvertToRadians(45);
    float				cameraMinPitch = DirectX::XMConvertToRadians(-45);
    float				cameraRange = 100.0f;
    float				characterHeight = 10.0f;

    float               flame = 60.0f;
    float               isInputEmpty = 0.0f;

    bool                isDrawUi = true;
    bool                isDrawUiEmpth = false;

    DirectX::XMFLOAT2   scereenPositionOffset = { 34.0f,25.0f };

    float               slashAttackInvincibleTime = 0.5f;
    float               magicAttackInvincibleTime = 0.1f;

    // �U�����@�̑I��
    int selectCheck = 0;

    // ���@�I��
    int selectMagicCheck = 0;

    // ����U���o��܂�
    float specialAttackCharge = 0.0f;
    float specialAttackChargeSlashValue = 0.1f;
    float specialAttackChargeMagicValue = 0.2f;
    float specialAttackChargeMin = 0.0f;
    float specialAttackChargeMax = 1.5f;
    float specialShotCharge = 0.0f;

    // �U���q�b�g��
    int attackNumberSave;
    int attackNumberSaveMax = 4;

    // ����Z�̃`���[�W
    int attackEnergyCharge = 0;
    int attackEnergyChargeMin = 0;
    int fireEnergyCharge = 0;
    int fireEnergyChargeMin = 0;
    int ThanderEnergyCharge = 0;
    int iceEnergyCharge = 0;
    int energyChargeMax = 4;

    // ����Z�̃`���[�W����
    bool isSpecialChargeComplete = false;

    // ����U�������߂�z
    std::stack<int> chargedSpecialMoves;

    // se�̉��̑傫��
    float seVolume = 0.5f;

    // �X�e�[�g�}�V���p
    std::unique_ptr<StateMachine> stateMachine;

    // �ړ��X��
    float moveSpeedAnimation = 0.0f;

    // �ړ����@��
    float magicMoveLength = 1000.0f;

    // �����蔻�薳������
    bool invalidJudgment = true;

    // ����U��
    static const int specialAttackNumMax = 4;

    int specialAttackNum = 0;

    // ����U�������߂�z
    //std::stack<int> specialAttack;
    //std::vector<int> specialAttack;
    //bool specialAttack[specialAttackNumMax];
    std::vector<SpecialAttack> specialAttack;

    bool isSkillHave = false;

    int spCmdMoveLimitMin = 0;
    int spCmdMoveLimitMax = 1;

    // ����U���̃T�C�Y���Œ�l�Ȃ��߂�B
    int specialAttackNumMin = 0;

    // ����A�N�V�����m�F
    bool specialAttackTime = false;

    // ���@�U����łV���[�g�J�b�g�p
    bool magicAction = false;

    // ����Z�R�}���h�J��
    bool specialAction = false;

    // ����Z�a�������܂�����
    bool isSpecialAttack = false;

    // ����Z�������܂�����
    bool isSpecialMagicFire = false;

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

    // ���@�̎˒������鋗��
    float magicRangeLength = 470;

    // ���@�����
    int magicConsumption = 5;

    // �`��̐F���ǂ����邩
    DirectX::XMFLOAT4 mpUiColor = {1,1,1,1};

    // �X�y�L�����[����
    int isSpecular = 0;
    // �e�I��
    int isRimRightning = 1;
    // model�I���I�t
    int StencilRef = 1;

    // �_���[�W��
    int applyDamageNormal = 3;
    int applyDamageFire = 3;
    int applyDamageThander = 2;
    int applyDamageIce = 4;
    // �}�W�b�N�_���[�W
    int applyDamageMagic = 0;
    // �K�E�Z�_���[�W��
    int applyDamageSpecial = 2;

    int hitMortion = 0;
    int hitMortionMax = 180;

    // �f�o�b�O�p�K�E�Z�̃J�����A���O�����m�F
    bool specialCameraRock = false;

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

    int stateSize = 5;

    // uiCount�ő�l
    int uiCountMax = 5;

    // �I���^�C�~���O
    bool endState = false;

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

    // shortecut pos
    DirectX::XMFLOAT2 fireShorteCutPos = { 179, 200 };
    DirectX::XMFLOAT2 thunderShorteCutPos = { 179, 285 };
    DirectX::XMFLOAT2 iceShorteCutPos = { 179, 370 };
    DirectX::XMFLOAT2 healeShorteCutPos = { 200, 525 };

    // �X�e�[�g�G�l�~�[���b�N�I���p
    int stateEnemyIndex = 0;

    // �q���g�`�悷�邩�ǂ���
    bool hintDrawCheck = false;

    // �s���`���ǂ���
    bool isPintch = false;

    // �`�悷�邩�ǂ���
    int isPlayerDrawCheck = 1;
    int skipDraw = 0;

    // Se�֌W�̒l
    bool isLoopSe = true;
    bool isLoopDisabled = false;


    DirectX::XMFLOAT4 vignettePinchColor = { 1,0,0,1 };
    float vignettePinchSmoothness = 3.0f;
    float vignettePinchIntensity = 0.8f;


    DirectX::XMFLOAT4 vignetteNormalColor = { 0,0,0,1 };
    float vignetteNormalSmoothness = 0.0f;
    float vignetteNormalIntensity = 0.0f;


    // �X�A�ˈꎞ��~
    bool iceMagicMoveCheck = false;

    // �f�o�b�O
    float debugLength;

    // ���������邩
    bool ButtonDownCountinue = true;

    // �f�o�b�O
    int debugInt = 0;
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