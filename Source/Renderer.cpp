#include <algorithm>
#include <imgui.h>
#include "Math.h"
#include "Movement.h"
#include "Renderer.h"

// �R���X�g���N�^
Rendere::Rendere()
{

}

// �f�X�g���N�^
Rendere::~Rendere()
{

}
#ifdef _DEBUG
// GUI�`��
void Rendere::OnGUI()
{
    ImGui::InputFloat("Render", &radius);
}
#endif // _DEBUG
bool Rendere::DrawDebugPrimitive()
{
    return false;
}




