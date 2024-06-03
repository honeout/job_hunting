#pragma once

#include <memory>
#include <wrl.h>
#include "Graphics/Shader.h"

class AfterImageShader : public ModelShader
{
public:
	AfterImageShader(ID3D11Device* device);
	~AfterImageShader() override {}

	void Begin( const RenderContext& rc) override;
	void Draw(const RenderContext& rc, const Model* model) override;
	void End(const RenderContext& rc) override;

	void SetAlpha(float alpha) { this->alpha = alpha; }
private:
	static const int MaxBones = 128;

	struct CbScene
	{
		DirectX::XMFLOAT4X4	viewProjection;
		DirectX::XMFLOAT4	lightDirection;
	};

	struct CbMesh
	{
		DirectX::XMFLOAT4X4	boneTransforms[MaxBones];
	};

	struct CbSubset
	{
		DirectX::XMFLOAT4	materialColor;
	};

	struct CbAlphaset
	{
		float               changeAlpha;
		DirectX::XMFLOAT3   dummy;
	};


	Microsoft::WRL::ComPtr<ID3D11Buffer>			sceneConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			meshConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			subsetConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			alphasetConstantBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>		vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		inputLayout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		samplerState;

	float alpha = 0;
};