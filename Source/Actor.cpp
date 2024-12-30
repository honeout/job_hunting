#include "Actor.h"
#include <imgui.h>
#include "Graphics\Graphics.h"
#include "Component.h"


// 開始処理
void Actor::Start()
{
	// 全て初期化
	for (std::shared_ptr<Component>& component : components)
	{
		component->Start();
	}
}

// 更新
void Actor::Update(float elapsedTime)
{
	// コンポーネント全て更新
	for (std::shared_ptr<Component>& component : components)
	{
		component->Update(elapsedTime);
	}
}

void Actor::Render(RenderContext rc, ModelShader* shader)
{
	// 3D描画
	for (std::shared_ptr<Component>& component : components)
	{
		component->Render(rc, *shader);
	}
}

void Actor::Render(RenderContext rc, SpriteShader* shader)
{
	// ２D描画
	for (std::shared_ptr<Component>& component : components)
	{
		component->Render(rc, *shader);
	}
}

void Actor::Render2D(RenderContext rc, SpriteShader* shader)
{
	// ２D描画
	for (std::shared_ptr<Component>& component : components)
	{


		component->Render2D(rc, *shader);
	}
}


#ifdef _DEBUG
// GUI表示
void Actor::OnGUI()
{
	// 名前
	{
		char buffer[1024];
		::strncpy_s(buffer, sizeof(buffer), GetName(), sizeof(buffer));
		if (ImGui::InputText("Name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			SetName(buffer);
		}
	}

	//// トランスフォーム
	//if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	//{
	//	ImGui::InputFloat3("Position", &position.x);
	//}

	// コンポーネント
	for (std::shared_ptr<Component>& component : components)
	{
		ImGui::Spacing();
		ImGui::Separator();

		if (ImGui::CollapsingHeader(component->GetName(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			//ImGui::PushID(StringToHash(component->GetName()));
			component->OnGUI();
			//ImGui::PopID();
		}
	}
}
#endif // _DEBUG

// 作成 作った奴を入れる。
std::shared_ptr<Actor> ActorManager::Create()
{
	std::shared_ptr<Actor> actor = std::make_shared<Actor>();
	{
		static int id = 0;
		char name[256];
		::sprintf_s(name, sizeof(name), "Actor%d", id++);
		actor->SetName(name);
	}
	startActors.emplace_back(actor);
	return actor;
}

// 削除 に追加
void ActorManager::Remove(std::shared_ptr<Actor> actor)
{
	removeActors.insert(actor);
}

// 更新
void ActorManager::Update(float elapsedTime)
{
	for (std::shared_ptr<Actor>& actor : startActors)
	{
		actor->Start();
		updateActors.emplace_back(actor);
	}
	startActors.clear();

	for (std::shared_ptr<Actor>& actor : updateActors)
	{
		actor->Update(elapsedTime);
	}

	for (const std::shared_ptr<Actor>& actor : removeActors)
	{
		std::vector<std::shared_ptr<Actor>>::iterator itStart = std::find(startActors.begin(), startActors.end(), actor);
		if (itStart != startActors.end())
		{
			startActors.erase(itStart);
		}

		std::vector<std::shared_ptr<Actor>>::iterator itUpdate = std::find(updateActors.begin(), updateActors.end(), actor);
		if (itUpdate != updateActors.end())
		{
			updateActors.erase(itUpdate);
		}

		std::set<std::shared_ptr<Actor>>::iterator itSelection = selectionActors.find(actor);
		if (itSelection != selectionActors.end())
		{
			selectionActors.erase(itSelection);
		}
	}
	removeActors.clear();
}


// 描画
void ActorManager::Render(RenderContext rc, ModelShader* shader)
{
	Graphics& graphics = Graphics::Instance();
	
	// 3D
    for (std::shared_ptr<Actor>& actor : updateActors)
	{
		if (!actor->GetCheck2d())
		// モデルがあれば描画
		actor->Render(rc, shader);
		
		
	}





//#ifdef _DEBUG
//	// リスター描画
//	DrawLister();
//
//	// 詳細描画
//	DrawDetail();
//#endif // _DEBUG

}

void ActorManager::Render(RenderContext rc, SpriteShader* shader)
{
	Graphics& graphics = Graphics::Instance();


	for (std::shared_ptr<Actor>& actor : updateActors)
	{
		if (actor->GetCheck2d())
			// モデルがあれば描画
			actor->Render2D(rc, shader);
	}
}

void ActorManager::RenderGui()
{
	#ifdef _DEBUG
	// リスター描画
	DrawLister();

	// 詳細描画
	DrawDetail();
#endif // _DEBUG
}

void ActorManager::Render2D(RenderContext rc, SpriteShader* shader)
{
	Graphics& graphics = Graphics::Instance();


	for (std::shared_ptr<Actor>& actor : updateActors)
	{
		if (actor->GetCheck2d())
		// モデルがあれば描画
		actor->Render2D(rc, shader);
	}
}


void ActorManager::Clear()
{
	
	for (std::shared_ptr<Actor>& actor : updateActors)// 
	{
		// 実体を消した管理している数はそのまま
		actor.reset();
		
	}
	updateActors.clear();


    
	
}
#ifdef _DEBUG
// リスター描画
void ActorManager::DrawLister()
{
	ImGui::SetNextWindowPos(ImVec2(30, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	hiddenLister = !ImGui::Begin("Actor Lister", nullptr, ImGuiWindowFlags_None);
	if (!hiddenLister)
	{
		for (std::shared_ptr<Actor>& actor : updateActors)
		{
			ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_Leaf;

			if (selectionActors.find(actor) != selectionActors.end())
			{
				nodeFlags |= ImGuiTreeNodeFlags_Selected;
			}

			ImGui::TreeNodeEx(actor.get(), nodeFlags, actor->GetName());

			if (ImGui::IsItemClicked())
			{
				// 単一選択だけ対応しておく
				ImGuiIO& io = ImGui::GetIO();
				selectionActors.clear();
				selectionActors.insert(actor);
			}

			ImGui::TreePop();
		}
	}
	ImGui::End();
}
#endif // _DEBUG


#ifdef _DEBUG
// 詳細描画
void ActorManager::DrawDetail()
{
	ImGui::SetNextWindowPos(ImVec2(950, 50), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	hiddenDetail = !ImGui::Begin("Actor Detail", nullptr, ImGuiWindowFlags_None);
	if (!hiddenDetail)
	{
		std::shared_ptr<Actor> lastSelected = selectionActors.empty() ? nullptr : *selectionActors.rbegin();
		if (lastSelected != nullptr)
		{
			lastSelected->OnGUI();
		}
	}
	ImGui::End();
}
#endif // _DEBUG
