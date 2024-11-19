#include "Finalpass.hlsli"

// �V�[���e�N�X�`��
Texture2D sceneTexture : register(t0);
SamplerState sceneSampler : register(s0);

// �u���[���e�N�X�`��
Texture2D bloomTexture : register(t1);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = sceneTexture.Sample(sceneSampler, pin.texcoord) * pin.color;

    // �u���[���e�N�X�`�������Z����
    color.rgb += bloomTexture.Sample(sceneSampler, pin.texcoord).rgb;

    // ���C���e�N�X�`���̃T�C�Y���Ȃǂ��擾
    float2 sceneTextureDimensions;
    sceneTexture.GetDimensions(sceneTextureDimensions.x, sceneTextureDimensions.y);



    // �F���␳����
    {
        // RGB > HSV�ɕϊ�
        color.rgb = RGB2HSV(color.rgb);

        // �F������
        color.r += hueShift;

        // �ʓx����
        color.g *= saturation;

        // ���x����
        color.b *= brightness;

        // HSV > RBG�ɕϊ�
        color.rgb = HSV2RGB(color.rgb);
    }


#if	defined( UNIT_VN_01 )
    //  ���B�l�b�g����
    if (vn_intensity > 0)
    {
#if	defined( UNIT_VN_02 )
        if (vn_iris > 0.5f) //  �A�C���X�C��/�A�E�g
        {
            float2 vn_vector = pin.texcoord - vn_center;
            float aspect = sceneTextureDimensions.x / sceneTextureDimensions.y;
            vn_vector.x *= lerp(1.0f, aspect, vn_rounded);
            float intensity = lerp(lerp(0.3f, 1.0f, vn_intensity), vn_intensity, vn_rounded);
            float factor = step(1 - intensity, length(vn_vector));
            color.rgb *= lerp((float3) 1.0f, vn_color.rgb, saturate(factor));
        }
        else    //  �r�l�b�g
#endif	//  defined( UNIT_VN_02 )
        {
            float2 d = abs(pin.texcoord - vn_center) * vn_intensity;
            //  �������X�N���[���ɍ��킷���ǂ���
            d.x *= lerp(1.0f, sceneTextureDimensions.x / sceneTextureDimensions.y, vn_rounded);
            d = pow(saturate(d), vn_roundness);
            //  ���x���Z�o
            float factor = pow(saturate(1.0f - dot(d, d)), vn_smoothness);
            color.rgb *= lerp(vn_color.rgb, (float3) 1.0f, factor);
        }
    }
#endif	//  defined( UNIT_VN_01 )
    return color;
}