#pragma once


#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"

#include "AfterimageManager.h"

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
	// �G�l�~�[HP�Q�[�W�`��
	void RenderEnemyGauge(
	ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);

	void AfterimageTime(float elapsedTime);


private:
	Player* player = nullptr;
	CameraController* cameraController = nullptr;

	Sprite* gauge = nullptr;

	AfterimageManager* afterimageManager;

	// �c���o���Ԋu�ő�l
	float spawnafterimagetimemax = 0.6f;

	// �c���o���Ԋu
	float elapsedFrame = spawnafterimagetimemax;

	// �c�������x�ő�l
	float afterImageAlphaMax = 0.6f;
	// �c�������x
	float afterImageAlpha = afterImageAlphaMax;

	// �c��������܂ōő�l
	float reduceMax = 0.4f;
	// �c��������܂�
	float reduce = reduceMax;
};
