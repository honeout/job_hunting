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

// ゲームシーン
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override{}

	// 初期化
	void Initialize() override;

	//　ゲーム始まった瞬間だけ
	void Start() override;

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

	// ポストエフェクトの初期化
	void PostProcessingRendererInitialize();

	// スロー状態変更
	void SetSlowState(float elapsedTime);

	bool InputMenue();

	// コンポネント登録
	void InitializeComponent();

	// BGM再生
	void StartBgm();

	// BGM停止
	void StopBgm();
private:
	// BGM
	std::unique_ptr<Audio> Bgm;

	// 画面サイズ
	float screenWidth = 1280;
	float screenHeight = 720;
	// コマンド位置の差
	float commandOffset = 10.0f;

	// カメラ位置
	DirectX::XMFLOAT3 cameraPosition = { 1.126f, -2.089f, -17.144f };

	// カメラ注視点
	DirectX::XMFLOAT3 cameraFocus = { 0,0,0 };

	// カメラ上情報
	DirectX::XMFLOAT3 cameraUp = { 0, 1, 0 };

	// プレイヤーHPバー位置情報
	DirectX::XMFLOAT2 playerHpBarPos = { screenWidth + 200 , screenHeight + 80 };
	DirectX::XMFLOAT2 enemyHpBarPos = { screenWidth + 130 , 0 };

	// playerMPバー位置
	DirectX::XMFLOAT2 playerMpBarPos = { playerHpBarPos.x , playerHpBarPos.y + 140 };

	// 必殺技チャージ枠位置
	DirectX::XMFLOAT2 SpecialGagePos = { 10, 300 };

    // コマンド大きさ
	DirectX::XMFLOAT2 commandSize = { 308,89 };

	DirectX::XMFLOAT2 commandSizeShortCut = { 358, 89 };

	// コマンド位置
	DirectX::XMFLOAT2 commandPos = {10, 500};

	// コマンド魔法差
	DirectX::XMFLOAT2 commandMagicOffset = { 280,79 };

	float commandMagicOffsetY = 10.0f;
	// コマンドスペシャル差
	DirectX::XMFLOAT2 commandSpecialOffset = { 0,166 };
	// コマンドボタン差
	DirectX::XMFLOAT2 commandButtonOffset = {80,42};

	// コマンドチャージ文字
	DirectX::XMFLOAT2 commandChargePos = { commandPos.x + commandMagicOffset.x + 50, 611};

	// チャージ矢印差
	DirectX::XMFLOAT2 commandSpecialGageOffset = { 70,5 };


	// 選択コマンド元画像位置
	DirectX::XMFLOAT2 commandSelectTexPos = { .0f,92.0f };
	// 選択コマンド元画像大きさ
	DirectX::XMFLOAT2 commandSelectTexScale = { .0f,92.0f };
	// 非選択コマンド基画像位置
	DirectX::XMFLOAT2 commandUnSelectTexPos = { .0f,0.0f };
	// 非選択コマンド基画像大きさ
	DirectX::XMFLOAT2 commandSUnelectTexScale = { .0f,92.0f };

	// 非選択状態透明度
	float commandAlphaUnSelect = 0.5f;

	DirectX::XMFLOAT2 timeIrastPos = { 10.0f, 0.0f };
	DirectX::XMFLOAT2 timeIrastSize = { 250.0f, 250.0f };

	// 簡易操作説明大きさ
	DirectX::XMFLOAT2 operationInstructionsSelect = { 460,126 };

	// hpの位置の差
	float enemyHpOffsestY = 163;

	// シェーダーをエフェクトして再生
	float shaderPlayStateTimer = 0;
	float shaderPlayStateTimerMax = 0.8f;

	// シェーダーをエフェクトして再生揺れ
	float shaderBlurStateTimer = 0;
	float shaderBlurStartStateTimer = 0;
	float shaderBlurStateTimerMax = 0.9f;

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

	// デバッグ用ライトポジション
	float lightPositionScale = -250.0f;

	// BGMの音の大きさ
	float bgmVolume = 0.8f;

	// オフスクリーンレンダリング用描画ターゲット
	std::unique_ptr<RenderTarget> renderTarget;
	std::unique_ptr<Sprite>	sprite;

	float dissolveThreshold = 0.0f;
	float edgeThreshold = 0.0f; // 緑の閾値
	DirectX::XMFLOAT4 edgeColor{0.0f,0.0f,0.0f,0.0f}; // 緑の色

	// 画面の色
	ColorGradingData       colorGradingDataMin;
	ColorGradingData       colorGradingData;

	// 画面歪み
	// ラジアルブラー情報
	RadialBlurData  radialBlurData;

	VignetteData vignetteData;

	// 画面の白ボケの最大値
	float colorGradingDataBrigthnessMax = 1.5f;

	// 画面歪み最大
	float radialBlurDataRadislBlurRadiusMax = 500.0f;

	// シャドウマップ用情報
	Light* mainDirectionalLight = nullptr; // シャドウマップを生成する平行光源
	//std::unique_ptr <Light> mainDirectionalLight; // シャドウマップを生成する平行光源
	std::unique_ptr<DepthStencil> shadowmapDepthStencil; // シャドウマップ用深度ステンシルバッファ
	float shadowDrawRect = 59.0f;// シャドウマップに描画する範囲
	DirectX::XMFLOAT4X4 lightViewProjeciton = {
	0.0f,0.0f,0.0f,0.0f, 
	0.0f,0.0f,0.0f,0.0f, 
	0.0f,0.0f,0.0f,0.0f, 
	0.0f,0.0f,0.0f,0.0f}; // ライトビュープロジェクション行列
	DirectX::XMFLOAT3 shadowColor = { 0.2f,0.2f,0.2f };// 影の色
	float shadowBias = 0.001f;// 深度比較用のオフセット値

	//CameraController* cameraControlle = nullptr;
	std::unique_ptr<CameraController> cameraControlle;

	// 周辺減光周り
		// 周辺減光
	DirectX::XMFLOAT4 vignette_color = { 1.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT2 vignette_center = { 0.5f, 0.5f };
	float vignette_intensity = 0.5f;
	float vignette_smoothness = 0.0f;

	bool vignette_rounded = false;
	float vignette_roundness = 1.0f;

	bool sceneChengeCheckDead = false;

	bool dlayTimeCheck = false;

	// スロー時間
	float dlayStateTimer = 0.0f;
	float dlayStateTimerMax = 0.5f;

	float lightRange = 130;

	// ブルーム用
	BloomData bloomData;

	// 行動範囲
	DirectX::XMFLOAT3 minPos = {0.0f,0.0f,0.0f};
	DirectX::XMFLOAT3 maxPos = {0.0f,0.0f,0.0f};

	

	// メニューのオンオフ
	bool isMenue = false;
	bool isMenueOn = true;
	bool isMenueOf = false;

	bool isAudioDebug = true;

	// デバッグ
	DirectX::XMFLOAT2 debugPush2Pos = { 480,0.0f };
	DirectX::XMFLOAT2 debugShortCutPos = { 480,0.0f };
	DirectX::XMFLOAT2 debugShortPushPos = { 480,120.0f };
	DirectX::XMFLOAT2 debugPush2Scale = { 300,204 };
	DirectX::XMFLOAT2 debugShortCutScale = { 300,204 };
	DirectX::XMFLOAT2 debugShortPushScale = { 82.0f,49.0f };

	// ２D描画させるかどうか
	bool DrawUi = true;
	bool DrawUiEmpth = false;
};
