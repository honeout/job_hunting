#pragma once
#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "Graphics/ModelResource.h"

// モデル
class Model
{
public:
	Model(const char* filename);
	~Model() {}
	// ボーン
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


	struct NodePose
	{
		DirectX::XMFLOAT3	position = { 0, 0, 0 };
		DirectX::XMFLOAT4	rotation = { 0, 0, 0, 1 };
		DirectX::XMFLOAT3	scale = { 1, 1, 1 };
	};
	// アニメーションの再生
	struct ModelAnim
	{
		int index = 0;
		bool loop = false;
		float currentanimationseconds = 0.0f;
		float blendSeconds = 0.7f;
		float currentAnimationAddSeconds = 0;
		float keyFrameEnd = 0.0f;
	};

	// 行列計算
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);
	// 行列計算　ボーン情報を共有
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform,  std::vector<Node> nodes);

	// アニメーション計算
	void ComputeAnimation(int animationIndex, int nodeIndex, float time, NodePose& nodePose, float blendRate) ;
	// 全骨の姿勢
	void ComputeAnimation(int animationIndex, float time, std::vector<NodePose>& nodePoses, float blendRate) ;

	// ルートモーションアップデート
	void UpdateRootMortion(float elapsedTime);

	// ノードリスト取得
	const std::vector<Node>& GetNodes() const { return nodes; }

	// リソース取得
	const ModelResource* GetResource() const { return resource.get(); }

	// アニメーション更新処理
	void UpdateAnimation(float elapsedTime, bool blend = true);

	// 逆再生
	void ReverseplaybackAnimation(float elapsedTime, bool blend = false);

	// 二つをブレンド
	void Update_blend_animations_gun(float elapsedTime, float blendrate,float currentAnimationSeconds, float currentAnimationSecondsblend,bool blend = false);

	// 二つのアニメーションブレンド
	void Update_blend_animations(float elapsedTime, bool blend = false);

	//上半身アニメーション更新処理
	void UpdateUpeerBodyAnimation(float elapsedTime, const char* start, const char* end, bool blend = false);

	// 上半身用アニメーション更新
	void ReverseplaybackUpeerBodyAnimation(float elapsedTime,  const char* end, bool blend = false);

	//下半身アニメーション更新処理
	void UpdateLowerBodyAnimation(float elapsedTime, const char* start, const char* end, bool blend = false);

	void ReverseplaybackLowerBodyAnimation(float elapsedTime, const char* end, bool blend = false);

	//アニメーション再生
	void PlayUpeerBodyAnimation(ModelAnim animeRule);


	// アニメーション途中でアニメーション再生ルールを変える。
	void UpeerBodyAnimationRuleChanger(float currentAnimationUpperAddSeconds = 0);

	//アニメーション再生
	void PlayLowerBodyAnimation(int index, bool loop, float blendSeconds = 0.2f);

	// アニメーション再生
	void PlayAnimation(ModelAnim animeRule);

	// アニメーション途中でアニメーション再生ルールを変える。
	void AnimationRuleChanger(float currentAnimationAddSeconds = 0);

	//逆再生アニメーション
	void PlayReverseAnimation(ModelAnim animeRule);

	// 二つのアニメーションブレンド
	void PlayAnimationBlend(int index,int index2, bool loop, float currentanimationseconds = 0.0f, float blendSeconds = 0.2f);

	// アニメーション再生中か
	bool IsPlayAnimation() const;

	bool IsPlayUpeerBodyAnimation() const;

	bool IsPlayAnimationBlend() const;

	// ノード検索
	Node* FindNode(const char* name);

	// ボーンをワールド位置
	DirectX::XMFLOAT3 ConvertLocalToWorld(Node* node);

	// 現在のアニメーション再生時間取得
	float GetCurrentANimationSeconds()const { return currentAnimationSeconds; }

	// 現在の上半身のアニメーション再生時間取得
	float GetCurrentAnimationSecondsUpeer()const { return currentAnimationSecondsUpeer; }

	// 現在のアニメーション種類
	int GetCurrentAnimationIndex() { return currentAnimationIndex; }
	int GetCurrentAnimationIndexUpeerIndex() { return currentAnimationIndexUpeer; }

	int GetNodeIndex(const char* name)const;

	// ノードポーズ設定
	void SetNodePoses(const std::vector<NodePose>& nodePoses);

	void SetAnimationEndFlag(bool end) { this->animationEndFlag = end; }
private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>				nodes;
	std::vector<Model::NodePose>		nodePoses;
	// 個々がーだとないという事
	int currentAnimationIndex = -1;           //アニメーション番号
	int currentAnimationIndexSeconds = -1;           //アニメーション番号 ブレンド用
	float currentAnimationSeconds = 0.0f;    //アニメーションの再生用時間
	float currentAnimationAddSeconds = 0.0f;    //アニメーションの再生用加算用
	float currentAnimationAddSecondsMin = 0.0f;    //アニメーションの再生用加算用
	float oldcurrentAnimationSeconds = 0;
	float rastcurrentAnimationSeconds = 0.67f;
	float animationSecondsLengthMax = 0.0f;

	float currentAnimationUpperAddSeconds = 0.0f;    //上半身アニメーションの再生用加算用
	float currentAnimationUpperAddSecondsMin = 0.0f;    //上半身アニメーションの再生用加算用

	// ループ終了か
	bool animationLoopFlag = false;
	// ワンショット終了
	bool animationEndFlag = false;

	float animationBlendTime = 0.0f;
	float animationBlendSeconds = 0.0f;

	bool agterimagestop = false;

	// 上半身用のアニメーション時間
	int currentAnimationIndexUpeer = -1;				//アニメーション番号
	float currentAnimationSecondsUpeer = 0.0f;		//アニメーションの再生用時間
	bool animationLoopFlagUpeer = false;				//アニメーションがループするか
	bool animationEndFlagUpeer = false;				//アニメーションが再生終了しているか
	float animationBlendTimeUpeer = 0.0f;			//アニメーション補完時間
	float animationBlendSecondsUpeer = 0.0f;			//〃

	// キーフレームの最後
	float keyFrameEnd = 0;
	float keyFrameEndUpper = 0;

     // 上半身のキーフレーム時間
	int keyframeUpper = 0;
	// 下半身のキーフレーム時間
	int keyframeLower = 0;

	// 再生　逆再生
	bool anim = true;
};
