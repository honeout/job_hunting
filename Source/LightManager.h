#pragma once
#include <vector>
#include "Light.h"
#include "Graphics/RenderContext.h"

// ���C�g�Ǘ��N���X
class LightManager
{
    LightManager() {}
    ~LightManager();

public:
    static LightManager& Instanes()
    {
        static LightManager lightManager;
        return lightManager;
    }

    // ���C�g��o�^����
    void Register(Light* light);

    // ���C�g�̓o�^����������
    void Remove(Light* light);

    // �o�^�ς݂̃��C�g��S�폜����
    void Clear();

    // ���C�g����RenderContext�ɂ� 
    void PushRenderContext(RenderContext& rc);

    // �f�o�b�O���̕\��
    void DrawDebugGUI();

    // �f�o�b�O�v���~�e�B�u�̕\��
    void DrawDebugPrimitive();

    // ����
    DirectX::XMFLOAT4 GetAmbientColor() { return ambientColor; }
    void SetAmbientColor(DirectX::XMFLOAT4 amvientColor) {
        this->ambientColor = amvientColor;    }

    // ���C�g�̏��
    int GetLightCount() const { return static_cast<int> (lights.size()); }
    Light* GetLight(int index) const { return lights.at(index); }
private:
    std::vector<Light*> lights;
    DirectX::XMFLOAT4 ambientColor = { 0.2f,0.2f,0.2f,1.0f };
};
