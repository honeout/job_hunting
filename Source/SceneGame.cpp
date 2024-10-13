#include "Graphics/Graphics.h"
#include "SceneGame.h"
#include "Camera.h"
#include "EnemySlime.h"

#include "EffectManager.h"

#include "Input/Input.h"

#include <DirectXMath.h>

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

#include "LightManager.h"

#include "SceneLoading.h"
#include "SceneManager.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"

#include "UiTime.h"

// シャドウマップのサイズ
static const UINT SHADOWMAP_SIZE = 2048;


// 初期化
void SceneGame::Initialize()
{

	
	// ステージ初期化
	{
		const char* filename = "Data/Model/ExampleStage/M_stage .mdl";
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

		// これが２Dかの確認
		bool check2d = false;
		actor->SetCheck2d(check2d);

		StageManager::Instance().Register(actor);

	}


	//player = new Player;
	{
		// プレイヤー初期化
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
		std::shared_ptr<HP> hp = actor->GetComponent<HP>();
		int life = 0;
		hp->SetLife(life);

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
			SetScale(DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f));
		actor->AddComponent<Movement>();
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
		int max = 3;
		int min = -3;

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
		//const char* filename = "Data/Sprite/HP.png";
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
		//const char* filename = "Data/Sprite/コマンド　選択攻撃.png";
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
		//const char* filename = "Data/Sprite/コマンド　選択魔法.png";
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
		//const char* filename = "Data/Sprite/コマンド　非選択攻撃.png";
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
		//const char* filename = "Data/Sprite/コマンド　非選択魔法.png";
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
		//const char* filename = "Data/Sprite/コマンド　非選択魔法.png";
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
		//const char* filename = "Data/Sprite/コマンド　非選択魔法.png";
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
		//const char* filename = "Data/Sprite/コマンド　非選択魔法.png";
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
		//const char* filename = "Data/Sprite/コマンド　非選択魔法.png";
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
		//const char* filename = "Data/Sprite/コマンド　非選択魔法.png";
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
		//const char* filename = "Data/Sprite/コマンド　非選択魔法.png";
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
		//const char* filename = "Data/Sprite/コマンド　選択攻撃.png";
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
		actor->SetName("PlayerCommandSpeciulFramePushu");
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
		int max = 3;
		int min =  - 3;

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
		int max =  3;
		int min =  - 3;

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
		int max =  3;
		int min =  - 3;

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
		int max =  3;
		int min =  - 3;

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
		int max =  3;
		int min =  - 3;

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
		int max =  3;
		int min =  - 3;

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

		DirectX::XMFLOAT2 scale = { 100,100 };
		transform2D->SetScale(scale);

		// 元の大きさ
		DirectX::XMFLOAT2 texScale = { 0,0 };
		transform2D->SetTexScale(texScale);

		// UI揺らす範囲を指定揺らす場合
		int max =   3;
		int min =  - 3;

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
		light->SetPosition(DirectX::XMFLOAT3(0, 1, 0));
		light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		LightManager::Instanes().Register(light);
	}

	// 新しい描画ターゲットの生成
	{
		Graphics& graphics = Graphics::Instance();
		renderTarget = std::make_unique<RenderTarget>(static_cast<UINT>(graphics.GetScreenWidth())
			, static_cast<UINT>(graphics.GetScreenHeight())
			, DXGI_FORMAT_R8G8B8A8_UNORM);

	}

	// シャドウマップ用に深度ステンシルの生成
	{
		Graphics& graphics = Graphics::Instance();
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
		postprocessingRenderer->SetSceneData(srvData);
	}

}


// 終了化
void SceneGame::Finalize()
{
	


	LightManager::Instanes().Clear();

	EnemyManager::Instance().Clear();

	PlayerManager::Instance().Clear();

	StageManager::Instance().Clear();

	UiManager::Instance().Clear();

	ActorManager::Instance().Clear();

}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	
	
	//　すべて更新処理
	ActorManager::Instance().Update(elapsedTime);


	// エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);

	// シーン切り替え
	{
		for (int i = 0; i < PlayerManager::Instance().GetPlayerCount(); ++i)
		{
			// HPゲームオーバー
			if (PlayerManager::Instance().GetPlayer(i)->GetComponent<HP>()->GetDead())
			{
				PlayerManager::Instance().GetPlayer(i)->GetComponent<HP>()->SetDead(false);
				ActorManager::Instance().Clear();
				//// プレイヤー更新処理
				//ActorManager::Instance().Update(elapsedTime);
				// ゲームオーバー
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGameOver));
			}
		}

		for (int i = 0; i < UiManager::Instance().GetUiesCount(); ++i)
		{
			// 時間ゲームオーバー
			std::shared_ptr<UiTime> uiTime = UiManager::Instance().GetUies((int)UiManager::UiCount::Time)->GetComponent<UiTime>();
			if (uiTime->GetTimeUp())
			{
				// ゲームオーバー
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGameOver));
			}
		}
		// クリア
		for (int i = 0; i < EnemyManager::Instance().GetEnemyCount(); ++i)
		{
			std::shared_ptr<HP> hp = EnemyManager::Instance().GetEnemy(i)->GetComponent<HP>();
			if (hp->GetDead())
			{
				hp->SetDead(false);
				ActorManager::Instance().Clear();
				//// プレイヤー更新処理
				//ActorManager::Instance().Update(elapsedTime);
				SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGameClear));
			}
			// TODO 1013改造しないと
			if (hp->GetHealth() <= 0 && hp->GetLife() >= 0)
			{
				// hpを回復
				hp->SetHealth(hp->GetMaxHealth());

				// 残機を減らす
				//hp->SetLife(hp->GetLife() - 1);
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
		Microsoft::WRL::ComPtr <ID3D11RenderTargetView> rtv = graphics.GetRenderTargetView();
		Microsoft::WRL::ComPtr <ID3D11DepthStencilView> dsv = graphics.GetDepthStencilView();

		// 画面クリア＆レンダーターゲット設定
		FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f }; // RGBA(0.0~1.0)
		dc->ClearRenderTargetView(rtv.Get(), color);
		dc->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		dc->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());


		// UINT11
		// ビューポートの設定
		D3D11_VIEWPORT vp = {};
		vp.Width = graphics.GetScreenWidth();
		vp.Height = graphics.GetScreenHeight();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		dc->RSSetViewports(1, &vp);

		 //ポストプロセスを処理を行う
		postprocessingRenderer->Render(dc);
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


	// 2DデバッグGUI描画
	{
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
		ImGui::Separator();

		postprocessingRenderer->DrawDebugGUI();
		ImGui::Separator();
		LightManager::Instanes().DrawDebugGUI();
	}

	
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



	// デバッグプリミティブの表示
	{
		// グリッド描画
		//DrawGrid(dc, 20, 10.0f);

		for (int i = 0; i < EnemyManager::Instance().GetEnemyCount(); i++)
		{
			EnemyManager::Instance().GetEnemy(i)->GetComponent<EnemySlime>()->DrawDebugPrimitive();
		}

		//// ライトのデバッグプリミティブの描画
		LightManager::Instanes().DrawDebugPrimitive();

		//// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, camera.GetView(), camera.GetProjection());

		//// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, camera.GetView(), camera.GetProjection());
	}


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
	//// 画面クリア＆レンダーターゲット設定
	dc->OMSetRenderTargets(1, &rtv, dsv);

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

