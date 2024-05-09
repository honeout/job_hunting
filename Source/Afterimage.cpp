#include "Afterimage.h"
#include "Graphics/Graphics.h"
#include "AfterimageManager.h"



// �f�o�b�O�v���~�e�B�u�`��
void Afterimage::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //// �Փ˔���p�̃f�o�b�O����`��
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    // �Փ˔���p�̃f�o�b�O�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

}


// �j��
void Afterimage::Destroy()
{
    //EnemyManager& manager = EnemyManager::Instance();
    ////// �������g�����Ă���Ō̂��������߂̏��ɓ����B�v
    ////manager->Remove(this);
    //manager.Remove(this);

    //EnemyManager::Instance().;
    AfterimageManager::Instance().Remove(this);
}