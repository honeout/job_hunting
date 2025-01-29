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
    //// �u���[�����
    //struct BloomData
    //{
    //    // ���P�x���o�p���
    //    LuminanceExtractionData luminanceExtractionData;
    //    // �K�E�X�u���[�p���
    //    GaussianFilterData gaussianFilterData;
    //};

public:
    PostprocessingRenderer();
    ~PostprocessingRenderer();

    static PostprocessingRenderer& Instance()
    {
        // �J�����͗l�X�ȏ��Ŏ��o������������V���O���g���ɂ���
        static PostprocessingRenderer postEffect;
        return postEffect;
    }

    // �`��
    void Render(RenderContext rc);

    // �V�[�����̐ݒ�
    void SetSceneData(ShaderResourceViewData srvData);

    // �f�o�b�O���̕\��
    void DrawDebugGUI();

    // �l��ω�
    void MoveTowards();

    // ��C�ɂ�������ς���B
    // �u���[���f�[�^
    void SetBloomData(BloomData bloomDataMax) {
        this->bloomDataMax = bloomDataMax;
      
    }
    // �F���␳�f�[�^
    void SetColorGradingData(ColorGradingData    colorGradingDataMax) {
   
        this->colorGradingData = colorGradingDataMax;
    }
    // ���W�A���u���[���
    void SetRadialBlurData(RadialBlurData radialBlurDataMax) {

        this->radialBlurData = radialBlurDataMax;
    }
    // �_���[�W�\��
    void SetVignetteData(VignetteData	vignetteDataMax) {

        this->vignetteData = vignetteDataMax;
    }


    // �u���[���f�[�^
    void SetBloomMaxData(BloomData bloomDataMax) {
        this->bloomDataMax = bloomDataMax;
        isIncreasingBloom = true;
    }
    // �F���␳�f�[�^
    void SetColorGradingMaxData(ColorGradingData    colorGradingDataMax) {
        isIncreasingColorGrading = true;
        this->colorGradingDataMax = colorGradingDataMax;
    }
    // ���W�A���u���[���
    void SetRadialBlurMaxData(RadialBlurData radialBlurDataMax) {
        isIncreasingRadialBlur = true;
        this->radialBlurDataMax = radialBlurDataMax;
    }
    // �_���[�W�\��
    void SetVignetteMaxData(VignetteData	vignetteDataMax) {
        isIncreasingModeVignetteData = true;
        this->vignetteDataMax = vignetteDataMax;
    }


    // �u���[���Œ�l�f�[�^
    void SetBloomMinData(BloomData bloomMinData) { this->bloomDataMin = bloomMinData; }
    // �F���␳�Œ�l�f�[�^
    void SetColorGradingMinData(ColorGradingData    colorGradingMinData) { this->colorGradingDataMin = colorGradingMinData; }
    // ���W�A���u���[�Œ�l���
    void SetRadialBlurMinData(RadialBlurData radialBlurMinData) { this->radialBlurDataMin = radialBlurMinData; }
    // �_���[�W�Œ�l�\��
    void SetVignetteMinData(VignetteData	vignetteMinData) { this->vignetteDataMin = vignetteMinData; }

private:
    // �V�[���`��f�[�^
    ShaderResourceViewData sceneData;

    // �`��p�X�v���C�g
    std::unique_ptr<Sprite> renderSprite;

    // ���P�x���o�e�N�X�`��
    std::unique_ptr<RenderTarget> luminanceExtractRenderTarget;

    // ���P�x���o�ڂ����e�N�X�`��
    std::unique_ptr<RenderTarget> luminanceExtractBokehRenderTarget;

    //struct vignette_data
    //{
    //    DirectX::XMFLOAT4 vignette_color = { 0.2f, 0.2f, 0.2f, 1.0f };
    //    DirectX::XMFLOAT2 vignette_center = { 0.5f, 0.5f };
    //    float vignette_intensity = 0.5f;
    //    float vignette_smoothness = 0.2f;

    //    bool vignette_rounded = false;
    //    float vignette_roundness = 1.0f;
    //};
    //struct vignette_constants
    //{
    //    DirectX::XMFLOAT4 vignette_color;
    //    DirectX::XMFLOAT2 vignette_center;
    //    float vignette_intensity;
    //    float vignette_smoothness;

    //    float vignette_rounded;
    //    float vignette_roundness;
    //    DirectX::XMFLOAT2 vignette_dummy;
    //};
    //vignette_data vignette_data;
    //// UNIT05 05
    //Microsoft::WRL::ComPtr<ID3D11Buffer> vignette_constant_buffer;
    //Microsoft::WRL::ComPtr<ID3D11PixelShader> vignette_pixel_shader;

    // �u���[���f�[�^
    BloomData bloomData;

    // �F���␳�f�[�^
    ColorGradingData    colorGradingData;


// ���W�A���u���[���
    RadialBlurData  radialBlurData;


#if	defined( UNIT_VN_01 )
    //	�r�l�b�g���
    VignetteData					vignetteData;
    
#endif	//	defined( UNIT_VN_01 )

    // �u���[���f�[�^
    BloomData bloomDataMax;

    // �F���␳�f�[�^
    ColorGradingData    colorGradingDataMax;


    // ���W�A���u���[���
    RadialBlurData  radialBlurDataMax;



    //	�r�l�b�g���
    VignetteData					vignetteDataMax;


    // �u���[���f�[�^
    BloomData bloomDataMin;

    // �F���␳�f�[�^
    ColorGradingData    colorGradingDataMin;


    // ���W�A���u���[���
    RadialBlurData  radialBlurDataMin;



    //	�r�l�b�g���
    VignetteData					vignetteDataMin;

    // �l������������
    float stepValueColor = 0.03f;
    float stepValueRadial = 0.5f;
    float stepValuevignette = 0.03f;
    //float stepDownValue = 0.03f;

    // �l�̕ω��`�F�b�N
    bool isIncreasingBloom = false;
    bool isIncreasingColorGrading = false;
    bool isIncreasingRadialBlur = false;
    bool isIncreasingModeVignetteData = false;
    bool isIncreasingVignetteData = false;

};
