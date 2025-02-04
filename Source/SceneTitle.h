#pragma once

#include <DirectXMath.h>

#include "CameraController.h"
#include "Graphics/Sprite.h"
#include "Scene.h"

#include "Graphics/RenderTarget.h"
#include "Graphics/DepthStencil.h"
#include "Light.h"
#include "PostprocessingRenderer.h"
#include "Audio/AudioSource.h"
#include "SpriteControll.h"
#include "TransForm2D.h"
#include "Ui.h"
#include "UiManager.h"

// �V���h�E�}�b�v�̃T�C�Y
static const UINT SHADOWMAP_SIZE = 2048;


class SceneTitle : public Scene
{
public:
    SceneTitle() {};
    ~SceneTitle() override {}

    // ������
    void Initialize() override;

    // �I����
    void Finalize() override;
    // �X�V����
    void Update(float elapsedTime)override;
    // �`�揈��
    void Render() override;
#ifdef _DEBUG
	// �f�o�b�O�p
	void DebugGui();
#endif // _DEBUG

	// 3D��Ԃ̕`��
	void Render3DScene();

	// �V���h�E�}�b�v�̕`��
	void RenderShadowmap();

	// �R���|�l���g�o�^
	void InitializeComponent();

	// �J�����̎��Ԑ��䏉����
	void CameraInitialize();
	// �J�����̎��Ԑ���
	void CameraUpdate(float elapsedTime);

	void PlayEffectsShaders(float elapsedTime);


	// �ǂ̃V�[���ɍs����
	void SelectScene();
private:
	enum class Select
	{
		Game,
		Exit
	};
private:
	// �J����
	CameraController* cameraControlle = nullptr;

	//// BGM
	//std::unique_ptr<AudioSource> bgm;
	//// SE
	//std::unique_ptr<AudioSource> se;

    // �I�t�X�N���[�������_�����O�p�`��^�[�Q�b�g
    std::unique_ptr<RenderTarget> renderTarget;


	// ��ʂ̐F
	ColorGradingData       colorGradingData;

	// ��ʘc��
	// ���W�A���u���[���
	RadialBlurData  radialBlurData;

	// ��ʂ̔��{�P�̍ő�l
	float colorGradingDataBrigthnessMax = 1.5f;

	// ��ʘc�ݍő�
	float radialBlurDataRadislBlurRadiusMax = 500.0f;


	// �V���h�E�}�b�v�p���
	Light* mainDirectionalLight = nullptr; // �V���h�E�}�b�v�𐶐����镽�s����
	std::unique_ptr<DepthStencil> shadowmapDepthStencil; // �V���h�E�}�b�v�p�[�x�X�e���V���o�b�t�@
	float shadowDrawRect = 500.0f;// �V���h�E�}�b�v�ɕ`�悷��͈�
	DirectX::XMFLOAT4X4 lightViewProjeciton = {
	0.0f,0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f,0.0f }; // ���C�g�r���[�v���W�F�N�V�����s��
	DirectX::XMFLOAT3 shadowColor = { 0.2f,0.2f,0.2f };// �e�̐F
	float shadowBias = 0.001f;// �[�x��r�p�̃I�t�Z�b�g�l

	// �|�X�g�v���Z�X
	//std::unique_ptr<PostprocessingRenderer> postprocessingRenderer;

	// �u���[���p
	BloomData bloomData;

	VignetteData vignetteData;


    std::unique_ptr<Sprite> sprite;
    std::unique_ptr<Sprite> spritePush;


    DirectX::XMFLOAT2 position = {500.0f,300.0f};
    DirectX::XMFLOAT2 scale = {0.0f,0.0f};


	// �s���͈�
	DirectX::XMFLOAT3 minPos = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 maxPos = { 0.0f,0.0f,0.0f };

	// �����a
	float lightRange = 130;


	// �V�F�[�_�[���G�t�F�N�g���čĐ�
	float shaderPlayStateTimer = 0;
	float shaderPlayStateTimerMax = 0.8f;

	// �V�F�[�_�[���G�t�F�N�g���čĐ��h��
	float shaderBlurStateTimer = 0;
	float shaderBlurStartStateTimer = 0;
	float shaderBlurStateTimerMax = 0.9f;



	// �X���[����
	float dlayStateTimer = 0.0f;
	float dlayStateTimerMax = 4.5f;

	bool dlayTimeCheck = false;


	// UI�傫��
	DirectX::XMFLOAT2 gameUiScaleSelected = { 181,104 };
	DirectX::XMFLOAT2 gameUiScaleUnselected = { 151,64 };


	// UI�ʒu
	float gameUiPositionSelected = 477;
	float exitUiPositionSelected = 515;
	float exitUiPositionUnselected = 554;
	// UI�傫��
	DirectX::XMFLOAT2 exitUiScaleSelected = { 181,104 };
	DirectX::XMFLOAT2 exitUiScaleUnselected = { 151,64 };



	// �ǂ��I�����Ă��邩UI
	int selectPush = 0;
};