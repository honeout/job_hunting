#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
//#include "EnemyManager.h"
#include "EnemySlime.h"

#include "EffectManager.h"

#include "Input/Input.h"

#include <DirectXMath.h>

#include "StageMain.h"


#include "Actor.h"
#include "Movement.h"
#include "HP.h"
#include "ModelControll.h"
#include "ProjectileStraight.h"
#include "Transform.h"
#include "Ui.h"
#include "TransForm2D.h"
#include "UiManager.h"

#include "LightManager.h"

#include "SceneLoading.h"
#include "SceneManager.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"

#include "UiTime.h"

#include "Mp.h"

// シャドウマップのサイズ
static const UINT SHADOWMAP_SIZE = 2048;


// 初期化
void SceneGame::Initialize()
{
	// 行動範囲
	{
		
		minPos.x = -30;
		minPos.y = -3.525f;
		minPos.z = -30;

		maxPos.x = 30;
		maxPos.y = 3.625f;
		maxPos.z = 30;
	}

	//// ステージ初期化
	//{
	//	const char* filename = "Data/Model/ExampleStage/M_stage .mdl";
	//	std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
	//	actor->AddComponent<ModelControll>();
	//	actor->GetComponent<ModelControll>()->LoadModel(filename);
	//	actor->SetName("StageMain");
	//	actor->AddComponent<Transform>();

	//	actor->GetComponent<Transform>()->
	//		SetPosition(DirectX::XMFLOAT3(0, -25, 0));

	//	actor->GetComponent<Transform>()->
	//		SetAngle(DirectX::XMFLOAT3(0, 0, 0));

	//	actor->GetComponent<Transform>()->
	//		SetScale(DirectX::XMFLOAT3(1, 1, 1));




	//	actor->AddComponent<StageMain>();

	//	// 影シェーダー
	//	actor->GetComponent<StageMain>()->SetIsRimRightning(0);

	//	// 解像度
	//	actor->GetComponent<StageMain>()->SetTexcoordMult(20);

	//	// スペキュラー
	//	actor->GetComponent<StageMain>()->SetIsSpecular(0);


	//	// これが２Dかの確認
	//	bool check2d = false;
	//	actor->SetCheck2d(check2d);

	//	StageManager::Instance().Register(actor);




	//}


	// ステージ初期化
	{
		const char* filename = "Data/Model/ExampleStage/stageNotRuby.mdl";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->AddComponent<ModelControll>();
		actor->GetComponent<ModelControll>()->LoadModel(filename);
		actor->SetName("StageMain");
		actor->AddComponent<Transform>();

		actor->GetComponent<Transform>()->
			SetPosition(DirectX::XMFLOAT3(0, -25, 0));

		actor->GetComponent<Transform>()->
			SetAngle(DirectX::XMFLOAT3(0, 0, 0));

		actor->GetComponent<Transform>()->
			SetScale(DirectX::XMFLOAT3(1, 1, 1));




		actor->AddComponent<StageMain>();

		// 影シェーダー
		actor->GetComponent<StageMain>()->SetIsRimRightning(0);

		// 解像度
		actor->GetComponent<StageMain>()->SetTexcoordMult(20);

		// スペキュラー
		actor->GetComponent<StageMain>()->SetIsSpecular(0);


		// これが２Dかの確認
		bool check2d = false;
		actor->SetCheck2d(check2d);

		StageManager::Instance().Register(actor);




	}



	// ステージルビー初期化
	{
		const char* filename = "Data/Model/ExampleStage/RubyStage.mdl";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->AddComponent<ModelControll>();
		actor->GetComponent<ModelControll>()->LoadModel(filename);
		actor->SetName("StageRuby");
		actor->AddComponent<Transform>();

		actor->GetComponent<Transform>()->
			SetPosition(DirectX::XMFLOAT3(0, -17.85f, 0));

		actor->GetComponent<Transform>()->
			SetAngle(DirectX::XMFLOAT3(0, 0, 0));

		actor->GetComponent<Transform>()->
			SetScale(DirectX::XMFLOAT3(1, 1, 1));




		actor->AddComponent<StageMain>();

		// 影シェーダー
		actor->GetComponent<StageMain>()->SetIsRimRightning(1);

		// 解像度
		actor->GetComponent<StageMain>()->SetTexcoordMult(1);

		// スペキュラー
		actor->GetComponent<StageMain>()->SetIsSpecular(1);


		// これが２Dかの確認
		bool check2d = false;
		actor->SetCheck2d(check2d);

		StageManager::Instance().Register(actor);



	}



	////player = new Player;
	{
		// プレイヤー初期化
		//const char* filename = "Data/Model/Jammo/Jammo.mdl";
		const char* filename = "Data/Model/Player/Maria.mdl";

		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->AddComponent<ModelControll>();
		actor->GetComponent<ModelControll>()->LoadModel(filename);
		actor->SetName("Player");
		actor->AddComponent<Transform>();

		actor->GetComponent<Transform>()->
			SetPosition(DirectX::XMFLOAT3(0, 0, -10));

		actor->GetComponent<Transform>()->
			SetAngle(DirectX::XMFLOAT3(0, 0, 0));

		actor->GetComponent<Transform>()->
			SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
		actor->AddComponent<Movement>();


		actor->GetComponent<Movement>()->SetArea(minPos, maxPos);

		actor->AddComponent<HP>();
		std::shared_ptr<HP> hp = actor->GetComponent<HP>();
		int life = 0;
		hp->SetLife(life);

		actor->AddComponent<Mp>();
		std::shared_ptr<Mp> mp = actor->GetComponent<Mp>();
		int mpMax = 50;
		mp->SetMaxMagic(mpMax);


		actor->AddComponent<Player>();
		actor->AddComponent<Collision>();

		// これが２Dかの確認
		bool check2d = false;
		actor->SetCheck2d(check2d);

		PlayerManager::Instance().Register(actor);



	}


	// 敵
	{

		const char* filename = "Data/Model/Boss/BossAnim8.mdl";
		//const char* filename = "Data/Model/Slime/Slime.mdl";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->AddComponent<ModelControll>();
		actor->GetComponent<ModelControll>()->LoadModel(filename);
		actor->SetName("ClestaleBoss");
		actor->AddComponent<Transform>();

		actor->GetComponent<Transform>()->
			SetPosition(DirectX::XMFLOAT3(0, 0, 1));

		actor->GetComponent<Transform>()->
			SetAngle(DirectX::XMFLOAT3(0, 0, 0));

		actor->GetComponent<Transform>()->
			SetScale(DirectX::XMFLOAT3(0.06f, 0.06f, 0.06f));
		actor->AddComponent<Movement>();

	
		// 行動範囲設定
		actor->GetComponent<Movement>()->SetArea(minPos, maxPos);
		actor->AddComponent<HP>();
		std::shared_ptr<HP> hp = actor->GetComponent<HP>();
		int life = 2;
		hp->SetLife(life);
		actor->AddComponent<Collision>();
		actor->AddComponent<EnemySlime>();

		// これが２Dかの確認
		bool check2d = false;
		actor->SetCheck2d(check2d);

		EnemyManager::Instance().Register(actor);

		//

	}

	// UI PlayerHP
	{
		const char* filename = "Data/Sprite/HP.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerHP");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 932, 421 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 358,310 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);


		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(true);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI EnemyHP
	{
		const char* filename = "Data/Sprite/enemyHPber.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("EnemyHPSet");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 900, -100 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 376,329 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(true);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}


	// UI PlayerCommandAttack
	{
		const char* filename = "Data/Sprite/コマンド　非選択攻撃.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandAttack");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 0, 310 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 358,310 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(true);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandMagick
	{
		const char* filename = "Data/Sprite/コマンド　非選択魔法.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandMagick");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 0, 390 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 358,310 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(true);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}


	// UI PlayerCommandAttackCheck
	{
		const char* filename = "Data/Sprite/コマンド　選択攻撃.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandAttackCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 0, 310 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 358,310 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(false);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandMagickCheck
	{
		const char* filename = "Data/Sprite/コマンド　選択魔法.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandMagickCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 0, 390 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 358,310 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(false);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandFire
	{
		const char* filename = "Data/Sprite/コマンド　非選択ファイアー.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandFire");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 179, 370 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 358,310 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(true);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandFireCheck
	{
		const char* filename = "Data/Sprite/コマンド　選択ファイアー.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandFireCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 179, 370 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 358,310 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(false);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}
	// UI PlayerCommandRigtning
	{
		const char* filename = "Data/Sprite/サンダラ　押し込み.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandRigtning");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 179, 455 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 358,310 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(true);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandRigtningCheck
	{
		const char* filename = "Data/Sprite/サンダラ.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandRigtningCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 179, 455 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 358,310 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(false);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandIce
	{
		const char* filename = "Data/Sprite/ブリザラ　押し込み.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandIce");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 179, 540 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 358,310 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(true);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandIceCheck
	{
		const char* filename = "Data/Sprite/ブリザラ.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandIceCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 179, 540 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 358,310 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		bool drawCheck = true;
		ui->SetDrawCheck(drawCheck);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}


	// UI PlayerCommandSpeciulCharge01
	{
		const char* filename = "Data/Sprite/矢印.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpeciulCharge");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 110, 250 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 160,170 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		bool drawCheck = false;
		ui->SetDrawCheck(drawCheck);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandSpeciulCharge02
	{
		const char* filename = "Data/Sprite/矢印.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpeciulCharge");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 158, 250 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 160,170 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		bool drawCheck = false;
		ui->SetDrawCheck(drawCheck);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandSpeciulCharge03
	{
		const char* filename = "Data/Sprite/矢印.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpeciulCharge");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 205, 200 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 160,170 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		bool drawCheck = false;
		ui->SetDrawCheck(drawCheck);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandSpeciulShurashu
	{
		const char* filename = "Data/Sprite/特殊技スラッシュ.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpeciulShurashu");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 0, 200 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 160,170 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		bool drawCheck = false;
		ui->SetDrawCheck(drawCheck);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandSpeciulShurashuPushu
	{
		const char* filename = "Data/Sprite/特殊技スラッシュ押し込み.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpeciulShurashuPushu");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 0, 200 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 160,170 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		bool drawCheck = false;
		ui->SetDrawCheck(drawCheck);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandSpeciulFrame
	{
		const char* filename = "Data/Sprite/特殊技フレイム.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpeciulFrame");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 0, 200 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 160,170 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		bool drawCheck = false;
		ui->SetDrawCheck(drawCheck);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerCommandSpeciulFramePushu
	{
		const char* filename = "Data/Sprite/特殊技フレイム押し込み.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpeciulThanderPushu");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 0, 250 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 160,170 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		bool drawCheck = false;
		ui->SetDrawCheck(drawCheck);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}


	// UI PlayerCommandSpeciulIce
	{
		const char* filename = "Data/Sprite/特殊技フレイム.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpeciulIce");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 0, 200 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 160,170 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		bool drawCheck = false;
		ui->SetDrawCheck(drawCheck);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}



	// UI PlayerCommandSpeciulThander
	{
		const char* filename = "Data/Sprite/特殊技フレイム.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerCommandSpeciulFrame");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 0, 200 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 160,170 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		bool drawCheck = false;
		ui->SetDrawCheck(drawCheck);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI PlayerHP
	{
		const char* filename = "Data/Sprite/player_status.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("PlayerHP");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 979, 593 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 243,30 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);


		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		bool drawCheck = true;
		ui->SetDrawCheck(drawCheck);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}


	// UI EnemyHP
	{
		const char* filename = "Data/Sprite/enemy_status.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("EnemyHP");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();

		DirectX::XMFLOAT2 pos = { 916, 31 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);

		DirectX::XMFLOAT2 scale = { 303,30 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);


		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		bool drawCheck = true;
		ui->SetDrawCheck(drawCheck);


		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}


	// UI EnemyHP 残機１
	{
		const char* filename = "Data/Sprite/enemy_status.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("EnemyHPLife1");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();

		DirectX::XMFLOAT2 pos = { 1142, 79 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);

		DirectX::XMFLOAT2 scale = { 27,17 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);


		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		bool drawCheck = true;
		ui->SetDrawCheck(drawCheck);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}


	// UI EnemyHP 残機2
	{
		const char* filename = "Data/Sprite/enemy_status.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("EnemyHPLife2");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();

		DirectX::XMFLOAT2 pos = { 1185, 79 };
		transform2D->SetPosition(pos);

		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);

		DirectX::XMFLOAT2 scale = { 27,17 };
		transform2D->SetScale(scale);

		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		bool drawCheck = true;
		ui->SetDrawCheck(drawCheck);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}
	
	// UI UiTime 制限時間１桁
	{
		const char* filename = "Data\\Font\\fonts\\font4.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UiTime 1digit");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();

		// 位置
		DirectX::XMFLOAT2 pos = { 112, 31 };
		//DirectX::XMFLOAT2 pos = { 32, 31 };
		transform2D->SetPosition(pos);

		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 60 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);

		DirectX::XMFLOAT2 scale = { 43,49 };
		transform2D->SetScale(scale);

		DirectX::XMFLOAT2 texScale = { 20,20 };
		transform2D->SetTexScale(texScale);

		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<UiTime>();
		// 描画チェック
		std::shared_ptr<UiTime> ui = actor->GetComponent<UiTime>();
		bool drawCheck = true;
		ui->SetDrawCheck(drawCheck);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		// 制限時間
		int timeMax = 180;
		//int timeMax = 10;
		ui->SetTimeMax(timeMax);

		int countDown = ui->GetTime() * ui->GetTimeMax();
		ui->SetCountDown(countDown);

		// 桁
		int digit = 3;
		ui->SetDigit(digit);

		UiManager::Instance().Register(actor);
	}


	// UI sight
	{
		const char* filename = "Data/Sprite/Loading.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("sight");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();

		DirectX::XMFLOAT2 pos = { 640, 360 };
		transform2D->SetPosition(pos);

		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);

		DirectX::XMFLOAT2 scale = { 50,50 };
		transform2D->SetScale(scale);

		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		bool drawCheck = true;
		ui->SetDrawCheck(drawCheck);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// UI sightCheck
	{
		const char* filename = "Data/Sprite/Loading_move.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("sightCheck");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();

		DirectX::XMFLOAT2 pos = { 616, 335 };
		transform2D->SetPosition(pos);

		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);

		DirectX::XMFLOAT2 scale = { 120,120 };
		transform2D->SetScale(scale);

		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		bool drawCheck = false;
		ui->SetDrawCheck(drawCheck);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}


	// UI MP
	{
		const char* filename = "Data/Sprite/enemyHPberGreen.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("MP");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();
		DirectX::XMFLOAT2 pos = { 932, 515 };
		transform2D->SetPosition(pos);
		// 元の位置
		DirectX::XMFLOAT2 texPos = { 0, 0 };
		transform2D->SetTexPosition(texPos);

		float angle = 0;
		transform2D->SetAngle(angle);
		DirectX::XMFLOAT2 scale = { 376,329 };
		transform2D->SetScale(scale);
		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		// UI揺らす範囲を指定揺らす場合
		int max = (int)pos.y + 3;
		int min = (int)pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<Ui>();
		// 描画チェック
		std::shared_ptr<Ui> ui = actor->GetComponent<Ui>();
		ui->SetDrawCheck(true);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);

		UiManager::Instance().Register(actor);
	}

	// カメラ初期設定 見える位置追いかけるものなど
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)

	);
	// どの範囲をどれだけ見るか奥行含め
	camera.SetPerspedtiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);



	// 平行光源を追加
	{
		mainDirectionalLight = new Light(LightType::Directional);
		//mainDirectionalLight = std::make_unique<Light>(LightType::Directional);
		mainDirectionalLight->SetDirection({ 1,-3,-1 });
		//ambientLightColor = { 0.2f,0.2f,0.2f,0.2f };
		LightManager::Instanes().Register(mainDirectionalLight);
	}

	// 点光源を追加
	{
		Light* light = new Light(LightType::Point);
		light->SetPosition(DirectX::XMFLOAT3(-40.000, -3, 1));
		light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		light->SetRange(lightRange);
		LightManager::Instanes().Register(light);

		Light* lightOne = new Light(LightType::Point);
		lightOne->SetPosition(DirectX::XMFLOAT3(40.000, -3, 1));
		lightOne->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		lightOne->SetRange(lightRange);
		LightManager::Instanes().Register(lightOne);


		Light* lightSeconde = new Light(LightType::Point);
		lightSeconde->SetPosition(DirectX::XMFLOAT3(1, -3, -40.000));
		lightSeconde->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		lightSeconde->SetRange(lightRange);
		LightManager::Instanes().Register(lightSeconde);


		Light* lightTherd = new Light(LightType::Point);
		lightTherd->SetPosition(DirectX::XMFLOAT3(1, -3, 40.000));
		lightTherd->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		lightTherd->SetRange(lightRange);
		LightManager::Instanes().Register(lightTherd);




	}

	// 新しい描画ターゲットの生成
	{
		Graphics& graphics = Graphics::Instance();
		//renderTarget = new RenderTarget(static_cast<UINT>(graphics.GetScreenWidth())
		renderTarget = std::make_unique<RenderTarget>(static_cast<UINT>(graphics.GetScreenWidth())
			, static_cast<UINT>(graphics.GetScreenHeight())
			, DXGI_FORMAT_R8G8B8A8_UNORM);
	}


	// シャドウマップ用に深度ステンシルの生成
	{
		shadowmapDepthStencil = std::make_unique<DepthStencil>(SHADOWMAP_SIZE, SHADOWMAP_SIZE);
	}

	// ポストプロセス描画クラス生成
	{
		postprocessingRenderer = std::make_unique<PostprocessingRenderer>();
		// シーンテクスチャを設定しておく
		ShaderResourceViewData srvData;
		srvData.srv = renderTarget->GetShaderResourceView();
		srvData.width = renderTarget->GetWidth();
		srvData.height = renderTarget->GetHeight();
		//srvData.width = 100;
		//srvData.height = 100;
		postprocessingRenderer->SetSceneData(srvData);
	}


	bloomData.luminanceExtractionData.threshold = 0.41f;
	bloomData.luminanceExtractionData.intensity = 1.6f;

	bloomData.gaussianFilterData.kernelSize = 15;
	bloomData.gaussianFilterData.deviation = 8.3f;

}


// 終了化
void SceneGame::Finalize()
{

	ActorManager::Instance().Clear();

	LightManager::Instanes().Clear();

	EnemyManager::Instance().Clear();

	PlayerManager::Instance().Clear();

	StageManager::Instance().Clear();

	UiManager::Instance().Clear();

	ProjectileManager::Instance().Clear();

}

// 更新処理
void SceneGame::Update(float elapsedTime)
{

	float dlayTime = dlayTimeCheck ?  elapsedTime / 2 : elapsedTime;

	ActorManager::Instance().Update(dlayTime);



	// エフェクト更新処理
	EffectManager::Instance().Update(dlayTime);


	// エフェクトしてシェーダーを使う
	PlayEffectsShaders(dlayTime);

	// シーン切り替え
	{
		
		for (int i = 0; i < PlayerManager::Instance().GetPlayerCount(); ++i)
		{
			
			vignette_smoothness = 0.0f;
			vignette_intensity = 0.0f;

			if (PlayerManager::Instance().GetPlayer(i)->GetComponent<Player>()->GetFlashOn())
			{
				bool flashOn = false;
				PlayerManager::Instance().GetPlayer(i)->GetComponent<Player>()->SetFlashOn(flashOn);
				// フラッシュ開始
				shaderPlayStateTimer = shaderPlayStateTimerMax;
			}
			
			if (PlayerManager::Instance().GetPlayer(i)->GetComponent<HP>()->HealthPinch() && !PlayerManager::Instance().GetPlayer(i)->GetComponent<HP>()->GetDead())
			{
				vignette_smoothness = 0.5f;
				vignette_intensity = 0.4f;
			}

			// ブラーを×
			int enemyCount = EnemyManager::Instance().GetEnemyCount();
			if (enemyCount > 0)
			{
				std::shared_ptr<EnemySlime> enemyid = EnemyManager::Instance().GetEnemy(enemyCount - 1)->GetComponent<EnemySlime>();
				shaderBlurStateTimer = enemyid->GetBlurCheck() ? shaderBlurStateTimerMax : shaderBlurStartStateTimer;
				bool blurCheck = false;
				enemyid->SetBlurCheck(blurCheck);

			}
			
			// 死んだ瞬間
			if (PlayerManager::Instance().GetPlayer(i)->GetComponent<HP>()->GetDead() && !sceneChengeCheckDead)
			{
				
				PlayerManager::Instance().GetPlayer(i)->GetComponent<Player>()->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Death));

				vignette_smoothness = 0.0f;

				vignette_intensity = 0.0f;

				sceneChengeCheckDead = true;

				colorGradingData.brigthness = 2.0f;
			}
			// 演出終了
			if (!PlayerManager::Instance().GetPlayer(i)->GetComponent<HP>()->GetDead() && sceneChengeCheckDead)
				// ゲームオーバー
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGameOver));
		}

		for (int i = 0; i < EnemyManager::Instance().GetEnemyCount(); ++i)
		{
			std::shared_ptr<HP> hp = EnemyManager::Instance().GetEnemy(i)->GetComponent<HP>();
			if (hp->GetDead())
			{
				vignette_smoothness = 0.0f;

				vignette_intensity = 0.0f;

				colorGradingData.brigthness = 3.0f;
				hp->SetDead(false);
				//ActorManager::Instance().Clear();

				EnemyManager::Instance().GetEnemy(i)->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState(static_cast<int>(EnemySlime::State::Death));


				

			}
			if (hp->GetHealth() <= 0 && hp->GetLife() >= 0)
			{
				// hpを回復
				hp->SetHealth(hp->GetMaxHealth());

				// 残機を減らす
				//hp->SetLife(hp->GetLife() - 1);
			}
			// クリア
			if (EnemyManager::Instance().GetEnemy(i)->GetComponent<EnemySlime>()->GetClearCheck())
			{
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGameClear));
			}
		}
		for (int i = 0; i < UiManager::Instance().GetUiesCount(); ++i)
		{
			// ゲームオーバー時間制限
			std::shared_ptr<UiTime> uiTime = UiManager::Instance().GetUies((int)UiManager::UiCount::Time)->GetComponent<UiTime>();
			if (uiTime->GetTimeUp())
			{
				colorGradingData.brigthness = 3.0f;
				// ゲームオーバー
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGameOver));
			}
		}

	}

}



// 描画処理
void SceneGame::Render()
{

	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	//// シャドウマップの描画
	RenderShadowmap();

	Render3DScene();



	// 書き込み先をバックバッファに変えてオフスクリーンレンダリングの結果を描画する
	{
		ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
		ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

		// 画面クリア＆レンダーターゲット設定
		FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f }; // RGBA(0.0~1.0)
		dc->ClearRenderTargetView(rtv, color);
		dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		dc->OMSetRenderTargets(1, &rtv, dsv);


		// UINT11
        // ビューポートの設定
		D3D11_VIEWPORT vp = {};
		vp.Width = graphics.GetScreenWidth();
		vp.Height = graphics.GetScreenHeight();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		dc->RSSetViewports(1, &vp);

		RenderContext rc;
		
		rc.deviceContext = dc;

		rc.radialBlurData = radialBlurData;

		rc.colorGradingData = colorGradingData;

		rc.bloomData = bloomData;


		// 周辺減光
		rc.vignetteData.color = vignette_color;
		rc.vignetteData.center = vignette_center;
		rc.vignetteData.intensity = vignette_intensity;
		rc.vignetteData.smoothness = vignette_smoothness;
		rc.vignetteData.rounded = vignette_rounded;
		rc.vignetteData.roundness = vignette_roundness;

	
		

		// ポストプロセスを処理を行う
		postprocessingRenderer->Render(rc);
	}


	// 2Dスプライト描画
	{
		// 描画処理
		RenderContext rc;
		rc.deviceContext = dc;

		// カメラパラメータ設定
		Camera& camera = Camera::Instance();
		rc.viewPosition.x = camera.GetEye().x;
		rc.viewPosition.y = camera.GetEye().y;
		rc.viewPosition.z = camera.GetEye().z;
		rc.viewPosition.w = 1;
		rc.view = camera.GetView();
		rc.projection = camera.GetProjection();
		
		SpriteShader* shaderUi = graphics.GetShader(SpriteShaderId::Default);

		ActorManager::Instance().Render2D(rc, shaderUi);
	}


#ifdef _DEBUG
		 //2DデバッグGUI描画
	{
		//ImGui::Separator();
		 //UNIT11
		if (ImGui::TreeNode("shadowmap"))
		{
			ImGui::SliderFloat("DrawRect", &shadowDrawRect, 1.0f, 2048.0f);
			ImGui::ColorEdit3("Color", &shadowColor.x);
			ImGui::SliderFloat("Bias", &shadowBias, 0.0f, 0.1f);
			ImGui::SliderFloat("lightPosition", &lightPositionScale, -13.0f, 13.0f);
			ImGui::Text("texture");
			ImGui::Image(shadowmapDepthStencil->GetShaderResourceView().Get(), { 256,256 }, { 0,0 }, { 1,1 },
				{ 1,1,1,1 });
			ImGui::TreePop();
		}
		ImGui::Separator();
		 //UNIT09
		if (ImGui::TreeNode("ColorGrading"))
		{
			ImGui::SliderFloat("hueShift", &colorGradingData.hueShift, 0.0f, +360.0f);
			ImGui::SliderFloat("saturation", &colorGradingData.saturation, 0.0f, +2.0f);
			ImGui::SliderFloat("brigtness", &colorGradingData.brigthness, 0.0f, +10.0f);

			ImGui::TreePop();
		}
		ImGui::Separator();


		if (ImGui::TreeNode("RadialBlur"))
		{
			ImGui::SliderFloat("radius", &radialBlurData.radius, 0.0f, 200.0f);
			ImGui::SliderInt("samplingCount", &radialBlurData.samplingCount, 0, 10);
			ImGui::SliderFloat2("center", &radialBlurData.center.x, 0.0f, 1.0f);


			ImGui::SliderFloat("mask radius", &radialBlurData.mask_radius, 0.0f, 600.0f);

			ImGui::TreePop();
		}
		ImGui::Separator();

		if (ImGui::TreeNode("BloomData"))
		{
			ImGui::SliderFloat("threshold", &bloomData.luminanceExtractionData.threshold, 0.0f, 1.0f);
			ImGui::SliderFloat("intensity", &bloomData.luminanceExtractionData.intensity, 0.0f, 10.0f);
			ImGui::SliderInt("kernelSize", &bloomData.gaussianFilterData.kernelSize, 1, MaxkernelSize - 1);
			ImGui::SliderFloat("deviation", &bloomData.gaussianFilterData.deviation, 1.0f, 10.0f);
			ImGui::TreePop();

		}
		ImGui::Separator();
	}

	LightManager::Instanes().DrawDebugGUI();

		postprocessingRenderer->DrawDebugGUI();
		//	ImGui::Separator();
		//	LightManager::Instanes().DrawDebugGUI();
		//}
#endif // _DEBUG

	
}

void SceneGame::Render3DScene()
{

	
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = renderTarget->GetRenderTargetView().Get();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// ビューポートの設定
	D3D11_VIEWPORT vp = {};
	vp.Width = graphics.GetScreenWidth();
	vp.Height = graphics.GetScreenHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// 描画処理
	RenderContext rc;
	rc.deviceContext = dc;


	// ライトの情報を詰め込む
	LightManager::Instanes().PushRenderContext(rc);

	// UNIT11
	// ライトの情報をつめこむ 
	rc.shadowMapData.shadowMap = shadowmapDepthStencil->GetShaderResourceView().Get();
	rc.shadowMapData.lightViewProjection = lightViewProjeciton;
	rc.shadowMapData.shadowColor = shadowColor;
	rc.shadowMapData.shadowBias = shadowBias;

	

	// カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();


	// 3Dモデル描画
	{

		ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);

		ActorManager::Instance().Render(rc, shader);

	}

	// 3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

#ifdef _DEBUG

	// デバッグプリミティブの表示
	{
		// グリッド描画
		//DrawGrid(dc, 20, 10.0f);

		for (int i = 0; i < EnemyManager::Instance().GetEnemyCount(); i++)
		{
			EnemyManager::Instance().GetEnemy(i)->GetComponent<EnemySlime>()->DrawDebugPrimitive();
		}

		// ライトのデバッグプリミティブの描画
		LightManager::Instanes().DrawDebugPrimitive();

		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, camera.GetView(), camera.GetProjection());

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, camera.GetView(), camera.GetProjection());
	}
#endif // _DEBUG

}

void SceneGame::RenderShadowmap()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11DepthStencilView* dsv = shadowmapDepthStencil->GetDepthStencilView().Get();

	// 画面クリア
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (!mainDirectionalLight)
		return;

	dc->OMSetRenderTargets(0, &rtv, dsv);

	// ビューポートの設定
	D3D11_VIEWPORT vp = {};
	vp.Width = static_cast<float>(shadowmapDepthStencil->GetWidth());
	vp.Height = static_cast<float> (shadowmapDepthStencil->GetHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	dc->RSSetViewports(1, &vp);

	// 描画処理
	RenderContext rc;
	rc.deviceContext = dc;

	// カメラパラメーター設定
	{
		// 平行光源からカメラ位置を作成し、そこから原点の位置を見るように視線行列を生成
		DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat3(&mainDirectionalLight->GetDirection());
		LightPosition = DirectX::XMVectorScale(LightPosition, -3.0f);
		//LightPosition = DirectX::XMVectorScale(LightPosition, lightPositionScale);
		DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(LightPosition,
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

		// シャドウマップに描画したい範囲の射影行列を生成
		DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(shadowDrawRect, shadowDrawRect, 0.1f,
			1000.0f);
		DirectX::XMStoreFloat4x4(&rc.view, V);
		DirectX::XMStoreFloat4x4(&rc.projection, P);
		DirectX::XMStoreFloat4x4(&lightViewProjeciton, V * P);
	}

	// シェーダー
	{
		ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);

		ActorManager::Instance().Render(rc, shader);
	}
}

void SceneGame::PlayEffectsShaders(float elapsedTime)
{
	Graphics& graphics = Graphics::Instance();

	shaderPlayStateTimer -= elapsedTime;
	// エフェクト
	if (shaderPlayStateTimer > 0)
	{
		// 画面白ボケ開始
		colorGradingData.brigthness = 
			(colorGradingData.brigthness + FLT_EPSILON) > (colorGradingDataBrigthnessMax - FLT_EPSILON )?
			colorGradingData.brigthness : colorGradingData.brigthness + (0.01f + elapsedTime);
		// 画面真ん中
		radialBlurData.center = { 0.5f ,0.5f };
		// 画面ブラー
		float radislBlurRadius = 200;
		radialBlurData.radius = 
			radialBlurData.radius + FLT_EPSILON > radialBlurDataRadislBlurRadiusMax - FLT_EPSILON ?
			radialBlurData.radius : radialBlurData.radius + (5 + elapsedTime);
			//radialBlurData.radius + FLT_EPSILON > radislBlurRadius - FLT_EPSILON ?
			//radialBlurData.radius + (5 + elapsedTime) : radialBlurData.radius;
		// 歪み具合
		float radislBlurSamplingCount = 10;
		radialBlurData.samplingCount = radislBlurSamplingCount;
		// 自分が見える範囲
		float radislBlurMaskRadius = 300;
		radialBlurData.mask_radius = radislBlurMaskRadius;
		if (!radialBlurData.radius + FLT_EPSILON > radialBlurDataRadislBlurRadiusMax - FLT_EPSILON)
		{
			radialBlurData.radius;
		}

		if (radialBlurData.radius + FLT_EPSILON > radialBlurDataRadislBlurRadiusMax - FLT_EPSILON)
		{
			radialBlurData.radius;
		}

		// ブルーム関係
		{
			bloomData.luminanceExtractionData.threshold;
			bloomData.luminanceExtractionData.intensity;

			bloomData.gaussianFilterData.kernelSize;
			bloomData.gaussianFilterData.deviation;
		}
		
	}
	else
	{
		// 画面白ボケ
		float colorGradingBrigthness = 0.8f;
		colorGradingData.brigthness = colorGradingBrigthness + FLT_EPSILON > colorGradingData.brigthness - FLT_EPSILON ? colorGradingBrigthness : colorGradingData.brigthness - (0.01f + elapsedTime);

		//shaderPlayStateTimer = shaderPlayStateTimerMax;

		// ブラー範囲
		float radislBlurRadius = 0;
		radialBlurData.radius = radislBlurRadius - FLT_EPSILON < radialBlurData.radius + FLT_EPSILON ? radialBlurData.radius - (5 + elapsedTime) : radislBlurRadius - FLT_EPSILON;

		//float radislBlurSamplingCount = 10;
		//radialBlurData.samplingCount = radislBlurSamplingCount;
		//float radislBlurMaskRadius = 30;
		//radialBlurData.mask_radius = radislBlurMaskRadius;
		// ブラーのかからない範囲
		float radislBlurMaskRadiusNormal = 600;
		float radislBlurMaskRadiusEffectOn = 300;
		radialBlurData.mask_radius = radislBlurRadius - FLT_EPSILON < radialBlurData.radius + FLT_EPSILON ? radislBlurMaskRadiusEffectOn : radislBlurMaskRadiusNormal;
	}

	// 画面スロー
	bool hitCheck = PlayerManager::Instance().GetPlayer(0)->GetComponent<Player>()->GetHitCheck();

	if (hitCheck)
	{
		dlayStateTimer = dlayStateTimerMax;
		PlayerManager::Instance().GetPlayer(0)->GetComponent<Player>()->SetHitCheck(false);
	}
	if (dlayStateTimer - FLT_EPSILON > 0.0f + FLT_EPSILON)
	{
		dlayStateTimer -= 0.1f;

		dlayTimeCheck = true;

		float saturationGageMin = 0.0f;

		colorGradingData.saturation = saturationGageMin - FLT_EPSILON > colorGradingData.saturation + FLT_EPSILON ? colorGradingData.saturation : colorGradingData.saturation - (0.01f + elapsedTime);
	}

	else
	{
		dlayTimeCheck = false;

		float saturationGageMax = 1;

		colorGradingData.saturation = saturationGageMax + FLT_EPSILON < colorGradingData.saturation - FLT_EPSILON ? colorGradingData.saturation : colorGradingData.saturation + (0.01f + elapsedTime);
	}


	// ブラーエフェクト
	if (shaderBlurStateTimer > 0 )
	{
		shaderBlurStateTimer -= elapsedTime;

		// 画面真ん中
		radialBlurData.center = { 0.5f ,0.5f };
		// 画面ブラー
		float radislBlurRadius = 200;
		radialBlurData.radius =
			radialBlurData.radius > radialBlurDataRadislBlurRadiusMax ?
			radialBlurData.radius : radialBlurData.radius + (5 + elapsedTime);
		// 歪み具合
		float radislBlurSamplingCount = 10;
		radialBlurData.samplingCount = radislBlurSamplingCount;
		// 自分が見える範囲
		float radislBlurMaskRadius = 300;
		radialBlurData.mask_radius = radislBlurMaskRadius;

	}
	else if(shaderPlayStateTimer < 0)
	{

		// ブラー範囲
		float radislBlurRadius = 0;
		radialBlurData.radius = radislBlurRadius - FLT_EPSILON < radialBlurData.radius + FLT_EPSILON ? radialBlurData.radius - (5 + elapsedTime) : radislBlurRadius - FLT_EPSILON;
		// ブラーのかからない範囲
		float radislBlurMaskRadiusNormal = 600;
		float radislBlurMaskRadiusEffectOn = 300;
		radialBlurData.mask_radius = radislBlurRadius - FLT_EPSILON < radialBlurData.radius + FLT_EPSILON ? radislBlurMaskRadiusEffectOn : radislBlurMaskRadiusNormal;
	}


	//if (hitCheck)
	//{
	//	dlayStateTimer = dlayStateTimerMax;

	//	dlayTimeCheck = true;

	//	float saturationGageMin = 0.0f;

	//	colorGradingData.saturation = saturationGageMin - FLT_EPSILON > colorGradingData.saturation + FLT_EPSILON ? colorGradingData.saturation : colorGradingData.saturation - (0.01f + elapsedTime);
	//}

	//else
	//{
	//	dlayTimeCheck = false;

	//	float saturationGageMax = 1;

	//	colorGradingData.saturation = saturationGageMax + FLT_EPSILON < colorGradingData.saturation - FLT_EPSILON ? colorGradingData.saturation : colorGradingData.saturation + (0.01f + elapsedTime);
	//}
}


