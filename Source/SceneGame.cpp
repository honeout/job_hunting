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
		//const char* filename = "Data/Model/ExampleStage/ExampleStage.mdl";
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
		//const char* filename = "Data/Model/Player/dualsword_player.mdl";
		//const char* filename = "Data/Model/Player/test.mdl";

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

		//actor->AddComponent<StageMain>();
		//actor->AddComponent<ProjectileStraight>();
		PlayerManager::Instance().Register(actor);



	}

	// 敵
	{
		// 重い
		//for (int i = 0; i < 10; ++i)
		//{
		// 敵初期化
		//const char* filename = "Data/Model/Slime/Slime.mdl";
		const char* filename = "Data/Model/Boss/BossAnim8.mdl";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->AddComponent<ModelControll>();
		actor->GetComponent<ModelControll>()->LoadModel(filename);
		//actor->SetName("EnemySlime");
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
		//actor->AddComponent<StageMain>();
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
		int max = pos.y + 3;
		int min = pos.y - 3;

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
		//const char* filename = "Data/Sprite/コマンド　選択攻撃.png";
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
		//const char* filename = "Data/Sprite/コマンド　選択攻撃.png";
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
		int max = pos.y + 3;
		int min = pos.y - 3;

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
		int max = pos.y + 3;
		int min = pos.y - 3;

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
		int max = pos.y + 3;
		int min = pos.y - 3;

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
		int max = pos.y + 3;
		int min = pos.y - 3;

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
		int max = pos.y + 3;
		int min = pos.y - 3;

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
		int max = pos.y + 3;
		int min = pos.y - 3;

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
		int max = pos.y + 3;
		int min = pos.y - 3;

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

	// UI UiTime 制限時間２桁
	{
		const char* filename = "Data\\Font\\fonts\\font4.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UiTime　2digit");
		actor->AddComponent<SpriteControll>();
		actor->GetComponent<SpriteControll>()->LoadSprite(filename);
		actor->AddComponent<TransForm2D>();
		// 位置　角度　スケール情報
		std::shared_ptr<TransForm2D> transform2D = actor->GetComponent<TransForm2D>();

		// 位置
		DirectX::XMFLOAT2 pos = { 72, 31 };
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
		int max = pos.y + 3;
		int min = pos.y - 3;

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

	// UI UiTime 制限時間３ケタ
	{
		const char* filename = "Data\\Font\\fonts\\font4.png";
		std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
		actor->SetName("UiTime 3digit");
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
		int max = pos.y + 3;
		int min = pos.y - 3;

		transform2D->SetUiMax(max);
		transform2D->SetUiMin(min);
		// UI揺らす時間
		int MaxTime = 30;

		transform2D->SetShakeTimeMax(MaxTime);

		actor->AddComponent<UiTime>();
		// 描画チェック
		std::shared_ptr<UiTime> ui = actor->GetComponent<UiTime>();
		bool drawCheck = false;
		ui->SetDrawCheck(drawCheck);

		// 桁
		int digit = 3;
		ui->SetDigit(digit);

		// 時間数

		//// 制限時間
		//int timeMax = 180;
		//ui->SetTimeMax(timeMax);

		//int countDown = ui->GetTime()* ui->GetTimeMax();
		//ui->SetCountDown(countDown);

		// これが２Dかの確認
		bool check2d = true;
		actor->SetCheck2d(check2d);



		UiManager::Instance().Register(actor);
	}

	//player = new Player();

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

	//// スポットライトを追加
	//{
	//	Light* light = new Light(LightType::Spot);
	//	light->SetPosition(DirectX::XMFLOAT3(-30, 20, 0));
	//	light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
	//	light->SetDirection(DirectX::XMFLOAT3(+1, -1, 0));
	//	light->SetRange(40.0f);
	//	LightManager::Instanes().Register(light);
	//}

	// 新しい描画ターゲットの生成
	{
		Graphics& graphics = Graphics::Instance();
		renderTarget = std::make_unique<RenderTarget>(static_cast<UINT>(graphics.GetScreenWidth())
			, static_cast<UINT>(graphics.GetScreenHeight())
			, DXGI_FORMAT_R8G8B8A8_UNORM);

		//sprite = std::make_unique<Sprite>();
		//sprite->SetShaderResourceView(renderTarget->GetShaderResourceView()
		//	, renderTarget->GetWidth()
		//	, renderTarget->GetHeight());


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
		postprocessingRenderer->SetSceneData(srvData);
	}


	// ゲージスプライト
	//gauge = new Sprite();
}


// 終了化
void SceneGame::Finalize()
{
	//// ゲージスプライト
	//if (this->gauge)
	//{
	//	delete gauge;
	//	gauge = nullptr;
	//}



	// エネミー終了化
	//EnemyManager::Instance().Clear();

	AfterimageManager::Instance().Clear();


	//// カメラコントーラー終了化
	//if (this->cameraController)
	//{
	//	delete cameraController;
	//	cameraController = nullptr;
	//}

	// プレイヤー終了化
	//if (this->player)
	//{
	//	delete player;
	//	player = nullptr;
	//}
	// ステージ終了化
	//StageManager::instance().Clear();

	ActorManager::Instance().Clear();

	LightManager::Instanes().Clear();

	EnemyManager::Instance().Clear();

	PlayerManager::Instance().Clear();

	StageManager::Instance().Clear();

	UiManager::Instance().Clear();


}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	
	// カメラコントローラー更新処理
	//DirectX::XMFLOAT3 target = player->GetPosition();
	//target.y += 0.5f;// 足元から５０センチぐらい
	//cameraController->SetTarget();// プレイヤーの腰当たり
	//cameraController->Update(elapsedTime);

	// ステージ更新処理
	//StageManager::instance().Update(elapsedTime);
	
	// プレイヤー更新処理
	//player->Update(elapsedTime);
	ActorManager::Instance().Update(elapsedTime);

	// 残像経過時間
	//AfterimageTime(elapsedTime);

	

	// 残像ステート更新
	//AfterimageManager::Instance().Update(elapsedTime);



	// エネミー更新処理
	//EnemyManager::Instance().Update(elapsedTime);

	// エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);

	// シーン切り替え
	{
		for (int i = 0; i < PlayerManager::Instance().GetPlayerCount(); ++i)
		{
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
			if (hp->GetHealth() <= 0 && hp->GetLife() >= 0)
			{
				// hpを回復
				hp->SetHealth(hp->GetMaxHealth());

				// 残機を減らす
				//hp->SetLife(hp->GetLife() - 1);
			}
		}
		// ゲームオーバー
		std::shared_ptr<UiTime> uiTime = UiManager::Instance().GetUies((int)UiManager::UiCount::Time)->GetComponent<UiTime>();
		if (uiTime->GetTimeUp())
		{
			// ゲームオーバー
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGameOver));
		}

	}

	//sprite->Update(0.0f, 0.0f,
	//	Graphics::Instance().GetScreenWidth(), Graphics::Instance().GetScreenHeight(),
	//	0.0f, 0.0f,
	//	static_cast<float>(renderTarget->GetWidth()), static_cast<float>(renderTarget->GetHeight()),
	//	0.0f,
	//	1.0f, 1.0f, 1.0f, 1.0f);

}



// 描画処理
void SceneGame::Render()
{
	
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	//ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	//ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	//// シャドウマップの描画
	//RenderShadowmap();

	//Render3DScene();

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

		// unit09
		//RenderContext rc;
		//rc.deviceContext = dc;

		//SpriteShader* shader = graphics.GetShader(SpriteShaderId::ColorGrading);
		//shader->Begin(rc);

		////rc.colorGradingData = colorGradingData;
		//shader->Draw(rc, sprite.get());

		//shader->End(rc);
		// ポストプロセスを処理を行う
		//postprocessingRenderer->Render(dc);
	}


	//FLOAT color[] = {0.0f, 0.0f, 0.5f, 1.0f};// RGBA
	//dc->ClearRenderTargetView(rtv, color);
	//dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//dc->OMSetRenderTargets(1, &rtv, dsv);

	//// UINT11
 //   // ビューポートの設定
	//D3D11_VIEWPORT vp = {};
	//vp.Width = graphics.GetScreenWidth();
	//vp.Height = graphics.GetScreenHeight();
	//vp.MinDepth = 0.0f;
	//vp.MaxDepth = 1.0f;
	////dc->RSSetViewports(1, &vp);


	//RenderContext rc;
	//rc.lightDirection = { 0.0f,-1.0f, 0.0f, 0.0f };

	//// カメラ情報
	//Camera& camera = Camera::Instance();
	//rc.view = camera.GetView();
	//rc.projection = camera.GetProjection();

	//rc.deviceContext = dc;


	//// UNIT11
 //   // ライトの情報をつめこむ 
	//rc.shadowMapData.shadowMap = shadowmapDepthStencil->GetShaderResourceView().Get();
	//rc.shadowMapData.lightViewProjection = lightViewProjeciton;
	//rc.shadowMapData.shadowColor = shadowColor;
	//rc.shadowMapData.shadowBias = shadowBias;

	// ポストプロセスを処理を行う
	//postprocessingRenderer->Render(dc);

	//// ライトの情報を詰め込む
	//LightManager::Instanes().PushRenderContext(rc);

	// 3Dモデル描画
	{
		//ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);

		//ActorManager::Instance().Render(rc, shader);
	//ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);

	//ActorManager::Instance().Render(rc, shader);




	}

	// 3Dエフェクト描画
	{
		//EffectManager::Instance().Render(rc.view, rc.projection);



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
		//RenderEnemyGauge(dc, rc.view, rc.projection);


		SpriteShader* shaderUi = graphics.GetShader(SpriteShaderId::Default);

		ActorManager::Instance().Render2D(rc, shaderUi);
	}


	// 3Dデバッグ描画
	{
		// 当たり判定の形をうつ
		// プレイヤーデバッグプリミティブ描画
		//player->DrawDebugPrimitive();

		// エネミーデバッグプリミティブ描画
		//EnemyManager::Instance().DrawDebugPrimitive();



		//for (int i = 0; i < EnemyManager::Instance().GetEnemyCount(); i++)
		//{
		//	EnemyManager::Instance().GetEnemy(i)->GetComponent<EnemySlime>()->DrawDebugPrimitive();
		//}

		//// ライトのデバッグプリミティブの描画
		//LightManager::Instanes().DrawDebugPrimitive();

		//// ラインレンダラ描画実行
		//graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		//// 実際の当たり判定描画
		//// デバッグレンダラ描画実行
		//graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	//// 2Dスプライト描画
	//{
	//	//RenderEnemyGauge(dc, rc.view, rc.projection);


	//	SpriteShader* shaderUi = graphics.GetShader(SpriteShaderId::Default);

	//	ActorManager::Instance().Render2D(rc, shaderUi);
	//}

	// 2DデバッグGUI描画
	{
		// プレイヤーデバッグ描画
		//player->DrawDebugGUI();
		//cameraController->DrawDebugGUI();

		//EnemyManager::Instance().DrawDebugGUI();
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
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> rtv = renderTarget->GetRenderTargetView().Get();
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv = graphics.GetDepthStencilView();

	//RenderShadowmap();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv.Get(), color);
	dc->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());

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

	//dissolveThreshold = 0.0f;
	//edgeThreshold = 0.2f; // 緑の閾値
	//edgeColor = { 1,0,0,1 }; // 緑の色


	// カメラパラメータ設定
	Camera& camera = Camera::Instance();
	rc.viewPosition.x = camera.GetEye().x;
	rc.viewPosition.y = camera.GetEye().y;
	rc.viewPosition.z = camera.GetEye().z;
	rc.viewPosition.w = 1;
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	//// 2Dスプライト描画
	//{
	//	SpriteShader* shaderUi = graphics.GetShader(SpriteShaderId::Default);

	//	ActorManager::Instance().Render2D(rc, shaderUi);
	//}

	// 3Dモデル描画
	{
		//ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);
		////ModelShader* shader = graphics.GetShader(ModelShaderId::Toon);
		//shader->Begin(rc);

		//shader->Draw(rc, stage.get());
		//shader->Draw(rc, jummo.get());
		//shader->Draw(rc, uncle.get());
		////shader->Draw(rc, earth.get());

		//shader->End(rc);]
		ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);

		ActorManager::Instance().Render(rc, shader);




		//shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);
		//ActorManager::Instance().Render(rc, shader);

	}

	// 3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}



	// デバッグプリミティブの表示
	{
		// グリッド描画
		//DrawGrid(dc, 20, 10.0f);

		//for (int i = 0; i < EnemyManager::Instance().GetEnemyCount(); i++)
		//{
		//	EnemyManager::Instance().GetEnemy(i)->GetComponent<EnemySlime>()->DrawDebugPrimitive();
		//}

		//// ライトのデバッグプリミティブの描画
		//LightManager::Instanes().DrawDebugPrimitive();

		//// ラインレンダラ描画実行
		//graphics.GetLineRenderer()->Render(dc, camera.GetView(), camera.GetProjection());

		//// デバッグレンダラ描画実行
		//graphics.GetDebugRenderer()->Render(dc, camera.GetView(), camera.GetProjection());
	}


}

void SceneGame::RenderShadowmap()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> rtv = nullptr;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilView> dsv = shadowmapDepthStencil->GetDepthStencilView().Get();

	// 画面クリア
	dc->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (!mainDirectionalLight)
		return;
	//// 画面クリア＆レンダーターゲット設定
	//FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	//dc->ClearRenderTargetView(rtv, color);
	//dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// レンダーターゲット設定
	//dc->OMSetRenderTargets(1, &rtv, dsv);
	dc->OMSetRenderTargets(0, rtv.GetAddressOf(), dsv.Get());

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
	//ActorManager::Instance().RenderShadowmap(rc.view, rc.projection);
}


// エネミーHPゲージ描画
void SceneGame::RenderEnemyGauge(ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection
)
{


	//// ビューポート 画面のサイズ等
	//// ビューポートとは2Dの画面に描画範囲の指定(クリッピング指定も出来る)位置を指定
	//D3D11_VIEWPORT viewport;
	//UINT numViewports = 1;
	//// ラスタライザーステートにバインドされているビューポート配列を取得
	//dc->RSGetViewports(&numViewports, &viewport);

	//// 変換行列
	//DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	//DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	//// ローカルからワールドに行くときにいる奴相手のポジションを渡す。
	//DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();


	//// 全ての敵の頭上にHPゲージを表示
	//EnemyManager& enemyManager = EnemyManager::Instance();
	//int enemyCount = enemyManager.GetEnemyCount();

	//for (int i = 0; i < enemyCount; ++i)
	//{

	//	Enemy* enemy = enemyManager.GetEnemy(i);
	//	// エネミー頭上
	//	DirectX::XMFLOAT3 worldPosition = enemy->GetPosition();
	//	worldPosition.y += enemy->GetHeight();

	//	// ワールドからスクリーン
	//	DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);


	//		// ゲージ描画 // ワールドからスクリーン

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
	//	// スクリーン座標
	//	DirectX::XMFLOAT3 scereenPosition;
	//	DirectX::XMStoreFloat3(&scereenPosition, ScreenPosition);

	//	// ゲージ長さ
	//	const float gaugeWidth = 30.0f;
	//	const float gaugeHeight = 5.0f;

	//	float healthRate = enemy->GetHealth() / static_cast<float>(enemy->GetMaxHealth());


	//	if (scereenPosition.z < 0.0f || scereenPosition.z > 1.0f)continue;

	//	// 2Dスプライト描画
	//	{
	//		// 描画
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
	//		// {位置}{サイズ}{画像どこから}{画像何処まで}
	//		// dc , ｛範囲｝｛｝
	//	}
	//
	//

	//	}


	//// エネミー配置処理
	//Mouse& mouse = Input::Instance().GetMouse();
	//if (mouse.GetButtonDown() & Mouse::BTN_LEFT)
	//{
	//	// マウスカーソル座標を取得
	//	DirectX::XMFLOAT3 scereenPosition;
	//	scereenPosition.x = static_cast<float>(mouse.GetPositionX());
	//	scereenPosition.y = static_cast<float>(mouse.GetPositionY());
	//	scereenPosition.z = 0.0f;

	//	// レイの始点を算出
	//	DirectX::XMVECTOR WorldPosition{};
	//	WorldPosition = DirectX::XMLoadFloat3(&scereenPosition);



	//	// スクリーン情報をワールド空間に
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
	//	// 始点
	//	DirectX::XMFLOAT3 rayStart;
	//	DirectX::XMStoreFloat3(&rayStart, WorldPosition);

	//	// レイの終点を算出
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

	//	// レイキャスト
	//	HitResult hit;
	////	if (StageManager::instance().RayCast(rayStart, rayEnd, hit))
	////	{
	////		// 敵を配置
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
	//// 動いていたら残像が出る。
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

	//// 残像出す間隔
	//if (elapsedFrame <= 0)
	//{

	//	PlayerAfterimage* playerAfterimage = new PlayerAfterimage();
	//	// 配置して
	//	playerAfterimage->SetPosition({ player->GetPosition() });
	//	playerAfterimage->SetAngle(player->GetAngle());
	//	playerAfterimage->SetTransform(player->GetTransform());
	//	// 動かして
	//	playerAfterimage->GetModel()->UpdateTransform(player->GetTransform(),
	//		player->GetModel()->GetNodes());

	//	// 残像の出る間隔再設定
	//	elapsedFrame = spawnafterimagetimemax;

	//	// 透明度 最大値を再設定
	//	afterImageAlpha = afterImageAlphaMax;
	//	
	//	// 生存時間セット
	//	playerAfterimage->SetSurvivalTime(afterImageAlpha);

	//	// 消えるまでの時間再設定
	//	reduce = reduceMax;

	//	// 消えるまでの時間セット
	//	playerAfterimage->SetReduce(reduce);

	//	// 登録
	//	AfterimageManager::Instance().Register(playerAfterimage);

	//	// 残業ステートアニメーションの時間 姿勢
	//	AfterimageManager::Instance().GetAfterimage(
	//		AfterimageManager::Instance().GetAfterimageCount() - 1
	//	)->SetCurrentAnimationSeconds(
	//		player->GetCurrentANimationSeconds());
	//	//playerAfterimage->GetModel()->UpdateTransform(player->GetTransform());



	//	

	//}
}
