#pragma once

#include <DirectXMath.h>
#include <Effekseer.h>

// エフェクト
class Effect
{
public:
    Effect(const char* filename);
    ~Effect() {};

    // 再生
    Effekseer::Handle Play(const DirectX::XMFLOAT3& position, float scale = 1.0f);

    // 停止
    void Stop(Effekseer::Handle handle);

    // 座標更新
    void SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position);

    // スケール更新
    void SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale);
    // ハンドルゲット
    Effekseer::Handle GetEfeHandle() const { return this->handle; }

private:
    // 読み込み
    Effekseer::EffectRef effekseerEffect;
    // 自分自身か
    Effekseer::Handle handle;
};
