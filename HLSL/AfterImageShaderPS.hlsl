#include "AfterImageShader.hlsli"

Texture2D diffuseMap : register(t0);
SamplerState diffuseMapSamplerState : register(s0);


float4 main(VS_OUT pin) : SV_TARGET
{
    pin.color.a = alpha;
    pin.color.rgb = 1;
    return diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;
}