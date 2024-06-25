#include <imgui.h>
#include "BulletFiring.h"

void BulletFiring::Move(float speed,float elapsedTime)
{
    //�@�������� 
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)// �������s�����玩�Q
    {
        // �������폜
        Destroy();
    }
    // �ړ��@�@�b����Ȃ��ƃt���[����
    float bulletspeed = speed * elapsedTime;

    // �@�����炩���Z�Ō����ɂ��ꂾ���i������ʒu�ɓ��ꂽ
    position.x += bulletspeed * direction.x;
    position.y += bulletspeed * direction.y;
    position.z += bulletspeed * direction.z;

    GetActor()->SetPosition(position);
    
    GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());
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

        GetActor()->SetPosition(position);

        if (lengthSq > 0.00001f)
        {

            // �^�[�Q�b�g�܂ł̃x�N�g����P�ʃx�N�g����
            Vec = DirectX::XMVector3Normalize(Vec);

            // �����Ă�����x�N�g�����Z�o�@direction�P�ʃx�N�g���O��
            DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);

            // �P�ʉ�����
            Direction = DirectX::XMVector3Normalize(Direction);

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


                DirectX::XMFLOAT4X4 transform = GetActor()->GetTransform();

                // ���݂̍s�����]������@�������g�̎p��
                DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
                Transform = DirectX::XMMatrixMultiply(Transform, Rotation); // ���������炽���~����  Transform*Rotation
                // DirectX::XMMatrixMultrixMultiply
                // ��]��̑O�����������o���A�P�ʃx�N�g��������
                Direction = DirectX::XMVector3Normalize(Transform.r[2]);// row
                DirectX::XMStoreFloat3(&direction, Direction);
                

                DirectX::XMStoreFloat4x4(&transform, Transform);


                //DirectX::XMStoreFloat3(&position, Transform.r[3]);

                GetActor()->SetTransform(transform);

                //GetActor()->SetPosition();
            }

            GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());

                //DirectX::XMFLOAT3 vec;

                //DirectX::XMStoreFloat3(&vec, Vec);

                ////// ����
                ////float length = sqrtf(vec.x * vec.x * vec.z * vec.z);
                ////// �P�ʉ��x�N�g��
                ////vec.x /= length;
                ////vec.z /= length;

                //float cross = (direction.z * vec.x) + (direction.x * vec.z);


                //DirectX::XMFLOAT4 rotate;
                //rotate = GetActor()->GetRotation();

                //if (cross < 0.0f)
                //{
                //    rotate.y -= rot;
                //}
                //else
                //{
                //    rotate.y += rot;
                //}

                //direction.x = sinf(rotate.y);
                //direction.y = 0;
                //direction.z = cosf(rotate.y);

                ////DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
                ////Direction = DirectX::XMVector3Normalize(Direction);

                ////DirectX::XMStoreFloat3(&direction, Direction);

                //GetActor()->SetRotation(rotate);

                //GetActor()->GetModel()->UpdateTransform(transform);

                //GetActor()->SetRotation({
                //    transform._21,
                //    transform._22,
                //    transform._23,
                //    transform._24 });
                //
                //DirectX::XMFLOAT3 homingdirection;
                //homingdirection = {
                //    transform._41 - position.x,
                //    transform._42 - position.y,
                //    transform._43 - position.z
                //};

                //// �ړ�
                //position.x += bulletspeed * homingdirection.x;
                //position.y += bulletspeed * homingdirection.y;
                //position.z += bulletspeed * homingdirection.z;


                //position = {
                //    transform._11,
                //    transform._12,
                //    transform._13

                //};

                //float cross;
                //DirectX::XMStoreFloat(&cross, Axis);

                //DirectX::XMFLOAT4 rotate = GetActor()->GetRotation();

                //

                //if (cross < 0.0f)
                //{
                //    rotate.y -= rot;
                //}
                //else
                //{
                //    rotate.y += rot;
                //}

                //GetActor()->SetRotation(rotate);

            //}
            
        }
    }

    

}

void BulletFiring::Lanch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float   lifeTimer)
{
    // direction �����@���K�����ē���鍷���Ȃ���X�s�[�h�ς��
    this->direction = direction;
    this->position = position;
    this->lifeTimer = lifeTimer;
}

void BulletFiring::Destroy()
{
    ActorManager::Instance().Remove(GetActor());
}

void BulletFiring::OnGUI()
{
    ImGui::InputFloat3("Move Speed", &position.x);
    ImGui::InputFloat3("direction", &direction.x);
    ImGui::InputFloat("lifeTimer", &lifeTimer);

}
