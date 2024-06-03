#pragma once

#include "Graphics/Shader.h"
#include "Character.h"

// �G�l�~�[
class Afterimage : public Character
{
public:
    Afterimage() {}
    ~Afterimage() override {}

    // �X�V����
    virtual void Update(float elapsedTime) = 0;

    // �`�揈���@
    virtual void Render(RenderContext& context, ModelShader* shade) = 0;

    // �j��
    void Destroy();

    // �f�o�b�O�v���~�e�B�u�`��
    virtual void DrawDebugPrimitive();


};