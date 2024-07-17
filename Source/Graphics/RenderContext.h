#pragma once

#include <DirectXMath.h>

// �����_�[�R���e�L�X�g
struct RenderContext
{
	ID3D11DeviceContext* deviceContext;

	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	DirectX::XMFLOAT4		lightDirection;
	float                   alpha;
};
