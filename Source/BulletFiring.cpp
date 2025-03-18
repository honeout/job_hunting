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
}
// ��]
bool BulletFiring::Turn(float turnspeed,const DirectX::XMFLOAT3 &target, float elapedTime)
{
   std::weak_ptr<Actor> actor = GetActor();
   std::weak_ptr<Transform> transformid = GetActor()->GetComponent<Transform>();
    DirectX::XMFLOAT3 angle = actor.lock()->GetComponent<Transform>()->GetAngle();
    //// ���ʂ̌����x�N�g��
    direction.x = sinf(angle.y);// �O�p���΂߂ɂ��Ĉʒu��ς���
    direction.y = sinf(angle.x);
    direction.z = cosf(angle.y);
    float bulletTurnSpeed = turnspeed * elapedTime;
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
            DirectX::XMMATRIX Transformmid = DirectX::XMLoadFloat4x4(&transformid.lock()->GetTransform());
            Transformmid = DirectX::XMMatrixMultiply(Transformmid, Rotation); // ���������炽���~����  Transform*Rotation
            //// DirectX::XMMatrixMultrixMultiply
            //// ��]��̑O�����������o���A�P�ʃx�N�g��������
            Direction = DirectX::XMVector3Normalize(Transformmid.r[2]);// row
            DirectX::XMStoreFloat3(&direction, Direction);
            actor.lock()->GetComponent<Transform>()->SetDirection(direction);
            return true;
        }
    }
    return false;
}

bool BulletFiring::Turn2D(float speed, DirectX::XMFLOAT3& direction, float elapedTime)
{
    std::shared_ptr<Actor> actor = GetActor();
    DirectX::XMFLOAT3 angle = GetActor()->GetComponent<Transform>()->GetAngle();
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
    //// ���ʂ̌����x�N�g��
    this->direction.x = sinf(angle.y);// �O�p���΂߂ɂ��Ĉʒu��ς���
    //this->direction.y = get;
    this->direction.z = cosf(angle.y);
    GetActor()->GetComponent<Transform>()->SetDirection(this->direction);
    if (dot >= 0.93f)
    {
        return true;
    }
    else if (dot <= 0.1f)
    {
        return true;
    }
    return false;
}

void BulletFiring::MoveHoming(float speed, float turnSpeed, DirectX::XMFLOAT3 target, float elapsedTime)
{
    std::weak_ptr<Transform> transformid = GetActor()->GetComponent<Transform>();
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
            float rot;
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
                DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transformid.lock()->GetTransform());
                Transform = DirectX::XMMatrixMultiply(Transform, Rotation); // ���������炽���~����  Transform*Rotation
                // ��]��̑O�����������o���A�P�ʃx�N�g��������
                Direction = DirectX::XMVector3Normalize(Transform.r[2]);// row
                DirectX::XMStoreFloat3(&direction, Direction);
            }
        }
    }
    transformid.lock()->SetDirection(direction);
    transformid.lock()->SetPosition(position);
}

void BulletFiring::JumpOut(float speed, float turnSpeed, DirectX::XMFLOAT3 target, float elapsedTime)
{
    std::weak_ptr<Transform> transformid = GetActor()->GetComponent<Transform>();
    //�@�������� 
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)// �������s�����玩�Q
    {
        Destroy();
    }
    else
    {
        ProjectileManager::Instance().DeleteUpdate(elapsedTime);
        // �ړ��@�@�b����Ȃ��ƃt���[����
        float bulletspeed = speed * elapsedTime;
        // �@�����炩���Z�Ō����ɂ��ꂾ���i������ʒu�ɓ��ꂽ
        position.x += bulletspeed * direction.x;
        position.y += bulletspeed * direction.y;
        position.z += bulletspeed * direction.z;
        if (position.y <= -3.5f)
            transformid.lock()->SetPosition(position);
    }
}

void BulletFiring::PushDown(float speed, float turnSpeed,  float elapsedTime)
{
    std::weak_ptr<Transform> transformid = GetActor()->GetComponent<Transform>();
    ProjectileManager::Instance().DeleteUpdate(elapsedTime);
    // �ړ��@�@�b����Ȃ��ƃt���[����
    float bulletspeed = speed * elapsedTime;
    position.x -= bulletspeed * direction.x;
    position.y -= bulletspeed * direction.y;
    position.z -= bulletspeed * direction.z;
    transformid.lock()->SetPosition(position);
        if (position.y <= -5.5f)
            Destroy();
}

void BulletFiring::Sunder(DirectX::XMFLOAT3 target, float elapsedTime)
{
    std::weak_ptr<Transform> transformid = GetActor()->GetComponent<Transform>();
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
    transformid.lock()->SetPosition(pos);
}

void BulletFiring::Throwing(float speed, float turnSpeed, DirectX::XMFLOAT3 target, bool turnPermission,float elapsedTime)
{
    std::weak_ptr<Transform> transformid = GetActor()->GetComponent<Transform>();
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
                DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transformid.lock()->GetTransform());
                Transform = DirectX::XMMatrixMultiply(Transform, Rotation); // ���������炽���~����  Transform*Rotation
                // DirectX::XMMatrixMultrixMultiply
                // ��]��̑O�����������o���A�P�ʃx�N�g��������
                Direction = DirectX::XMVector3Normalize(Transform.r[2]);// row
                DirectX::XMStoreFloat3(&direction, Direction);
            }
        }

    }
    float dirX = 0;
    if (position.y <= -1)
        direction.x += 0.0001f;
    if (position.y >= 0)
        direction.x += -0.01f;
    transformid.lock()->SetDirection(direction);
    position.x += bulletspeed * direction.x;
    position.y += bulletspeed * direction.y;
    position.z += bulletspeed * direction.z;
    // if (position.y <= 0)
    transformid.lock()->SetPosition(position);
}

void BulletFiring::Lanch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float   lifeTimer)
{
    // �g�����X�t�H�[���擾
    std::weak_ptr<Transform> transformid = GetActor()->GetComponent<Transform>();
    // direction �����@���K�����ē���鍷���Ȃ���X�s�[�h�ς��
    this->direction = direction;
    this->position = position;
    this->lifeTimer = lifeTimer;
    model = GetActor()->GetComponent<ModelControll>()->GetModel();
    transformid.lock()->SetDirection(direction);
}

void BulletFiring::Destroy()
{
    ActorManager::Instance().Remove(GetActor());
    ProjectileManager::Instance().Remove(GetActor());
   
}
#ifdef _DEBUG
void BulletFiring::OnGUI()
{
    ImGui::InputFloat3("Move Speed", &position.x);
    ImGui::InputFloat3("direction", &direction.x);
    ImGui::InputFloat("lifeTimer", &lifeTimer);
    

}
#endif // _DEBUG