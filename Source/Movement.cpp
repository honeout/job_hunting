#include <algorithm>
#include <imgui.h>
#include "Math.h"
#include "Mathf.h"
#include "Movement.h"
#include "Collision.h"
#include "StageMain.h"
#include "ModelControll.h"

// �R���X�g���N�^
Movement::Movement()
{
    area.max.x = 0.0f;
    area.max.y = 0.0f;
    area.max.z = 0.0f;

    area.min.x = 0.0f;
    area.min.y = 0.0f;
    area.min.z = 0.0f;
}

// �f�X�g���N�^
Movement::~Movement()
{

}
#ifdef _DEBUG
// GUI�`��
void Movement::OnGUI()
{
	ImGui::InputFloat("Move Speed", &moveSpeed);
	ImGui::InputFloat("Turn Speed", &turnSpeed);
    ImGui::InputInt("Jump max", &jumpCount);
    //ImGui::InputFloat("stepOffSet", &stepOffSet);
    ImGui::InputFloat3("velocity", &velocity.x);

    if (onLadius)
        ++butten;
    ImGui::InputInt("butten", &butten);

    
}
#endif // _DEBUG
// ���[���h�ړ�
void Movement::Move(const DirectX::XMFLOAT3& direction,float speed ,float elapsedTime)
{
	std::weak_ptr<Actor> actor = GetActor();
    maxMoveSpeed = speed;
    moveVecX = direction.x;
    moveVecZ = direction.z;
}

// ����
bool Movement::Turn(const DirectX::XMFLOAT3& direction,float speed, float elapsedTime)
{
    speed = speed * elapsedTime;
    std::weak_ptr<Actor> actor = GetActor();
    std::weak_ptr<Transform> transformid = actor.lock()->GetComponent<Transform>();
    DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();
    // �i�s�x�N�g����0�x�N�g���̏ꍇ�͏�������K�v�Ȃ�
    if (direction.x == isImputEmpty && direction.z == isImputEmpty)
        return true;
    float vx = direction.x;
    float vz = direction.z;
    float length = sqrtf(direction.x * direction.x + direction.z * direction.z);
    if (length < 0.001f) return true;
    vx /= length;
    vz /= length;
    // ���g�̉�]�l����O���������߂�
    float frontX = sinf(angle.y); // ���E�������
    float frontZ = cosf(angle.y); // �O�㔻��̂���
    //��]�p�����߂邽�߁A�Q�̃x�N�g���̓��ς��v�Z����
    float dot = (frontX * vx) + (frontZ * vz);// ����
    // ���ϒn��-1~1�ŕ\������Ă��ĂQ�̒P�ʃx�N�g���̊p�x��
    // �������قǂP�D�O�ɂ����Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�
    float rot = 1.0f - dot;
    if (rot > speed) rot = speed;
    // ���E������s���ׂɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
    float cross = (frontZ * vx) - (frontX * vz);
    // 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
    // ���E������s�����Ƃɂ���č��E��]��I������
    if (cross < 0.0f)
    {
        angle.y -= rot;
    }
    else
    {
        angle.y += rot;
    }
    transformid.lock()->SetAngle(angle);
    if (dot + FLT_EPSILON >= -(0.9f - FLT_EPSILON))
    {
        return true;
    }
    else if (dot - FLT_EPSILON <= (0.9f + FLT_EPSILON))
    {
        return true;
    }
    return false;
}
// ��]�o���邩
bool Movement::TurnCheck(const DirectX::XMFLOAT3& direction, DirectX::XMFLOAT2 angleRange, float elapsedTime)
{
    std::weak_ptr<Actor> actor = GetActor();
    std::weak_ptr<Transform> transformid = actor.lock()->GetComponent<Transform>();
    DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();
    // �i�s�x�N�g����0�x�N�g���̏ꍇ�͏�������K�v�Ȃ�
    if (direction.x == isImputEmpty && direction.z == isImputEmpty)
        return true;
    float vx = direction.x;
    float vz = direction.z;
    float length = sqrtf(direction.x * direction.x + direction.z * direction.z);
    if (length < 0.001f) return true;
    vx /= length;
    vz /= length;
    // ���g�̉�]�l����O���������߂�
    float frontX = sinf(angle.y); // ���E�������
    float frontZ = cosf(angle.y); // �O�㔻��̂���
    // ���E������s���ׂɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
    float cross = (frontZ * vx) - (frontX * vz);
    // 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
    // ���E������s�����Ƃɂ���č��E��]��I������
    if (cross > angleRange.x)
    {
        return true;
    }
    else if (cross < angleRange.y)
    {
        return true;
    }
    return false;
}

// ���[���h�ړ�
void Movement::OnLanding()
{
    std::weak_ptr<Actor> actor = GetActor();
    // ���n�������烊�Z�b�g
    jumpCount = jumpCountMin;
    //// �������̑��͂����ȏ�Ȃ璅�n�X�e�[�g��  �\���ȑ��x�ŗ��Ƃ��d�͂̂T�{�Q�A�R�b��ɒ��n���[�V����������B
    if (velocity.y < gravity * 5.0f)
    {
        onLadius = true;
    }
}

// �W�����v
void Movement::JumpVelocity(float speed)
{
    std::weak_ptr<Actor> actor = GetActor();
    ++jumpCount;
    velocity.y = speed;
}

// ������΂�
void Movement::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}
// �������͍X�V����
void Movement::UpdateHorizontalVelocity( float elapsedFrame)
{
    std::weak_ptr<Actor> actor = GetActor();
    std::weak_ptr<Transform> transformid = actor.lock()->GetComponent<Transform>();
    DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();

    // XZ���ʂ̑��͂��������� ���x�̒���
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if ((length + FLT_EPSILON )> (0.0f - FLT_EPSILON))
    {
        // ���C��
        float friction = this->friction * elapsedFrame;
        // �󒆂ɂ���Ƃ��͖��C�͂����炷
        if (!isGround) friction *= airControl;// ���X�̖��C�̌v�������炷��
        // ���C�ɂ�鉡�����̌�������
        if (length > friction)
        {
            // �P�ʃx�N�g��������B
            float vx = velocity.x / length;
            float vz = velocity.z / length;
            // ������R�͂̕�����
            velocity.x -= vx * friction;
            velocity.z -= vz * friction;
        }
        // �������̑��͂����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
        else
        {
            velocity.x = stopVelocity;
            velocity.z = stopVelocity;

        }
    }
    // XZ���ʂ̑��͂���������
    if (length <= maxMoveSpeed)
    {
        // �ړ��x�N�g����0�x�N�g���łȂ��Ȃ��������
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
        if (moveVecLength > 0.0f)
        {
            // ������
            float accelration = this->acceleration * elapsedFrame;
            // �󒆂ɂ���Ƃ��͖��C�͂����炷
            if (!isGround) accelration *= isGround;// �݂�����B30�p�[�Z���g
            // �ړ��x�N�g���ɂ��������� �܂����x��������������
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;
            // �ő呬�x����
            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;
                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }
            // �����ŃK�^�K�^���Ȃ��悤�ɂ���
            if (isGround && slopeRate > 0.0f)
            {
                velocity.y -= length * slopeRate * elapsedFrame;
            }
        }
    }
    // �ړ��x�N�g�������Z�b�g �A�N�Z������ł��Ԃ�����
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}
// �����ړ��X�V����
void Movement::UpdateHorizontalMove( float elapsedTime)
{
    std::weak_ptr<Actor> actor = GetActor();
    std::weak_ptr<Transform> transformid = actor.lock()->GetComponent<Transform>();
    DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();

    // �������͗ʌv�Z
    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (velocityLengthXZ > 0.0f)
    {
        // �����ړ��l
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;
        // ���C�̊J�n�ʒu�ƏI�_�ʒu
        // �i��������
        DirectX::XMFLOAT3 start = { position.x, position.y + stepOffSet, position.z };
        // �ړ��������������z
        DirectX::XMFLOAT3 end = { position.x + mx, position.y + stepOffSet, position.z + mz };
        if (!IsOutOfAreaX(position.x))
        {
            // �ړ�X
            position.x += mx;
        }
        if (!IsOutOfAreaZ(position.z))
        {
            // �ړ�Z
            position.z += mz;
        }
    }
    transformid.lock()->SetPosition(position);
}
// �������͍X�V����
void Movement::UpdateVerticalVelocity( float elapsedFrame)
{
    std::weak_ptr<Actor> actor = GetActor();
    // �d�͏���
    velocity.y += gravity * elapsedFrame;
}
// �����ړ��X�V����
void Movement::UpdateVerticalMove( float elapsedTime)
{
    std::weak_ptr<Actor> actor = GetActor();
    std::weak_ptr<Transform> transformid = actor.lock()->GetComponent<Transform>();
    DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();
    DirectX::XMFLOAT3 angle = {transformid.lock()->GetAngle()};

    // ���������̈ړ���
    float my = velocity.y * elapsedTime;
    slopeRate = 0.0f; // �X�Η�
    // �L�����N�^�[��Y�������ƂȂ�@���x�N�g��
    DirectX::XMFLOAT3 normal = { 0,1,0 };
    // ������
    if (my <= 0.0f)
    {
        // ���C�̊J�n�ʒu�͑�����菭����
        DirectX::XMFLOAT3 start = { position.x, position.y + stepOffSet, position.z };
        // ���C�̏I�_�ʒu�͈ړ���̈ʒu
        DirectX::XMFLOAT3 end = { position.x, position.y + my , position.z };
        if (position.y + FLT_EPSILON >= (area.min.y - FLT_EPSILON))
        {
            // �󒆂ɕ����Ă���
            position.y += my;
            isGround = false;
            onLadius = false;
        }
        else
        {
            //// ���n
            //if (!isGround)
            //{

            //    OnLanding();
            //}
            // ���n�������烊�Z�b�g
            jumpCount = jumpCountMin;
            isGround = true;
            onLadius = true;
            velocity.y = 0.0f;
        }
    }
    // �㏸��
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
        onLadius = false;
    }
    // �n�ʂ̌����ɉ����悤��XZ����]
    {
        // Y�����@���x�N�g�������Ɍ����I�C���[�p��]���Z�o����
        float storageAngleX = atan2f(normal.z, normal.y);
        float storageAngleZ = -atan2f(normal.x, normal.y);

        // ���`�⊮�Ŋ��炩�ɉ�]����
        // (�ω�����l,�ŏI�I�Ȓl,���ꂾ���i)
        angle.x = Mathf::Lerp(angle.x, storageAngleX, 0.2f);
        angle.z = Mathf::Lerp(angle.z, storageAngleZ, 0.2f);

    }
    transformid.lock()->SetPosition(position);
    transformid.lock()->SetAngle(angle);
}
// ���͏����X�V
void Movement::UpdateVelocity( float elapsedTime)
{
    // �o�߃t���[��
    float elapsedFrame = flame * elapsedTime;
    if (!stopMove)
    {
        // �������͍X�V����
        UpdateHorizontalVelocity(elapsedFrame);
        // �����ړ��X�V����
        UpdateHorizontalMove(elapsedTime);
    }
    else
    {
        // �ړ���~����Ɉړ���h������
        velocity.x = stopVelocity;
        velocity.z = stopVelocity;
    }
    if (!stopFall)
    {
        // �������͍X�V����
        UpdateVerticalVelocity(elapsedFrame);
        // �����ړ��X�V����
        UpdateVerticalMove(elapsedTime);
    }
    else
    {
        // ������������肳���邽��
        velocity.y = stopVelocity;
    }
}
// �s���͈̓`�F�b�NX
bool Movement::IsOutOfAreaX(const float& positionX)
{
    if ((positionX + velocity.x) < area.min.x || (positionX + velocity.x) > area.max.x)
    {
        velocity.x = stopVelocity;
        return true;
    }
    return false;
}
// �s���͈̓`�F�b�NZ
bool Movement::IsOutOfAreaZ( const float& positionZ)
{
    if ((positionZ + velocity.z) < area.min.z || (positionZ + velocity.z) > area.max.z)
    {
        velocity.z = stopVelocity;
        return true;
    }
    return false;
}
// �s���͈̓`�F�b�NY
bool Movement::IsOutOfAreaY(const float& positionY)
{
    return positionY < area.min.y || positionY > area.max.y;
}
