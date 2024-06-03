#include "StageManager.h"

void StageManager::Update(float elapsedTime)
{
    for (Stage* stage : stages)
    {
        stage->Update(elapsedTime);
    }
}

void StageManager::Render(const RenderContext& rc, ModelShader* shader)
{
    for (Stage* stage : stages)
    {
        stage->Render( rc,shader);
    }
}

void StageManager::Register(Stage* stage)
{
    stages.emplace_back(stage);
}

void StageManager::Clear()
{
    for (Stage* stage : stages)
    {
        delete stage;
    }
    // �S�ď����Ă���{�̏����B
    stages.clear();
}

bool StageManager::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    bool result = false;
    hit.distance = FLT_MAX;
    for (Stage* stage : stages)
    {
        HitResult tmp;
        if (stage->RayCast(start, end, tmp))
        {
            if (hit.distance > tmp.distance)
            {
                hit = tmp;
                result = true;


            }
        }
        //DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&start);
        //// �G�̈ʒu
        //DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&end);
        //// ��������G�܂ł̈ʒu���v��
        //DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
        //// �x�N�g���̂Ȃ������Q�悷��B����Ă��Ȃ��z
        //DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
        //float d;
        //DirectX::XMStoreFloat(&d, D);

        //if (d > tmp.distance)
        //{
        //    result = true;
        //}

    }

    return result;
}
