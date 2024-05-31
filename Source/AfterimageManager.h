#pragma once

#include <vector>
#include "Afterimage.h"
#include <set>

// �c��_�}�l�[�W���[
class AfterimageManager
{
private:
    AfterimageManager() {}
    ~AfterimageManager() {}

    //// �c��_���m�̏Փˏ���
    //void CollisiionEnemyVsEnemies();

public:
    // �B��̃X�^���X�擾
    // ����������Ȃ�����V���O���g���ō��
    static AfterimageManager& Instance()
    {
        static AfterimageManager instance;
        return instance;
    }



    // �X�V����
    void Update(float elapsedTime);

    // �f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive();

    // �`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // �c��_�폜
    void Remove(Afterimage* enemy);

    // �Ǘ�����ׂ̓z
    // �c��_�o�^
    void Register(Afterimage* enemy);

    void DrawDebugGUI();

    // �c��_�S�폜
    void Clear();

    // ���ԂȂ̂��̊m�F
    // �ő吔�l�����o��
    // �c��_���擾
    int GetAfterimageCount() const { return static_cast<int>(afterimages.size()); }

    // �c��_�擾
    // �����̔ԍ��̏ꏊ�ɓ����Ă��镨���o���B
    Afterimage* GetAfterimage(int index) { return afterimages.at(index); }


private:
    // �X�V�@�`��𕡐�����ׂ̂���
    std::vector<Afterimage*> afterimages;
    // �E���z�Z�b�g
    std::set<Afterimage*>  removes;



};
