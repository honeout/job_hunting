#pragma once


#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"

#include "Graphics/RenderTarget.h"
#include "Graphics/DepthStencil.h"
#include "Light.h"
#include "PostprocessingRenderer.h"

// �Q�[���V�[��
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override{}

	// ������
	void Initialize() override;

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


	// �G�l�~�[HP�Q�[�W�`��
	void RenderEnemyGauge(
	ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);

	void AfterimageTime(float elapsedTime);


private:
	

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


	// �I�t�X�N���[�������_�����O�p�`��^�[�Q�b�g
	std::unique_ptr<RenderTarget> renderTarget;
	std::unique_ptr<Sprite>	sprite;

	float dissolveThreshold;
	float edgeThreshold; // �΂�臒l
	DirectX::XMFLOAT4 edgeColor; // �΂̐F

	// ��ʂ̐F
	ColorGradingData       colorGradingData;

	// �V���h�E�}�b�v�p���
	Light* mainDirectionalLight = nullptr; // �V���h�E�}�b�v�𐶐����镽�s����
	//std::unique_ptr <Light> mainDirectionalLight; // �V���h�E�}�b�v�𐶐����镽�s����
	std::unique_ptr<DepthStencil> shadowmapDepthStencil; // �V���h�E�}�b�v�p�[�x�X�e���V���o�b�t�@
	float shadowDrawRect = 500.0f;// �V���h�E�}�b�v�ɕ`�悷��͈�
	DirectX::XMFLOAT4X4 lightViewProjeciton; // ���C�g�r���[�v���W�F�N�V�����s��
	DirectX::XMFLOAT3 shadowColor = { 0.2,0.2f,0.2f };// �e�̐F
	float shadowBias = 0.001f;// �[�x��r�p�̃I�t�Z�b�g�l

	// �|�X�g�v���Z�X
	std::unique_ptr<PostprocessingRenderer> postprocessingRenderer;

	// ���ӌ�������
		// ���ӌ���
	DirectX::XMFLOAT4 vignette_color = { 1.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT2 vignette_center = { 0.5f, 0.5f };
	float vignette_intensity = 0.5f;
	float vignette_smoothness = 0.0f;

	bool vignette_rounded = false;
	float vignette_roundness = 1.0f;

	bool sceneChengeCheckDead = false;
};
