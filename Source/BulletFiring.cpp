#include <imgui.h>
#include "BulletFiring.h"
#include "ModelControll.h"
#include "ProjectileManager.h"

void BulletFiring::Move(float speed,float elapsedTime)
{
    //�@�������� 
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)// �������s�����玩�Q
    {
        // �������폜
        Destroy();
       

    }
    ProjectileManager::Instance().DeleteUpdate(elapsedTime);
    // �ړ��@�@�b����Ȃ��ƃt���[����
    float bulletspeed = speed * elapsedTime;

    // �@�����炩���Z�Ō����ɂ��ꂾ���i������ʒu�ɓ��ꂽ
    position.x += bulletspeed * direction.x;
    position.y += bulletspeed * direction.y;
    position.z += bulletspeed * direction.z;



    GetActor()->GetComponent<Transform>()->SetPosition(position);
    
    //transformid->UpdateTransform();

    //GetActor()->GetModel()->UpdateAnimation(elapsedTime, true);


   // model->UpdateTransform(transformid->GetTransform());
    //GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());
    // ����
    //UpdateReflection(elapsedTime);

    // �I�u�W�F�N�g�s����X�V�@�ς�����ʒu�̍s���
    //UpdateTransform();

    // ���f���s��X�V�@����������炱���Ɍ�����
    //model->UpdateTransform(transform);
}

void BulletFiring::MoveHoming(float speed, float turnSpeed, DirectX::XMFLOAT3 target, float elapsedTime)
{
    //�@�������� 
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)// �������s�����玩�Q
    {
        // �������폜
        Destroy();
        
        
    }
    ProjectileManager::Instance().DeleteUpdate(elapsedTime);
    // �ړ��@�@�b����Ȃ��ƃt���[����
    float bulletspeed = speed * elapsedTime;

    // �@�����炩���Z�Ō����ɂ��ꂾ���i������ʒu�ɓ��ꂽ
    position.x += bulletspeed * direction.x;
    position.y += bulletspeed * direction.y;
    position.z += bulletspeed * direction.z;


    // ����
    {
        float bulletTurnSpeed = turnSpeed * elapsedTime;

        // �^�[�Q�b�g�܂ł̃x�N�g�����Z�o
        DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);

        // �[���x�N�g���łȂ��Ȃ��]�����@�s�b�^�������Ȃ��]�ł��邩��m�F
        DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
        float lengthSq;
        DirectX::XMStoreFloat(&lengthSq, LengthSq);

      

        if (lengthSq > rangeMin && lengthSq <= rangeMax)
        {

            // �^�[�Q�b�g�܂ł̃x�N�g����P�ʃx�N�g����
            Vec = DirectX::XMVector3Normalize(Vec);

            // �����Ă�����x�N�g�����Z�o�@direction�P�ʃx�N�g���O��
            DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);


            // �O�������x�N�g���ƃ^�[�Q�b�g�܂ł̃x�N�g���̓��ρi�p�x�j���Z�o
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);



            float dot;
            DirectX::XMStoreFloat(&dot, Dot);

            // ��]���x�����Ō�̔������s���߂��Ȃ��悤�ɏ������������o����B
            // �A�[�N�R�T�C���ł��o���邵������u�ł��̒l������B
            // 2�̒P�ʃx�N�g���̊p�x����������
            // 1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
            //if (dot )
            float rot;
            // 1.0f�@dot�͂O�ɋ߂Â����P�ɂȂ邩��[���Ƃ��������O�ɂȂ�
            rot = 1.0f - dot;
            // ������P�̂ق����ł�����΃X�s�[�h������
            // �^�[���X�s�[�h��胍�b�g�̕�������������
            if (rot > bulletTurnSpeed)
            {
                rot = bulletTurnSpeed;
            }


            // ��]�p�x������Ȃ��]��������@�����łO������ĂȂ��ƊO�ς��S�������ɂȂ��Čv�Z�o���Ȃ�
            if (fabsf(rot) >= 0.0001)
            {
                // ��]�����Z�o  �O��  ���������������� 
                DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction, Vec);
                // �덷�h�~�ׂ̈ɒP�ʃx�N�g�������������S
                Axis = DirectX::XMVector3Normalize(Axis);
                // ��]���Ɖ�]�ʂ����]�s����Z�o ��]�ʂ����߂Ă���B
                DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);


               

                // ���݂̍s�����]������@�������g�̎p��
                DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transformid->GetTransform());
                Transform = DirectX::XMMatrixMultiply(Transform, Rotation); // ���������炽���~����  Transform*Rotation
                // DirectX::XMMatrixMultrixMultiply
                // ��]��̑O�����������o���A�P�ʃx�N�g��������
                Direction = DirectX::XMVector3Normalize(Transform.r[2]);// row
                DirectX::XMStoreFloat3(&direction, Direction);


                //DirectX::XMStoreFloat4x4(&transform, Transform);


               

            }


              
            
        }
    }

    transformid->SetDirection(direction);
    transformid->SetPosition(position);

    //transformid->UpdateTransformProjectile();

    //GetActor()->GetModel()->UpdateAnimation(elapsedTime, true);



  
            //GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());
          
}

void BulletFiring::JumpOut(float speed, float turnSpeed, DirectX::XMFLOAT3 target, float elapsedTime)
{
    //�@�������� 
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)// �������s�����玩�Q
    {
        // �������폜
        //PushDown(speed, turnSpeed, elapsedTime);
        Destroy();

    }
    else
    {

        ProjectileManager::Instance().DeleteUpdate(elapsedTime);
        // �ړ��@�@�b����Ȃ��ƃt���[����
        float bulletspeed = speed * elapsedTime;



        // �@�����炩���Z�Ō����ɂ��ꂾ���i������ʒu�ɓ��ꂽ
        //if (direction.x >= 0.4f)
        position.x += bulletspeed * direction.x;
        position.y += bulletspeed * direction.y;
        position.z += bulletspeed * direction.z;


        if (position.y <= -3.5f)
            transformid->SetPosition(position);
    }
}

void BulletFiring::PushDown(float speed, float turnSpeed,  float elapsedTime)
{
    ////�@�������� 
    //lifeTimer -= elapsedTime;
    //if (lifeTimer <= 0.0f)// �������s�����玩�Q
    //{
    //    // �������폜
    //    Destroy();


    //}

    ProjectileManager::Instance().DeleteUpdate(elapsedTime);
    // �ړ��@�@�b����Ȃ��ƃt���[����
    float bulletspeed = speed * elapsedTime;



    // �@�����炩���Z�Ō����ɂ��ꂾ���i������ʒu�ɓ��ꂽ
    //if (direction.x >= 0.4f)
    //    position.x -= bulletspeed * direction.x;
    //if (direction.y >= 0.4f)
    //    position.y -= bulletspeed * direction.y;
    //if (direction.z >= 0.4f)
    //    position.z -= bulletspeed * direction.z;
    position.x -= bulletspeed * direction.x;
    position.y -= bulletspeed * direction.y;
    position.z -= bulletspeed * direction.z;


    transformid->SetPosition(position);
        if (position.y <= -5.5f)
            Destroy();
}

void BulletFiring::Sunder(DirectX::XMFLOAT3 target,  float elapsedTime)
{
    //�@�������� 
lifeTimer -= elapsedTime;
if (lifeTimer <= 0.0f)// �������s�����玩�Q
{
    // �������폜
    Destroy();


}

DirectX::XMFLOAT3 pos;
pos =
{
    target.x,
    target.y,
    target.z

};
transformid->SetPosition(pos);
//float bulletspeed = speed * elapsedTime;

}


void BulletFiring::Throwing(float speed, float turnSpeed, DirectX::XMFLOAT3 target, bool turnPermission,float elapsedTime)
{
    //�@�������� 
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)// �������s�����玩�Q
    {
        // �������폜
        Destroy();


    }

    ProjectileManager::Instance().DeleteUpdate(elapsedTime);

    // �ړ��@�@�b����Ȃ��ƃt���[����
    float bulletspeed = speed * elapsedTime;

    // �@�����炩���Z�Ō����ɂ��ꂾ���i������ʒu�ɓ��ꂽ

    // ����
    if (turnPermission)
    {
        float bulletTurnSpeed = turnSpeed * elapsedTime;

        // �^�[�Q�b�g�܂ł̃x�N�g�����Z�o
        DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);

        // �[���x�N�g���łȂ��Ȃ��]�����@�s�b�^�������Ȃ��]�ł��邩��m�F
        DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
        float lengthSq;
        DirectX::XMStoreFloat(&lengthSq, LengthSq);



        if (lengthSq > 0.00001f)
        {

            // �^�[�Q�b�g�܂ł̃x�N�g����P�ʃx�N�g����
            Vec = DirectX::XMVector3Normalize(Vec);

            // �����Ă�����x�N�g�����Z�o�@direction�P�ʃx�N�g���O��
            DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);


            // �O�������x�N�g���ƃ^�[�Q�b�g�܂ł̃x�N�g���̓��ρi�p�x�j���Z�o
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);



            float dot;
            DirectX::XMStoreFloat(&dot, Dot);

            // ��]���x�����Ō�̔������s���߂��Ȃ��悤�ɏ������������o����B
            // �A�[�N�R�T�C���ł��o���邵������u�ł��̒l������B
            // 2�̒P�ʃx�N�g���̊p�x����������
            // 1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
            //if (dot )
            float rot;
            // 1.0f�@dot�͂O�ɋ߂Â����P�ɂȂ邩��[���Ƃ��������O�ɂȂ�
            rot = 1.0f - dot;
            // ������P�̂ق����ł�����΃X�s�[�h������
            // �^�[���X�s�[�h��胍�b�g�̕�������������
            if (rot > bulletTurnSpeed)
            {
                rot = bulletTurnSpeed;
            }


            // ��]�p�x������Ȃ��]��������@�����łO������ĂȂ��ƊO�ς��S�������ɂȂ��Čv�Z�o���Ȃ�
            if (fabsf(rot) >= 0.0001)
            {
                // ��]�����Z�o  �O��  ���������������� 
                DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction, Vec);
                // �덷�h�~�ׂ̈ɒP�ʃx�N�g�������������S
                Axis = DirectX::XMVector3Normalize(Axis);
                // ��]���Ɖ�]�ʂ����]�s����Z�o ��]�ʂ����߂Ă���B
                DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);




                // ���݂̍s�����]������@�������g�̎p��
                DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transformid->GetTransform());
                Transform = DirectX::XMMatrixMultiply(Transform, Rotation); // ���������炽���~����  Transform*Rotation
                // DirectX::XMMatrixMultrixMultiply
                // ��]��̑O�����������o���A�P�ʃx�N�g��������
                Direction = DirectX::XMVector3Normalize(Transform.r[2]);// row
                DirectX::XMStoreFloat3(&direction, Direction);


                //DirectX::XMStoreFloat4x4(&transform, Transform);








            }




        }

    }
    float dirX = 0;

    if (position.y <= -1)
        direction.x += 0.0001f;

    if (position.y >= 0)
        direction.x += -0.01f;

    transformid->SetDirection(direction);




    position.x += bulletspeed * direction.x;
    position.y += bulletspeed * direction.y;
    position.z += bulletspeed * direction.z;

    // if (position.y <= 0)
    transformid->SetPosition(position);

}

//void BulletFiring::Throwing(float speed, float turnSpeed, DirectX::XMFLOAT3 target, float elapsedTime)
//{
//    //�@�������� 
//    lifeTimer -= elapsedTime;
//    if (lifeTimer <= 0.0f)// �������s�����玩�Q
//    {
//        // �������폜
//        Destroy();
//
//
//    }
//
//    ProjectileManager::Instance().DeleteUpdate(elapsedTime);
//
//
//
//
//
//    // �ړ��@�@�b����Ȃ��ƃt���[����
//    float bulletspeed = speed * elapsedTime;
//
//    // �@�����炩���Z�Ō����ɂ��ꂾ���i������ʒu�ɓ��ꂽ
//
//
//
//    // ����
//    {
//        float bulletTurnSpeed = turnSpeed * elapsedTime;
//
//        // �^�[�Q�b�g�܂ł̃x�N�g�����Z�o
//        DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
//        DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
//        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);
//
//        // �[���x�N�g���łȂ��Ȃ��]�����@�s�b�^�������Ȃ��]�ł��邩��m�F
//        DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
//        float lengthSq;
//        DirectX::XMStoreFloat(&lengthSq, LengthSq);
//
//
//
//        if (lengthSq > 0.00001f)
//        {
//
//            // �^�[�Q�b�g�܂ł̃x�N�g����P�ʃx�N�g����
//            Vec = DirectX::XMVector3Normalize(Vec);
//
//            // �����Ă�����x�N�g�����Z�o�@direction�P�ʃx�N�g���O��
//            DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
//
//
//            // �O�������x�N�g���ƃ^�[�Q�b�g�܂ł̃x�N�g���̓��ρi�p�x�j���Z�o
//            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);
//
//
//
//            float dot;
//            DirectX::XMStoreFloat(&dot, Dot);
//
//            // ��]���x�����Ō�̔������s���߂��Ȃ��悤�ɏ������������o����B
//            // �A�[�N�R�T�C���ł��o���邵������u�ł��̒l������B
//            // 2�̒P�ʃx�N�g���̊p�x����������
//            // 1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
//            //if (dot )
//            float rot;
//            // 1.0f�@dot�͂O�ɋ߂Â����P�ɂȂ邩��[���Ƃ��������O�ɂȂ�
//            rot = 1.0f - dot;
//            // ������P�̂ق����ł�����΃X�s�[�h������
//            // �^�[���X�s�[�h��胍�b�g�̕�������������
//            if (rot > bulletTurnSpeed)
//            {
//                rot = bulletTurnSpeed;
//            }
//
//
//            // ��]�p�x������Ȃ��]��������@�����łO������ĂȂ��ƊO�ς��S�������ɂȂ��Čv�Z�o���Ȃ�
//            if (fabsf(rot) >= 0.0001)
//            {
//                // ��]�����Z�o  �O��  ���������������� 
//                DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction, Vec);
//                // �덷�h�~�ׂ̈ɒP�ʃx�N�g�������������S
//                Axis = DirectX::XMVector3Normalize(Axis);
//                // ��]���Ɖ�]�ʂ����]�s����Z�o ��]�ʂ����߂Ă���B
//                DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);
//
//
//
//
//                // ���݂̍s�����]������@�������g�̎p��
//                DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transformid->GetTransform());
//                Transform = DirectX::XMMatrixMultiply(Transform, Rotation); // ���������炽���~����  Transform*Rotation
//                // DirectX::XMMatrixMultrixMultiply
//                // ��]��̑O�����������o���A�P�ʃx�N�g��������
//                Direction = DirectX::XMVector3Normalize(Transform.r[2]);// row
//                DirectX::XMStoreFloat3(&direction, Direction);
//
//
//                //DirectX::XMStoreFloat4x4(&transform, Transform);
//
//
//             
//
//
//
//
//
//            }
//
//
//
//
//        }
//        
//    }
//    float dirX = 0;
//
//    if (position.y <= -1)
//        direction.x += 0.0001f;
//
//    if (position.y >= 0)
//        direction.x += -0.01f;
//
//    transformid->SetDirection(direction);
//
//
//
//
//    position.x += bulletspeed * direction.x;
//    position.y += bulletspeed * direction.y;
//    position.z += bulletspeed * direction.z;
//
//   // if (position.y <= 0)
//    transformid->SetPosition(position);
//    
//}

void BulletFiring::Lanch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float   lifeTimer)
{
    // direction �����@���K�����ē���鍷���Ȃ���X�s�[�h�ς��
    this->direction = direction;
    this->position = position;
    this->lifeTimer = lifeTimer;

    model = GetActor()->GetComponent<ModelControll>()->GetModel();

    // �g�����X�t�H�[���擾
    transformid = GetActor()->GetComponent<Transform>();

    transformid->SetDirection(direction);

}

void BulletFiring::Destroy()
{
    ActorManager::Instance().Remove(GetActor());
    ProjectileManager::Instance().Remove(GetActor());
   
}

void BulletFiring::OnGUI()
{
    ImGui::InputFloat3("Move Speed", &position.x);
    ImGui::InputFloat3("direction", &direction.x);
    ImGui::InputFloat("lifeTimer", &lifeTimer);
    

}
