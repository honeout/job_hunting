#include "Phong.hlsli"

VS_OUT main(
    float4 position : POSITION,
    float3 normal : NORMAL,
    float3 tangent : TANGENT,// 接線
    float2 texcoord : TEXCOORD,
    float4 color : COLOR,
    float4 boneWeights : WEIGHTS,
    uint4 boneIndices : BONES
)
{


    float3 p = { 0,0,0 };
    float3 n = { 0,0,0 };
    float3 t = { 0,0,0 };
    for (int i = 0; i < 4; i++)
    {
        p += (boneWeights[i] * mul(position, boneTransforms[boneIndices[i]])).xyz;
        n += (boneWeights[i] * mul(float4(normal.xyz, 0), boneTransforms[boneIndices[i]])).xyz;
        t += (boneWeights[i] * mul(float4(tangent.xyz, 0), boneTransforms[boneIndices[i]])).xyz;
    }

    VS_OUT vout;
    vout.position = mul(float4(p, 1.0f), viewProjection);
    vout.world_position = p;
    vout.normal = normalize(n);
    vout.tangent = normalize(t);
    vout.color = color * materialColor;

    // 法線マップの計算
    vout.binormal = normalize(cross(vout.normal,vout.tangent));

    // テクスチャの解像度を上げる。
    vout.texcoord = texcoord * texcoordMult;

    // UNIT11
    // シャドウマップで使用する情報を算出
    vout.shadowTexcoord = CalcShadowTexcoord(p, lightViewProjection);
    return vout;
}