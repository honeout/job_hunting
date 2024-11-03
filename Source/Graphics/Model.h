#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "Graphics/ModelResource.h"

// ���f��
class Model
{
public:
	Model(const char* filename);
	~Model() {}

	struct Node
	{
		const char*			name;
		Node*				parent;
		DirectX::XMFLOAT3	position = { 0, 0, 0 };
		DirectX::XMFLOAT4	rotation = { 0, 0, 0, 1 };
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4	localTransform;
		DirectX::XMFLOAT4X4	worldTransform;

		std::vector<Node*>	children;
		int now;
	};

	// 
	struct NodePose
	{
		DirectX::XMFLOAT3	position = { 0, 0, 0 };
		DirectX::XMFLOAT4	rotation = { 0, 0, 0, 1 };
		DirectX::XMFLOAT3	scale = { 1, 1, 1 };
	};



	//struct VectorKeyframe
	//{
	//	float					seconds;
	//	DirectX::XMFLOAT3		value;

	//	template<class Archive>
	//	void serialize(Archive& archive);
	//};

	//struct QuaternionKeyframe
	//{
	//	float					seconds;
	//	DirectX::XMFLOAT4		value;

	//	template<class Archive>
	//	void serialize(Archive& archive);
	//};


	//struct NodeAnim
	//{
	//	std::vector<VectorKeyframe>		positionKeyframes;
	//	std::vector<QuaternionKeyframe>	rotationKeyframes;
	//	std::vector<VectorKeyframe>		scaleKeyframes;

	//	template<class Archive>
	//	void serialize(Archive& archive);
	//};

	// �s��v�Z
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);
	// �s��v�Z�@�{�[���������L
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform,  std::vector<Node> nodes);

	//// �A�j���[�V�����v�Z
	void ComputeAnimation(int animationIndex, int nodeIndex, float time, NodePose& nodePose, float blendRate) ;
	//// �S���̎p��
	void ComputeAnimation(int animationIndex, float time, std::vector<NodePose>& nodePoses, float blendRate) ;

	// ���[�g���[�V�����A�b�v�f�[�g
	void UpdateRootMortion(float elapsedTime);

	// �m�[�h���X�g�擾
	const std::vector<Node>& GetNodes() const { return nodes; }
	//std::vector<Node>& GetNodes() { return nodes; }



	// ���\�[�X�擾
	const ModelResource* GetResource() const { return resource.get(); }

	// �A�j���[�V�����X�V����
	void UpdateAnimation(float elapsedTime, bool blend = false);

	// ���[�g���[�V����
	//void UpdateRootAnimation(float elapsedTime,char * bornName, bool blend = false);

	// �t�Đ�
	void ReverseplaybackAnimation(float elapsedTime, bool blend = false);

	// ����u�����h
	void Update_blend_animations_gun(float elapsedTime, float blendrate,float currentAnimationSeconds, float currentAnimationSecondsblend,bool blend = false);

	// ��̃A�j���[�V�����u�����h
	void Update_blend_animations(float elapsedTime, bool blend = false);


	//�㔼�g�A�j���[�V�����X�V����
	void UpdateUpeerBodyAnimation(float elapsedTime, const char* start, bool blend = false);

	// �㔼�g�p�A�j���[�V�����X�V
	void ReverseplaybackUpeerBodyAnimation(float elapsedTime, const char* start, bool blend = false);

	//�����g�A�j���[�V�����X�V����
	void UpdateLowerBodyAnimation(float elapsedTime, const char* end, bool blend = false);

	void ReverseplaybackLowerBodyAnimation(float elapsedTime, const char* end, bool blend = false);

	//�A�j���[�V�����Đ�
	void PlayUpeerBodyAnimation(int index, bool loop, float currentanimationseconds, float blendSeconds = 0.2f);

	//�A�j���[�V�����Đ�
	void PlayLowerBodyAnimation(int index, bool loop, float blendSeconds = 0.2f);


	// �A�j���[�V�����Đ�
	void PlayAnimation(int index, bool loop,float currentanimationseconds = 0.0f, float blendSeconds = 0.2f);

	//�t�Đ��A�j���[�V����
	void PlayReverseAnimation(int index, bool loop, float currentanimationseconds = 0.0f, float blendSeconds = 0.2f);

	// ��̃A�j���[�V�����u�����h
	void PlayAnimationBlend(int index,int index2, bool loop, float currentanimationseconds = 0.0f, float blendSeconds = 0.2f);
	

	// �A�j���[�V�����Đ�����
	bool IsPlayAnimation() const;

	bool IsPlayUpeerBodyAnimation() const;

	bool IsPlayAnimationBlend() const;

	// �m�[�h����
	Node* FindNode(const char* name);

	// ���݂̃A�j���[�V�����Đ����Ԏ擾
	float GetCurrentANimationSeconds()const { return currentAnimationSeconds; }

	// ���݂̏㔼�g�̃A�j���[�V�����Đ����Ԏ擾
	float GetCurrentAnimationSecondsUpeer()const { return currentAnimationSecondsUpeer; }

	int GetNodeIndex(const char* name)const;

	// �m�[�h�|�[�Y�ݒ�
	void SetNodePoses(const std::vector<NodePose>& nodePoses);

	void SetAnimationEndFlag(bool end) { this->animationEndFlag = end; }

private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>				nodes;

	std::vector<Model::NodePose>		nodePoses;

	// �X���[���ƂȂ��Ƃ�����
	int currentAnimationIndex = -1;           //�A�j���[�V�����ԍ�
	int currentAnimationIndexSeconds = -1;           //�A�j���[�V�����ԍ� �u�����h�p
	float currentAnimationSeconds = 0.0f;    //�A�j���[�V�����̍Đ��p����
	float oldcurrentAnimationSeconds = 0;
	float rastcurrentAnimationSeconds = 0.67;
	float animationSecondsLengthMax = 0.0f;

	//�p���`���������[�P�܂�A�j���[�V�����I���܂�T�|�[�Y�ɖ߂�B

	// ���[�v�I����
	bool animationLoopFlag = false;
	// �����V���b�g�I��
	bool animationEndFlag = false;


	float animationBlendTime = 0.0f;
	float animationBlendSeconds = 0.0f;

	bool agterimagestop = false;


	// �㔼�g�p�̃A�j���[�V��������
	int currentAnimationIndexUpeer = -1;				//�A�j���[�V�����ԍ�
	float currentAnimationSecondsUpeer = 0.0f;		//�A�j���[�V�����̍Đ��p����
	bool animationLoopFlagUpeer = false;				//�A�j���[�V���������[�v���邩
	bool animationEndFlagUpeer = false;				//�A�j���[�V�������Đ��I�����Ă��邩
	float animationBlendTimeUpeer = 0.0f;			//�A�j���[�V�����⊮����
	float animationBlendSecondsUpeer = 0.0f;			//�V


     // �㔼�g�̃L�[�t���[������
	int keyframeUpper = 0;
	// �����g�̃L�[�t���[������
	int keyframeLower = 0;

	// �Đ��@�t�Đ�
	bool anim = true;
};
//
//template<class Archive>
//inline void Model::VectorKeyframe::serialize(Archive& archive)
//{
//	archive(
//		CEREAL_NVP(seconds),
//		CEREAL_NVP(value)
//	);
//}
//
//template<class Archive>
//inline void Model::QuaternionKeyframe::serialize(Archive& archive)
//{
//	archive(
//		CEREAL_NVP(seconds),
//		CEREAL_NVP(value)
//	);
//}
//
//template<class Archive>
//inline void Model::NodeAnim::serialize(Archive& archive)
//{
//	archive(
//		CEREAL_NVP(positionKeyframes),
//		CEREAL_NVP(rotationKeyframes),
//		CEREAL_NVP(scaleKeyframes)
//	);
//}
