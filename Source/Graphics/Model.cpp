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

void Model::ComputeAnimation(int animationIndex, int nodeIndex, float time, NodePose& nodePose, float blendRate) 
{
	//const Animation& animation = animations.at(animationIndex);
	//const NodeAnim& nodeAnim = animation.nodeAnims.at(nodeIndex);

	//// �w��̃A�j���[�V�����f�[�^���擾
	//const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	//const ModelResource::Animation& animation = animations.at(currentAnimationIndex);



		// �w��̃A�j���[�V�����f�[�^���擾
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(animationIndex);

	// �A�j���[�V�����f�[�^����L�[�t���[���f�[�^���X�g���擾
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int> (keyframes.size());

	//int keyCount = static_cast<int> (keyframes.size());
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

			//int nodeCount = static_cast<int>(nodes.size());
			//for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			//{
				//2�̃L�[�t���[���Ԃ̕⊮�v�Z
			const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
			const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

			Node& node = nodes[nodeIndex];
			// �u�����h�v�Z
			if (blendRate < 1.0f)
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



			break;
		}
	}



	//// �ʒu
	//for (size_t index = 0; index < keyCount - 1; ++index)
	//{
	//	// ���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
	//	ModelResource::Keyframe& keyframe0 = nodeAnim.positionKeyframes.at(index);
	//	ModelResource::Keyframe& keyframe1 = nodeAnim.positionKeyframes.at(index + 1);
	//	if (time >= keyframe0.seconds && time <= keyframe1.seconds)
	//	{
	//		// �Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
	//		float rate = (time - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

	//		// �O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
	//		DirectX::XMVECTOR V0 = DirectX::XMLoadFloat3(&keyframe0.value);
	//		DirectX::XMVECTOR V1 = DirectX::XMLoadFloat3(&keyframe1.value);
	//		DirectX::XMVECTOR V = DirectX::XMVectorLerp(V0, V1, rate);
	//		// �v�Z���ʂ��m�[�h�Ɋi�[
	//		DirectX::XMStoreFloat3(&nodePose.position, V);
	//	}
	//}
	//// ��]
	//for (size_t index = 0; index < nodeAnim.rotationKeyframes.size() - 1; ++index)
	//{
	//	// ���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
	//	const QuaternionKeyframe& keyframe0 = nodeAnim.rotationKeyframes.at(index);
	//	const QuaternionKeyframe& keyframe1 = nodeAnim.rotationKeyframes.at(index + 1);
	//	if (time >= keyframe0.seconds && time <= keyframe1.seconds)
	//	{
	//		// �Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
	//		float rate = (time - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

	//		// �O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
	//		DirectX::XMVECTOR Q0 = DirectX::XMLoadFloat4(&keyframe0.value);
	//		DirectX::XMVECTOR Q1 = DirectX::XMLoadFloat4(&keyframe1.value);
	//		DirectX::XMVECTOR Q = DirectX::XMQuaternionSlerp(Q0, Q1, rate);
	//		// �v�Z���ʂ��m�[�h�Ɋi�[
	//		DirectX::XMStoreFloat4(&nodePose.rotation, Q);
	//	}
	//}
	//// �X�P�[��
	//for (size_t index = 0; index < nodeAnim.scaleKeyframes.size() - 1; ++index)
	//{
	//	// ���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩���肷��
	//	const VectorKeyframe& keyframe0 = nodeAnim.scaleKeyframes.at(index);
	//	const VectorKeyframe& keyframe1 = nodeAnim.scaleKeyframes.at(index + 1);
	//	if (time >= keyframe0.seconds && time <= keyframe1.seconds)
	//	{
	//		// �Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
	//		float rate = (time - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

	//		// �O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
	//		DirectX::XMVECTOR V0 = DirectX::XMLoadFloat3(&keyframe0.value);
	//		DirectX::XMVECTOR V1 = DirectX::XMLoadFloat3(&keyframe1.value);
	//		DirectX::XMVECTOR V = DirectX::XMVectorLerp(V0, V1, rate);
	//		// �v�Z���ʂ��m�[�h�Ɋi�[
	//		DirectX::XMStoreFloat3(&nodePose.scale, V);
	//	}
	//}
}

void Model::ComputeAnimation(int animationIndex, float time, std::vector<NodePose>& nodePoses, float blendRate) 
{

	if (nodePoses.size() != nodes.size())
	{
		nodePoses.resize(nodes.size());
	}
	for (size_t nodeIndex = 0; nodeIndex < nodePoses.size(); ++nodeIndex)
	{
		ComputeAnimation(animationIndex, static_cast<int>(nodeIndex), time, nodePoses.at(nodeIndex), blendRate);
	}
}

void Model::UpdateRootMortion(float elapsedTime)
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

	// �w�莞�Ԃ̃A�j���[�V�����̎p�����擾
	ComputeAnimation(currentAnimationIndex, currentAnimationSeconds, nodePoses, blendRate);

	// TODO�@:���[�g���[�V�����������s���A�L�����N�^�[���ړ�����
	{
		// ���[�g���[�V�����m�[�h�ԍ��擾 �����ԍ�
		const int rootMotionNodeIndex = GetNodeIndex("B_Pelvis");

		// ����A�O��A����̃��[�g���[�V�����̎p�����擾
		Model::NodePose beginPose, oldPose, newPose;
		ComputeAnimation(currentAnimationIndex, rootMotionNodeIndex, 0.0f, beginPose, blendRate);
		ComputeAnimation(currentAnimationIndex, rootMotionNodeIndex, oldcurrentAnimationSeconds, oldPose, blendRate);
		ComputeAnimation(currentAnimationIndex, rootMotionNodeIndex, currentAnimationSeconds, newPose, blendRate);


		DirectX::XMFLOAT3 localTranslation;
		DirectX::XMFLOAT3 movepos;

		if (oldcurrentAnimationSeconds > currentAnimationSeconds)
		{
			// TODO�A:���[�v�A�j���[�V�����ɑΉ�����

			// �I�[�̎p�����擾
			Model::NodePose rastPose;
			ComputeAnimation(currentAnimationIndex, rootMotionNodeIndex, rastcurrentAnimationSeconds, rastPose, blendRate);



			// ���[�J���ړ��l���Z�o
			movepos.x = oldPose.position.x - rastPose.position.x;
			movepos.y = oldPose.position.y - rastPose.position.y;
			movepos.z = oldPose.position.z - rastPose.position.z;
			// ���[�J���ړ��l���Z�o
			movepos.x += beginPose.position.x - newPose.position.x;
			movepos.y += beginPose.position.y - newPose.position.y;
			movepos.z += beginPose.position.z - newPose.position.z;

			localTranslation.x = movepos.x;
			localTranslation.y = movepos.y;
			localTranslation.z = movepos.z;

		}
		else
		{
			// ���[�J���ړ��l���擾

			localTranslation.x = newPose.position.x - oldPose.position.x;
			localTranslation.y = newPose.position.y - oldPose.position.y;
			localTranslation.z = newPose.position.z - oldPose.position.z;


		}
	

		// �O���[�o���ړ��l���Z�o
		Model::Node& rootMotionNode = nodes[rootMotionNodeIndex];
		DirectX::XMVECTOR LocalTranslation = DirectX::XMLoadFloat3(&localTranslation);
	//	// �e�̃O���[�o���s��
	//	DirectX::XMMATRIX ParentGlobalTransform = DirectX::XMLoadFloat4x4(&rootMotionNode.parent->globalTransform);
	//	// ���[�J�����O���[�o����
	//	DirectX::XMVECTOR GlobalTranslation = DirectX::XMVector3TransformNormal(LocalTranslation, ParentGlobalTransform);


	//	//if (bakeTranslationY)
	//	//{
	//	//	DirectX::XMFLOAT3 globalTranslation;

	//	//	DirectX::XMStoreFloat3(&globalTranslation, GlobalTranslation);

	//	//	// TODO�B:���[�g���[�V������Y�ړ��͓K�p���Ȃ��悤�ɂ���

	//	//	// Y�����̈ړ��l�𔲂�
	//	//	globalTranslation.y = 0;
	//	//	// ����̎p���̃O���[�o���ʒu���Z�o

	//	//	// XZ�������폜
	//	//	//globalTranslation.x = 0;
	//	//	//globalTranslation.z = 0;

	//	//	GlobalTranslation = DirectX::XMLoadFloat3(&globalTranslation);
	//	//	// ���[�J����ԕϊ�

	//	//	// ���[�g���[�V�����m�[�h�̈ʒu��ݒ�


	//	//}
	//	//else
	//	{
	//		// �ړ���ɃO���[�o�����W�����̈ʒu�ɖ߂�̂�h�����߂�
	//		// ���[�g���[�V�����m�[�h������̎p���ɂ���
	//		nodePoses[rootMotionNodeIndex].position = beginPose.position;
	//	}
	//	// ���[���h�ړ��l���Z�o
	//	DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&worldTransform);
	//	DirectX::XMVECTOR WorldTranslation = DirectX::XMVector3TransformNormal(GlobalTranslation, WorldTransform);
	//	DirectX::XMFLOAT3 worldTranslation;
	//	DirectX::XMStoreFloat3(&worldTranslation, WorldTranslation);


	//	// �L�����N�^�[�̈ʒu���X�V
	//	position.x += worldTranslation.x;
	//	position.y += worldTranslation.y;
	//	position.z += worldTranslation.z;
	}

	// �A�j���[�V�������ԍX�V
	oldcurrentAnimationSeconds = currentAnimationSeconds;
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


	// �p���X�V
	SetNodePoses(nodePoses);


//// ���[���h�g�����X�t�H�[���X�V
//DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
//DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
//DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
//DirectX::XMMATRIX WorldTransform = S * R * T;
//DirectX::XMStoreFloat4x4(&worldTransform, WorldTransform);


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
	// �L�[�t���[���̍ő�l
	int keyCount = 
		static_cast<int>(keyFrameEnd) <= 0 ? static_cast<int> (keyframes.size()) : static_cast<int>(keyFrameEnd);
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



	// ���Ԍo�� ���Z���A�j���[�V�����𑁂߂邩�ǂ���
	currentAnimationSeconds += currentAnimationAddSeconds > currentAnimationAddSecondsMin ? currentAnimationAddSeconds : elapsedTime;

	// �ʏ�ő�l
	float animSecondsLength = animation.secondsLength;

	// �Đ����Ԃ��I�[���Ԃ𒴂�����
	// �ō��𒴂����ꍇ�̃��[�v�̏ꍇ�����B
	if (keyFrameEnd + FLT_EPSILON > 0 + FLT_EPSILON)
	{

		const ModelResource::Keyframe& keyframe = keyframes.at(keyFrameEnd);
		animSecondsLength = keyframe.seconds;
	}
	if (currentAnimationSeconds >= animSecondsLength)
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
//
//void Model::UpdateRootAnimation(float elapsedTime, char* bornName, bool blend)
//{
//
//	// �A�j���[�V�����X�V����
//	UpdateAnimation(elapsedTime, blend);
//		//const Model::Animation& animation = character->GetAnimations().at(animationIndex);
//	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
//	
//
//		// �w�莞�Ԃ̃A�j���[�V�����̎p�����擾
//		character->ComputeAnimation(animationIndex, currentAnimationSeconds, nodePoses);
//	
//		// TODO�@:���[�g���[�V�����������s���A�L�����N�^�[���ړ�����
//		{
//			// ���[�g���[�V�����m�[�h�ԍ��擾 �����ԍ�
//			//const int rootMotionNodeIndex = character->GetNodeIndex("B_Pelvis");
//			const int rootMotionNodeIndex = character->GetNodeIndex(bornName);
//
//			// ����A�O��A����̃��[�g���[�V�����̎p�����擾
//			Model::NodePose beginPose, oldPose, newPose;
//			character->ComputeAnimation(animationIndex, rootMotionNodeIndex, 0.0f, beginPose);
//			character->ComputeAnimation(animationIndex, rootMotionNodeIndex, oldAnimationSeconds, oldPose);
//			character->ComputeAnimation(animationIndex, rootMotionNodeIndex, animationSeconds, newPose);
//
//
//			DirectX::XMFLOAT3 localTranslation;
//			DirectX::XMFLOAT3 movepos;
//
//			if (oldAnimationSeconds > animationSeconds)
//			{
//				// TODO�A:���[�v�A�j���[�V�����ɑΉ�����
//
//				// �I�[�̎p�����擾
//				Model::NodePose rastPose;
//				character->ComputeAnimation(animationIndex, rootMotionNodeIndex, rastAnimationSeconds, rastPose);
//
//
//
//				// ���[�J���ړ��l���Z�o
//				movepos.x = oldPose.position.x - rastPose.position.x;
//				movepos.y = oldPose.position.y - rastPose.position.y;
//				movepos.z = oldPose.position.z - rastPose.position.z;
//				// ���[�J���ړ��l���Z�o
//				movepos.x += beginPose.position.x - newPose.position.x;
//				movepos.y += beginPose.position.y - newPose.position.y;
//				movepos.z += beginPose.position.z - newPose.position.z;
//
//				localTranslation.x = movepos.x;
//				localTranslation.y = movepos.y;
//				localTranslation.z = movepos.z;
//
//			}
//			else
//			{
//				// ���[�J���ړ��l���擾
//
//				localTranslation.x = newPose.position.x - oldPose.position.x;
//				localTranslation.y = newPose.position.y - oldPose.position.y;
//				localTranslation.z = newPose.position.z - oldPose.position.z;
//
//
//			}
//
//
//			// �O���[�o���ړ��l���Z�o
//			Model::Node& rootMotionNode = character->GetNodes().at(rootMotionNodeIndex);
//			DirectX::XMVECTOR LocalTranslation = DirectX::XMLoadFloat3(&localTranslation);
//			// �e�̃O���[�o���s��
//			DirectX::XMMATRIX ParentGlobalTransform = DirectX::XMLoadFloat4x4(&rootMotionNode.parent->globalTransform);
//			// ���[�J�����O���[�o����
//			DirectX::XMVECTOR GlobalTranslation = DirectX::XMVector3TransformNormal(LocalTranslation, ParentGlobalTransform);
//
//
//			if (bakeTranslationY)
//			{
//				DirectX::XMFLOAT3 globalTranslation;
//
//				DirectX::XMStoreFloat3(&globalTranslation, GlobalTranslation);
//
//				// TODO�B:���[�g���[�V������Y�ړ��͓K�p���Ȃ��悤�ɂ���
//
//				// Y�����̈ړ��l�𔲂�
//				globalTranslation.y = 0;
//				// ����̎p���̃O���[�o���ʒu���Z�o
//
//				// XZ�������폜
//				//globalTranslation.x = 0;
//				//globalTranslation.z = 0;
//
//				GlobalTranslation = DirectX::XMLoadFloat3(&globalTranslation);
//				// ���[�J����ԕϊ�
//
//				// ���[�g���[�V�����m�[�h�̈ʒu��ݒ�
//
//
//			}
//			else
//			{
//				// �ړ���ɃO���[�o�����W�����̈ʒu�ɖ߂�̂�h�����߂�
//				// ���[�g���[�V�����m�[�h������̎p���ɂ���
//				nodePoses[rootMotionNodeIndex].position = beginPose.position;
//			}
//			// ���[���h�ړ��l���Z�o
//			DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&worldTransform);
//			DirectX::XMVECTOR WorldTranslation = DirectX::XMVector3TransformNormal(GlobalTranslation, WorldTransform);
//			DirectX::XMFLOAT3 worldTranslation;
//			DirectX::XMStoreFloat3(&worldTranslation, WorldTranslation);
//
//
//			// �L�����N�^�[�̈ʒu���X�V
//			position.x += worldTranslation.x;
//			position.y += worldTranslation.y;
//			position.z += worldTranslation.z;
//		}
//
//		// �A�j���[�V�������ԍX�V
//		oldAnimationSeconds = animationSeconds;
//		animationSeconds += elapsedTime;
//		if (animationSeconds > animation.secondsLength)
//		{
//			if (animationLoop)
//			{
//				animationSeconds -= animation.secondsLength;
//			}
//			else
//			{
//				animationSeconds = animation.secondsLength;
//			}
//		}
//
//		// �p���X�V
//		character->SetNodePoses(nodePoses);
//	
//
//	// ���[���h�g�����X�t�H�[���X�V
//	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
//	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
//	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
//	DirectX::XMMATRIX WorldTransform = S * R * T;
//	DirectX::XMStoreFloat4x4(&worldTransform, WorldTransform);
//
//	// ���f���g�����X�t�H�[���X�V����
//	character->UpdateTransform(worldTransform);
//}

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
	int keyCount =
		static_cast<int>(keyFrameEnd) <= 0 ? static_cast<int> (keyframes.size()) : static_cast<int>(keyFrameEnd);
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


	//float animationKeyFrames;
	
	//DirectX::XMStoreFloat(&animationKeyFrames, DirectX::XMVector3Normalize(DirectX::XMLoadFloat(&animation.keyframes)));
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	const std::vector<ModelResource::Keyframe>& keyframesSeconds = animationSeconds.keyframes;



	int keyCount =
		static_cast<int>(keyFrameEnd) <= 0 ? static_cast<int> (keyframes.size()) : static_cast<int>(keyFrameEnd);

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
	// �ʏ�ő�l
	float animSecondsLength = animation.secondsLength;

	// �Đ����Ԃ��I�[���Ԃ𒴂�����
	// �ō��𒴂����ꍇ�̃��[�v�̏ꍇ�����B
	if (keyFrameEnd + FLT_EPSILON > 0 + FLT_EPSILON)
	{

		const ModelResource::Keyframe& keyframe = keyframes.at(keyFrameEnd);
		animSecondsLength = keyframe.seconds;
	}
	if (currentAnimationSeconds >= animSecondsLength)
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
	//int nodeStart = GetNodeIndex(start);
	//int nodeEnd = GetNodeIndex(end);

	//Model::Node* node = FindNode(start);
	Model::Node* startNode = FindNode(start);
	Model::Node* endNode = FindNode(end);

	//int nodeIndexStart = node->now;
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
	int keyCount =
		static_cast<int> (keyFrameEndUpper) <= 0 ? static_cast<int> (keyframes.size()) : static_cast<int> (keyFrameEndUpper);
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
			
			int nodeIndexMax = static_cast<int>(endNode->now);
			for (int nodeIndex = startNode->now; nodeIndex < nodeIndexMax; ++nodeIndex)
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
	// ���Ԍo�� ���Z���A�j���[�V�����𑁂߂邩�ǂ���
	currentAnimationSecondsUpeer += currentAnimationUpperAddSeconds > currentAnimationUpperAddSecondsMin ? currentAnimationUpperAddSeconds : elapsedTime;
	
	// �ʏ�ő�l
	float animSecondsLength = animation.secondsLength;

	// �Đ����Ԃ��I�[���Ԃ𒴂�����
	// �ō��𒴂����ꍇ�̃��[�v�̏ꍇ�����B
	if (keyFrameEnd + FLT_EPSILON > 0 + FLT_EPSILON)
	{

		const ModelResource::Keyframe& keyframe = keyframes.at(keyFrameEnd);
		animSecondsLength = keyframe.seconds;
	}
	
	//�Đ����Ԃ��T�Z���Ԃ𒴂�����
	if (currentAnimationSecondsUpeer >= animSecondsLength)
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

void Model::ReverseplaybackUpeerBodyAnimation(float elapsedTime,  const char* end, bool blend)
{

	Model::Node* node = FindNode(end);

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
			//int nodeCount = static_cast<int>(endNode->now);
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
void Model::UpdateLowerBodyAnimation(float elapsedTime,const char* start, const char* end, bool blend)
{
	Model::Node* startNode = FindNode(start);
	Model::Node* endNode = FindNode(end);
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
	int keyCount = static_cast<int>(keyFrameEnd) <= 0 ? static_cast<int>(keyframes.size()): static_cast<int>(keyFrameEnd);
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

			int nodeCount = static_cast<int>(endNode->now);
			for (int nodeIndex = startNode->now; nodeIndex < nodeCount; ++nodeIndex)
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

	// �ʏ�ő�l
	float animSecondsLength = animation.secondsLength;

	// �Đ����Ԃ��I�[���Ԃ𒴂�����
	// �ō��𒴂����ꍇ�̃��[�v�̏ꍇ�����B
	if (keyFrameEnd + FLT_EPSILON > 0 + FLT_EPSILON)
	{

		const ModelResource::Keyframe& keyframe = keyframes.at(keyFrameEnd);
		animSecondsLength = keyframe.seconds;
	}
	if (currentAnimationSeconds >= animSecondsLength)
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

void Model::PlayUpeerBodyAnimation(int index, bool loop, float currentanimationseconds, float blendSeconds, float currentAnimationUpperAddSeconds, float keyFrameEndUpper)
{
	currentAnimationIndexUpeer = index;
	currentAnimationSecondsUpeer = currentanimationseconds;
	animationLoopFlagUpeer = loop;
	animationEndFlagUpeer = false;
	animationBlendTimeUpeer = 0.0f;
	animationBlendSecondsUpeer = blendSeconds;
	this->currentAnimationUpperAddSeconds = currentAnimationUpperAddSeconds;

	this->keyFrameEndUpper = keyFrameEndUpper;
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
void Model::PlayAnimation(int index, bool loop,float currentanimationseconds,float blendSeconds, float currentAnimationAddSeconds, float keyFrameEnd)
{
	currentAnimationIndex = index;
	currentAnimationSeconds = oldcurrentAnimationSeconds = currentanimationseconds;
	animationLoopFlag = loop;
	animationEndFlag = false;

	animationBlendTime = 0.0f;
	animationBlendSeconds = blendSeconds;

	this->currentAnimationAddSeconds = currentAnimationAddSeconds;

	this->keyFrameEnd = keyFrameEnd;
}

void Model::PlayReverseAnimation(int index, bool loop,float currentanimationseconds, float blendSeconds, float keyFrameEnd)
{
	//currentAnimationIndex = index;
	currentAnimationIndex = index;
	currentAnimationSeconds = currentanimationseconds;
	animationLoopFlag = loop;
	animationEndFlag = false;
	animationBlendTime = 0.0f;
	animationBlendSeconds = blendSeconds;

	this->keyFrameEnd = keyFrameEnd;
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

void Model::SetNodePoses(const std::vector<NodePose>& nodePoses)
{
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		const NodePose& pose = nodePoses.at(nodeIndex);
		Node& node = nodes.at(nodeIndex);

		node.position = pose.position;
		node.rotation = pose.rotation;
		node.scale = pose.scale;
	}
}
