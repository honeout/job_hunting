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
#include "ProjectileStraight.h"




// ������
void SceneGame::Initialize()
{
	// �X�e�[�W������
	//StageManager& stageManager = StageManager::instance();
	//StageMain* stageMain = new StageMain();
	//stageManager.Register(stageMain);

	//StageMoveFloor* stageMoveFloor = new StageMoveFloor();
	//stageMoveFloor->SetStartPosint(DirectX::XMFLOAT3(0, 1, 3));
	//stageMoveFloor->SetGoalPoint(DirectX::XMFLOAT3(10, 2, 3));
	//stageMoveFloor->SetTorque(DirectX::XMFLOAT3(0, 1.0f, 0));
	//stageManager.Register(stageMoveFloor);
	
	// �X�e�[�W������
	{
		const char* filename = "Data/Model/ExampleStage/ExampleStage.mdl";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->LoadModel(filename);
		actor->SetName("StageMain");
		actor->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetRotation(DirectX::XMFLOAT4(0, 0, 0, 1));
		actor->SetScale(DirectX::XMFLOAT3(1, 1, 1));
		actor->AddComponent<StageMain>();
		StageManager::Instance().Register(actor.get());
		
		//StageManager::instance().Register();
		//StageManager::instance().Register();

	}


	player = new Player;
	{
		// �v���C���[������
		const char* filename = "Data/Model/Jammo/Jammo.mdl";
		
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->LoadModel(filename);
		actor->SetName("Player");
		actor->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
		actor->SetRotation(DirectX::XMFLOAT4(0, 0, 0, 1));
		actor->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
		actor->AddComponent<Movement>();
		actor->AddComponent<HP>();
		actor->AddComponent<Player>();
		actor->AddComponent<Collision>();
		actor->AddComponent<StageMain>();
		//actor->AddComponent<ProjectileStraight>();
		PlayerManager::Instance().Register(actor.get());
		
	}

	{
		// �d��
		//for (int i = 0; i < 10; ++i)
		//{
		// �G������
		const char* filename = "Data/Model/Slime/Slime.mdl";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->LoadModel(filename);
		actor->SetName("EnemySlime");
		actor->SetPosition(DirectX::XMFLOAT3(0, 0, 1));
		actor->SetRotation(DirectX::XMFLOAT4(0, 0, 0, 1));
		actor->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
		actor->AddComponent<Movement>();
		actor->AddComponent<HP>();
		actor->AddComponent<Collision>();
		actor->AddComponent<StageMain>();
		actor->AddComponent<EnemySlime>();
		EnemyManager::Instance().Register(actor.get());
		
		

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
	// �J�����R���g���[���[������
	//cameraController = new CameraController();

	// �G�l�~�[������
	//EnemyManager& enemyManager = EnemyManager::Instance();
	//for (int i = 0; i < 2; ++i)
	//{
	//	EnemySlime* slime = new EnemySlime();
	//	slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 5));
	//	// �꒣��
	//	slime->SetTerritory(slime->GetPosition(), 10.0f);
	//	enemyManager.Register(slime);

	//}


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
	if (this->player)
	{
		delete player;
		player = nullptr;
	}
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

		ActorManager::Instance().UpdateTransform();
		ActorManager::Instance().Render(rc.view, rc.projection);
		

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
