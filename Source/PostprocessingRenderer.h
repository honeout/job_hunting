#pragma once

// UNIT09

#include "Graphics/Shader.h"

#include "Graphics/Sprite.h"
#include "Graphics/RenderTarget.h"

// シェーダーリソースビューの情報
struct ShaderResourceViewData
{
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
    int width = 0;
    int height = 0;
};

// ポストプロセス用のレンダラ―
class PostprocessingRenderer
{
    // ブルーム情報
    struct BloomData
    {
        // 高輝度抽出用情報
        LuminanceExtractionData luminanceExtractionData;
        // ガウスブラー用情報
        GaussianFilterData gaussianFilterData;
    };

public:
    PostprocessingRenderer();
    ~PostprocessingRenderer();

    // 描画
    void Render(RenderContext rc);

    // シーン情報の設定
    void SetSceneData(ShaderResourceViewData srvData);

    // デバッグ情報の表示
    void DrawDebugGUI();



private:
    // シーン描画データ
    ShaderResourceViewData sceneData;

    // 描画用スプライト
    std::unique_ptr<Sprite> renderSprite;

    // ブルームデータ
    BloomData bloomData;

    // 色調補正データ
    ColorGradingData    colorGradingData;

    // 高輝度抽出テクスチャ
    std::unique_ptr<RenderTarget> luminanceExtractRenderTarget;

    // 高輝度抽出ぼかしテクスチャ
    std::unique_ptr<RenderTarget> luminanceExtractBokehRenderTarget;

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
    vignette_data vignette_data;
    // UNIT05 05
    Microsoft::WRL::ComPtr<ID3D11Buffer> vignette_constant_buffer;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> vignette_pixel_shader;
};
