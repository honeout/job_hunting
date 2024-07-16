//---------------------------------------------------
//  RGB色空間の数値からHSV色空間の数値への変換関数
//---------------------------------------------------
// rgb:RGB色空間の数値　(光の三原色の度合いを用いて
// 表現する物をRGB色空間)
float3 RGB2HSV(float3 rgb)
{
    float3 hsv = 0;
    // v(明度)の最大値と最小値を求めましょう。
    float Vmax = max(rgb.x,max(rgb.y, rgb.z));
    float Vmin = min(rgb.x,min(rgb.y, rgb.z));

    // 最大値と最小値の差
    float delta = Vmax - Vmin;


    // V(明度)１番強い色をV値にする
    hsv.z = Vmax;

    // S(彩度)最大値と最小値の差を正規化して求める。
    hsv.y = (delta / Vmax);

    // H(色相)RGBのうち最大値と最小値の差から求める
    if (hsv.y > 0.0f)
    {
        // rの時
        if (Vmax == rgb.x)
        {
            //hsv.x = ((rgb.g - rgb.b) / hsv.y)+ (60*hsv.y);
            //hsv.x = ((rgb.y - rgb.z) / delta)+ (60* delta);
            hsv.x = 60*((rgb.y - rgb.z) / delta);
            //hsv.x = (((rgb.g - rgb.b) + (60 * delta)) /  delta);
        }
        // gの時
        if (Vmax == rgb.y)
        {
            //hsv.x = (((rgb.b - rgb.r) / hsv.y) + (60 * hsv.y)) +120;
            hsv.x = 60 * ((rgb.z - rgb.x)/ delta) +120;
            //hsv.x = (((rgb.b - rgb.r)+ (60 * delta)) /  delta) +120;
        }
        // bの時
        if (Vmax == rgb.z)
        {
            //hsv.x = (((rgb.r - rgb.g) / hsv.y) + (60 * hsv.y)) +240;
            hsv.x = 60 * ((rgb.x - rgb.y) / delta) +240;
            //hsv.x = (((rgb.r - rgb.g) + (60 * delta)) / delta) +240;
        }

        if (hsv.x < 0)
        {
            hsv.x += 360;
        }
    }
    return hsv;
}

//---------------------------------------------------
//  HSV色空間の数値からRGB色空間の数値への変換関数
//---------------------------------------------------
// hsv:HSV色空間の数値　(色の三属性と呼ばれる色相(Hue)、
// 彩度(Saturation)、明度(Value, Brightness)で表現する)
float3 HSV2RGB(float3 hsv)
{
    float3 rgb = 0;
    if (hsv.y == 0)
    {
        // S(彩度)が０と等しいならば無色もしくは灰色
        rgb.x = rgb.y = rgb.z = hsv.z;
    }
    else
    {
        // 色循環のH(色相)の位置とS(彩度)、V(明度)
        // からRGB値を算出する。
        float Vmax = hsv.z;
        float Vmin = Vmax - (hsv.y * Vmax);
        hsv.x %= 360;// 0~360に変換
        float Huei = (int)(hsv.x / 60);
        float Huef = hsv.x / 60 - Huei;
        float p = Vmax * (1.0 - hsv.y);
        float q = Vmax * (1.0 - hsv.y*Huef);
        float t = Vmax * (1.0 - hsv.y*(1.0f - Huef));
        if (Huei == 0)
        {
            rgb.x = Vmax;
            rgb.y = t;
            rgb.z = p;
        }
        else if (Huei == 1)
        {
            rgb.x = q;
            rgb.y = Vmax;
            rgb.z = p;
        }
        else if (Huei == 2)
        {
            rgb.x = p;
            rgb.y = Vmax;
            rgb.z = t;
        }
        else if (Huei == 3)
        {
            rgb.x = p;
            rgb.y = q;
            rgb.z = Vmax;

        }
        else if (Huei == 4)
        {
            rgb.x = t;
            rgb.y = p;
            rgb.z = Vmax;
        }
        else if (Huei == 5)
        {
            rgb.x = Vmax;
            rgb.y = p;
            rgb.z = q;
        }

    }
    return rgb;
}
//---------------------------------------------------
//  RGB色空間の数値から輝度値への変換関数
//---------------------------------------------------
// rgb:RGB色空間の数値
float3 RGB2Luminance(float3 rgb)
{
    static const float3 luminanceValue = float3(0.299f, 0.587f, 0.114f);
    return dot(luminanceValue, rgb);
}