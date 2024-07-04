#pragma once

// ������
class IStart
{
public:
    IStart() {};
    virtual ~IStart() {};

    virtual void Start() = 0;
};
// �X�V����
class IUpdate
{
public:
    IUpdate() {};
    virtual ~IUpdate() {};

    virtual void Update() = 0;
};
// �`�揈��
class IRendere
{
    IRendere() {};
    virtual ~IRendere() {};

    virtual void Render() = 0;
};
// �V�F�[�_�[����
class IShader
{
    IShader() {};
    virtual ~IShader() {};

    virtual void Render() = 0;
    virtual void RenderStart() = 0;
};