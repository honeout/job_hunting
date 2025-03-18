#pragma once
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
public:
    PostprocessingRenderer();
    ~PostprocessingRenderer();

    static PostprocessingRenderer& Instance()
    {
        // 様々な所で取り出したいだからシングルトンにする
        static PostprocessingRenderer postEffect;
        return postEffect;
    }

    // 描画
    void Render(RenderContext rc);

    // シーン情報の設定
    void SetSceneData(ShaderResourceViewData srvData);

    // デバッグ情報の表示
    void DrawDebugGUI();

    // 値を変化
    void MoveTowards();

    // 一気にいくつかを変える。
    // ブルームデータ
    void SetBloomData(BloomData bloomDataMax) {
        this->bloomDataMax = bloomDataMax;
      
    }
    // 色調補正データ
    void SetColorGradingData(ColorGradingData    colorGradingDataMax) {
   
        this->colorGradingData = colorGradingDataMax;
    }
    // ラジアルブラー情報
    void SetRadialBlurData(RadialBlurData radialBlurDataMax) {

        this->radialBlurData = radialBlurDataMax;
    }
    // ダメージ表現
    void SetVignetteData(VignetteData	vignetteDataMax) {

        this->vignetteData = vignetteDataMax;
    }

    // ブルームデータ
    void SetBloomMaxData(BloomData bloomDataMax) {
        this->bloomDataMax = bloomDataMax;
        isIncreasingBloom = true;
    }
    // 色調補正データ
    void SetColorGradingMaxData(ColorGradingData    colorGradingDataMax) {
        isIncreasingColorGrading = true;
        this->colorGradingDataMax = colorGradingDataMax;
    }
    // ラジアルブラー情報
    void SetRadialBlurMaxData(RadialBlurData radialBlurDataMax) {
        isIncreasingRadialBlur = true;
        this->radialBlurDataMax = radialBlurDataMax;
    }
    // ダメージ表現
    void SetVignetteMaxData(VignetteData	vignetteDataMax) {
        isIncreasingModeVignetteData = true;

        this->vignetteDataMax = vignetteDataMax;
    }
    // 色を変える
    void SetVignetteDataColor(DirectX::XMFLOAT4	vignetteDataColor) {

        this->vignetteData.color = vignetteDataColor;
    }

    // ブルーム最低値データ
    void SetBloomMinData(BloomData bloomMinData) { this->bloomDataMin = bloomMinData; }
    // 色調補正最低値データ
    void SetColorGradingMinData(ColorGradingData    colorGradingMinData) { this->colorGradingDataMin = colorGradingMinData; }
    // ラジアルブラー最低値情報
    void SetRadialBlurMinData(RadialBlurData radialBlurMinData) { this->radialBlurDataMin = radialBlurMinData; }
    // ダメージ最低値表現
    void SetVignetteMinData(VignetteData	vignetteMinData) { 
        isIncreasingModeVignetteData = true;

        this->vignetteDataMin = vignetteMinData; 
    }

    // 加算
    void StepValueColor(float add) {
        stepValueColor = add;
    }
    // 加算
    void StepValueRadial(float add) {
        stepValueRadial = add;
    }
    // 加算
    void StepValuevignette(float add) {
        stepValuevignette = add;
    }

    // 最低値
    void StepValueMin() {
        stepValueColor = stepValueColorMin;
        stepValueRadial = stepValueRadialMin;
        stepValuevignette = stepValuevignetteMin;
    }

private:
    // シーン描画データ
    ShaderResourceViewData sceneData;

    // 描画用スプライト
    std::unique_ptr<Sprite> renderSprite;

    // 高輝度抽出テクスチャ
    std::unique_ptr<RenderTarget> luminanceExtractRenderTarget;

    // 高輝度抽出ぼかしテクスチャ
    std::unique_ptr<RenderTarget> luminanceExtractBokehRenderTarget;

    // ブルームデータ
    BloomData bloomData;

    // 色調補正データ
    ColorGradingData    colorGradingData;

// ラジアルブラー情報
    RadialBlurData  radialBlurData;

#if	defined( UNIT_VN_01 )
    //	ビネット情報
    VignetteData					vignetteData;
    
#endif	//	defined( UNIT_VN_01 )

    // ブルームデータ
    BloomData bloomDataMax;

    // 色調補正データ
    ColorGradingData    colorGradingDataMax;


    // ラジアルブラー情報
    RadialBlurData  radialBlurDataMax;

    //	ビネット情報
    VignetteData					vignetteDataMax;

    // ブルームデータ
    BloomData bloomDataMin;

    // 色調補正データ
    ColorGradingData    colorGradingDataMin;

    // ラジアルブラー情報
    RadialBlurData  radialBlurDataMin;

    //	ビネット情報
    VignetteData					vignetteDataMin;

    // 値を少しずつ減少
    float stepValueColor = 0.03f;
    float stepValueColorMin = 0.03f;
    float stepValueRadial = 0.5f;
    float stepValueRadialMin = 5.5f;
    float stepValuevignette = 0.03f;
    float stepValuevignetteMin = 0.03f;

    // 値の変化チェック
    bool isIncreasingBloom = false;
    bool isIncreasingColorGrading = false;
    bool isIncreasingRadialBlur = false;
    bool isIncreasingModeVignetteData = false;
    bool isIncreasingVignetteData = false;
};
