#pragma once

#include <memory>
#include <vector>
#include <set>
#include <string>
#include <DirectXMath.h>
#include "Graphics\Model.h"
#include "Graphics\RenderContext.h"
#include "Graphics\Shader.h"

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

	// 描画
	virtual void Render(RenderContext rc, ModelShader* shader);
	virtual void Render2D(RenderContext rc, SpriteShader* shader);




#ifdef _DEBUG
	// GUI表示
	virtual void OnGUI();
#endif // _DEBUG

	// 名前の設定
	void SetName(const char* name) { this->name = name; }

	// 名前の取得
	const char* GetName() const { return name.c_str(); }

	// ２Dチェック
	bool GetCheck2d() { return check2d; }
	void SetCheck2d(bool check2d) { this->check2d = check2d; }

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
	
	// ２D判定
	bool                    check2d;
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

	// 一つづつ削除
	void Remove(std::shared_ptr<Actor> actor);

	// 更新
	void Update(float elapsedTime);



	// 3D描画
	void Render(RenderContext rc, ModelShader* shader);
	// 2D描画
	void Render2D(RenderContext rc, SpriteShader* shader);
	// 全削除
	void Clear();


private:
#ifdef _DEBUG
	void DrawLister();
	void DrawDetail();
#endif // _DEBUG
private:
	std::vector<std::shared_ptr<Actor>>		startActors;
	std::vector<std::shared_ptr<Actor>>		updateActors;
	std::set<std::shared_ptr<Actor>>		selectionActors;
	std::set<std::shared_ptr<Actor>>		removeActors;



	bool					hiddenLister = false;
	bool					hiddenDetail = false;
};
