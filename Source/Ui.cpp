#include "Ui.h"


Ui::~Ui()
{
}

void Ui::Start()
{
    // �摜
    sprite = GetActor()->GetComponent<SpriteControll>()->GetSprite();
    // �ʒu��
    transForm2D = GetActor()->GetComponent<TransForm2D>();
}

void Ui::Update(float elapsedTime)
{
}

void Ui::Render(RenderContext& rc, ModelShader& shader)
{
	// �r���[�|�[�g ��ʂ̃T�C�Y��
    // �r���[�|�[�g�Ƃ�2D�̉�ʂɕ`��͈͂̎w��(�N���b�s���O�w����o����)�ʒu���w��
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    //// ���X�^���C�U�[�X�e�[�g�Ƀo�C���h����Ă���r���[�|�[�g�z����擾
    rc.deviceContext->RSGetViewports(&numViewports, &viewport);
    
    // �ϊ��s��
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&rc.view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&rc.projection);
    // ���[�J�����烏�[���h�ɍs���Ƃ��ɂ���z����̃|�W�V������n���B
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

    // 2D
    {
        		// �`��
        sprite->Render(rc.deviceContext,
            transForm2D->GetPosition().x,
            transForm2D->GetPosition().y
			, transForm2D->GetScale().x
			, transForm2D->GetScale().y,
			0, 0,
			static_cast<float> (sprite->GetTextureWidth()),
			static_cast<float> (sprite->GetTextureHeight()),
            transForm2D->GetAngle(),
			1, 1, 1, 1);
    }
}

void Ui::OnGUI()
{
}
