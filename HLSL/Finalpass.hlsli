#include "FilterFunctions.hlsli"

#define	UNIT_VN_01	//	���B�l�b�g
#define	UNIT_VN_02	//	�A�C���X����

struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

cbuffer CBFinalpass: register(b0)

{
    // �F���␳
    float hueShift;  // �F������
    float saturation; // �ʓx����
    float brightness; // ���x����
    float dummy;

#if	defined( UNIT_VN_01 )
    //	���B�l�b�g���
    float4	vn_color;
    float2	vn_center;
    float	vn_intensity;
    float	vn_smoothness;

    float	vn_rounded;
    float	vn_roundness;
    float	vn_iris;
    float	vn_dummy;
#endif	//	defined( UNIT_VN_01 )
}