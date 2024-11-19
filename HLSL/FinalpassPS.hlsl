#include "Finalpass.hlsli"

// シーンテクスチャ
Texture2D sceneTexture : register(t0);
SamplerState sceneSampler : register(s0);

// ブルームテクスチャ
Texture2D bloomTexture : register(t1);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = sceneTexture.Sample(sceneSampler, pin.texcoord) * pin.color;

    // ブルームテクスチャを加算する
    color.rgb += bloomTexture.Sample(sceneSampler, pin.texcoord).rgb;

    // メインテクスチャのサイズ情報などを取得
    float2 sceneTextureDimensions;
    sceneTexture.GetDimensions(sceneTextureDimensions.x, sceneTextureDimensions.y);



    // 色調補正処理
    {
        // RGB > HSVに変換
        color.rgb = RGB2HSV(color.rgb);

        // 色相調整
        color.r += hueShift;

        // 彩度調整
        color.g *= saturation;

        // 明度調整
        color.b *= brightness;

        // HSV > RBGに変換
        color.rgb = HSV2RGB(color.rgb);
    }


#if	defined( UNIT_VN_01 )
    //  ヴィネット処理
    if (vn_intensity > 0)
    {
#if	defined( UNIT_VN_02 )
        if (vn_iris > 0.5f) //  アイリスイン/アウト
        {
            float2 vn_vector = pin.texcoord - vn_center;
            float aspect = sceneTextureDimensions.x / sceneTextureDimensions.y;
            vn_vector.x *= lerp(1.0f, aspect, vn_rounded);
            float intensity = lerp(lerp(0.3f, 1.0f, vn_intensity), vn_intensity, vn_rounded);
            float factor = step(1 - intensity, length(vn_vector));
            color.rgb *= lerp((float3) 1.0f, vn_color.rgb, saturate(factor));
        }
        else    //  ビネット
#endif	//  defined( UNIT_VN_02 )
        {
            float2 d = abs(pin.texcoord - vn_center) * vn_intensity;
            //  減光をスクリーンに合わすかどうか
            d.x *= lerp(1.0f, sceneTextureDimensions.x / sceneTextureDimensions.y, vn_rounded);
            d = pow(saturate(d), vn_roundness);
            //  強度を算出
            float factor = pow(saturate(1.0f - dot(d, d)), vn_smoothness);
            color.rgb *= lerp(vn_color.rgb, (float3) 1.0f, factor);
        }
    }
#endif	//  defined( UNIT_VN_01 )
    return color;
}