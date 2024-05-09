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
    // ����projectiles�͈̔�for������erase()����ƕs����������Ă��܂����߁A
    //  �X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
    for (Afterimage* after : removes)// �E���܂��惊�X�g���E��
    {
        // std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
        //      �C�e���[�^�[�ׂ̈�std::find���g��              (enemies�̍ŏ�����Ō�܂Œ��ׂ�enemy����������)
        std::vector<Afterimage*>::iterator it = std::find(afterimages.begin(), afterimages.end(),
            after);//vector����enemy�T���ďo��
        if (it != afterimages.end())// ����������
        {
            //erase�֐���vector�v�f���폜�A�j�����X�g�̃|�C���^����C�e���[�^�[���������A�֐��ɓn��
            afterimages.erase(it);// �폜
            // �Ǘ��ۂ������������
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

void AfterimageManager::DrawDebugPrimitive()
{
    // �S�Ă̓z������Ă���
    for (Afterimage* after : afterimages)
    {
        after->DrawDebugPrimitive();
    }
}


// �`�揈��
void AfterimageManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
    for (Afterimage* after : afterimages)
    {
        after->Render(context, shader);
    }
}

void AfterimageManager::Remove(Afterimage* after)
{
    // vector�Ƃ͈Ⴄset�͍폜����
    // �j�����X�g�ɒǉ� ����enemies�v�f�������ƕs����N���邩��j�����X�g�ɓo�^
    removes.insert(after);// �E�����X�g�E���ׂ̃��X�g
    // ���̂����ŏ����Ȃ������ŏ�������for���ɂ���Ă��܂�����
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

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        // �g�����X�t�H�[��
        if (ImGui::CollapsingHeader("Enemy", ImGuiTreeNodeFlags_DefaultOpen))
        {

            //  ImGui::InputFloat3("Position", &);


            ImGui::Button("Add");

            //for (Enemy* enemy : enemies)
            //{
            //    enemies.emplace_back(enemy);
            //}





        }
    }
    ImGui::End();
}

// �G�l�~�[�S�폜
void AfterimageManager::Clear()
{
    for (Afterimage* after : afterimages)// 
    {
        // ���̂��������Ǘ����Ă��鐔�͂��̂܂�
        delete after;

    }
    // vector�̐����O��
    afterimages.clear();
}
