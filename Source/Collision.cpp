#include "Collision.h"
#ifdef _DEBUG
void Collision::OnGUI()
{
}
#endif // _DEBUG

// ���Ƌ��̌�������
    bool Collision::IntersectSpherVsSphere(
    const DirectX::XMFLOAT3& positionA,
    float radiusA,
    const DirectX::XMFLOAT3& positionB,
    float radiusB,
    DirectX::XMFLOAT3& outPositionB
)
{
    // A->B�̒P�ʃx�N�g�����Z�o
    DirectX::XMVECTOR PositionA = DirectX::XMLoadFloat3(&positionA);
    DirectX::XMVECTOR PositionB = DirectX::XMLoadFloat3(&positionB);
    DirectX::XMVECTOR Vec =  DirectX::XMVectorSubtract(PositionB, PositionA);
    DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);// ��O������ 2��̂܂�
    float lengthSq;
    DirectX::XMStoreFloat(&lengthSq, LengthSq);
    // ��������
    float range = radiusA+radiusB;
    if (lengthSq > range*range)// �����̔�r�@2�擯�m�Ȃ�s����
    {
        // ����Ă��邩�� ��΂�
        return false;
    }
    // A��B�������o��
    Vec = DirectX::XMVector3Normalize(Vec);
    // ����lengthSq�������~
    Vec = DirectX::XMVectorScale(Vec, range);
    // a�̃|�W�V����������𑫂��Ƃ�������L�т錋�ʂ����ɓ����
    PositionB = DirectX::XMVectorAdd(PositionA, Vec);
    // �����outPositionB�ɓ����
    DirectX::XMStoreFloat3(&outPositionB, PositionB);
    return true;
}

// �~���Ɖ~���̓����蔻��
bool Collision::IntersectCylinderVsCylinder(
    const DirectX::XMFLOAT3& positionA, 
    float radiusA, 
    float heightA,
    const DirectX::XMFLOAT3& positionB,
    float radiusB,
    float heightB,
    DirectX::XMFLOAT3& outPositionB)
{
    // A�̑�����B�̓���肤���Ȃ瓖�����Ă��Ȃ�
    if (positionA.y > heightB + positionB.y)
    {
        return false;
    }
    // A�̓���B�̑�����肵���Ȃ瓖�����Ă��Ȃ�
    if (positionA.y + heightA < positionB.y)
    {
        return false;
    }
    // XZ���ʂł͈̔̓`�F�b�N
    float vx = positionB.x - positionA.x;//B-A�ł�a->B
    float vz = positionB.z - positionA.z;// ���P�ʃx�N�g��
    float range = radiusA + radiusB;// �ǂꂾ�������o��
    float distXZ = sqrtf(vx * vx + vz * vz); // 
    if (distXZ > range)
    {
        return false;
    }
    float rangeAdd = 0.001f;
    //A��B�������o��
    vx /= distXZ;
    vz /= distXZ;
    outPositionB.x = positionA.x + (vx * (range + rangeAdd));
    outPositionB.y = positionB.y;
    outPositionB.z = positionA.z + (vz * (range + rangeAdd));
    return true;
}

bool Collision::IntersectCylinderWidthVsCylinderWidth(const DirectX::XMFLOAT3& positionA, float radiusA, float widthA, const DirectX::XMFLOAT3& positionB, float radiusB, float widthB, DirectX::XMFLOAT3& outPositionB)
{
    // A�̑�����B�̓���肤���Ȃ瓖�����Ă��Ȃ�
    if (positionA.y > widthB + positionB.y)
    {
        return false;
    }
    // A�̓���B�̑�����肵���Ȃ瓖�����Ă��Ȃ�
    if (positionA.y + widthA < positionB.y)
    {
        return false;
    }
    // XZ���ʂł͈̔̓`�F�b�N
    float vx = positionB.x - positionA.x;//B-A�ł�a->B
    float vz = positionB.z + widthB - positionA.z + widthA;// ���P�ʃx�N�g��
    float range = radiusA + radiusB;// �ǂꂾ�������o��
    float distXZ = sqrtf(vx * vx + vz * vz); // 
    if (distXZ > range)
    {
        return false;
    }
    // ���̒[������a��菬����������A�E�g
    vz = positionB.z - widthB > positionA.z - widthA;
    distXZ = sqrtf(vx * vx + vz * vz);
    if (distXZ > range)
    {
        return false;
    }
    //A��B�������o��
    vx /= distXZ;
    vz /= distXZ;
    outPositionB.x = positionA.x + (vx * range);
    outPositionB.y = positionB.y;
    outPositionB.z = positionA.z + (vz * range);
    return true;
}

 // ���Ɖ~���̌�������
bool Collision::IntersectSphereVsCylinder(
     const DirectX::XMFLOAT3& spherePosition,
     float sphereRadius,
     const DirectX::XMFLOAT3& cylinderPosition,
     float cylinderRadius,
     float cylinderHeight,
     DirectX::XMFLOAT3& outCylinderPosition)
 {
         
     // �����`�F�b�N
     if (spherePosition.y + sphereRadius < cylinderPosition.y)return false;
     if (spherePosition.y - sphereRadius > cylinderPosition.y + cylinderHeight)return false;
     // XZ���ʂł͈̔̓`�F�b�N
     float vx = cylinderPosition.x - spherePosition.x;
     float vz = cylinderPosition.z - spherePosition.z;
     float range = sphereRadius + cylinderRadius;
     float distXZ = sqrtf(vx * vx + vz * vz);
     if (distXZ > range)return false;
     // �e���~���������o��
     vx /= distXZ;
     vz /= distXZ;
     outCylinderPosition.x = spherePosition.x + (vx * range);
     outCylinderPosition.x = cylinderPosition.y;
     outCylinderPosition.x = spherePosition.z + (vz * range);
     // �~���̒�ʂ��܂ޕ��ʂƋ��̌����ɂƂ��ďo�����B
     // �~�̔��a�̂Q�悪�����܂��B
     float Radius;
     Radius = sqrtf(sphereRadius * sphereRadius);
     if (sphereRadius < Radius &&
         cylinderRadius < Radius)
     {
         return true;
      }

     return true;
 }
 // ���C�ƃ��f���̌�������
bool Collision::IntersectRayVsModel(
    // ���C�̔��˓_
    const DirectX::XMFLOAT3& start,
    // ���C�̏I���_�d������{���͖��������L����
    const DirectX::XMFLOAT3& end,
    const Model* model,
    // ���C�����������|�C���g��������葼�̎����ł̏������
    HitResult& result)
{
    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
    //�@�����x�N�g���@���C
    DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
    // ���C����
    DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);
    // ���[���h��Ԃ̃��C�̒���
    DirectX::XMStoreFloat(&result.distance, WorldRayLength);
    bool hit = false;
    // ���b�V���̏��
    const ModelResource* resource = model->GetResource();
    for (const ModelResource::Mesh& mesh : resource->GetMeshes())
    {
        // ���b�V���m�[�h�擾
        const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);
        // ���C�����[���h��Ԃ��烍�[�J����Ԃ֕ϊ�
        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.worldTransform);
        DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);
        // �t�s����g���Ίy�ɍs����
        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
        DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
        DirectX::XMVECTOR Dir = DirectX::XMVector3Normalize(SE);
        DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);
        // ���C�̒���
        float neart;
        DirectX::XMStoreFloat(&neart, Length);
        // �O�p�`(��)�Ƃ̌�������
        const std::vector<ModelResource::Vertex>& vertices = mesh.vertices;
        const std::vector<UINT> indices = mesh.indices;
        int materialIndex = -1;
        DirectX::XMVECTOR HitPosition;
        DirectX::XMVECTOR HitNormal;
        for (const ModelResource::Subset& subset : mesh.subsets)
        {
            for (UINT i = 0; i < subset.indexCount; i += 3)
            {
                UINT index = subset.startIndex + i;
                // �������b�V���̒��ɂ��镡���T�u�Z�b�g�̒��ɕ����̒��_�����邩����o���B
                // �O�p�`�̒��_�𒊏o
                const ModelResource::Vertex& a = vertices.at(indices.at(index));
                const ModelResource::Vertex& b = vertices.at(indices.at((index + 1) % UINT_MAX));
                const ModelResource::Vertex& c = vertices.at(indices.at((index + 2) % UINT_MAX));
                DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
                DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
                DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);
                // �O�p�`�̎O�Ӄx�N�g�����Z�o�@���v���
                DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
                DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
                DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);
                // �O�p�`�̖@���x�N�g�����Z�o �����ȃx�N�g��
                DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB, BC);
                // ���ς̌��ʂ��v���X�Ȃ�Ε\����
                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Dir, N);
                float dot;
                DirectX::XMStoreFloat(&dot, Dot);
                if (dot >= 0)continue;
                // ���C�ƕ��ʂ̌�_���Z�o
                DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(A, S);
                DirectX::XMVECTOR T = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), Dot);
                float t;
                DirectX::XMStoreFloat(&t, T);
                // neart�Ƃ͕������_������Ƃ��ł��߂����̈ȊO��؂�B
                if (t < .0f || t > neart) continue; // ��_�܂ł̋��������܂łɌv�Z�����ŋߋ������
                DirectX::XMVECTOR P = DirectX::XMVectorAdd(DirectX::XMVectorMultiply(Dir, T), S);
                // ��_���O�p�`�̓����ɂ��邩����
                // 1��
                DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(A, P);
                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
                float dot1;
                DirectX::XMStoreFloat(&dot1, Dot1);
                if (dot1 < 0.0f)continue;
                // 2��
                DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(B, P);
                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
                float dot2;
                DirectX::XMStoreFloat(&dot2, Dot2);
                if (dot2 < 0.0f)continue;
                // 3��
                DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(C, P);
                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);
                float dot3;
                DirectX::XMStoreFloat(&dot3, Dot3);
                if (dot3 < 0.0f)continue;
                // �ŋߋ������X�V
                neart = t;
                // ��_�Ɩ@�����X�V
                HitPosition = P;
                HitNormal = N;
                // �}�e���A���C���f�b�N�X�͍ŏ��[�P�ɂȂ��Ă��郁�b�V���P�ʂň��\��������
                materialIndex = subset.materialIndex;
            }
        }
        if (materialIndex >= 0)
        {
            // ���[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
            // �ʒu
            DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition,
                WorldTransform);
            // �����Ŗ@����ۑ����Ă��������ʂɕۑ����Ȃ��\�����Ȃ��̂ł��Ȃ��Ă���
            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
            float distance;
            DirectX::XMStoreFloat(&distance, WorldCrossLength);
            // �b��`�����s�I���ƍ��̋��������Ƌ߂���������B
            // �q�b�g���ۑ�
            if (result.distance > distance)
            {
                // �@��
                DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal,
                    WorldTransform);
                result.distance = distance;
                result.materialIndex = materialIndex;
                DirectX::XMStoreFloat3(&result.position, WorldPosition);
                DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
                hit = true;
            }
        }
    }
    return hit;
}
