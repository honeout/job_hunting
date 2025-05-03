#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <map>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include "Sprite.h"

// �X�v���C�g
class Sprite
{
public:
	Sprite();
	Sprite(const char* filename);
    ~Sprite() {}



	struct Vertex
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT2	texcoord;
	};

	// �o�b�t�@�X�V
	void Update(
		float dx, float dy,
		float dw, float dh,
		float sx, float sy,
		float sw, float sh,
		float angle,
		float r, float g, float b, float a) const;

	// �`����s
	void Render(ID3D11DeviceContext *dc,
		float dx, float dy,
		float dw, float dh,
		float sx, float sy,
		float sw, float sh,
		float angle,
		float r, float g, float b, float a) const;

	// �A�j���[�V�����t���[��
	struct AnimFrame
	{
		// �`��̈�(UV�Z�`)
		RECT rect;
		// ���̃A�j���[�V�����̍Đ�����
		float duration;
	};

	// �A�j���[�V�����̎��
	struct AnimClip
	{
		std::string name;
		std::vector<AnimFrame> frames;
		bool isLooping;
	};

	// �A�j���[�V�����̒ǉ�
	void AddAnim(const AnimClip& clip);

	// �A�j���[�V�����̑I��
	void PlayAnimation(const std::string& name, bool forceRestart = false);
	
	// �X�v���C�g�̃A�j���[�V�����Đ�
	void UpdateAnimation(float elapsedTime);

	// �V�F�[�_�[���\�[�X�r���[�̐ݒ�
	void SetShaderResourceView(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& srv, int texWidth, int texHeight);

	// ���_�o�b�t�@�̎擾
	const Microsoft::WRL::ComPtr<ID3D11Buffer>& GetVertexBuffer() const { return vertexBuffer; }
	
	// �V�F�[�_�[���\�[�X�r���[�̎擾
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const { return shaderResourceView; }


	// �e�N�X�`�����擾
	int GetTextureWidth() const { return textureWidth; }

	// �e�N�X�`�������擾
	int GetTextureHeight() const { return textureHeight; }
private:
	struct Texture
	{
		// ���̃A�j���[�V�����̉摜�ʒu
		int animIndex;
		// ���摜�̈ʒu�P�|�[�Y
		DirectX::XMFLOAT2 texPos;
		// ���摜�̑S��
		DirectX::XMFLOAT2 texPosMax;
		// �]��
		float top = 0.0f;
		float left = 0.0f;
		// �������Q�Ƃ��邩
		DirectX::XMFLOAT2 spriteSheat;
	};

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				vertexBuffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView;

	int textureWidth = 0;
	int textureHeight = 0;

	// �A�j���[�V�����̎��
	std::map<std::string, AnimClip> animationClips;
	const AnimClip* currentClip = nullptr;
	int currentFrameIndex = 0;
	float frameTimer = 0.0f;


};