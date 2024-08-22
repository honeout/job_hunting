#pragma once

#include "Graphics/Model.h"
//#include "Enemy.h"
#include "Component.h"
#include "Movement.h"
#include "HP.h"
#include "ModelControll.h"
#include "Transform.h"

#include "StateMachine.h"
// �X�e�[�g




// �X���C��
class EnemySlime : public Component
{
public:
    EnemySlime() {};
    ~EnemySlime() override;

    // ���O�擾
    const char* GetName() const override { return "EnemySlime"; }

    void Start() override;

    //// �X�V����
    void Update(float elapsedTime) override;

    // �`�揈��
    void Render(RenderContext& rc, ModelShader& shader) override;

    // �V���h�E�}�b�v����
    void RenderShadowmap(RenderContext& rc) override;

    //// �`�揈��
    //void Render(const RenderContext& rc, ModelShader* shader) override;

    // �f�o�b�O�v���~�e�B�u�`��@�f�o�b�O�p
    void DrawDebugPrimitive();

    // �꒣��ݒ�
    void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

    // �^�[�Q�b�g�|�W�V�����ݒ�
    void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }

    // �^�[�Q�b�g�|�W�V�����擾
    DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }


    // �X�e�[�g�}�V�[���擾
    StateMachine* GetStateMachine() { return stateMachine; }


    // �A�j���[�V�����@�Q�b�g
    int GetAnimationStateNormal() { return Animation::Anim_IdleNormal; }
    int GetAnimationStateAttack() { return Animation::Anim_Attack1; }
    int GetAnimationStateWalk() { return Animation::Anim_WalkFWD; }
    int GetAnimationStateHit() { return Animation::Anim_GetHit; }
    int GetAnimationStateDie() { return Animation::Anim_Die; }

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

//private:
    // �^�[�Q�b�g�ʒu�������_���ݒ�
    void SetRandomTargetPosition();

    // �ڕW�n�_�ֈړ�
    void MoveToTarget(float elapsedTime, float speedRate);

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

    // �^�[�Q�b�g�|�W�V�����擾
    //DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }

    // �|�W�V�����擾
    DirectX::XMFLOAT3 GetPosition() { return position; }

    // ���a
    float GetRadius() { return radius; }

public:
        enum class State
        {
            Wander,
            Idle,
            Pursuit,
            Attack,
            Damage,
            Death,
            IdleBattle,
        };

private:

    // �A�j���[�V����
    enum Animation
    {
        Anim_IdleNormal,       
        Anim_IdleBattle,       
        Anim_Attack1,
        Anim_Attack2,
        Anim_WalkFWD,
        Anim_WalkBWD,
        Anim_WalkLeft,
        Anim_WalkRight,
        Anim_RunFWD,
        Anim_SceneSomthinST,
        Anim_SceneSomthinPRT,
        Anim_Taunt,
        Anim_Victory,
        Anim_GetHit,
        Anim_Dizzy,
        Anim_Die,
    };




protected:
    //// ���S�������ɌĂ΂��
    //void OnDead() override;

    //// �_���[�W���󂯂����ɌĂ΂��
    //void OnDamaged() override;

private:
    // ���f�������m��
    Model* model = nullptr;

    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 angle = {};
    DirectX::XMFLOAT3 scale = {};

    std::shared_ptr<Movement>	movement;
    std::shared_ptr<HP>	hp;
    std::shared_ptr<Transform>	transform;

    //std::unique_ptr<Model> model;
    // Model* model;

    // ���x
    //float moveSpeed = 5;


    // �ǂ̃X�e�[�g�œ�����
    State state = State::Wander;
    // �^�[�Q�b�g�̈ʒu
    DirectX::XMFLOAT3 targetPosition = { 0,0,0 };
    // �꒣��
    DirectX::XMFLOAT3 territoryOrigin = { 0,0,0 };
    // �꒣�蔼�a
    float territoryRange = 10.0f;
    // �������x
    float moveSpeed = 3.0f;
    // ��]���x
    float turnSpeed = DirectX::XMConvertToRadians(360);
    // �ǐՎ���
    float stateTimer = 0.0f;

    
    // �T�����a
    float searchRange = 5.0f;
    // �U�����a
    float attackRange = 1.5f;


    // Hp
    int health = 50;

    // �ő�HP
    int maxHealth = 50;

    // ���a
    float radius = 0.5f;

    // ����
    float height = 1.0f;

    float territoryarea = 10.0f;

    // �X�e�[�g�}�V���p
    StateMachine* stateMachine = nullptr ;

    // �����蔻�薳������
    bool invalidJudgment = true;

    // player�J�E���^�[�p
    bool counterJudgment = false;

    // �X�e�[�g�؂�ւ����ԊǗ�
   // float				stateTimer = 0.0f;

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

    // �`��
    //void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

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