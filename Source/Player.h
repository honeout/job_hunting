#pragma once

#include <DirectXMath.h>
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
//#include "Character.h"
//#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileManager.h"
#include "Component.h"
#include "Movement.h"
#include "HP.h"
#include "Input\GamePad.h"
#include "CameraController.h"
#include "Effect.h"
#include "ModelControll.h"
#include "Transform.h"
#include "StateMachine.h"

enum class UpAnim
{
    Normal,
    Doble,
    Blend,

};

// �v���C���[
class Player : public Component
{
public:
    Player() {};
    ~Player() override;



    //// �C���X�^���X�擾
    //static Player& Instance();


    // �A�j���[�V����
    enum Animation
    {
        Anim_Attack,
        Anim_Death,
        Anim_Falling,
        Anim_GetHit1,
        Anim_GetHit2,
        Anim_Idle,
        Anim_Jump,
        Anim_Jump_Flip,
        Anim_Landing,
        Anim_Revive,
        Anim_Running,
        Anim_Walking
    };

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
    // GUI�`��
    void OnGUI() override;

    // �`�揈��
    void Render(RenderContext& rc, ModelShader& shader) override;

    // �V���h�E�}�b�v����
    void RenderShadowmap(RenderContext& rc) override;


    // �e�ۂƓG�̏Փˏ���
    void CollisionProjectilesVsEnemies();

    // �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
    void CollisionPlayerVsEnemies();

    // �f�o�b�O�pGUI�`��
    void DrawDebugGUI();

    // �j��
    void Destroy();

protected:
    //// ���n�������ɌĂ΂��
    //void OnLanding() override;

    //// ���S�������ɌĂ΂��
    //void OnDead() override;

    //// �_���[�W���󂯂����ɌĂ΂��
    //void OnDamaged() override;

public:

    // �J��������
    void CameraControl(float elapsedTime);

    // �X�e�B�b�N���͒l����ړ��x�N�g�����擾 �i�s�x�N�g�������i�ނׂ�����
    DirectX::XMFLOAT3 GetMoveVec(float elapsedTime) const;

    //// �L�����N�^�[����
    void CharacterControl(float elapsedTime);


    // �ړ����͏���
    bool InputMove(float elapsedTime);

    // �U�����@�I��
    bool InputSelectCheck();

    // ����U���I��
    bool InputSpecialAttackCharge();
    bool InputSpecialShotCharge();
 


    // �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
    //void CollisionPlayerVsEnemies();

    // �m�[�h�ƃG�l�~�[�̏Փˏ���
    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

    // �m�[�h�ƃG�l�~�[�̏Փˏ���
    void CollisionNodeVsEnemiesCounter(const char* nodeName, float nodeRadius);


    // �W�����v���͏���
    bool InputJump();

    // �e�ۓ���
    bool InputProjectile();

    // ������
    bool InputAvoidance();

    // �U������
    bool InputAttack();

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

   // void Update();

    void inFloat3(DirectX::XMFLOAT3 value, DirectX::XMFLOAT3& inValue);

    DirectX::XMFLOAT3 GetForwerd(DirectX::XMFLOAT3 angle);

    // �X�e�[�g�}�V�[���擾
    StateMachine* GetStateMachine() { return stateMachine; }

    // �U�����@�ύX
    void SetSelectCheck(bool selectCheck) { this->selectCheck = selectCheck; }
    // �U�����@�ύX
    bool GetSelectCheck() const { return selectCheck; }

    // ���ꖂ�@��������
    void SetSpecialAttackCharge(float specialAttackCharge) { this->specialAttackCharge = specialAttackCharge; }

    // ����A�N�V������������
    float GetSpecialAttackCharge() const { return specialAttackCharge; }

    // �W�����v�̍ő�l
    int GetJumpCount() { return jumpCount; }

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

public:
    // �X�e�[�g
    enum class State
    {
        // �X�e�[�g�����̂͌�납��
        Idle,
        Move,
        Jump,
        Land,
        JumpFlip,
        Attack,
        Damage,
        Death,
        Revive,
        Avoidance,
        Reflection,
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

   // Model* GetModel() const { return model; }

    bool Ground();

    State GetStateTuren() const { return this->state; }

    void SetStateTuren(State state)  { this->state = state; }

    // �Đ����@
    void  SetUpdateAnim(UpAnim  updateanim) { this->updateanim = updateanim; }

    UpAnim  GetUpdateAnim() const  { return this->updateanim; }

    float GetMoveSpeedAnimation() const { return moveSpeedAnimation; }

private:
    std::shared_ptr<Movement>	movement;
    std::shared_ptr<HP>	hp;
    std::shared_ptr<Transform> transform;

    
    DirectX::XMFLOAT3 velocity = { 0,0,0 };
 
    Model* model = nullptr;

    GamePad        gamePad;

    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 angle = {};
    DirectX::XMFLOAT3 scale = {};

    // �g��
    float height = 1.0f;
    
    

    float          moveSpeed = 5.0f;

    float         turnSpeed = DirectX::XMConvertToRadians(720);

    float          jumpSpeed = 20.0f;
    int                     jumpCount = 0;
    int                     jumpLimit = 2;

    ProjectileManager projectileManager;

    Effect* hitEffect = nullptr;
    Effect* desEffect = nullptr;

    State                   state = State::Idle;
    State                   stated;
    
    float gravity =-1.0f;

    // ���n�ꏊ�܂ł̋����@ �\���ȑ��x�ŗ��Ƃ��d�͂̂T�{�Q�A�R�b��ɒ��n���[�V����������B
    float jumpfliptime = gravity * 5;

    float            leftHandRadius = 0.4f;

    bool             attackCollisionFlag = false;
  
    //int   health = 10;

    // �����Ă��锻��
    bool afterimagemove = false;

    bool blend = false;

    // �A�b�v�f�[�g�Đ��㔼�g�����g��
    UpAnim  updateanim ;

    DirectX::XMFLOAT3 moveVec;

    DirectX::XMFLOAT3 frontVec = { 0 ,0,0 };

    // �㔼�g�X�V�J�n�ʒu
    char* bornUpStartPoint;

    // �㔼�g�̉����܂ōX�V���邩
    char* bornUpEndPoint = "";

    // �����g�X�V�I���ʒu
    char* bornDownerEndPoint = "";

    // �����蔻�蔼�a
    float radius = 0.5f;

    // Hp
    int health = 5;

    // �ő�HP
    int maxHealth = 5;



    // �A�j���[�V�����̎��� 
    float currentANimationSeconds = 0.0f;


    // �e�ې�������
    float   lifeTimer = 3.0f;

    /////////////////// �J�����֌W

    CameraController* cameraControlle;

    DirectX::XMFLOAT3	cameraAngle = DirectX::XMFLOAT3(0, 0, 0);
    float				cameraRollSpeed = DirectX::XMConvertToRadians(90);
    float				cameraMaxPitch = DirectX::XMConvertToRadians(45);
    float				cameraMinPitch = DirectX::XMConvertToRadians(-45);
    float				cameraRange = 100.0f;
    float				characterHeight = 10.0f;

    // �U�����@�̑I��
    int selectCheck = false;

    // ����U���o��܂�
    float specialAttackCharge = 0.0f;
    float specialShotCharge = 0.0f;

    // �X�e�[�g�}�V���p
    StateMachine* stateMachine = nullptr;

    // �ړ��X��
    float moveSpeedAnimation;

    // �����蔻�薳������
    bool invalidJudgment = true;
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