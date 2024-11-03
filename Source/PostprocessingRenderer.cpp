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

        // 高輝度抽出用動画ターゲットを生成
        luminanceExtractRenderTarget = std::make_unique<RenderTarget>(width, height,
            DXGI_FORMAT_R8G8B8A8_UNORM);

        // 高輝度抽出ぼかし用描画ターゲットを生成
        luminanceExtractBokehRenderTarget = std::make_unique<RenderTarget>(width, height,
            DXGI_FORMAT_R8G8B8A8_UNORM);
        bloomData.gaussianFilterData.textureSize.x = static_cast<float>(luminanceExtractBokehRenderTarget->
            GetWidth());
        bloomData.gaussianFilterData.textureSize.y = static_cast<float>(luminanceExtractBokehRenderTarget->
            GetHeight());
    }
    //	定数バッファの生成
    {
        D3D11_BUFFER_DESC buffer_desc{};
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        buffer_desc.CPUAccessFlags = 0;
        buffer_desc.MiscFlags = 0;
        buffer_desc.StructureByteStride = 0;
        ////	ヴィネット用定数バッファ
        //{
        //    
        //    buffer_desc.ByteWidth = sizeof(vignette_constants);
        //    HRESULT hr = Graphics::Instance().GetDevice()->CreateBuffer(&buffer_desc, nullptr, vignette_constant_buffer.GetAddressOf());
        //    _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
        //}


        //	スプライトシェーダー準備
        {
            ////	ヴィネットシェーダー
            ////create_ps_from_cso(Graphics::Instance().GetDevice(), "vignette_ps.cso", vignette_pixel_shader.GetAddressOf());
            //// ファイルを開く
            //FILE* fp{ nullptr };
            //fopen_s(&fp, "Shader\\vignette_ps.cso", "rb");
            //_ASSERT_EXPR_A(fp, "CSO File not found");

            //fseek(fp, 0, SEEK_END);
            //long cso_sz{ ftell(fp) };
            //fseek(fp, 0, SEEK_SET);

            //std::unique_ptr<unsigned char[]> cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
            //fread(cso_data.get(), cso_sz, 1, fp);
            //fclose(fp);

            //HRESULT hr{ S_OK };
            //hr = Graphics::Instance().GetDevice()->CreatePixelShader(cso_data.get(), cso_sz, nullptr, vignette_pixel_shader.GetAddressOf());
            //_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
        }
    }

    //// 高輝度抽出用描画ターゲットを生成
    //luminanceExtractRenderTarget =
    //    std::make_unique<RenderTarget>(static_cast<UINT>(graphics.GetScreenWidth()),
    //        static_cast<UINT>(graphics.GetScreenHeight()), DXGI_FORMAT_R8G8B8A8_UNORM);

    //// 高輝度抽出ぼかし用描画ターゲットを生成
    //luminanceExtractBokehRenderTarget =
    //    std::make_unique<RenderTarget>(static_cast<UINT>(graphics.GetScreenWidth()),
    //        static_cast<UINT>(graphics.GetScreenHeight()), DXGI_FORMAT_R8G8B8A8_UNORM);
    //bloomData.gaussianFilterData.textureSize.x = static_cast<float>(luminanceExtractBokehRenderTarget->GetWidth());
    //bloomData.gaussianFilterData.textureSize.y = static_cast<float>(luminanceExtractBokehRenderTarget->GetHeight());

}

PostprocessingRenderer::~PostprocessingRenderer()
{
}

void PostprocessingRenderer::Render(RenderContext rc)
{
 
    Graphics& graphics = Graphics::Instance();

    // 現在設定されているバッファを避難して初期化しておく
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

    //RenderContext rc;
    
    rc.luminanceExtractionData = bloomData.luminanceExtractionData;
    rc.gaussianFilterData = bloomData.gaussianFilterData;

    // 高輝度抽出用バッファに描画先を変更して高輝度抽出
    {
        //      描画先を変更
        //Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = luminanceExtractRenderTarget->GetRenderTargetView().Get();
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

        // 高輝度抽出処理
        SpriteShader* shader = graphics.GetShader(SpriteShaderId::LuminanceExtraction);
 
        shader->Begin(rc);
        
        // 描画対象を変更
        renderSprite->SetShaderResourceView(sceneData.srv, sceneData.width, sceneData.height);
        renderSprite->Update(0, 0, viewport.Width, viewport.Height,
            0, 0, static_cast<float>(sceneData.width), static_cast<float>(sceneData.height),
            0,
            1, 1, 1, 1);
        shader->Draw(rc, renderSprite.get());
        shader->End(rc);
       
    }

    // 抽出した高輝描画対象を暈して書き込む
    {
        // 描画先を変更
        ID3D11RenderTargetView* rtv = luminanceExtractBokehRenderTarget->GetRenderTargetView().Get();
        //Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = luminanceExtractBokehRenderTarget->GetRenderTargetView().Get();
        FLOAT color[] = { 0.0f,0.0f,0.0f,0.0f };
        //      描画先を変更
        rc.deviceContext->ClearRenderTargetView(rtv, color);
        rc.deviceContext->OMSetRenderTargets(1, &rtv, nullptr);
        D3D11_VIEWPORT viewport{};
        viewport.Width = static_cast<float>(luminanceExtractBokehRenderTarget->GetWidth());
        viewport.Height = static_cast<float>(luminanceExtractBokehRenderTarget->GetHeight());
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        rc.deviceContext->RSSetViewports(cachedViewportCount, cachedViewports);

        // 高輝度抽出処理
        SpriteShader* shader = graphics.GetShader(SpriteShaderId::GaussianBlur);
        shader->Begin(rc);

        // 描画対象を変更
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

    // 元のバッファに戻す
    {
        rc.deviceContext->RSSetViewports(cachedViewportCount, cachedViewports);
        rc.deviceContext->OMSetRenderTargets(1, cachedRenderTargetView.GetAddressOf(),
            cachedDepthStencilView.Get());
    }

    // 最終パス
    {
    


        // 高輝度抽出処理
        SpriteShader* shader = graphics.GetShader(SpriteShaderId::Finalpass);
        shader->Begin(rc);

        // 描画対象を変更
    
        renderSprite->SetShaderResourceView(sceneData.srv,sceneData.width,sceneData.height);
        //renderSprite->SetShaderResourceView(luminanceExtractBokehRenderTarget->GetShaderResourceView(),
        //    luminanceExtractBokehRenderTarget->GetWidth(),
        //    luminanceExtractBokehRenderTarget->GetHeight());


        renderSprite->Update(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),
            0, 0, static_cast<float>(sceneData.width),static_cast<float>(sceneData.height),
            0,
            1, 1, 1, 1);
        //renderSprite->Update(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),
        //    0, 0, static_cast<float>(luminanceExtractBokehRenderTarget->GetWidth()), static_cast<float>(luminanceExtractBokehRenderTarget->GetHeight()),
        //    0,
        //    1, 1, 1, 1);
        // シェーダーに渡すテクスチャを設定
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
        if (ImGui::TreeNode("Bloom"))
        {
            ImGui::SliderFloat("threshold", &bloomData.luminanceExtractionData.threshold, 0.0f,1.0f);
            ImGui::SliderFloat("intensity", &bloomData.luminanceExtractionData.intensity, 0.0f,10.0f);
            ImGui::SliderInt("kernelSize", &bloomData.gaussianFilterData.kernelSize, 1, MaxkernelSize-1);
            ImGui::SliderFloat("deviation", &bloomData.gaussianFilterData.deviation, 1.0f, 10.0f);
            ImGui::TreePop();

        }

        if (ImGui::TreeNode("ColorGrading"))
        {
            ImGui::SliderFloat("hueShift", &colorGradingData.hueShift, 0.0f, 360.0f);
            ImGui::SliderFloat("saturation", &colorGradingData.saturation, 0.0f, 2.0f);
            ImGui::SliderFloat("brigthness", &colorGradingData.brigthness, 0.0f, 2.0f);
            ImGui::TreePop();

        }
        if (ImGui::TreeNode("vignette"))
        {
            ImGui::ColorEdit3("color", &vignette_data.vignette_color.x);
            ImGui::SliderFloat2("center", &vignette_data.vignette_center.x, 0, 1);
            ImGui::SliderFloat("intensity", &vignette_data.vignette_intensity, 0.0f, +1.0f);
            ImGui::SliderFloat("smoothness", &vignette_data.vignette_smoothness, 0.0f, +1.0f);
            ImGui::Checkbox("rounded", &vignette_data.vignette_rounded);
            ImGui::SliderFloat("roundness", &vignette_data.vignette_roundness, 0.0f, +1.0f);
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
}
