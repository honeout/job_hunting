#include <algorithm>
#include <imgui.h>
#include "Math.h"
#include "Mathf.h"
#include "Movement.h"
#include "Collision.h"
#include "StageMain.h"
#include "ModelControll.h"


//#include "StageManager.h"

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
   // moveSpeed = speed * elapsedTime;
    maxMoveSpeed = speed;
	//DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
	//DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(Direction, moveSpeed);

    //collision = actor.lock()->GetComponent<Collision>();
    //transformid = actor.lock()->GetComponent<Transform>();

    //DirectX::XMStoreFloat3(&velocity, Velocity);

    moveVecX = direction.x;
    moveVecZ = direction.z;



}

// ���[�J���ړ�
void Movement::MoveLocal(const DirectX::XMFLOAT3& direction, float elapsedTime)
{
   // transformid = GetActor()->GetComponent<Transform>();
	//std::shared_ptr<Actor> actor = GetActor();
	//speed = moveSpeed * elapsedTime;
 //   //turnSpeed = moveSpeed * elapsedTime;
	//DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
	//DirectX::XMVECTOR Velocity = DirectX::XMVectorScale(Direction, speed);
	//DirectX::XMVECTOR Rotation = DirectX::XMLoadFloat3(&GetActor()->GetComponent<Transform>()->GetAngle());
	//DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationQuaternion(Rotation);
	//DirectX::XMVECTOR Move = DirectX::XMVector3TransformNormal(Velocity, Transform);
	//DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&GetActor()->GetComponent<Transform>()->GetPosition());

	//Position = DirectX::XMVectorAdd(Position, Move);

	//DirectX::XMFLOAT3 position;
	//DirectX::XMStoreFloat3(&position, Position);
 //   GetActor()->GetComponent<Transform>()->SetPosition(position);

   // DirectX::XMStoreFloat3(&velocity, Velocity);
}
//
// ����
bool Movement::Turn(const DirectX::XMFLOAT3& direction,float speed, float elapsedTime)
{
    speed = speed * elapsedTime;

    std::weak_ptr<Actor> actor = GetActor();
    std::weak_ptr<Transform> transformid = actor.lock()->GetComponent<Transform>();

    DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();

    // �i�s�x�N�g����0�x�N�g���̏ꍇ�͏�������K�v�Ȃ�
    if (direction.x == 0 && direction.z == 0)
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

    //if ((dot + FLT_EPSILON) >= -(0.1f - FLT_EPSILON) && 
    //    dot - FLT_EPSILON <= (0.1f + FLT_EPSILON))
    //{
    //    return true;
    //}

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
bool Movement::TurnCheck(const DirectX::XMFLOAT3& direction, DirectX::XMFLOAT2 angleRange, float elapsedTime)
{
    std::weak_ptr<Actor> actor = GetActor();
    std::weak_ptr<Transform> transformid = actor.lock()->GetComponent<Transform>();

    DirectX::XMFLOAT3 angle = transformid.lock()->GetAngle();

    // �i�s�x�N�g����0�x�N�g���̏ꍇ�͏�������K�v�Ȃ�
    if (direction.x == 0 && direction.z == 0)
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
    std::weak_ptr<Actor> actor = GetActor();
    
    
    // ���n�������烊�Z�b�g
    jumpCount = 0;
    
    //// �������̑��͂����ȏ�Ȃ璅�n�X�e�[�g��  �\���ȑ��x�ŗ��Ƃ��d�͂̂T�{�Q�A�R�b��ɒ��n���[�V����������B
    if (velocity.y < gravity * 5.0f)
    {
        onLadius = true;
        
        
    }
}

void Movement::JumpVelocity(float speed)
{
    std::weak_ptr<Actor> actor = GetActor();
    //DirectX::XMFLOAT3 velocity = Mathf::Scale(direction, speed);
    //DirectX::XMFLOAT3 position = actor->GetPosition();




    ++jumpCount;


    velocity.y = speed;
    //jumpSpeed = speed;

    //position.y = position.y * jumpSpeed;

   // position.y += velocity.y;

    //actor->SetPosition(position);
    //position.y = (std::min)(jumpSpeedMax, jumpSpeed);
    //actor->SetPosition(position);
}

void Movement::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}

void Movement::UpdateHorizontalVelocity( float elapsedFrame)
{
    std::weak_ptr<Actor> actor = GetActor();
    std::weak_ptr<Transform> transformid = actor.lock()->GetComponent<Transform>();
    //speed = moveSpeed * elapsedFrame;
    //DirectX::XMFLOAT3 velocity = Mathf::Scale(direction, speed);


    
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
    std::weak_ptr<Actor> actor = GetActor();
    std::weak_ptr<Transform> transformid = actor.lock()->GetComponent<Transform>();
    //speed = moveSpeed * elapsedTime;
    //DirectX::XMFLOAT3 velocity = Mathf::Scale(direction, speed);
    DirectX::XMFLOAT3 position = transformid.lock()->GetPosition();
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

        //// ���C�L���X�g�ɂ��ǔ���
        //HitResult hit;



        //// ���f���f�[�^
        //Model* stagemodel = StageManager::Instance().GetStage(StageManager::Instance().GetStageCount() - 1)->GetComponent<ModelControll>()->GetModel();
        //

        //if (collision->IntersectRayVsModel(start,end, stagemodel,hit))
        //{

        //  /*  actor2->SetName("Player");*/
        //    // �ǂ܂ł̃x�N�g��
        //    DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
        //    DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
        //    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

        //    // �ǂ̖@��
        //    DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

        //    // ���˃x�N�g����@���Ɏˉe��
        //    DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
        //    Dot = DirectX::XMVectorScale(Dot, 1.1f); // �ǂɂ߂荞�܂Ȃ��悤�ɏ��������␳

        //    // �␳�ʒu�̌v�Z
        //    DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
        //    DirectX::XMFLOAT3 collectPosition;
        //    DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

        //    // �ǂ�������փ��C�L���X�g
        //    HitResult hit2;


        //    // ���f���f�[�^
        //    Model* stagemodel = StageManager::Instance().GetStage(StageManager::Instance().GetStageCount() - 1)->GetComponent<ModelControll>()->GetModel();

        //    //// ���f���f�[�^������B
        //    //Model* model2 = GetActor()->GetModel();
        //           
        //   // if (!StageManager::instance().RayCast(hit.position, collectPosition, hit2))
        //    //if (!stageMain->RayCast(hit.position, collectPosition,  hit))
        //    //if (!Collision::IntersectRayVsModel(hit.position, collectPosition, GetActor()->GetComponent<HP>()->GetActor()->GetModel(),  hit))
        //    if (!collision->IntersectRayVsModel(hit.position, collectPosition, stagemodel, hit2))
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
    transformid.lock()->SetPosition(position);
}

void Movement::UpdateVerticalVelocity( float elapsedFrame)
{
    std::weak_ptr<Actor> actor = GetActor();
  

    // �d�͏���
    velocity.y += gravity * elapsedFrame;

 

}

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

        //

        if (position.y + FLT_EPSILON >= (area.min.y - FLT_EPSILON))
        {
            // �󒆂ɕ����Ă���
            position.y += my;
            isGround = false;
            onLadius = false;
        }
        else
        {
            // ���n
            if (!isGround)
            {

                OnLanding();
            }
            isGround = true;
            onLadius = true;
            velocity.y = 0.0f;

            //jumpSpeed = 0.0f;
        }


       // // ���C�L���X�g�ɂ��n�ʔ���
       // HitResult hit;



       // // ���f���f�[�^������B
       // //Model* model = GetActor()->GetModel();

       // //std::shared_ptr<Actor> actor2 = GetActor();
       // //actor2->SetName("StageMain");
       // //// ���f���f�[�^������B
       // //Model* model = actor2->GetModel();
       // //Model* model = stageMain->GetModel();

       // // ���f���f�[�^
       // Model* stagemodel = StageManager::Instance().GetStage(StageManager::Instance().GetStageCount() - 1)->GetComponent<ModelControll>()->GetModel();

       // // ���C�L���X�g���ĂԂ��߂̊֐�
       // //if (StageManager::instance().RayCast(start, end, hit))
       //// if (collision->IntersectRayVsModel(start, end, GetActor()->GetComponent<StageMain>()->GetActor()->GetModelSabe(), hit))
       // if (collision->IntersectRayVsModel(start, end, stagemodel, hit))
       // {
    

       //     // �@���x�N�g���擾
       //     normal = hit.normal;


       //     // �n�ʂɐݒu���Ă���
       //     position = hit.position;

       //     // ��]
       //     // �������ȗ��R�͊p�x���^���ɂȂ������ɃN���݂����ɂЂ����Ȃ��悤��
       //     angle.y += hit.rotation.y;

       //     // �X�Η��̌v�Z �@����XZ���ʂɒu����x�N�g���Ƃ����̃x�N�g���ŌX�������߂���B
       //     float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
       //     slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));
       //     // ���n
       //     if (!isGround)
       //     {

       //         OnLanding();
       //     }
       //     isGround = true;
       //     onLadius = true;
       //     velocity.y = 0.0f;
       //     
       //     //jumpSpeed = 0.0f;

       // }
       // else
       // {
       //     // �󒆂ɕ����Ă���
       //     position.y += my;
       //     isGround = false;
       //     onLadius = false;
       // }


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

void Movement::UpdateVelocity( float elapsedTime)
{


    // �o�߃t���[��
    float elapsedFrame = 60.0f * elapsedTime;

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
        velocity.x = 0;
        velocity.z = 0;
        // ������������肳���邽��
        //velocity.y = 0;
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
        velocity.y = 0;
    }

}

bool Movement::IsOutOfAreaX(const float& positionX)
{
    if ((positionX + velocity.x) < area.min.x || (positionX + velocity.x) > area.max.x)
    {
        velocity.x = 0;

        return true;
    }

    return false;
}

bool Movement::IsOutOfAreaZ( const float& positionZ)
{
    if ((positionZ + velocity.z) < area.min.z || (positionZ + velocity.z) > area.max.z)
    {
        velocity.z = 0;

        return true;
    }

    return false;
}

bool Movement::IsOutOfAreaY(const float& positionY)
{
    return positionY < area.min.y || positionY > area.max.y;
}
