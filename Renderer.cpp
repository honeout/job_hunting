#include <algorithm>
#include <imgui.h>
#include "Math.h"
#include "Movement.h"
#include "Renderer.h"

// �R���X�g���N�^
Renderer::Renderer()
{

}

// �f�X�g���N�^
Rendere::~Rendere()
{

}

// GUI�`��
void Rendere::OnGUI()
{
    ImGui::InputFloat("Render", &radius);
}

bool Rendere::DrawDebugPrimitive()
{
    return false;
}




