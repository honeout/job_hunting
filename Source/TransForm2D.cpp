#include <imgui.h>
#include "TransForm2D.h"

TransForm2D::~TransForm2D()
{
}

void TransForm2D::OnGUI()
{
    ImGui::SliderFloat2("Position", &position.x,0,1280);
    ImGui::SliderFloat("Angle", &angle,0,180);
    ImGui::SliderFloat2("Scale", &scale.x,0,500);
}

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


