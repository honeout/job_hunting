#include <imgui.h>
#include "EnemyManager.h"
#include "Collision.h"




// �X�V����
void EnemyManager::Update(float elapsedTime)
{
    for (Enemy* enemy :  enemies)
    {
        enemy->Update(elapsedTime);
    }

   
    // �j������ ���t���[�������ň�C�ɏ����B
    for (Enemy* enemy : removes)
    {
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(),
                                                                              enemy);//vector����enemy�T���ďo��
        if (it != enemies.end())// ����������
        {
            enemies.erase(it);
        }

        // �������󂶂�Ȃ��̂�
        delete enemy;// ���ꕨ�j��

    }
    // �j�����X�g���N���A
    removes.clear();

    // �G���m�̏Փˏ���
    CollisiionEnemyVsEnemies();
}

void EnemyManager::DrawDebugPrimitive()
{
    // �S�Ă̓z������Ă���
    for (Enemy* enemy : enemies)
    {
        enemy->DrawDebugPrimitive();
    }
}


// �`�揈��
void EnemyManager::Render(const RenderContext& rc, ModelShader* shader)
{
    for (Enemy* enemy : enemies)
    {
        enemy->Render(rc, shader);
    }
}

void EnemyManager::Remove(Enemy* enemy)
{
    removes.insert(enemy);
}




void EnemyManager::Register(Enemy* enemy)
{
    // ��������납��ǉ����Ă��������Ă�������
    enemies.emplace_back(enemy);
}

void EnemyManager::CollisiionEnemyVsEnemies()
{
    // �ő吔
    size_t enemyCount = enemies.size();
    for (int i = 0; i < enemyCount; ++i) {
        // i�Ԗڂ̈ʒu
        Enemy* enemyA = enemies.at(i);
        for (int j = i + 1; j < enemyCount; ++j)
            {
            Enemy * enemyB = enemies.at(j);
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSpherVsSphere(
                enemyA->GetPosition(),
                enemyA->GetRadius(),
                enemyB->GetPosition(),
                enemyB->GetRadius(),
                outPosition))
            {
                enemyB->SetPosition(outPosition);
            }
            }
            
    }

}



void EnemyManager::DrawDebugGUI()
{
    
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        // �g�����X�t�H�[��
        if (ImGui::CollapsingHeader("Enemy", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Button("Add");
        }
    }
    ImGui::End();
}

// �G�l�~�[�S�폜
void EnemyManager::Clear()
{
    for (Enemy* enemy : enemies)// 
    {
        // ���̂��������Ǘ����Ă��鐔�͂��̂܂�
        delete enemy;

    }
    // vector�̐����O��
    enemies.clear();
}
