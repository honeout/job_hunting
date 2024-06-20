#pragma once

#include <memory>
#include <vector>
#include <set>
#include <string>
#include <DirectXMath.h>
#include "Graphics\Model.h"

// 前方宣言
class Component;

// アクター
class Actor : public std::enable_shared_from_this<Actor>
{
public:
	Actor() {}
	virtual ~Actor() {}

	// 開始処理
	virtual void Start();

	// 更新処理
	virtual void Update(float elapsedTime);

	// 行列の更新
	virtual void UpdateTransform();

	// GUI表示
	virtual void OnGUI();

	// 名前の設定
	void SetName(const char* name) { this->name = name; }

	// 名前の取得
	const char* GetName() const { return name.c_str(); }

	// 位置の設定
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	// 位置の取得
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// 回転の設定
	void SetRotation(const DirectX::XMFLOAT4& rotation) { this->rotation = rotation; }
	void SetRotation3(const DirectX::XMFLOAT3& rotation) { 
		this->rotation.x = rotation.x;
		this->rotation.y = rotation.y;
		this->rotation.z = rotation.z;
	}

	// 回転の取得
	const DirectX::XMFLOAT4& GetRotation() const { return rotation; }

	// スケールの設定
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	// スケールの取得
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	// 行列の取得
	const DirectX::XMFLOAT4X4& GetTransform() const { return transform; }

	// モデルの読み込み
	void LoadModel(const char* filename);

	void LoadModelSabe(const char* filename);

	// モデルの取得
	Model* GetModel() const { return model.get(); }
	Model* GetModelSabe() const { return modelsabe.get(); }
	


	// アニメーションの時間
	float GetCurrentANimationSeconds() const { return currentANimationSeconds; }
	// アニメーションの時間
	void SetCurrentAnimationSeconds(const float currentANimationSeconds) { this->currentANimationSeconds = currentANimationSeconds; }

	float GetRadius() {return radius;}
	void  SetRadius(float radius) { this->radius = radius;}


	float GetHeight() { return height; }
	void  SetHeight(float height) { this->height = height; }

	// コンポーネント追加
	template<class T, class... Args>
	std::shared_ptr<T> AddComponent(Args... args)
	{
		std::shared_ptr<T> component = std::make_shared<T>(args...);
		component->SetActor(shared_from_this());
		components.emplace_back(component);
		return component;
	}

	// コンポーネント取得
	template<class T>
	std::shared_ptr<T> GetComponent()
	{
		for (std::shared_ptr<Component>& component : components)
		{
			std::shared_ptr<T> p = std::dynamic_pointer_cast<T>(component);
			if (p == nullptr) continue;
			return p;
		}
		return nullptr;
	}


private:
	std::vector<std::shared_ptr<Component>>	components;
	std::string			name;
	//////////////////////////////////////
	// モデル系
	//////////////////////////////////////
	std::unique_ptr<Model>	model;

	std::unique_ptr<Model>	modelsabe;

	DirectX::XMFLOAT3	position = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT4	rotation = DirectX::XMFLOAT4(0, 0, 0, 1);
	DirectX::XMFLOAT3	scale = DirectX::XMFLOAT3(1, 1, 1);
	DirectX::XMFLOAT4X4	transform = DirectX::XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	
	// 半径05 
	float radius = 0.5f;

	float height = 1;

	//	透明度
	float alpha = 1;


	// アニメーションの時間
	float currentANimationSeconds = 0.0f;
	//////////////////////////////////////
	// 現象
	//////////////////////////////////////

	// 重力
	//float              grabity = -1.0f;

	//最大値HP
	//int          maxHealth = 5;
	// HP
 //    int          health = maxHealth;
     


	//// 空中での移動速度減少
	//float airControl = 0.3f;


	//// 地面確認
	//bool         isGround = false;

};

// アクターマネージャー
class ActorManager
{
private:
	ActorManager() {}
	~ActorManager() {};

public:
	// インスタンス取得
	static ActorManager& Instance()
	{
		static ActorManager instance;
		return instance;
	}

	// 作成
	std::shared_ptr<Actor> Create();

	// 削除
	void Remove(std::shared_ptr<Actor> actor);

	// 更新
	void Update(float elapsedTime);

	// 行列更新
	void UpdateTransform();

	// 描画
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
	//void Render(const RenderContext& rc);

private:
	void DrawLister();
	void DrawDetail();

private:
	std::vector<std::shared_ptr<Actor>>		startActors;
	std::vector<std::shared_ptr<Actor>>		updateActors;
	std::set<std::shared_ptr<Actor>>		selectionActors;
	std::set<std::shared_ptr<Actor>>		removeActors;

	bool					hiddenLister = false;
	bool					hiddenDetail = false;
};
