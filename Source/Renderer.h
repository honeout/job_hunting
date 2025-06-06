#pragma once
#pragma once

#include <DirectXMath.h>
#include "Component.h"

// 移動
class Rendere : public Component
{
public:
	Rendere();
	virtual ~Rendere();

	// 名前取得
	const char* GetName() const override { return "Render"; }

	// GUI描画
	void OnGUI() override;

	// 半径取得 05
	float GetRadius() const { return radius; }

	// ダメージを与える ダメージを受けたら他に渡す。
	bool DrawDebugPrimitive();

private:
	// 半径05
	float radius = 0.5f;

};
