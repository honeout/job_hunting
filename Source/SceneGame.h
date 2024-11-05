#pragma once


#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"

#include "Graphics/RenderTarget.h"
#include "Graphics/DepthStencil.h"
#include "Light.h"
#include "PostprocessingRenderer.h"

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
	// 3D空間の描画
	void Render3DScene();

	// シャドウマップの描画
	void RenderShadowmap();


	// エネミーHPゲージ描画
	void RenderEnemyGauge(
	ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);

	void AfterimageTime(float elapsedTime);


private:
	

	// 残像出す間隔最大値
	float spawnafterimagetimemax = 0.1f;

	// 残像出す間隔
	float elapsedFrame = spawnafterimagetimemax;

	// 残像透明度最大値
	float afterImageAlphaMax = 0.7f;
	// 残像透明度
	float afterImageAlpha = afterImageAlphaMax;

	// 残像消えるまで最大値
	float reduceMax = 0.4f;
	// 残像消えるまで
	float reduce = reduceMax;


	// オフスクリーンレンダリング用描画ターゲット
	std::unique_ptr<RenderTarget> renderTarget;
	std::unique_ptr<Sprite>	sprite;

	float dissolveThreshold;
	float edgeThreshold; // 緑の閾値
	DirectX::XMFLOAT4 edgeColor; // 緑の色

	// 画面の色
	ColorGradingData       colorGradingData;

	// シャドウマップ用情報
	Light* mainDirectionalLight = nullptr; // シャドウマップを生成する平行光源
	//std::unique_ptr <Light> mainDirectionalLight; // シャドウマップを生成する平行光源
	std::unique_ptr<DepthStencil> shadowmapDepthStencil; // シャドウマップ用深度ステンシルバッファ
	float shadowDrawRect = 500.0f;// シャドウマップに描画する範囲
	DirectX::XMFLOAT4X4 lightViewProjeciton; // ライトビュープロジェクション行列
	DirectX::XMFLOAT3 shadowColor = { 0.2,0.2f,0.2f };// 影の色
	float shadowBias = 0.001f;// 深度比較用のオフセット値

	// ポストプロセス
	std::unique_ptr<PostprocessingRenderer> postprocessingRenderer;

	// 周辺減光周り
		// 周辺減光
	DirectX::XMFLOAT4 vignette_color = { 1.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT2 vignette_center = { 0.5f, 0.5f };
	float vignette_intensity = 0.5f;
	float vignette_smoothness = 0.0f;

	bool vignette_rounded = false;
	float vignette_roundness = 1.0f;

	bool sceneChengeCheckDead = false;
};
