// UNIT11
//--------------------------------------
//  ƒVƒƒƒhƒEƒ}ƒbƒvQÆ—pî•ñ‚ÌŒvZ
//--------------------------------------
// worldPosition          : ƒ[ƒ‹ƒhÀ•W
// lightViewProjection    : ƒ‰ƒCƒgƒrƒ…[ƒvƒƒWƒFƒNƒgs—ñ
// •Ô‚·’l@                : ƒVƒƒƒhƒEƒ}ƒbƒvQÆ—p‚ÌUVÀ•W‹y‚Ñ[“xî•ñ
float3 CalcShadowTexcoord(float3 worldPosition, matrix lightViewProjection)
{
    // ƒNƒŠƒbƒv‹óŠÔ‚ÌÀ•W‚É•ÏŠ·
    float4 worldViewProjectionPosition = mul(float4(worldPosition, 1), lightViewProjection);

    // “§‹œZ‚µ‚ÄNDCÀ•W‚É•ÏŠ·
    worldViewProjectionPosition /= worldViewProjectionPosition.w;

    // NDCÀ•W‚ÌXYÀ•W‚ğUVÀ•W‚É•ÏŠ·
    // Z’l‚Í‚·‚Å‚É‚O`‚P‚Ì”ÍˆÍ‚É•ÏŠ·‚³‚ê‚Ä‚¢‚é‚Ì‚ÅA‰½‚à‚µ‚È‚­‚Ä‚¢‚¢
    //worldViewProjectionPosition.xy = normalize(mul(lightViewProjection, float4(worldPosition,1)));
    worldViewProjectionPosition.y = -worldViewProjectionPosition.y;
    worldViewProjectionPosition.xy = (0.5* worldViewProjectionPosition.xy + 0.5f);


    return worldViewProjectionPosition.xyz;
}

// UNIT11
//--------------------------------------
//  ƒVƒƒƒhƒEƒ}ƒbƒv‚©‚ç[“x’l‚ğæ“¾‚µ‚Ä‰e‚©‚Ç‚¤‚©‚ğ“n‚·
//--------------------------------------
// tex          : ƒVƒƒƒhƒEƒ}ƒbƒv
// lightViewProjection    : ƒTƒ“ƒvƒ‰ƒXƒXƒe[ƒg
// shadowTexcoord@                : ƒVƒƒƒhƒEƒ}ƒbƒvQÆ—pî•ñ
// shadowColor@                : ‰e‚ÌF
// shadowBias@                : [“x”äŠr—p‚ÌƒIƒtƒZƒbƒg’l
// •Ô‚·’l@                : ‰e‚©‚Ç‚¤‚©
float3 CalcShadowColor(Texture2D tex, SamplerState samplerState, float3 shadowTexcoord, float3
shadowColor, float shadowBias)
{
    // ƒVƒƒƒhƒEƒ}ƒbƒv‚©‚ç[“x’l‚ğæ“¾
    float depth = tex.Sample(samplerState, shadowTexcoord.xy).r;
    
    // [“x’l‚ğ”äŠr‚µ‚Ä‰e‚©‚Ç‚¤‚©‚ğ”»’è‚·‚é
    float ahadow = step(shadowTexcoord.z - depth, shadowBias);



    return lerp(shadowColor, 1, ahadow);
}

<<<<<<< HEAD

//// UNIT12
////--------------------------------------
////  PCFƒtƒBƒ‹ƒ^[Œƒ\ƒtƒgƒVƒƒƒhƒEƒ}ƒbƒv
////--------------------------------------
//// tex          : ƒVƒƒƒhƒEƒ}ƒbƒv
//// lightViewProjection    : ƒTƒ“ƒvƒ‰ƒXƒXƒe[ƒg
//// shadowTexcoord@                : ƒVƒƒƒhƒEƒ}ƒbƒvQÆ—pî•ñ
//// shadowColor@                : ‰e‚ÌF
//// shadowBias@                : [“x”äŠr—p‚ÌƒIƒtƒZƒbƒg’l
//// shadowColor                : ‰e‚ÌF
//// •Ô‚·’l@                : ‰e‚ÌF
//float3 CalcShadowColorPCFFilter(Texture2D tex, SamplerState samplerState, float3 shadowTexcoord, float3
//    shadowColor, float shadowBias)
//{
//    // ƒeƒNƒZƒ‹ƒTƒCƒY‚ÌŒvZ
//    float2 texelSize;
//    {
//        // ƒeƒNƒXƒ`ƒƒ‚Ì—§•‰¡•‚ğæ“¾‚·‚é
//        uint width, height;
//        tex.GetDimensions(width, height);
//
//        // Zo
//        texelSize = float2(1.0f / width, 1.0f / height);
//    }
//
//    float factor = 0;
//    static const int PCFKernelSize = 5; // w’è‚ÍŠï”‚É‚·‚é‚±‚Æ
//    for (int x = -PCFKernelSize / 2; x <= PCFKernelSize / 2; ++x)
//    {
//        for (int y = -PCFKernelSize / 2; y <= PCFKernelSize / 2; ++y)
//        {
//            // ƒVƒƒƒhƒEƒ}ƒbƒv‚©‚ç[“x’læ“¾
//            float depth = tex.Sample(samplerState, shadowTexcoord.xy + texelSize * float2(x, y)).r;
//            factor += step(shadowTexcoord.z - depth, shadowBias);
//        }
//    }
//    // [“x’l‚ğ”äŠr‚µ‚Ä‰e‚©‚Ç‚¤‚©‚ğ”»’è‚·‚é
//    return lerp(shadowColor, 1, factor / (PCFKernelSize*PCFKernelSize));
//}

=======
>>>>>>> parent of 8a0ff20 (ã¨ã‚Šã‚ãˆãšã‚·ã‚§ãƒ¼ãƒ€ãƒ¼ç¶ºéº—ãªå¥´å…¥ã£ãŸ)
// UNIT12
//--------------------------------------
//  PCFƒtƒBƒ‹ƒ^[Œƒ\ƒtƒgƒVƒƒƒhƒEƒ}ƒbƒv
//--------------------------------------
// tex          : ƒVƒƒƒhƒEƒ}ƒbƒv
// lightViewProjection    : ƒTƒ“ƒvƒ‰ƒXƒXƒe[ƒg
// shadowTexcoord@                : ƒVƒƒƒhƒEƒ}ƒbƒvQÆ—pî•ñ
// shadowColor@                : ‰e‚ÌF
// shadowBias@                : [“x”äŠr—p‚ÌƒIƒtƒZƒbƒg’l
// shadowColor                : ‰e‚ÌF
// •Ô‚·’l@                : ‰e‚ÌF
float3 CalcShadowColorPCFFilter(Texture2D tex, SamplerState samplerState, float3 shadowTexcoord, float3
    shadowColor, float shadowBias)
{
    // ƒeƒNƒZƒ‹ƒTƒCƒY‚ÌŒvZ
    float2 texelSize;
    {
        // ƒeƒNƒXƒ`ƒƒ‚Ì—§•‰¡•‚ğæ“¾‚·‚é
        uint width, height;
        tex.GetDimensions(width, height);

        // Zo
        texelSize = float2(1.0f / width, 1.0f / height);
    }

    float factor = 0;
    static const int PCFKernelSize = 5; // w’è‚ÍŠï”‚É‚·‚é‚±‚Æ
    for (int x = -PCFKernelSize / 2; x <= PCFKernelSize / 2; ++x)
    {
        for (int y = -PCFKernelSize / 2; y <= PCFKernelSize / 2; ++y)
        {
            // ƒVƒƒƒhƒEƒ}ƒbƒv‚©‚ç[“x’læ“¾
            float depth = tex.Sample(samplerState, shadowTexcoord.xy + texelSize * float2(x, y)).r;
            factor += step(shadowTexcoord.z - depth, shadowBias);
        }
    }
    // [“x’l‚ğ”äŠr‚µ‚Ä‰e‚©‚Ç‚¤‚©‚ğ”»’è‚·‚é
    return lerp(shadowColor, 1, factor / (PCFKernelSize*PCFKernelSize));
<<<<<<< HEAD
}

=======
}
>>>>>>> parent of 8a0ff20 (ã¨ã‚Šã‚ãˆãšã‚·ã‚§ãƒ¼ãƒ€ãƒ¼ç¶ºéº—ãªå¥´å…¥ã£ãŸ)
