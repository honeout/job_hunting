#include "Light.hlsli"
// UNIT11
#include "ShadowmapFunctions.hlsli"

struct VS_OUT
{
    // �����_�����O�p�C�v���C���𗬂��f�[�^
    // GPU�����m�łȂ����Ă���
    // ps�P�s�N�Z�������߂�
    float4 position : SV_POSITION;
    // vs�P���_��ݒ肷��
    float3 world_position : POSITION;
    float3 normal : NORMAL;
    float3 tangent:TANGENT;
    float3 binormal:BINORMAL;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
    // UNIT11
    float3 shadowTexcoord : TEXCOORD1; // �V���h�E�}�b�v��������擾���邽�߂�UV���W�{�[�x�l
};

cbuffer CbScene : register(b0)
{
    // CPU����n�����f�[�^
    float4 viewPosition;
    row_major float4x4      viewProjection;
    float4                  ambientLightColor;
    DirectionalLightData    directionalLightData;
    PointLightData          pointLightData[PointLightMax];// �_�������
    SpotLightData           spotLightData[SpotLightMax];// �X�|�b�g���C�g���
    int                     pointLightCount;  // �_������
    //float3                  dummy2;
    int                     spotLightCount; // �X�|�b�g���C�g��

    float2                  colorGB;
    //float2                  dummy2;
    //float2                  dummy2;

    // �𑜓x���グ�邩�ǂ���
    int texcoordMult; 
    int isSpecular;
    int isRimLighting;
    //float dummy;
    //// �F��ς���
    //float colorValue;

    //float dummy;

    // �`�悩�ǂ���
    int StencilRef;

    //float4 dummy2;

};

#define MAX_BONES 128
// ����zvs��
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
    row_major float4x4 lightViewProjection; // ���C�g�r���[�v���W�F�N�V�����s��
    float3             shadowColor;         // �e�̐F
    float              shadowBias;          // �[�x�l��r���̃I�t�Z�b�g
};


//--------------------------------------------
//	�t�H�O
//--------------------------------------------
//color		:���݂̐F
//viewPos	:���_
//Pos		:�s�N�Z���̈ʒu
//fogColor	:�t�H�O�̐F
//Near		:�t�H�O�̋ߋ���
//Far		:�t�H�O�̉�����
float3 Fog(float3 color, float3 viewPos, float3 Pos, float3 fogColor, float Near, float Far)
{
    float Len = length(viewPos - Pos);
    float fogalpha = saturate((Len - Near) / (Far - Near));
    return color.rgb * (1.0 - fogalpha) + fogColor * fogalpha;
};



//--------------------------------------------
//	�ꕔ�Ԃ�����
//--------------------------------------------
//color		:���݂̐F
//changeColor	:�ς���F
float3 RedChange(float3 color,float colorValue)
{

    float colorR = 0.5f;
    float colorAfterChange = color.r >= colorValue ? saturate(color.r + colorValue) : color.r;



    color.r = colorAfterChange;

    //return color.rgb * (1.0 - colorValue) + changeColor * colorValue;
    return color.rgb;
};