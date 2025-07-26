#pragma once
#include "Component.h"
#include "SpriteControll.h"
#include "TransForm2D.h"
#include "Graphics\Graphics.h"

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
    // 色
    void SetColor(DirectX::XMFLOAT4 color) { this->color = color; }
    DirectX::XMFLOAT4 GetColor() { return color; }
    // 透かし
    void SetAlpha(float alpha) { this->color.w = alpha; }

    // 画像枚数
    //void SetTextureU(float textureU) { this->textureU = textureU; }
    //void SetTextureV(float textureV) { this->textureV = textureV; }

    // シート最大値
    void SetSheet(int clos, int rows) { 
        this->textureU = 1.0f / clos;
        this->textureV = 1.0f / rows;
    }

    // テクスチャの一つのUV
    void GetSpriteUV(int col, int row) {
        Sprite::UVRect uv;
        uv.u0 = col * textureU;
        uv.v0 = row * textureV;
        uv.u1 = uv.u0 + textureU;
        uv.u1 = uv.v0 + textureV;

        texPosition = {transForm2D->GetTexPosition().x + uv.u0, transForm2D->GetTexPosition().y + uv.v0 };
        texScale = { transForm2D->GetTexScale().x + uv.u1, transForm2D->GetTexScale().y + uv.v1 };
    }
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

    float textureU, textureV;

    // 元画像の左端
    DirectX::XMFLOAT2 texPosition = {0,0};
    // 元画像の右端
    DirectX::XMFLOAT2 texScale = { 0,0 };
};