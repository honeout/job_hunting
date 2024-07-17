// ���s�������
struct DirectionalLightData
{
	float4	direction;	// ����
	float4	color;		// �F
};

// �_�������
struct PointLightData
{
	float4	position;	// ���W
	float4	color;		// �F
	float	range;		// �͈�
	float3	dummy;
};

// �_�����̍ő吔
static const int PointLightMax = 8;

// �X�|�b�g���C�g���
struct SpotLightData
{
	float4	position;	// ���W
	float4	direction;	// ����
	float4	color;		// �F
	float	range;		// �͈�
	float	innerCorn;
	float	outerCorn;
	float	dummy;
};

// �X�|�b�g���C�g�̍ő吔
static const int SpotLightMax = 8;

/// <summary>
/// �����o�[�g�g�U���ˌv�Z�֐�
/// </summary>
/// <param name="normal">�@��(���K���ς�)</param>
/// <param name="lightVector">���˃x�N�g��(���K���ς�)</param>
/// <param name="lightColor">���ˌ��F</param>
/// <param name="kd">���˗�(���˂̋���)</param>
/// <returns>�g�U���ːF</returns>
float3 CalcLambertDiffuse(float3 normal, float3 lightVector, float3 lightColor, float3 kd)
{
	// �@���ƃ��C�g�����ւ̃x�N�g���ł̓���
	// ���ς������ʁA���C�g���猩�ė������̖ʂ��ƃ}�C�i�X�̌��ʂ��o�Ă��܂�����
	// ��������̂܂܏�Z����ƐF�����������Ȃ�܂��B
	// 0 �ȉ��ł���� 0 �ɂȂ�悤�ɐ��������܂��傤�B
	float d = max(0, dot(normal, -lightVector));

	// ���ˌ��F�Ɠ��ς̌��ʁA����є��˗������ׂď�Z���ĕԋp���܂��傤�B
	return kd * lightColor * d;
}


/// <summary>
/// �t�H���̋��ʔ��ˌv�Z�֐�
/// </summary>
/// <param name="normal">�@��(���K���ς�)</param>
/// <param name="lightVector">���˃x�N�g��(���K���ς�)</param>
/// <param name="lightColor">���ˌ��F</param>
/// <param name="eyeVector">�����x�N�g��(���K���ς�)</param>
/// <param name="shininess">����x(���������l)</param>
/// <param name="ks">���˗�(���˂̋���)</param>
/// <returns>���ʔ��ːF</returns>
float3 CalcPhongSpecular(float3 normal, float3 lightVector, float3 lightColor, float3 eyeVector, float shininess, float3 ks)
{
	// ���˃x�N�g����@�������ɑ΂��Ĕ��˂��������˃x�N�g�����Z�o���܂��傤
	float3 R = reflect(lightVector, normal);

	// ���˃x�N�g���Ǝ����x�N�g���Ƃœ���
	// ���ς������ʂ�0�ȉ��ł����0�ɂȂ�悤�ɐ��������܂��傤�B
	float d = max(dot(eyeVector, R), 0);

	// ���򏈗�
	// ���˂͕\�ʂ��c���c���Ȃقǃn�C���C�g�͋����Ȃ�A�͈͂��������Ȃ�܂��B
	// �t�ɕ\�ʂ��U���U���ȂقǁA�n�C���C�g�͎キ�Ȃ�A�͈͍͂L����܂��B
	// ���̕ω��͎w���I�ɕω����邽�ߌ���x���搔�Ƃ��Ďg�p���܂��B
	d = pow(d, shininess);

	// ���ˌ��F�Ɠ��ς̌��ʁA�y�є��˗������ׂď�Z���ĕԋp���܂��傤�B
	return	lightColor * d * ks;
}

/// <summary>
/// �n�[�t�����o�[�g�g�U���ˌv�Z�֐�
/// </summary>
/// <param name="normal">�@��(���K���ς�)</param>
/// <param name="lightVector">���˃x�N�g��(���K���ς�)</param>
/// <param name="lightColor">���ˌ��F</param>
/// <param name="kd"></param>
/// <returns></returns>
float3 CalcHalfLambert(float3 normal, float3 lightVector, float3 lightColor, float3 kd)
{
	float d = saturate(dot(normal, -lightVector));
	return lightColor * d * kd;
}

/// <summary>
/// �������C�g
/// </summary>
/// <param name="normal">�@��(���K���ς�)</param>
/// <param name="normal">�����x�N�g��(���K���ς�)</param>
/// <param name="lightVector">���˃x�N�g��(���K���ς�)</param>
/// <param name="lightColor">���ˌ��F</param>
/// <param name="rimPower">�������C�g�̋���(�����l�̓e�L�g�[�Ȃ̂Ŏ����Őݒ肷�邪�g)</param>
/// <returns></returns>
float3 CalcRimLight(float3 normal, float3 eyeVector, float3 lightVector, float3 lightColor, float rimPower = 3.0f)
{
	float rim = 1.0f - saturate(dot(normal, -eyeVector));
	return lightColor * pow(rim, rimPower) * saturate(dot(lightVector, -eyeVector));
}

/// <summary>
/// �g�D�[���g�U���ˌv�Z�֐�
/// </summary>
/// <param name="toonTexture">�g�D�[���pU���������v�e�N�X�`��</param>
/// <param name="toonSamplerState">�g�D�[���p�T���v���[�X�e�[�g</param>
/// <param name="normal">�@��(���K���ς�)</param>
/// <param name="lightVector">���˃x�N�g��(���K���ς�)</param>
/// <param name="lightColor">���ˌ��F</param>
/// <param name="kd">���˗�(���˂̋���)</param>
/// <returns>�g�U���ːF</returns>
float3 CalcToonDiffuse(Texture2D toonTexture, SamplerState toonSamplerState, float3 normal, float3 lightVector, float3 lightColor, float3 kd)
{
	// �@���Ɠ��˃x�N�g������U���W�����߂�
	float u = saturate(dot(normal, -lightVector) * 0.5f + 0.5f);

	// �g�D�[���e�N�X�`������F��
	float3 c = toonTexture.Sample(toonSamplerState, float2(u, 0.1f));

	return lightColor * c * kd;
}