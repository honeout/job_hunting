#pragma once

#include <memory>
#include <vector>
#include <set>
#include <string>
#include <DirectXMath.h>
#include "Graphics\Model.h"
#include "Graphics\RenderContext.h"
#include "Graphics\Shader.h"

// �O���錾
class Component;

// �A�N�^�[
class Actor : public std::enable_shared_from_this<Actor>
{
public:
	Actor() {}
	virtual ~Actor() {}

	// �J�n����
	virtual void Start();

	// �X�V����
	virtual void Update(float elapsedTime);

	// �`��
	virtual void Render(RenderContext rc, ModelShader* shader);
	virtual void Render2D(RenderContext rc, SpriteShader* shader);




#ifdef _DEBUG
	// GUI�\��
	virtual void OnGUI();
#endif // _DEBUG

	// ���O�̐ݒ�
	void SetName(const char* name) { this->name = name; }

	// ���O�̎擾
	const char* GetName() const { return name.c_str(); }

	// �QD�`�F�b�N
	bool GetCheck2d() { return check2d; }
	void SetCheck2d(bool check2d) { this->check2d = check2d; }

	// �R���|�[�l���g�ǉ�
	template<class T, class... Args>
	std::shared_ptr<T> AddComponent(Args... args)
	{
		std::shared_ptr<T> component = std::make_shared<T>(args...);
		component->SetActor(shared_from_this());
		components.emplace_back(component);
		return component;
	}

	// �R���|�[�l���g�擾
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
	
	// �QD����
	bool                    check2d;
};

// �A�N�^�[�}�l�[�W���[
class ActorManager
{
private:
	ActorManager() {}
	~ActorManager() {};

public:
	// �C���X�^���X�擾
	static ActorManager& Instance()
	{
		static ActorManager instance;
		return instance;
	}

	// �쐬
	std::shared_ptr<Actor> Create();

	// ��Â폜
	void Remove(std::shared_ptr<Actor> actor);

	// �X�V
	void Update(float elapsedTime);



	// 3D�`��
	void Render(RenderContext rc, ModelShader* shader);
	// 2D�`��
	void Render2D(RenderContext rc, SpriteShader* shader);
	// �S�폜
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
