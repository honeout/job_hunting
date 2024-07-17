#pragma once

#include <DirectXMath.h>

<<<<<<< HEAD
// •½sŒõŒ¹î•ñ
struct DirectionalLightData
{
	DirectX::XMFLOAT4 direction;	// Œü‚«
	DirectX::XMFLOAT4 color;		// F
};

// “_ŒõŒ¹î•ñ
struct PointLightData
{
	DirectX::XMFLOAT4	position;	// À•W
	DirectX::XMFLOAT4	color;		// F
	float				range;		// ”ÍˆÍ
	DirectX::XMFLOAT3	dummy;
};

// “_ŒõŒ¹‚ÌÅ‘å”
static constexpr int PointLightMax = 8;

// ƒXƒ|ƒbƒgƒ‰ƒCƒg
struct SpotLightData
{
	DirectX::XMFLOAT4	position;	// À•W
	DirectX::XMFLOAT4	direction;	// Œü‚«
	DirectX::XMFLOAT4	color;		// F
	float				range;		// ”ÍˆÍ
	float				innerCorn;	// ƒCƒ“ƒi[Šp“x”ÍˆÍ
	float				outerCorn;	// ƒAƒEƒ^[Šp“x”ÍˆÍ
	float				dummy;
};

// ƒXƒ|ƒbƒgƒ‰ƒCƒg‚ÌÅ‘å”
static constexpr int SpotLightMax = 8;

// F’²•â³î•ñ
struct ColorGradingData
{
	float	hueShift = 0;	// F‘Š’²®
	float	saturation = 1;	// Ê“x’²®
	float	brightness = 1;	// –¾“x’²®
	float	dummy;
};

// ƒKƒEƒXƒtƒBƒ‹ƒ^[ŒvZî•ñ
struct GaussianFilterData
{
	int		kernelSize = 8;		// ƒJ[ƒlƒ‹ƒTƒCƒY
	float	deviation = 10.0f;	// •W€•Î·
	DirectX::XMFLOAT2 textureSize;	// ‚Ú‚©‚·ƒeƒNƒXƒ`ƒƒ‚ÌƒTƒCƒY
};

// ƒKƒEƒXƒtƒBƒ‹ƒ^[‚ÌÅ‘åƒJ[ƒlƒ‹ƒTƒCƒY
static const int MaxKernelSize = 16;

// ‚‹P“x’Šo—pî•ñ
struct LuminanceExtractionData
{
	float	threshold = 0.5f;	// è‡’l
	float	intensity = 1.0f;	// ƒuƒ‹[ƒ€‚Ì‹­“x
	DirectX::XMFLOAT2 dummy2;
};

// ƒ|ƒXƒgƒGƒtƒFƒNƒg‚ÌÅIƒpƒX—pî•ñ
struct FinalpassData
{
	// ƒuƒ‹[ƒ€ƒeƒNƒXƒ`ƒƒ
	ID3D11ShaderResourceView* bloomTexture;
};

//	ƒVƒƒƒhƒEƒ}ƒbƒv‚Ì–‡”
static	const	int	ShadowmapCount = 4;

// ƒVƒƒƒhƒEƒ}ƒbƒv—pî•ñ
struct ShadowMapData
{
	//ID3D11ShaderResourceView* shadowMap;				//	ƒVƒƒƒhƒEƒ}ƒbƒvƒeƒNƒXƒ`ƒƒ
	//DirectX::XMFLOAT4X4			lightViewProjection;	//	ƒ‰ƒCƒgƒrƒ…[ƒvƒƒWƒFƒNƒVƒ‡ƒ“s—ñ
	//DirectX::XMFLOAT3			shadowColor;			//	‰e‚ÌF
	//float						shadowBias;				//	[“x”äŠr—p‚ÌƒIƒtƒZƒbƒg’l

	//	ƒVƒƒƒhƒEƒ}ƒbƒv—p[“xƒXƒeƒ“ƒVƒ‹ƒoƒbƒtƒ@
	ID3D11ShaderResourceView* shadowMap[ShadowmapCount];
	//	ƒ‰ƒCƒgƒrƒ…[ƒvƒƒWƒFƒNƒVƒ‡ƒ“s—ñ
	DirectX::XMFLOAT4X4		lightViewProjection[ShadowmapCount];
	//	‰e‚ÌF
	DirectX::XMFLOAT3			shadowColor;
	//	[“x”äŠr—p‚ÌƒIƒtƒZƒbƒg’l
	float					shadowBias[ShadowmapCount];

};

=======
>>>>>>> parent of 8a0ff20 (ã¨ã‚Šã‚ãˆãšã‚·ã‚§ãƒ¼ãƒ€ãƒ¼ç¶ºéº—ãªå¥´å…¥ã£ãŸ)
// ƒŒƒ“ƒ_[ƒRƒ“ƒeƒLƒXƒg
struct RenderContext
{
	ID3D11DeviceContext* deviceContext;

<<<<<<< HEAD
	// ƒJƒƒ‰î•ñ
	DirectX::XMFLOAT4		viewPosition;
=======
>>>>>>> parent of 8a0ff20 (ã¨ã‚Šã‚ãˆãšã‚·ã‚§ãƒ¼ãƒ€ãƒ¼ç¶ºéº—ãªå¥´å…¥ã£ãŸ)
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	DirectX::XMFLOAT4		lightDirection;
	float                   alpha;
<<<<<<< HEAD


	// ƒ‰ƒCƒgî•ñ
	DirectX::XMFLOAT4		ambientLightColor;
	DirectionalLightData	directionalLightData;
	PointLightData			pointLightData[PointLightMax];	// “_ŒõŒ¹î•ñ
	SpotLightData			spotLightData[SpotLightMax];	// ƒXƒ|ƒbƒgƒ‰ƒCƒgî•ñ
	int						pointLightCount = 0;			// “_ŒõŒ¹”
	int						spotLightCount = 0;				// ƒXƒ|ƒbƒgƒ‰ƒCƒg”

	// F’²•â³î•ñ
	ColorGradingData		colorGradingData;

	// ƒKƒEƒXƒtƒBƒ‹ƒ^[î•ñ
	GaussianFilterData		gaussianFilterData;

	// ‚‹P“x’Šo—pî•ñ
	LuminanceExtractionData	luminanceExtractionData;

	// ƒVƒƒƒhƒEƒ}ƒbƒvî•ñ
	ShadowMapData			shadowMapData;
=======
>>>>>>> parent of 8a0ff20 (ã¨ã‚Šã‚ãˆãšã‚·ã‚§ãƒ¼ãƒ€ãƒ¼ç¶ºéº—ãªå¥´å…¥ã£ãŸ)
};
