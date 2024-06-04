#include <algorithm>
#include <imgui.h>
#include "Math.h"
#include "Movement.h"
#include "Renderer.h"

// コンストラクタ
Renderer::Renderer()
{

}

// デストラクタ
Rendere::~Rendere()
{

}

// GUI描画
void Rendere::OnGUI()
{
    ImGui::InputFloat("Render", &radius);
}

bool Rendere::DrawDebugPrimitive()
{
    return false;
}




