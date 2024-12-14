#include "FilterFunctions.hlsli"


#define	UNIT_VN_01	//	ヴィネット
#define	UNIT_VN_02	//	アイリス処理

struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

cbuffer CBFinalpass: register(b0)

{
    // 色調補正
    float hueShift;  // 色相調整
    float saturation; // 彩度調整
    float brightness; // 明度調整
    float dummy;


    //	ラジアルブラー情報
    float	rb_radius;			//	ブラーの半径
    int		rb_samplingCount;	//	サンプルカウント
    float2  rb_center;          //	ブラーの中心点


    float   rb_mask_radius;
    float3  rb_dummy;


#if	defined( UNIT_VN_01 )
    //	ヴィネット情報
    float4	vn_color;
    float2	vn_center;
    float	vn_intensity;
    float	vn_smoothness;

    float	vn_rounded;
    float	vn_roundness;
    float	vn_iris;
    float	vn_dummy;
#endif	//	defined( UNIT_VN_01 )
    //int     is_bl;
}