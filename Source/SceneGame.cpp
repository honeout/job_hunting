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



// ƒVƒƒƒhƒEƒ}ƒbƒv‚ÌƒTƒCƒY
static const UINT SHADOWMAP_SIZE = 2048;
=======


>>>>>>> parent of 8a0ff20 (ã¨ã‚Šã‚ãˆãšã‚·ã‚§ãƒ¼ãƒ€ãƒ¼ç¶ºéº—ãªå¥´å…¥ã£ãŸ)

// ‰Šú‰»
void SceneGame::Initialize()
{
	
	// ƒXƒe[ƒW‰Šú‰»
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
		// ƒvƒŒƒCƒ„[‰Šú‰»
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
	// “G
	{
		// d‚¢
		//for (int i = 0; i < 10; ++i)
		//{
		// “G‰Šú‰»
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

	// ƒJƒƒ‰‰Šúİ’è Œ©‚¦‚éˆÊ’u’Ç‚¢‚©‚¯‚é‚à‚Ì‚È‚Ç
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)

		);
	// ‚Ç‚Ì”ÍˆÍ‚ğ‚Ç‚ê‚¾‚¯Œ©‚é‚©‰œsŠÜ‚ß
	camera.SetPerspedtiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

<<<<<<< HEAD
<<<<<<< HEAD


	 // •½sŒõŒ¹‚ğ’Ç‰Á
=======
	// •½sŒõŒ¹‚ğ’Ç‰Á
>>>>>>> parent of 0db33c3 (ä¸€åº¦ä¿å­˜ãƒ¯ãƒ¼ãƒ‹ãƒ³ã‚°å¤§é‡)
	{
		mainDirectionalLight = new Light(LightType::Directional);
		mainDirectionalLight->SetDirection({ 1,-1,-1 });
		LightManager::Instance().Register(mainDirectionalLight);
	}
<<<<<<< HEAD
	
	// “_ŒõŒ¹‚ğ’Ç‰Á
	{
		Light* light = new Light(LightType::Point);
		light->SetPosition(DirectX::XMFLOAT3(0, 1, 0));
		light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		LightManager::Instance().Register(light);
	}


	// ƒXƒ|ƒbƒgƒ‰ƒCƒg‚ğ’Ç‰Á
	{
		Light* light = new Light(LightType::Spot);
		light->SetPosition(DirectX::XMFLOAT3(-30, 1, 0));
		light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		light->SetDirection(DirectX::XMFLOAT3(+1, -1, 0));
		light->SetRange(40.0f);
		LightManager::Instance().Register(light);
	}

=======
>>>>>>> parent of 0db33c3 (ä¸€åº¦ä¿å­˜ãƒ¯ãƒ¼ãƒ‹ãƒ³ã‚°å¤§é‡)

	// V‚µ‚¢•`‰æƒ^[ƒQƒbƒg‚Ì¶¬
	{
		Graphics& graphics = Graphics::Instance();
		renderTarget = std::make_unique<RenderTarget>(static_cast<UINT>(graphics.GetScreenWidth())
			, static_cast<UINT>(graphics.GetScreenHeight())
			, DXGI_FORMAT_R8G8B8A8_UNORM);
		

	}

	// ƒVƒƒƒhƒEƒ}ƒbƒv—p‚É[“xƒXƒeƒ“ƒVƒ‹‚Ì¶¬
	{
		shadowmapDepthStencil = std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	}

	// ƒ|ƒXƒgƒvƒƒZƒX•`‰æƒNƒ‰ƒX¶¬
	{
		postprocessingRenderer = std::make_unique<PostprocessingRenderer>();
		// ƒV[ƒ“ƒeƒNƒXƒ`ƒƒ‚ğİ’è‚µ‚Ä‚¨‚­
		ShaderResourceViewData srvData;
		srvData.srv = renderTarget->GetShaderResourceView();
		srvData.width = renderTarget->GetWidth();
		srvData.height = renderTarget->GetHeight();
		postprocessingRenderer->SetSceneData(srvData);
	}
=======
>>>>>>> parent of 8a0ff20 (ã¨ã‚Šã‚ãˆãšã‚·ã‚§ãƒ¼ãƒ€ãƒ¼ç¶ºéº—ãªå¥´å…¥ã£ãŸ)



	// ƒQ[ƒWƒXƒvƒ‰ƒCƒg
	gauge = new Sprite();
}

// I—¹‰»
void SceneGame::Finalize()
{
	// ƒQ[ƒWƒXƒvƒ‰ƒCƒg
	if (this->gauge)
	{
		delete gauge;
		gauge = nullptr;
	}

	// ƒGƒlƒ~[I—¹‰»
	//EnemyManager::Instance().Clear();

	AfterimageManager::Instance().Clear();

	//// ƒJƒƒ‰ƒRƒ“ƒg[ƒ‰[I—¹‰»
	//if (this->cameraController)
	//{
	//	delete cameraController;
	//	cameraController = nullptr;
	//}

	// ƒvƒŒƒCƒ„[I—¹‰»
	//if (this->player)
	//{
	//	delete player;
	//	player = nullptr;
	//}
	// ƒXƒe[ƒWI—¹‰»
	//StageManager::instance().Clear();


}

// XVˆ—
void SceneGame::Update(float elapsedTime)
{
	
	// ƒJƒƒ‰ƒRƒ“ƒgƒ[ƒ‰[XVˆ—
	//DirectX::XMFLOAT3 target = player->GetPosition();
	//target.y += 0.5f;// ‘«Œ³‚©‚ç‚T‚OƒZƒ“ƒ`‚®‚ç‚¢
	//cameraController->SetTarget();// ƒvƒŒƒCƒ„[‚Ì˜“–‚½‚è
	//cameraController->Update(elapsedTime);

	// ƒXƒe[ƒWXVˆ—
	//StageManager::instance().Update(elapsedTime);
	
	// ƒvƒŒƒCƒ„[XVˆ—
	//player->Update(elapsedTime);
	ActorManager::Instance().Update(elapsedTime);

	// c‘œŒo‰ßŠÔ
	//AfterimageTime(elapsedTime);

	

	// c‘œƒXƒe[ƒgXV
	AfterimageManager::Instance().Update(elapsedTime);



	// ƒGƒlƒ~[XVˆ—
	//EnemyManager::Instance().Update(elapsedTime);

	// ƒGƒtƒFƒNƒgXVˆ—
	EffectManager::Instance().Update(elapsedTime);
}



// •`‰æˆ—
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

<<<<<<< HEAD
<<<<<<< HEAD
=======
	// ƒVƒƒƒhƒEƒ}ƒbƒv‚Ì•`‰æ
	RenderShadowmap();
>>>>>>> parent of 0db33c3 (ä¸€åº¦ä¿å­˜ãƒ¯ãƒ¼ãƒ‹ãƒ³ã‚°å¤§é‡)

	Render3DScene();



=======
>>>>>>> parent of 8a0ff20 (ã¨ã‚Šã‚ãˆãšã‚·ã‚§ãƒ¼ãƒ€ãƒ¼ç¶ºéº—ãªå¥´å…¥ã£ãŸ)
	// ‰æ–ÊƒNƒŠƒA•ƒŒƒ“ƒ_[ƒ^[ƒQƒbƒgİ’è
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// •`‰æˆ— 
	RenderContext rc;// •`‰æ‚·‚é‚½‚ß‚É•K—v‚Èî•ñ‚ğ‚Ü‚Æ‚ß‚½\‘¢‘Ì
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ƒ‰ƒCƒg•ûŒüi‰º•ûŒüj

	// ƒJƒƒ‰ƒpƒ‰ƒ[ƒ^İ’è
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	// ƒ‚ƒfƒ‹‚»‚ê‚¼‚ê‚ÅƒVƒF[ƒ_[‚ğ‚·‚é‚½‚ß‚É
	rc.deviceContext = dc;

<<<<<<< HEAD
<<<<<<< HEAD

	// ƒVƒƒƒhƒEƒ}ƒbƒv‚Ì•`‰æ
	RenderShadowmap();

	Render3DScene();


	// ƒ‰ƒCƒg‚Ìî•ñ‚ğ‹l‚ß‚Ş
	//LightManager::Instanes().PushRenderContext(rc);


     // ‘‚«‚İæ‚ğƒoƒbƒNƒoƒbƒtƒ@‚É•Ï‚¦‚ÄƒIƒtƒXƒNƒŠ[ƒ“ƒŒƒ“ƒ_ƒŠƒ“ƒO‚ÌŒ‹‰Ê‚ğ•`‰æ‚·‚é
	{
		ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

		// ‰æ–ÊƒNƒŠƒA•ƒŒƒ“ƒ_[ƒ^[ƒQƒbƒgİ’è
		FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f }; // RGBA(0.0~1.0)
		dc->ClearRenderTargetView(rtv, color);
		dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		dc->OMSetRenderTargets(1, &rtv, dsv);

		// UINT11
		// ƒrƒ…[ƒ|[ƒg‚Ìİ’è
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
		// ƒ|ƒXƒgƒvƒƒZƒX‚ğˆ—‚ğs‚¤
		postprocessingRenderer->Render(dc);
	}


=======
>>>>>>> parent of 8a0ff20 (ã¨ã‚Šã‚ãˆãšã‚·ã‚§ãƒ¼ãƒ€ãƒ¼ç¶ºéº—ãªå¥´å…¥ã£ãŸ)
=======
	// ƒ‰ƒCƒg‚Ìî•ñ‚ğ‹l‚ß‚Ş
	//LightManager::Instanes().PushRenderContext(rc);

>>>>>>> parent of 0db33c3 (ä¸€åº¦ä¿å­˜ãƒ¯ãƒ¼ãƒ‹ãƒ³ã‚°å¤§é‡)
	// 3Dƒ‚ƒfƒ‹•`‰æ
	{
		ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
		// ƒVƒF[ƒ_[‚É•K—v‚Èî•ñ‚ğ‘‚­
		shader->Begin(rc);// ƒVƒF[ƒ_[‚ÉƒJƒƒ‰‚Ìî•ñ‚ğ“n‚·
		// ƒXƒe[ƒW•`‰æ
		//StageManager::instance().Render(rc, shader);
	// ƒvƒŒƒCƒ„[•`‰æ
		//player->Render(rc, shader);

		// c‘œ•`‰æ
		//AfterimageManager::Instance().Render(dc, shader);

		// ƒGƒlƒ~[•`‰æ
		//EnemyManager::Instance().Render(rc, shader);

		shader->End(rc);

		//ActorManager::Instance().UpdateTransform();
<<<<<<< HEAD

		//ActorManager::Instance().Render(rc.view, rc.projection);
		//
		// ‘‚«‚İæ‚ğƒoƒbƒNƒoƒbƒtƒ@‚É•Ï‚¦‚ÄƒIƒtƒXƒNƒŠ[ƒ“ƒŒƒ“ƒ_ƒŠƒ“ƒO‚ÌŒ‹‰Ê‚ğ•`‰æ‚·‚é
		{
			ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
			ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

			// ‰æ–ÊƒNƒŠƒA•ƒŒƒ“ƒ_[ƒ^[ƒQƒbƒgİ’è
			FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f }; // RGBA(0.0~1.0)
			dc->ClearRenderTargetView(rtv, color);
			dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			dc->OMSetRenderTargets(1, &rtv, dsv);

			// UINT11
			// ƒrƒ…[ƒ|[ƒg‚Ìİ’è
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
			// ƒ|ƒXƒgƒvƒƒZƒX‚ğˆ—‚ğs‚¤
			postprocessingRenderer->Render(dc);
		}


		//ModelShader* mdlshader = graphics.GetShader(ModelShaderId::AfterImage);
		//
		//mdlshader->Begin(rc);// ƒVƒF[ƒ_[‚ÉƒJƒƒ‰‚Ìî•ñ‚ğ“n‚·
=======
		ActorManager::Instance().Render(rc.view, rc.projection);
		

		ModelShader* mdlshader = graphics.GetShader(ModelShaderId::AfterImage);
		
		mdlshader->Begin(rc);// ƒVƒF[ƒ_[‚ÉƒJƒƒ‰‚Ìî•ñ‚ğ“n‚·
>>>>>>> parent of 8a0ff20 (ã¨ã‚Šã‚ãˆãšã‚·ã‚§ãƒ¼ãƒ€ãƒ¼ç¶ºéº—ãªå¥´å…¥ã£ãŸ)

		ActorManager::Instance().Render(rc);
		

		ModelShader* mdlshader = graphics.GetShader(ModelShaderId::AfterImage);
		
		mdlshader->Begin(rc);// ƒVƒF[ƒ_[‚ÉƒJƒƒ‰‚Ìî•ñ‚ğ“n‚·


		
		// c‘œ•`‰æ
	    AfterimageManager::Instance().Render(rc, mdlshader);


		mdlshader->End(rc);
	}

	// 3DƒGƒtƒFƒNƒg•`‰æ
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

<<<<<<< HEAD

	// ƒfƒoƒbƒOî•ñ‚Ì•\¦
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
>>>>>>> parent of 0db33c3 (ä¸€åº¦ä¿å­˜ãƒ¯ãƒ¼ãƒ‹ãƒ³ã‚°å¤§é‡)
	// 3DƒfƒoƒbƒO•`‰æ
	{
		// “–‚½‚è”»’è‚ÌŒ`‚ğ‚¤‚Â
		// ƒvƒŒƒCƒ„[ƒfƒoƒbƒOƒvƒŠƒ~ƒeƒBƒu•`‰æ
		//player->DrawDebugPrimitive();

		// ƒGƒlƒ~[ƒfƒoƒbƒOƒvƒŠƒ~ƒeƒBƒu•`‰æ
		//EnemyManager::Instance().DrawDebugPrimitive();
		for (int i = 0; i >= EnemyManager::Instance().GetEnemyCount(); i++)
		{
			EnemyManager::Instance().GetEnemy(i)->GetComponent<EnemySlime>()->DrawDebugPrimitive();
		}
		// ƒ‰ƒCƒ“ƒŒƒ“ƒ_ƒ‰•`‰æÀs
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// ÀÛ‚Ì“–‚½‚è”»’è•`‰æ
		// ƒfƒoƒbƒOƒŒƒ“ƒ_ƒ‰•`‰æÀs
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	// 2DƒXƒvƒ‰ƒCƒg•`‰æ
	{
		//RenderEnemyGauge(dc, rc.view, rc.projection);
	}

	// 2DƒfƒoƒbƒOGUI•`‰æ
	{
		// ƒvƒŒƒCƒ„[ƒfƒoƒbƒO•`‰æ
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

	// X²
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

	// Y²
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

	// Z²
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
>>>>>>> parent of 0db33c3 (ä¸€åº¦ä¿å­˜ãƒ¯ãƒ¼ãƒ‹ãƒ³ã‚°å¤§é‡)
void SceneGame::Render3DScene()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = renderTarget->GetRenderTargetView().Get();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// ‰æ–ÊƒNƒŠƒA•ƒŒƒ“ƒ_[ƒ^[ƒQƒbƒgİ’è
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// ƒrƒ…[ƒ|[ƒg‚Ìİ’è
	D3D11_VIEWPORT vp = {};
	vp.Width = graphics.GetScreenWidth();
	vp.Height = graphics.GetScreenHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// •`‰æˆ—
	RenderContext rc;
	rc.deviceContext = dc;

	// ƒ‰ƒCƒg‚Ìî•ñ‚ğ‹l‚ß‚Ş
	LightManager::Instance().PushRenderContext(rc);

	// UNIT11
	// ƒ‰ƒCƒg‚Ìî•ñ‚ğ‚Â‚ß‚±‚Ş 
	for (int i = 0; i < ShadowmapCount; ++i)
	{
		rc.shadowMapData.shadowMap[i] = shadowmapDepthStencil[i]->GetShaderResourceView().Get();
		rc.shadowMapData.lightViewProjection[i] = lightViewProjection[i];
		rc.shadowMapData.shadowBias[i] = shadowBias[i];
	}
	rc.shadowMapData.shadowColor = shadowColor;

	// ƒJƒƒ‰ƒpƒ‰ƒ[ƒ^İ’è
	Camera& camera = Camera::Instance();
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();


	// 3Dƒ‚ƒfƒ‹•`‰æ
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

	// ƒfƒoƒbƒOƒvƒŠƒ~ƒeƒBƒu‚Ì•\¦
	{
		// ƒOƒŠƒbƒh•`‰æ
		//DrawGrid(dc, 20, 10.0f);

		// ƒ‰ƒCƒg‚ÌƒfƒoƒbƒOƒvƒŠƒ~ƒeƒBƒu‚Ì•`‰æ
		LightManager::Instance().DrawDebugPrimitive();

		// ƒ‰ƒCƒ“ƒŒƒ“ƒ_ƒ‰•`‰æÀs
		graphics.GetLineRenderer()->Render(dc, camera.GetView(), camera.GetProjection());

		// ƒfƒoƒbƒOƒŒƒ“ƒ_ƒ‰•`‰æÀs
		graphics.GetDebugRenderer()->Render(dc, camera.GetView(), camera.GetProjection());
	}
}

void SceneGame::RenderShadowmap()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = renderTarget->GetRenderTargetView().Get();
	ID3D11DepthStencilView* dsv = depthStencil->GetDepthStencilView().Get();
	// ‰æ–ÊƒNƒŠƒA
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);



	// ƒŒƒ“ƒ_[ƒ^[ƒQƒbƒgİ’è
	dc->OMSetRenderTargets(0, &rtv, dsv);

	// ƒrƒ…[ƒ|[ƒg‚Ìİ’è
	D3D11_VIEWPORT vp = {};
	vp.Width = static_cast<float>(shadowmapDepthStencil->GetWidth());
	vp.Height = static_cast<float> (shadowmapDepthStencil->GetHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// •`‰æˆ—
	RenderContext rc;
	rc.deviceContext = dc;

	// ƒJƒƒ‰ƒpƒ‰ƒ[ƒ^[İ’è
	{
		// •½sŒõŒ¹‚©‚çƒJƒƒ‰ˆÊ’u‚ğì¬‚µA‚»‚±‚©‚çŒ´“_‚ÌˆÊ’u‚ğŒ©‚é‚æ‚¤‚É‹üs—ñ‚ğ¶¬
		DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat3(&mainDirectionalLight->GetDirection());
		LightPosition = DirectX::XMVectorScale(LightPosition, -250.0f);
		DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(LightPosition,
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

		// ƒVƒƒƒhƒEƒ}ƒbƒv‚É•`‰æ‚µ‚½‚¢”ÍˆÍ‚ÌË‰es—ñ‚ğ¶¬
		DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(shadowDrawRect, shadowDrawRect, 0.1f,
			1000.0f);
		DirectX::XMStoreFloat4x4(&rc.view, V);
		DirectX::XMStoreFloat4x4(&rc.projection, P);
		DirectX::XMStoreFloat4x4(&lightViewProjeciton, V * P);
	}

	ActorManager::Instance().RenderShadowmap(rc.view, rc.projection);
}


=======
>>>>>>> parent of 8a0ff20 (ã¨ã‚Šã‚ãˆãšã‚·ã‚§ãƒ¼ãƒ€ãƒ¼ç¶ºéº—ãªå¥´å…¥ã£ãŸ)
// ƒGƒlƒ~[HPƒQ[ƒW•`‰æ
void SceneGame::RenderEnemyGauge(ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection
)
{


	//// ƒrƒ…[ƒ|[ƒg ‰æ–Ê‚ÌƒTƒCƒY“™
	//// ƒrƒ…[ƒ|[ƒg‚Æ‚Í2D‚Ì‰æ–Ê‚É•`‰æ”ÍˆÍ‚Ìw’è(ƒNƒŠƒbƒsƒ“ƒOw’è‚ào—ˆ‚é)ˆÊ’u‚ğw’è
	//D3D11_VIEWPORT viewport;
	//UINT numViewports = 1;
	//// ƒ‰ƒXƒ^ƒ‰ƒCƒU[ƒXƒe[ƒg‚ÉƒoƒCƒ“ƒh‚³‚ê‚Ä‚¢‚éƒrƒ…[ƒ|[ƒg”z—ñ‚ğæ“¾
	//dc->RSGetViewports(&numViewports, &viewport);

	//// •ÏŠ·s—ñ
	//DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	//DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	//// ƒ[ƒJƒ‹‚©‚çƒ[ƒ‹ƒh‚És‚­‚Æ‚«‚É‚¢‚é“z‘Šè‚Ìƒ|ƒWƒVƒ‡ƒ“‚ğ“n‚·B
	//DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();


	//// ‘S‚Ä‚Ì“G‚Ì“ªã‚ÉHPƒQ[ƒW‚ğ•\¦
	//EnemyManager& enemyManager = EnemyManager::Instance();
	//int enemyCount = enemyManager.GetEnemyCount();

	//for (int i = 0; i < enemyCount; ++i)
	//{

	//	Enemy* enemy = enemyManager.GetEnemy(i);
	//	// ƒGƒlƒ~[“ªã
	//	DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();
	//	worldPosition.y += enemy->GetHeight();

	//	// ƒ[ƒ‹ƒh‚©‚çƒXƒNƒŠ[ƒ“
	//	DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);


	//		// ƒQ[ƒW•`‰æ // ƒ[ƒ‹ƒh‚©‚çƒXƒNƒŠ[ƒ“

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
	//	// ƒXƒNƒŠ[ƒ“À•W
	//	DirectX::XMFLOAT3 scereenPosition;
	//	DirectX::XMStoreFloat3(&scereenPosition, ScreenPosition);

	//	// ƒQ[ƒW’·‚³
	//	const float gaugeWidth = 30.0f;
	//	const float gaugeHeight = 5.0f;

	//	float healthRate = enemy->GetHealth() / static_cast<float>(enemy->GetMaxHealth());


	//	if (scereenPosition.z < 0.0f || scereenPosition.z > 1.0f)continue;

	//	// 2DƒXƒvƒ‰ƒCƒg•`‰æ
	//	{
	//		// •`‰æ
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
	//		// {ˆÊ’u}{ƒTƒCƒY}{‰æ‘œ‚Ç‚±‚©‚ç}{‰æ‘œ‰½ˆ‚Ü‚Å}
	//		// dc , o”ÍˆÍpop
	//	}
	//
	//

	//	}


	//// ƒGƒlƒ~[”z’uˆ—
	//Mouse& mouse = Input::Instance().GetMouse();
	//if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	//{
	//	// ƒ}ƒEƒXƒJ[ƒ\ƒ‹À•W‚ğæ“¾
	//	DirectX::XMFLOAT3 scereenPosition;
	//	scereenPosition.x = static_cast<float>(mouse.GetPositionX());
	//	scereenPosition.y = static_cast<float>(mouse.GetPositionY());
	//	scereenPosition.z = 0.0f;

	//	// ƒŒƒC‚Ìn“_‚ğZo
	//	DirectX::XMVECTOR WorldPosition{};
	//	WorldPosition = DirectX::XMLoadFloat3(&scereenPosition);



	//	// ƒXƒNƒŠ[ƒ“î•ñ‚ğƒ[ƒ‹ƒh‹óŠÔ‚É
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
	//	// n“_
	//	DirectX::XMFLOAT3 rayStart;
	//	DirectX::XMStoreFloat3(&rayStart, WorldPosition);

	//	// ƒŒƒC‚ÌI“_‚ğZo
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

	//	// ƒŒƒCƒLƒƒƒXƒg
	//	HitResult hit;
	////	if (StageManager::instance().RayCast(rayStart, rayEnd, hit))
	////	{
	////		// “G‚ğ”z’u
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
	//// “®‚¢‚Ä‚¢‚½‚çc‘œ‚ªo‚éB
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

	//// c‘œo‚·ŠÔŠu
	//if (elapsedFrame <= 0)
	//{

	//	PlayerAfterimage* playerAfterimage = new PlayerAfterimage();
	//	// ”z’u‚µ‚Ä
	//	playerAfterimage->SetPosition({ player->GetPosition() });
	//	playerAfterimage->SetAngle(player->GetAngle());
	//	playerAfterimage->SetTransform(player->GetTransform());
	//	// “®‚©‚µ‚Ä
	//	playerAfterimage->GetModel()->UpdateTransform(player->GetTransform(),
	//		player->GetModel()->GetNodes());

	//	// c‘œ‚Ìo‚éŠÔŠuÄİ’è
	//	elapsedFrame = spawnafterimagetimemax;

	//	// “§–¾“x Å‘å’l‚ğÄİ’è
	//	afterImageAlpha = afterImageAlphaMax;
	//	
	//	// ¶‘¶ŠÔƒZƒbƒg
	//	playerAfterimage->SetSurvivalTime(afterImageAlpha);

	//	// Á‚¦‚é‚Ü‚Å‚ÌŠÔÄİ’è
	//	reduce = reduceMax;

	//	// Á‚¦‚é‚Ü‚Å‚ÌŠÔƒZƒbƒg
	//	playerAfterimage->SetReduce(reduce);

	//	// “o˜^
	//	AfterimageManager::Instance().Register(playerAfterimage);

	//	// c‹ÆƒXƒe[ƒgƒAƒjƒ[ƒVƒ‡ƒ“‚ÌŠÔ p¨
	//	AfterimageManager::Instance().GetAfterimage(
	//		AfterimageManager::Instance().GetAfterimageCount() - 1
	//	)->SetCurrentAnimationSeconds(
	//		player->GetCurrentANimationSeconds());
	//	//playerAfterimage->GetModel()->UpdateTransform(player->GetTransform());



	//	

	//}
}
