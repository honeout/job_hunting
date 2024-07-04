#pragma once

// 初期化
class IStart
{
public:
    IStart() {};
    virtual ~IStart() {};

    virtual void Start() = 0;
};
// 更新処理
class IUpdate
{
public:
    IUpdate() {};
    virtual ~IUpdate() {};

    virtual void Update() = 0;
};
// 描画処理
class IRendere
{
    IRendere() {};
    virtual ~IRendere() {};

    virtual void Render() = 0;
};
// シェーダー処理
class IShader
{
    IShader() {};
    virtual ~IShader() {};

    virtual void Render() = 0;
    virtual void RenderStart() = 0;
};