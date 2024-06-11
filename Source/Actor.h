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

	// ���f���̎擾
	Model* GetModel() const { return model.get(); }

	void SetStepOffset(float stepOffset) {  this->stepOffset = stepOffset; }
	float GetStepOffset() const { return this->stepOffset; }

	// �A�j���[�V�����̎���
	float GetCurrentANimationSeconds() const { return currentANimationSeconds; }
	// �A�j���[�V�����̎���
	void SetCurrentAnimationSeconds(const float currentANimationSeconds) { this->currentANimationSeconds = currentANimationSeconds; }


	// HP�ǂݎ��
	int   GetHealth() { return health; }

	// HP��������
	void   SetHealth(int health) { this->health = health; }

	//DirectX::XMFLOAT3 GetVelocity() { return this->velocity; }

	//void SetVelocity(DirectX::XMFLOAT3 velocity) { this->velocity = velocity; }

	// �n�ʂɐݒu���Ă��邩 �G�t�F�N�g����
	//bool IsGround() const { return isGround; }

	//void SetAirControl(float airControl) { this->airControl = airControl; }
	//float GetAirControl() { return this->airControl; }

	// �����擾
	float GetHeight() const { return height; }

	//// �������͍X�V����
	//void UpdateHorizontalVelocity(float elapsedFrame);
	//// �����ړ��X�V����
	//void UpdateHorizontalMove(float elapsedTime);

	//// �������͍X�V����
	//void UpdateVerticalVelocity(float elapsedFrame);

	//// �����ړ��X�V����
	//void UpdateVerticalMove(float elapsedTime);
	//// ���͏����X�V
	//void UpdateVelocity(float elapsedTime);


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
	DirectX::XMFLOAT3	position = DirectX::XMFLOAT3(0, 0, 0);
	DirectX::XMFLOAT4	rotation = DirectX::XMFLOAT4(0, 0, 0, 1);
	DirectX::XMFLOAT3	scale = DirectX::XMFLOAT3(1, 1, 1);
	DirectX::XMFLOAT4X4	transform = DirectX::XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	
	//	�����x
	float alpha = 1;
	// ��
	float stepOffset = 1.0f;

	// �g�����[�g��
	float               height = 2.0f;

	// �A�j���[�V�����̎���
	float currentANimationSeconds = 0.0f;
	//////////////////////////////////////
	// ����
	//////////////////////////////////////

	// �d��
	//float              grabity = -1.0f;

	//�ő�lHP
	int          maxHealth = 5;
	 //HP
     int          health = maxHealth;
     


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
