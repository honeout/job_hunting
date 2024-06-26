#include "Actor.h"
#include <imgui.h>
#include "Graphics\Graphics.h"
#include "Component.h"
#include "Actor.h"

// 開始処理
void Actor::Start()
{
	for (std::shared_ptr<Component>& component : components)
	{
		component->Start();
	}
}

// 更新
void Actor::Update(float elapsedTime)
{
	// アニメーションの更新
	if (model != nullptr)
	{
		model->UpdateAnimation(elapsedTime,true);
	}

	for (std::shared_ptr<Component>& component : components)
	{
		component->Update(elapsedTime);
	}
}

// 行列の更新
void Actor::UpdateTransform()
{


	// スケールだけ行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	// 回転行列作成
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(rotation.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(rotation.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(rotation.z);
	DirectX::XMMATRIX R = Y * X * Z;


	// 位置行列だけを作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// 3つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;// 行列は計算順番変えると結果が変わる
	// 計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);

	// モデルの行列更新
	if (model != nullptr)
	{
		model->UpdateTransform(transform);
	}

	

	//// ワールド行列の更新
	//DirectX::XMVECTOR Q = DirectX::XMLoadFloat4(&rotation);
	//DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(Q);
	//DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	//DirectX::XMMATRIX W = S * R * T;
	//DirectX::XMStoreFloat4x4(&transform, W);

	//// モデルの行列更新
	//if (model != nullptr)
	//{
	//	model->UpdateTransform(transform);
	//}
}

void Actor::UpdateTransformProjectile()
{

	// 何か
	DirectX::XMVECTOR Front, Up, Right;
	// 前ベクトルを算出　まず向きを手に入れる
	Front = DirectX::XMLoadFloat3(&direction);
	Front = DirectX::XMVector3Normalize(Front);
	// 仮の上ベクトルを算出　少しおかしい値を入れておく真上に打った時に
	// ここと同じ値だとバグるから
	Up = DirectX::XMVectorSet(0.001f, 1, 0, 0);
	Up = DirectX::XMVector3Normalize(Up);

	// 右ベクトル算出　上二つを使い一つの新しい軸を作る
	Right = DirectX::XMVector3Cross(Up, Front);// ２つで１つの奴を出す

	Right = DirectX::XMVector3Normalize(Right);
	// 上ベクトル算出 さらに作った二つでさらに新しいひとつを作る。
	Up = DirectX::XMVector3Cross(Front, Right);// 斜め上の奴出来る



	// 計算結果を取り出し
	DirectX::XMFLOAT3 right, up, front;
	DirectX::XMStoreFloat3(&right, Right);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&front, Front);

	// 算出した軸ベクトルから行列を作成 回転あり
	transform._11 = right.x * scale.x; // 大きさだけやった奴０．１倍ならそうなる
	transform._12 = right.y * scale.x;// 角度
	transform._13 = right.z * scale.x;
	transform._14 = 0.0f;
	transform._21 = up.x * scale.y;
	transform._22 = up.y * scale.y;// 裏返る
	transform._23 = up.z * scale.y;
	transform._24 = 0.0f;
	transform._31 = front.x * scale.z;
	transform._32 = front.y * scale.z;
	transform._33 = front.z * scale.z;// 向き
	transform._34 = 0.0f;
	transform._41 = position.x;
	transform._42 = position.y;
	transform._43 = position.z;
	transform._44 = 1.0f;

	this->direction = front;// 向きをこっちに入れるこれでむきの計算
	// を外に

		// モデルの行列更新
	if (model != nullptr)
	{
		model->UpdateTransform(transform);
	}
}

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

	// トランスフォーム
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::InputFloat3("Position", &position.x);
	}

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

// モデルの読み込み
void Actor::LoadModel(const char* filename)
{
	model = std::make_unique<Model>(filename);
}

void Actor::LoadModelSabe(const char* filename)
{
	modelsabe = std::make_unique<Model>(filename);
}

// 作成
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

// 削除
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

// 行列更新
void ActorManager::UpdateTransform()
{
	for (std::shared_ptr<Actor>& actor : updateActors)
	{
		
		//actor->UpdateTransform();

	}
}

// 描画
void ActorManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	Graphics& graphics = Graphics::Instance();
	//Shader* shader = graphics.GetShader();
	ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	// 描画処理 
	RenderContext rc;// 描画するために必要な情報をまとめた構造体

	rc.view = view;
	rc.projection = projection;

	// ライトの方向
	DirectX::XMFLOAT3 lightDirection = DirectX::XMFLOAT3(0.2f, -0.8f, 0.0f);
	
	rc.lightDirection = { lightDirection.x,lightDirection.y,lightDirection.z ,0};
	// モデルそれぞれでシェーダーをするために
	rc.deviceContext = dc;
	// 描画
	//shader->Begin(dc, view, projection, lightDirection);

	//for (std::shared_ptr<Actor>& actor : updateActors)
	//{
	//	// モデルがあれば描画
	//	Model* model = actor->GetModel();
	//	if (model != nullptr)
	//	{
	//		shader->Draw(dc, actor->GetModel());
	//	}
	//}

	//shader->End(dc);

	shader->Begin(rc);// シェーダーにカメラの情報を渡す

	for (std::shared_ptr<Actor>& actor : updateActors)
	{
		// モデルがあれば描画
		Model* model = actor->GetModel();
		if (model != nullptr)
		{
			shader->Draw(rc, model);
		}
	}

	shader->End(rc);

	// リスター描画
	DrawLister();

	// 詳細描画
	DrawDetail();

}

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
//
//// 垂直速度
//void Actor::UpdateVerticalVelocity(float elapsedFrame)
//{
//    // 重力処理
//    this->velocity.y += this->grabity * elapsedFrame;
//
//}
//// 垂直移動
//void Actor::UpdateVerticalMove(float elapsedTime)
//{
//
//    // 垂直方向の移動量
//    float my = velocity.y * elapsedTime;
//
//    slopeRate = 0.0f; // 傾斜率
//
//    // キャラクターのY軸方向となる法線ベクトル
//    DirectX::XMFLOAT3 normal = { 0,1,0 };
//
//    // 落下中
//    if (my < 0.0f)
//    {
//        // レイの開始位置は足元より少し上
//        DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
//        // レイの終点位置は移動後の位置
//        DirectX::XMFLOAT3 end = { position.x, position.y + my , position.z };
//
//        // レイキャストによる地面判定
//        HitResult hit;
//        // レイキャストを呼ぶための関数
//        //if (StageManager::instance().RayCast(start, end, hit))
//        if (StageManager::instance().RayCast(start, end, hit))
//        {
//
//            // 法線ベクトル取得
//            normal = hit.normal;
//
//
//            // 地面に設置している
//            position = hit.position;
//
//            // 回転
//            // ｙだけな理由は角度が真横になった時にクモみたいにひっつかないように
//            angle.y += hit.rotation.y;
//
//            // 傾斜率の計算 法線とXZ平面に置けるベクトルとｙ軸のベクトルで傾きが求められる。
//            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
//            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));
//            // 着地
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
//            // 空中に浮いている
//            position.y += my;
//            isGround = false;
//        }
//
//    }
//    // 上昇中
//    else if (my > 0.0f)
//    {
//        position.y += my;
//        isGround = false;
//    }
//
//    // 地面の向きに沿うようにXZ軸回転
//    {
//        // Y軸が法線ベクトル方向に向くオイラー角回転を算出する
//        float storageAngleX = atan2f(normal.z, normal.y);
//        float storageAngleZ = -atan2f(normal.x, normal.y);
//
//        // 線形補完で滑らかに回転する
//        // (変化する値,最終的な値,これだけ進)
//        angle.x = Mathf::Lerp(angle.x, storageAngleX, 0.2f);
//        angle.z = Mathf::Lerp(angle.z, storageAngleZ, 0.2f);
//
//    }
//}
//
//void Actor::UpdateVelocity(float elapsedTime)
//{
//    // 経過フレーム
//    float elapsedFrame = 60.0f * elapsedTime;
//
//    // 垂直速力更新処理
//    UpdateVerticalVelocity(elapsedFrame);
//
//    // 水平速力更新処理
//    UpdateHorizontalVelocity(elapsedFrame);
//    // 垂直移動更新処理
//    UpdateVerticalMove(elapsedTime);
//
//    // 水平移動更新処理
//    UpdateHorizontalMove(elapsedTime);
//}
