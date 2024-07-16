// ���s�������

struct DirectionalLightData
{
    float4 direction; // ����
    float4 color; // �F
};

// �_�������
struct PointLightData
{
    float4 position; // ���W
    float4 color;// �F
    float range; //�͈�
    float3 dummy;
};

// �_�����̍ő吔
static const int PointLightMax = 8;

// �X�|�b�g���C�g���
struct SpotLightData
{
    float4 position; // ���W
    float4 direction; // ����
    float4 color;// �F
    float range; //�͈�
    float innerCorn;
    float outerCorn;
    float dummy;
};
// �X�|�b�g���C�g�̍ő吔
static const int SpotLightMax = 8;

//---------------------------------------------------
//  �����o�[�g�g�U���ˌv�Z�֐�
//---------------------------------------------------
// normal�@�@�@: �@��(���K���ς�)
// lightVector : ���˃x�N�g��(���K���ς�)
// lightColor  : ���ˌ��F
// kd          : ���˗�(�ǂ�y�̋���)
// �Ԃ��l�@�@�@ : �g�U���ːF 
float3 CalcLambertDiffuse(float3 normal, float3 LightVector, float3 LightColor, float3 kd)
{
    // �@���ƃ��C�g�����ւ̃x�N�g���ł̓���
    // ���ς������ʁA���C�g����݂ė������̖ʂ��ƃ}�C�i�X�̌��ʂ��o�Ă��܂�����
    // ��������̂܂܏�Z����ƐF�����������Ȃ�܂��B
    // 0�ȉ��ŗL��΂O�ɂȂ�悤�ɐ��������܂��傤�B
    // -LightVector���R����
    float d = max(dot(-LightVector,normal),0)*LightColor*kd;
    //float d = clamp((dot(-lightVector,normal)*0.5+0.5),0,1)*lightColor*kd;

    // ���ˌ��F�Ɠ��ς̌��ʁA����є��˗���S�ď��U���ĕԋp���܂��傤�B
    return kd* LightColor*d;
 }

//-----------------------------------------------------
//  �t�H���̋��ʔ��ˌv�Z�֐�
//-----------------------------------------------------
// normal        : �@��(���K���ς�)
// lightVector   : ���˃x�N�g��(���K���ς�)
// lightColor    : ���ˌ��F
// eyeVector     : �����x�N�g��(���K���ς�)
// Shiness       : ���ړx(���������l)
// ks            : ���˗�(���˂̋���)
// �Ԃ��l         : ���ʔ��ːF
float3 CalcPhongSpecular(float3 normal, float3 lightVector,
    float3 lightColor, float3 eyeVector, float shininess, float3 ks)
{
    // ���˃x�N�g����@�������ɑ΂��Ĕ��˂��������˃x�N�g�����Z�o���܂��傤
    float3 R = reflect(lightVector, normal);

    // ���˃x�N�g���Ǝ����x�N�g���Ƃœ���
    // ���ς������ʂ��O�ȉ��ŗL��΂O�ɂȂ�悤�ɐ��������܂��傤
    float d = max(dot(R,eyeVector),0);

    // ���ڏ���
    // ���˂͕\�ʂ����Ȃقǃn�C���C�g�͋����Ȃ�A�͈͂��������Ȃ�܂��B
    // �t�ɕ\�ʂ����炴��Ȓ��A�n�C���C�g�͎キ�Ȃ�A�͈͍͂L����܂��B
    // ���̕ω��͎w���I�ɕω����邽�ߌ��ړx���搔�Ƃ��Ďg�p���܂��B
    d = pow(d, shininess);
    // ���ˌ��F�Ɠ��ς̌��ʁA�y�є��˗�������t�����U���ĕԋp���܂��傤
    return d * lightColor * ks;
}
 
//---------------------------------------------------
//  �n�[�t�����o�[�g�g�U���ˌv�Z�֐�
//---------------------------------------------------
// normal�@�@�@: �@��(���K���ς�)
// lightVector : ���˃x�N�g��(���K���ς�)
// lightColor  : ���ˌ��F
// kd          : ���˗�(�ǂ�y�̋���)
// �Ԃ��l�@�@�@ : �g�U���ːF 
float3 CalcHalfLambert(float3 normal, float3 LightVector, float3 LightColor, float3 kd)
{
    // �@���ƃ��C�g�����ւ̃x�N�g���ł̓���
    // ���ς������ʁA���C�g����݂ė������̖ʂ��ƃ}�C�i�X�̌��ʂ��o�Ă��܂�����
    // ��������̂܂܏�Z����ƐF�����������Ȃ�܂��B
    // 0�ȉ��ŗL��΂O�ɂȂ�悤�ɐ��������܂��傤�B
    // -LightVector���R����
    //float d = max(dot(-LightVector,normal),0)*LightColor*kd;
    float d = clamp((dot(-LightVector, normal) * 0.5 + 0.5), 0, 1) * LightColor * kd;


    // ���ˌ��F�Ɠ��ς̌��ʁA����є��˗���S�ď��U���ĕԋp���܂��傤�B
    return  LightColor * d * kd;
}


//---------------------------------------------------
//  �������C�g
//---------------------------------------------------
// normal�@�@�@ : �@��(���K���ς�)
// eyeVector   : �����x�N�g��(���K���ς�)
// lightVector : ���˃x�N�g��(���K���ς�)
// lightColor  : ���ˌ��F
// rimPower    : �������C�g�̋���(�����l�̓e�L�g�[�Ȃ̂Ŏ����Ŏ����Ă����邪�g)
//
float3 CalcRimLight(float3 normal,float3 eyeVector, float3 LightVector, float3 LightColor, float rimPower
    = 3.0f)
{

    float rim = 1.0 - saturate(dot(eyeVector, normal));
    rim = pow(rim, rimPower);

    // ���ˌ��F�Ɠ��ς̌��ʁA����є��˗���S�ď��U���ĕԋp���܂��傤�B
    return  LightColor * rim*saturate(dot(eyeVector, LightVector));
}

//-----------------------------------------------------
//  �g�D�[���g�U���ˌv�Z�֐�
//-----------------------------------------------------
// toonTexture   : �g�D�[���pU���������v�e�N�X�`��
// toonSamplerState : �g�D�[���p�T���v���X�X�e�[�g
// normal        : �@��(���K���ς�)
// lightVector   : ���˃x�N�g��(���K���ς�)
// lightColor    : ���ˌ��F
// kd            : ���˗�(���˂̋���)
// �Ԃ��l         : �g�U���ːF
float3 CalcToonDiffuse(Texture2D toonTexture, SamplerState toonSamplerState,
    float3 normal, float3 LightVector,
    float3 LightColor, float3 kd)
{
    // ���˂Ɠ��˃x�N�g������U���W�����߂�
    // �O�`�P�܂ł�[�߂�ׂ̌v�Z���̊ԂŃe�N�X�`������Z���F�����F�𔲂����
    float u = clamp(dot(normal, - LightVector) * 0.5f + 0.5f,0.01f,0.99f);

    float3 c = toonTexture.Sample(toonSamplerState, float2(u,0.1f));

    // ���˃x�N�g���Ǝ����x�N�g���Ƃœ���
    // ���ς������ʂ��O�ȉ��ŗL��΂O�ɂȂ�悤�ɐ��������܂��傤
 /*   float lightColor*c*kd;*/

    // ���ڏ���
    // ���˂͕\�ʂ����Ȃقǃn�C���C�g�͋����Ȃ�A�͈͂��������Ȃ�܂��B
    // �t�ɕ\�ʂ����炴��Ȓ��A�n�C���C�g�͎キ�Ȃ�A�͈͍͂L����܂��B
    // ���̕ω��͎w���I�ɕω����邽�ߌ��ړx���搔�Ƃ��Ďg�p���܂��B

    return LightColor * c * kd;
}