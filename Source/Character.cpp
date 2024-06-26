#include "Character.h"


//#include "StageManager.h"

#include "Mathf.h"



// �s��X�V����
void Character::UpdateTransform()
{
    // �X�P�[�������s����쐬
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x,scale.y,scale.z);
    
    // ��]�s��쐬
    DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
    DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
    DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
    DirectX::XMMATRIX R =  Y*X*Z;


    // �ʒu�s�񂾂����쐬
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x,position.y,position.z);
    // 3�̍s���g�ݍ��킹�A���[���h�s����쐬
    DirectX::XMMATRIX W = S * R * T;// �s��͌v�Z���ԕς���ƌ��ʂ��ς��
    // �v�Z�������[���h�s������o��
    DirectX::XMStoreFloat4x4(&transform, W);
    
}



// �ӂ��Ƃ΂�
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    //  �Փ� impulse
    // ���͂ɗ͂�������
    this->velocity.x += impulse.x;
    this->velocity.y += impulse.y;
    this->velocity.z += impulse.z;
}


// �_���[�W��^����
bool Character::ApplyDamage(int damage , float invincibleTime)
{
    // �_���[�W���O�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
    if (damage == 0) return false;

    // ���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
    if (health <= 0)return false;

    if (this->invincibleTimer >= 0.0f)return false;

    // ���b���G
    this->invincibleTimer = invincibleTime;

    // �_���[�W����
    health -= damage;


    // ���S�ʒm
    if (health <= 0)
    {
        OnDead();
    }
    // �_���[�W�ʒm
    else
    {
        OnDamaged();
    }

    // ���N��Ԃ��ύX�����ꍇ��true��Ԃ�
    return true;
}


// �ړ�����
void Character::Move(float vx, float vz, float speed)
{

    // �ړ������x�N�g����ݒ�
    moveVecX = vx;
    moveVecZ = vz;

    // �ő呬�x�ݒ�@���x
    maxMoveSpeed = speed;
}

//���񏈗�
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    // 1�t���[���łǂꂾ���ړ�
    speed *= elapsedTime;

    // �i�s�x�N�g����0�x�N�g���̏ꍇ�͏�������K�v�Ȃ�
    if (vx == 0 && vz == 0)return;
    
    float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.001f) return;

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


}

// �W�����v
void Character::Jump(float speed)
{
    velocity.y += speed;
    //position.y = (std::min)(position.y, speed);
}
// ���͏���
//elapsedTime��60���ꂪ   elapsedTime�o�ߎ���
void Character::UpdateVelocity(float elapsedTime)
{ 
    // �o�߃t���[��
    float elapsedFrame = 60.0f * elapsedTime;

    // �������͍X�V����
    UpdateVerticalVelocity(elapsedFrame);

    // �������͍X�V����
    UpdateHorizontalVelocity(elapsedFrame);
    // �����ړ��X�V����
    UpdateVerticalMove(elapsedTime);

    // �����ړ��X�V����
    UpdateHorizontalMove(elapsedTime);

}

// ���G���ԍX�V
void Character::UpdateInbincibleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}

// �������x�X�V����
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    // �d�͏���
    this->velocity.y += this->grabity * elapsedFrame;

}

// �����ړ��X�V����
void Character::UpdateVerticalMove(float elapsedTime)
{

    // ���������̈ړ���
    float my = velocity.y * elapsedTime;

    slopeRate = 0.0f; // �X�Η�

    // �L�����N�^�[��Y�������ƂȂ�@���x�N�g��
    DirectX::XMFLOAT3 normal = { 0,1,0 };

    //// ������
    //if (my < 0.0f)
    //{
    //    // ���C�̊J�n�ʒu�͑�����菭����
    //    DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
    //    // ���C�̏I�_�ʒu�͈ړ���̈ʒu
    //    DirectX::XMFLOAT3 end = {position.x, position.y+my , position.z};
    //
    //    // ���C�L���X�g�ɂ��n�ʔ���
    //    HitResult hit;
    //    // ���C�L���X�g���ĂԂ��߂̊֐�
    //    //if (StageManager::instance().RayCast(start, end, hit))
    //    if (StageManager::instance().RayCast(start, end, hit))
    //    {

    //        // �@���x�N�g���擾
    //        normal = hit.normal;
    // 

    //        // �n�ʂɐݒu���Ă���
    //        position = hit.position;

    //        // ��]
    //        // �������ȗ��R�͊p�x���^���ɂȂ������ɃN���݂����ɂЂ����Ȃ��悤��
    //        angle.y += hit.rotation.y;

    //        // �X�Η��̌v�Z �@����XZ���ʂɒu����x�N�g���Ƃ����̃x�N�g���ŌX�������߂���B
    //        float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
    //        slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));
    //        // ���n
    //        if (!isGround)
    //        {
    //           
    //            OnLanding();
    //        }
    //        isGround = true;
    //        velocity.y = 0.0f;

    //    }
    //    else
    //    {
    //        // �󒆂ɕ����Ă���
    //        position.y += my;
    //        isGround = false;
    //    }

    //}
    //// �㏸��
    //else if (my > 0.0f)
    //{
    //    position.y += my;
    //    isGround = false;
    //}

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

}

// �������͍X�V����
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
    // XZ���ʂ̑��͂��������� ���x�̒���
    float length = sqrtf(velocity.x*velocity.x+velocity.z*velocity.z);
    if (length > 0.0f)
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
            velocity.x -= vx*friction;
            velocity.z -= vz * friction;
        }
        // �������̑��͂����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
        else
        {
            velocity.x = 0;
            velocity.z = 0;

        }
    }

    // XZ���ʂ̑��͂���������
    if (length <= maxMoveSpeed)
    {
        // �ړ��x�N�g����0�x�N�g���łȂ��Ȃ��������
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ* moveVecZ);
        if (moveVecLength > 0.0f)
        {
            // ������
            float accelration = this->acceleration * elapsedFrame;

            // �󒆂ɂ���Ƃ��͖��C�͂����炷
            if (!isGround) accelration *= airControl;// �݂�����B30�p�[�Z���g

            // �ړ��x�N�g���ɂ��������� �܂����x��������������
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            // �ő呬�x����
            float length = sqrtf(velocity.x* velocity.x+ velocity.z* velocity.z);
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
void Character::UpdateHorizontalMove(float elapsedTime)
{

    // �������͗ʌv�Z
    float velocityLengthXZ = sqrtf(velocity.x*velocity.x+velocity.z*velocity.z);
    if (velocityLengthXZ > 0.0f)
    {
        // �����ړ��l
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        // ���C�̊J�n�ʒu�ƏI�_�ʒu
        // �i��������
        DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
        // �ړ��������������z
        DirectX::XMFLOAT3 end = { position.x + mx, position.y + stepOffset, position.z + mz };

        //    // ���C�L���X�g�ɂ��ǔ���
        //    HitResult hit;
        //    if(StageManager::instance().RayCast(start, end, hit))
        //    {
        //        // �ǂ܂ł̃x�N�g��
        //        DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
        //        DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
        //        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End,Start);

        //        // �ǂ̖@��
        //        DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

        //        // ���˃x�N�g����@���Ɏˉe��
        //        DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec),Normal);
        //        Dot = DirectX::XMVectorScale(Dot, 1.1f); // �ǂɂ߂荞�܂Ȃ��悤�ɏ��������␳

        //        // �␳�ʒu�̌v�Z
        //        DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
        //        DirectX::XMFLOAT3 collectPosition;
        //        DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

        //        // �ǂ�������փ��C�L���X�g
        //        HitResult hit2;
        //        if (!StageManager::instance().RayCast(hit.position, collectPosition, hit2))
        //        {
        //            // �ǂ�������ŕǂɓ�����Ȃ�������␳�ʒu�Ɉړ�
        //            position.x = collectPosition.x;
        //            position.z = collectPosition.z;
        //        }
        //        else
        //        {
        //            position.x = hit2.position.x;
        //            position.z = hit2.position.z;
        //        }
        //    }
        //    else
        //    {
        //        // �ړ�
        //        position.x += mx;
        //        position.z += mz;
        //    }
        //}
    }

}

void Character::UpdateReflection(float elapsedTime)
{
    // �������͗ʌv�Z
    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (velocityLengthXZ > 0.0f)
    {
        // �����ړ��l
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        // ���C�̊J�n�ʒu�ƏI�_�ʒu
        // �i��������
        DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
        // �ړ��������������z
        DirectX::XMFLOAT3 end = { position.x + mx, position.y + stepOffset, position.z + mz };

        //// ���C�L���X�g�ɂ��ǔ���
        //HitResult hit;
        //if (StageManager::instance().RayCast(start, end, hit))
        //{
        //    // �ǂ܂ł̃x�N�g��
        //    DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
        //    DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
        //    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

        //    // �ǂ̖@��
        //    DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

        //    // ���˃x�N�g����@���Ɏˉe�� a
        //    DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
        //    Dot = DirectX::XMVectorScale(DirectX::XMVectorScale(Dot,2.0f), 1.1f); // �ǂɂ߂荞�܂Ȃ��悤�ɏ��������␳

        //    // �␳�ʒu�̌v�Z
        //    DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
        //    DirectX::XMFLOAT3 collectPosition;
        //    DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

        //    // �ǂ�������փ��C�L���X�g
        //    HitResult hit2;
        //    if (!StageManager::instance().RayCast(hit.position, collectPosition, hit2))
        //    {
        //        // �ǂ�������ŕǂɓ�����Ȃ�������␳�ʒu�Ɉړ�
        //        position.x = collectPosition.x;
        //        position.z = collectPosition.z;
        //    }
        //    else
        //    {
        //        position.x = hit2.position.x;
        //        position.z = hit2.position.z;
        //    }
        //}
        //else
        //{
        //    // �ړ�
        //    position.x += mx;
        //    position.z += mz;
        //}
    }
}

