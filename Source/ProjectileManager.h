#pragma once

#include <vector>
#include "Actor.h"
#include <set>

//Projectil�P�̒e��
// �e�ۃ}�l�[�W���[
class ProjectileManager// �����̒e���񋅐�
{
public:

    ProjectileManager() {};
    // �N������������Ȃ����畡���̓z���ʁX�Ɏ���������Ȃ�
    ~ProjectileManager() {};

    // �C���X�^���X�擾
    static ProjectileManager& Instance()
    {
        static ProjectileManager instance;
        return instance;
    }




    // �X�V����
    void DeleteUpdate(float elapsedTime) ;

    // �`�揈��
    //void Render(const RenderContext& rc, ModelShader* shader);

    // �f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    // �e�ۓo�^
    void Register(std::shared_ptr<Actor> projectile);

    // �e�ۑS�폜
    void Clear();

    // �e�ې��擾
    int GetProjectileCount() const { return static_cast<int> (projectiles.size()); }


    // �e�ێ擾
    std::shared_ptr<Actor> GetProjectile(int index) { return projectiles.at(index); }

    // �e�ۍ폜
    void Remove(std::shared_ptr<Actor> projectile);

private:
    std::vector<std::shared_ptr<Actor>>   projectiles;
    // ����̒e�ۍ폜����폜����
    std::set<std::shared_ptr<Actor>>       removes;
};