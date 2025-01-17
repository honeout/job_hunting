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
void TransForm2D::Shake()
{
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
        shakeTimeUi = 0;
    }
}
// Žûk
void TransForm2D::ShrinkTexture(float shrinkValueMax, float shrinkValueMin, float elapsedTime)
{
    //timeInterval += elapsedTime;

    //if (timeInterval >= timeIntervalMax)
    //{
    //    //scale.x = (scale.x + FLT_EPSILON) >= (shrinkValueMax - FLT_EPSILON) ? scale.x - shrinkValue : scale.x;

    //    //scale.x = (scale.x + FLT_EPSILON) <= (shrinkValueMin - FLT_EPSILON) ? scale.x + shrinkValue : scale.x;
    //}
    //isShrink = timeInterval >= timeIntervalMax ? true : false;

    //if (isShrink)

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

    /*++timeInterval;*/

    //else
    //{
    //    timeInterval = 0;
    //}
}



