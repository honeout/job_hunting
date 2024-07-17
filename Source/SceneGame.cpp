#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
//#include "EnemyManager.h"
#include "EnemySlime.h"

#include "EffectManager.h"

#include "Input/Input.h"

#include <DirectXMath.h>

//#include "StageManager.h"
#include "StageMain.h"

#include "StageMoveFloor.h"

#include "PlayerAfterimage.h"

#include "Actor.h"
#include "Movement.h"
#include "HP.h"
#include "ModelControll.h"
#include "ProjectileStraight.h"
#include "Transform.h"
#include "Ui.h"
#include "TransForm2D.h"
#include "UiManager.h"

<<<<<<< HEAD



// �V���h�E�}�b�v�̃T�C�Y
static const UINT SHADOWMAP_SIZE = 2048;
=======


>>>>>>> parent of 8a0ff20 (とりあえずシェーダー綺麗な奴入った)

// ������
void SceneGame::Initialize()
{
	
	// �X�e�[�W������
	{
		const char* filename = "Data/Model/ExampleStage/ExampleStage.mdl";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->AddComponent<ModelControll>();
		actor->GetComponent<ModelControll>()->LoadModel(filename);
		actor->SetName("StageMain");
		actor->AddComponent<Transform>();
		
		actor->GetComponent<Transform>()->
			SetPosition(DirectX::XMFLOAT3(0, 0, 0));

		actor->GetComponent<Transform>()->
			SetAngle(DirectX::XMFLOAT3( 0, 0, 0));

		actor->GetComponent<Transform>()->
			SetScale(DirectX::XMFLOAT3(1, 1, 1));

		actor->AddComponent<StageMain>();
		StageManager::Instance().Register(actor.get());
		

	}


	//player = new Player;
	{
		// �v���C���[������
		const char* filename = "Data/Model/Jammo/Jammo.mdl";
		
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->AddComponent<ModelControll>();
		actor->GetComponent<ModelControll>()->LoadModel(filename);
		actor->SetName("Player");
		actor->AddComponent<Transform>();

		actor->GetComponent<Transform>()->
			SetPosition(DirectX::XMFLOAT3(0, 0, 0));

		actor->GetComponent<Transform>()->
			SetAngle(DirectX::XMFLOAT3(0, 0, 0));

		actor->GetComponent<Transform>()->
			SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
		actor->AddComponent<Movement>();
		actor->AddComponent<HP>();
		actor->AddComponent<Player>();
		actor->AddComponent<Collision>();
		//actor->AddComponent<StageMain>();
		//actor->AddComponent<ProjectileStraight>();
		PlayerManager::Instance().Register(actor);
		
	}
	// �G
	{
		// �d��
		//for (int i = 0; i < 10; ++i)
		//{
		// �G������
		const char* filename = "Data/Model/Slime/Slime.mdl";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->AddComponent<ModelControll>();
		actor->GetComponent<ModelControll>()->LoadModel(filename);
		actor->SetName("EnemySlime");
		actor->AddComponent<Transform>();

		actor->GetComponent<Transform>()->
			SetPosition(DirectX::XMFLOAT3(0, 0, 1));

		actor->GetComponent<Transform>()->
			SetAngle(DirectX::XMFLOAT3(0, 0, 0));

		actor->GetComponent<Transform>()->
			SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
		actor->AddComponent<Movement>();
		actor->AddComponent<HP>();
		actor->AddComponent<Collision>();
		//actor->AddComponent<StageMain>();
		actor->AddComponent<EnemySlime>();
		EnemyManager::Instance().Register(actor);
		
		

	}

	// UI
	{
		const char* filename = "Data/Sprite/Title.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UI");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		actor->AddComponent<Ui>();
		UiManager::Instance().Register(actor);
	}

	//player = new Player();

	// �J���������ݒ� ������ʒu�ǂ���������̂Ȃ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)

		);
	// �ǂ͈̔͂��ǂꂾ�����邩���s�܂�
	camera.SetPerspedtiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

<<<<<<< HEAD
<<<<<<< HEAD


	 // ���s������ǉ�
=======
	// ���s������ǉ�
>>>>>>> parent of 0db33c3 (一度保存ワーニング大量)
	{
		mainDirectionalLight = new Light(LightType::Directional);
		mainDirectionalLight->SetDirection({ 1,-1,-1 });
		LightManager::Instance().Register(mainDirectionalLight);
	}
<<<<<<< HEAD
	
	// �_������ǉ�
	{
		Light* light = new Light(LightType::Point);
		light->SetPosition(DirectX::XMFLOAT3(0, 1, 0));
		light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		LightManager::Instance().Register(light);
	}


	// �X�|�b�g���C�g��ǉ�
	{
		Light* light = new Light(LightType::Spot);
		light->SetPosition(DirectX::XMFLOAT3(-30, 1, 0));
		light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		light->SetDirection(DirectX::XMFLOAT3(+1, -1, 0));
		light->SetRange(40.0f);
		LightManager::Instance().Register(light);
	}

=======
>>>>>>> parent of 0db33c3 (一度保存ワーニング大量)

	// �V�����`��^�[�Q�b�g�̐���
	{
		Graphics& graphics = Graphics::Instance();
		renderTarget = std::make_unique<RenderTarget>(static_cast<UINT>(graphics.GetScreenWidth())
			, static_cast<UINT>(graphics.GetScreenHeight())
			, DXGI_FORMAT_R8G8B8A8_UNORM);
		

	}

	// �V���h�E�}�b�v�p�ɐ[�x�X�e���V���̐���
	{
		shadowmapDepthStencil = std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	}

	// �|�X�g�v���Z�X�`��N���X����
	{
		postprocessingRenderer = std::make_unique<PostprocessingRenderer>();
		// �V�[���e�N�X�`����ݒ肵�Ă���
		ShaderResourceViewData srvData;
		srvData.srv = renderTarget->GetShaderResourceView();
		srvData.width = renderTarget->GetWidth();
		srvData.height = renderTarget->GetHeight();
		postprocessingRenderer->SetSceneData(srvData);
	}
=======
>>>>>>> parent of 8a0ff20 (とりあえずシェーダー綺麗な奴入った)



	// �Q�[�W�X�v���C�g
	gauge = new Sprite();
}

// �I����
void SceneGame::Finalize()
{
	// �Q�[�W�X�v���C�g
	if (this->gauge)
	{
		delete gauge;
		gauge = nullptr;
	}

	// �G�l�~�[�I����
	//EnemyManager::Instance().Clear();

	AfterimageManager::Instance().Clear();

	//// �J�����R���g�[���[�I����
	//if (this->cameraController)
	//{
	//	delete cameraController;
	//	cameraController = nullptr;
	//}

	// �v���C���[�I����
	//if (this->player)
	//{
	//	delete player;
	//	player = nullptr;
	//}
	// �X�e�[�W�I����
	//StageManager::instance().Clear();


}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	
	// �J�����R���g���[���[�X�V����
	//DirectX::XMFLOAT3 target = player->GetPosition();
	//target.y += 0.5f;// ��������T�O�Z���`���炢
	//cameraController->SetTarget();// �v���C���[�̍�������
	//cameraController->Update(elapsedTime);

	// �X�e�[�W�X�V����
	//StageManager::instance().Update(elapsedTime);
	
	// �v���C���[�X�V����
	//player->Update(elapsedTime);
	ActorManager::Instance().Update(elapsedTime);

	// �c���o�ߎ���
	//AfterimageTime(elapsedTime);

	

	// �c���X�e�[�g�X�V
	AfterimageManager::Instance().Update(elapsedTime);



	// �G�l�~�[�X�V����
	//EnemyManager::Instance().Update(elapsedTime);

	// �G�t�F�N�g�X�V����
	EffectManager::Instance().Update(elapsedTime);
}



// �`�揈��
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

<<<<<<< HEAD
<<<<<<< HEAD
=======
	// �V���h�E�}�b�v�̕`��
	RenderShadowmap();
>>>>>>> parent of 0db33c3 (一度保存ワーニング大量)

	Render3DScene();



=======
>>>>>>> parent of 8a0ff20 (とりあえずシェーダー綺麗な奴入った)
	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �`�揈�� 
	RenderContext rc;// �`�悷�邽�߂ɕK�v�ȏ����܂Ƃ߂��\����
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// ���f�����ꂼ��ŃV�F�[�_�[�����邽�߂�
	rc.deviceContext = dc;

<<<<<<< HEAD
<<<<<<< HEAD

	// �V���h�E�}�b�v�̕`��
	RenderShadowmap();

	Render3DScene();


	// ���C�g�̏����l�ߍ���
	//LightManager::Instanes().PushRenderContext(rc);


     // �������ݐ���o�b�N�o�b�t�@�ɕς��ăI�t�X�N���[�������_�����O�̌��ʂ�`�悷��
	{
		ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

		// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
		FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f }; // RGBA(0.0~1.0)
		dc->ClearRenderTargetView(rtv, color);
		dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		dc->OMSetRenderTargets(1, &rtv, dsv);

		// UINT11
		// �r���[�|�[�g�̐ݒ�
		D3D11_VIEWPORT vp = {};
		vp.Width = graphics.GetScreenWidth();
		vp.Height = graphics.GetScreenHeight();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		dc->RSSetViewports(1, &vp);

		// unit09
		//RenderContext rc;
		//rc.deviceContext = dc;

		//SpriteShader* shader = graphics.GetShader(SpriteShaderId::ColorGrading);
		//shader->Begin(rc);

		////rc.colorGradingData = colorGradingData;
		//shader->Draw(rc, sprite.get());

		//shader->End(rc);
		// �|�X�g�v���Z�X���������s��
		postprocessingRenderer->Render(dc);
	}


=======
>>>>>>> parent of 8a0ff20 (とりあえずシェーダー綺麗な奴入った)
=======
	// ���C�g�̏����l�ߍ���
	//LightManager::Instanes().PushRenderContext(rc);

>>>>>>> parent of 0db33c3 (一度保存ワーニング大量)
	// 3D���f���`��
	{
		ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
		// �V�F�[�_�[�ɕK�v�ȏ�������
		shader->Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��
		// �X�e�[�W�`��
		//StageManager::instance().Render(rc, shader);
	// �v���C���[�`��
		//player->Render(rc, shader);

		// �c���`��
		//AfterimageManager::Instance().Render(dc, shader);

		// �G�l�~�[�`��
		//EnemyManager::Instance().Render(rc, shader);

		shader->End(rc);

		//ActorManager::Instance().UpdateTransform();
<<<<<<< HEAD

		//ActorManager::Instance().Render(rc.view, rc.projection);
		//
		// �������ݐ���o�b�N�o�b�t�@�ɕς��ăI�t�X�N���[�������_�����O�̌��ʂ�`�悷��
		{
			ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
			ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

			// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
			FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f }; // RGBA(0.0~1.0)
			dc->ClearRenderTargetView(rtv, color);
			dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			dc->OMSetRenderTargets(1, &rtv, dsv);

			// UINT11
			// �r���[�|�[�g�̐ݒ�
			D3D11_VIEWPORT vp = {};
			vp.Width = graphics.GetScreenWidth();
			vp.Height = graphics.GetScreenHeight();
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			dc->RSSetViewports(1, &vp);

			// unit09
			//RenderContext rc;
			//rc.deviceContext = dc;

			//SpriteShader* shader = graphics.GetShader(SpriteShaderId::ColorGrading);
			//shader->Begin(rc);

			////rc.colorGradingData = colorGradingData;
			//shader->Draw(rc, sprite.get());

			//shader->End(rc);
			// �|�X�g�v���Z�X���������s��
			postprocessingRenderer->Render(dc);
		}


		//ModelShader* mdlshader = graphics.GetShader(ModelShaderId::AfterImage);
		//
		//mdlshader->Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��
=======
		ActorManager::Instance().Render(rc.view, rc.projection);
		

		ModelShader* mdlshader = graphics.GetShader(ModelShaderId::AfterImage);
		
		mdlshader->Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��
>>>>>>> parent of 8a0ff20 (とりあえずシェーダー綺麗な奴入った)

		ActorManager::Instance().Render(rc);
		

		ModelShader* mdlshader = graphics.GetShader(ModelShaderId::AfterImage);
		
		mdlshader->Begin(rc);// �V�F�[�_�[�ɃJ�����̏���n��


		
		// �c���`��
	    AfterimageManager::Instance().Render(rc, mdlshader);


		mdlshader->End(rc);
	}

	// 3D�G�t�F�N�g�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

<<<<<<< HEAD

	// �f�o�b�O���̕\��
	{
		//DrawDebugParameter(jummo_transform, "jummo");
		//DrawDebugParameter(uncle_transform, "uncle");
		//DrawDebugParameter(stage_transform, "stage");
		LightManager::Instance().DrawDebugGUI();
		ImGui::Separator();
		// UNIT11
		if (ImGui::TreeNode("shadowmap"))
		{
			ImGui::SliderFloat("DrawRect", &shadowDrawRect, 1.0f, 2048.0f);
			ImGui::ColorEdit3("Color", &shadowColor.x);
			ImGui::SliderFloat("Bias", &shadowBias, 0.0f, 0.1f);
			ImGui::Text("texture");
			ImGui::Image(shadowmapDepthStencil->GetShaderResourceView().Get(), { 256,256 }, { 0,0 }, { 1,1 },
				{ 1,1,1,1 });
			ImGui::TreePop();
		}
	}

=======
>>>>>>> parent of 0db33c3 (一度保存ワーニング大量)
	// 3D�f�o�b�O�`��
	{
		// �����蔻��̌`������
		// �v���C���[�f�o�b�O�v���~�e�B�u�`��
		//player->DrawDebugPrimitive();

		// �G�l�~�[�f�o�b�O�v���~�e�B�u�`��
		//EnemyManager::Instance().DrawDebugPrimitive();
		for (int i = 0; i >= EnemyManager::Instance().GetEnemyCount(); i++)
		{
			EnemyManager::Instance().GetEnemy(i)->GetComponent<EnemySlime>()->DrawDebugPrimitive();
		}
		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// ���ۂ̓����蔻��`��
		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2D�X�v���C�g�`��
	{
		//RenderEnemyGauge(dc, rc.view, rc.projection);
	}

	// 2D�f�o�b�OGUI�`��
	{
		// �v���C���[�f�o�b�O�`��
		//player->DrawDebugGUI();
		//cameraController->DrawDebugGUI();

		//EnemyManager::Instance().DrawDebugGUI();

	}

	
}

<<<<<<< HEAD
<<<<<<< HEAD

void SceneGame::DrawDebugParameter(DirectX::XMFLOAT4X4& transform, const char* label)
{
	ImGui::PushID(label);
	if (ImGui::TreeNode(label))
	{
		DirectX::XMVECTOR Scale, Rotation, Position;
		DirectX::XMMatrixDecompose(&Scale, &Rotation, &Position, DirectX::XMLoadFloat4x4(&transform));
		DirectX::XMFLOAT3 scale, rotation, position;
		DirectX::XMStoreFloat3(&scale, Scale);
		DirectX::XMStoreFloat3(&rotation, Rotation);
		DirectX::XMStoreFloat3(&position, Position);
		ImGui::SliderFloat3("scale", &scale.x, 0.0f, 10.0f);
		ImGui::SliderFloat3("rotation", &rotation.x, -DirectX::XM_PI, DirectX::XM_PI);
		ImGui::SliderFloat3("position", &position.x, -300.0f, 1000.0f);
		DirectX::XMMATRIX Transform;
		Transform = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)
			* DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)
			* DirectX::XMMatrixTranslation(position.x, position.y, position.z);
		DirectX::XMStoreFloat4x4(&transform, Transform);
		ImGui::TreePop();
	}
	ImGui::PopID();
}

void SceneGame::DrawGrid(ID3D11DeviceContext* context, int subdivisions, float scale)
{
	int numLines = (subdivisions + 1) * 2;
	int vertexCount = numLines * 2;

	float corner = 0.5f;
	float step = 1.0f / static_cast<float>(subdivisions);

	int index = 0;
	float s = -corner;

	const DirectX::XMFLOAT4 white = DirectX::XMFLOAT4(1, 1, 1, 1);

	LineRenderer* lineRenderer = Graphics::Instance().GetLineRenderer();
	// Create vertical lines
	float scaling = static_cast<float>(subdivisions) * scale;
	DirectX::XMMATRIX M = DirectX::XMMatrixScaling(scaling, scaling, scaling);
	DirectX::XMVECTOR V, P;
	DirectX::XMFLOAT3 position;
	for (int i = 0; i <= subdivisions; i++)
	{
		V = DirectX::XMVectorSet(s, 0, corner, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		lineRenderer->AddVertex(position, white);

		V = DirectX::XMVectorSet(s, 0, -corner, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		lineRenderer->AddVertex(position, white);

		s += step;
	}

	// Create horizontal lines
	s = -corner;
	for (int i = 0; i <= subdivisions; i++)
	{
		V = DirectX::XMVectorSet(corner, 0, s, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		lineRenderer->AddVertex(position, white);

		V = DirectX::XMVectorSet(-corner, 0, s, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		lineRenderer->AddVertex(position, white);

		s += step;
	}

	// X��
	{
		const DirectX::XMFLOAT4 red = DirectX::XMFLOAT4(1, 0, 0, 1);
		V = DirectX::XMVectorSet(0, 0, 0, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		lineRenderer->AddVertex(position, red);

		V = DirectX::XMVectorSet(corner, 0, 0, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		lineRenderer->AddVertex(position, red);
	}

	// Y��
	{
		const DirectX::XMFLOAT4 green = DirectX::XMFLOAT4(0, 1, 0, 1);
		V = DirectX::XMVectorSet(0, 0, 0, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		lineRenderer->AddVertex(position, green);

		V = DirectX::XMVectorSet(0, corner, 0, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		lineRenderer->AddVertex(position, green);
	}

	// Z��
	{
		const DirectX::XMFLOAT4 blue = DirectX::XMFLOAT4(0, 0, 1, 1);
		V = DirectX::XMVectorSet(0, 0, 0, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		lineRenderer->AddVertex(position, blue);

		V = DirectX::XMVectorSet(0, 0, corner, 0);
		P = DirectX::XMVector3TransformCoord(V, M);
		DirectX::XMStoreFloat3(&position, P);
		lineRenderer->AddVertex(position, blue);
	}
}

=======
>>>>>>> parent of 0db33c3 (一度保存ワーニング大量)
void SceneGame::Render3DScene()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = renderTarget->GetRenderTargetView().Get();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp = {};
	vp.Width = graphics.GetScreenWidth();
	vp.Height = graphics.GetScreenHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// �`�揈��
	RenderContext rc;
	rc.deviceContext = dc;

	// ���C�g�̏����l�ߍ���
	LightManager::Instance().PushRenderContext(rc);

	// UNIT11
	// ���C�g�̏����߂��� 
	for (int i = 0; i < ShadowmapCount; ++i)
	{
		rc.shadowMapData.shadowMap[i] = shadowmapDepthStencil[i]->GetShaderResourceView().Get();
		rc.shadowMapData.lightViewProjection[i] = lightViewProjection[i];
		rc.shadowMapData.shadowBias[i] = shadowBias[i];
	}
	rc.shadowMapData.shadowColor = shadowColor;

	// �J�����p�����[�^�ݒ�
	Camera& camera = Camera::Instance();
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();


	// 3D���f���`��
	{
		//ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);
		////ModelShader* shader = graphics.GetShader(ModelShaderId::Toon);
		//shader->Begin(rc);

		//shader->Draw(rc, stage.get());
		//shader->Draw(rc, jummo.get());
		//shader->Draw(rc, uncle.get());
		////shader->Draw(rc, earth.get());

		//shader->End(rc);

		ActorManager::Instance().Render(rc.view, rc.projection);


	}

	// �f�o�b�O�v���~�e�B�u�̕\��
	{
		// �O���b�h�`��
		//DrawGrid(dc, 20, 10.0f);

		// ���C�g�̃f�o�b�O�v���~�e�B�u�̕`��
		LightManager::Instance().DrawDebugPrimitive();

		// ���C�������_���`����s
		graphics.GetLineRenderer()->Render(dc, camera.GetView(), camera.GetProjection());

		// �f�o�b�O�����_���`����s
		graphics.GetDebugRenderer()->Render(dc, camera.GetView(), camera.GetProjection());
	}
}

void SceneGame::RenderShadowmap()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = renderTarget->GetRenderTargetView().Get();
	ID3D11DepthStencilView* dsv = depthStencil->GetDepthStencilView().Get();
	// ��ʃN���A
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);



	// �����_�[�^�[�Q�b�g�ݒ�
	dc->OMSetRenderTargets(0, &rtv, dsv);

	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp = {};
	vp.Width = static_cast<float>(shadowmapDepthStencil->GetWidth());
	vp.Height = static_cast<float> (shadowmapDepthStencil->GetHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// �`�揈��
	RenderContext rc;
	rc.deviceContext = dc;

	// �J�����p�����[�^�[�ݒ�
	{
		// ���s��������J�����ʒu���쐬���A�������猴�_�̈ʒu������悤�Ɏ����s��𐶐�
		DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat3(&mainDirectionalLight->GetDirection());
		LightPosition = DirectX::XMVectorScale(LightPosition, -250.0f);
		DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(LightPosition,
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

		// �V���h�E�}�b�v�ɕ`�悵�����͈͂̎ˉe�s��𐶐�
		DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(shadowDrawRect, shadowDrawRect, 0.1f,
			1000.0f);
		DirectX::XMStoreFloat4x4(&rc.view, V);
		DirectX::XMStoreFloat4x4(&rc.projection, P);
		DirectX::XMStoreFloat4x4(&lightViewProjeciton, V * P);
	}

	ActorManager::Instance().RenderShadowmap(rc.view, rc.projection);
}


=======
>>>>>>> parent of 8a0ff20 (とりあえずシェーダー綺麗な奴入った)
// �G�l�~�[HP�Q�[�W�`��
void SceneGame::RenderEnemyGauge(ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection
)
{


	//// �r���[�|�[�g ��ʂ̃T�C�Y��
	//// �r���[�|�[�g�Ƃ�2D�̉�ʂɕ`��͈͂̎w��(�N���b�s���O�w����o����)�ʒu���w��
	//D3D11_VIEWPORT viewport;
	//UINT numViewports = 1;
	//// ���X�^���C�U�[�X�e�[�g�Ƀo�C���h����Ă���r���[�|�[�g�z����擾
	//dc->RSGetViewports(&numViewports, &viewport);

	//// �ϊ��s��
	//DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	//DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	//// ���[�J�����烏�[���h�ɍs���Ƃ��ɂ���z����̃|�W�V������n���B
	//DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();


	//// �S�Ă̓G�̓����HP�Q�[�W��\��
	//EnemyManager& enemyManager = EnemyManager::Instance();
	//int enemyCount = enemyManager.GetEnemyCount();

	//for (int i = 0; i < enemyCount; ++i)
	//{

	//	Enemy* enemy = enemyManager.GetEnemy(i);
	//	// �G�l�~�[����
	//	DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();
	//	worldPosition.y += enemy->GetHeight();

	//	// ���[���h����X�N���[��
	//	DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);


	//		// �Q�[�W�`�� // ���[���h����X�N���[��

	//	DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
	//		WorldPosition,
	//		viewport.TopLeftX,
	//		viewport.TopLeftY,
	//		viewport.Width,
	//		viewport.Height,
	//		viewport.MinDepth,
	//		viewport.MaxDepth,
	//		Projection,
	//		View,
	//		World

	//	);
	//	// �X�N���[�����W
	//	DirectX::XMFLOAT3 scereenPosition;
	//	DirectX::XMStoreFloat3(&scereenPosition, ScreenPosition);

	//	// �Q�[�W����
	//	const float gaugeWidth = 30.0f;
	//	const float gaugeHeight = 5.0f;

	//	float healthRate = enemy->GetHealth() / static_cast<float>(enemy->GetMaxHealth());


	//	if (scereenPosition.z < 0.0f || scereenPosition.z > 1.0f)continue;

	//	// 2D�X�v���C�g�`��
	//	{
	//		// �`��
	//		gauge->Render(dc,
	//			scereenPosition.x - gaugeWidth * 0.5f,
	//			scereenPosition.y - gaugeHeight
	//			,gaugeWidth * healthRate
	//			,gaugeHeight,
	//			0, 0,
	//			static_cast<float> (gauge->GetTextureWidth()),
	//			static_cast<float> (gauge->GetTextureHeight()),
	//			0.0f,
	//			1, 0, 0, 1);
	//		// {�ʒu}{�T�C�Y}{�摜�ǂ�����}{�摜�����܂�}
	//		// dc , �o�͈́p�o�p
	//	}
	//
	//

	//	}


	//// �G�l�~�[�z�u����
	//Mouse& mouse = Input::Instance().GetMouse();
	//if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	//{
	//	// �}�E�X�J�[�\�����W���擾
	//	DirectX::XMFLOAT3 scereenPosition;
	//	scereenPosition.x = static_cast<float>(mouse.GetPositionX());
	//	scereenPosition.y = static_cast<float>(mouse.GetPositionY());
	//	scereenPosition.z = 0.0f;

	//	// ���C�̎n�_���Z�o
	//	DirectX::XMVECTOR WorldPosition{};
	//	WorldPosition = DirectX::XMLoadFloat3(&scereenPosition);



	//	// �X�N���[���������[���h��Ԃ�
	//	WorldPosition = DirectX::XMVector3Unproject(
	//		WorldPosition,
	//		viewport.TopLeftX,
	//		viewport.TopLeftY,
	//		viewport.Width,
	//		viewport.Height,
	//		viewport.MinDepth,
	//		viewport.MaxDepth,
	//		Projection,
	//		View,
	//		World
	//	);
	//	// �n�_
	//	DirectX::XMFLOAT3 rayStart;
	//	DirectX::XMStoreFloat3(&rayStart, WorldPosition);

	//	// ���C�̏I�_���Z�o
	//	scereenPosition.z = 1.0f;
	//	WorldPosition = DirectX::XMLoadFloat3(&scereenPosition);
	//	WorldPosition = DirectX::XMVector3Unproject(
	//		WorldPosition,
	//		viewport.TopLeftX,
	//		viewport.TopLeftY,
	//		viewport.Width,
	//		viewport.Height,
	//		viewport.MinDepth,
	//		viewport.MaxDepth,
	//		Projection,
	//		View,
	//		World
	//	);
	//	DirectX::XMFLOAT3 rayEnd;
	//	DirectX::XMStoreFloat3(&rayEnd, WorldPosition);

	//	// ���C�L���X�g
	//	HitResult hit;
	////	if (StageManager::instance().RayCast(rayStart, rayEnd, hit))
	////	{
	////		// �G��z�u
	/////*		EnemySlime* slime = new EnemySlime();
	////		slime->SetPosition(hit.position);
	////		EnemyManager::Instance().Register(slime);*/
	////	}
	//	
	//}
}

void SceneGame::AfterimageTime(float elapsedTime)
{
	//afterImageAlpha -=  elapsedTime;
	//// �����Ă�����c�����o��B
	//if (player->GetAfterimageMove())
	//{
	//	elapsedFrame -= elapsedTime;
	//	
	//	


	//}
	//else
	//{
	//	elapsedFrame = spawnafterimagetimemax;
	//}

	//// �c���o���Ԋu
	//if (elapsedFrame <= 0)
	//{

	//	PlayerAfterimage* playerAfterimage = new PlayerAfterimage();
	//	// �z�u����
	//	playerAfterimage->SetPosition({ player->GetPosition() });
	//	playerAfterimage->SetAngle(player->GetAngle());
	//	playerAfterimage->SetTransform(player->GetTransform());
	//	// ��������
	//	playerAfterimage->GetModel()->UpdateTransform(player->GetTransform(),
	//		player->GetModel()->GetNodes());

	//	// �c���̏o��Ԋu�Đݒ�
	//	elapsedFrame = spawnafterimagetimemax;

	//	// �����x �ő�l���Đݒ�
	//	afterImageAlpha = afterImageAlphaMax;
	//	
	//	// �������ԃZ�b�g
	//	playerAfterimage->SetSurvivalTime(afterImageAlpha);

	//	// ������܂ł̎��ԍĐݒ�
	//	reduce = reduceMax;

	//	// ������܂ł̎��ԃZ�b�g
	//	playerAfterimage->SetReduce(reduce);

	//	// �o�^
	//	AfterimageManager::Instance().Register(playerAfterimage);

	//	// �c�ƃX�e�[�g�A�j���[�V�����̎��� �p��
	//	AfterimageManager::Instance().GetAfterimage(
	//		AfterimageManager::Instance().GetAfterimageCount() - 1
	//	)->SetCurrentAnimationSeconds(
	//		player->GetCurrentANimationSeconds());
	//	//playerAfterimage->GetModel()->UpdateTransform(player->GetTransform());



	//	

	//}
}
