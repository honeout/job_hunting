#pragma once

#include <memory>
#include <vector>
#include <set>
#include <string>
#include <DirectXMath.h>
#include "Graphics\Model.h"
#include "Graphics\RenderContext.h"
#include "Graphics\Shader.h"

// ‘O•ûéŒ¾
class Component;

// ƒAƒNƒ^[
class Actor : public std::enable_shared_from_this<Actor>
{
public:
	Actor() {}
	virtual ~Actor() {}

	// ŠJnˆ—
	virtual void Start();

	// XVˆ—
	virtual void Update(float elapsedTime);

	// •`‰æ
	virtual void Render(RenderContext rc);

	// GUI•\¦
	virtual void OnGUI();

	// –¼‘O‚Ìİ’è
	void SetName(const char* name) { this->name = name; }

	// –¼‘O‚Ìæ“¾
	const char* GetName() const { return name.c_str(); }

	// ƒRƒ“ƒ|[ƒlƒ“ƒg’Ç‰Á
	template<class T, class... Args>
	std::shared_ptr<T> AddComponent(Args... args)
	{
		std::shared_ptr<T> component = std::make_shared<T>(args...);
		component->SetActor(shared_from_this());
		components.emplace_back(component);
		return component;
	}

	// ƒRƒ“ƒ|[ƒlƒ“ƒgæ“¾
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
	
};

// ƒAƒNƒ^[ƒ}ƒl[ƒWƒƒ[
class ActorManager
{
private:
	ActorManager() {}
	~ActorManager() {};

public:
	// ƒCƒ“ƒXƒ^ƒ“ƒXæ“¾
	static ActorManager& Instance()
	{
		static ActorManager instance;
		return instance;
	}

	// ì¬
	std::shared_ptr<Actor> Create();

	// íœ
	void Remove(std::shared_ptr<Actor> actor);

	// XV
	void Update(float elapsedTime);

	// s—ñXV
	//void UpdateTransform();

	// •`‰æ
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
<<<<<<< HEAD

<<<<<<< HEAD

	void RenderShadowmap(RenderContext rc);
=======
	void RenderShadowmap(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
>>>>>>> parent of 0db33c3 (ä¸€åº¦ä¿å­˜ãƒ¯ãƒ¼ãƒ‹ãƒ³ã‚°å¤§é‡)

=======

	void RenderShadowmap(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
>>>>>>> parent of 0db33c3 (ä¸€åº¦ä¿å­˜ãƒ¯ãƒ¼ãƒ‹ãƒ³ã‚°å¤§é‡)

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
