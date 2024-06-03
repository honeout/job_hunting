#pragma once

#include <DirectXMath.h>
#include "Effect.h"
// �L�����N�^�[
class Character
{
public:
    Character() = default;
    virtual ~Character() = default;


    // �s��X�V����
    void UpdateTransform();

    // �ʒu�擾
    const DirectX::XMFLOAT3& GetPosition() const { return position; }
    // �ʒu�ݒ�
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    // ��]�擾
    const DirectX::XMFLOAT3& GetAngle() const { return angle; }

    // ��]�ݒ�
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    // �X�P�[���擾
    const DirectX::XMFLOAT3& GetSclae() const { return scale; }
    // �X�P�[���擾
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    //  �͂�������
    void AddImpulse(const DirectX::XMFLOAT3& impulse);

    // ���a�擾 05
    float GetRadius() const { return radius; }

    // �n�ʂɐݒu���Ă��邩 �G�t�F�N�g����
    bool IsGround() const { return isGround; }

    // �����擾
    float GetHeight() const { return height; }

    // �_���[�W��^���� �_���[�W���󂯂��瑼�ɓn���B
    bool ApplyDamage(int damage, float invincibleTime);

    // HP��Ԃ��擾
    int GetHealth() const { return health; }

    // �ő�HP��Ԃ��擾
    int GetMaxHealth() const { return maxHealth; }

    // �A�j���[�V�����̎���
    float GetCurrentANimationSeconds() const { return currentANimationSeconds; }
    // �A�j���[�V�����̎���
    void SetCurrentAnimationSeconds(const float currentANimationSeconds) { this->currentANimationSeconds = currentANimationSeconds; }

    // �s��̎p��
    DirectX::XMFLOAT4X4 GetTransform() { return this->transform; }

    // �s��̎p�����Z�b�g
    void SetTransform(DirectX::XMFLOAT4X4 transform) { this->transform = transform; }

    // �������Ԃ̃Z�b�g
    void SetSurvivalTime(float survivaltime) {this->survivaltime = survivaltime;}

    // ���炷���ԁ@�������Ԃ�
    void SetReduce(float reduce) {this->reduce = reduce;}

protected:
    // �ړ�����
    void Move(float vx, float vz, float speed);

    // ���񏈗�
    void Turn(float elapsedTime, float vx, float vz, float speed);

    // �W�����v����
    void Jump(float speed);

    // ���͏����X�V
    void UpdateVelocity(float elapsedTime);
    void UpdateInbincibleTimer(float elapsedTime);

    // ���n�������ɌĂׂ��
    virtual void OnLanding() {};

    // �_���[�W���󂯂����ɌĂ΂��
    virtual void OnDamaged() {};
    // ���S�������ɌĂ΂��
    virtual void OnDead() {};
private:
    // �������͍X�V����
    void UpdateVerticalVelocity(float elapsedFrame);

    // �����ړ��X�V����
    void UpdateVerticalMove(float elapsedTime);

    // �������͍X�V����
    void UpdateHorizontalVelocity(float elapsedFrame);
    // �����ړ��X�V����
    void UpdateHorizontalMove(float elapsedTime);

    // ���˒e�ۍX�V����
    void UpdateReflection(float elapsedTime);


protected:// �p����ł��g����
    DirectX::XMFLOAT3             position = { 0, 0, 0 };
    DirectX::XMFLOAT3             angle = { 0, 0, 0 };
    DirectX::XMFLOAT3             scale = { 1, 1, 1 };
    DirectX::XMFLOAT4X4 transform = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
    // ���a05
    float radius = 0.5f;

    // �d��
    float              grabity = -1.0f;
    // ���x
    DirectX::XMFLOAT3  velocity = { 0,0,0 };

    // �n�ʊm�F
    bool         isGround = false;
    // �g�����[�g��
    float               height = 2.0f;

    // HP
    int          health = 5;
    // �ő�lHP
    int          maxHealth = 5;
    // ���G����
    float   invincibleTimer = 1.0f;
    // ���C
    float   friction = 0.5f;

    // ����
    float acceleration = 1.0f;
    // speed�ő�l
    float maxMoveSpeed = 5.0f;
    // �ړ�����X�Ƒ��x
    float moveVecX = 0.0f;
    // �ړ�����Y�Ƒ��x
    float moveVecZ = 0.0f;

    // �󒆂ł̈ړ����x����
    float airControl = 0.3f;


    // ��
    float stepOffset = 1.0f;
    // �X�Η�
    float slopeRate = 1.0f;

    // �A�j���[�V�����̎���
    float currentANimationSeconds = 0.0f;

    // ��������
    float survivaltime = 0.0f;

    // ���炷�b���@�������Ԏ�
    float reduce = 0;

    float alpha = 1;
};