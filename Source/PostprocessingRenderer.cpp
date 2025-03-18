#include "Misc.h"
#include "PostprocessingRenderer.h"
#include "Graphics/Graphics.h"

PostprocessingRenderer::PostprocessingRenderer()
{
    Graphics& graphics = Graphics::Instance();

    renderSprite = std::make_unique<Sprite>();

    {
        UINT width = static_cast<UINT>(graphics.GetScreenWidth()) / 2;
        UINT height = static_cast<UINT>(graphics.GetScreenHeight()) / 2;
        // ���P�x���o�p����^�[�Q�b�g�𐶐�
        luminanceExtractRenderTarget = std::make_unique<RenderTarget>(width, height,
            DXGI_FORMAT_R8G8B8A8_UNORM);
        // ���P�x���o�ڂ����p�`��^�[�Q�b�g�𐶐�
        luminanceExtractBokehRenderTarget = std::make_unique<RenderTarget>(width, height,
            DXGI_FORMAT_R8G8B8A8_UNORM);
        bloomData.gaussianFilterData.textureSize.x = static_cast<float>(luminanceExtractBokehRenderTarget->
            GetWidth());
        bloomData.gaussianFilterData.textureSize.y = static_cast<float>(luminanceExtractBokehRenderTarget->
            GetHeight());
    }
}

PostprocessingRenderer::~PostprocessingRenderer()
{
}

void PostprocessingRenderer::Render(RenderContext rc)
{
    
    Graphics& graphics = Graphics::Instance();
    // ���ݐݒ肳��Ă���o�b�t�@����ď��������Ă���
    UINT         cachedViewportCount{ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE };
    D3D11_VIEWPORT cachedViewports[ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE ];
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> cachedRenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> cachedDepthStencilView;
    {
        rc.deviceContext->RSGetViewports(&cachedViewportCount, cachedViewports);
        rc.deviceContext->OMGetRenderTargets(1
            , cachedRenderTargetView.ReleaseAndGetAddressOf()
            , cachedDepthStencilView.ReleaseAndGetAddressOf()
        );
    }
    // �l���������߂�
    MoveTowards();
    // �Z�b�^�[�ύX�l�p
    {
        rc.bloomData.gaussianFilterData = bloomData.gaussianFilterData;
        rc.bloomData.luminanceExtractionData = bloomData.luminanceExtractionData;
        rc.gaussianFilterData.textureSize = bloomData.gaussianFilterData.textureSize;
        rc.colorGradingData = colorGradingData;
        rc.radialBlurData = radialBlurData;
        rc.vignetteData = vignetteData;
    }
    // ���P�x���o�p�o�b�t�@�ɕ`����ύX���č��P�x���o
    {
        ID3D11RenderTargetView* rtv = luminanceExtractRenderTarget->GetRenderTargetView().Get();
        FLOAT color[] = { 0.0f,0.0f,0.0f,0.0f };
        rc.deviceContext->ClearRenderTargetView(rtv, color);
        rc.deviceContext->OMSetRenderTargets(1, &rtv, nullptr);
        D3D11_VIEWPORT viewport{};
        viewport.Width = static_cast<float>(luminanceExtractRenderTarget->GetWidth());
        viewport.Height = static_cast<float>(luminanceExtractRenderTarget->GetHeight());
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        rc.deviceContext->RSSetViewports(cachedViewportCount, cachedViewports);
        // ���P�x���o����
        SpriteShader* shader = graphics.GetShader(SpriteShaderId::LuminanceExtraction);
        shader->Begin(rc);
        // �`��Ώۂ�ύX
        renderSprite->SetShaderResourceView(sceneData.srv, sceneData.width, sceneData.height);
        renderSprite->Update(0, 0, viewport.Width, viewport.Height,
            0, 0, static_cast<float>(sceneData.width), static_cast<float>(sceneData.height),
            0,
            1, 1, 1, 1);
        shader->Draw(rc, renderSprite.get());
        shader->End(rc);
    }

    // ���o�������P�`��Ώۂ�򂵂ď�������
    {
        // �`����ύX
        ID3D11RenderTargetView* rtv = luminanceExtractBokehRenderTarget->GetRenderTargetView().Get();
        FLOAT color[] = { 0.0f,0.0f,0.0f,0.0f };
        //      �`����ύX
        rc.deviceContext->ClearRenderTargetView(rtv, color);
        rc.deviceContext->OMSetRenderTargets(1, &rtv, nullptr);
        D3D11_VIEWPORT viewport{};
        viewport.Width = static_cast<float>(luminanceExtractBokehRenderTarget->GetWidth());
        viewport.Height = static_cast<float>(luminanceExtractBokehRenderTarget->GetHeight());
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        rc.deviceContext->RSSetViewports(cachedViewportCount, cachedViewports);
        // ���P�x���o����
        SpriteShader* shader = graphics.GetShader(SpriteShaderId::GaussianBlur);
        shader->Begin(rc);

        // �`��Ώۂ�ύX
        renderSprite->SetShaderResourceView(luminanceExtractRenderTarget->GetShaderResourceView(),
            luminanceExtractRenderTarget->GetWidth(),
            luminanceExtractRenderTarget->GetHeight());
        renderSprite->Update(0, 0, viewport.Width, viewport.Height,
            0, 0, static_cast<float>(luminanceExtractRenderTarget->GetWidth()),
            static_cast<float>(luminanceExtractRenderTarget->GetHeight()),
            0,
            1, 1, 1, 1);
        shader->Draw(rc, renderSprite.get());
        shader->End(rc);
    }

    // ���̃o�b�t�@�ɖ߂�
    {
        rc.deviceContext->RSSetViewports(cachedViewportCount, cachedViewports);
        rc.deviceContext->OMSetRenderTargets(1, cachedRenderTargetView.GetAddressOf(),
            cachedDepthStencilView.Get());
    }

    // �ŏI�p�X
    {
        // ���P�x���o����
        SpriteShader* shader = graphics.GetShader(SpriteShaderId::Finalpass);
        shader->Begin(rc);
#if	defined( UNIT_VN_01 )
//	�r�l�b�g���ݒ�
        //rc.vignetteData;

#endif	//	defined( UNIT_VN_01 )
        // �`��Ώۂ�ύX
        renderSprite->SetShaderResourceView(sceneData.srv,sceneData.width,sceneData.height);

        renderSprite->Update(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),
            0, 0, static_cast<float>(sceneData.width),static_cast<float>(sceneData.height),
            0,
            1, 1, 1, 1);
        // �V�F�[�_�[�ɓn���e�N�X�`����ݒ�
        rc.finalpassnData.bloomTexture = luminanceExtractBokehRenderTarget->GetShaderResourceView().Get();
    
        shader->Draw(rc, renderSprite.get());
    
        shader->End(rc);
    }
}

void PostprocessingRenderer::SetSceneData(ShaderResourceViewData srvData)
{
    sceneData = srvData;
}

void PostprocessingRenderer::DrawDebugGUI()
{
    if (ImGui::TreeNode("PostProcess"))
    {
    }
}

void PostprocessingRenderer::MoveTowards()
{
    // �l�㏸
    if (isIncreasingColorGrading)
    {
        // �J���[�O���f�B�G���X
        colorGradingData.brigthness = colorGradingDataMax.brigthness + FLT_EPSILON <= colorGradingData.brigthness - FLT_EPSILON ?
            colorGradingDataMax.brigthness : colorGradingData.brigthness + stepValueColor;
        colorGradingData.hueShift = colorGradingDataMax.hueShift + FLT_EPSILON <= colorGradingData.hueShift - FLT_EPSILON ?
            colorGradingDataMax.hueShift : colorGradingData.hueShift + stepValueColor;
        colorGradingData.saturation = colorGradingDataMax.saturation + FLT_EPSILON <= colorGradingData.saturation - FLT_EPSILON ?
            colorGradingDataMax.saturation : colorGradingData.saturation + stepValueColor;
    }
    // �l����
    else
    {
        // �J���[�O���f�B�G���X
        colorGradingData.brigthness = colorGradingDataMin.brigthness + FLT_EPSILON >= colorGradingData.brigthness - FLT_EPSILON ?
            colorGradingDataMin.brigthness : colorGradingData.brigthness - stepValueColor;
        colorGradingData.hueShift = colorGradingDataMin.hueShift + FLT_EPSILON >= colorGradingData.hueShift - FLT_EPSILON ?
            colorGradingDataMin.hueShift : colorGradingData.hueShift - stepValueColor;
        colorGradingData.saturation = colorGradingDataMin.saturation + FLT_EPSILON >= colorGradingData.saturation - FLT_EPSILON ?
            colorGradingDataMin.saturation : colorGradingData.saturation - stepValueColor;
    }
    // �l�㏸
    if (isIncreasingRadialBlur)
    {
        // ���W�A���u���[
        radialBlurData.radius = radialBlurDataMax.radius + FLT_EPSILON <= radialBlurData.radius - FLT_EPSILON ?
            radialBlurDataMax.radius : radialBlurData.radius + stepValueRadial;
        radialBlurData.samplingCount = (int)(radialBlurDataMax.samplingCount + FLT_EPSILON <= radialBlurData.samplingCount - FLT_EPSILON ?
            radialBlurDataMax.samplingCount : radialBlurData.samplingCount + stepValueRadial);
        radialBlurData.mask_radius = radialBlurDataMax.mask_radius + FLT_EPSILON <= radialBlurData.mask_radius - FLT_EPSILON ?
            radialBlurDataMax.mask_radius : radialBlurData.mask_radius + stepValueRadial;
    }
    // �l����
    else
    {
        // ���W�A���u���[
        radialBlurData.radius = radialBlurDataMin.radius + FLT_EPSILON >= radialBlurData.radius - FLT_EPSILON ?
            radialBlurDataMin.radius : radialBlurData.radius - stepValueRadial;
        radialBlurData.samplingCount = (int)(radialBlurDataMin.samplingCount + FLT_EPSILON >= radialBlurData.samplingCount - FLT_EPSILON ?
            radialBlurDataMin.samplingCount : radialBlurData.samplingCount - stepValueRadial);
        radialBlurData.mask_radius = radialBlurDataMin.mask_radius + FLT_EPSILON >= radialBlurData.mask_radius - FLT_EPSILON ?
            radialBlurDataMin.mask_radius : radialBlurData.mask_radius + stepValueRadial;
    }
    // �㏸�I��
    isIncreasingColorGrading = false;
    isIncreasingRadialBlur = false;

    if (!isIncreasingModeVignetteData)
    {
        // ���B�W�F�b�g
        vignetteData.intensity = vignetteDataMax.intensity + FLT_EPSILON <= vignetteData.intensity - FLT_EPSILON ?
            vignetteDataMax.intensity : vignetteData.intensity + stepValuevignette;
        vignetteData.intensity = vignetteDataMin.intensity + FLT_EPSILON >= vignetteData.intensity - FLT_EPSILON ?
            vignetteDataMin.intensity : vignetteData.intensity - stepValuevignette;
        return;
    }
    
    // �l�㏸
    if (isIncreasingVignetteData)
    {
        // ���B�W�F�b�g
        vignetteData.intensity = vignetteDataMax.intensity + FLT_EPSILON <= vignetteData.intensity - FLT_EPSILON ?
            vignetteDataMax.intensity : vignetteData.intensity + stepValuevignette;

        vignetteData.smoothness = vignetteDataMax.smoothness + FLT_EPSILON <= vignetteData.smoothness - FLT_EPSILON ?
            vignetteDataMax.smoothness : vignetteData.smoothness + stepValuevignette;

        isIncreasingVignetteData = vignetteDataMax.intensity + FLT_EPSILON <= vignetteData.intensity - FLT_EPSILON ?
            false : true;
    }
    // �l����
    else
    {
        // ���B�W�F�b�g
        vignetteData.intensity = vignetteDataMin.intensity + FLT_EPSILON >= vignetteData.intensity - FLT_EPSILON ?
            vignetteDataMin.intensity : vignetteData.intensity - stepValuevignette;

        vignetteData.smoothness = vignetteDataMin.smoothness + FLT_EPSILON >= vignetteData.smoothness - FLT_EPSILON ?
            vignetteDataMin.smoothness : vignetteData.smoothness - stepValuevignette;


        isIncreasingVignetteData = vignetteDataMin.intensity + FLT_EPSILON >= vignetteData.intensity - FLT_EPSILON ?
            true : false;
    }
    isIncreasingModeVignetteData = false;
}
