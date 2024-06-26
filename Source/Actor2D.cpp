//#include "Actor2D.h"
//#include <imgui.h>
//#include "Graphics\Graphics.h"
//#include "Component.h"
//
//
//// 開始処理
//void Actor2D::Start()
//{
//	for (std::shared_ptr<Component>& component : components)
//	{
//		component->Start();
//	}
//}
//
//// 更新
//void Actor2D::Update(float elapsedTime)
//{
//	// アニメーションの更新
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
//// 行列の更新
//void Actor2D::UpdateTransform()
//{
//
//
//	// スケールだけ行列を作成
//	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
//
//	// 回転行列作成
//	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(rotation.x);
//	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(rotation.y);
//	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(rotation.z);
//	DirectX::XMMATRIX R = Y * X * Z;
//
//
//	// 位置行列だけを作成
//	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
//	// 3つの行列を組み合わせ、ワールド行列を作成
//	DirectX::XMMATRIX W = S * R * T;// 行列は計算順番変えると結果が変わる
//	// 計算したワールド行列を取り出す
//	DirectX::XMStoreFloat4x4(&transform, W);
//
//	// モデルの行列更新
//	if (model != nullptr)
//	{
//		model->UpdateTransform(transform);
//	}
//
//
//
//	//// ワールド行列の更新
//	//DirectX::XMVECTOR Q = DirectX::XMLoadFloat4(&rotation);
//	//DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
//	//DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(Q);
//	//DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
//
//	//DirectX::XMMATRIX W = S * R * T;
//	//DirectX::XMStoreFloat4x4(&transform, W);
//
//	//// モデルの行列更新
//	//if (model != nullptr)
//	//{
//	//	model->UpdateTransform(transform);
//	//}
//}
//
//// GUI表示
//void Actor2D::OnGUI()
//{
//	// 名前
//	{
//		char buffer[1024];
//		::strncpy_s(buffer, sizeof(buffer), GetName(), sizeof(buffer));
//		if (ImGui::InputText("Name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
//		{
//			SetName(buffer);
//		}
//	}
//
//	// トランスフォーム
//	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
//	{
//		ImGui::InputFloat3("Position", &position.x);
//	}
//
//	// コンポーネント
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
//// モデルの読み込み
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
//// 作成
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
//// 削除
//void Actor2DManager::Remove(std::shared_ptr<Actor2D> actor)
//{
//	removeActors.insert(actor);
//}
//
//// 更新
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
//// 行列更新
//void Actor2DManager::UpdateTransform()
//{
//	for (std::shared_ptr<Actor>& actor : updateActors)
//	{
//		actor->UpdateTransform();
//
//	}
//}
//
//// 描画
//void Actor2DManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
//{
//	Graphics& graphics = Graphics::Instance();
//	//Shader* shader = graphics.GetShader();
//	ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
//	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
//	// 描画処理 
//	RenderContext rc;// 描画するために必要な情報をまとめた構造体
//
//	rc.view = view;
//	rc.projection = projection;
//
//	// ライトの方向
//	DirectX::XMFLOAT3 lightDirection = DirectX::XMFLOAT3(0.2f, -0.8f, 0.0f);
//
//	rc.lightDirection = { lightDirection.x,lightDirection.y,lightDirection.z ,0 };
//	// モデルそれぞれでシェーダーをするために
//	rc.deviceContext = dc;
//	// 描画
//	//shader->Begin(dc, view, projection, lightDirection);
//
//	//for (std::shared_ptr<Actor>& actor : updateActors)
//	//{
//	//	// モデルがあれば描画
//	//	Model* model = actor->GetModel();
//	//	if (model != nullptr)
//	//	{
//	//		shader->Draw(dc, actor->GetModel());
//	//	}
//	//}
//
//	//shader->End(dc);
//
//	shader->Begin(rc);// シェーダーにカメラの情報を渡す
//
//	for (std::shared_ptr<Actor>& actor : updateActors)
//	{
//		// モデルがあれば描画
//		Model* model = actor->GetModel();
//		if (model != nullptr)
//		{
//			shader->Draw(rc, model);
//		}
//	}
//
//	shader->End(rc);
//
//	// リスター描画
//	DrawLister();
//
//	// 詳細描画
//	DrawDetail();
//
//}
//
//// リスター描画
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
//				// 単一選択だけ対応しておく
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
//// 詳細描画
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
