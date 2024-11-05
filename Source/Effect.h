#pragma once

#include <DirectXMath.h>
#include <Effekseer.h>

// �G�t�F�N�g
class Effect
{
public:
    Effect(const char* filename);
    ~Effect() {};

    // �Đ�
    Effekseer::Handle Play(const DirectX::XMFLOAT3& position, float scale = 1.0f);

    // ��~
    void Stop(Effekseer::Handle handle);

    // ���W�X�V
    void SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position);

    // �X�P�[���X�V
    void SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale);
    // �n���h���Q�b�g
    Effekseer::Handle GetEfeHandle() const { return this->handle; }

private:
    // �ǂݍ���
    Effekseer::EffectRef effekseerEffect;
    // �������g��
    Effekseer::Handle handle;
};
