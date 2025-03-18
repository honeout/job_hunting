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
    // 値を少しずつ戻す
    MoveTowards();
    // セッター変更値用
    {
        rc.bloomData.gaussianFilterData = bloomData.gaussianFilterData;
        rc.bloomData.luminanceExtractionData = bloomData.luminanceExtractionData;
        rc.gaussianFilterData.textureSize = bloomData.gaussianFilterData.textureSize;
        rc.colorGradingData = colorGradingData;
        rc.radialBlurData = radialBlurData;
        rc.vignetteData = vignetteData;
    }
    // 高輝度抽出用バッファに描画先を変更して高輝度抽出
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
#if	defined( UNIT_VN_01 )
//	ビネット情報設定
        //rc.vignetteData;

#endif	//	defined( UNIT_VN_01 )
        // 描画対象を変更
        renderSprite->SetShaderResourceView(sceneData.srv,sceneData.width,sceneData.height);

        renderSprite->Update(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),
            0, 0, static_cast<float>(sceneData.width),static_cast<float>(sceneData.height),
            0,
            1, 1, 1, 1);
        // シェーダーに渡すテクスチャを設定
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
    // 値上昇
    if (isIncreasingColorGrading)
    {
        // カラーグラディエンス
        colorGradingData.brigthness = colorGradingDataMax.brigthness + FLT_EPSILON <= colorGradingData.brigthness - FLT_EPSILON ?
            colorGradingDataMax.brigthness : colorGradingData.brigthness + stepValueColor;
        colorGradingData.hueShift = colorGradingDataMax.hueShift + FLT_EPSILON <= colorGradingData.hueShift - FLT_EPSILON ?
            colorGradingDataMax.hueShift : colorGradingData.hueShift + stepValueColor;
        colorGradingData.saturation = colorGradingDataMax.saturation + FLT_EPSILON <= colorGradingData.saturation - FLT_EPSILON ?
            colorGradingDataMax.saturation : colorGradingData.saturation + stepValueColor;
    }
    // 値減少
    else
    {
        // カラーグラディエンス
        colorGradingData.brigthness = colorGradingDataMin.brigthness + FLT_EPSILON >= colorGradingData.brigthness - FLT_EPSILON ?
            colorGradingDataMin.brigthness : colorGradingData.brigthness - stepValueColor;
        colorGradingData.hueShift = colorGradingDataMin.hueShift + FLT_EPSILON >= colorGradingData.hueShift - FLT_EPSILON ?
            colorGradingDataMin.hueShift : colorGradingData.hueShift - stepValueColor;
        colorGradingData.saturation = colorGradingDataMin.saturation + FLT_EPSILON >= colorGradingData.saturation - FLT_EPSILON ?
            colorGradingDataMin.saturation : colorGradingData.saturation - stepValueColor;
    }
    // 値上昇
    if (isIncreasingRadialBlur)
    {
        // ラジアルブラー
        radialBlurData.radius = radialBlurDataMax.radius + FLT_EPSILON <= radialBlurData.radius - FLT_EPSILON ?
            radialBlurDataMax.radius : radialBlurData.radius + stepValueRadial;
        radialBlurData.samplingCount = (int)(radialBlurDataMax.samplingCount + FLT_EPSILON <= radialBlurData.samplingCount - FLT_EPSILON ?
            radialBlurDataMax.samplingCount : radialBlurData.samplingCount + stepValueRadial);
        radialBlurData.mask_radius = radialBlurDataMax.mask_radius + FLT_EPSILON <= radialBlurData.mask_radius - FLT_EPSILON ?
            radialBlurDataMax.mask_radius : radialBlurData.mask_radius + stepValueRadial;
    }
    // 値減少
    else
    {
        // ラジアルブラー
        radialBlurData.radius = radialBlurDataMin.radius + FLT_EPSILON >= radialBlurData.radius - FLT_EPSILON ?
            radialBlurDataMin.radius : radialBlurData.radius - stepValueRadial;
        radialBlurData.samplingCount = (int)(radialBlurDataMin.samplingCount + FLT_EPSILON >= radialBlurData.samplingCount - FLT_EPSILON ?
            radialBlurDataMin.samplingCount : radialBlurData.samplingCount - stepValueRadial);
        radialBlurData.mask_radius = radialBlurDataMin.mask_radius + FLT_EPSILON >= radialBlurData.mask_radius - FLT_EPSILON ?
            radialBlurDataMin.mask_radius : radialBlurData.mask_radius + stepValueRadial;
    }
    // 上昇終了
    isIncreasingColorGrading = false;
    isIncreasingRadialBlur = false;

    if (!isIncreasingModeVignetteData)
    {
        // ヴィジェット
        vignetteData.intensity = vignetteDataMax.intensity + FLT_EPSILON <= vignetteData.intensity - FLT_EPSILON ?
            vignetteDataMax.intensity : vignetteData.intensity + stepValuevignette;
        vignetteData.intensity = vignetteDataMin.intensity + FLT_EPSILON >= vignetteData.intensity - FLT_EPSILON ?
            vignetteDataMin.intensity : vignetteData.intensity - stepValuevignette;
        return;
    }
    
    // 値上昇
    if (isIncreasingVignetteData)
    {
        // ヴィジェット
        vignetteData.intensity = vignetteDataMax.intensity + FLT_EPSILON <= vignetteData.intensity - FLT_EPSILON ?
            vignetteDataMax.intensity : vignetteData.intensity + stepValuevignette;

        vignetteData.smoothness = vignetteDataMax.smoothness + FLT_EPSILON <= vignetteData.smoothness - FLT_EPSILON ?
            vignetteDataMax.smoothness : vignetteData.smoothness + stepValuevignette;

        isIncreasingVignetteData = vignetteDataMax.intensity + FLT_EPSILON <= vignetteData.intensity - FLT_EPSILON ?
            false : true;
    }
    // 値減少
    else
    {
        // ヴィジェット
        vignetteData.intensity = vignetteDataMin.intensity + FLT_EPSILON >= vignetteData.intensity - FLT_EPSILON ?
            vignetteDataMin.intensity : vignetteData.intensity - stepValuevignette;

        vignetteData.smoothness = vignetteDataMin.smoothness + FLT_EPSILON >= vignetteData.smoothness - FLT_EPSILON ?
            vignetteDataMin.smoothness : vignetteData.smoothness - stepValuevignette;


        isIncreasingVignetteData = vignetteDataMin.intensity + FLT_EPSILON >= vignetteData.intensity - FLT_EPSILON ?
            true : false;
    }
    isIncreasingModeVignetteData = false;
}
