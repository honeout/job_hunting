#include <imgui.h>
#include "AfterimageManager.h"
#include "Collision.h"




// �X�V����
void AfterimageManager::Update(float elapsedTime)
{
    for (Afterimage* after : afterimages)
    {
        after->Update(elapsedTime);
    }


    // �j������ ���t���[�������ň�C�ɏ����B
    for (Afterimage* after : removes)
    {
        std::vector<Afterimage*>::iterator it = std::find(afterimages.begin(), afterimages.end(),
            after);

        // ����������
        if (it != afterimages.end())
        {
            // �폜�o�^
            afterimages.erase(it);

        }

        // �������󂶂�Ȃ��̂�
        // �G�̔j������
        delete after;// ���ꕨ�j��

    }
    // �j�����X�g���N���A
    removes.clear();

    //// �G���m�̏Փˏ���
    //CollisiionEnemyVsEnemies();
}

// �����蔻��`��
void AfterimageManager::DrawDebugPrimitive()
{
   
    for (Afterimage* after : afterimages)
    {
        after->DrawDebugPrimitive();
    }
}


// �`�揈��
void AfterimageManager::Render(RenderContext& context, ModelShader* shader)
{
    for (Afterimage* after : afterimages)
    {
        after->Render(context, shader);
    }
}

void AfterimageManager::Remove(Afterimage* after)
{
    // �폜�ݒ�
    removes.insert(after);// �����Ȃ�����ƃo�O�邽��
  
}




void AfterimageManager::Register(Afterimage* after)
{
    // ��������납��ǉ����Ă��������Ă�������
    afterimages.emplace_back(after);
}
//
//void AfterimageManager::CollisiionEnemyVsEnemies()
//{
//    // �ő吔
//    size_t playerafterimageCount = playerafterimages.size();
//    for (int i = 0; i < playerafterimageCount; ++i) {
//        // i�Ԗڂ̈ʒu
//        Enemy* enemyA = enemies.at(i);
//        for (int j = i + 1; j < enemyCount; ++j)
//        {
//            Enemy* enemyB = enemies.at(j);
//            DirectX::XMFLOAT3 outPosition;
//            if (Collision::IntersectSpherVsSphere(
//                enemyA->GetPosition(),
//                enemyA->GetRadius(),
//                enemyB->GetPosition(),
//                enemyB->GetRadius(),
//                outPosition))
//            {
//                enemyB->SetPosition(outPosition);
//            }
//        }
//
//    }
//
//}



void AfterimageManager::DrawDebugGUI()
{
}

// �G�l�~�[�S�폜
void AfterimageManager::Clear()
{
    for (Afterimage* after : afterimages)
    {
        // ���̂��������Ǘ����Ă��鐔�͂��̂܂�
        delete after;

    }
    // vector�̐����O��
    afterimages.clear();
}
