#include "Phong.hlsli"
//
//Texture2D diffuseMap : register(t0);
//SamplerState diffuseMapSamplerState : register(s0);
//
//Texture2D normalMap : register(t1); // �@���}�b�v

// UNIT11
Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1); // �@���}�b�v
Texture2D shadowMap : register(t2);// �V���h�E�}�b�v

SamplerState diffuseMapSamplerState : register(s0);

SamplerState shadowMapSamplerState : register(s1); // �@���}�b�v

float4 main(VS_OUT pin) : SV_TARGET
{
   pin.color.gb = colorGB;

    float4 diffuseColor = diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;

    // UNIT06
    // �T���v�����Ă���B
    // �m�[�}���}�b�v�F
    float3 normal = normalMap.Sample(diffuseMapSamplerState, pin.texcoord).rgb;

    float3x3 CM = { normalize(pin.tangent) ,normalize(pin.binormal),normalize(pin.normal) };

    // �m�[�}�����P����O�ɂ���B�ڋ�Ԃ����[���h��Ԃɂ���B

    float3 N = normalize(mul(normal * 2.0f - 1.0f, CM));
    //float3 N = normalize(mul(normal , CM));
    
    //float3 N = normalize(pin.normal);
    float3 L = normalize(directionalLightData.direction.xyz);
    float3 E = normalize(viewPosition.xyz - pin.world_position.xyz);



    // �}�e���A���萔
    float3 ka = float3(1, 1, 1);
    float3 kd = float3(1, 1, 1);
    // �����ς���ƃX�y�L�����̔��˂��Ȃ��Ȃ�
    float3 ks = float3(1, 1, 1);
    float shiness = 128;

    // �����̌v�Z
    float3 ambient = ka * ambientLightColor;

    // ���s�����̃��C�e�B���O�v�Z
    //float3 directionalDiffuse = CalcLambertDiffuse(N, L, directionalLightData.color.rgb, kd);
    //float3 directionalDiffuse = CalcHalfLambert(N, L, directionalLightData.color.rgb, kd);
    //float3 directionalSpecular = CalcPhongSpecular(N, L, directionalLightData.color.rgb, E, shiness, ks);

    //UNIT11
    // ���s�����̃��C�e�B���O�v�Z
    float3 directionalDiffuse = CalcLambertDiffuse(N, L, directionalLightData.color.rgb, kd);
    float3 directionalSpecular = CalcPhongSpecular(N, L, directionalLightData.color.rgb, E, shiness, ks);

    // UNIT11
    //// ���s�����̉e�Ȃ̂ŁA���s�����ɑވʂ��ĉe��K��
    //float3 shadow = CalcShadowColor(shadowMap, shadowMapSamplerState, pin.shadowTexcoord, shadowColor,
    //    shadowBias);

    // UNIT12
        // ���s�����̉e�Ȃ̂ŁA���s�����ɑވʂ��ĉe��K��
    float3 shadow = CalcShadowColorPCFFilter(shadowMap, shadowMapSamplerState, pin.shadowTexcoord, shadowColor,
        shadowBias);
    directionalDiffuse *= shadow;
    directionalSpecular *= shadow;
    //float4 color = float4(ambient,diffuseColor.a);
    //color.rgb += diffuseColor.rgb*directionalDiffuse;
    //color.rgb += directionalSpecular;

    // UNIT04
    // �_�����̏���
    // �g�U��
    // ����
    float3 pointDiffuse = (float3)0;
    // �~��
    // ����
    float3 pointSpecular = (float3)0;
        int i;
    for (i = 0; i < pointLightCount; ++i)
    {
        // ���C�g�x�N�g�����Z�o
        float3 lightVector =    pin.world_position - pointLightData[i].position;

        // ���C�g�x�N�g���̒������Z�o �T�C�Y�^
        float lightLenght = length(lightVector);

        // ���C�g�̉e���͈͊O�Ȃ��̌v�Z�����Ȃ��B
        if (lightLenght > pointLightData[i].range)
            continue;

        // ���Ń}�C�i�X�����闝�R
        // �����������Z�o �t�����Č��͈̔͂��o�� �N�����v�Ȃ��łP�P���o���ꍇ����������ƃ}�C�i�X�ɂȂ邾���炻��ȏ�͏���
        //float attenuate = clamp(1.0f - lightLenght / pointLightData[i].range,0.0f,1.0f);
        float attenuate = clamp(1.0f - lightLenght / pointLightData[i].range,0.0f,1.0f);
        // �P�ʉ��x�N�g��
        lightVector = lightVector / lightLenght;
        pointDiffuse += CalcLambertDiffuse(N, lightVector,
            pointLightData[i].color.rgb, kd.rgb) * attenuate;

        pointSpecular += CalcPhongSpecular(N, lightVector,
            pointLightData[i].color.rgb, E, shiness, ks.rgb) * attenuate;
    }




    // UNIT04
// �X�|�b�g���C�g�̏���
// �g�U��
// ����
    float3 spotDiffuse = (float3)0;
    // �~��
    // ����
    float3 spotSpecular = (float3)0;
 
    for (i = 0; i < spotLightCount; ++i)
    {
        // ���C�g�x�N�g�����Z�o
        float3 lightVector =  pin.world_position - spotLightData[i].position;

        // ���C�g�x�N�g���̒������Z�o �T�C�Y�^
        float lightLenght = length(lightVector);

        // ���C�g�̉e���͈͊O�Ȃ��̌v�Z�����Ȃ��B
        if (lightLenght > spotLightData[i].range)
            continue;

        // ���Ń}�C�i�X�����闝�R
        // �����������Z�o �t�����Č��͈̔͂��o�� �N�����v�Ȃ��łP�P���o���ꍇ����������ƃ}�C�i�X�ɂȂ邾���炻��ȏ�͏���
        //float attenuate = clamp(1.0f - lightLenght / pointLightData[i].range,0.0f,1.0f);
        float attenuate = clamp(1.0f - lightLenght / spotLightData[i].range, 0.0f, 1.0f);

        //lightVector = normalize(lightVector);
        lightVector = normalize(lightVector);

        // �p�x�������Z�o����attenuate�ɏ�Z����
        float3 spotDirection = normalize(spotLightData[i].direction.xyz);
        float  angle = dot(spotDirection,lightVector );
        float area = spotLightData[i].innerCorn - spotLightData[i].outerCorn;
        attenuate *= clamp(1.0f - (spotLightData[i].innerCorn - angle) / area, 0.0f, 1.0f);

        // �P�ʉ��x�N�g��
        //lightVector = lightVector / lightLenght;
      spotDiffuse += CalcLambertDiffuse(N, lightVector,
            spotLightData[i].color.rgb, kd.rgb) * attenuate;

        spotSpecular += CalcPhongSpecular(N, lightVector,
            spotLightData[i].color.rgb, E, shiness, ks.rgb) * attenuate;

    }


    // �������F�̗ǂ�����
    //float4 color = diffuseColor;
    //color.rgb *= ambient + directionalDiffuse;
    //color.rgb += directionalSpecular;



    float4 color = float4(ambient, diffuseColor.a);
    // �e
    color.rgb += diffuseColor.rgb * (directionalDiffuse + pointDiffuse + spotDiffuse);
    color.rgb += directionalSpecular + pointSpecular + spotSpecular;

    // �������C�e�B���O
    color.rgb += CalcRimLight(N, E, L, directionalLightData.color.rgb);

    return color;
}