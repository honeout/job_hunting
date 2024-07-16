// 平行光源情報

struct DirectionalLightData
{
    float4 direction; // 向き
    float4 color; // 色
};

// 点光源情報
struct PointLightData
{
    float4 position; // 座標
    float4 color;// 色
    float range; //範囲
    float3 dummy;
};

// 点光源の最大数
static const int PointLightMax = 8;

// スポットライト情報
struct SpotLightData
{
    float4 position; // 座標
    float4 direction; // 向き
    float4 color;// 色
    float range; //範囲
    float innerCorn;
    float outerCorn;
    float dummy;
};
// スポットライトの最大数
static const int SpotLightMax = 8;

//---------------------------------------------------
//  ランバート拡散反射計算関数
//---------------------------------------------------
// normal　　　: 法線(正規化済み)
// lightVector : 入射ベクトル(正規化済み)
// lightColor  : 入射光色
// kd          : 反射率(班さyの強さ)
// 返す値　　　 : 拡散反射色 
float3 CalcLambertDiffuse(float3 normal, float3 LightVector, float3 LightColor, float3 kd)
{
    // 法線とライト方向へのベクトルでの内積
    // 内積した結果、ライトからみて裏向きの面だとマイナスの結果が出てしまうため
    // それをそのまま乗算すると色がおかしくなります。
    // 0以下で有れば０になるように制限をつけましょう。
    // -LightVector理由内積
    float d = max(dot(-LightVector,normal),0)*LightColor*kd;
    //float d = clamp((dot(-lightVector,normal)*0.5+0.5),0,1)*lightColor*kd;

    // 入射光色と内積の結果、および反射率を全て蒸散して返却しましょう。
    return kd* LightColor*d;
 }

//-----------------------------------------------------
//  フォンの鏡面反射計算関数
//-----------------------------------------------------
// normal        : 法線(正規化済み)
// lightVector   : 入射ベクトル(正規化済み)
// lightColor    : 入射光色
// eyeVector     : 視線ベクトル(正規化済み)
// Shiness       : 光尺度(質感調整値)
// ks            : 反射率(反射の強さ)
// 返す値         : 鏡面反射色
float3 CalcPhongSpecular(float3 normal, float3 lightVector,
    float3 lightColor, float3 eyeVector, float shininess, float3 ks)
{
    // 入射ベクトルを法線方向に対して反射させた反射ベクトルを算出しましょう
    float3 R = reflect(lightVector, normal);

    // 反射ベクトルと視線ベクトルとで内積
    // 内積した結果を０以下で有れば０になるように制限をつけましょう
    float d = max(dot(R,eyeVector),0);

    // 光尺処理
    // 反射は表面がつるつるなほどハイライトは強くなり、範囲が小さくなります。
    // 逆に表面がざらざらな程、ハイライトは弱くなり、範囲は広がります。
    // この変化は指数的に変化するため光尺度を乗数として使用します。
    d = pow(d, shininess);
    // 入射光色と内積の結果、及び反射率をすべtえ蒸散して返却しましょう
    return d * lightColor * ks;
}
 
//---------------------------------------------------
//  ハーフランバート拡散反射計算関数
//---------------------------------------------------
// normal　　　: 法線(正規化済み)
// lightVector : 入射ベクトル(正規化済み)
// lightColor  : 入射光色
// kd          : 反射率(班さyの強さ)
// 返す値　　　 : 拡散反射色 
float3 CalcHalfLambert(float3 normal, float3 LightVector, float3 LightColor, float3 kd)
{
    // 法線とライト方向へのベクトルでの内積
    // 内積した結果、ライトからみて裏向きの面だとマイナスの結果が出てしまうため
    // それをそのまま乗算すると色がおかしくなります。
    // 0以下で有れば０になるように制限をつけましょう。
    // -LightVector理由内積
    //float d = max(dot(-LightVector,normal),0)*LightColor*kd;
    float d = clamp((dot(-LightVector, normal) * 0.5 + 0.5), 0, 1) * LightColor * kd;


    // 入射光色と内積の結果、および反射率を全て蒸散して返却しましょう。
    return  LightColor * d * kd;
}


//---------------------------------------------------
//  リムライト
//---------------------------------------------------
// normal　　　 : 法線(正規化済み)
// eyeVector   : 視線ベクトル(正規化済み)
// lightVector : 入射ベクトル(正規化済み)
// lightColor  : 入射光色
// rimPower    : リムライトの強さ(初期値はテキトーなので自分で死っていするが吉)
//
float3 CalcRimLight(float3 normal,float3 eyeVector, float3 LightVector, float3 LightColor, float rimPower
    = 3.0f)
{

    float rim = 1.0 - saturate(dot(eyeVector, normal));
    rim = pow(rim, rimPower);

    // 入射光色と内積の結果、および反射率を全て蒸散して返却しましょう。
    return  LightColor * rim*saturate(dot(eyeVector, LightVector));
}

//-----------------------------------------------------
//  トゥーン拡散反射計算関数
//-----------------------------------------------------
// toonTexture   : トゥーン用U方向ランプテクスチャ
// toonSamplerState : トゥーン用サンプラスステート
// normal        : 法線(正規化済み)
// lightVector   : 入射ベクトル(正規化済み)
// lightColor    : 入射光色
// kd            : 反射率(反射の強さ)
// 返す値         : 拡散反射色
float3 CalcToonDiffuse(Texture2D toonTexture, SamplerState toonSamplerState,
    float3 normal, float3 LightVector,
    float3 LightColor, float3 kd)
{
    // 入射と入射ベクトルからU座標を求める
    // ０～１までを納める為の計算その間でテクスチャから濃い色薄い色を抜き取る
    float u = clamp(dot(normal, - LightVector) * 0.5f + 0.5f,0.01f,0.99f);

    float3 c = toonTexture.Sample(toonSamplerState, float2(u,0.1f));

    // 反射ベクトルと視線ベクトルとで内積
    // 内積した結果を０以下で有れば０になるように制限をつけましょう
 /*   float lightColor*c*kd;*/

    // 光尺処理
    // 反射は表面がつるつるなほどハイライトは強くなり、範囲が小さくなります。
    // 逆に表面がざらざらな程、ハイライトは弱くなり、範囲は広がります。
    // この変化は指数的に変化するため光尺度を乗数として使用します。

    return LightColor * c * kd;
}