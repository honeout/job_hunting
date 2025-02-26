#include "Phong.hlsli"

VS_OUT main(
    float4 position : POSITION,
    float3 normal : NORMAL,
    float3 tangent : TANGENT,// �ڐ�
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

    // �@���}�b�v�̌v�Z
    vout.binormal = normalize(cross(vout.normal,vout.tangent));

    // �e�N�X�`���̉𑜓x���グ��B
    vout.texcoord = texcoord * texcoordMult;

    // UNIT11
    // �V���h�E�}�b�v�Ŏg�p��������Z�o
    vout.shadowTexcoord = CalcShadowTexcoord(p, lightViewProjection);
    return vout;
}