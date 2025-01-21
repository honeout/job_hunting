#pragma once
#include "Graphics/Sprite.h"
#include "CameraController.h"
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
class SceneGameOver : public Scene
{
public:
    SceneGameOver() {};
    ~SceneGameOver() override {}

    // 初期化
    void Initialize() override;

    // 終了化
    void Finalize() override;
    // 更新処理
    void Update(float elapsedTime)override;
    // 描画処理
    void Render() override;


    // 3D空間の描画
    void Render3DScene();

    // シャドウマップの描画
    void RenderShadowmap();
    // コンポネント登録
    void InitializeComponent();

    // 画面エフェクト
    void PlayEffectsShaders(float elapsedTime);

	// どのシーンに行くか
	void SelectScene();
private:
	enum class Select
	{
		Title,
		Game
	};

	

private:
    //Sprite* sprite = nullptr;


    CameraController* cameraControlle = nullptr;

	// オフスクリーンレンダリング用描画ターゲット
	std::unique_ptr<RenderTarget> renderTarget;


	// 画面の色
	ColorGradingData       colorGradingData;

	// 画面歪み
	// ラジアルブラー情報
	RadialBlurData  radialBlurData;

	// 画面の白ボケの最大値
	float colorGradingDataBrigthnessMax = 1.5f;

	// 画面歪み最大
	float radialBlurDataRadislBlurRadiusMax = 500.0f;


	// シャドウマップ用情報
	Light* mainDirectionalLight = nullptr; // シャドウマップを生成する平行光源
	std::unique_ptr<DepthStencil> shadowmapDepthStencil; // シャドウマップ用深度ステンシルバッファ
	float shadowDrawRect = 500.0f;// シャドウマップに描画する範囲
	DirectX::XMFLOAT4X4 lightViewProjeciton = {
	0.0f,0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f,0.0f,
	0.0f,0.0f,0.0f,0.0f }; // ライトビュープロジェクション行列
	DirectX::XMFLOAT3 shadowColor = { 0.2f,0.2f,0.2f };// 影の色
	float shadowBias = 0.001f;// 深度比較用のオフセット値

	// ポストプロセス
	std::unique_ptr<PostprocessingRenderer> postprocessingRenderer;

	// ブルーム用
	BloomData bloomData;

	VignetteData vignetteData;


	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Sprite> spritePush;


	DirectX::XMFLOAT2 position = { 500.0f,300.0f };
	DirectX::XMFLOAT2 scale = { 0.0f,0.0f };


	// 行動範囲
	DirectX::XMFLOAT3 minPos = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 maxPos = { 0.0f,0.0f,0.0f };

	// 光半径
	float lightRange = 130;


	// シェーダーをエフェクトして再生
	float shaderPlayStateTimer = 0;
	float shaderPlayStateTimerMax = 0.8f;

	// シェーダーをエフェクトして再生揺れ
	float shaderBlurStateTimer = 0;
	float shaderBlurStartStateTimer = 0;
	float shaderBlurStateTimerMax = 0.9f;



	// スロー時間
	float dlayStateTimer = 0.0f;
	float dlayStateTimerMax = 4.5f;

	bool dlayTimeCheck = false;

	// UI大きさ
	DirectX::XMFLOAT2 titleUiScaleSelected = { 181,104 };
	DirectX::XMFLOAT2 titleUiScaleUnselected = { 151,64 };


	// UI位置
	float gameUiPositionSelected = 477;
	float titleUiPositionSelected = 515;
	float titleUiPositionUnselected = 554;
	// UI大きさ
	DirectX::XMFLOAT2 gameUiScaleSelected = { 181,104 };
	DirectX::XMFLOAT2 gameUiScaleUnselected = { 151,64 };



	int selectPush = 0;




};
