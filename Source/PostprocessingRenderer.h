#pragma once

// UNIT09

#include "Graphics/Shader.h"

#include "Graphics/Sprite.h"
#include "Graphics/RenderTarget.h"

// �V�F�[�_�[���\�[�X�r���[�̏��
struct ShaderResourceViewData
{
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
    int width = 0;
    int height = 0;
};

// �|�X�g�v���Z�X�p�̃����_���\
class PostprocessingRenderer
{
    // �u���[�����
    struct BloomData
    {
        // ���P�x���o�p���
        LuminanceExtractionData luminanceExtractionData;
        // �K�E�X�u���[�p���
        GaussianFilterData gaussianFilterData;
    };

public:
    PostprocessingRenderer();
    ~PostprocessingRenderer();

    // �`��
    void Render(RenderContext rc);

    // �V�[�����̐ݒ�
    void SetSceneData(ShaderResourceViewData srvData);

    // �f�o�b�O���̕\��
    void DrawDebugGUI();



private:
    // �V�[���`��f�[�^
    ShaderResourceViewData sceneData;

    // �`��p�X�v���C�g
    std::unique_ptr<Sprite> renderSprite;

    // �u���[���f�[�^
    BloomData bloomData;

    // �F���␳�f�[�^
    ColorGradingData    colorGradingData;

    // ���P�x���o�e�N�X�`��
    std::unique_ptr<RenderTarget> luminanceExtractRenderTarget;

    // ���P�x���o�ڂ����e�N�X�`��
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
