#include <imgui.h>
#include "TransForm2D.h"

TransForm2D::~TransForm2D()
{
}

void TransForm2D::OnGUI()
{
    ImGui::InputFloat2("Position", &position.x);
    ImGui::InputFloat("Angle", &angle);
    ImGui::InputFloat2("Scale", &scale.x);
}


