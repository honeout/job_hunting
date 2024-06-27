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

	struct Node
	{
		const char*			name;
		Node*				parent;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4	localTransform;
		DirectX::XMFLOAT4X4	worldTransform;

		std::vector<Node*>	children;
		int now;
	};


	// 行列計算
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);
	// 行列計算　ボーン情報を共有
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform,  std::vector<Node> nodes);

	// ノードリスト取得
	const std::vector<Node>& GetNodes() const { return nodes; }
	//std::vector<Node>& GetNodes() { return nodes; }



	// リソース取得
	const ModelResource* GetResource() const { return resource.get(); }

	// アニメーション更新処理
	void UpdateAnimation(float elapsedTime, bool blend = false);


	// 逆再生
	void ReverseplaybackAnimation(float elapsedTime, bool blend = false);

	// 二つをブレンド
	void Update_blend_animations(float elapsedTime, float blendrate,float currentAnimationSeconds, float currentAnimationSecondsblend,bool blend = false);


	//上半身アニメーション更新処理
	void UpdateUpeerBodyAnimation(float elapsedTime, const char* start,const char* end, bool blend = false);

	// 上半身用アニメーション更新
	void ReverseplaybackUpeerBodyAnimation(float elapsedTime, const char* start, bool blend = false);

	//下半身アニメーション更新処理
	void UpdateLowerBodyAnimation(float elapsedTime, const char* end, bool blend = false);

	void ReverseplaybackLowerBodyAnimation(float elapsedTime, const char* end, bool blend = false);

	//アニメーション再生
	void PlayUpeerBodyAnimation(int index, bool loop, float blendSeconds = 0.2f);

	//アニメーション再生
	void PlayLowerBodyAnimation(int index, bool loop, float blendSeconds = 0.2f);


	// アニメーション再生
	void PlayAnimation(int index, bool loop,float currentanimationseconds = 0.0f,bool end = false, float blendSeconds = 0.2f);

	//逆再生アニメーション
	void PlayReverseAnimation(int index, bool loop, float blendSeconds = 0.2f);

	

	// アニメーション再生中か
	bool IsPlayAnimation() const;

	bool IsPlayUpeerBodyAnimation() const;

	// ノード検索
	Node* FindNode(const char* name);

	// 現在のアニメーション再生時間取得
	float GetCurrentANimationSeconds()const { return currentAnimationSeconds; }

	// 現在の上半身のアニメーション再生時間取得
	float GetCurrentAnimationSecondsUpeer()const { return currentAnimationSecondsUpeer; }

	int GetNodeIndex(const char* name)const;

private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>				nodes;

	// 個々がーだとないという事
	int currentAnimationIndex = -1;           //アニメーション番号
	float currentAnimationSeconds = 0.0f;    //アニメーションの再生用時間

	//パンチをした即ー１つまりアニメーション終了つまりTポーズに戻る。

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


     // 上半身のキーフレーム時間
	int keyframeUpper = 0;
	// 下半身のキーフレーム時間
	int keyframeLower = 0;

	// 再生　逆再生
	bool anim = true;
};
