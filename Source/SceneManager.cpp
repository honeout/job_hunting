#include "SceneManager.h"

// �X�V����
void SceneManager::Update(float elapsedTime)
{
    if (currentScene )
    {
        // ���t���[���X�Vnext�̒��g�̓z
        currentScene->Update(elapsedTime);
    }
}
// �`�揈��
void SceneManager::Render()
{
    // �������Ă���̂�����
    if (currentScene)
    {
        currentScene->Render();
    }
}
// �V�[���N���A
void SceneManager::Clear()
{
    if (currentScene)
    {
        currentScene->Finalize();
        delete currentScene;
        currentScene = nullptr;
    }
}
// �V�[���؂�ւ�
void SceneManager::ChangeScene(Scene* scene)
{
    // �V�����V�[����ݒ�
    nextScene = scene;

    // �Â��V�[���I������
    Clear();

    // �V�����V�[����ݒ�
    currentScene = nextScene;
    nextScene = nullptr;

    // �V�[������������
    // �Ⴄ�ꏊ�ŌĂ΂�Ă邩������Ă΂�Ă�����ʂ�Ȃ����ɂ���B
    if (!currentScene->IsReady())
        currentScene->Initialize();
}
