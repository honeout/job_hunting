#include <algorithm>
#include <imgui.h>
#include "Math.h"
#include "Movement.h"
#include "Renderer.h"

// コンストラクタ
Rendere::Rendere()
{

}

// デストラクタ
Rendere::~Rendere()
{

}
#ifdef _DEBUG
// GUI描画
void Rendere::OnGUI()
{
    ImGui::InputFloat("Render", &radius);
}
#endif // _DEBUG
bool Rendere::DrawDebugPrimitive()
{
    return false;
}




