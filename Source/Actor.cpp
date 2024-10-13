#include "Actor.h"
#include <imgui.h>
#include "Graphics\Graphics.h"
#include "Component.h"


// �J�n����
void Actor::Start()
{
	for (std::shared_ptr<Component>& component : components)
	{
		component->Start();
	}
}

// �X�V
void Actor::Update(float elapsedTime)
{
	//// �A�j���[�V�����̍X�V
	//if (model != nullptr)
	//{
	//	model->UpdateAnimation(elapsedTime,true);
	//}

	for (std::shared_ptr<Component>& component : components)
	{
		component->Update(elapsedTime);
	}
}

void Actor::Render(RenderContext rc, ModelShader* shader)
{
	for (std::shared_ptr<Component>& component : components)
	{


		component->Render(rc, *shader);
	}
}

void Actor::Render2D(RenderContext rc, SpriteShader* shader)
{
	for (std::shared_ptr<Component>& component : components)
	{


		component->Render2D(rc, *shader);
	}
}

void Actor::RenderShadwomap(RenderContext rc)
{
	for (std::shared_ptr<Component>& component : components)
	{
		component->RenderShadowmap(rc);
	}
}


// GUI�\��
void Actor::OnGUI()
{
	// ���O
	{
		char buffer[1024];
		::strncpy_s(buffer, sizeof(buffer), GetName(), sizeof(buffer));
		if (ImGui::InputText("Name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			SetName(buffer);
		}
	}

	//// �g�����X�t�H�[��
	//if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	//{
	//	ImGui::InputFloat3("Position", &position.x);
	//}

	// �R���|�[�l���g
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

// ���f���̓ǂݍ���
//void Actor::LoadModel(const char* filename)
//{
//	model = std::make_unique<Model>(filename);
//}

//void Actor::LoadModelSabe(const char* filename)
//{
//	modelsabe = std::make_unique<Model>(filename);
//}

// �쐬
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

// �폜
void ActorManager::Remove(std::shared_ptr<Actor> actor)
{
	removeActors.insert(actor);
}

// �X�V
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

// �s��X�V
//void ActorManager::UpdateTransform()
//{
//	for (std::shared_ptr<Actor>& actor : updateActors)
//	{
//		
//		//actor->UpdateTransform();
//
//	}
//}

// �`��
void ActorManager::Render(RenderContext rc, ModelShader* shader)
{
	Graphics& graphics = Graphics::Instance();
	//Shader* shader = graphics.GetShader();
	//ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
	//ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	// �`�揈�� 
	//RenderContext rc;// �`�悷�邽�߂ɕK�v�ȏ����܂Ƃ߂��\����

	//rc.view = view;
	//rc.projection = projection;

	//// ���C�g�̕���
	//DirectX::XMFLOAT3 lightDirection = DirectX::XMFLOAT3(0.2f, -0.8f, 0.0f);
	//
	//rc.lightDirection = { lightDirection.x,lightDirection.y,lightDirection.z ,0};
	//// ���f�����ꂼ��ŃV�F�[�_�[�����邽�߂�
	//rc.deviceContext = dc;
	// �`��
	//shader->Begin(dc, view, projection, lightDirection);

	//for (std::shared_ptr<Actor>& actor : updateActors)
	//{
	//	// ���f��������Ε`��
	//	Model* model = actor->GetModel();
	//	if (model != nullptr)
	//	{
	//		shader->Draw(dc, actor->GetModel());
	//	}
	//}

	//shader->End(dc);
	
	// 3D
    for (std::shared_ptr<Actor>& actor : updateActors)
	{
		if (!actor->GetCheck2d())
		// ���f��������Ε`��
		actor->Render(rc, shader);


	}



	//shader->Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��

	//for (std::shared_ptr<Actor>& actor : updateActors)
	//{
	//	// ���f��������Ε`��
	//	Model* model = actor->GetModel();
	//	if (model != nullptr)
	//	{
	//		shader->Draw(rc, model);
	//	}
	//}

	//shader->End(rc);



	// ���X�^�[�`��
	DrawLister();

	// �ڍו`��
	DrawDetail();

}

void ActorManager::Render2D(RenderContext rc, SpriteShader* shader)
{
	Graphics& graphics = Graphics::Instance();


	for (std::shared_ptr<Actor>& actor : updateActors)
	{
		if (actor->GetCheck2d())
		// ���f��������Ε`��
		actor->Render2D(rc, shader);
	}
}

void ActorManager::RenderShadowmap(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	// �`�揈�� 
	Graphics& graphics = Graphics::Instance();
	RenderContext rc;// �`�悷�邽�߂ɕK�v�ȏ����܂Ƃ߂��\����
	// �`��p
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();

	rc.deviceContext = dc;

	rc.view = view;
	rc.projection = projection;

	for (std::shared_ptr<Actor>& actor : updateActors)
	{
		// ���f��������Ε`��
		actor->RenderShadwomap(rc);
	}
}

void ActorManager::Clear()
{
	
	for (std::shared_ptr<Actor>& actor : updateActors)// 
	{
		// ���̂��������Ǘ����Ă��鐔�͂��̂܂�
	   // delete projectile;
		//Remove(actor);
		//actor.reset();
		actor.reset();
		
	}
	updateActors.clear();
    
	
}

// ���X�^�[�`��
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
				// �P��I�������Ή����Ă���
				ImGuiIO& io = ImGui::GetIO();
				selectionActors.clear();
				selectionActors.insert(actor);
			}

			ImGui::TreePop();
		}
	}
	ImGui::End();
}

// �ڍו`��
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
//// �������x
//void Actor::UpdateVerticalVelocity(float elapsedFrame)
//{
//    // �d�͏���
//    this->velocity.y += this->grabity * elapsedFrame;
//
//}
//// �����ړ�
//void Actor::UpdateVerticalMove(float elapsedTime)
//{
//
//    // ���������̈ړ���
//    float my = velocity.y * elapsedTime;
//
//    slopeRate = 0.0f; // �X�Η�
//
//    // �L�����N�^�[��Y�������ƂȂ�@���x�N�g��
//    DirectX::XMFLOAT3 normal = { 0,1,0 };
//
//    // ������
//    if (my < 0.0f)
//    {
//        // ���C�̊J�n�ʒu�͑�����菭����
//        DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
//        // ���C�̏I�_�ʒu�͈ړ���̈ʒu
//        DirectX::XMFLOAT3 end = { position.x, position.y + my , position.z };
//
//        // ���C�L���X�g�ɂ��n�ʔ���
//        HitResult hit;
//        // ���C�L���X�g���ĂԂ��߂̊֐�
//        //if (StageManager::instance().RayCast(start, end, hit))
//        if (StageManager::instance().RayCast(start, end, hit))
//        {
//
//            // �@���x�N�g���擾
//            normal = hit.normal;
//
//
//            // �n�ʂɐݒu���Ă���
//            position = hit.position;
//
//            // ��]
//            // �������ȗ��R�͊p�x���^���ɂȂ������ɃN���݂����ɂЂ����Ȃ��悤��
//            angle.y += hit.rotation.y;
//
//            // �X�Η��̌v�Z �@����XZ���ʂɒu����x�N�g���Ƃ����̃x�N�g���ŌX�������߂���B
//            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
//            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));
//            // ���n
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
//            // �󒆂ɕ����Ă���
//            position.y += my;
//            isGround = false;
//        }
//
//    }
//    // �㏸��
//    else if (my > 0.0f)
//    {
//        position.y += my;
//        isGround = false;
//    }
//
//    // �n�ʂ̌����ɉ����悤��XZ����]
//    {
//        // Y�����@���x�N�g�������Ɍ����I�C���[�p��]���Z�o����
//        float storageAngleX = atan2f(normal.z, normal.y);
//        float storageAngleZ = -atan2f(normal.x, normal.y);
//
//        // ���`�⊮�Ŋ��炩�ɉ�]����
//        // (�ω�����l,�ŏI�I�Ȓl,���ꂾ���i)
//        angle.x = Mathf::Lerp(angle.x, storageAngleX, 0.2f);
//        angle.z = Mathf::Lerp(angle.z, storageAngleZ, 0.2f);
//
//    }
//}
//
//void Actor::UpdateVelocity(float elapsedTime)
//{
//    // �o�߃t���[��
//    float elapsedFrame = 60.0f * elapsedTime;
//
//    // �������͍X�V����
//    UpdateVerticalVelocity(elapsedFrame);
//
//    // �������͍X�V����
//    UpdateHorizontalVelocity(elapsedFrame);
//    // �����ړ��X�V����
//    UpdateVerticalMove(elapsedTime);
//
//    // �����ړ��X�V����
//    UpdateHorizontalMove(elapsedTime);
//}
