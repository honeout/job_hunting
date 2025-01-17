#include "SceneGameOver.h"
#include "Graphics/Graphics.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "Input/Input.h"
#include "SceneLoading.h"
void SceneGameOver::Initialize()
{
    // �X�v���C�g������
    sprite = new Sprite("Data/Sprite/��End.png");

    // �J����������
    cameraControlle = nullptr;
    cameraControlle = new CameraController();
}

void SceneGameOver::Finalize()
{
    // �X�v���C�g�I����
    if (this->sprite)
    {
        delete sprite;
        sprite = nullptr;
    }

    if (cameraControlle != nullptr)
    {
        delete cameraControlle;
        cameraControlle = nullptr;
    }

}

void SceneGameOver::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    // �J�����R���g���[���[�X�V����
    cameraControlle->Update(elapsedTime);

    // �����{�^�����������烍�[�f�B���O���͂���ŃQ�[���V�[���֐؂�ւ�
    const GamePadButton anyButton =
        GamePad::BTN_A |
        GamePad::BTN_B |
        GamePad::BTN_X |
        GamePad::BTN_Y;

    if (gamePad.GetButtonUp() & anyButton)// ���[�h�̎��Q�[���Ƃ���������
        SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));

}

void SceneGameOver::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };   // RGBA(0.0~1.0)
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // 2D�X�v���C�g�`��
    {
        float screenWidth = static_cast<float>(graphics.GetScreenWidth());
        float screenHeight = static_cast<float>(graphics.GetScreenHeight());
        float textureWidth = static_cast<float>(sprite->GetTextureWidth());
        float textureHeight = static_cast<float>(sprite->GetTextureHeight());
        // �`��
        sprite->Render(dc,
            0, 0, screenWidth, screenHeight,
            0, 0, textureWidth, textureHeight,
            0,
            1, 1, 1, 1);
        // {�ʒu}{�T�C�Y}{�摜�ǂ�����}{�摜�����܂�}
        // dc , �o�͈́p�o�p
    }
}
