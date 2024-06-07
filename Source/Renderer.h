#pragma once
#pragma once

#include <DirectXMath.h>
#include "Component.h"

// ˆÚ“®
class Rendere : public Component
{
public:
	Rendere();
	virtual ~Rendere();

	// –¼‘Oæ“¾
	const char* GetName() const override { return "Render"; }

	// GUI•`‰æ
	void OnGUI() override;

	// ”¼Œaæ“¾ 05
	float GetRadius() const { return radius; }

	// ƒ_ƒ[ƒW‚ğ—^‚¦‚é ƒ_ƒ[ƒW‚ğó‚¯‚½‚ç‘¼‚É“n‚·B
	bool DrawDebugPrimitive();

private:
	// ”¼Œa05
	float radius = 0.5f;

};
