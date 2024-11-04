#pragma once

#include "Actor.h"

// コンポーネント
class Component
{
public:
	Component() {}
	virtual ~Component() {}

	// 名前取得
	virtual const char* GetName() const = 0;

	// 開始処理
	virtual void Start() {}

	// 更新処理
	virtual void Update(float elapsedTime) {}

	// 描画
	virtual void Render(RenderContext& rc, ModelShader& shader) {}
	virtual void Render2D(RenderContext& rc, SpriteShader& shader) {}

	// シャドウマップ描画
	virtual void RenderShadowmap(RenderContext& rc) {}
#ifdef _DEBUG
	// GUI描画
	virtual void OnGUI() {}
#endif // _DEBUG
	// アクター設定
	void SetActor(std::shared_ptr<Actor> actor) { this->actor = actor; }

	// アクター取得
	std::shared_ptr<Actor> GetActor() { return actor.lock(); }

private:
	std::weak_ptr<Actor>	actor;
	
};
