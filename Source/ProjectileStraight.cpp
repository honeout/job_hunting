#include "ProjectileStraight.h"
#include "Collision.h"
//#include "StageManager.h"
// �R���X�g���N�^
ProjectileStraight::ProjectileStraight()
                   
{
    //model = new Model("Data/Model/Sword/Sword.mdl");

    //// �\���T�C�Y�𒲐�
    //scale.x = scale.y = scale.z = 3.0f;
}
// �f�X�g���N�^
ProjectileStraight::~ProjectileStraight()
{

    //delete model;
}
void ProjectileStraight::Start()
{
    // ���f����l
    model = GetActor()->GetModel();

    
    // �e�ړ��̃R���|�[�l���g
    bulletFiring = GetActor()->GetComponent<BulletFiring>();

    
}
// �X�V����
void ProjectileStraight::Update(float elapsedTime)
{
    // �����܂Ŕ��
    bulletFiring->Move(speed,elapsedTime);
    ////�@�������� 

    //lifeTimer -= elapsedTime;
    //if (lifeTimer <= 0.0f)// �������s�����玩�Q
    //{
    //    // �������폜
    //    Destroy();
    //}
    //// �ړ��@�@�b����Ȃ��ƃt���[����
    //float speed = this->speed * elapsedTime;

    //// �@�����炩���Z�Ō����ɂ��ꂾ���i������ʒu�ɓ��ꂽ
    //position.x += speed * direction.x;
    //position.y += speed * direction.y;
    //position.z += speed * direction.z;

    //UpdateReflection(elapsedTime);

    //// �I�u�W�F�N�g�s����X�V�@�ς�����ʒu�̍s���
    //UpdateTransform();

    //// ���f���s��X�V�@����������炱���Ɍ�����
    //model->UpdateTransform(transform);
}
//// �`�揈��
//void ProjectileStraight::Render(const RenderContext& rc, ModelShader* shader)
//{
//    shader->Draw(rc, model);
//}
//// ����
//void ProjectileStraight::Lanch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3&position)
//{
//    // direction �����@���K�����ē���鍷���Ȃ���X�s�[�h�ς��
//    this->direction = direction;
//    this->position = position;
//}
//
//void ProjectileStraight::UpdateReflection(float elapsedTime)
//{
//    DirectX::XMFLOAT2 velocity = {  this->direction.x * this->speed,   this->direction.z * this->speed };
//    // �������͗ʌv�Z
//    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
//    
//    if (velocityLengthXZ > 0.0f)
//    {
//        // �����ړ��l
//        float mx = velocity.x * elapsedTime;
//        float mz = velocity.y * elapsedTime;
//
//        // ���C�̊J�n�ʒu�ƏI�_�ʒu
//        // �i��������
//        DirectX::XMFLOAT3 start = { position.x,position.y, position.z };
//        // �ړ��������������z
//        DirectX::XMFLOAT3 end = { position.x + mx,position.y, position.z + mz };
//
//        //////////////////////////////////
//
//    //    // ���C�L���X�g�ɂ��ǔ���
//    //    HitResult hit;
//    //    if (StageManager::instance().RayCast(start, end, hit))
//    //    {
//    //           
//    //        // �ǂ܂ł̃x�N�g��
//    //        DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
//    //        DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
//    //        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
//
//    //        // �ǂ̖@��
//    //        DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);
//
//    //        // ���˃x�N�g����@���Ɏˉe�� a
//    //        DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
//    //        Dot = DirectX::XMVectorScale(DirectX::XMVectorScale(Dot, 2.0f), 1.1f); // �ǂɂ߂荞�܂Ȃ��悤�ɏ��������␳
//
//    //       /* DirectX::XMVECTOR Rpositon;*/
//    //        //        // �ǔ���
//    //        //Dot = DirectX::XMVectorAdd(End, Dot);
//
//    //        // �␳�ʒu�̌v�Z
//    //        DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
//    //        DirectX::XMFLOAT3 collectPosition;
//    //        DirectX::XMStoreFloat3(&collectPosition, CollectPosition);
//
//    //        DirectX::XMVECTOR R = DirectX::XMVectorSubtract(CollectPosition, Start);
//    //        DirectX::XMFLOAT3 r; 
//    //        R = DirectX::XMVector3Normalize(R);
//    //        DirectX::XMStoreFloat3(&r, R);
//
//
//    //        direction.x = r.x;
//    //        direction.z = r.z;
//
//    //        // �ǂ�������փ��C�L���X�g
//    //        //HitResult hit2;
//    //        //if (!StageManager::instance().RayCast(hit.position, collectPosition, hit2))
//    //        {
//    //            // �ǂ�������ŕǂɓ�����Ȃ�������␳�ʒu�Ɉړ�
//    //            position.x = collectPosition.x;
//    //            position.z = collectPosition.z;
//
//
//
//    //        }
//    //        //else
//    //        //{
//    //        //    position.x = hit2.position.x;
//    //        //    position.z = hit2.position.z;
//
//    //        //       
//    //        //      
//    //        //}
//    //    }
//    //    else
//    //    {
//    //        // �ړ�
//    //        position.x += mx;
//    //        position.z += mz;
//    //    }
//    //}
//        /////////////////////////////////
//    }
//    //// ���C�L���X�g�ɂ��ǔ���
//    //HitResult hit;
//    //if (StageManager::instance().RayCast(start, end, hit))
//    //{
//    //    // �ǂ܂ł̃x�N�g��
//    //    DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
//    //    DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
//    //    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
//
//    //    // �ǂ̖@��
//    //    DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);
//
//    //    // ���˃x�N�g����@���Ɏˉe�� a
//    //    DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
//    //    Dot = DirectX::XMVectorScale(DirectX::XMVectorScale(Dot, 2.0f), 1.1f); // �ǂɂ߂荞�܂Ȃ��悤�ɏ��������␳
//
//    //    // �ǔ���
//    //    Dot = DirectX::XMVectorAdd(End, Dot);
//
//
//    //    // �␳�ʒu�̌v�Z
//    //    DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
//    //    DirectX::XMFLOAT3 collectPosition;
//    //    DirectX::XMStoreFloat3(&collectPosition, CollectPosition);
//
//
// //           // �ǂ�������փ��C�L���X�g
// //           HitResult hit2;
// //           if (!StageManager::instance().RayCast( hit.position, collectPosition, hit2))
// //           {
// //               // �ǂ�������ŕǂɓ�����Ȃ�������␳�ʒu�Ɉړ�
// ///*               position.x = collectPosition.x;
// //               position.z = collectPosition.z;*/
// //              /* DirectX::XMStoreFloat3(&direction,Dot);*/
// //               
// //               //position.x = hit.position.x;
// //               //position.z = hit.position.z;
// //               //direction.x = sinf(hit2.rotation.y);
// //               //direction.z = cosf(hit2.rotation.y);
//
//
//
// //           }
// //           else
// //           {
// //               //DirectX::XMVECTOR Start2 = DirectX::XMLoadFloat3(&hit.position);
// //               //DirectX::XMVECTOR End2 = DirectX::XMLoadFloat3(&collectPosition);
// //               //DirectX::XMVECTOR Vector2 = DirectX::XMVectorSubtract(End2, Start2);
// //               //Vector2 = DirectX::XMVector3Normalize(Vector2);
//
// //               //DirectX::XMFLOAT3 vector2;
// //               //DirectX::XMStoreFloat3(&vector2,Vector2);
//
// //               //position.x = hit.position.x;
// //               //position.z = hit.position.z;
//
// //               direction.x = 0;
// //               direction.z = 0;
//
// //           }
// //       }
// //       else
// //       {
// //           // �ړ�
// //           position.x += mx;
// //           position.z += mz;
// //       }
// //   }
//}



