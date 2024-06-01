#pragma once


#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"

#include "AfterimageManager.h"

// ゲームシーン
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override{}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;

private:
	// エネミーHPゲージ描画
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

	// 残像出す間隔最大値
	float spawnafterimagetimemax = 0.6f;

	// 残像出す間隔
	float elapsedFrame = spawnafterimagetimemax;

	// 残像透明度最大値
	float afterImageAlphaMax = 0.6f;
	// 残像透明度
	float afterImageAlpha = afterImageAlphaMax;

	// 残像消えるまで最大値
	float reduceMax = 0.4f;
	// 残像消えるまで
	float reduce = reduceMax;
};
