// UNIT05 03
#include "sprite.hlsli"

//#include "scene_constant_buffer.hlsli"
// UNIT05 04
cbuffer VIGNETTE_CONSTANT_BUFFER : register(b2)
{
    float4 vignette_color;
    float2 vignette_center;
    float vignette_intensity;
    float vignette_smoothness;

    float vignette_rounded;
    float vignette_roundness;
    float vignette_opacity;
    float vignette_dummy;
};

Texture2D scene_map : register(t0);
SamplerState linear_sampler_state : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
    float2 scene_map_size;
    scene_map.GetDimensions(scene_map_size.x, scene_map_size.y);

    float4 color = scene_map.Sample(linear_sampler_state, pin.texcoord);

    //  ü•ÓŒ¸Œõˆ—
    float2 d = abs(pin.texcoord - vignette_center) * (vignette_intensity);
    //  Œ¸Œõ‚ğƒXƒNƒŠ[ƒ“‚É‡‚í‚·‚©‚Ç‚¤‚©
    d.x *= lerp(1.0f, scene_map_size.x / scene_map_size.y, vignette_rounded);
    //  ‹÷‚Ì”Z‚³
    d = pow(saturate(d), vignette_roundness);
    half vignette_factor = pow(saturate(1.0f - dot(d, d)), vignette_smoothness);
    color.rgb *= lerp(vignette_color.rgb, (float3) 1.0f, vignette_factor);
    return color;
}
