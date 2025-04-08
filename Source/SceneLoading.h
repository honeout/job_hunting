#pragma once
#include "Graphics/Sprite.h"
#include "SpriteControll.h"
#include "TransForm2D.h"
#include "Ui.h"
#include "UiManager.h"
#include "Actor.h"
#include "Scene.h"
// �N���X
#include <thread>

// ���[�f�B���O�V�[��
class SceneLoading : public Scene
{
public:
    // ���̃V�[���������̃V�[���ɓ����
    SceneLoading(Scene* nextScene): nextScene(nextScene) {}
    ~SceneLoading() override {}

    // ������
    void Initialize() override;

    // �X�^�[�g
    void Start() override;

    // �I����
    void Finalize() override;

    // �X�V����
    void Update(float elapsedTime)override;

    // �`�揈��
    void Render() override;

    // �`��QD
    void Render2D(std::shared_ptr<Sprite> sprite, float angle, ID3D11DeviceContext* dc, DirectX::XMFLOAT2 pos);

    // �R���|�l���g�o�^
    void InitializeComponent();

    // icon��]
    void RotateLoadingIcon(float elapsedTime);

private:
    // ���[�f�B���O�X���b�h
    // �X�^�e�B�b�N������N���X�̒��g��������Ȃ�
    static void LoadingThread(SceneLoading* scene);
private:
    std::shared_ptr<Sprite> spriteLoading;
    std::shared_ptr<Sprite> spriteControlManual;

    float angle = 0.0f;
    float angleLoading = 0.0f;

    Scene* nextScene = nullptr;
    std::thread* thread = nullptr;

    // �s���͈�
    DirectX::XMFLOAT3 minPos = { 0.0f,0.0f,0.0f };
    DirectX::XMFLOAT3 maxPos = { 0.0f,0.0f,0.0f };

    // actorManager���P�ȏ�Ȃ�
    int sceneGameCountMin = 1;

    std::unique_ptr<Scene> currentScene;

    bool isFinalizeGame = false;
};
