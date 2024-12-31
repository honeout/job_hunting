#include "Misc.h"
//#include "GpuResourceUtils.h"
#include "PrimitiveRenderer.h"

// �R���X�g���N�^
PrimitiveRenderer::PrimitiveRenderer(ID3D11Device* device)
{

	//// ���_�V�F�[�_�[
	//GpuResourceUtils::LoadVertexShader(
	//	device,
	//	"Data/Shader/PrimitiveRendererVS.cso",
	//	inputElementDesc,
	//	_countof(inputElementDesc),
	//	inputLayout.GetAddressOf(),
	//	vertexShader.GetAddressOf());

	// ���_�V�F�[�_�[
	{
		// �t�@�C�����J��
		FILE* fp = nullptr;
		fopen_s(&fp, "Shader\\PrimitiveRendererVS.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");
		// �t�@�C���̃T�C�Y�����߂�
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// ��������ɒ��_�V�F�[�_�[�f�[�^���i�[����̈��p�ӂ���
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		// ���_�V�F�[�_�[����
		HRESULT hr = device->CreateVertexShader(csoData.get(), csoSize, nullptr, vertexShader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// ���̓��C�A�E�g
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), csoData.get(), csoSize, inputLayout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}



	//// �s�N�Z���V�F�[�_�[
	//GpuResourceUtils::LoadPixelShader(
	//	device,
	//	"Data/Shader/PrimitiveRendererPS.cso",
	//	pixelShader.GetAddressOf());

	// �s�N�Z���V�F�[�_�[
	{
		// �t�@�C�����J��
		FILE* fp = nullptr;
		fopen_s(&fp, "Shader\\PrimitiveRendererPS.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// �t�@�C���̃T�C�Y�����߂�
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// ��������Ƀs�N�Z���V�F�[�_�[�f�[�^���i�[����̈���悤������
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		// �s�N�Z���V�F�[�_�[����
		HRESULT hr = device->CreatePixelShader(csoData.get(), csoSize, nullptr,
			pixelShader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}



	//// �萔�o�b�t�@
	//GpuResourceUtils::CreateConstantBuffer(
	//	device,
	//	sizeof(CbScene),
	//	constantBuffer.GetAddressOf());



	//�萔�o�b�t�@
	{
		// �V�[���p�o�b�t�@
		D3D11_BUFFER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(CbScene);
		desc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&desc, 0, constantBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}



	// ���_�o�b�t�@
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(Vertex) * VertexCapacity;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	HRESULT hr = device->CreateBuffer(&desc, nullptr, vertexBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// ���X�^���C�U�[�X�e�[�g
	{
		D3D11_RASTERIZER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.FrontCounterClockwise = false;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = device->CreateRasterizerState(&desc, rasterizerState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
}

void PrimitiveRenderer::Begin(const RenderContext& rc)
{
	// �V�F�[�_�[�ݒ�
	rc.deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	rc.deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	rc.deviceContext->IASetInputLayout(inputLayout.Get());

	// �萔�o�b�t�@�ݒ�
	rc.deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

	// ���X�^���C�U�[�X�e�[�g�ݒ�
	rc.deviceContext->RSSetState(rasterizerState.Get());
}

void PrimitiveRenderer::Draw(const RenderContext& rc, const Sprite* sprite)
{


	// �r���[�v���W�F�N�V�����s��쐬
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&rc.projection);
	DirectX::XMMATRIX VP = V * P;

	// �萔�o�b�t�@�X�V
	CbScene cbScene;
	//cbScene.color = rc.colorGradingData;
	cbScene.color = rc.swordTraileData.color;
	DirectX::XMStoreFloat4x4(&cbScene.viewProjection, VP);
	rc.deviceContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &cbScene, 0, 0);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	rc.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	rc.deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	// �`��
	UINT totalVertexCount = static_cast<UINT>(vertices.size());
	UINT start = 0;
	UINT count = (totalVertexCount < VertexCapacity) ? totalVertexCount : VertexCapacity;

	while (start < totalVertexCount)
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		HRESULT hr = rc.deviceContext->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		memcpy(mappedSubresource.pData, &vertices[start], sizeof(Vertex) * count);

		rc.deviceContext->Unmap(vertexBuffer.Get(), 0);

		rc.deviceContext->Draw(count, 0);

		start += count;
		if ((start + count) > totalVertexCount)
		{
			count = totalVertexCount - start;
		}
	}
}

void PrimitiveRenderer::End(const RenderContext& rc)
{
	vertices.clear();

	rc.deviceContext->VSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->PSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->IASetInputLayout(nullptr);

	rc.deviceContext->RSSetState(nullptr);

}

// ���_�ǉ�
void PrimitiveRenderer::AddVertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& color)
{
	Vertex& v = vertices.emplace_back();
	v.position = position;
	v.color = color;
}

// ���`��
void PrimitiveRenderer::DrawAxis(const DirectX::XMFLOAT4X4& transform, const DirectX::XMFLOAT4& color)
{
	DirectX::XMMATRIX W = DirectX::XMLoadFloat4x4(&transform);
	DirectX::XMFLOAT3 p, x, y, z;
	DirectX::XMStoreFloat3(&p, W.r[3]);
	DirectX::XMStoreFloat3(&x, DirectX::XMVector3Transform(DirectX::XMVectorSet(1, 0, 0, 0), W));
	DirectX::XMStoreFloat3(&y, DirectX::XMVector3Transform(DirectX::XMVectorSet(0, 1, 0, 0), W));
	DirectX::XMStoreFloat3(&z, DirectX::XMVector3Transform(DirectX::XMVectorSet(0, 0, 1, 0), W));
	AddVertex(p, { 1, 0, 0, 1 });
	AddVertex(x, { 1, 0, 0, 1 });
	AddVertex(p, { 0, 1, 0, 1 });
	AddVertex(y, { 0, 1, 0, 1 });
	AddVertex(p, { 0, 0, 1, 1 });
	AddVertex(z, { 0, 0, 1, 1 });
}

// �O���b�h�`��
void PrimitiveRenderer::DrawGrid(int subdivisions, float scale)
{
	int numLines = (subdivisions + 1) * 2;
	int vertexCount = numLines * 2;

	float corner = 0.5f;
	float step = 1.0f / static_cast<float>(subdivisions);

	int index = 0;
	float s = -corner;

	const DirectX::XMFLOAT4 white = DirectX::XMFLOAT4(1, 1, 1, 1);

	// Create vertical lines
	float scaling = static_cast<float>(subdivisions) * scale;
	DirectX::XMMATRIX M = DirectX::XMMatrixScaling(scaling, scaling, scaling);
	DirectX::XMVECTOR V, P;
	DirectX::XMFLOAT3 position;
	for (int i = 0; i <= subdivisions; i++)
	{
		V = DirectX::XMVectorSet(s, 0, corner, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, white);

		V = DirectX::XMVectorSet(s, 0, -corner, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, white);

		s += step;
	}

	// Create horizontal lines
	s = -corner;
	for (int i = 0; i <= subdivisions; i++)
	{
		V = DirectX::XMVectorSet(corner, 0, s, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, white);

		V = DirectX::XMVectorSet(-corner, 0, s, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, white);

		s += step;
	}

	// X��
	{
		const DirectX::XMFLOAT4 red = DirectX::XMFLOAT4(1, 0, 0, 1);
		V = DirectX::XMVectorSet(0, 0, 0, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, red);

		V = DirectX::XMVectorSet(corner, 0, 0, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, red);
	}

	// Y��
	{
		const DirectX::XMFLOAT4 green = DirectX::XMFLOAT4(0, 1, 0, 1);
		V = DirectX::XMVectorSet(0, 0, 0, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, green);

		V = DirectX::XMVectorSet(0, corner, 0, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, green);
	}

	// Z��
	{
		const DirectX::XMFLOAT4 blue = DirectX::XMFLOAT4(0, 0, 1, 1);
		V = DirectX::XMVectorSet(0, 0, 0, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, blue);

		V = DirectX::XMVectorSet(0, 0, corner, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		AddVertex(position, blue);
	}
}

// �`����s
void PrimitiveRenderer::Render(
	ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection,
	D3D11_PRIMITIVE_TOPOLOGY primitiveTopology)
{
	// �V�F�[�_�[�ݒ�
	dc->VSSetShader(vertexShader.Get(), nullptr, 0);
	dc->PSSetShader(pixelShader.Get(), nullptr, 0);
	dc->IASetInputLayout(inputLayout.Get());

	// �萔�o�b�t�@�ݒ�
	dc->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

	// ���X�^���C�U�[�X�e�[�g�ݒ�
	dc->RSSetState(rasterizerState.Get());

	// �r���[�v���W�F�N�V�����s��쐬
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX VP = V * P;

	// �萔�o�b�t�@�X�V
	CbScene cbScene;
	DirectX::XMStoreFloat4x4(&cbScene.viewProjection, VP);
	dc->UpdateSubresource(constantBuffer.Get(), 0, 0, &cbScene, 0, 0);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	dc->IASetPrimitiveTopology(primitiveTopology);
	dc->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	// �`��
	UINT totalVertexCount = static_cast<UINT>(vertices.size());
	UINT start = 0;
	UINT count = (totalVertexCount < VertexCapacity) ? totalVertexCount : VertexCapacity;

	while (start < totalVertexCount)
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		HRESULT hr = dc->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		memcpy(mappedSubresource.pData, &vertices[start], sizeof(Vertex) * count);

		dc->Unmap(vertexBuffer.Get(), 0);

		dc->Draw(count, 0);

		start += count;
		if ((start + count) > totalVertexCount)
		{
			count = totalVertexCount - start;
		}
	}

	vertices.clear();
}
