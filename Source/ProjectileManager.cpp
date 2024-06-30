#include "ProjectileManager.h"





//void ProjectileManager::Update(float elapsedTime)
//{
//
//    // �X�V����
//    for (Projectile* projectile : projectiles)
//    {
//        projectile->Update(elapsedTime);
//    }
//
//    // �j������ ���t���[�������ň�C�ɏ����B
//    for (Projectile* projectile : removes)// �E���܂��惊�X�g���E��
//    {
//        std::vector<Projectile*>::iterator it = std::find(projectiles.begin(), projectiles.end(),
//                                                                                  projectile);
//        if (it != projectiles.end())
//        {
//            projectiles.erase(it);// �폜
//        }
//
//        // �e�ۂ̔j������
//        delete projectile;// ���ꕨ�j��
//
//    }
//    // �j�����X�g���N���A
//    removes.clear();
//
//}

//void ProjectileManager::Render(const RenderContext& rc, ModelShader* shader)
//{
//    for (Actor* projectile : projectiles)
//    {
//        projectile->Render(rc, shader);
//    }
//}
//
//void ProjectileManager::DrawDebugPrimitive()
//{
//
//    // �S�Ă̓z������Ă���
//    for (Actor* projectile : projectiles)
//    {
//        projectile->DrawDebugPrimitive();
//    }
//}
// �폜�֐�
void ProjectileManager::DeleteUpdate(float elapsedTime)
{
        // �j������ ���t���[�������ň�C�ɏ����B
    for (std::shared_ptr<Actor> projectile : removes)// �E���܂��惊�X�g���E��
    {
        std::vector<std::shared_ptr<Actor>>::iterator it = std::find(projectiles.begin(), projectiles.end(),
                                                                     projectile);
        if (it != projectiles.end())
        {
            projectiles.erase(it);// �폜
        }

        // �e�ۂ̔j������
        //delete projectile;// ���ꕨ�j��

    }
    // �j�����X�g���N���A
    removes.clear();
}

void ProjectileManager::Register(std::shared_ptr<Actor> projectile)
{
    // ��������납��ǉ����Ă��������Ă�������
    projectiles.emplace_back(projectile);
}

void ProjectileManager::Clear()
{
        for (std::shared_ptr<Actor>  projectile : projectiles)// 
    {
        // ���̂��������Ǘ����Ă��鐔�͂��̂܂�
       // delete projectile;

    }
    // vector�̐����O��
    projectiles.clear();
}

void ProjectileManager::Remove(std::shared_ptr<Actor> projectile)
{
    // �폜�o�^
    removes.insert(projectile);// �E�����X�g�E���ׂ̃��X�g
}

//void ProjectileManager::Clear()
//{
//    for (Projectile* projectile : projectiles)// 
//    {
//        // ���̂��������Ǘ����Ă��鐔�͂��̂܂�
//        delete projectile;
//
//    }
//    // vector�̐����O��
//    projectiles.clear();
//}

//// �e�ۍ폜 �}�l�[�W���[�̂��̒e�ۏ����ĉ�����
//void ProjectileManager::Remove(Projectile* projectile)
//{
//    removes.insert(projectile);// �E�����X�g�E���ׂ̃��X�g
//}
