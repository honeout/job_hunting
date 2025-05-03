#pragma once
#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/DepthStencil.h"
#include "Light.h"
#include "PostprocessingRenderer.h"
#include "Graphics\PrimitiveRenderer.h"
#include "Audio/AudioSource.h"
#include "Audio\AudioParam.h"
#include "Audio\Audio.h"
#include "Audio\AudioResourceManager.h"

// �Q�[���V�[��
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override{}

	// ������
	void Initialize() override;

	//�@�Q�[���n�܂����u�Ԃ���
	void Start() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;



private:
	// 3D��Ԃ̕`��
	void Render3DScene();

	// �V���h�E�}�b�v�̕`��
	void RenderShadowmap();

	void PostProcessingRendererInitialize();
	void PostProcessingRendererFinalize();

	void PlayEffectsShaders(float elapsedTime);

	bool InputMenue();

	// �R���|�l���g�o�^
	void InitializeComponent();

	// BGM�Đ�
	void StartBgm();

	// BGM��~
	void StopBgm();

private:
	// BGM
	std::unique_ptr<Audio> Bgm;

	// �V�F�[�_�[���G�t�F�N�g���čĐ�
	float shaderPlayStateTimer = 0;
	float shaderPlayStateTimerMax = 0.8f;

	// �V�F�[�_�[���G�t�F�N�g���čĐ��h��
	float shaderBlurStateTimer = 0;
	float shaderBlurStartStateTimer = 0;
	float shaderBlurStateTimerMax = 0.9f;

	// �c���o���Ԋu�ő�l
	float spawnafterimagetimemax = 0.1f;

	// �c���o���Ԋu
	float elapsedFrame = spawnafterimagetimemax;

	// �c�������x�ő�l
	float afterImageAlphaMax = 0.7f;
	// �c�������x
	float afterImageAlpha = afterImageAlphaMax;

	// �c��������܂ōő�l
	float reduceMax = 0.4f;
	// �c��������܂�
	float reduce = reduceMax;

	// �f�o�b�O�p���C�g�|�W�V����
	float lightPositionScale = -250.0f;

	// BGM�̉��̑傫��
	float bgmVolume = 0.8f;

	// �I�t�X�N���[�������_�����O�p�`��^�[�Q�b�g
	std::unique_ptr<RenderTarget> renderTarget;
	std::unique_ptr<Sprite>	sprite;

	float dissolveThreshold = 0.0f;
	float edgeThreshold = 0.0f; // �΂�臒l
	DirectX::XMFLOAT4 edgeColor{0.0f,0.0f,0.0f,0.0f}; // �΂̐F

	// ��ʂ̐F
	ColorGradingData       colorGradingDataMin;
	ColorGradingData       colorGradingData;

	// ��ʘc��
	// ���W�A���u���[���
	RadialBlurData  radialBlurData;

	VignetteData vignetteData;

	// ��ʂ̔��{�P�̍ő�l
	float colorGradingDataBrigthnessMax = 1.5f;

	// ��ʘc�ݍő�
	float radialBlurDataRadislBlurRadiusMax = 500.0f;

	// �V���h�E�}�b�v�p���
	Light* mainDirectionalLight = nullptr; // �V���h�E�}�b�v�𐶐����镽�s����
	//std::unique_ptr <Light> mainDirectionalLight; // �V���h�E�}�b�v�𐶐����镽�s����
	std::unique_ptr<DepthStencil> shadowmapDepthStencil; // �V���h�E�}�b�v�p�[�x�X�e���V���o�b�t�@
	float shadowDrawRect = 500.0f;// �V���h�E�}�b�v�ɕ`�悷��͈�
	DirectX::XMFLOAT4X4 lightViewProjeciton = {
	0.0f,0.0f,0.0f,0.0f, 
	0.0f,0.0f,0.0f,0.0f, 
	0.0f,0.0f,0.0f,0.0f, 
	0.0f,0.0f,0.0f,0.0f}; // ���C�g�r���[�v���W�F�N�V�����s��
	DirectX::XMFLOAT3 shadowColor = { 0.2f,0.2f,0.2f };// �e�̐F
	float shadowBias = 0.001f;// �[�x��r�p�̃I�t�Z�b�g�l

	//CameraController* cameraControlle = nullptr;
	std::unique_ptr<CameraController> cameraControlle;

	// ���ӌ�������
		// ���ӌ���
	DirectX::XMFLOAT4 vignette_color = { 1.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT2 vignette_center = { 0.5f, 0.5f };
	float vignette_intensity = 0.5f;
	float vignette_smoothness = 0.0f;

	bool vignette_rounded = false;
	float vignette_roundness = 1.0f;

	bool sceneChengeCheckDead = false;

	bool dlayTimeCheck = false;

	// �X���[����
	float dlayStateTimer = 0.0f;
	float dlayStateTimerMax = 0.5f;

	float lightRange = 130;

	// �u���[���p
	BloomData bloomData;

	// �s���͈�
	DirectX::XMFLOAT3 minPos = {0.0f,0.0f,0.0f};
	DirectX::XMFLOAT3 maxPos = {0.0f,0.0f,0.0f};

	// ���j���[�̃I���I�t
	bool isMenue = false;
	bool isMenueOn = true;
	bool isMenueOf = false;

	bool isAudioDebug = true;

	// �f�o�b�O
	DirectX::XMFLOAT2 debugPush2Pos = { 480,0.0f };
	DirectX::XMFLOAT2 debugShortCutPos = { 480,0.0f };
	DirectX::XMFLOAT2 debugShortPushPos = { 480,120.0f };
	DirectX::XMFLOAT2 debugPush2Scale = { 300,204 };
	DirectX::XMFLOAT2 debugShortCutScale = { 300,204 };
	DirectX::XMFLOAT2 debugShortPushScale = { 82.0f,49.0f };

	// �QD�`�悳���邩�ǂ���
	bool DrawUi = true;
	bool DrawUiEmpth = false;
};
