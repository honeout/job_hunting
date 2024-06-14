#include <algorithm>
#include <imgui.h>
#include "Math.h"
#include "Mathf.h"
#include "Movement.h"

#include "StageManager.h"

// �R���X�g���N�^
Movement::Movement()
{

}

// �f�X�g���N�^
Movement::~Movement()
{

}

// GUI�`��
void Movement::OnGUI()
{
	ImGui::InputFloat("Move Speed", &moveSpeed);
	ImGui::InputFloat("Turn Speed", &turnSpeed);
    ImGui::InputInt("Jump max", &jumpCount);
    ImGui::InputFloat("stepOffSet", &stepOffSet);
}

// ���[���h�ړ�
void Movement::Move(const DirectX::XMFLOAT3& direction,float speed ,float elapsedTime)
{
	std::shared_ptr<Actor> actor = GetActor();
    moveSpeed = speed * elapsedTime;
	DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
	DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(Direction, moveSpeed);


    DirectX::XMStoreFloat3(&velocity, Velocity);

    moveVecX = velocity.x;
    moveVecZ = velocity.z;



}

// ���[�J���ړ�
void Movement::MoveLocal(const DirectX::XMFLOAT3& direction, float elapsedTime)
{
	std::shared_ptr<Actor> actor = GetActor();
	speed = moveSpeed * elapsedTime;
    //turnSpeed = moveSpeed * elapsedTime;
	DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
	DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(Direction, speed);
	DirectX::XMVECTOR Rotation = DirectX::XMLoadFloat4(&actor->GetRotation());
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationQuaternion(Rotation);
	DirectX::XMVECTOR Move = DirectX::XMVector3TransformNormal(Velocity, Transform);
	DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&actor->GetPosition());

	Position = DirectX::XMVectorAdd(Position, Move);

	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, Position);
	actor->SetPosition(position);

   // DirectX::XMStoreFloat3(&velocity, Velocity);
}
//
// ����
void Movement::Turn(const DirectX::XMFLOAT3& direction, float elapsedTime)
{/*
	std::shared_ptr<Actor> actor = GetActor();
	speed = turnSpeed * elapsedTime;
	DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
	DirectX::XMVECTOR Rotation = DirectX::XMLoadFloat4(&actor->GetRotation());
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationQuaternion(Rotation);
	DirectX::XMVECTOR OneZ = DirectX::XMVectorSet(0, 0, 1, 0);
	DirectX::XMVECTOR Front = DirectX::XMVector3TransformNormal(OneZ, Transform);

    if (direction.x == 0 && direction.z == 0)return;

	Direction = DirectX::XMVector3Normalize(Direction);
	DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Front, Direction);
	if (DirectX::XMVector3Equal(Axis, DirectX::XMVectorZero()))
	{
		return;
	}

	DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Front, Direction);

	float dot;
	DirectX::XMStoreFloat(&dot, Dot);
    speed = (std::min)(1.0f - dot, speed);

	DirectX::XMVECTOR Turn = DirectX::XMQuaternionRotationAxis(Axis, speed);
	Rotation = DirectX::XMQuaternionMultiply(Rotation, Turn);

	DirectX::XMFLOAT4 rotation;
	DirectX::XMStoreFloat4(&rotation, Rotation);
	actor->SetRotation(rotation);*/



     //1�t���[���łǂꂾ���ړ�
    speed = turnSpeed * elapsedTime;
    //speed *= elapsedTime;

    std::shared_ptr<Actor> actor = GetActor();

    DirectX::XMFLOAT4 rotate = actor->GetRotation();

    // �i�s�x�N�g����0�x�N�g���̏ꍇ�͏�������K�v�Ȃ�
    if (direction.x == 0 && direction.z == 0)return;

    float vx = direction.x;
    float vz = direction.z;

    float length = sqrtf(direction.x * direction.x + direction.z * direction.z);
    if (length < 0.001f) return;

    vx /= length;
    vz /= length;

    // ���g�̉�]�l����O���������߂�
    float frontX = sinf(rotate.y); // ���E�������
    float frontZ = cosf(rotate.y); // �O�㔻��̂���

    //��]�p�����߂邽�߁A�Q�̃x�N�g���̓��ς��v�Z����
    float dot = (frontX * vx) + (frontZ * vz);// ����

    // ���ϒn��-1~1�ŕ\������Ă��ĂQ�̒P�ʃx�N�g���̊p�x��
    // �������قǂP�D�O�ɂ����Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�
    float rot = 1.0 - dot;
    if (rot > speed) rot = speed;


    // ���E������s���ׂɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
    float cross = (frontZ * vx) - (frontX * vz);

    // 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
    // ���E������s�����Ƃɂ���č��E��]��I������
    if (cross < 0.0f)
    {
        rotate.y -= rot;
    }
    else
    {
        rotate.y += rot;
    }

    actor->SetRotation(rotate);
    
}
//
////���񏈗�
//void Movement::Turn(float elapsedTime, float vx, float vz, float speed)
//{
//    // 1�t���[���łǂꂾ���ړ�
//    speed *= elapsedTime;
//
//    std::shared_ptr<Actor> actor = GetActor();
//
//    DirectX::XMFLOAT3 angle = 
//    {
//        actor->GetRotation().x,
//        actor->GetRotation().y,
//        actor->GetRotation().z
//    };
//
//    // �i�s�x�N�g����0�x�N�g���̏ꍇ�͏�������K�v�Ȃ�
//    if (vx == 0 && vz == 0)return;
//
//    float length = sqrtf(vx * vx + vz * vz);
//    if (length < 0.001f) return;
//
//    vx /= length;
//    vz /= length;
//
//    // ���g�̉�]�l����O���������߂�
//    float frontX = sinf(angle.y); // ���E�������
//    float frontZ = cosf(angle.y); // �O�㔻��̂���
//
//    //��]�p�����߂邽�߁A�Q�̃x�N�g���̓��ς��v�Z����
//    float dot = (frontX * vx) + (frontZ * vz);// ����
//
//    // ���ϒn��-1~1�ŕ\������Ă��ĂQ�̒P�ʃx�N�g���̊p�x��
//    // �������قǂP�D�O�ɂ����Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�
//    float rot = 1.0 - dot;
//    if (rot > speed) rot = speed;
//
//
//    // ���E������s���ׂɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
//    float cross = (frontZ * vx) - (frontX * vz);
//
//    // 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
//    // ���E������s�����Ƃɂ���č��E��]��I������
//    if (cross < 0.0f)
//    {
//        angle.y -= rot;
//    }
//    else
//    {
//        angle.y += rot;
//    }
//
//    actor->SetRotation(
//        { angle.x,angle.y,angle.z,0 });
//}


// ���[���h�ړ�
void Movement::OnLanding()
{
    std::shared_ptr<Actor> actor = GetActor();
    
    
    // ���n�������烊�Z�b�g
    jumpCount = 0;
    
    //// �������̑��͂����ȏ�Ȃ璅�n�X�e�[�g��  �\���ȑ��x�ŗ��Ƃ��d�͂̂T�{�Q�A�R�b��ɒ��n���[�V����������B
    if (velocity.y < gravity * 5.0f)
    {
        onLadius = true;
        
        
    }
}

void Movement::JumpVelocity( float speed)
{
    std::shared_ptr<Actor> actor = GetActor();
    //DirectX::XMFLOAT3 velocity = Mathf::Scale(direction, speed);
    DirectX::XMFLOAT3 position = actor->GetPosition();




    


    velocity.y = speed;
    //jumpSpeed = speed;

    //position.y = position.y * jumpSpeed;

   // position.y += velocity.y;

    //actor->SetPosition(position);
    //position.y = (std::min)(jumpSpeedMax, jumpSpeed);
    actor->SetPosition(position);
}

void Movement::UpdateHorizontalVelocity( float elapsedFrame)
{
    std::shared_ptr<Actor> actor = GetActor();
    //speed = moveSpeed * elapsedFrame;
    //DirectX::XMFLOAT3 velocity = Mathf::Scale(direction, speed);



    DirectX::XMFLOAT3 position = actor->GetPosition();


    // XZ���ʂ̑��͂��������� ���x�̒���
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
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
            velocity.x -= vx * friction;
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

void Movement::UpdateHorizontalMove( float elapsedTime)
{
    std::shared_ptr<Actor> actor = GetActor();
    //speed = moveSpeed * elapsedTime;
    //DirectX::XMFLOAT3 velocity = Mathf::Scale(direction, speed);
    DirectX::XMFLOAT3 position = actor->GetPosition();
    //float stepOffSet = actor->GetStepOffset();


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

        // ���C�L���X�g�ɂ��ǔ���
        HitResult hit;
        if (StageManager::instance().RayCast(start, end, hit))
        {
            // �ǂ܂ł̃x�N�g��
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

            // �ǂ̖@��
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

            // ���˃x�N�g����@���Ɏˉe��
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
            Dot = DirectX::XMVectorScale(Dot, 1.1f); // �ǂɂ߂荞�܂Ȃ��悤�ɏ��������␳

            // �␳�ʒu�̌v�Z
            DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
            DirectX::XMFLOAT3 collectPosition;
            DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

            // �ǂ�������փ��C�L���X�g
            HitResult hit2;
            if (!StageManager::instance().RayCast(hit.position, collectPosition, hit2))
            {
                // �ǂ�������ŕǂɓ�����Ȃ�������␳�ʒu�Ɉړ�
                position.x = collectPosition.x;
                position.z = collectPosition.z;
            }
            else
            {
                position.x = hit2.position.x;
                position.z = hit2.position.z;
            }
        }
        else
        {
            // �ړ�
            position.x += mx;
            position.z += mz;
        }

        actor->SetPosition(position);
        
    }
}

void Movement::UpdateVerticalVelocity( float elapsedFrame)
{
    std::shared_ptr<Actor> actor = GetActor();
    //speed = moveSpeed * elapsedFrame;
    //DirectX::XMFLOAT3 velocity = Mathf::Scale(direction, speed);
    //DirectX::XMFLOAT3 position = actor->GetPosition();
    //float stepOffSet = actor->GetStepOffset();

    //DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
    //DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(Direction, speed);

   // DirectX::XMStoreFloat3(&velocity, Velocity);

    // �d�͏���
    velocity.y += gravity * elapsedFrame;

    //actor->SetPosition(position);

}

void Movement::UpdateVerticalMove( float elapsedTime)
{
    std::shared_ptr<Actor> actor = GetActor();

    DirectX::XMFLOAT3 position = actor->GetPosition();

    DirectX::XMFLOAT4 rotation = { actor->GetRotation()};

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

        // ���C�L���X�g�ɂ��n�ʔ���
        HitResult hit;
        // ���C�L���X�g���ĂԂ��߂̊֐�
        //if (StageManager::instance().RayCast(start, end, hit))
        if (StageManager::instance().RayCast(start, end, hit))
        {

            // �@���x�N�g���擾
            normal = hit.normal;


            // �n�ʂɐݒu���Ă���
            position = hit.position;

            // ��]
            // �������ȗ��R�͊p�x���^���ɂȂ������ɃN���݂����ɂЂ����Ȃ��悤��
            rotation.y += hit.rotation.y;

            // �X�Η��̌v�Z �@����XZ���ʂɒu����x�N�g���Ƃ����̃x�N�g���ŌX�������߂���B
            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));
            // ���n
            if (!isGround)
            {

                OnLanding();
            }
            isGround = true;
            velocity.y = 0.0f;
            
            //jumpSpeed = 0.0f;

        }
        else
        {
            // �󒆂ɕ����Ă���
            position.y += my;
            isGround = false;
            onLadius = false;
        }

    }
    // �㏸��
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }

    // �n�ʂ̌����ɉ����悤��XZ����]
    {
        // Y�����@���x�N�g�������Ɍ����I�C���[�p��]���Z�o����
        float storageAngleX = atan2f(normal.z, normal.y);
        float storageAngleZ = -atan2f(normal.x, normal.y);

        // ���`�⊮�Ŋ��炩�ɉ�]����
        // (�ω�����l,�ŏI�I�Ȓl,���ꂾ���i)
        rotation.x = Mathf::Lerp(rotation.x, storageAngleX, 0.2f);
        rotation.z = Mathf::Lerp(rotation.z, storageAngleZ, 0.2f);

    }

    actor->SetPosition(position);
    actor->SetRotation(rotation);
}

void Movement::UpdateVelocity( float elapsedTime)
{
    //DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
    //DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(Direction, speed);

  /*  DirectX::XMStoreFloat3(&velocity, Velocity);*/


    // �o�߃t���[��
    float elapsedFrame = 60.0f * elapsedTime;

    // �������͍X�V����
    UpdateVerticalVelocity(elapsedFrame);

    // �������͍X�V����
    UpdateHorizontalVelocity( elapsedFrame);

    // �����ړ��X�V����
    UpdateHorizontalMove(elapsedFrame);

    // �����ړ��X�V����
    UpdateVerticalMove(elapsedTime);
}
