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

        //UINT width = static_cast<float>(50);
        //UINT height = static_cast<float>(50);

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
    ////	�萔�o�b�t�@�̐���
    //{
    //    D3D11_BUFFER_DESC buffer_desc{};
    //    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    //    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    //    buffer_desc.CPUAccessFlags = 0;
    //    buffer_desc.MiscFlags = 0;
    //    buffer_desc.StructureByteStride = 0;
    //    //	���B�l�b�g�p�萔�o�b�t�@
    //    {
    //        
    //        buffer_desc.ByteWidth = sizeof(vignette_constants);
    //        HRESULT hr = Graphics::Instance().GetDevice()->CreateBuffer(&buffer_desc, nullptr, vignette_constant_buffer.GetAddressOf());
    //        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    //    }


    //    //	�X�v���C�g�V�F�[�_�[����
    //    {
    //        //	���B�l�b�g�V�F�[�_�[
    //        //create_ps_from_cso(Graphics::Instance().GetDevice(), "vignette_ps.cso", vignette_pixel_shader.GetAddressOf());
    //        // �t�@�C�����J��
    //        FILE* fp{ nullptr };
    //        fopen_s(&fp, "Shader\\vignette_ps.cso", "rb");
    //        _ASSERT_EXPR_A(fp, "CSO File not found");

    //        fseek(fp, 0, SEEK_END);
    //        long cso_sz{ ftell(fp) };
    //        fseek(fp, 0, SEEK_SET);

    //        std::unique_ptr<unsigned char[]> cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
    //        fread(cso_data.get(), cso_sz, 1, fp);
    //        fclose(fp);

    //        HRESULT hr{ S_OK };
    //        hr = Graphics::Instance().GetDevice()->CreatePixelShader(cso_data.get(), cso_sz, nullptr, vignette_pixel_shader.GetAddressOf());
    //        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    //    }
    //}

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

    //RenderContext rc;
    
    //rc.luminanceExtractionData = rc.bloomData.luminanceExtractionData;
    //rc.gaussianFilterData = rc.bloomData.gaussianFilterData;

    //rc.gaussianFilterData.textureSize = bloomData.gaussianFilterData.textureSize;

    // ���P�x���o�p�o�b�t�@�ɕ`����ύX���č��P�x���o
    {
        //      �`����ύX
        //Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = luminanceExtractRenderTarget->GetRenderTargetView().Get();
        ID3D11RenderTargetView* rtv = luminanceExtractRenderTarget->GetRenderTargetView().Get();
        FLOAT color[] = { 0.0f,0.0f,0.0f,0.0f };
        rc.deviceContext->ClearRenderTargetView(rtv, color);
        rc.deviceContext->OMSetRenderTargets(1, &rtv, nullptr);
        D3D11_VIEWPORT viewport{};
        viewport.Width = static_cast<float>(luminanceExtractRenderTarget->GetWidth());
        viewport.Height = static_cast<float>(luminanceExtractRenderTarget->GetHeight());
        //viewport.Width = static_cast<float>(50);
        //viewport.Height = static_cast<float>(50);
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
        //Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = luminanceExtractBokehRenderTarget->GetRenderTargetView().Get();
        FLOAT color[] = { 0.0f,0.0f,0.0f,0.0f };
        //      �`����ύX
        rc.deviceContext->ClearRenderTargetView(rtv, color);
        rc.deviceContext->OMSetRenderTargets(1, &rtv, nullptr);
        D3D11_VIEWPORT viewport{};
        viewport.Width = static_cast<float>(luminanceExtractBokehRenderTarget->GetWidth());
        viewport.Height = static_cast<float>(luminanceExtractBokehRenderTarget->GetHeight());
        //viewport.Width = static_cast<float>(100);
        //viewport.Height = static_cast<float>(100);
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



        ////	���B�l�b�g�p�萔�o�b�t�@
        //{
        //    static constexpr int VignetteCBVIndex = 2;
        //    vignette_constants constant;
        //    // Todo���ۂ̏�����ImGui�ł�����邩�̈Ⴂ
        //    //constant.vignette_color = vignette_data.vignette_color;
        //    constant.vignette_color = rc.vignette_color;
        //    //constant.vignette_center = vignette_data.vignette_center;
        //    constant.vignette_center = rc.vignette_center;
        //    //constant.vignette_intensity = vignette_data.vignette_intensity * 3.0f;
        //    constant.vignette_intensity = rc.vignette_intensity * 3.0f;
        //    //constant.vignette_smoothness = max(0.000001f, vignette_data.vignette_smoothness * 5.0f);
        //    constant.vignette_smoothness = max(0.000001f, rc.vignette_smoothness * 5.0f);
        //    constant.vignette_rounded = vignette_data.vignette_rounded ? 1.0f : 0.0f;
        //    constant.vignette_roundness = 6.0f * (1.0f - vignette_data.vignette_roundness) + vignette_data.vignette_roundness;


        //    rc.deviceContext->UpdateSubresource(vignette_constant_buffer.Get(), 0, 0, &constant, 0, 0);
        //    //Graphics::Instance().GetDevice()->set_constant_buffer(VignetteCBVIndex, 1, vignette_constant_buffer.GetAddressOf());
        //    rc.deviceContext->VSSetConstantBuffers(VignetteCBVIndex, 1, vignette_constant_buffer.GetAddressOf());
        //    rc.deviceContext->HSSetConstantBuffers(VignetteCBVIndex, 1, vignette_constant_buffer.GetAddressOf());
        //    rc.deviceContext->DSSetConstantBuffers(VignetteCBVIndex, 1, vignette_constant_buffer.GetAddressOf());
        //    rc.deviceContext->GSSetConstantBuffers(VignetteCBVIndex, 1, vignette_constant_buffer.GetAddressOf());
        //    rc.deviceContext->PSSetConstantBuffers(VignetteCBVIndex, 1, vignette_constant_buffer.GetAddressOf());
        //    rc.deviceContext->CSSetConstantBuffers(VignetteCBVIndex, 1, vignette_constant_buffer.GetAddressOf());

        //}
        //rc.deviceContext->PSSetShader(vignette_pixel_shader.Get(), nullptr, 0);


//	���W�A���u���[���ݒ�
      //  rc.radialBlurData;


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
        //rc.colorGradingData = colorGradingData;


    
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
        //if (ImGui::TreeNode("Bloom"))
        //{
        //    ImGui::SliderFloat("threshold", &bloomData.luminanceExtractionData.threshold, 0.0f,1.0f);
        //    ImGui::SliderFloat("intensity", &bloomData.luminanceExtractionData.intensity, 0.0f,10.0f);
        //    ImGui::SliderInt("kernelSize", &bloomData.gaussianFilterData.kernelSize, 1, MaxkernelSize-1);
        //    ImGui::SliderFloat("deviation", &bloomData.gaussianFilterData.deviation, 1.0f, 10.0f);
        //    ImGui::TreePop();

        //}

        //if (ImGui::TreeNode("ColorGrading"))
        //{
        //    ImGui::SliderFloat("hueShift", &colorGradingData.hueShift, 0.0f, 360.0f);
        //    ImGui::SliderFloat("saturation", &colorGradingData.saturation, 0.0f, 2.0f);
        //    ImGui::SliderFloat("brigthness", &colorGradingData.brigthness, 0.0f, 2.0f);
        //    ImGui::TreePop();

        //}


//#if	defined( UNIT_RB_01 )
//        if (ImGui::TreeNode("RadialBlur"))
//        {
//            ImGui::SliderFloat("radius", &radialBlurData.radius, 0.0f, 200.0f);
//            ImGui::SliderInt("samplingCount", &radialBlurData.samplingCount, 0, 10);
//            ImGui::SliderFloat2("center", &radialBlurData.center.x, 0.0f, 1.0f);
//
//#if defined(UNIT_RB_02)
//            ImGui::SliderFloat("mask radius", &radialBlurData.mask_radius, 0.0f, 600.0f);
//#endif	//	defined(UNIT_RB_02)
//            ImGui::TreePop();
//        }
//#endif	//	defined( UNIT_RB_01 )

        //if (ImGui::TreeNode("vignette"))
        //{
        //    ImGui::ColorEdit3("color", &vignette_data.vignette_color.x);
        //    ImGui::SliderFloat2("center", &vignette_data.vignette_center.x, 0, 1);
        //    ImGui::SliderFloat("intensity", &vignette_data.vignette_intensity, 0.0f, +1.0f);
        //    ImGui::SliderFloat("smoothness", &vignette_data.vignette_smoothness, -1.0f, +1.0f);
        //    ImGui::Checkbox("rounded", &vignette_data.vignette_rounded);
        //    ImGui::SliderFloat("roundness", &vignette_data.vignette_roundness, 0.0f, +1.0f);
        //    ImGui::TreePop();
        //}
        //ImGui::TreePop();
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

        radialBlurData.samplingCount = radialBlurDataMax.samplingCount + FLT_EPSILON <= radialBlurData.samplingCount - FLT_EPSILON ?
            radialBlurDataMax.samplingCount : radialBlurData.samplingCount + stepValueRadial;


        radialBlurData.mask_radius = radialBlurDataMax.mask_radius + FLT_EPSILON <= radialBlurData.mask_radius - FLT_EPSILON ?
            radialBlurDataMax.mask_radius : radialBlurData.mask_radius + stepValueRadial;
    }
    // �l����
    else
    {
        // ���W�A���u���[
        radialBlurData.radius = radialBlurDataMin.radius + FLT_EPSILON >= radialBlurData.radius - FLT_EPSILON ?
            radialBlurDataMin.radius : radialBlurData.radius - stepValueRadial;

        radialBlurData.samplingCount = radialBlurDataMin.samplingCount + FLT_EPSILON >= radialBlurData.samplingCount - FLT_EPSILON ?
            radialBlurDataMin.samplingCount : radialBlurData.samplingCount - stepValueRadial;


    }




    // �㏸�I��
    isIncreasingColorGrading = false;
    isIncreasingRadialBlur = false;

    //--timeState;


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

        //vignetteData.samplingCount = vignetteDataMax.samplingCount + FLT_EPSILON >= vignetteData.samplingCount - FLT_EPSILON ?
        //    vignetteDataMax.samplingCount : vignetteData.samplingCount - stepDownValue;
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

        //vignetteData.samplingCount = vignetteDataMin.samplingCount + FLT_EPSILON >= vignetteData.samplingCount - FLT_EPSILON ?
        //    vignetteDataMin.samplingCount : vignetteData.samplingCount - stepDownValue;


    }

  /*  if (timeState - FLT_EPSILON <= timeStateMin + FLT_EPSILON)*/
    isIncreasingModeVignetteData = false;
}
