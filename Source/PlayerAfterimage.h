#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Player.h"
#include "AfterimageManager.h"
//#include "Collision.h"
//#include "ProjectileManager.h"


// �v���C���[
class PlayerAfterimage : public Afterimage
{
public:
    PlayerAfterimage();
    ~PlayerAfterimage() override;

    //// �C���X�^���X�擾
    //static PlayerAfterimage& Instance();


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




    // �X�V����
    void Update(float elapsedTime) override;

    // �f�o�b�O�v���~�e�B�u�`��@�f�o�b�O�p
    void DrawDebugPrimitive() override;


    // �`�揈��
    void Render(RenderContext& rc, ModelShader* shade) override;


    // �e�ۂƓG�̏Փˏ���
    void CollisionProjectilesVsEnemies();

    //// �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
    //void CollisionPlayerVsEnemies();

    // �f�o�b�O�pGUI�`��
    void DrawDebugGUI();
protected:
    // ���n�������ɌĂ΂��
    void OnLanding() override;

    // ���S�������ɌĂ΂��
    void OnDead() override;

    // �_���[�W���󂯂����ɌĂ΂��
    void OnDamaged() override;

private:
    // �X�e�B�b�N���͒l����ړ��x�N�g�����擾 �i�s�x�N�g�������i�ނׂ�����
    DirectX::XMFLOAT3 GetMoveVec() const;


    // �ړ����͏���
    bool InputMove(float elapsedTime);



    // �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
    void CollisionPlayerVsEnemies();

    // �m�[�h�ƃG�l�~�[�̏Փˏ���
    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

    // �W�����v���͏���
    bool InputJump();

    // �e�ۓ���
    bool InputProjectile();

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
public:

    Model* GetModel() const { return model; }


private:
    // �X�e�[�g
    enum class State
    {
        Idle,
        Move,
        Jump,
        Land,
        JumpFlip,
        Attack,
        Damage,
        Death,
        Revive
    };
    public:
        void SetModel(Model* model) { this->model = model; }



private:
    Model* model = nullptr;
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

    // ���n�ꏊ�܂ł̋����@ �\���ȑ��x�ŗ��Ƃ��d�͂̂T�{�Q�A�R�b��ɒ��n���[�V����������B
    int jumpfliptime = grabity * 5;

    float            leftHandRadius = 0.4f;

    bool             attackCollisionFlag = false;

    int   healse = 10;


    bool             first = false;

    float            timeremove = 0.5f;
};
