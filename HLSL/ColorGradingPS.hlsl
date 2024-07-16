//float4 main() : SV_TARGET
//{
//	return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}
#include "ColorGrading.hlsli"

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = texture0.Sample(sampler0, pin.texcoord) * pin.color;
    // RGB>HSVВ╔Х╧К╖
    color.rgb = RGB2HSV(color.rgb);
    // РFСКТ▓Ро
    color.r += hueShift;

    // Н╩УxТ▓Ро
    color.g *= saturation;

    color.b *= brightness;

    // Ц╛УxТ▓Ро
    color.rgb = HSV2RGB(color.rgb);

    return color;
}

