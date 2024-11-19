#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

// 平行光源情報(太陽)
struct DirectionalLightData
{
	DirectX::XMFLOAT4 direction; // 向き
	DirectX::XMFLOAT4 color;     // 色
};

// 点光源
struct PointLightData
{
	DirectX::XMFLOAT4 position; // 座標
	DirectX::XMFLOAT4 color; // 色
	float range;              // 範囲
	DirectX::XMFLOAT3 dummy;
};

// 点光源の最大数
static constexpr int PointLightMax = 8;

// スポットライト情報
struct SpotLightData
{
	DirectX::XMFLOAT4 position;			// 座標
	DirectX::XMFLOAT4 direction;		// 向き
	DirectX::XMFLOAT4 color;			// 色
	float			  range;			// 範囲
	float			  innerCorn;        // インナー角度範囲
	float			  outerCorn;		// アウター角度範囲
	float			  dummy;
};

// スポットライトの最大数
static constexpr      int SpotLightMax = 8;

// 色調補正情報
struct ColorGradingData
{
	float hueShift = 0;// 色相調整
	float saturation = 1;// 彩度調整
	float brigthness = 1;// 明度調整
	float dummy;
};


#define	UNIT_VN_01	//	ヴィネット
#define	UNIT_VN_02	//	アイリス処理


#if	defined( UNIT_VN_01 )
//	ビネット情報
struct VignetteData
{
	DirectX::XMFLOAT4	color = { 0.0f, 0.0f, 0.0f, 1.0f };	//	減光色
	DirectX::XMFLOAT2	center = { 0.5f, 0.5f };				//	減光中心点
	float				intensity = 0.5f;							//	強度
	float				smoothness = 0.2f;							//	滑らかさ

	bool				rounded = false;						//	スクリーンサイズを考慮
	float				roundness = 1.0f;							//	考慮度合い

#if defined( UNIT_VN_02 )
	bool				iris = false;						//	アイリス処理
#endif	//	defined( UNIT_VN_02 )
};
#endif	//	defined( UNIT_VN_01 )

// ガウスフィルター計算情報
struct GaussianFilterData
{
	int             kernelSize = 8; // カーネルサイズ
	float			deviation = 10.0f; // 標準偏差 
	DirectX::XMFLOAT2 textureSize;    // 暈すテクスチャのサイズ
};

// ガウスフィルターの最大カーネルサイズ
static const int MaxkernelSize = 16;

// 高輝度抽出用情報
struct LuminanceExtractionData
{
	float threshold = 0.5f; // 閾値
	float intensity = 1.0f; // ブルームの強度
	DirectX::XMFLOAT2 dummy2;
};

// ポストエフェクトの最終パス用情報
struct FinalpassnData
{
	// ブルームテクスチャ
	ID3D11ShaderResourceView* bloomTexture;
};

// シャドウマップ用情報
struct ShadowMapData
{
	ID3D11ShaderResourceView* shadowMap; // シャドウマップテクスチャ
	DirectX::XMFLOAT4X4       lightViewProjection; // ライトビュープロジェクション行列
	DirectX::XMFLOAT3         shadowColor; // 影の色 
	float                     shadowBias; // 深度比較用のオフセット値
};

// レンダーコンテキスト
struct RenderContext
{
	ID3D11DeviceContext* deviceContext;

	// カメラ情報
	DirectX::XMFLOAT4       viewPosition;
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;

	DirectX::XMFLOAT4		lightDirection;
	float                   alpha;

	// ライト情報
	DirectX::XMFLOAT4 ambientLightColor;
	DirectionalLightData directionalLightData;
	PointLightData pointLightData[PointLightMax]; // 点光源情報
	SpotLightData spotLightData[SpotLightMax];//スポットライト情報
	int      pointLightCount = 0;// 点光源数
	int      spotLightCount = 0; // スポットライト数

	// 周辺減光
	DirectX::XMFLOAT4 vignette_color = { 0.5f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT2 vignette_center = { 0.5f, 0.5f };
	float vignette_intensity = 0.5f;
	float vignette_smoothness = 0.2f;

	bool vignette_rounded = false;
	float vignette_roundness = 1.0f;

	// 色調補正情報
	ColorGradingData colorGradingData;


	//ガウスフィルター情報
	GaussianFilterData    gaussianFilterData;


	// 高輝度抽出用情報
	LuminanceExtractionData luminanceExtractionData;


	// 最終パス情報
	FinalpassnData          finalpassnData;

    // シャドウマップ情報
	ShadowMapData shadowMapData;

#if	defined( UNIT_VN_01 )
	//	ビネット情報
	VignetteData			vignetteData;
#endif	//	defined( UNIT_VN_01 )

	//Microsoft::WRL::ComPtr<ID3D11Buffer> vignette_constant_buffer;
	//Microsoft::WRL::ComPtr<ID3D11PixelShader> vignette_pixel_shader;
};
