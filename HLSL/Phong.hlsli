#include "Light.hlsli"
// UNIT11
#include "ShadowmapFunctions.hlsli"

struct VS_OUT
{
    // レンダリングパイプラインを流れるデータ
    // GPU側同士でつながっている
    // ps１ピクセルを決める
    float4 position : SV_POSITION;
    // vs１頂点を設定する
    float3 world_position : POSITION;
    float3 normal : NORMAL;
    float3 tangent:TANGENT;
    float3 binormal:BINORMAL;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
    // UNIT11
    float3 shadowTexcoord : TEXCOORD1; // シャドウマップから情報を取得するためのUV座標＋深度値
};

cbuffer CbScene : register(b0)
{
    // CPUから渡されるデータ
    float4 viewPosition;
    row_major float4x4      viewProjection;
    float4                  ambientLightColor;
    DirectionalLightData    directionalLightData;
    PointLightData          pointLightData[PointLightMax];// 点光源情報
    SpotLightData           spotLightData[SpotLightMax];// スポットライト情報
    int                     pointLightCount;  // 点光源数
    //float3                  dummy2;
    int                     spotLightCount; // スポットライト数

    float2                  colorGB;
    //float2                  dummy2;
    //float2                  dummy2;



};

#define MAX_BONES 128
// 送る奴vsに
cbuffer CbMesh : register(b1)
{
    row_major float4x4 boneTransforms[MAX_BONES];
};

cbuffer CbSubset : register(b2)
{
    float4 materialColor;
};
// UNIT11
cbuffer CbShadowmap : register(b3)
{
    // UNIT11
    row_major float4x4 lightViewProjection; // ライトビュープロジェクション行列
    float3             shadowColor;         // 影の色
    float              shadowBias;          // 深度値比較時のオフセット
};