#include <string.h>
#include "Graphics/Graphics.h"
#include "Graphics/Model.h"

#include "ResourceManager.h"

// �R���X�g���N�^
Model::Model(const char* filename)
{
	// ���\�[�X�ǂݍ���
	//resource = std::make_shared<ModelResource>();
	//resource->Load(Graphics::Instance().GetDevice(), filename);
	resource = ResourceManager::Instance().LoadModelResource(filename);

	// �m�[�h
	const std::vector<ModelResource::Node>& resNodes = resource->GetNodes();

	nodes.resize(resNodes.size());
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		auto&& src = resNodes.at(nodeIndex);
		auto&& dst = nodes.at(nodeIndex);

		dst.name = src.name.c_str();
		dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;

		if (dst.parent != nullptr)
		{
			dst.parent->children.emplace_back(&dst);
		}
	}

	// �s��v�Z
	const DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	UpdateTransform(transform);
}

// �ϊ��s��v�Z
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);


	for (Node& node : nodes)
	{
		// ���[�J���s��Z�o
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ���[���h�s��Z�o
		DirectX::XMMATRIX ParentTransform;
		if (node.parent != nullptr)
		{
			ParentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
		}
		else
		{
			ParentTransform = Transform;
		}
		DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

		// �v�Z���ʂ��i�[
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}

void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform,  std::vector<Node> nodes)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);

	this->nodes = nodes;
	for (Node& node : this->nodes)
	{
		// ���[�J���s��Z�o
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ���[���h�s��Z�o
		DirectX::XMMATRIX ParentTransform;
		if (node.parent != nullptr)
		{
			ParentTransform = DirectX::XMLoadFloat4x4(&node.parent->worldTransform);
		}
		else
		{
			ParentTransform = Transform;
		}
		DirectX::XMMATRIX WorldTransform = LocalTransform * ParentTransform;

		// �v�Z���ʂ��i�[
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}


void Model::UpdateAnimation(float elapsedTime, bool blend)
{
	// �Đ����łȂ��Ȃ珈�����Ȃ�
	if (!IsPlayAnimation())return;

	// �u�����h���̌v�Z
	float blendRate = 1.0f;
	// ���݂̃u�����h�����Ԃ�艺�Ȃ�
	if (animationBlendTime < animationBlendSeconds)
	{
		// �����Ă���B
		animationBlendTime += elapsedTime;
		if (animationBlendTime >= animationBlendSeconds)
		{
			animationBlendTime = animationBlendSeconds;
		}
		blendRate = animationBlendTime / animationBlendSeconds;
		// ������ĂЂÂ݂������B
		blendRate *= blendRate;
	}


	// �w��̃A�j���[�V�����f�[�^���擾
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);

	// �A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int> (keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		// ���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
		if (currentAnimationSeconds >= keyframe0.seconds &&
			currentAnimationSeconds < keyframe1.seconds)
		{
			// �Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
			// ����Ɍ��݂̃A�j���[�V�����ƃL�[�t���[���́[���ƂŁA���q�ɂP�ƂO�����邱�ƂłO�D���p�[�Z���g�Ⴆ��
			// �܂�^���I�����Ⴆ��B
			float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds
				- keyframe0.seconds);

			int nodeCount = static_cast<int>(nodes.size());
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				//2�̃L�[�t���[���Ԃ̕⊮�v�Z
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];
				// �u�����h�v�Z
				if (blendRate < 1.0f && blend)
				{
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					// �L�[�t���[���̊��������߂�B
					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);
					// �v�Z���ʂ��{�[���Ɋi�[
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
				// �ʏ�̌v�Z

				else
				{
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					// �L�[�t���[���̊��������߂�B
					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);
					// �v�Z���ʂ��{�[���Ɋi�[
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

			}

			break;
		}
	}

	// �ŏI�t���[������
	if (animationEndFlag)
	{
		animationEndFlag = false;
		currentAnimationIndex = -1;
		return;
	}



	// ���Ԍo��
	currentAnimationSeconds += elapsedTime;

	// �Đ����Ԃ��I�[���Ԃ𒴂�����
	// �ō��𒴂����ꍇ�̃��[�v�̏ꍇ�����B
	if (currentAnimationSeconds >= animation.secondsLength)
	{
		// ���[�v�Ȃ�g�D���[
		if (animationLoopFlag)
		{
			// �Đ����Ԃ��I�[���Ԃ𒴂�����


				// ���Ł[���I�[�o�[�������̎����l���������ł����Ƃ��Ȃ��B
				// �Đ������������߂�
			currentAnimationSeconds -= animation.secondsLength;

		}
		else
		{
			// �Đ����Ԃ��I�[���Ԃ𒴂�����

				// ���Ł[���I�[�o�[�������̎����l���������ł����Ƃ��Ȃ��B
				// �Đ������������߂�
			// ��ԍŌ�ɃI�[�o�[������~�߂�B
			currentAnimationSeconds = animation.secondsLength;
			// �I���܂����B
			animationEndFlag = true;
		}
	}




}

void Model::ReverseplaybackAnimation(float elapsedTime, bool blend)
{
	//�Đ����łȂ��Ȃ珈�����Ȃ�
	if (!IsPlayAnimation()) return;

	//�u�����h���̌v�Z
	float blendRate = 1.0f;
	if (animationBlendTime < animationBlendSeconds)
	{
		animationBlendTime += elapsedTime;
		if (animationBlendTime >= animationBlendSeconds)
		{
			animationBlendTime = animationBlendSeconds;
		}
		blendRate = animationBlendTime / animationBlendSeconds;
		blendRate *= blendRate;
	}

	//�w��̃A�j���[�V�����f�[�^���擾
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);
	if (anim)
	{
		currentAnimationSeconds = animation.secondsLength;
		anim = false;
	}

	//�A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = keyCount - 1; keyIndex > 0; --keyIndex)
	{

		//���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex - 1);
		if (currentAnimationSeconds <= keyframe0.seconds &&
			currentAnimationSeconds > keyframe1.seconds)
		{
			//�Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
			float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

			// 12

			int nodeCount = static_cast<int>(nodes.size());
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				//2�̃L�[�t���[���Ԃ̕⊮�v�Z
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];

				//�u�����h�⊮����
				if (blendRate < 1.0f && blend)
				{
					//���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕⊮
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

					//�v�Z���ʂ��{�[���Ɋi�[
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

				else
				{
					//�O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

					//�v�Z���ʂ��{�[���Ɋi�[
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
			}
			break;
		}
	}

	//�ŏI�t���[������
	if (animationEndFlag)
	{
		animationEndFlag = false;
		currentAnimationIndex = -1;
		anim = true;
		return;
	}

	//���Ԍo��
	currentAnimationSeconds -= elapsedTime;

	//�Đ����Ԃ��T�Z���Ԃ𒴂�����
	if (currentAnimationSeconds <= 0)
	{
		//�Đ����Ԃ������߂�
		if (animationLoopFlag)
		{
			currentAnimationSeconds += animation.secondsLength;
		}
		//�Đ��I�����Ԃɂ���
		else
		{
			currentAnimationSeconds = 0;
			animationEndFlag = true;
		}
	}

}

void Model::Update_blend_animations_gun(float elapsedTime, float blendrate, float currentAnimationSeconds, float currentAnimationSecondsblend, bool blend)
{


	// �Đ����łȂ��Ȃ珈�����Ȃ�
	if (!IsPlayAnimation())return;


	// �w��̃A�j���[�V�����f�[�^���擾
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);

	// �A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	
	//////////////////////����������
	// ���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
	const ModelResource::Keyframe& keyframe0 = keyframes.at(10);
	const ModelResource::Keyframe& keyframe1 = keyframes.at(15);
	if (currentAnimationSeconds <= keyframe0.seconds &&
		currentAnimationSecondsblend > keyframe1.seconds)
	{

		int nodeCount = static_cast<int>(nodes.size());
		for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
		{
			//2�̃L�[�t���[���Ԃ̕⊮�v�Z
			const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
			const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

			Node& node = nodes[nodeIndex];
			// �ʏ�̌v�Z
			{
				DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
				DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
				DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
				DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
				DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
				DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

				// �L�[�t���[���̊��������߂�B
				DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendrate);
				DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendrate);
				DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendrate);
				// �v�Z���ʂ��{�[���Ɋi�[
				DirectX::XMStoreFloat3(&node.scale, S);
				DirectX::XMStoreFloat4(&node.rotate, R);
				DirectX::XMStoreFloat3(&node.translate, T);
			}

		}
	}

	
}

void Model::Update_blend_animations(float elapsedTime,   bool blend)
{

	// �Đ����łȂ��Ȃ珈�����Ȃ�
	if (!IsPlayAnimation())return;

	// �u�����h���̌v�Z
	float blendRate = 1.0f;
	// ���݂̃u�����h�����Ԃ�艺�Ȃ�
	if (animationBlendTime < animationBlendSeconds)
	{
		// �����Ă���B
		animationBlendTime += elapsedTime;
		if (animationBlendTime >= animationBlendSeconds)
		{
			animationBlendTime = animationBlendSeconds;
		}
		blendRate = animationBlendTime / animationBlendSeconds;
		// ������ĂЂÂ݂������B
		blendRate *= blendRate;
	}

	// �w��̃A�j���[�V�����f�[�^���擾
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	// �A�j���[�V�������
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);
	const ModelResource::Animation& animationSeconds = animations.at(currentAnimationIndexSeconds);

	// �A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾


	float animationKeyFrames;
	
	//DirectX::XMStoreFloat(&animationKeyFrames, DirectX::XMVector3Normalize(DirectX::XMLoadFloat(&animation.keyframes)));
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	const std::vector<ModelResource::Keyframe>& keyframesSeconds = animationSeconds.keyframes;



	int keyCount = static_cast<int> (keyframes.size());

	int keyCountSeconds = static_cast<int> (keyframesSeconds.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1 && keyIndex < keyCountSeconds - 1; ++keyIndex)
	{

		//////////////////////����������
		// ���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
		
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
		
		const ModelResource::Keyframe& keyframeSeconds0 = keyframesSeconds.at(keyIndex);
		const ModelResource::Keyframe& keyframeSeconds1 = keyframesSeconds.at(keyIndex + 1);





		if (currentAnimationSeconds >= keyframe0.seconds &&
			currentAnimationSeconds < keyframe1.seconds&&
			currentAnimationSeconds >= keyframeSeconds0.seconds &&
			currentAnimationSeconds < keyframeSeconds1.seconds)
		{
			// �Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
			//float rate = (currentAnimationSeconds - keyframe0.seconds ) / (keyframe1.seconds
			//	- keyframe0.seconds);

			int nodeCount = static_cast<int>(nodes.size());
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				//2�̃L�[�t���[���Ԃ̕⊮�v�Z
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				const ModelResource::NodeKeyData& keySeconds0 = keyframeSeconds0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& keySeconds1 = keyframeSeconds1.nodeKeys.at(nodeIndex);
				
				Node& node = nodes[nodeIndex];
				// �A�j���[�V������
				if (blendRate < 1.0f && blend)
				{

					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S0Seconds = DirectX::XMLoadFloat3(&keySeconds0.scale);
					DirectX::XMVECTOR S1Seconds = DirectX::XMLoadFloat3(&keySeconds1.scale);
					DirectX::XMVECTOR R0Seconds = DirectX::XMLoadFloat4(&keySeconds0.rotate);
					DirectX::XMVECTOR R1Seconds = DirectX::XMLoadFloat4(&keySeconds1.rotate);
					DirectX::XMVECTOR T0Seconds = DirectX::XMLoadFloat3(&keySeconds0.translate);
					DirectX::XMVECTOR T1Seconds = DirectX::XMLoadFloat3(&keySeconds1.translate);

					S0 = DirectX::XMVectorLerp(S0, S0Seconds, blendRate);
					S1 = DirectX::XMVectorLerp(S1, S1Seconds, blendRate);
					R0 = DirectX::XMQuaternionSlerp(R0, R0Seconds, blendRate);
					R1 = DirectX::XMQuaternionSlerp(R1, R1Seconds, blendRate);
					T0 = DirectX::XMVectorLerp(T0, T0Seconds, blendRate);
					T1 = DirectX::XMVectorLerp(T1, T1Seconds, blendRate);


					// �L�[�t���[���̊��������߂�B
					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);
					// �v�Z���ʂ��{�[���Ɋi�[
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
				// �ʏ�̌v�Z
				else
				{

					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S0Seconds = DirectX::XMLoadFloat3(&keySeconds0.scale);
					DirectX::XMVECTOR S1Seconds = DirectX::XMLoadFloat3(&keySeconds1.scale);
					DirectX::XMVECTOR R0Seconds = DirectX::XMLoadFloat4(&keySeconds0.rotate);
					DirectX::XMVECTOR R1Seconds = DirectX::XMLoadFloat4(&keySeconds1.rotate);
					DirectX::XMVECTOR T0Seconds = DirectX::XMLoadFloat3(&keySeconds0.translate);
					DirectX::XMVECTOR T1Seconds = DirectX::XMLoadFloat3(&keySeconds1.translate);

					S0 = DirectX::XMVectorLerp(S0, S0Seconds, animationBlendSeconds);
					S1 = DirectX::XMVectorLerp(S1, S1Seconds, animationBlendSeconds);
					R0 = DirectX::XMQuaternionSlerp(R0, R0Seconds, animationBlendSeconds);
					R1 = DirectX::XMQuaternionSlerp(R1, R1Seconds, animationBlendSeconds);
					T0 = DirectX::XMVectorLerp(T0, T0Seconds, animationBlendSeconds);
					T1 = DirectX::XMVectorLerp(T1, T1Seconds, animationBlendSeconds);


					// �L�[�t���[���̊��������߂�B
					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, animationBlendSeconds);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, animationBlendSeconds);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, animationBlendSeconds);


					// �v�Z���ʂ��{�[���Ɋi�[
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

			}
			break;
		}
	}


	// �ŏI�t���[������
	if (animationEndFlag)
	{
		animationEndFlag = false;
		currentAnimationIndex = -1;
		currentAnimationIndexSeconds = -1;
		return;
	}



	// ���Ԍo��
	currentAnimationSeconds += elapsedTime;

	//float animationtime = animation.secondsLength - animationSeconds.secondsLength;

	//float animationSecondsLength;
	//DirectX::XMStoreFloat(&animationSecondsLength,DirectX::XMVector3Normalize(DirectX::XMLoadFloat(&animation.secondsLength)));

	// �Đ����Ԃ��I�[���Ԃ𒴂�����
	// �ō��𒴂����ꍇ�̃��[�v�̏ꍇ�����B
	//if (currentAnimationSeconds >= animationSecondsLength)
	if (currentAnimationSeconds >= animation.secondsLength)
	{
		// ���[�v�Ȃ�g�D���[
		if (animationLoopFlag)
		{
			// �Đ����Ԃ��I�[���Ԃ𒴂�����

			
			// ���Ł[���I�[�o�[�������̎����l���������ł����Ƃ��Ȃ��B
			// �Đ������������߂�
			


			//currentAnimationSeconds -= animation.secondsLength;
			DirectX::XMVECTOR anim = DirectX::XMLoadFloat(&animation.secondsLength);
			DirectX::XMVECTOR animSecond = DirectX::XMLoadFloat(&animationSeconds.secondsLength);

			// �A�j���[�V��������
			float animSecondsLength;
			DirectX::XMStoreFloat(&animSecondsLength, DirectX::XMVectorLerp(anim, animSecond, animationBlendSeconds));
			currentAnimationSeconds -= animSecondsLength;



		}
		else
		{
			// �Đ����Ԃ��I�[���Ԃ𒴂�����

				// ���Ł[���I�[�o�[�������̎����l���������ł����Ƃ��Ȃ��B
				// �Đ������������߂�
			// ��ԍŌ�ɃI�[�o�[������~�߂�B
			
			currentAnimationSeconds = animation.secondsLength;

			// �I���܂����B
			animationEndFlag = true;
		}
	}





}


// �㔼�g�p�A�j���[�V����
void Model::UpdateUpeerBodyAnimation(float elapsedTime, const char* start, const char* end, bool blend)
{
	//int nodeIndexStart = GetNodeIndex(start);
	//int nodeIndexEnd = GetNodeIndex(end);

	Model::Node* node = FindNode(start);

	int nodeIndexStart = node->now;
	//�Đ����łȂ��Ȃ珈�����Ȃ�
	if (!IsPlayUpeerBodyAnimation()) return;

	//�u�����h���̌v�Z
	float blendRate = 1.0f;
	if (animationBlendTimeUpeer < animationBlendSecondsUpeer)
	{
		animationBlendTimeUpeer += elapsedTime;
		if (animationBlendTimeUpeer >= animationBlendSecondsUpeer)
		{
			animationBlendTimeUpeer = animationBlendSecondsUpeer;
		}
		blendRate = animationBlendTimeUpeer / animationBlendSecondsUpeer;
		blendRate *= blendRate;
	}

	//�w��̃A�j���[�V�����f�[�^���擾
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndexUpeer);

	//�A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		//���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
		if (currentAnimationSecondsUpeer >= keyframe0.seconds &&
			currentAnimationSecondsUpeer < keyframe1.seconds)
		{
			//�Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
			float rate = (currentAnimationSecondsUpeer - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

			// 13


			// �㔼�g�A�j���[�V����
			
			int nodeIndexMax = static_cast<int>(nodes.size());
			for (int nodeIndex = nodeIndexStart; nodeIndex < nodeIndexMax; ++nodeIndex)
			{




				//2�̃L�[�t���[���Ԃ̕⊮�v�Z
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];




				//�u�����h�⊮����
				if (blendRate < 1.0f && blend)
				{
					//���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕⊮
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

					//�v�Z���ʂ��{�[���Ɋi�[
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

				else
				{
					//�O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

					//�v�Z���ʂ��{�[���Ɋi�[
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

			}
			break;
		}
	}

	//�ŏI�t���[������
	if (animationEndFlagUpeer)
	{
		animationEndFlagUpeer = false;
		currentAnimationIndexUpeer = -1;
		return;
	}

	//���Ԍo��
	currentAnimationSecondsUpeer += elapsedTime;

	//�Đ����Ԃ��T�Z���Ԃ𒴂�����
	if (currentAnimationSecondsUpeer >= animation.secondsLength)
	{
		//�Đ����Ԃ������߂�
		if (animationLoopFlagUpeer)
		{
			currentAnimationSecondsUpeer -= animation.secondsLength;
		}
		//�Đ��I�����Ԃɂ���
		else
		{
			currentAnimationSecondsUpeer = animation.secondsLength;
			animationEndFlagUpeer = true;
		}
	}
}

void Model::ReverseplaybackUpeerBodyAnimation(float elapsedTime, const char* start, bool blend)
{
	Model::Node* node = FindNode(start);

	//�Đ����łȂ��Ȃ珈�����Ȃ�
	if (!IsPlayUpeerBodyAnimation()) return;

	//�u�����h���̌v�Z
	float blendRate = 1.0f;
	if (animationBlendTimeUpeer < animationBlendSecondsUpeer)
	{
		animationBlendTimeUpeer += elapsedTime;
		if (animationBlendTimeUpeer >= animationBlendSecondsUpeer)
		{
			animationBlendTimeUpeer = animationBlendSecondsUpeer;
		}
		blendRate = animationBlendTimeUpeer / animationBlendSecondsUpeer;
		blendRate *= blendRate;
	}

	//�w��̃A�j���[�V�����f�[�^���擾
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndexUpeer);

	//�A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int>(keyframes.size());


	if (anim)
	{
		currentAnimationSecondsUpeer = animation.secondsLength;
		anim = false;
	}

	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		//���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
		if (currentAnimationSecondsUpeer >= keyframe0.seconds &&
			currentAnimationSecondsUpeer < keyframe1.seconds)
		{
			//�Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
			float rate = (currentAnimationSecondsUpeer - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

			// 13

			// �㔼�g�A�j���[�V����
			int nodeCount = static_cast<int>(nodes.size());
			for (int nodeIndex = node->now; nodeIndex < nodeCount; ++nodeIndex)
			{




				//2�̃L�[�t���[���Ԃ̕⊮�v�Z
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];




				//�u�����h�⊮����
				if (blendRate < 1.0f && blend)
				{
					//���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕⊮
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

					//�v�Z���ʂ��{�[���Ɋi�[
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

				else
				{
					//�O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

					//�v�Z���ʂ��{�[���Ɋi�[
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
			}
			break;
		}
	}

	//�ŏI�t���[������
	if (animationEndFlagUpeer)
	{
		animationEndFlagUpeer = false;
		currentAnimationIndexUpeer = -1;
		return;
	}

	//���Ԍo��
	currentAnimationSecondsUpeer -= elapsedTime;

	//�Đ����Ԃ��T�Z���Ԃ𒴂�����
	if (currentAnimationSecondsUpeer <= 0)
	{
		//�Đ����Ԃ������߂�
		if (animationLoopFlagUpeer)
		{
			currentAnimationSecondsUpeer += animation.secondsLength;
		}
		//�Đ��I�����Ԃɂ���
		else
		{
			currentAnimationSecondsUpeer = 0;
			animationEndFlagUpeer = true;
		}
	}
}
// �����g�p
void Model::UpdateLowerBodyAnimation(float elapsedTime, const char* end, bool blend)
{
	Model::Node* node = FindNode(end);
	//�Đ����łȂ��Ȃ珈�����Ȃ�
	if (!IsPlayAnimation()) return;

	//�u�����h���̌v�Z
	float blendRate = 1.0f;
	if (animationBlendTime < animationBlendSeconds)
	{
		animationBlendTime += elapsedTime;
		if (animationBlendTime >= animationBlendSeconds)
		{
			animationBlendTime = animationBlendSeconds;
		}
		blendRate = animationBlendTime / animationBlendSeconds;
		blendRate *= blendRate;
	}

	//�w��̃A�j���[�V�����f�[�^���擾
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);

	//�A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;

	// �Đ�
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{

		//���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
		if (currentAnimationSeconds >= keyframe0.seconds &&
			currentAnimationSeconds < keyframe1.seconds)
		{
			//�Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
			float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

			// 12

			int nodeCount = static_cast<int>(node->now);
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				//2�̃L�[�t���[���Ԃ̕⊮�v�Z
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];

				//�u�����h�⊮����
				if (blendRate < 1.0f && blend)
				{
					//���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕⊮
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

					//�v�Z���ʂ��{�[���Ɋi�[
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

				else
				{
					//�O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

					//�v�Z���ʂ��{�[���Ɋi�[
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
			}
			break;
		}
	}

	//�ŏI�t���[������
	if (animationEndFlag)
	{
		animationEndFlag = false;
		currentAnimationIndex = -1;
		return;
	}

	//���Ԍo��
	currentAnimationSeconds += elapsedTime;

	//�Đ����Ԃ��T�Z���Ԃ𒴂�����
	if (currentAnimationSeconds >= animation.secondsLength)
	{
		//�Đ����Ԃ������߂�
		if (animationLoopFlag)
		{
			currentAnimationSeconds -= animation.secondsLength;
		}
		//�Đ��I�����Ԃɂ���
		else
		{
			currentAnimationSeconds = animation.secondsLength;
			animationEndFlag = true;
		}
	}
}
// �t�Đ�
void Model::ReverseplaybackLowerBodyAnimation(float elapsedTime, const char* end, bool blend)
{
	Model::Node* node = FindNode(end);
	//�Đ����łȂ��Ȃ珈�����Ȃ�
	if (!IsPlayAnimation()) return;

	//�u�����h���̌v�Z
	float blendRate = 1.0f;
	if (animationBlendTime < animationBlendSeconds)
	{
		animationBlendTime += elapsedTime;
		if (animationBlendTime >= animationBlendSeconds)
		{
			animationBlendTime = animationBlendSeconds;
		}
		blendRate = animationBlendTime / animationBlendSeconds;
		blendRate *= blendRate;
	}

	//�w��̃A�j���[�V�����f�[�^���擾
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);
	if (anim)
	{
		currentAnimationSeconds = animation.secondsLength;
		anim = false;
	}

	//�A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;

	// �t�Đ�
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = keyCount - 1; keyIndex > 0; --keyIndex)
	{

		//���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex - 1);
		if (currentAnimationSeconds <= keyframe0.seconds &&
			currentAnimationSeconds > keyframe1.seconds)
		{
			//�Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
			float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

			// 12

			int nodeCount = static_cast<int>(node->now);
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				//2�̃L�[�t���[���Ԃ̕⊮�v�Z
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];

				//�u�����h�⊮����
				if (blendRate < 1.0f && blend)
				{
					//���݂̎p���Ǝ��̃L�[�t���[���Ƃ̎p���̕⊮
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

					//�v�Z���ʂ��{�[���Ɋi�[
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

				else
				{
					//�O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

					//�v�Z���ʂ��{�[���Ɋi�[
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
			}
			break;
		}
	}

	//�ŏI�t���[������
	if (animationEndFlag)
	{
		animationEndFlag = false;
		currentAnimationIndex = -1;
		anim = true;
		return;
	}

	//���Ԍo��
	currentAnimationSeconds -= elapsedTime;

	//�Đ����Ԃ��T�Z���Ԃ𒴂�����
	if (currentAnimationSeconds <= 0)
	{
		//�Đ����Ԃ������߂�
		if (animationLoopFlag)
		{
			currentAnimationSeconds += animation.secondsLength;
		}
		//�Đ��I�����Ԃɂ���
		else
		{
			currentAnimationSeconds = 0;
			animationEndFlag = true;
		}
	}
}

void Model::PlayUpeerBodyAnimation(int index, bool loop, float blendSeconds)
{
	currentAnimationIndexUpeer = index;
	currentAnimationSecondsUpeer = 0.0f;
	animationLoopFlagUpeer = loop;
	animationEndFlagUpeer = false;
	animationBlendTimeUpeer = 0.0f;
	animationBlendSecondsUpeer = blendSeconds;
}

void Model::PlayLowerBodyAnimation(int index, bool loop, float blendSeconds)
{
	currentAnimationIndex = index;
	currentAnimationSeconds = 0.0f;
	animationLoopFlag = loop;
	animationEndFlag = false;
	animationBlendTime = 0.0f;
	animationBlendSeconds = blendSeconds;
}

// �A�j���[�V�����Đ�
void Model::PlayAnimation(int index, bool loop,float currentanimationseconds,bool end, float blendSeconds)
{
	currentAnimationIndex = index;
	currentAnimationSeconds = currentanimationseconds;
	animationLoopFlag = loop;
	animationEndFlag = end;

	animationBlendTime = 0.0f;
	animationBlendSeconds = blendSeconds;
}

void Model::PlayReverseAnimation(int index, bool loop, float blendSeconds)
{
	currentAnimationIndex = index;
	currentAnimationIndex = index;
	currentAnimationSeconds = 0.0f;
	animationLoopFlag = loop;
	animationEndFlag = false;
	animationBlendTime = 0.0f;
	animationBlendSeconds = blendSeconds;
}

void Model::PlayAnimationBlend(int index,int index2, bool loop, float currentanimationseconds,float blendSeconds)
{
	currentAnimationIndex = index;
	currentAnimationIndexSeconds = index2;
	currentAnimationSeconds = currentanimationseconds;
	animationLoopFlag = loop;
	animationEndFlag = false;
	animationBlendTime = 0.0f;
	animationBlendSeconds = blendSeconds;
}

bool Model::IsPlayAnimation() const
{
	if (currentAnimationIndex < 0) return false;
	if (currentAnimationIndex >= resource->GetAnimations().size()) return false;
	return true;
}

bool Model::IsPlayUpeerBodyAnimation() const
{
	if (currentAnimationIndexUpeer < 0) return false;
	if (currentAnimationIndexUpeer >= resource->GetAnimations().size()) return false;
	return true;
}

bool Model::IsPlayAnimationBlend() const
{
	
	if (currentAnimationIndex < 0 && currentAnimationIndexSeconds < 0) return false;
	if (currentAnimationIndex >= resource->GetAnimations().size() && currentAnimationIndexSeconds >= resource->GetAnimations().size()) return false;
	return true;
}

Model::Node* Model::FindNode(const char* name)
{
	int bonenowsize = 0;
	// �S�Ẵm�[�h�𑍓�����Ŗ��O��r����
	for (Node& node : nodes)
	{
		// char���m�������Ȃ�
		// std�ȗ�::
		if (::strcmp(name , node.name) == 0)
		{
			node.now = bonenowsize;
			return &node;
		}
		++bonenowsize;
	}

	// ������Ȃ�����
	return nullptr;
}

int Model::GetNodeIndex(const char* name) const
{
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		if (nodes.at(nodeIndex).name == name)
		{
			return  static_cast<int>(nodeIndex);
		}
	}
	return -1;
}
