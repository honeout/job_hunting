// 平行光源情報
struct DirectionalLightData
{
	float4	direction;	// 向き
	float4	color;		// 色
};

// 点光源情報
struct PointLightData
{
	float4	position;	// 座標
	float4	color;		// 色
	float	range;		// 範囲
	float3	dummy;
};

// 点光源の最大数
static const int PointLightMax = 8;

// スポットライト情報
struct SpotLightData
{
	float4	position;	// 座標
	float4	direction;	// 向き
	float4	color;		// 色
	float	range;		// 範囲
	float	innerCorn;
	float	outerCorn;
	float	dummy;
};

// スポットライトの最大数
static const int SpotLightMax = 8;

/// <summary>
/// ランバート拡散反射計算関数
/// </summary>
/// <param name="normal">法線(正規化済み)</param>
/// <param name="lightVector">入射ベクトル(正規化済み)</param>
/// <param name="lightColor">入射光色</param>
/// <param name="kd">反射率(反射の強さ)</param>
/// <returns>拡散反射色</returns>
float3 CalcLambertDiffuse(float3 normal, float3 lightVector, float3 lightColor, float3 kd)
{
	// 法線とライト方向へのベクトルでの内積
	// 内積した結果、ライトから見て裏向きの面だとマイナスの結果が出てしまうため
	// それをそのまま乗算すると色がおかしくなります。
	// 0 以下であれば 0 になるように制限をつけましょう。
	float d = max(0, dot(normal, -lightVector));

	// 入射光色と内積の結果、および反射率をすべて乗算して返却しましょう。
	return kd * lightColor * d;
}


/// <summary>
/// フォンの鏡面反射計算関数
/// </summary>
/// <param name="normal">法線(正規化済み)</param>
/// <param name="lightVector">入射ベクトル(正規化済み)</param>
/// <param name="lightColor">入射光色</param>
/// <param name="eyeVector">視線ベクトル(正規化済み)</param>
/// <param name="shininess">光沢度(質感調整値)</param>
/// <param name="ks">反射率(反射の強さ)</param>
/// <returns>鏡面反射色</returns>
float3 CalcPhongSpecular(float3 normal, float3 lightVector, float3 lightColor, float3 eyeVector, float shininess, float3 ks)
{
	// 入射ベクトルを法線方向に対して反射させた反射ベクトルを算出しましょう
	float3 R = reflect(lightVector, normal);

	// 反射ベクトルと視線ベクトルとで内積
	// 内積した結果を0以下であれば0になるように制限をつけましょう。
	float d = max(dot(eyeVector, R), 0);

	// 光沢処理
	// 反射は表面がツルツルなほどハイライトは強くなり、範囲が小さくなります。
	// 逆に表面がザラザラなほど、ハイライトは弱くなり、範囲は広がります。
	// この変化は指数的に変化するため光沢度を乗数として使用します。
	d = pow(d, shininess);

	// 入射光色と内積の結果、及び反射率をすべて乗算して返却しましょう。
	return	lightColor * d * ks;
}

/// <summary>
/// ハーフランバート拡散反射計算関数
/// </summary>
/// <param name="normal">法線(正規化済み)</param>
/// <param name="lightVector">入射ベクトル(正規化済み)</param>
/// <param name="lightColor">入射光色</param>
/// <param name="kd"></param>
/// <returns></returns>
float3 CalcHalfLambert(float3 normal, float3 lightVector, float3 lightColor, float3 kd)
{
	float d = saturate(dot(normal, -lightVector));
	return lightColor * d * kd;
}

/// <summary>
/// リムライト
/// </summary>
/// <param name="normal">法線(正規化済み)</param>
/// <param name="normal">視線ベクトル(正規化済み)</param>
/// <param name="lightVector">入射ベクトル(正規化済み)</param>
/// <param name="lightColor">入射光色</param>
/// <param name="rimPower">リムライトの強さ(初期値はテキトーなので自分で設定するが吉)</param>
/// <returns></returns>
float3 CalcRimLight(float3 normal, float3 eyeVector, float3 lightVector, float3 lightColor, float rimPower = 3.0f)
{
	float rim = 1.0f - saturate(dot(normal, -eyeVector));
	return lightColor * pow(rim, rimPower) * saturate(dot(lightVector, -eyeVector));
}

/// <summary>
/// トゥーン拡散反射計算関数
/// </summary>
/// <param name="toonTexture">トゥーン用U方向ランプテクスチャ</param>
/// <param name="toonSamplerState">トゥーン用サンプラーステート</param>
/// <param name="normal">法線(正規化済み)</param>
/// <param name="lightVector">入射ベクトル(正規化済み)</param>
/// <param name="lightColor">入射光色</param>
/// <param name="kd">反射率(反射の強さ)</param>
/// <returns>拡散反射色</returns>
float3 CalcToonDiffuse(Texture2D toonTexture, SamplerState toonSamplerState, float3 normal, float3 lightVector, float3 lightColor, float3 kd)
{
	// 法線と入射ベクトルからU座標を求める
	float u = saturate(dot(normal, -lightVector) * 0.5f + 0.5f);

	// トゥーンテクスチャから色を
	float3 c = toonTexture.Sample(toonSamplerState, float2(u, 0.1f));

	return lightColor * c * kd;
}