#pragma once
#include "Component.h"
#include "SpriteControll.h"
#include "TransForm2D.h"

class UiMpGauge : public Component
{
public:
    UiMpGauge() {};
    ~UiMpGauge() override;

    // 名前取得
    const char* GetName() const override { return "UiHp"; }

    // 開始処理
    void Start() override;

    // 更新処理
    void Update(float elapsedTime) override;

    // 描画処理
    void Render2D(RenderContext& rc, SpriteShader& shader) override;

    // hpゲージ処理
    void MpGaugeUpdate();
#ifdef _DEBUG
    // GUI描画
    void OnGUI() override;
#endif // _DEBUG

    void SetColor(DirectX::XMFLOAT4 color) { this->color = color; }
    DirectX::XMFLOAT4 GetColor() { return color; }

private:
    // 描画の切り替えよう
    bool       drawCheck = false;

    // １秒
    int        time = 60;
    // 制限時間秒読み
    int        timeMax = 0;

    // カウントダウン
    int        countDown = 0;

    DirectX::XMFLOAT4 color = { 1,1,1,1 };

    DirectX::XMFLOAT2 scale = { 0,0 };

    float alphaMax = 1.0f;
};
