#include "Actor.h"
#include <imgui.h>
#include "Graphics\Graphics.h"
#include "Component.h"


// ŠJnˆ—
void Actor::Start()
{
	for (std::shared_ptr<Component>& component : components)
	{
		component->Start();
	}
}

// XV
void Actor::Update(float elapsedTime)
{
	//// ƒAƒjƒ[ƒVƒ‡ƒ“‚ÌXV
	//if (model != nullptr)
	//{
	//	model->UpdateAnimation(elapsedTime,true);
	//}

	for (std::shared_ptr<Component>& component : components)
	{
		component->Update(elapsedTime);
	}
}

void Actor::Render(RenderContext rc)
{
	for (std::shared_ptr<Component>& component : components)
	{


		component->Render(rc);
	}
}


// GUI•\¦
void Actor::OnGUI()
{
	// –¼‘O
	{
		char buffer[1024];
		::strncpy_s(buffer, sizeof(buffer), GetName(), sizeof(buffer));
		if (ImGui::InputText("Name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			SetName(buffer);
		}
	}

	//// ƒgƒ‰ƒ“ƒXƒtƒH[ƒ€
	//if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	//{
	//	ImGui::InputFloat3("Position", &position.x);
	//}

	// ƒRƒ“ƒ|[ƒlƒ“ƒg
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

// ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ
//void Actor::LoadModel(const char* filename)
//{
//	model = std::make_unique<Model>(filename);
//}

//void Actor::LoadModelSabe(const char* filename)
//{
//	modelsabe = std::make_unique<Model>(filename);
//}

// ì¬
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

// íœ
void ActorManager::Remove(std::shared_ptr<Actor> actor)
{
	removeActors.insert(actor);
}

// XV
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

// s—ñXV
//void ActorManager::UpdateTransform()
//{
//	for (std::shared_ptr<Actor>& actor : updateActors)
//	{
//		
//		//actor->UpdateTransform();
//
//	}
//}

// •`‰æ
void ActorManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	Graphics& graphics = Graphics::Instance();
	//Shader* shader = graphics.GetShader();
	//ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	// •`‰æˆ— 
	RenderContext rc;// •`‰æ‚·‚é‚½‚ß‚É•K—v‚Èî•ñ‚ğ‚Ü‚Æ‚ß‚½\‘¢‘Ì

	rc.view = view;
	rc.projection = projection;

	// ƒ‰ƒCƒg‚Ì•ûŒü
	DirectX::XMFLOAT3 lightDirection = DirectX::XMFLOAT3(0.2f, -0.8f, 0.0f);
	
	rc.lightDirection = { lightDirection.x,lightDirection.y,lightDirection.z ,0};
	// ƒ‚ƒfƒ‹‚»‚ê‚¼‚ê‚ÅƒVƒF[ƒ_[‚ğ‚·‚é‚½‚ß‚É
	rc.deviceContext = dc;
	// •`‰æ
	//shader->Begin(dc, view, projection, lightDirection);

	//for (std::shared_ptr<Actor>& actor : updateActors)
	//{
	//	// ƒ‚ƒfƒ‹‚ª‚ ‚ê‚Î•`‰æ
	//	Model* model = actor->GetModel();
	//	if (model != nullptr)
	//	{
	//		shader->Draw(dc, actor->GetModel());
	//	}
	//}

	//shader->End(dc);
	

    for (std::shared_ptr<Actor>& actor : updateActors)
	{
		// ƒ‚ƒfƒ‹‚ª‚ ‚ê‚Î•`‰æ
		actor->Render(rc);
	}

	//shader->Begin(rc);// ƒVƒF[ƒ_[‚ÉƒJƒƒ‰‚Ìî•ñ‚ğ“n‚·

	//for (std::shared_ptr<Actor>& actor : updateActors)
	//{
	//	// ƒ‚ƒfƒ‹‚ª‚ ‚ê‚Î•`‰æ
	//	Model* model = actor->GetModel();
	//	if (model != nullptr)
	//	{
	//		shader->Draw(rc, model);
	//	}
	//}

	//shader->End(rc);



	// ƒŠƒXƒ^[•`‰æ
	DrawLister();

	// Ú×•`‰æ
	DrawDetail();

}

<<<<<<< HEAD
<<<<<<< HEAD

void ActorManager::RenderShadowmap(RenderContext rc)
=======
void ActorManager::RenderShadowmap(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
>>>>>>> parent of 0db33c3 (ä¸€åº¦ä¿å­˜ãƒ¯ãƒ¼ãƒ‹ãƒ³ã‚°å¤§é‡)
{
	// •`‰æˆ— 
	Graphics& graphics = Graphics::Instance();
	RenderContext rc;// •`‰æ‚·‚é‚½‚ß‚É•K—v‚Èî•ñ‚ğ‚Ü‚Æ‚ß‚½\‘¢‘Ì
	// •`‰æ—p
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();

	rc.deviceContext = dc;

	rc.view = view;
	rc.projection = projection;

	for (std::shared_ptr<Actor>& actor : updateActors)
	{
		// ƒ‚ƒfƒ‹‚ª‚ ‚ê‚Î•`‰æ
		actor->RenderShadwomap(rc);
	}
}


=======
>>>>>>> parent of 8a0ff20 (ã¨ã‚Šã‚ãˆãšã‚·ã‚§ãƒ¼ãƒ€ãƒ¼ç¶ºéº—ãªå¥´å…¥ã£ãŸ)
// ƒŠƒXƒ^[•`‰æ
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
				// ’Pˆê‘I‘ğ‚¾‚¯‘Î‰‚µ‚Ä‚¨‚­
				ImGuiIO& io = ImGui::GetIO();
				selectionActors.clear();
				selectionActors.insert(actor);
			}

			ImGui::TreePop();
		}
	}
	ImGui::End();
}

// Ú×•`‰æ
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
//
//// ‚’¼‘¬“x
//void Actor::UpdateVerticalVelocity(float elapsedFrame)
//{
//    // d—Íˆ—
//    this->velocity.y += this->grabity * elapsedFrame;
//
//}
//// ‚’¼ˆÚ“®
//void Actor::UpdateVerticalMove(float elapsedTime)
//{
//
//    // ‚’¼•ûŒü‚ÌˆÚ“®—Ê
//    float my = velocity.y * elapsedTime;
//
//    slopeRate = 0.0f; // ŒXÎ—¦
//
//    // ƒLƒƒƒ‰ƒNƒ^[‚ÌY²•ûŒü‚Æ‚È‚é–@üƒxƒNƒgƒ‹
//    DirectX::XMFLOAT3 normal = { 0,1,0 };
//
//    // —‰º’†
//    if (my < 0.0f)
//    {
//        // ƒŒƒC‚ÌŠJnˆÊ’u‚Í‘«Œ³‚æ‚è­‚µã
//        DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
//        // ƒŒƒC‚ÌI“_ˆÊ’u‚ÍˆÚ“®Œã‚ÌˆÊ’u
//        DirectX::XMFLOAT3 end = { position.x, position.y + my , position.z };
//
//        // ƒŒƒCƒLƒƒƒXƒg‚É‚æ‚é’n–Ê”»’è
//        HitResult hit;
//        // ƒŒƒCƒLƒƒƒXƒg‚ğŒÄ‚Ô‚½‚ß‚ÌŠÖ”
//        //if (StageManager::instance().RayCast(start, end, hit))
//        if (StageManager::instance().RayCast(start, end, hit))
//        {
//
//            // –@üƒxƒNƒgƒ‹æ“¾
//            normal = hit.normal;
//
//
//            // ’n–Ê‚Éİ’u‚µ‚Ä‚¢‚é
//            position = hit.position;
//
//            // ‰ñ“]
//            // ‚™‚¾‚¯‚È——R‚ÍŠp“x‚ª^‰¡‚É‚È‚Á‚½‚ÉƒNƒ‚‚İ‚½‚¢‚É‚Ğ‚Á‚Â‚©‚È‚¢‚æ‚¤‚É
//            angle.y += hit.rotation.y;
//
//            // ŒXÎ—¦‚ÌŒvZ –@ü‚ÆXZ•½–Ê‚É’u‚¯‚éƒxƒNƒgƒ‹‚Æ‚™²‚ÌƒxƒNƒgƒ‹‚ÅŒX‚«‚ª‹‚ß‚ç‚ê‚éB
//            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
//            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));
//            // ’…’n
//            if (!isGround)
//            {
//
//                OnLanding();
//            }
//            isGround = true;
//            velocity.y = 0.0f;
//
//        }
//        else
//        {
//            // ‹ó’†‚É•‚‚¢‚Ä‚¢‚é
//            position.y += my;
//            isGround = false;
//        }
//
//    }
//    // ã¸’†
//    else if (my > 0.0f)
//    {
//        position.y += my;
//        isGround = false;
//    }
//
//    // ’n–Ê‚ÌŒü‚«‚É‰ˆ‚¤‚æ‚¤‚ÉXZ²‰ñ“]
//    {
//        // Y²‚ª–@üƒxƒNƒgƒ‹•ûŒü‚ÉŒü‚­ƒIƒCƒ‰[Šp‰ñ“]‚ğZo‚·‚é
//        float storageAngleX = atan2f(normal.z, normal.y);
//        float storageAngleZ = -atan2f(normal.x, normal.y);
//
//        // üŒ`•âŠ®‚ÅŠŠ‚ç‚©‚É‰ñ“]‚·‚é
//        // (•Ï‰»‚·‚é’l,ÅI“I‚È’l,‚±‚ê‚¾‚¯i)
//        angle.x = Mathf::Lerp(angle.x, storageAngleX, 0.2f);
//        angle.z = Mathf::Lerp(angle.z, storageAngleZ, 0.2f);
//
//    }
//}
//
//void Actor::UpdateVelocity(float elapsedTime)
//{
//    // Œo‰ßƒtƒŒ[ƒ€
//    float elapsedFrame = 60.0f * elapsedTime;
//
//    // ‚’¼‘¬—ÍXVˆ—
//    UpdateVerticalVelocity(elapsedFrame);
//
//    // …•½‘¬—ÍXVˆ—
//    UpdateHorizontalVelocity(elapsedFrame);
//    // ‚’¼ˆÚ“®XVˆ—
//    UpdateVerticalMove(elapsedTime);
//
//    // …•½ˆÚ“®XVˆ—
//    UpdateHorizontalMove(elapsedTime);
//}
