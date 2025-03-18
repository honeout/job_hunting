#pragma once

#include <memory>
#include <wrl.h>
#include "Graphics/Shader.h"

class PhonShader : public ModelShader
{
public:
    PhonShader(ID3D11Device* device);
    ~PhonShader() override {}

    void Begin(const RenderContext& rc)override;
    void Draw(const RenderContext& rc, const Model* model)override;
    void End(const RenderContext& rc)override;
private:
    static const int MaxBones = 128;

    struct CbScene
    {
        DirectX::XMFLOAT4  viewPosition;
        DirectX::XMFLOAT4X4 viewProjection;
        DirectX::XMFLOAT4 ambientLightColor;
        DirectionalLightData directionalLightData;

        PointLightData pointLightData[PointLightMax]; // �_����
        SpotLightData spotLightData[SpotLightMax]; // �X�|�b�g���C�g
        int            pointLightCount;               // �_������

        int            spotLightCount;                // �X�|�b�g���C�g��

        DirectX::XMFLOAT2 colorGB = {1,1};
        // �e�N�X�`���̓\��t���p
        int           texcoordMult;

        // �X�y�L�����[�p
        int           isSpecular;

        // �������C�g
        int           isRimRightning;

        int          StencilRef;
    };

    struct CbMesh
    {
        DirectX::XMFLOAT4X4 boneTransforms[MaxBones];
    };

    struct CbSubset
    {
        DirectX::XMFLOAT4 materialColor;
    };

    // UNIT11
    struct CbShadowMap
    {
        DirectX::XMFLOAT4X4 lightViewProjection; // ���C�g�r���[�v���W�F�N�V�����s��
        DirectX::XMFLOAT3 shadowColor; // �e�̐F
        float             shadowBias; // �[�x��r�p�̃I�t�Z�b�g�l
    };


    Microsoft::WRL::ComPtr<ID3D11Buffer> sceneConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> meshConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> subsetConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> shadowMapConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilMask;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> shadowMapSamplerState;
};
