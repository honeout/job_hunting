#include "ProjectileManager.h"

ProjectileManager::ProjectileManager()
{
}

ProjectileManager::~ProjectileManager()
{
    Clear();
}

void ProjectileManager::Update(float elapsedTime)
{

    // �X�V����
    for (Projectile* projectile : projectiles)
    {
        projectile->Update(elapsedTime);
    }

    // �j������ ���t���[�������ň�C�ɏ����B
    for (Projectile* projectile : removes)// �E���܂��惊�X�g���E��
    {
        std::vector<Projectile*>::iterator it = std::find(projectiles.begin(), projectiles.end(),
                                                                                  projectile);
        if (it != projectiles.end())
        {
            projectiles.erase(it);// �폜
        }

        // �e�ۂ̔j������
        delete projectile;// ���ꕨ�j��

    }
    // �j�����X�g���N���A
    removes.clear();

}

void ProjectileManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    for (Projectile* projectile : projectiles)
    {
        projectile->Render(dc, shader);
    }
}

void ProjectileManager::DrawDebugPrimitive()
{

    // �S�Ă̓z������Ă���
    for (Projectile* projectile : projectiles)
    {
        projectile->DrawDebugPrimitive();
    }
}

void ProjectileManager::Register(Projectile* projectile)
{
    // ��������납��ǉ����Ă��������Ă�������
    projectiles.emplace_back(projectile);
}

void ProjectileManager::Clear()
{
    for (Projectile* projectile : projectiles)// 
    {
        // ���̂��������Ǘ����Ă��鐔�͂��̂܂�
        delete projectile;

    }
    // vector�̐����O��
    projectiles.clear();
}

// �e�ۍ폜 �}�l�[�W���[�̂��̒e�ۏ����ĉ�����
void ProjectileManager::Remove(Projectile* projectile)
{
    removes.insert(projectile);// �E�����X�g�E���ׂ̃��X�g
}
