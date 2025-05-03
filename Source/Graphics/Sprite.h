#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <map>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include "Sprite.h"

// スプライト
class Sprite
{
public:
	Sprite();
	Sprite(const char* filename);
    ~Sprite() {}



	struct Vertex
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT2	texcoord;
	};

	// バッファ更新
	void Update(
		float dx, float dy,
		float dw, float dh,
		float sx, float sy,
		float sw, float sh,
		float angle,
		float r, float g, float b, float a) const;

	// 描画実行
	void Render(ID3D11DeviceContext *dc,
		float dx, float dy,
		float dw, float dh,
		float sx, float sy,
		float sw, float sh,
		float angle,
		float r, float g, float b, float a) const;

	// アニメーションフレーム
	struct AnimFrame
	{
		// 描画領域(UV短形)
		RECT rect;
		// このアニメーションの再生時間
		float duration;
	};

	// アニメーションの種類
	struct AnimClip
	{
		std::string name;
		std::vector<AnimFrame> frames;
		bool isLooping;
	};

	// アニメーションの追加
	void AddAnim(const AnimClip& clip);

	// アニメーションの選択
	void PlayAnimation(const std::string& name, bool forceRestart = false);
	
	// スプライトのアニメーション再生
	void UpdateAnimation(float elapsedTime);

	// シェーダーリソースビューの設定
	void SetShaderResourceView(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& srv, int texWidth, int texHeight);

	// 頂点バッファの取得
	const Microsoft::WRL::ComPtr<ID3D11Buffer>& GetVertexBuffer() const { return vertexBuffer; }
	
	// シェーダーリソースビューの取得
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const { return shaderResourceView; }


	// テクスチャ幅取得
	int GetTextureWidth() const { return textureWidth; }

	// テクスチャ高さ取得
	int GetTextureHeight() const { return textureHeight; }
private:
	struct Texture
	{
		// 今のアニメーションの画像位置
		int animIndex;
		// 元画像の位置１ポーズ
		DirectX::XMFLOAT2 texPos;
		// 元画像の全体
		DirectX::XMFLOAT2 texPosMax;
		// 余白
		float top = 0.0f;
		float left = 0.0f;
		// 何処を参照するか
		DirectX::XMFLOAT2 spriteSheat;
	};

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				vertexBuffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView;

	int textureWidth = 0;
	int textureHeight = 0;

	// アニメーションの種類
	std::map<std::string, AnimClip> animationClips;
	const AnimClip* currentClip = nullptr;
	int currentFrameIndex = 0;
	float frameTimer = 0.0f;


};