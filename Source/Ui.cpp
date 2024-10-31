#include <imgui.h>
#include "Ui.h"

Ui::~Ui()
{
    if (transForm2D)
        transForm2D.reset();


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
    UiTimeUpdate();
}

void Ui::Render2D(RenderContext& rc, SpriteShader& shader)
{
	// �r���[�|�[�g ��ʂ̃T�C�Y��
    // �r���[�|�[�g�Ƃ�2D�̉�ʂɕ`��͈͂̎w��(�N���b�s���O�w����o����)�ʒu���w��
    //D3D11_VIEWPORT viewport;
    //UINT numViewports = 1;
    ////// ���X�^���C�U�[�X�e�[�g�Ƀo�C���h����Ă���r���[�|�[�g�z����擾
    //rc.deviceContext->RSGetViewports(&numViewports, &viewport);
    
    // �ϊ��s��
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&rc.view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&rc.projection);
    // ���[�J�����烏�[���h�ɍs���Ƃ��ɂ���z����̃|�W�V������n���B
    //DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

    // 2D
    if (drawCheck)
    {
        		// �`��
        sprite->Render(rc.deviceContext,
            transForm2D->GetPosition().x,
            transForm2D->GetPosition().y
			, transForm2D->GetScale().x
			, transForm2D->GetScale().y,
            transForm2D->GetTexPosition().x, transForm2D->GetTexPosition().y,
            transForm2D->GetTexScale().x > 0 ?  transForm2D->GetTexScale().x : static_cast<float> (sprite->GetTextureWidth()),
            transForm2D->GetTexScale().y > 0 ?  transForm2D->GetTexScale().y : static_cast<float> (sprite->GetTextureHeight()),
            transForm2D->GetAngle(),
            color.x,color.y,color.z,color.w);
    }
}

void Ui::OnGUI()
{

    ImGui::SliderFloat4("Color", &color.x, 0, 1);
}

void Ui::UiTimeUpdate()
{
    if (timeMax > 0)
    --countDown;
}
