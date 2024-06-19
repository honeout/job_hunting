#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

// �X���C��
class EnemySlime : public Enemy
{
public:
    EnemySlime();
    ~EnemySlime() override;

    //void Start() override;

    // �X�V����
    void Update(float elapsedTime) override;

    // �`�揈��
    void Render(const RenderContext& rc, ModelShader* shader) override;

    // �f�o�b�O�v���~�e�B�u�`��@�f�o�b�O�p
    void DrawDebugPrimitive();

    // �꒣��ݒ�
    void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

private:
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

private:
    // �X�e�[�g
    enum class State
    {
        Wander,
        Idle,
        Pursuit,
        Attack,
        IdleBattle,
        Damage,
        Death
    };

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

    //DirectX::XMFLOAT3 position = {};
    //DirectX::XMFLOAT3 angle = {};

    //std::shared_ptr<Movement>	movement;
    //std::shared_ptr<HP>	hp;

    // ���x
    /*float moveSpeed = 5;*/


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


    //// Hp
    //int health = 5;

    //// �ő�HP
    //int maxHealth = 5;


    //int radius = 5;

};