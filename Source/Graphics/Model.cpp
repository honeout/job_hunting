#include <string.h>
#include "Graphics/Graphics.h"
#include "Graphics/Model.h"

#include "ResourceManager.h"

// コンストラクタ
Model::Model(const char* filename)
{
	// リソース読み込み
	//resource = std::make_shared<ModelResource>();
	//resource->Load(Graphics::Instance().GetDevice(), filename);
	resource = ResourceManager::Instance().LoadModelResource(filename);

	// ノード
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

	// 行列計算
	const DirectX::XMFLOAT4X4 transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	UpdateTransform(transform);
}

// 変換行列計算
void Model::UpdateTransform(const DirectX::XMFLOAT4X4& transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);


	for (Node& node : nodes)
	{
		// ローカル行列算出
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ワールド行列算出
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

		// 計算結果を格納
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
		// ローカル行列算出
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// ワールド行列算出
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

		// 計算結果を格納
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}

void Model::ComputeAnimation(int animationIndex, int nodeIndex, float time, NodePose& nodePose, float blendRate) 
{
	//const Animation& animation = animations.at(animationIndex);
	//const NodeAnim& nodeAnim = animation.nodeAnims.at(nodeIndex);

	//// 指定のアニメーションデータを取得
	//const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	//const ModelResource::Animation& animation = animations.at(currentAnimationIndex);



		// 指定のアニメーションデータを取得
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(animationIndex);

	// アニメーションデータからキーフレームデータリストを取得
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int> (keyframes.size());

	//int keyCount = static_cast<int> (keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		// 現在の時間がどのキーフレームの間にいるか判定する
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
		if (currentAnimationSeconds >= keyframe0.seconds &&
			currentAnimationSeconds < keyframe1.seconds)
		{
			// 再生時間とキーフレームの時間から補完率を算出する
			// 分母に現在のアニメーションとキーフレームのーことで、分子に１と０をすることで０．何パーセント貰える
			// つまり疑似的％が貰える。
			float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds
				- keyframe0.seconds);

			//int nodeCount = static_cast<int>(nodes.size());
			//for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			//{
				//2つのキーフレーム間の補完計算
			const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
			const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

			Node& node = nodes[nodeIndex];
			// ブレンド計算
			if (blendRate < 1.0f)
			{
				DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
				DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
				DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
				DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
				DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
				DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

				// キーフレームの割合を求める。
				DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
				DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
				DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);
				// 計算結果をボーンに格納
				DirectX::XMStoreFloat3(&node.scale, S);
				DirectX::XMStoreFloat4(&node.rotate, R);
				DirectX::XMStoreFloat3(&node.translate, T);
			}
			// 通常の計算

			else
			{
				DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
				DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
				DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
				DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
				DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
				DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

				// キーフレームの割合を求める。
				DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
				DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
				DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);
				// 計算結果をボーンに格納
				DirectX::XMStoreFloat3(&node.scale, S);
				DirectX::XMStoreFloat4(&node.rotate, R);
				DirectX::XMStoreFloat3(&node.translate, T);
			}



			break;
		}
	}



	//// 位置
	//for (size_t index = 0; index < keyCount - 1; ++index)
	//{
	//	// 現在の時間がどのキーフレームの間にいるか判定する
	//	ModelResource::Keyframe& keyframe0 = nodeAnim.positionKeyframes.at(index);
	//	ModelResource::Keyframe& keyframe1 = nodeAnim.positionKeyframes.at(index + 1);
	//	if (time >= keyframe0.seconds && time <= keyframe1.seconds)
	//	{
	//		// 再生時間とキーフレームの時間から補完率を算出する
	//		float rate = (time - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

	//		// 前のキーフレームと次のキーフレームの姿勢を補完
	//		DirectX::XMVECTOR V0 = DirectX::XMLoadFloat3(&keyframe0.value);
	//		DirectX::XMVECTOR V1 = DirectX::XMLoadFloat3(&keyframe1.value);
	//		DirectX::XMVECTOR V = DirectX::XMVectorLerp(V0, V1, rate);
	//		// 計算結果をノードに格納
	//		DirectX::XMStoreFloat3(&nodePose.position, V);
	//	}
	//}
	//// 回転
	//for (size_t index = 0; index < nodeAnim.rotationKeyframes.size() - 1; ++index)
	//{
	//	// 現在の時間がどのキーフレームの間にいるか判定する
	//	const QuaternionKeyframe& keyframe0 = nodeAnim.rotationKeyframes.at(index);
	//	const QuaternionKeyframe& keyframe1 = nodeAnim.rotationKeyframes.at(index + 1);
	//	if (time >= keyframe0.seconds && time <= keyframe1.seconds)
	//	{
	//		// 再生時間とキーフレームの時間から補完率を算出する
	//		float rate = (time - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

	//		// 前のキーフレームと次のキーフレームの姿勢を補完
	//		DirectX::XMVECTOR Q0 = DirectX::XMLoadFloat4(&keyframe0.value);
	//		DirectX::XMVECTOR Q1 = DirectX::XMLoadFloat4(&keyframe1.value);
	//		DirectX::XMVECTOR Q = DirectX::XMQuaternionSlerp(Q0, Q1, rate);
	//		// 計算結果をノードに格納
	//		DirectX::XMStoreFloat4(&nodePose.rotation, Q);
	//	}
	//}
	//// スケール
	//for (size_t index = 0; index < nodeAnim.scaleKeyframes.size() - 1; ++index)
	//{
	//	// 現在の時間がどのキーフレームの間にいるか判定する
	//	const VectorKeyframe& keyframe0 = nodeAnim.scaleKeyframes.at(index);
	//	const VectorKeyframe& keyframe1 = nodeAnim.scaleKeyframes.at(index + 1);
	//	if (time >= keyframe0.seconds && time <= keyframe1.seconds)
	//	{
	//		// 再生時間とキーフレームの時間から補完率を算出する
	//		float rate = (time - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

	//		// 前のキーフレームと次のキーフレームの姿勢を補完
	//		DirectX::XMVECTOR V0 = DirectX::XMLoadFloat3(&keyframe0.value);
	//		DirectX::XMVECTOR V1 = DirectX::XMLoadFloat3(&keyframe1.value);
	//		DirectX::XMVECTOR V = DirectX::XMVectorLerp(V0, V1, rate);
	//		// 計算結果をノードに格納
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

	// 再生中でないなら処理しない
	if (!IsPlayAnimation())return;

	// ブレンド率の計算
	float blendRate = 1.0f;
	// 現在のブレンドが時間より下なら
	if (animationBlendTime < animationBlendSeconds)
	{
		// 足している。
		animationBlendTime += elapsedTime;
		if (animationBlendTime >= animationBlendSeconds)
		{
			animationBlendTime = animationBlendSeconds;
		}
		blendRate = animationBlendTime / animationBlendSeconds;
		// 二条してひづみを消す。
		blendRate *= blendRate;
	}

	// 指定のアニメーションデータを取得
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);

	// 指定時間のアニメーションの姿勢を取得
	ComputeAnimation(currentAnimationIndex, currentAnimationSeconds, nodePoses, blendRate);

	// TODO①:ルートモーション処理を行い、キャラクターを移動せよ
	{
		// ルートモーションノード番号取得 腰骨番号
		const int rootMotionNodeIndex = GetNodeIndex("B_Pelvis");

		// 初回、前回、今回のルートモーションの姿勢を取得
		Model::NodePose beginPose, oldPose, newPose;
		ComputeAnimation(currentAnimationIndex, rootMotionNodeIndex, 0.0f, beginPose, blendRate);
		ComputeAnimation(currentAnimationIndex, rootMotionNodeIndex, oldcurrentAnimationSeconds, oldPose, blendRate);
		ComputeAnimation(currentAnimationIndex, rootMotionNodeIndex, currentAnimationSeconds, newPose, blendRate);


		DirectX::XMFLOAT3 localTranslation;
		DirectX::XMFLOAT3 movepos;

		if (oldcurrentAnimationSeconds > currentAnimationSeconds)
		{
			// TODO②:ループアニメーションに対応せよ

			// 終端の姿勢を取得
			Model::NodePose rastPose;
			ComputeAnimation(currentAnimationIndex, rootMotionNodeIndex, rastcurrentAnimationSeconds, rastPose, blendRate);



			// ローカル移動値を算出
			movepos.x = oldPose.position.x - rastPose.position.x;
			movepos.y = oldPose.position.y - rastPose.position.y;
			movepos.z = oldPose.position.z - rastPose.position.z;
			// ローカル移動値を算出
			movepos.x += beginPose.position.x - newPose.position.x;
			movepos.y += beginPose.position.y - newPose.position.y;
			movepos.z += beginPose.position.z - newPose.position.z;

			localTranslation.x = movepos.x;
			localTranslation.y = movepos.y;
			localTranslation.z = movepos.z;

		}
		else
		{
			// ローカル移動値を取得

			localTranslation.x = newPose.position.x - oldPose.position.x;
			localTranslation.y = newPose.position.y - oldPose.position.y;
			localTranslation.z = newPose.position.z - oldPose.position.z;


		}
	

		// グローバル移動値を算出
		Model::Node& rootMotionNode = nodes[rootMotionNodeIndex];
		DirectX::XMVECTOR LocalTranslation = DirectX::XMLoadFloat3(&localTranslation);
	//	// 親のグローバル行列
	//	DirectX::XMMATRIX ParentGlobalTransform = DirectX::XMLoadFloat4x4(&rootMotionNode.parent->globalTransform);
	//	// ローカルをグローバルに
	//	DirectX::XMVECTOR GlobalTranslation = DirectX::XMVector3TransformNormal(LocalTranslation, ParentGlobalTransform);


	//	//if (bakeTranslationY)
	//	//{
	//	//	DirectX::XMFLOAT3 globalTranslation;

	//	//	DirectX::XMStoreFloat3(&globalTranslation, GlobalTranslation);

	//	//	// TODO③:ルートモーションのY移動は適用しないようにせよ

	//	//	// Y成分の移動値を抜く
	//	//	globalTranslation.y = 0;
	//	//	// 今回の姿勢のグローバル位置を算出

	//	//	// XZ成分を削除
	//	//	//globalTranslation.x = 0;
	//	//	//globalTranslation.z = 0;

	//	//	GlobalTranslation = DirectX::XMLoadFloat3(&globalTranslation);
	//	//	// ローカル空間変換

	//	//	// ルートモーションノードの位置を設定


	//	//}
	//	//else
	//	{
	//		// 移動後にグローバル座標が元の位置に戻るのを防ぐために
	//		// ルートモーションノードを初回の姿勢にする
	//		nodePoses[rootMotionNodeIndex].position = beginPose.position;
	//	}
	//	// ワールド移動値を算出
	//	DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&worldTransform);
	//	DirectX::XMVECTOR WorldTranslation = DirectX::XMVector3TransformNormal(GlobalTranslation, WorldTransform);
	//	DirectX::XMFLOAT3 worldTranslation;
	//	DirectX::XMStoreFloat3(&worldTranslation, WorldTranslation);


	//	// キャラクターの位置を更新
	//	position.x += worldTranslation.x;
	//	position.y += worldTranslation.y;
	//	position.z += worldTranslation.z;
	}

	// アニメーション時間更新
	oldcurrentAnimationSeconds = currentAnimationSeconds;
	// 時間経過
	currentAnimationSeconds += elapsedTime;

	// 再生時間が終端時間を超えたら
	// 最高を超えた場合のループの場合を作る。
	if (currentAnimationSeconds >= animation.secondsLength)
	{
		// ループならトゥルー
		if (animationLoopFlag)
		{
			// 再生時間が終端時間を超えたら


				// 何でー＝オーバーした分の事を考え引く事でがくとしない。
				// 再生時価を巻き戻す
			currentAnimationSeconds -= animation.secondsLength;

		}
		else
		{
			// 再生時間が終端時間を超えたら

				// 何でー＝オーバーした分の事を考え引く事でがくとしない。
				// 再生時価を巻き戻す
			// 一番最後にオーバーしたら止める。
			currentAnimationSeconds = animation.secondsLength;
			// 終わりました。
			animationEndFlag = true;
		}
	}


	// 姿勢更新
	SetNodePoses(nodePoses);


//// ワールドトランスフォーム更新
//DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
//DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
//DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
//DirectX::XMMATRIX WorldTransform = S * R * T;
//DirectX::XMStoreFloat4x4(&worldTransform, WorldTransform);


}


void Model::UpdateAnimation(float elapsedTime, bool blend)
{
	// 再生中でないなら処理しない
	if (!IsPlayAnimation())return;

	// ブレンド率の計算
	float blendRate = 1.0f;
	// 現在のブレンドが時間より下なら
	if (animationBlendTime < animationBlendSeconds)
	{
		// 足している。
		animationBlendTime += elapsedTime;
		if (animationBlendTime >= animationBlendSeconds)
		{
			animationBlendTime = animationBlendSeconds;
		}
		blendRate = animationBlendTime / animationBlendSeconds;
		// 二条してひづみを消す。
		blendRate *= blendRate;
	}


	// 指定のアニメーションデータを取得
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);

	// アニメーションデータからキーフレームデータリストを取得
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	// キーフレームの最大値
	int keyCount = 
		static_cast<int>(keyFrameEnd) <= 0 ? static_cast<int> (keyframes.size()) : static_cast<int>(keyFrameEnd);
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		// 現在の時間がどのキーフレームの間にいるか判定する
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
		if (currentAnimationSeconds >= keyframe0.seconds &&
			currentAnimationSeconds < keyframe1.seconds)
		{
			// 再生時間とキーフレームの時間から補完率を算出する
			// 分母に現在のアニメーションとキーフレームのーことで、分子に１と０をすることで０．何パーセント貰える
			// つまり疑似的％が貰える。
			float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds
				- keyframe0.seconds);

			int nodeCount = static_cast<int>(nodes.size());
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				//2つのキーフレーム間の補完計算
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];
				// ブレンド計算
				if (blendRate < 1.0f && blend)
				{
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					// キーフレームの割合を求める。
					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);
					// 計算結果をボーンに格納
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
				// 通常の計算

				else
				{
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					// キーフレームの割合を求める。
					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);
					// 計算結果をボーンに格納
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

			}

			break;
		}
	}

	// 最終フレーム処理
	if (animationEndFlag)
	{
		animationEndFlag = false;
		currentAnimationIndex = -1;
		return;
	}



	// 時間経過 加算分アニメーションを早めるかどうか
	currentAnimationSeconds += currentAnimationAddSeconds > currentAnimationAddSecondsMin ? currentAnimationAddSeconds : elapsedTime;

	// 通常最大値
	float animSecondsLength = animation.secondsLength;

	// 再生時間が終端時間を超えたら
	// 最高を超えた場合のループの場合を作る。
	if (keyFrameEnd + FLT_EPSILON > 0 + FLT_EPSILON)
	{

		const ModelResource::Keyframe& keyframe = keyframes.at(keyFrameEnd);
		animSecondsLength = keyframe.seconds;
	}
	if (currentAnimationSeconds >= animSecondsLength)
	{
		// ループならトゥルー
		if (animationLoopFlag)
		{
			// 再生時間が終端時間を超えたら


				// 何でー＝オーバーした分の事を考え引く事でがくとしない。
				// 再生時価を巻き戻す
			currentAnimationSeconds -= animation.secondsLength;

		}
		else
		{
			// 再生時間が終端時間を超えたら

				// 何でー＝オーバーした分の事を考え引く事でがくとしない。
				// 再生時価を巻き戻す
			// 一番最後にオーバーしたら止める。
			currentAnimationSeconds = animation.secondsLength;
			// 終わりました。
			animationEndFlag = true;
		}
	}




}
//
//void Model::UpdateRootAnimation(float elapsedTime, char* bornName, bool blend)
//{
//
//	// アニメーション更新処理
//	UpdateAnimation(elapsedTime, blend);
//		//const Model::Animation& animation = character->GetAnimations().at(animationIndex);
//	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
//	
//
//		// 指定時間のアニメーションの姿勢を取得
//		character->ComputeAnimation(animationIndex, currentAnimationSeconds, nodePoses);
//	
//		// TODO①:ルートモーション処理を行い、キャラクターを移動せよ
//		{
//			// ルートモーションノード番号取得 腰骨番号
//			//const int rootMotionNodeIndex = character->GetNodeIndex("B_Pelvis");
//			const int rootMotionNodeIndex = character->GetNodeIndex(bornName);
//
//			// 初回、前回、今回のルートモーションの姿勢を取得
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
//				// TODO②:ループアニメーションに対応せよ
//
//				// 終端の姿勢を取得
//				Model::NodePose rastPose;
//				character->ComputeAnimation(animationIndex, rootMotionNodeIndex, rastAnimationSeconds, rastPose);
//
//
//
//				// ローカル移動値を算出
//				movepos.x = oldPose.position.x - rastPose.position.x;
//				movepos.y = oldPose.position.y - rastPose.position.y;
//				movepos.z = oldPose.position.z - rastPose.position.z;
//				// ローカル移動値を算出
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
//				// ローカル移動値を取得
//
//				localTranslation.x = newPose.position.x - oldPose.position.x;
//				localTranslation.y = newPose.position.y - oldPose.position.y;
//				localTranslation.z = newPose.position.z - oldPose.position.z;
//
//
//			}
//
//
//			// グローバル移動値を算出
//			Model::Node& rootMotionNode = character->GetNodes().at(rootMotionNodeIndex);
//			DirectX::XMVECTOR LocalTranslation = DirectX::XMLoadFloat3(&localTranslation);
//			// 親のグローバル行列
//			DirectX::XMMATRIX ParentGlobalTransform = DirectX::XMLoadFloat4x4(&rootMotionNode.parent->globalTransform);
//			// ローカルをグローバルに
//			DirectX::XMVECTOR GlobalTranslation = DirectX::XMVector3TransformNormal(LocalTranslation, ParentGlobalTransform);
//
//
//			if (bakeTranslationY)
//			{
//				DirectX::XMFLOAT3 globalTranslation;
//
//				DirectX::XMStoreFloat3(&globalTranslation, GlobalTranslation);
//
//				// TODO③:ルートモーションのY移動は適用しないようにせよ
//
//				// Y成分の移動値を抜く
//				globalTranslation.y = 0;
//				// 今回の姿勢のグローバル位置を算出
//
//				// XZ成分を削除
//				//globalTranslation.x = 0;
//				//globalTranslation.z = 0;
//
//				GlobalTranslation = DirectX::XMLoadFloat3(&globalTranslation);
//				// ローカル空間変換
//
//				// ルートモーションノードの位置を設定
//
//
//			}
//			else
//			{
//				// 移動後にグローバル座標が元の位置に戻るのを防ぐために
//				// ルートモーションノードを初回の姿勢にする
//				nodePoses[rootMotionNodeIndex].position = beginPose.position;
//			}
//			// ワールド移動値を算出
//			DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&worldTransform);
//			DirectX::XMVECTOR WorldTranslation = DirectX::XMVector3TransformNormal(GlobalTranslation, WorldTransform);
//			DirectX::XMFLOAT3 worldTranslation;
//			DirectX::XMStoreFloat3(&worldTranslation, WorldTranslation);
//
//
//			// キャラクターの位置を更新
//			position.x += worldTranslation.x;
//			position.y += worldTranslation.y;
//			position.z += worldTranslation.z;
//		}
//
//		// アニメーション時間更新
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
//		// 姿勢更新
//		character->SetNodePoses(nodePoses);
//	
//
//	// ワールドトランスフォーム更新
//	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
//	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
//	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
//	DirectX::XMMATRIX WorldTransform = S * R * T;
//	DirectX::XMStoreFloat4x4(&worldTransform, WorldTransform);
//
//	// モデルトランスフォーム更新処理
//	character->UpdateTransform(worldTransform);
//}

void Model::ReverseplaybackAnimation(float elapsedTime, bool blend)
{
	//再生中でないなら処理しない
	if (!IsPlayAnimation()) return;

	//ブレンド率の計算
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

	//指定のアニメーションデータを取得
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);
	if (anim)
	{
		currentAnimationSeconds = animation.secondsLength;
		anim = false;
	}

	//アニメーションデータからキーフレームデータリストを取得
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount =
		static_cast<int>(keyFrameEnd) <= 0 ? static_cast<int> (keyframes.size()) : static_cast<int>(keyFrameEnd);
	for (int keyIndex = keyCount - 1; keyIndex > 0; --keyIndex)
	{

		//現在の時間がどのキーフレームの間にいるか判定する
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex - 1);
		if (currentAnimationSeconds <= keyframe0.seconds &&
			currentAnimationSeconds > keyframe1.seconds)
		{
			//再生時間とキーフレームの時間から補完率を算出する
			float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

			// 12

			int nodeCount = static_cast<int>(nodes.size());
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				//2つのキーフレーム間の補完計算
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];

				//ブレンド補完処理
				if (blendRate < 1.0f && blend)
				{
					//現在の姿勢と次のキーフレームとの姿勢の補完
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

					//計算結果をボーンに格納
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

				else
				{
					//前のキーフレームと次のキーフレームの姿勢を補完
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

					//計算結果をボーンに格納
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
			}
			break;
		}
	}

	//最終フレーム処理
	if (animationEndFlag)
	{
		animationEndFlag = false;
		currentAnimationIndex = -1;
		
		anim = true;
		return;
	}

	//時間経過
	currentAnimationSeconds -= elapsedTime;

	//再生時間が週短時間を超えたら
	if (currentAnimationSeconds <= 0)
	{
		//再生時間を巻き戻す
		if (animationLoopFlag)
		{
			currentAnimationSeconds += animation.secondsLength;
		}
		//再生終了時間にする
		else
		{
			currentAnimationSeconds = 0;
			animationEndFlag = true;
		}
	}

}

void Model::Update_blend_animations_gun(float elapsedTime, float blendrate, float currentAnimationSeconds, float currentAnimationSecondsblend, bool blend)
{


	// 再生中でないなら処理しない
	if (!IsPlayAnimation())return;


	// 指定のアニメーションデータを取得
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);

	// アニメーションデータからキーフレームデータリストを取得
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	
	//////////////////////ここ直して
	// 現在の時間がどのキーフレームの間にいるか判定する
	const ModelResource::Keyframe& keyframe0 = keyframes.at(10);
	const ModelResource::Keyframe& keyframe1 = keyframes.at(15);
	if (currentAnimationSeconds <= keyframe0.seconds &&
		currentAnimationSecondsblend > keyframe1.seconds)
	{

		int nodeCount = static_cast<int>(nodes.size());
		for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
		{
			//2つのキーフレーム間の補完計算
			const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
			const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

			Node& node = nodes[nodeIndex];
			// 通常の計算
			{
				DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
				DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
				DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
				DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
				DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
				DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

				// キーフレームの割合を求める。
				DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendrate);
				DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendrate);
				DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendrate);
				// 計算結果をボーンに格納
				DirectX::XMStoreFloat3(&node.scale, S);
				DirectX::XMStoreFloat4(&node.rotate, R);
				DirectX::XMStoreFloat3(&node.translate, T);
			}

		}
	}

	
}

void Model::Update_blend_animations(float elapsedTime,   bool blend)
{

	// 再生中でないなら処理しない
	if (!IsPlayAnimation())return;

	// ブレンド率の計算
	float blendRate = 1.0f;
	// 現在のブレンドが時間より下なら
	if (animationBlendTime < animationBlendSeconds)
	{
		// 足している。
		animationBlendTime += elapsedTime;
		if (animationBlendTime >= animationBlendSeconds)
		{
			animationBlendTime = animationBlendSeconds;
		}
		blendRate = animationBlendTime / animationBlendSeconds;
		// 二条してひづみを消す。
		blendRate *= blendRate;
	}

	// 指定のアニメーションデータを取得
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	// アニメーション種類
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);
	const ModelResource::Animation& animationSeconds = animations.at(currentAnimationIndexSeconds);

	// アニメーションデータからキーフレームデータリストを取得


	//float animationKeyFrames;
	
	//DirectX::XMStoreFloat(&animationKeyFrames, DirectX::XMVector3Normalize(DirectX::XMLoadFloat(&animation.keyframes)));
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	const std::vector<ModelResource::Keyframe>& keyframesSeconds = animationSeconds.keyframes;



	int keyCount =
		static_cast<int>(keyFrameEnd) <= 0 ? static_cast<int> (keyframes.size()) : static_cast<int>(keyFrameEnd);

	int keyCountSeconds = static_cast<int> (keyframesSeconds.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1 && keyIndex < keyCountSeconds - 1; ++keyIndex)
	{

		//////////////////////ここ直して
		// 現在の時間がどのキーフレームの間にいるか判定する
		
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
		
		const ModelResource::Keyframe& keyframeSeconds0 = keyframesSeconds.at(keyIndex);
		const ModelResource::Keyframe& keyframeSeconds1 = keyframesSeconds.at(keyIndex + 1);





		if (currentAnimationSeconds >= keyframe0.seconds &&
			currentAnimationSeconds < keyframe1.seconds&&
			currentAnimationSeconds >= keyframeSeconds0.seconds &&
			currentAnimationSeconds < keyframeSeconds1.seconds)
		{
			// 再生時間とキーフレームの時間から補完率を算出する
			//float rate = (currentAnimationSeconds - keyframe0.seconds ) / (keyframe1.seconds
			//	- keyframe0.seconds);

			int nodeCount = static_cast<int>(nodes.size());
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				//2つのキーフレーム間の補完計算
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				const ModelResource::NodeKeyData& keySeconds0 = keyframeSeconds0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& keySeconds1 = keyframeSeconds1.nodeKeys.at(nodeIndex);
				
				Node& node = nodes[nodeIndex];
				// アニメーション間
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


					// キーフレームの割合を求める。
					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);
					// 計算結果をボーンに格納
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
				// 通常の計算
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


					// キーフレームの割合を求める。
					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, animationBlendSeconds);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, animationBlendSeconds);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, animationBlendSeconds);


					// 計算結果をボーンに格納
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

			}
			break;
		}
	}


	// 最終フレーム処理
	if (animationEndFlag)
	{
		animationEndFlag = false;
		currentAnimationIndex = -1;
		currentAnimationIndexSeconds = -1;
		return;
	}



	// 時間経過
	currentAnimationSeconds += elapsedTime;

	//float animationtime = animation.secondsLength - animationSeconds.secondsLength;

	//float animationSecondsLength;
	//DirectX::XMStoreFloat(&animationSecondsLength,DirectX::XMVector3Normalize(DirectX::XMLoadFloat(&animation.secondsLength)));

	// 再生時間が終端時間を超えたら
	// 最高を超えた場合のループの場合を作る。
	// 通常最大値
	float animSecondsLength = animation.secondsLength;

	// 再生時間が終端時間を超えたら
	// 最高を超えた場合のループの場合を作る。
	if (keyFrameEnd + FLT_EPSILON > 0 + FLT_EPSILON)
	{

		const ModelResource::Keyframe& keyframe = keyframes.at(keyFrameEnd);
		animSecondsLength = keyframe.seconds;
	}
	if (currentAnimationSeconds >= animSecondsLength)
	{
		// ループならトゥルー
		if (animationLoopFlag)
		{
			// 再生時間が終端時間を超えたら

			
			// 何でー＝オーバーした分の事を考え引く事でがくとしない。
			// 再生時価を巻き戻す
			


			//currentAnimationSeconds -= animation.secondsLength;
			DirectX::XMVECTOR anim = DirectX::XMLoadFloat(&animation.secondsLength);
			DirectX::XMVECTOR animSecond = DirectX::XMLoadFloat(&animationSeconds.secondsLength);

			// アニメーション時間
			float animSecondsLength;
			DirectX::XMStoreFloat(&animSecondsLength, DirectX::XMVectorLerp(anim, animSecond, animationBlendSeconds));
			currentAnimationSeconds -= animSecondsLength;



		}
		else
		{
			// 再生時間が終端時間を超えたら

				// 何でー＝オーバーした分の事を考え引く事でがくとしない。
				// 再生時価を巻き戻す
			// 一番最後にオーバーしたら止める。
			
			currentAnimationSeconds = animation.secondsLength;

			// 終わりました。
			animationEndFlag = true;
		}
	}





}


// 上半身用アニメーション
void Model::UpdateUpeerBodyAnimation(float elapsedTime, const char* start, const char* end, bool blend)
{
	//int nodeStart = GetNodeIndex(start);
	//int nodeEnd = GetNodeIndex(end);

	//Model::Node* node = FindNode(start);
	Model::Node* startNode = FindNode(start);
	Model::Node* endNode = FindNode(end);

	//int nodeIndexStart = node->now;
	//再生中でないなら処理しない
	if (!IsPlayUpeerBodyAnimation()) return;

	//ブレンド率の計算
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

	//指定のアニメーションデータを取得
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndexUpeer);

	//アニメーションデータからキーフレームデータリストを取得
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount =
		static_cast<int> (keyFrameEndUpper) <= 0 ? static_cast<int> (keyframes.size()) : static_cast<int> (keyFrameEndUpper);
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		//現在の時間がどのキーフレームの間にいるか判定する
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
		if (currentAnimationSecondsUpeer >= keyframe0.seconds &&
			currentAnimationSecondsUpeer < keyframe1.seconds)
		{
			//再生時間とキーフレームの時間から補完率を算出する
			float rate = (currentAnimationSecondsUpeer - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

			// 13


			// 上半身アニメーション
			
			int nodeIndexMax = static_cast<int>(endNode->now);
			for (int nodeIndex = startNode->now; nodeIndex < nodeIndexMax; ++nodeIndex)
			{




				//2つのキーフレーム間の補完計算
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];




				//ブレンド補完処理
				if (blendRate < 1.0f && blend)
				{
					//現在の姿勢と次のキーフレームとの姿勢の補完
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

					//計算結果をボーンに格納
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

				else
				{
					//前のキーフレームと次のキーフレームの姿勢を補完
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

					//計算結果をボーンに格納
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

			}
			break;
		}
	}

	//最終フレーム処理
	if (animationEndFlagUpeer)
	{
		animationEndFlagUpeer = false;
		currentAnimationIndexUpeer = -1;
		return;
	}

	//時間経過
	currentAnimationSecondsUpeer += elapsedTime;
	// 時間経過 加算分アニメーションを早めるかどうか
	currentAnimationSecondsUpeer += currentAnimationUpperAddSeconds > currentAnimationUpperAddSecondsMin ? currentAnimationUpperAddSeconds : elapsedTime;
	
	// 通常最大値
	float animSecondsLength = animation.secondsLength;

	// 再生時間が終端時間を超えたら
	// 最高を超えた場合のループの場合を作る。
	if (keyFrameEnd + FLT_EPSILON > 0 + FLT_EPSILON)
	{

		const ModelResource::Keyframe& keyframe = keyframes.at(keyFrameEnd);
		animSecondsLength = keyframe.seconds;
	}
	
	//再生時間が週短時間を超えたら
	if (currentAnimationSecondsUpeer >= animSecondsLength)
	{
		//再生時間を巻き戻す
		if (animationLoopFlagUpeer)
		{
			currentAnimationSecondsUpeer -= animation.secondsLength;
		}
		//再生終了時間にする
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

	//再生中でないなら処理しない
	if (!IsPlayUpeerBodyAnimation()) return;

	//ブレンド率の計算
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

	//指定のアニメーションデータを取得
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndexUpeer);

	//アニメーションデータからキーフレームデータリストを取得
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;
	int keyCount = static_cast<int>(keyframes.size());


	if (anim)
	{
		currentAnimationSecondsUpeer = animation.secondsLength;
		anim = false;
	}

	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		//現在の時間がどのキーフレームの間にいるか判定する
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
		if (currentAnimationSecondsUpeer >= keyframe0.seconds &&
			currentAnimationSecondsUpeer < keyframe1.seconds)
		{
			//再生時間とキーフレームの時間から補完率を算出する
			float rate = (currentAnimationSecondsUpeer - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

			// 13

			// 上半身アニメーション
			int nodeCount = static_cast<int>(nodes.size());
			//int nodeCount = static_cast<int>(endNode->now);
			for (int nodeIndex = node->now; nodeIndex < nodeCount; ++nodeIndex)
			{




				//2つのキーフレーム間の補完計算
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];




				//ブレンド補完処理
				if (blendRate < 1.0f && blend)
				{
					//現在の姿勢と次のキーフレームとの姿勢の補完
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

					//計算結果をボーンに格納
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

				else
				{
					//前のキーフレームと次のキーフレームの姿勢を補完
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

					//計算結果をボーンに格納
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
			}
			break;
		}
	}

	//最終フレーム処理
	if (animationEndFlagUpeer)
	{
		animationEndFlagUpeer = false;
		currentAnimationIndexUpeer = -1;
		return;
	}

	//時間経過
	currentAnimationSecondsUpeer -= elapsedTime;

	//再生時間が週短時間を超えたら
	if (currentAnimationSecondsUpeer <= 0)
	{
		//再生時間を巻き戻す
		if (animationLoopFlagUpeer)
		{
			currentAnimationSecondsUpeer += animation.secondsLength;
		}
		//再生終了時間にする
		else
		{
			currentAnimationSecondsUpeer = 0;
			animationEndFlagUpeer = true;
		}
	}
}
// 下半身用
void Model::UpdateLowerBodyAnimation(float elapsedTime,const char* start, const char* end, bool blend)
{
	Model::Node* startNode = FindNode(start);
	Model::Node* endNode = FindNode(end);
	//再生中でないなら処理しない
	if (!IsPlayAnimation()) return;

	//ブレンド率の計算
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

	//指定のアニメーションデータを取得
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);

	//アニメーションデータからキーフレームデータリストを取得
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;

	// 再生
	int keyCount = static_cast<int>(keyFrameEnd) <= 0 ? static_cast<int>(keyframes.size()): static_cast<int>(keyFrameEnd);
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{

		//現在の時間がどのキーフレームの間にいるか判定する
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex + 1);
		if (currentAnimationSeconds >= keyframe0.seconds &&
			currentAnimationSeconds < keyframe1.seconds)
		{
			//再生時間とキーフレームの時間から補完率を算出する
			float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

			// 12

			int nodeCount = static_cast<int>(endNode->now);
			for (int nodeIndex = startNode->now; nodeIndex < nodeCount; ++nodeIndex)
			{
				//2つのキーフレーム間の補完計算
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];

				//ブレンド補完処理
				if (blendRate < 1.0f && blend)
				{
					//現在の姿勢と次のキーフレームとの姿勢の補完
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

					//計算結果をボーンに格納
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

				else
				{
					//前のキーフレームと次のキーフレームの姿勢を補完
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

					//計算結果をボーンに格納
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
			}
			break;
		}
	}

	//最終フレーム処理
	if (animationEndFlag)
	{
		animationEndFlag = false;
		currentAnimationIndex = -1;
		return;
	}

	//時間経過
	currentAnimationSeconds += elapsedTime;

	// 通常最大値
	float animSecondsLength = animation.secondsLength;

	// 再生時間が終端時間を超えたら
	// 最高を超えた場合のループの場合を作る。
	if (keyFrameEnd + FLT_EPSILON > 0 + FLT_EPSILON)
	{

		const ModelResource::Keyframe& keyframe = keyframes.at(keyFrameEnd);
		animSecondsLength = keyframe.seconds;
	}
	if (currentAnimationSeconds >= animSecondsLength)
	{
		//再生時間を巻き戻す
		if (animationLoopFlag)
		{
			currentAnimationSeconds -= animation.secondsLength;
		}
		//再生終了時間にする
		else
		{
			currentAnimationSeconds = animation.secondsLength;
			animationEndFlag = true;
		}
	}
}
// 逆再生
void Model::ReverseplaybackLowerBodyAnimation(float elapsedTime, const char* end, bool blend)
{
	Model::Node* node = FindNode(end);
	//再生中でないなら処理しない
	if (!IsPlayAnimation()) return;

	//ブレンド率の計算
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

	//指定のアニメーションデータを取得
	const std::vector<ModelResource::Animation>& animations = resource->GetAnimations();
	const ModelResource::Animation& animation = animations.at(currentAnimationIndex);
	if (anim)
	{
		currentAnimationSeconds = animation.secondsLength;
		anim = false;
	}

	//アニメーションデータからキーフレームデータリストを取得
	const std::vector<ModelResource::Keyframe>& keyframes = animation.keyframes;

	// 逆再生
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = keyCount - 1; keyIndex > 0; --keyIndex)
	{

		//現在の時間がどのキーフレームの間にいるか判定する
		const ModelResource::Keyframe& keyframe0 = keyframes.at(keyIndex);
		const ModelResource::Keyframe& keyframe1 = keyframes.at(keyIndex - 1);
		if (currentAnimationSeconds <= keyframe0.seconds &&
			currentAnimationSeconds > keyframe1.seconds)
		{
			//再生時間とキーフレームの時間から補完率を算出する
			float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);

			// 12

			int nodeCount = static_cast<int>(node->now);
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				//2つのキーフレーム間の補完計算
				const ModelResource::NodeKeyData& key0 = keyframe0.nodeKeys.at(nodeIndex);
				const ModelResource::NodeKeyData& key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node& node = nodes[nodeIndex];

				//ブレンド補完処理
				if (blendRate < 1.0f && blend)
				{
					//現在の姿勢と次のキーフレームとの姿勢の補完
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&node.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, blendRate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, blendRate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, blendRate);

					//計算結果をボーンに格納
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}

				else
				{
					//前のキーフレームと次のキーフレームの姿勢を補完
					DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&key1.scale);
					DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&key0.translate);
					DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&key1.translate);

					DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
					DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
					DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

					//計算結果をボーンに格納
					DirectX::XMStoreFloat3(&node.scale, S);
					DirectX::XMStoreFloat4(&node.rotate, R);
					DirectX::XMStoreFloat3(&node.translate, T);
				}
			}
			break;
		}
	}

	//最終フレーム処理
	if (animationEndFlag)
	{
		animationEndFlag = false;
		currentAnimationIndex = -1;
		anim = true;
		return;
	}

	//時間経過
	currentAnimationSeconds -= elapsedTime;

	//再生時間が週短時間を超えたら
	if (currentAnimationSeconds <= 0)
	{
		//再生時間を巻き戻す
		if (animationLoopFlag)
		{
			currentAnimationSeconds += animation.secondsLength;
		}
		//再生終了時間にする
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

// アニメーション再生
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
	// 全てのノードを総当たりで名前比較する
	for (Node& node : nodes)
	{
		// char同士が同じなら
		// std省略::
		if (::strcmp(name , node.name) == 0)
		{
			node.now = bonenowsize;
			return &node;
		}
		++bonenowsize;
	}

	// 見つからなかった
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
