#pragma once


#include "Player.h"
#include "CameraController.h"
#include "Scene.h"
#include "Graphics/Sprite.h"

#include "AfterimageManager.h"

// ƒQ[ƒ€ƒV[ƒ“
class SceneGame : public Scene
{
public:
	SceneGame() {}
	~SceneGame() override{}

	// ‰Šú‰»
	void Initialize() override;

	// I—¹‰»
	void Finalize() override;

	// XVˆ—
	void Update(float elapsedTime) override;

	// •`‰æˆ—
	void Render() override;

<<<<<<< HEAD
	// •`‰æˆ—
	void DrawDebugParameter(DirectX::XMFLOAT4X4& transform, const char* label);

	// ƒOƒŠƒbƒh•`‰æ
	void DrawGrid(ID3D11DeviceContext* context, int subdivisions, float scale);

=======
>>>>>>> parent of 8a0ff20 (ã¨ã‚Šã‚ãˆãšã‚·ã‚§ãƒ¼ãƒ€ãƒ¼ç¶ºéº—ãªå¥´å…¥ã£ãŸ)
private:
	// ƒGƒlƒ~[HPƒQ[ƒW•`‰æ
	void RenderEnemyGauge(
	ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);

	void AfterimageTime(float elapsedTime);


private:
	Player* player = nullptr;
	CameraController* cameraController = nullptr;

	Sprite* gauge = nullptr;

	AfterimageManager* afterimageManager;

	// c‘œo‚·ŠÔŠuÅ‘å’l
	float spawnafterimagetimemax = 0.1f;

	// c‘œo‚·ŠÔŠu
	float elapsedFrame = spawnafterimagetimemax;

	// c‘œ“§–¾“xÅ‘å’l
	float afterImageAlphaMax = 0.7f;
	// c‘œ“§–¾“x
	float afterImageAlpha = afterImageAlphaMax;

	// c‘œÁ‚¦‚é‚Ü‚ÅÅ‘å’l
	float reduceMax = 0.4f;
	// c‘œÁ‚¦‚é‚Ü‚Å
	float reduce = reduceMax;
<<<<<<< HEAD


	// ƒIƒtƒXƒNƒŠ[ƒ“ƒŒƒ“ƒ_ƒŠƒ“ƒO—p•`‰æƒ^[ƒQƒbƒg
	std::unique_ptr<RenderTarget> renderTarget;


	// ƒVƒƒƒhƒEƒ}ƒbƒv—pî•ñ
	Light* mainDirectionalLight = nullptr; // ƒVƒƒƒhƒEƒ}ƒbƒv‚ğ¶¬‚·‚é•½sŒõŒ¹
	std::unique_ptr<DepthStencil> shadowmapDepthStencil; // ƒVƒƒƒhƒEƒ}ƒbƒv—p[“xƒXƒeƒ“ƒVƒ‹ƒoƒbƒtƒ@
	float shadowDrawRect = 500.0f;// ƒVƒƒƒhƒEƒ}ƒbƒv‚É•`‰æ‚·‚é”ÍˆÍ
	DirectX::XMFLOAT4X4 lightViewProjeciton; // ƒ‰ƒCƒgƒrƒ…[ƒvƒƒWƒFƒNƒVƒ‡ƒ“s—ñ
	DirectX::XMFLOAT3 shadowColor = { 0.2,0.2f,0.2f };// ‰e‚ÌF
	float shadowBias = 0.001f;// [“x”äŠr—p‚ÌƒIƒtƒZƒbƒg’l

	// ƒ|ƒXƒgƒvƒƒZƒX
	std::unique_ptr<PostprocessingRenderer> postprocessingRenderer;

=======
>>>>>>> parent of 8a0ff20 (ã¨ã‚Šã‚ãˆãšã‚·ã‚§ãƒ¼ãƒ€ãƒ¼ç¶ºéº—ãªå¥´å…¥ã£ãŸ)
};
