#include <imgui.h>
#include "TransForm2D.h"

TransForm2D::~TransForm2D()
{
}
#ifdef _DEBUG
void TransForm2D::OnGUI()
{
    ImGui::SliderFloat2("Position", &position.x,0,1280);
    ImGui::SliderFloat2("TexPosition", &texPosition.x,0,1280);
    ImGui::SliderFloat("Angle", &angle,0,180);
    ImGui::SliderFloat2("Scale", &scale.x,0,500);
    ImGui::SliderFloat2("TexScale", &texScale.x,0,500);
}
#endif // _DEBUG

// 徐々に縮める
float TransForm2D::UpdateGage(float currentScale, float maxScale,  float lerpSpeed,  float elapsedTime)
{
    // 線形補完に使う割合
    float interpolationRate = lerpSpeed * elapsedTime;

    // 線形補完で徐々に縮める
    return Mathf::Lerp(currentScale, maxScale, interpolationRate);
}
void TransForm2D::Shake()
{
    if (shakeTimeUi <= shakeTimeUiMin)
        memoryPosition = position;
    if (!GetShakeEnd())
    {
        if (position.y > max)
        {
            checkPositionY = true;
        }
        if (position.y < min)
        {
            checkPositionY = false;
        }
        if (checkPositionY)
            --position.y;
        if (!checkPositionY)
            ++position.y;
        ++shakeTimeUi;
    }
    else
    {
        shakeTimeUi = shakeTimeUiMin;
        position = memoryPosition;
    }
}
// 収縮
void TransForm2D::ShrinkTexture(float shrinkValueMax, float shrinkValueMin, float elapsedTime)
{
    // X
    if (scale.x > shrinkValueMax)
    {
        isAtShrinkX = true;
    }
    if (scale.x < shrinkValueMin)
    {
        isAtShrinkX = false;
    }
    if (isAtShrinkX)
        scale.x -= shrinkValue;
    if (!isAtShrinkX)
        scale.x += shrinkValue;
    // Y
    if (scale.y > shrinkValueMax)
    {
        isAtShrinkY = true;
    }
    if (scale.y < shrinkValueMin)
    {
        isAtShrinkY = false;
    }
    if (isAtShrinkY)
        scale.y -= shrinkValue;
    if (!isAtShrinkY)
        scale.y += shrinkValue;
}

void TransForm2D::IncrementToMax(float increment, float incrementMax, float& currentValue)
{
    if (currentValue - FLT_EPSILON <= incrementMax + FLT_EPSILON)
    currentValue += increment;
}



