#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

// ���s�������(���z)
struct DirectionalLightData
{
	DirectX::XMFLOAT4 direction; // ����
	DirectX::XMFLOAT4 color;     // �F
};

// �_����
struct PointLightData
{
	DirectX::XMFLOAT4 position; // ���W
	DirectX::XMFLOAT4 color; // �F
	float range;              // �͈�
	DirectX::XMFLOAT3 dummy;
};

// �_�����̍ő吔
static constexpr int PointLightMax = 8;

// �X�|�b�g���C�g���
struct SpotLightData
{
	DirectX::XMFLOAT4 position;			// ���W
	DirectX::XMFLOAT4 direction;		// ����
	DirectX::XMFLOAT4 color;			// �F
	float			  range;			// �͈�
	float			  innerCorn;        // �C���i�[�p�x�͈�
	float			  outerCorn;		// �A�E�^�[�p�x�͈�
	float			  dummy;
};

// �X�|�b�g���C�g�̍ő吔
static constexpr      int SpotLightMax = 8;

// �F���␳���
struct ColorGradingData
{
	float hueShift = 3;// �F������
	float saturation = 1;// �ʓx����
	float brigthness = 1.0f;// ���x����
	float dummy;
};



#define	UNIT_VN_01	//	���B�l�b�g
#define	UNIT_VN_02	//	�A�C���X����


// ���W�A���u���[���
struct RadialBlurData
{
	float radius = 10.0f;					// �u���[�̔��a
	int samplingCount = 10;					// �T���v���J�E���g
	DirectX::XMFLOAT2 center = {0.5f,0.5f}; // �u���[���S�_

	float mask_radius = 0;
};

#if	defined( UNIT_VN_01 )
//	�r�l�b�g���
struct VignetteData
{
	DirectX::XMFLOAT4	color = { 1.0f, 0.0f, 0.0f, 1.0f };	//	�����F
	DirectX::XMFLOAT2	center = { 0.5f, 0.5f };				//	�������S�_
	float				intensity = 0.5f;							//	���x
	float				smoothness = 0.2f;							//	���炩��

	bool				rounded = false;						//	�X�N���[���T�C�Y���l��
	float				roundness = 1.0f;							//	�l���x����

#if defined( UNIT_VN_02 )
	bool				iris = false;						//	�A�C���X����
#endif	//	defined( UNIT_VN_02 )
};
#endif	//	defined( UNIT_VN_01 )

// �K�E�X�t�B���^�[�v�Z���
struct GaussianFilterData
{
	int             kernelSize = 8; // �J�[�l���T�C�Y
	float			deviation = 10.0f; // �W���΍� 
	DirectX::XMFLOAT2 textureSize;    // �򂷃e�N�X�`���̃T�C�Y
};

// �K�E�X�t�B���^�[�̍ő�J�[�l���T�C�Y
static const int MaxkernelSize = 16;

// ���P�x���o�p���
struct LuminanceExtractionData
{
	float threshold = 0.5f; // 臒l
	float intensity = 1.0f; // �u���[���̋��x
	DirectX::XMFLOAT2 dummy2;
};

// �u���[�����
struct BloomData
{
	// ���P�x���o�p���
	LuminanceExtractionData luminanceExtractionData;
	// �K�E�X�u���[�p���
	GaussianFilterData gaussianFilterData;
};

// �|�X�g�G�t�F�N�g�̍ŏI�p�X�p���
struct FinalpassnData
{
	// �u���[���e�N�X�`��
	ID3D11ShaderResourceView* bloomTexture;
};

// �V���h�E�}�b�v�p���
struct ShadowMapData
{
	ID3D11ShaderResourceView* shadowMap; // �V���h�E�}�b�v�e�N�X�`��
	DirectX::XMFLOAT4X4       lightViewProjection; // ���C�g�r���[�v���W�F�N�V�����s��
	DirectX::XMFLOAT3         shadowColor; // �e�̐F 
	float                     shadowBias; // �[�x��r�p�̃I�t�Z�b�g�l
};

// �\�[�h�g���C���p���
struct SwordTraileData
{
	DirectX::XMFLOAT4X4		viewProjection;
	DirectX::XMFLOAT4		color;
};

// �����_�[�R���e�L�X�g
struct RenderContext
{
	ID3D11DeviceContext* deviceContext;

	// �J�������
	DirectX::XMFLOAT4       viewPosition;
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;

	DirectX::XMFLOAT4		lightDirection;
	float                   alpha;

	// ���C�g���
	DirectX::XMFLOAT4 ambientLightColor;
	DirectionalLightData directionalLightData;
	PointLightData pointLightData[PointLightMax]; // �_�������
	SpotLightData spotLightData[SpotLightMax];//�X�|�b�g���C�g���
	int      pointLightCount = 0;// �_������
	int      spotLightCount = 0; // �X�|�b�g���C�g��

	// ���ӌ���
	DirectX::XMFLOAT4 vignette_color = { 0.5f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT2 vignette_center = { 0.5f, 0.5f };
	float vignette_intensity = 0.5f;
	float vignette_smoothness = 0.2f;

	bool vignette_rounded = false;
	float vignette_roundness = 1.0f;

	DirectX::XMFLOAT2  color = { 1,1 };

	// �F���␳���
	ColorGradingData colorGradingData;


	//�K�E�X�t�B���^�[���
	GaussianFilterData    gaussianFilterData;


	// ���P�x���o�p���
	LuminanceExtractionData luminanceExtractionData;


	// �ŏI�p�X���
	FinalpassnData          finalpassnData;

    // �V���h�E�}�b�v���
	ShadowMapData shadowMapData;

	// ���W�A���u���[���
	RadialBlurData  radialBlurData;

	// �u���[���f�[�^
	BloomData bloomData;

	// �\�[�h�g���C�����
	SwordTraileData swordTraileData;


#if	defined( UNIT_VN_01 )
	//	�r�l�b�g���
	VignetteData			vignetteData;
#endif	//	defined( UNIT_VN_01 )

	// �e�N�X�`�����炷
	int texcoordMult = 1;

	int isSpecular = 1;

	// �������C�g������
	int isRimRightning = 1;

	// �`�悷�邩�ǂ���
	int StencilRef = 0;
};
