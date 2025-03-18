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


        //	ラジアルブラー情報
        float				rb_radius;
        int					rb_samplingCount;
        DirectX::XMFLOAT2	rb_center;


        float				rb_mask_radius;
        DirectX::XMFLOAT3	rb_dummy;


#if	defined( UNIT_VN_01 )
        //	ビネット情報
        DirectX::XMFLOAT4	vn_color;
        DirectX::XMFLOAT2	vn_center;
        float				vn_intensity;
        float				vn_smoothness;

        float				vn_rounded;
        float				vn_roundness;
        float				vn_iris;
        float				vn_dummy;
#endif	//	defined( UNIT_VN_01 )
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
};
