//---------------------------------------------------
//  RGB�F��Ԃ̐��l����HSV�F��Ԃ̐��l�ւ̕ϊ��֐�
//---------------------------------------------------
// rgb:RGB�F��Ԃ̐��l�@(���̎O���F�̓x������p����
// �\�����镨��RGB�F���)
float3 RGB2HSV(float3 rgb)
{
    float3 hsv = 0;
    // v(���x)�̍ő�l�ƍŏ��l�����߂܂��傤�B
    float Vmax = max(rgb.x,max(rgb.y, rgb.z));
    float Vmin = min(rgb.x,min(rgb.y, rgb.z));

    // �ő�l�ƍŏ��l�̍�
    float delta = Vmax - Vmin;


    // V(���x)�P�ԋ����F��V�l�ɂ���
    hsv.z = Vmax;

    // S(�ʓx)�ő�l�ƍŏ��l�̍��𐳋K�����ċ��߂�B
    hsv.y = (delta / Vmax);

    // H(�F��)RGB�̂����ő�l�ƍŏ��l�̍����狁�߂�
    if (hsv.y > 0.0f)
    {
        // r�̎�
        if (Vmax == rgb.x)
        {
            //hsv.x = ((rgb.g - rgb.b) / hsv.y)+ (60*hsv.y);
            //hsv.x = ((rgb.y - rgb.z) / delta)+ (60* delta);
            hsv.x = 60*((rgb.y - rgb.z) / delta);
            //hsv.x = (((rgb.g - rgb.b) + (60 * delta)) /  delta);
        }
        // g�̎�
        if (Vmax == rgb.y)
        {
            //hsv.x = (((rgb.b - rgb.r) / hsv.y) + (60 * hsv.y)) +120;
            hsv.x = 60 * ((rgb.z - rgb.x)/ delta) +120;
            //hsv.x = (((rgb.b - rgb.r)+ (60 * delta)) /  delta) +120;
        }
        // b�̎�
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
//  HSV�F��Ԃ̐��l����RGB�F��Ԃ̐��l�ւ̕ϊ��֐�
//---------------------------------------------------
// hsv:HSV�F��Ԃ̐��l�@(�F�̎O�����ƌĂ΂��F��(Hue)�A
// �ʓx(Saturation)�A���x(Value, Brightness)�ŕ\������)
float3 HSV2RGB(float3 hsv)
{
    float3 rgb = 0;
    if (hsv.y == 0)
    {
        // S(�ʓx)���O�Ɠ������Ȃ�Ζ��F�������͊D�F
        rgb.x = rgb.y = rgb.z = hsv.z;
    }
    else
    {
        // �F�z��H(�F��)�̈ʒu��S(�ʓx)�AV(���x)
        // ����RGB�l���Z�o����B
        float Vmax = hsv.z;
        float Vmin = Vmax - (hsv.y * Vmax);
        hsv.x %= 360;// 0~360�ɕϊ�
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
//  RGB�F��Ԃ̐��l����P�x�l�ւ̕ϊ��֐�
//---------------------------------------------------
// rgb:RGB�F��Ԃ̐��l
float3 RGB2Luminance(float3 rgb)
{
    static const float3 luminanceValue = float3(0.299f, 0.587f, 0.114f);
    return dot(luminanceValue, rgb);
}