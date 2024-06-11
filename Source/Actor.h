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

	// モデルの取得
	Model* GetModel() const { return model.get(); }

	void SetStepOffset(float stepOffset) {  this->stepOffset = stepOffset; }
	float GetStepOffset() const { return this->stepOffset; }

	// アニメーションの時間
	float GetCurrentANimationSeconds() const { return currentANimationSeconds; }
	// アニメーションの時間
	void SetCurrentAnimationSeconds(const float currentANimationSeconds) { this->currentANimationSeconds = currentANimationSeconds; }


	// HP読み取り
	int   GetHealth() { return health; }

	// HP書き込み
	void   SetHealth(int health) { this->health = health; }

	//DirectX::XMFLOAT3 GetVelocity() { return this->velocity; }

	//void SetVelocity(DirectX::XMFLOAT3 velocity) { this->velocity = velocity; }

	// 地面に設置しているか エフェクト等も
	//bool IsGround() const { return isGround; }

	//void SetAirControl(float airControl) { this->airControl = airControl; }
	//float GetAirControl() { return this->airControl; }

	// 高さ取得
	float GetHeight() const { return height; }

	//// 水平速力更新処理
	//void UpdateHorizontalVelocity(float elapsedFrame);
	//// 水平移動更新処理
	//void UpdateHorizontalMove(float elapsedTime);

	//// 垂直速力更新処理
	//void UpdateVerticalVelocity(float elapsedFrame);

	//// 垂直移動更新処理
	//void UpdateVerticalMove(float elapsedTime);
	//// 速力処理更新
	//void UpdateVelocity(float elapsedTime);


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
	DirectX::XMFLOAT3	position = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT4	rotation = DirectX::XMFLOAT4(0, 0, 0, 1);
	DirectX::XMFLOAT3	scale = DirectX::XMFLOAT3(1, 1, 1);
	DirectX::XMFLOAT4X4	transform = DirectX::XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	
	//	透明度
	float alpha = 1;
	// 腰
	float stepOffset = 1.0f;

	// 身長メートル
	float               height = 2.0f;

	// アニメーションの時間
	float currentANimationSeconds = 0.0f;
	//////////////////////////////////////
	// 現象
	//////////////////////////////////////

	// 重力
	//float              grabity = -1.0f;

	//最大値HP
	int          maxHealth = 5;
	 //HP
     int          health = maxHealth;
     


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
