#pragma once

#include "Graphics/Model.h"
#include "Component.h"
#include "Audio/AudioSource.h"
#include "Movement.h"
#include "HP.h"
#include "ModelControll.h"
#include "Transform.h"

#include "StateMachine.h"
#include "StageMain.h"
#include "Effect.h"
// �X�e�[�g




// ��΃{�X
class EnemyBoss : public Component
{
public:
    EnemyBoss() {}
    ~EnemyBoss() override;

    // ���O�擾
    const char* GetName() const override { return "EnemyBoss"; }

    void Start() override;

    //// �X�V����
    void Update(float elapsedTime) override;

    // �`�揈��
    void Render(RenderContext& rc, ModelShader& shader) override;

#ifdef _DEBUG
    // GUI�`��
    void OnGUI() override;
#endif // _DEBUG

    // �f�o�b�O�v���~�e�B�u�`��@�f�o�b�O�p
    void DrawDebugPrimitive();


    // SE�Đ� �؂��
    void InputSlashSe();
    void InputStopSlashSe();

    // SE�Đ� �W�����v
    void InputJampSe();
    void InputStopJampSe();


    // SE�Đ� �_�b�V��
    void InputDashSe();
    // SE�Đ� �_�b�V��
    void InputStopDashSe();


    // SE���ѐ�
    void InputAwakeSe();

    // SE�Ռ��g
    void InputImpactSe();

    // SE�_���[�W��
    void InputDamageSe();

    // SE�`���[�W�I��
    void InputChargeSe();

    // �����蔻��Ռ��g
    void CollisionImpactVsPlayer();

    // �����蔻�胋�r�[
    void CollisionRubyVsPlayer();

    // �����蔻�胋�r�[��
    void CollisionRubyWidthVsPlayer();

    // �����蔻���΂ƒn��
    void CollisionRubyWidthVsOnGraound();

    // �����蔻��{�X�ƃv���C���[
    bool CollisionPlayerWithRushEnemy();

    // �Ռ��g����
    void CollisionInpact();

    // �G�o���Ǘ�
    void ManageAwakeTime(float elapsedTime);

    // �G�o�����ԏ�����
    void ResetAwakeTime();

    // �����蔻��E�[
    void DetectHitByBodyPart(DirectX::XMFLOAT3 partBodyPosition, int applyDamage);

    void InputImpact(DirectX::XMFLOAT3 pos);

    // ��Δ�яo��
    void InputProjectile();

    // ��Όʂ̂��Ŕ��
    void InputThrowingRuby(DirectX::XMFLOAT3 target);
    
    // �꒣��ݒ�
    void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

    // �^�[�Q�b�g�|�W�V�����ݒ�
    void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }

    // �^�[�Q�b�g�|�W�V�����擾
    DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }


    // �X�e�[�g�}�V�[���擾
    //StateMachine* GetStateMachine() { return stateMachine; }
    StateMachine*GetStateMachine() { return stateMachine.get(); }


    // �O���ŏ��������邽�߂̏��u
    void StateMachineCreate() { stateMachine = std::make_unique<StateMachine>(); }



public:
    enum class ClearState
    {
        Death,
    };

    enum class State
    {
        Wander,
        Idle,
        Pursuit,
        Jump,
        Attack,
        Shot,
        ShotThrowing,
        IdleBattle,
        Damage,
        Death,
        AwakeStart,
    };



    // �A�j���[�V����
    enum Animation
    {

        Anim_Walk,
        Anim_Standby,
        Anim_jewelattack,
        Anim_Attack,
        Anim_CloseEye,
        Anim_Shot,
        Anim_Die,
        Anim_Movie
    };

    // �A�j���[�V�����Đ��̎��
    enum class UpAnim
    {
        Stop = -1,
        Normal,
        Doble,
        Blend,
        Reverseplayback,

    };

    enum class AttackMode
    {
        AssaultAttack,
        JumpStompAttack,
        DushAttack,
    };

    enum class ExitRoot
    {
        ExitJamp,
        Confusion
    };
public:
    // �X�e�[�g�^�C�}�[�ݒ�
    void SetStateTimer(float timer) {
        stateTimer = timer;
    }
    // �X�e�[�g�^�C�}�[�擾
    float GetStateTimer() { return stateTimer; }

    // �Q�b�g�����蔻�薳��
    bool GetInvalidJudgment() const { return invalidJudgment; }
    // �Z�b�g�����蔻�薳��
    void SetInvalidJudgment(bool Judgment) { invalidJudgment = Judgment; }

    // �Q�b�g�J�E���^�[
    bool GetCounterJudgment() const { return counterJudgment; }
    // �Z�b�g�J�E���^�[
    void SetCounterJudgment(bool counterJudgment) { this->counterJudgment = counterJudgment; }

    //  UI����
    void UiControlle(float elapsedTime);

    // HP���ێ��_��
    void OnHit(float elapsedTime);

//private:
    // �^�[�Q�b�g�ʒu�������_���ݒ�
    void SetRandomTargetPosition();

    // �ڕW�n�_�ֈړ�
    void MoveToTarget(float elapsedTime, float speedRate);
    // �ړI�����ւ̉�]
    void TurnToTarget(float elapsedTime, float speedRate);

    // �W�����v�ݒ�
    void InputJump();

    // �p�j�X�e�[�g�֑J��
    void TransitionWanderState();

    // �p�j�X�e�[�g�X�V����
    void UpdateWanderState(float elapsedTime);

    // �m�[�h�ƃv���C���[�̏Փˏ���
    void CollisitionNodeVsPlayer(const char* nodeName, float nodeRadius);

    // �v���C���[���G
    bool SearchPlayer();

    // �ҋ@�X�e�[�g�֑J��
    void TransitionIdleState();

    // �ҋ@�X�e�[�g�X�V����
    void UpdateIdleState(float elapsedTime);

    // �ǐՃX�e�[�g�֑J��
    void TransitionPursuitState();

    // �ǐՃX�e�[�g�X�V����
    void UpdatePursuitState(float elapsedTime);

    // �U���X�e�[�g�֑J��
    void TransitionAttackState();

    // �U���X�e�[�g�X�V����
    void UpdateAttackState(float elapsedTime);

    // �퓬�ҋ@�X�e�[�g�֑J��
    void TransitionIdleBattleState();

    // �퓬�ҋ@�X�e�[�g�X�V����
    void UpdateIdleBattleState(float elapsedTime);

    // �_���[�W�X�e�[�g�֑J��
    void TransitionDamageState();

    // �_���[�W�X�e�[�g�֍X�V����
    void UpdateDamageState(float elapsedTime);

    // ���S�X�e�[�g�֑J��
    void TransitionDeathState();

    // ���S�X�e�[�g�X�V����
    void UpdateDeathState(float elapsedTime);

    // �j��
    void Destroy();

    // �|�W�V�����擾
    DirectX::XMFLOAT3 GetPosition() { return position; }

    // ���a
    float GetRadius() { return radius; }

    // �ߋ����U�����̓����蔻��
    void SetAttackRightFootRange(float attackRightFootRange) { this->attackRightFootRange = attackRightFootRange; }

    // �ߋ����U�����̓����蔻��
    float GetAttackRightFootRange() const { return attackRightFootRange; }

    // �Đ����@�Z�b�g
    void  SetUpdateAnim(UpAnim  updateanim) { this->updateanim = updateanim; }
    // �Đ����@�Q�b�g
    UpAnim  GetUpdateAnim() const { return this->updateanim; }

    // Todo���݂��������̂Ŏg���Ă��Ȃ��������ăv���C���[�����˕Ԃ���U���̈ꎞ�I�n�ʔ������
    // �n�ʔ��蕜��
    void SetPushuThrow(bool pushuThrow) { this->pushuThrow = pushuThrow; }
    // �n�ʔ��蕜��
    bool GetPushuThrow() const { return pushuThrow; }

    void SetModelDrawCheck(bool modelDrawCheck) {this->modelDrawCheck = modelDrawCheck;}
    
    // �N���A�m�F
    bool GetClearCheck() { return clearCheck; }
    void SetClearCheck(bool clearCheck) { this->clearCheck = clearCheck; }

    void SetMoveCheck(bool moveCheck) 
    { this->moveCheck = moveCheck; }

    // �T���͈�
    float GetSearchRange() { return searchRange; }

    // �u���[��������
    bool GetBlurCheck() { return blurCheck; }

    void SetBlurCheck(bool blurCheck) {  this->blurCheck = blurCheck; }

    // �o���L��
    bool GetIsEnemyAwakened() { return isEnemyAwakened; }

    // �V�F�C�N���� �p���[
    void StartDamageShake();

private:
    // ���f�������m��
    Model* model = nullptr;
    
    
    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 angle = {};
    DirectX::XMFLOAT3 scale = {};

    std::weak_ptr<Movement>	movement;
    std::weak_ptr<HP>	hp;
    std::weak_ptr<Transform>	transform;





    // �o���L��
    bool isEnemyAwakened = false;
    // �o������
    float enemyAwakeningDuration = 0.0f;
    float enemyAwakeningDurationMax = 30;
    float enemyAwakeningDurationEnd = 0.0f;
    

    // �v���C���[�ɗ^�_���G�t�F
    std::unique_ptr<Effect> moveAttackEffect;

    std::unique_ptr<Effect> inpactEffect;

    std::unique_ptr<Effect> awakeEffect;

    //// �Ռ��gSE
    //std::unique_ptr<AudioSource> impactSe;
    //std::unique_ptr<AudioSource> moveAttackSe;

    // �ǂ̃X�e�[�g�œ�����
    State state = State::Wander;
    // �^�[�Q�b�g�̈ʒu
    DirectX::XMFLOAT3 targetPosition = { 0,0,0 };
    // �꒣��
    DirectX::XMFLOAT3 territoryOrigin = { 0,0,0 };

    // �A�b�v�f�[�g�Đ��㔼�g�����g��
    UpAnim  updateanim = UpAnim::Normal;
    
    //// �㔼�g�X�V�J�n�ʒu
    //char* bornUpStartPoint;

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


    // �꒣�蔼�a
    float territoryRange = 10.0f;
    // �������x
    float moveSpeed = 3.0f;
    // ��]���x
    float turnSpeed = DirectX::XMConvertToRadians(360);

    // �W�����v�̒l
    float jumpSpeed = 0;

    // �W�����v�̃v���X��
    float jumpSpeedMin = 20.0f;

    // �ǐՎ���
    float stateTimer = 0.0f;

    
    // �T�����a
    float searchRange = 5.0f;
    // �U�����a
    float attackRange = 5.5f;


    // Hp
    int health = 50;

    // �ő�HP
    int maxHealth = 50;

    // ���a
    float radius = 1.0f;

    // ����
    float height = 9.0f;

    float territoryarea = 10.0f;

    // �X�e�[�g�}�V���p
    std::unique_ptr <StateMachine> stateMachine;
    //StateMachine* stateMachine;

    // �����蔻�薳������
    bool invalidJudgment = true;

    // player�J�E���^�[�p
    bool counterJudgment = false;

    // �ߋ����U�����̓����蔻��
    float attackRightFootRange = 0;

    // �Ռ��g�N����͈͊O��
    float radiusInpactOutSide = 0.3f;

    // �Ռ��g�N����͈͓���
    float radiusInpactInSide = 0.3f;

    // �Ռ��g����
    float radiusInpactHeight = 0.0f;

    // �Ռ��g�L��
    bool IsInpact = false;

    bool pushuThrow = false;

    // model�`��`�F�b�N
    bool modelDrawCheck = true;



    // �_���[�W�o�ߎ���
    int damageStateTime = 0;
    int damageStateTimeMax = 15;
    bool damageStateCheck = false;

    // �N���A�m�F
    bool clearCheck = false;

    DirectX::XMFLOAT2 colorGB = { 1,1 };


    // ����`�F�b�N
    bool moveCheck = true;

    // �u���[������
    bool blurCheck = false;

    // uiCount�ő�l
    int uiCountMax = 3;


    // �_���[�W
    int applyDamageSlash = 5;
    int applyDamageJamp = 4;
    int applyDamageStamp = 6;
    int applyDamageImpact = 5;
    int applyDamageDush = 7;


    // �V�F�C�N�_���[�W���o
    float shakePower = 0.6f;
    float shakeTimer = 0.2f;

    // �V�F�C�N�p
    RadialBlurData damageDistortion;
};

// �G�l�~�[�}�l�[�W���[
class EnemyManager
{
private:
    EnemyManager() {}
    ~EnemyManager() {};

public:
    // �C���X�^���X�擾
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    // �X�V����
    void DeleteUpdate(float elapsedTime);

    // �o�^
    void Register(std::shared_ptr<Actor> actor);

    void Clear();

    // �X�e�[�W���擾
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

    // �G�l�~�[�擾
    std::shared_ptr<Actor> GetEnemy(int index) { return enemies.at(index); }

    void Remove(std::shared_ptr<Actor> projectile);



private:
    std::vector<std::shared_ptr<Actor>> enemies;

    // �폜�\��
    std::set<std::shared_ptr<Actor>>       removes;


};