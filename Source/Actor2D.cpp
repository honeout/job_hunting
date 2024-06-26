//#include "Actor2D.h"
//#include <imgui.h>
//#include "Graphics\Graphics.h"
//#include "Component.h"
//
//
//// �J�n����
//void Actor2D::Start()
//{
//	for (std::shared_ptr<Component>& component : components)
//	{
//		component->Start();
//	}
//}
//
//// �X�V
//void Actor2D::Update(float elapsedTime)
//{
//	// �A�j���[�V�����̍X�V
//	if (model != nullptr)
//	{
//		model->UpdateAnimation(elapsedTime, true);
//	}
//
//	for (std::shared_ptr<Component>& component : components)
//	{
//		component->Update(elapsedTime);
//	}
//}
//
//// �s��̍X�V
//void Actor2D::UpdateTransform()
//{
//
//
//	// �X�P�[�������s����쐬
//	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
//
//	// ��]�s��쐬
//	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(rotation.x);
//	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(rotation.y);
//	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(rotation.z);
//	DirectX::XMMATRIX R = Y * X * Z;
//
//
//	// �ʒu�s�񂾂����쐬
//	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
//	// 3�̍s���g�ݍ��킹�A���[���h�s����쐬
//	DirectX::XMMATRIX W = S * R * T;// �s��͌v�Z���ԕς���ƌ��ʂ��ς��
//	// �v�Z�������[���h�s������o��
//	DirectX::XMStoreFloat4x4(&transform, W);
//
//	// ���f���̍s��X�V
//	if (model != nullptr)
//	{
//		model->UpdateTransform(transform);
//	}
//
//
//
//	//// ���[���h�s��̍X�V
//	//DirectX::XMVECTOR Q = DirectX::XMLoadFloat4(&rotation);
//	//DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
//	//DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(Q);
//	//DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
//
//	//DirectX::XMMATRIX W = S * R * T;
//	//DirectX::XMStoreFloat4x4(&transform, W);
//
//	//// ���f���̍s��X�V
//	//if (model != nullptr)
//	//{
//	//	model->UpdateTransform(transform);
//	//}
//}
//
//// GUI�\��
//void Actor2D::OnGUI()
//{
//	// ���O
//	{
//		char buffer[1024];
//		::strncpy_s(buffer, sizeof(buffer), GetName(), sizeof(buffer));
//		if (ImGui::InputText("Name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
//		{
//			SetName(buffer);
//		}
//	}
//
//	// �g�����X�t�H�[��
//	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
//	{
//		ImGui::InputFloat3("Position", &position.x);
//	}
//
//	// �R���|�[�l���g
//	for (std::shared_ptr<Component>& component : components)
//	{
//		ImGui::Spacing();
//		ImGui::Separator();
//
//		if (ImGui::CollapsingHeader(component->GetName(), ImGuiTreeNodeFlags_DefaultOpen))
//		{
//			//ImGui::PushID(StringToHash(component->GetName()));
//			component->OnGUI();
//			//ImGui::PopID();
//		}
//	}
//}
//
//// ���f���̓ǂݍ���
//void Actor2D::LoadModel(const char* filename)
//{
//	model = std::make_unique<Model>(filename);
//}
//
//void Actor2D::LoadModelSabe(const char* filename)
//{
//	modelsabe = std::make_unique<Model>(filename);
//}
//
//// �쐬
//std::shared_ptr<Actor2D> Actor2DManager::Create()
//{
//	std::shared_ptr<Actor2D> actor = std::make_shared<Actor2D>();
//	{
//		static int id = 0;
//		char name[256];
//		::sprintf_s(name, sizeof(name), "Actor%d", id++);
//		actor->SetName(name);
//	}
//	startActors.emplace_back(actor);
//	return actor;
//}
//
//// �폜
//void Actor2DManager::Remove(std::shared_ptr<Actor2D> actor)
//{
//	removeActors.insert(actor);
//}
//
//// �X�V
//void Actor2DManager::Update(float elapsedTime)
//{
//	for (std::shared_ptr<Actor>& actor : startActors)
//	{
//		actor->Start();
//		updateActors.emplace_back(actor);
//	}
//	startActors.clear();
//
//	for (std::shared_ptr<Actor>& actor : updateActors)
//	{
//		actor->Update(elapsedTime);
//	}
//
//	for (const std::shared_ptr<Actor>& actor : removeActors)
//	{
//		std::vector<std::shared_ptr<Actor>>::iterator itStart = std::find(startActors.begin(), startActors.end(), actor);
//		if (itStart != startActors.end())
//		{
//			startActors.erase(itStart);
//		}
//
//		std::vector<std::shared_ptr<Actor>>::iterator itUpdate = std::find(updateActors.begin(), updateActors.end(), actor);
//		if (itUpdate != updateActors.end())
//		{
//			updateActors.erase(itUpdate);
//		}
//
//		std::set<std::shared_ptr<Actor>>::iterator itSelection = selectionActors.find(actor);
//		if (itSelection != selectionActors.end())
//		{
//			selectionActors.erase(itSelection);
//		}
//	}
//	removeActors.clear();
//}
//
//// �s��X�V
//void Actor2DManager::UpdateTransform()
//{
//	for (std::shared_ptr<Actor>& actor : updateActors)
//	{
//		actor->UpdateTransform();
//
//	}
//}
//
//// �`��
//void Actor2DManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
//{
//	Graphics& graphics = Graphics::Instance();
//	//Shader* shader = graphics.GetShader();
//	ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
//	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
//	// �`�揈�� 
//	RenderContext rc;// �`�悷�邽�߂ɕK�v�ȏ����܂Ƃ߂��\����
//
//	rc.view = view;
//	rc.projection = projection;
//
//	// ���C�g�̕���
//	DirectX::XMFLOAT3 lightDirection = DirectX::XMFLOAT3(0.2f, -0.8f, 0.0f);
//
//	rc.lightDirection = { lightDirection.x,lightDirection.y,lightDirection.z ,0 };
//	// ���f�����ꂼ��ŃV�F�[�_�[�����邽�߂�
//	rc.deviceContext = dc;
//	// �`��
//	//shader->Begin(dc, view, projection, lightDirection);
//
//	//for (std::shared_ptr<Actor>& actor : updateActors)
//	//{
//	//	// ���f��������Ε`��
//	//	Model* model = actor->GetModel();
//	//	if (model != nullptr)
//	//	{
//	//		shader->Draw(dc, actor->GetModel());
//	//	}
//	//}
//
//	//shader->End(dc);
//
//	shader->Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��
//
//	for (std::shared_ptr<Actor>& actor : updateActors)
//	{
//		// ���f��������Ε`��
//		Model* model = actor->GetModel();
//		if (model != nullptr)
//		{
//			shader->Draw(rc, model);
//		}
//	}
//
//	shader->End(rc);
//
//	// ���X�^�[�`��
//	DrawLister();
//
//	// �ڍו`��
//	DrawDetail();
//
//}
//
//// ���X�^�[�`��
//void Actor2DManager::DrawLister()
//{
//	ImGui::SetNextWindowPos(ImVec2(30, 50), ImGuiCond_FirstUseEver);
//	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
//
//	hiddenLister = !ImGui::Begin("Actor Lister", nullptr, ImGuiWindowFlags_None);
//	if (!hiddenLister)
//	{
//		for (std::shared_ptr<Actor>& actor : updateActors)
//		{
//			ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_Leaf;
//
//			if (selectionActors.find(actor) != selectionActors.end())
//			{
//				nodeFlags |= ImGuiTreeNodeFlags_Selected;
//			}
//
//			ImGui::TreeNodeEx(actor.get(), nodeFlags, actor->GetName());
//
//			if (ImGui::IsItemClicked())
//			{
//				// �P��I�������Ή����Ă���
//				ImGuiIO& io = ImGui::GetIO();
//				selectionActors.clear();
//				selectionActors.insert(actor);
//			}
//
//			ImGui::TreePop();
//		}
//	}
//	ImGui::End();
//}
//
//// �ڍו`��
//void Actor2DManager::DrawDetail()
//{
//	ImGui::SetNextWindowPos(ImVec2(950, 50), ImGuiCond_FirstUseEver);
//	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
//
//	hiddenDetail = !ImGui::Begin("Actor Detail", nullptr, ImGuiWindowFlags_None);
//	if (!hiddenDetail)
//	{
//		std::shared_ptr<Actor> lastSelected = selectionActors.empty() ? nullptr : *selectionActors.rbegin();
//		if (lastSelected != nullptr)
//		{
//			lastSelected->OnGUI();
//		}
//	}
//	ImGui::End();
//}
