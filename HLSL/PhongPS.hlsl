#include "Phong.hlsli"
//
//Texture2D diffuseMap : register(t0);
//SamplerState diffuseMapSamplerState : register(s0);
//
//Texture2D normalMap : register(t1); // 法線マップ

// UNIT11
Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1); // 法線マップ
Texture2D shadowMap : register(t2);// シャドウマップ

SamplerState diffuseMapSamplerState : register(s0);

SamplerState shadowMapSamplerState : register(s1); // 法線マップ

float4 main(VS_OUT pin) : SV_TARGET
{
   pin.color.gb = colorGB;

    float4 diffuseColor = diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;

    // UNIT06
    // サンプルしている。
    // ノーマルマップ色
    float3 normal = normalMap.Sample(diffuseMapSamplerState, pin.texcoord).rgb;

    float3x3 CM = { normalize(pin.tangent) ,normalize(pin.binormal),normalize(pin.normal) };

    // ノーマルを１から０にする。接空間をワールド空間にする。

    float3 N = normalize(mul(normal * 2.0f - 1.0f, CM));
    //float3 N = normalize(mul(normal , CM));
    
    //float3 N = normalize(pin.normal);
    float3 L = normalize(directionalLightData.direction.xyz);
    float3 E = normalize(viewPosition.xyz - pin.world_position.xyz);



    // マテリアル定数
    float3 ka = float3(1, 1, 1);
    float3 kd = float3(1, 1, 1);
    // ここ変えるとスペキュラの反射がなくなる
    float3 ks = float3(1, 1, 1);
    float shiness = 128;

    // 環境光の計算
    float3 ambient = ka * ambientLightColor;

    // 平行光源のライティング計算
    //float3 directionalDiffuse = CalcLambertDiffuse(N, L, directionalLightData.color.rgb, kd);
    //float3 directionalDiffuse = CalcHalfLambert(N, L, directionalLightData.color.rgb, kd);
    //float3 directionalSpecular = CalcPhongSpecular(N, L, directionalLightData.color.rgb, E, shiness, ks);

    //UNIT11
    // 平行光源のライティング計算
    float3 directionalDiffuse = CalcLambertDiffuse(N, L, directionalLightData.color.rgb, kd);
    float3 directionalSpecular = CalcPhongSpecular(N, L, directionalLightData.color.rgb, E, shiness, ks);

    // UNIT11
    //// 平行光源の影なので、平行光源に退位して影を適応
    //float3 shadow = CalcShadowColor(shadowMap, shadowMapSamplerState, pin.shadowTexcoord, shadowColor,
    //    shadowBias);

    // UNIT12
        // 平行光源の影なので、平行光源に退位して影を適応
    float3 shadow = CalcShadowColorPCFFilter(shadowMap, shadowMapSamplerState, pin.shadowTexcoord, shadowColor,
        shadowBias);
    directionalDiffuse *= shadow;
    directionalSpecular *= shadow;
    //float4 color = float4(ambient,diffuseColor.a);
    //color.rgb += diffuseColor.rgb*directionalDiffuse;
    //color.rgb += directionalSpecular;

    // UNIT04
    // 点光源の処理
    // 拡散光
    // 結果
    float3 pointDiffuse = (float3)0;
    // 円光
    // 結果
    float3 pointSpecular = (float3)0;
        int i;
    for (i = 0; i < pointLightCount; ++i)
    {
        // ライトベクトルを算出
        float3 lightVector =    pin.world_position - pointLightData[i].position;

        // ライトベクトルの長さを算出 サイズ型
        float lightLenght = length(lightVector);

        // ライトの影響範囲外なら後の計算をしない。
        if (lightLenght > pointLightData[i].range)
            continue;

        // 中でマイナスをする理由
        // 距離減衰を算出 逆数して光の範囲を出す クランプないで１１が出た場合それを引くとマイナスになるだからそれ以上は消す
        //float attenuate = clamp(1.0f - lightLenght / pointLightData[i].range,0.0f,1.0f);
        float attenuate = clamp(1.0f - lightLenght / pointLightData[i].range,0.0f,1.0f);
        // 単位化ベクトル
        lightVector = lightVector / lightLenght;
        pointDiffuse += CalcLambertDiffuse(N, lightVector,
            pointLightData[i].color.rgb, kd.rgb) * attenuate;

        pointSpecular += CalcPhongSpecular(N, lightVector,
            pointLightData[i].color.rgb, E, shiness, ks.rgb) * attenuate;
    }




    // UNIT04
// スポットライトの処理
// 拡散光
// 結果
    float3 spotDiffuse = (float3)0;
    // 円光
    // 結果
    float3 spotSpecular = (float3)0;
 
    for (i = 0; i < spotLightCount; ++i)
    {
        // ライトベクトルを算出
        float3 lightVector =  pin.world_position - spotLightData[i].position;

        // ライトベクトルの長さを算出 サイズ型
        float lightLenght = length(lightVector);

        // ライトの影響範囲外なら後の計算をしない。
        if (lightLenght > spotLightData[i].range)
            continue;

        // 中でマイナスをする理由
        // 距離減衰を算出 逆数して光の範囲を出す クランプないで１１が出た場合それを引くとマイナスになるだからそれ以上は消す
        //float attenuate = clamp(1.0f - lightLenght / pointLightData[i].range,0.0f,1.0f);
        float attenuate = clamp(1.0f - lightLenght / spotLightData[i].range, 0.0f, 1.0f);

        //lightVector = normalize(lightVector);
        lightVector = normalize(lightVector);

        // 角度減衰を算出してattenuateに乗算する
        float3 spotDirection = normalize(spotLightData[i].direction.xyz);
        float  angle = dot(spotDirection,lightVector );
        float area = spotLightData[i].innerCorn - spotLightData[i].outerCorn;
        attenuate *= clamp(1.0f - (spotLightData[i].innerCorn - angle) / area, 0.0f, 1.0f);

        // 単位化ベクトル
        //lightVector = lightVector / lightLenght;
      spotDiffuse += CalcLambertDiffuse(N, lightVector,
            spotLightData[i].color.rgb, kd.rgb) * attenuate;

        spotSpecular += CalcPhongSpecular(N, lightVector,
            spotLightData[i].color.rgb, E, shiness, ks.rgb) * attenuate;

    }


    // 改造発色の良い処理
    //float4 color = diffuseColor;
    //color.rgb *= ambient + directionalDiffuse;
    //color.rgb += directionalSpecular;



    float4 color = float4(ambient, diffuseColor.a);
    // 影
    color.rgb += diffuseColor.rgb * (directionalDiffuse + pointDiffuse + spotDiffuse);
    color.rgb += directionalSpecular + pointSpecular + spotSpecular;

    // リムライティング
    color.rgb += CalcRimLight(N, E, L, directionalLightData.color.rgb);

    return color;
}