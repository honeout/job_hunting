#pragma once

#include "Graphics/Shader.h"
#include "Character.h"
#include "Movement.h"
#include "HP.h"
#include "Actor.h"
#include "Component.h"

// �G�l�~�[
class Enemy : public Character
{
public:
    Enemy() {}
    ~Enemy() override {}


    virtual void Start() = 0;
    // �X�V����
    //�������z�֐����������������ō��Ȃ���
    //�R���p�C���G���[�ɂȂ�
    //
    virtual void Update(float elapsedTime) = 0;

    // �`�揈���@
    virtual void Render(const RenderContext& rc, ModelShader* shader) = 0;

    // �j��
    void Destroy();

    // �f�o�b�O�v���~�e�B�u�`��
    virtual void DrawDebugPrimitive();

private:

};