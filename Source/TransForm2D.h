#pragma once
#include "Component.h"

class TransForm2D : public Component
{
public:
    TransForm2D() {};
    ~TransForm2D() override;

    // 名前取得
    const char* GetName() const override { return "TransForm2D"; }
#ifdef _DEBUG
    // GUI描画
    void OnGUI() override;
#endif // _DEBUG

    // 揺れ
    void Shake();
    // 揺らす範囲最大
    void SetUiMax(int max) { this->max = max; }
    // 揺らす範囲最小
    void SetUiMin(int min) { this->min = min; }

    // 位置の設定
    void SetPosition(const DirectX::XMFLOAT2& position) { this->position = position; }

    // 元位置の設定
    void SetTexPosition(const DirectX::XMFLOAT2& texPosition) { this->texPosition = texPosition; }

    // 位置の取得
    const DirectX::XMFLOAT2& GetPosition() const { return position; }

    // 元位置の取得
    const DirectX::XMFLOAT2& GetTexPosition() const { return texPosition; }

    // 角度の設定
    void SetAngle(const float& angle) {  this->angle = angle; }

    // 角度の設定
    const float& GetAngle() const { return angle; }

    // スケールの設定
    void SetScale(const DirectX::XMFLOAT2& scale) { this->scale = scale; }

    // 元スケールの設定
    void SetTexScale(const DirectX::XMFLOAT2& texScale) { this->texScale = texScale; }

    // スケールの取得
    const DirectX::XMFLOAT2& GetScale() const { return scale; }

    // 元スケールの取得
    const DirectX::XMFLOAT2& GetTexScale() const { return texScale; }

    // 揺れ時間最大
    void SetShakeTimeMax(int shakeTimeUiMax) { this->shakeTimeUiMax = shakeTimeUiMax; }
   
    // 揺れ経過時間変更
    void SetShakeTime(int shakeTimeUi) { this->shakeTimeUi = shakeTimeUi; }

    // 時間終了かどうか
    bool GetShakeEnd() { return shakeTimeUi >= shakeTimeUiMax ? true : false; }

private:

    // 画像の位置
    DirectX::XMFLOAT2 position = {0,0};

    // 画像の元位置
    DirectX::XMFLOAT2 texPosition = { 0,0 };

    // 角度
    float angle = 0;
    // 画像の大きさ
    DirectX::XMFLOAT2 scale = {0,0};

    // 画像の元の大きさ
    DirectX::XMFLOAT2 texScale = { 0,0 };

    bool checkPositionY = false;

    // UI揺らす範囲
    int max = 0;
    int min = 0;

    // UIを揺らすための時間
    int shakeTimeUi = 0;
    // UIを揺らす時間最大
    int shakeTimeUiMax = 0;

    

};