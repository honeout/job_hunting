#pragma once
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
public:
    PostprocessingRenderer();
    ~PostprocessingRenderer();

    static PostprocessingRenderer& Instance()
    {
        // �l�X�ȏ��Ŏ��o������������V���O���g���ɂ���
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
    // �F��ς���
    void SetVignetteDataColor(DirectX::XMFLOAT4	vignetteDataColor) {

        this->vignetteData.color = vignetteDataColor;
    }

    // �u���[���Œ�l�f�[�^
    void SetBloomMinData(BloomData bloomMinData) { this->bloomDataMin = bloomMinData; }
    // �F���␳�Œ�l�f�[�^
    void SetColorGradingMinData(ColorGradingData    colorGradingMinData) { this->colorGradingDataMin = colorGradingMinData; }
    // ���W�A���u���[�Œ�l���
    void SetRadialBlurMinData(RadialBlurData radialBlurMinData) { this->radialBlurDataMin = radialBlurMinData; }
    // �_���[�W�Œ�l�\��
    void SetVignetteMinData(VignetteData	vignetteMinData) { 
        isIncreasingModeVignetteData = true;

        this->vignetteDataMin = vignetteMinData; 
    }

    // ���Z
    void StepValueColor(float add) {
        stepValueColor = add;
    }
    // ���Z
    void StepValueRadial(float add) {
        stepValueRadial = add;
    }
    // ���Z
    void StepValuevignette(float add) {
        stepValuevignette = add;
    }

    // �Œ�l
    void StepValueMin() {
        stepValueColor = stepValueColorMin;
        stepValueRadial = stepValueRadialMin;
        stepValuevignette = stepValuevignetteMin;
    }

private:
    // �V�[���`��f�[�^
    ShaderResourceViewData sceneData;

    // �`��p�X�v���C�g
    std::unique_ptr<Sprite> renderSprite;

    // ���P�x���o�e�N�X�`��
    std::unique_ptr<RenderTarget> luminanceExtractRenderTarget;

    // ���P�x���o�ڂ����e�N�X�`��
    std::unique_ptr<RenderTarget> luminanceExtractBokehRenderTarget;

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
    float stepValueColorMin = 0.03f;
    float stepValueRadial = 0.5f;
    float stepValueRadialMin = 5.5f;
    float stepValuevignette = 0.03f;
    float stepValuevignetteMin = 0.03f;

    // �l�̕ω��`�F�b�N
    bool isIncreasingBloom = false;
    bool isIncreasingColorGrading = false;
    bool isIncreasingRadialBlur = false;
    bool isIncreasingModeVignetteData = false;
    bool isIncreasingVignetteData = false;
};
