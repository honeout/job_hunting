#pragma once

#include <memory>
#include <vector>
#include <set>
#include <string>
#include <DirectXMath.h>
#include "Graphics\Model.h"

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

	// �s��̍X�V
	virtual void UpdateTransform();

	// GUI�\��
	virtual void OnGUI();

	// ���O�̐ݒ�
	void SetName(const char* name) { this->name = name; }

	// ���O�̎擾
	const char* GetName() const { return name.c_str(); }

	// �ʒu�̐ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	// �ʒu�̎擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	// ��]�̐ݒ�
	void SetRotation(const DirectX::XMFLOAT4& rotation) { this->rotation = rotation; }
	void SetRotation3(const DirectX::XMFLOAT3& rotation) { 
		this->rotation.x = rotation.x;
		this->rotation.y = rotation.y;
		this->rotation.z = rotation.z;
	}

	// ��]�̎擾
	const DirectX::XMFLOAT4& GetRotation() const { return rotation; }

	// �X�P�[���̐ݒ�
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	// �X�P�[���̎擾
	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	// �s��̎擾
	const DirectX::XMFLOAT4X4& GetTransform() const { return transform; }

	// ���f���̓ǂݍ���
	void LoadModel(const char* filename);

	void LoadModelSabe(const char* filename);

	// ���f���̎擾
	Model* GetModel() const { return model.get(); }
	Model* GetModelSabe() const { return modelsabe.get(); }
	


	// �A�j���[�V�����̎���
	float GetCurrentANimationSeconds() const { return currentANimationSeconds; }
	// �A�j���[�V�����̎���
	void SetCurrentAnimationSeconds(const float currentANimationSeconds) { this->currentANimationSeconds = currentANimationSeconds; }

	float GetRadius() {return radius;}
	void  SetRadius(float radius) { this->radius = radius;}


	float GetHeight() { return height; }
	void  SetHeight(float height) { this->height = height; }

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
	//////////////////////////////////////
	// ���f���n
	//////////////////////////////////////
	std::unique_ptr<Model>	model;

	std::unique_ptr<Model>	modelsabe;

	DirectX::XMFLOAT3	position = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT4	rotation = DirectX::XMFLOAT4(0, 0, 0, 1);
	DirectX::XMFLOAT3	scale = DirectX::XMFLOAT3(1, 1, 1);
	DirectX::XMFLOAT4X4	transform = DirectX::XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	
	// ���a05 
	float radius = 0.5f;

	float height = 1;

	//	�����x
	float alpha = 1;


	// �A�j���[�V�����̎���
	float currentANimationSeconds = 0.0f;
	//////////////////////////////////////
	// ����
	//////////////////////////////////////

	// �d��
	//float              grabity = -1.0f;

	//�ő�lHP
	//int          maxHealth = 5;
	// HP
 //    int          health = maxHealth;
     


	//// �󒆂ł̈ړ����x����
	//float airControl = 0.3f;


	//// �n�ʊm�F
	//bool         isGround = false;

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

	// �폜
	void Remove(std::shared_ptr<Actor> actor);

	// �X�V
	void Update(float elapsedTime);

	// �s��X�V
	void UpdateTransform();

	// �`��
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
