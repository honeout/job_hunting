#pragma once

#include <memory>
#include <wrl.h>
#include "Graphics/Shader.h"

class FinalpassShader : public SpriteShader
{
public:
    FinalpassShader(ID3D11Device* device);
    ~FinalpassShader() override {}

    void Begin(const RenderContext& rc) override;
    void Draw(const RenderContext& rc, const Sprite* sprite)override;
    void End(const RenderContext& rc)override;

private:
    struct CBFinalpass
    {
        // 色調補正
        float hueShift; // 色相調整
        float saturation; // 彩度調整
        float brightness; // 明度調整
        float dummy;

    };

    // 周辺減光
    struct vignette_data
    {
        DirectX::XMFLOAT4 vignette_color = { 0.2f, 0.2f, 0.2f, 1.0f };
        DirectX::XMFLOAT2 vignette_center = { 0.5f, 0.5f };
        float vignette_intensity = 0.5f;
        float vignette_smoothness = 0.2f;

        bool vignette_rounded = false;
        float vignette_roundness = 1.0f;
    };
    struct vignette_constants
    {
        DirectX::XMFLOAT4 vignette_color;
        DirectX::XMFLOAT2 vignette_center;
        float vignette_intensity;
        float vignette_smoothness;

        float vignette_rounded;
        float vignette_roundness;
        DirectX::XMFLOAT2 vignette_dummy;
    };

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer>     finalpassConstatBuffer;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

    Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

    // 周辺減光
    vignette_data vignette_data;
    // UNIT05 05
    Microsoft::WRL::ComPtr<ID3D11Buffer> vignette_constant_buffer;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> vignette_pixel_shader;
};
