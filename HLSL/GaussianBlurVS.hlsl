#include "GaussianBlur.hlsli"

VS_OUT main(float3 position: POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD)
{
    VS_OUT vout;
    vout.position = float4(position,1.0f);
    vout.color = color;
    vout.texcoord = texcoord;

    return vout;
}