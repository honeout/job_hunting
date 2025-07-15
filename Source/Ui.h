#pragma once
#include "Component.h"
#include "SpriteControll.h"
#include "TransForm2D.h"

class Ui : public Component
{
public:
    Ui() {};
    ~Ui() override;

    // 名前取得
    const char* GetName() const override { return "Ui"; }

    // 開始処理
    void Start() override;

    // 更新処理
    void Update(float elapsedTime) override;

    // 描画処理
    void Render2D(RenderContext& rc, SpriteShader& shader) override;
#ifdef _DEBUG
    // GUI描画
    void OnGUI() override;
#endif // _DEBUG

    // 上昇
    void IncrementToAlpha(float increment);

    // 時間
    void UiTimeUpdate();

    // 描画確認
    void SetDrawCheck(bool drawCheck) { this->drawCheck = drawCheck; }

    // 描画確認
    bool GetDrawCheck() const { return drawCheck; }

    // セット時間秒読み
    void SetTimeMax(int timeMax) { this->timeMax = timeMax; }

    // ゲット時間秒読み
    int GetTimeMax() const { return timeMax; }

    // ゲット時間１フレーム
    int GetTime() const { return time; }

    void SetCountDown(int countDown) { this->countDown = countDown; }

    void SetColor(DirectX::XMFLOAT4 color) { this->color = color; }
    DirectX::XMFLOAT4 GetColor() { return color; }

private:
    Sprite* sprite = nullptr;

    // 位置や動き
    std::shared_ptr<TransForm2D> transForm2D;


    // 描画の切り替えよう
    bool       drawCheck = false;

    // １秒
    int        time = 60;
    // 制限時間秒読み
    int        timeMax = 0;

    // カウントダウン
    int        countDown = 0;

    DirectX::XMFLOAT4 color = { 1,1,1,1 };

    float alphaMax = 1.0f;
};