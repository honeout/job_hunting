#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
//#include "EnemyManager.h"
#include "Collision.h"
#include "Audio\Audio.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "ProjectileRuby.h"
#include "ProjectileThrowing.h"
#include "Graphics/Model.h"
#include "EnemyBoss.h"

#include "ProjectileTornade.h"

#include "StateDerived.h"

#include "TransForm2D.h"

#include "ProjectileImpact.h"
#include "ProjectileSunder.h"
#include "BulletFiring.h"

#include "UiManager.h"
#include "Ui.h"

#include "Graphics\PrimitiveRenderer.h"






// デストラクタ
Player::~Player()
{

    //if (movement)
    //    movement.reset();
    //if (hp)
    //    hp.reset();
    //if (transform)
    //    transform.reset();

    //if (stateMachine)
    //{
    //    delete stateMachine;
    //    stateMachine = nullptr;
    //}

    

    if (hitEffect != nullptr)
    {
        hitEffect->Stop(hitEffect->GetEfeHandle());
        delete hitEffect;
        hitEffect = nullptr;
    }

    if (ImpactEffect != nullptr)
    {
        ImpactEffect->Stop(ImpactEffect->GetEfeHandle());
        delete ImpactEffect;
        ImpactEffect = nullptr;
    }
    if (desEffect != nullptr)
    {
        desEffect->Stop(desEffect->GetEfeHandle());
        delete desEffect;
        desEffect = nullptr;
    }
    //if (hitFire != nullptr)
    //{
    //    hitFire->Stop(hitFire->GetEfeHandle());
    //    delete hitFire;
    //    hitFire = nullptr;
    //}

    //if (cameraControlle != nullptr)
    //{
    //    delete cameraControlle;
    //    cameraControlle = nullptr;
    //}

    stateMachine.reset(nullptr);

    //if (seSouce != nullptr)
    //{
    //    delete seSouce;
    //    seSouce = nullptr;
    //}

   // ProjectileManager::Instance().Clear();
  
}



void Player::Start()
{

    // BGM
    //Bgm = Audio::Instance().LoadAudioSource("Data/Audio/SE/炎飛行時.wav");

    //// SE
    //SePlayer = Audio::Instance().LoadAudioSource("Data/Audio/SE/炎着弾時.wav","flame");

    // ムーブメント関数を使えるように
    movement = GetActor()->GetComponent<Movement>();

    // 落下停止
    bool stopFall = false;
    movement.lock()->SetStopFall(stopFall);

    // 移動の停止
    bool stopMove = false;
    movement.lock()->SetStopMove(stopMove);

    // hp関数を使えるように
    hp = GetActor()->GetComponent<HP>();

    // mp関数を使えるように
    mp = GetActor()->GetComponent<Mp>();

    // トランスフォーム関数を呼び出し
    transform = GetActor()->GetComponent<Transform>();

    // 位置等
    position = transform.lock()->GetPosition();

    angle = transform.lock()->GetAngle();

    scale = transform.lock()->GetScale();

    // 重力設定
    movement.lock()->SetGravity(gravity);

    // モデルデータを入れる。
    model = GetActor()->GetComponent<ModelControll>()->GetModel();

    //// カメラ初期化
    //cameraControlle = nullptr;
    //cameraControlle = new CameraController();

    // ヒットエフェクト読込 
    hitEffect = new Effect("Data/Effect/Hit.efk");
    ImpactEffect = new Effect("Data/Effect/rehleckte.efk");
    desEffect = new Effect("Data/Effect/F.efk");

    // エフェクト読み込み
    float effectScale = 1.0f;

    // 斬撃
    hitSlash = std::make_unique<Effect>("Data/Effect/slashHit.efk");

    hitFire = std::make_unique<Effect>("Data/Effect/hit fire.efk");
    hitThander = std::make_unique<Effect>("Data/Effect/HitThunder.efk");
    hitIce = std::make_unique<Effect>("Data/Effect/hit Ice.efk");

    lightningAttack = std::make_unique<Effect>("Data/Effect/sunder.efk");


    // エフェクト竜巻
    areWork = std::make_unique<Effect>("Data/Effect/tornade.efk");

    // エフェクト２D注目
    effectFocus2D = std::make_unique<Effect>("Data/Effect/effectFocus2D.efk");


    //// SE
    //seSouce = Audio::Instance().LoadAudioSource();
    //seSouce->LoadSE("Data/Audio/SE/足音.wav","walk");
    //seSouce->LoadSE("Data/Audio/SE/打撃.wav","hitButton");
    //seSouce->LoadSE("Data/Audio/SE/Enemy着地.wav","land");
    //seSouce->LoadSE("Data/Audio/SE/スラッシュ１回目.wav","slash");
    //seSouce->LoadSE("Data/Audio/SE/スラッシュ２回目.wav","slashTwo");
    //seSouce->LoadSE("Data/Audio/SE/炎着弾時.wav","flamePush");
    //seSouce->LoadSE("Data/Audio/SE/炎飛行時.wav","flame");
    //seSouce->LoadSE("Data/Audio/SE/氷着弾時.wav","icePush");
    //seSouce->LoadSE("Data/Audio/SE/氷発射.wav","ice");
    //seSouce->LoadSE("Data/Audio/SE/雷.wav","lightningStrike");
    //seSouce->LoadSE("Data/Audio/SE/ヒットストップ.wav","hitStop");
    //seSouce->LoadSE("Data/Audio/SE/必殺技炎.wav","specileFirePush");
    //seSouce->LoadSE("Data/Audio/SE/必殺技炎ため.wav","specileFireCharge");
    //seSouce->LoadSE("Data/Audio/SE/必殺技ため完了.wav","specileChargeClear");
    //seSouce->LoadSE("Data/Audio/SE/必殺技雷.wav","specileLightning");
    //seSouce->LoadSE("Data/Audio/SE/必殺技雷ため.wav","specileLightningCharge");
    //seSouce->LoadSE("Data/Audio/SE/ヒットストップ.wav","hitStop");

    //// 上半身
    //bornUpStartPoint = "mixamorig:Hips";
    //// 下半身
    //bornDownerEndPoint = "mixamorig:LeftUpLeg";

     // 上半身スタート再生開始場所
    bornUpStartPoint = "mixamorig:Hips";
    // 上半身エンド再生停止場所
    bornUpEndPoint = "mixamorig:LeftUpLeg";
    // 下半身スタート再生開始場所
    bornDownerStartPoint = "mixamorig:LeftUpLeg";
    // 下半身エンド再生停止場所
    bornDownerEndPoint = "mixamorig:RightToe_End";

    // hp設定
    hp.lock()->SetHealth(health);
    // hp最大値の設定
    hp.lock()->SetMaxHealth(maxHealth);


    // mp設定
    mp.lock()->SetMagic(magicPoint);
    // mp最大値
    mp.lock()->SetMaxMagic(magicPoint);

    // 半径
    transform.lock()->SetRadius(radius);
    // 身長
    transform.lock()->SetHeight(height);

    // コマンド操作用
    selectCheck = (int)CommandAttack::Attack;

    // 魔法選択用
    selectMagicCheck = (int)CommandMagic::Normal;

    // 特殊攻撃ため初期値
    specialAttackCharge = 0.0f;

    // ステートマシン
    //stateMachine = new StateMachine();
    //stateMachine = std::make_unique<StateMachine>();

    //stateMachine->RegisterState(new PlayerIdleState(GetActor()));
    //stateMachine->RegisterState(new PlayerMovestate(GetActor()));
    //stateMachine->RegisterState(new PlayerJumpState(GetActor()));
    //stateMachine->RegisterState(new PlayerLandState(GetActor()));
    //stateMachine->RegisterState(new PlayerJumpFlipState(GetActor()));
    //stateMachine->RegisterState(new PlayerQuickJabState(GetActor()));
    //stateMachine->RegisterState(new PlayerSideCutState(GetActor()));
    //stateMachine->RegisterState(new PlayerCycloneStrikeState(GetActor()));
    //stateMachine->RegisterState(new PlayerSpecialAttackState(GetActor()));
    //stateMachine->RegisterState(new PlayerMagicState(GetActor()));
    //stateMachine->RegisterState(new PlayerSpecialMagicState(GetActor()));
    //stateMachine->RegisterState(new PlayerSpecialMagicIceState(GetActor()));
    //stateMachine->RegisterState(new PlayerSpecialThanderMagicState(GetActor()));
    //stateMachine->RegisterState(new PlayerDamageState(GetActor()));
    //stateMachine->RegisterState(new PlayerDeathState(GetActor()));
    //stateMachine->RegisterState(new PlayerReviveState(GetActor()));
    //stateMachine->RegisterState(new PlayerAvoidanceState(GetActor()));
    //stateMachine->RegisterState(new PlayerReflectionState(GetActor()));

    //// ステートセット
    //stateMachine->SetState(static_cast<int>(State::Idle));

    // アニメーションルール
    updateanim = UpAnim::Normal;

    moveSpeedAnimation = 0.0f;

    // 特殊アクションの種類
    //specialAttack.push((int)SpecialAttack::Normal);

    // 特殊アクション発動不
    specialAttackTime = false;

    // 揺れモード
    shakeMode = false;

    // 回転確認
    angleCheck = false;

    // 曲がる速度
    turnSpeedAdd = 0;

    
    // 攻撃ヒット回数初期化
    attackNumberSave = 0;


    endState = false;

    // エネミーロックオン用ステート確認
    stateEnemyIndex = 0;


    // 経過時間測る用
    timeElapsed = 0.0f;
}

// 更新処理
// elapsedTime(経過時間)
void Player::Update(float elapsedTime)
{
  //  if (isMenue) return;

    GamePad& gamePad = Input::Instance().GetGamePad();
    //// ステート毎の処理
    stateMachine->Update(elapsedTime);

    //// カメラコントローラー更新処理
    //cameraControlle->Update(elapsedTime);

    // エフェクト位置更新
    if (areWork->GetEfeHandle())
    {
        areWork->SetPosition(areWork->GetEfeHandle(), position);
    }

    //if (InputMenue())
    //{
    //    isMenue = isMenue ? isMenueOf : isMenueOn;
    //}



    // ソードトレイル
    //UpdateSwordeTraile();
    //

    // 行動時間
    areAttackTime -= areAttackTimeValue;

    // 空中行動回数制限
    if (areAttackState <= areAttackStateEnd && !isAreAttack)
    {
        // 攻撃禁止
        isAreAttack = true;
        // 空中攻撃間隔時間
        areAttackTime = areAttackTimeMax;

    }

    // 地上
    if (movement.lock()->GetOnLadius())
    {
        // 攻撃許可
        isAreAttack = false;
        // 攻撃回数
        areAttackState = areAttackStateMax;
        
    }

    if (areAttackTime <= areAttackTimeEnd && isAreAttack)
    {
        // 攻撃許可
        isAreAttack = false;
        // 攻撃回数
        areAttackState = areAttackStateMax;
    }
    
    // コマンド操作
    if (uiControlleCheck &&
        stateMachine->GetStateSize() > stateSize)
    {
        // 行動選択
        InputSelectCheck();
        // 魔法選択
        InputSelectMagicCheck();
        // 魔法選択ショートカットキー
        InputShortCutkeyMagic();

        // ポストエフェクトｈｐの一定以下
        PinchMode(elapsedTime);


        // 着地時にエフェクト切る
        if (movement.lock()->GetOnLadius() || GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Death))
        {
            areWork->Stop(areWork->GetEfeHandle());
        }
        // 攻撃の時にステートを変更
        if (
            InputAttack() && GetSelectCheck() ==
            (int)Player::CommandAttack::Attack &&
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::QuickJab) &&
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::SideCut)&&
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::CycloneStrike)&&
                GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage) && 
                    GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Death) &&
                        !isAreAttack
            )
        {
            // ステート遷移
            GetStateMachine()->ChangeState(static_cast<int>(Player::State::QuickJab));
            // 音再生
            InputAttackSlashSE();

            // 炎音
            //Bgm->Play(false);
            // 音大きい
            //Bgm->SetVolume(2.0f);
            
            // もし地面なら何もしない
            bool noStart = false;
            // エフェクト再生
            areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;

            areWork->Play(position);
            // Se再生
            //InputAttackSE();
        }

        //　魔法入力処理
        if (InputMagick() && GetSelectCheck() == (int)Player::CommandAttack::Magic &&
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Magic) && 
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage) &&
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Death) && 
            !gamePad.GetButtonDownCountinue()
                )
        {
            if (mp.lock()->GetMpEmpth())
            {
                magicAction = false;
                selectCheck = (int)CommandAttack::Attack;

                return;
            }

            GetStateMachine()->ChangeState(static_cast<int>(Player::State::Magic));


            // もし地面なら何もしない
            bool noStart = false;
            // エフェクト再生
            areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;

            areWork->Play(position);
        }


        

 
        if(GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage) &&
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Death))
        // 特殊攻撃
        InputSpecialAttackCharge();

        // UI必殺技演出
        SpecialPlayUlEffect(elapsedTime);

        // 攻撃範囲内なのでUI描画
        AttackCheckUI();


        // ゲージ管理
        UiControlle(elapsedTime);

        //if (InputSpecialAttackCharge())
        //{
        //    
        //    //TransitionAttackState();
        //    GetStateMachine()->ChangeState(static_cast<int>(Player::State::SpecialAttack));
        //    // 特殊攻撃の発生条件 解消
        //    specialAttackTime = false;


        //    // もし地面なら何もしない
        //    bool noStart = false;
        //    // エフェクト再生
        //    areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;

        //    areWork->Play(position);
        //        
        //}
        //// 特殊魔法
        //if (InputSpecialShotCharge())
        //{

        //    GetStateMachine()->ChangeState(static_cast<int>(Player::State::SpecialMagic));

        //    /*InputProjectile();*/
        //    // 特殊攻撃の発生条件 解消
        //    specialAttackTime = false;


        //    // もし地面なら何もしない
        //    bool noStart = false;
        //    // エフェクト再生
        //    areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;

        //    areWork->Play(position);
        //}

    }

    // 速力処理更新


    position = transform.lock()->GetPosition();

    angle = transform.lock()->GetAngle();

    scale = transform.lock()->GetScale();

    hp.lock()->UpdateInbincibleTimer(elapsedTime);

    // マジック回復
    mp.lock()->MpCharge(elapsedTime);

    //// ロックオン
    InputRockOn();

    hitEffect->SetScale(hitEffect->GetEfeHandle(),{ 1,1,1 });
    // 加速度等
    movement.lock()->UpdateVelocity(elapsedTime);
    

    // プレイヤーと敵との衝突処理
    //CollisionBornVsProjectile("shoulder");
    CollisionBornVsProjectile("body2");
    CollisionPlayerVsEnemies();
    // 弾丸当たり判定
    CollisionProjectilesVsEnemies();
    // ルビー当たり判定
    CollisionRubyVsEnemies();


    // 位置更新
    transform.lock()->UpdateTransform();




    // モーション更新処理
    switch (updateanim)
    {
    case UpAnim::Stop:
    {
        break;
    }
    // 通常アニメーション
    case UpAnim::Normal:
    {
        // アニメーション再生
        model->UpdateAnimation(elapsedTime, true);
        break;
    }
    // 部分再生
    case UpAnim::Doble:
    {
        // モデル部分アニメーション更新処理
        model->UpdateUpeerBodyAnimation(elapsedTime, bornUpStartPoint, bornUpEndPoint,true);
        model->UpdateLowerBodyAnimation(elapsedTime, bornDownerStartPoint, bornDownerEndPoint, true);
        break;
    }
    // 複数ブレンド再生
    case UpAnim::Blend:
    {
        // モデル複数ブレンドアニメーション更新処理
        model->Update_blend_animations(elapsedTime, true);
        break;
    }

    case UpAnim::Reverseplayback:
    {
        // モデル逆再生アニメーション更新処理
        model->ReverseplaybackAnimation(elapsedTime, true);
        break;

    }
    }


    // 位置更新
    model->UpdateTransform(transform.lock()->GetTransform());
    
}

void Player::Render(RenderContext& rc, ModelShader& shader)
{
    RockOnUI(rc.deviceContext,rc.view,rc.projection);



    rc.colorGradingData = colorGradingData;

    // スペキュラー無効化
    rc.isSpecular = 0;

    // 影オンオフ
    rc.isRimRightning = 1;

    // modelオンオフ
    rc.StencilRef = 1;

    Graphics& graphics = Graphics::Instance();
    shader.Begin(rc);// シェーダーにカメラの情報を渡す
    
    
    shader.Draw(rc, model);
   
    shader.End(rc);


}

// シャドウマップ
void Player::RenderShadowmap(RenderContext& rc)
{
    Graphics& graphics = Graphics::Instance();
    ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);
    shader->Begin(rc);// シェーダーにカメラの情報を渡す


    shader->Draw(rc, model);

    shader->End(rc);
    
}

void Player::InputWalkSE()
{
    Audio& Se = Audio::Instance();

    AudioParam audioParam;

    audioParam.filename = "Data/Audio/SE/足音.wav";

    audioParam.keyName = "BGM";

    audioParam.loop = true;

    audioParam.volume = 3.0f;

    Se.Play(audioParam);
}

void Player::InputStopWalkSE()
{
    Audio& Se = Audio::Instance();

    std::string filename = "Data/Audio/SE/足音.wav";

    // 種類停止
    Se.Stop(filename);
}

void Player::InputJampSE()
{
    Audio& Se = Audio::Instance();

    AudioParam audioParam;

    audioParam.filename = "Data/Audio/SE/Enemy歩き攻撃ヒット.wav";

    audioParam.keyName = "BGM";

    audioParam.loop = false;

    audioParam.volume = 3.0f;

    Se.Play(audioParam);
}

void Player::InputStopJampSE()
{
    Audio& Se = Audio::Instance();

    std::string filename = "Data/Audio/SE/Enemy歩き攻撃ヒット.wav";

    // 種類停止
    Se.Stop(filename);
}

void Player::InputAreWalkSE()
{
    Audio& Se = Audio::Instance();

    AudioParam audioParam;

    audioParam.filename = "Data/Audio/SE/空中攻撃時.wav";

    audioParam.keyName = "BGM";

    audioParam.loop = false;

    audioParam.volume = 3.0f;

    Se.Play(audioParam);
}

void Player::InputDashSE()
{
    Audio& Se = Audio::Instance();

    AudioParam audioParam;

    audioParam.filename = "Data/Audio/SE/ヒットストップ.wav";

    audioParam.keyName = "BGM";

    audioParam.loop = false;

    audioParam.volume = 3.0f;

    Se.Play(audioParam);
}

// SE再生 斬撃
void Player::InputAttackSlashSE()
{
    Audio& Se = Audio::Instance();

    AudioParam audioParam;

    audioParam.filename = "Data/Audio/SE/スラッシュ２回目.wav";

    audioParam.keyName = "BGM";

    audioParam.loop = false;

    audioParam.volume = 3.0f;

    Se.Play(audioParam);
}

void Player::InputAttackFlameSE()
{
    Audio& Se = Audio::Instance();

    AudioParam audioParam;

    audioParam.filename = "Data/Audio/SE/炎飛行時.wav";

    audioParam.keyName = "BGM";

    audioParam.loop = false;

    audioParam.volume = 3.0f;

    Se.Play(audioParam);
}

void Player::InputAttackThanderSE()
{
    Audio& Se = Audio::Instance();

    AudioParam audioParam;

    audioParam.filename = "Data/Audio/SE/雷.wav";

    audioParam.keyName = "BGM";

    audioParam.loop = false;

    audioParam.volume = 3.0f;

    Se.Play(audioParam);
}

void Player::InputStopAttackThanderSE()
{
    Audio& Se = Audio::Instance();

    std::string filename = "Data/Audio/SE/雷.wav";

    // 種類停止
    Se.Stop(filename);
}

void Player::InputAttackIceSE()
{
    Audio& Se = Audio::Instance();

    AudioParam audioParam;

    audioParam.filename = "Data/Audio/SE/氷発射.wav";

    audioParam.keyName = "BGM";

    audioParam.loop = false;

    audioParam.volume = 3.0f;

    Se.Play(audioParam);
}

void Player::InputAttackHealeSE()
{
    Audio& Se = Audio::Instance();

    AudioParam audioParam;

    audioParam.filename = "Data/Audio/SE/maou_se_magical11.wav";

    audioParam.keyName = "BGM";

    audioParam.loop = false;

    audioParam.volume = 3.0f;

    Se.Play(audioParam);
}

void Player::InputAttackSlashSpecileLightningStrikeSE()
{
    Audio& Se = Audio::Instance();

    AudioParam audioParam;

    audioParam.filename = "Data/Audio/SE/必殺技雷.wav";

    audioParam.keyName = "BGM";

    audioParam.loop = false;

    audioParam.volume = 3.0f;

    Se.Play(audioParam);
}

void Player::InputAttackFlameSpecileSE()
{
    Audio& Se = Audio::Instance();

    AudioParam audioParam;

    audioParam.filename = "Data/Audio/SE/必殺技炎.wav";

    audioParam.keyName = "BGM";

    audioParam.loop = false;

    audioParam.volume = 3.0f;

    Se.Play(audioParam);
}

void Player::UpdateCameraState(float elapsedTime)
{
    CameraState oldLockonState = lockonState;
    DirectX::XMFLOAT3 oldLockonCharacter = lockonCharactor;
    lockonState = CameraState::NotLockOn;
    lockonCharactor = {0,0,0};

    //if (stateMachine->GetStateSize() <= stateSize)
    //{
    //    Model::Node* PRock = model->FindNode("mixamorig:Spine1");
    //    DirectX::XMFLOAT3 pPosition =
    //    {
    //                PRock->worldTransform._41,
    //                PRock->worldTransform._42,
    //                PRock->worldTransform._43
    //    };

    //    MessageData::CAMERACHANGEFREESELECTMODEDATA	p = { pPosition };
    //    Messenger::Instance().SendData(MessageData::CAMERACHANGEFREESELECTMODE, &p);


    //    return;
    //}

        // ロックオンモード
    if (rockCheck)
    {
        Model::Node* PRock = model->FindNode("mixamorig:Spine1");
        //Model::Node* PRock = model->FindNode("mixamorig:Neck");
        DirectX::XMFLOAT3 pPosition =
        {
                    PRock->worldTransform._41,
                    PRock->worldTransform._42,
                    PRock->worldTransform._43
        };
        pPosition.z *= 1.1f;
        DirectX::XMVECTOR p, t, v;
        switch (oldLockonState)
        {
        case	CameraState::NotLockOn:
        {
            // 一番近い距離のキャラクターを検索
            float	length1, length2;
            int enemyCount = EnemyManager::Instance().GetEnemyCount();
            for (int ii = 0; ii < enemyCount; ++ii)
            {
                Model::Node* characterWorld = EnemyManager::Instance().GetEnemy(ii)->GetComponent<ModelControll>()->GetModel()->FindNode("shoulder");
                //Model::Node* characterWorld = EnemyManager::Instance().GetEnemy(ii)->GetComponent<ModelControll>()->GetModel()->FindNode("body1");
                
                DirectX::XMFLOAT3 character = 
                {
                    characterWorld->worldTransform._41,
                    characterWorld->worldTransform._42,
                    characterWorld->worldTransform._43
                };

                if (lockonState != CameraState::NotLockOn)
                {
                   
                    p = DirectX::XMLoadFloat3(&pPosition);
                    t = DirectX::XMLoadFloat3(&lockonCharactor);
                    v = DirectX::XMVectorSubtract(t, p);
                    DirectX::XMStoreFloat(&length2, DirectX::XMVector3LengthSq(v));
                    p = DirectX::XMLoadFloat3(&pPosition);
                    t = DirectX::XMLoadFloat3(&character);
                    v = DirectX::XMVectorSubtract(t, p);
                    DirectX::XMStoreFloat(&length1, DirectX::XMVector3LengthSq(v));
                    if (length1 < length2)
                    {
                        lockonCharactor = character;

                        
                    }
                }
                else
                {
                    p = DirectX::XMLoadFloat3(&pPosition);
                    t = DirectX::XMLoadFloat3(&character);
                    v = DirectX::XMVectorSubtract(t, p);
                    DirectX::XMStoreFloat(&length1, DirectX::XMVector3LengthSq(v));

                    lockonCharactor = character;
                    lockonState = CameraState::LockOn;
                }
            }
            break;
        }
        case	CameraState::LockOn:
        {
            // ロックオン対象が存在しているかチェックして
            // 対象がいればロックオンを継続させる
            int enemyCount = EnemyManager::Instance().GetEnemyCount();
            for (int ii = 0; ii < enemyCount; ++ii)
            {
                Model::Node* characterWorld = EnemyManager::Instance().GetEnemy(ii)->GetComponent<ModelControll>()->GetModel()->FindNode("body1");

                DirectX::XMFLOAT3 character =
                {
                    characterWorld->worldTransform._41,
                    characterWorld->worldTransform._42,
                    characterWorld->worldTransform._43
                };

                //if (character.y >= topHeight)
                //{
                //    lockonState = CameraState::AttackLock;
                //    
                //}

                stateEnemyIndex = EnemyManager::Instance().GetEnemy(ii)->GetComponent<EnemyBoss>()->GetStateMachine()->
                    GetStateIndex();

                if (stateEnemyIndex == (int)EnemyBoss::State::Jump ||
                    stateEnemyIndex == (int)EnemyBoss::State::Attack)
                {
                    lockonState = CameraState::AttackLock;
                }

                
                //if (character == oldLockonCharacter)
                //{
                //lockonCharactor = character;
                else
                {
                    lockonState = CameraState::LockOn;
                }


                p = DirectX::XMLoadFloat3(&pPosition);
                t = DirectX::XMLoadFloat3(&character);
                v = DirectX::XMVectorSubtract(t, p);

                lockonCharactor = character;
                //DirectX::XMStoreFloat3(&lockDirection, DirectX::XMVector3Normalize(v));
                //break;

            }
            // 右スティックでロックオン対象を変更する処理
            GamePad& gamePad = Input::Instance().GetGamePad();
            float ax = gamePad.GetAxisRX();	// 水平のみ
            // 垂直方向は使わないでおく
            lockonTargetChangeTime -= 60.0f * elapsedTime;
            if (
                lockonTargetChangeTime <= 0 &&
                ax * ax > 0.3f)
            {
                lockonTargetChangeTime = lockonTargetChangeTimeMax;
                // ロックオン対象と自分自身の位置からベクトルを算出
                float dx = oldLockonCharacter.x - pPosition.x;
                float dz = oldLockonCharacter.z - pPosition.z;
                float l = sqrtf(dx * dx + dz * dz);
                dx /= l;
                dz /= l;
                // 外積を用いて左右判定を行い、角度的に最も近い対象にロックオンを変える
                float angleMax = FLT_MAX;
                for (int ii = 0; ii < enemyCount; ++ii)
                {
                    //std::shared_ptr<Transform> character = EnemyManager::Instance().GetEnemy(ii)->GetComponent<Transform>();
                    
                    Model::Node* characterWorld = EnemyManager::Instance().GetEnemy(ii)->GetComponent<ModelControll>()->GetModel()->FindNode("body1");

                    DirectX::XMFLOAT3 character =
                    {
                        characterWorld->worldTransform._41,
                        characterWorld->worldTransform._42,
                        characterWorld->worldTransform._43
                    };

       /*             if (character == oldLockonCharacter)
                        continue;*/
                    float ddx = character.x - pPosition.x;
                    float ddz = character.z - pPosition.z;
                    float ll = sqrtf(ddx * ddx + ddz * ddz);
                    ddx /= ll;
                    ddz /= ll;
                    float cross = dx * ddz - dz * ddx;
                    if (ax > 0 && cross < 0)
                    {
                        cross = abs(cross);
                        if (cross < angleMax)
                        {
                            angleMax = cross;
                            lockonCharactor = character;
                        }
                    }
                    else if (ax < 0 && cross > 0)
                    {
                        if (cross < angleMax)
                        {
                            angleMax = cross;
                            lockonCharactor = character;
                        }
                    }
                }
            }
            break;
        }
        case CameraState::AttackLock:
        {
            lockonState = CameraState::AttackLock;

            int enemyCount = EnemyManager::Instance().GetEnemyCount();
            for (int ii = 0; ii < enemyCount; ++ii)
            {
                Model::Node* characterWorld = EnemyManager::Instance().GetEnemy(ii)->GetComponent<ModelControll>()->GetModel()->FindNode("body1");

                DirectX::XMFLOAT3 character;

                lockonCharactor =
                    EnemyManager::Instance().GetEnemy(ii)->
                    GetComponent<ModelControll>()->GetModel()->
                    ConvertLocalToWorld(characterWorld);

                stateEnemyIndex = EnemyManager::Instance().GetEnemy(ii)->GetComponent<EnemyBoss>()->GetStateMachine()->
                    GetStateIndex();

                //if (character.y <= minHeight)
                if (stateEnemyIndex != (int)EnemyBoss::State::Jump &&
                    stateEnemyIndex != (int)EnemyBoss::State::Attack)
                {
                    lockonState = CameraState::LockOn;
                }

                //lockonCharactor = character;
            }


            break;
        }
        }
        if (lockonState == CameraState::LockOn)
        {
            MessageData::CAMERACHANGELOCKONMODEDATA	p = { pPosition, lockonCharactor };
            Messenger::Instance().SendData(MessageData::CAMERACHANGELOCKONMODE, &p);
            //break;
        }

        if (lockonState == CameraState::AttackLock)
        {
            MessageData::CAMERACHANGELOCKONHEIGHTMODEDATA	p = { pPosition, lockonCharactor };
            Messenger::Instance().SendData(MessageData::CAMERACHANGELOCKONTOPHEIGHTMODE, &p);
        }
    }
    else if(freeCameraCheck)
    {
        Model::Node* PRock = model->FindNode("mixamorig:Spine1");
        //Model::Node* PRock = model->FindNode("mixamorig:Hips");
        //Model::Node* PRock = model->FindNode("mixamorig:Neck");
        DirectX::XMFLOAT3 pPosition =
        {
                    PRock->worldTransform._41,
                    PRock->worldTransform._42,
                    PRock->worldTransform._43
        };
        MessageData::CAMERACHANGEFREEMODEDATA	p = { pPosition };
        Messenger::Instance().SendData(MessageData::CAMERACHANGEFREEMODE, &p);
    }


    
}



// デバッグプリミティブ描画
void Player::DrawDebugPrimitive()
{
    // もし立法体等ほしいなら自分で追加
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //// 衝突判定用のデバッグ球を描画
    debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    // 衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder( 
            position, radius, height, DirectX::XMFLOAT4(0, 0, 1, 1));


    if (attackCollisionFlag)
    {


        // 攻撃衝突用の左手ノードデバッグ球を描画
        Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
        debugRenderer->DrawSphere(DirectX::XMFLOAT3(
            leftHandBone->worldTransform._41,
            leftHandBone->worldTransform._42,
            leftHandBone->worldTransform._43),
            leftHandRadius,
            DirectX::XMFLOAT4(1, 0, 0, 1)
        );
    }
}
#ifdef _DEBUG
void Player::OnGUI()
{

    ImGui::InputFloat("Move Speed", &moveSpeed);
    ImGui::InputInt("selectCheck", &selectCheck);
    ImGui::InputInt("selectMagicCheck", &selectMagicCheck);
    //ImGui::InputInt("specialAttack.top", &specialAttack.top());
    ImGui::SliderFloat("specialAttackCharge", &specialAttackCharge,0,1.5f);
    ImGui::SliderFloat("specialShotCharge", &specialShotCharge,0,1.5f);

    ImGui::SliderFloat("dot", &dotfake, 0, 1);

    ImGui::SliderFloat("blend", &moveSpeedAnimation, 0.0f, 1.0f);

    
    if (ImGui::Button("RockCamera"))
    {
        SpecialCameraRock = SpecialCameraRock ? false : true;
    }

    ImGui::SliderFloat("ShakeTimer", &shakeTimer,0,10);
    ImGui::SliderFloat("shakePower", &shakePower, 0, 10);


    if (ImGui::Button("CameraShake"))
    {
        // カメラ振動
        MessageData::CAMERASHAKEDATA cameraShakeData;


        cameraShakeData = { shakeTimer , shakePower };

        Messenger::Instance().SendData(MessageData::CAMERASHAKE, &cameraShakeData);

    }


    if (SpecialCameraRock)
    {
        MessageData::CAMERACHANGEMOTIONMODEDATA	p;

        float vx = sinf(angle.y) * 6;
        float vz = cosf(angle.y) * 6;
        p.data.push_back({ 0, {position.x + vx , position.y + 3, position.z + vz }, position });
        p.data.push_back({ 90, {position.x - vx , position.y + 3, position.z - vz }, position });
        p.data.push_back({ 180, {position.x - vx , position.y + 3, position.z - vz }, position });
        

        Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

    }


    if (ImGui::Button("Samunail"))
    {

        //stateMachine->ChangeState((int)State::SpecialAttack);

        MessageData::CAMERACHANGEMOTIONMODEDATA	p;

        position = transform.lock()->GetPosition();
        angle = transform.lock()->GetAngle();



        float vx = sinf(angle.y) * 6;
        float vz = cosf(angle.y) * 6;

        float vx2 = sinf(angle.y) - 10;
        float vx3 = sinf(angle.y);
        float vz2 = cosf(angle.y) * 7;

        //float vx2 = sinf(angle.y);
        //float vz2 = cosf(angle.y);

        //DirectX::XMFLOAT3 ePos = EnemyManager::Instance().GetEnemy(0)->GetComponent<Transform>()->GetPosition();

        //DirectX::XMFLOAT3 pos;

        //pos =
        //{
        //     position.x - ePos.x,
        //     position.y - ePos.y,
        //     position.z - ePos.z,

        //};

        p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
        p.data.push_back({ 50, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
        p.data.push_back({ 90, {position.x + vx3 , position.y + 2, (position.z + 0.1f) - vz2 }, position });

        Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

        Model::ModelAnim modelAnim;

        modelAnim.index = Player::Animation::Anim_SpecialAttack;

        // アニメーション再生
        model->PlayAnimation(modelAnim);


        angleCameraCheck = true;

       

        //MessageData::CAMERACHANGEMOTIONMODEDATA	p;

        //float vx = sinf(angle.y) * 6;
        //float vz = cosf(angle.y) * 6;
        //p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });

        //Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);



        
    }

    if (angleCameraCheck)
    {
        Model::Node* pHPosiiton = model->FindNode("mixamorig:LeftHand");

        DirectX::XMFLOAT3 pPosition =
        {
                    pHPosiiton->worldTransform._41,
                    pHPosiiton->worldTransform._42,
                    pHPosiiton->worldTransform._43
        };


        // 任意のアニメーション再生区間でのみ衝突判定処理をする
        float animationTime = model->GetCurrentANimationSeconds();

        if (animationTime >= 1.1f - FLT_EPSILON && animationTime <= 1.2f + FLT_EPSILON)
        {
            hitCheck = true;
        }

        // アニメーション
        if (animationTime >= 1.6f)
        {
            // カメラ振動
            MessageData::CAMERASHAKEDATA cameraShakeData;

            //float shakeTimer = 0.5f;
            //float shakePower = 0.8f;

            float shakeTimer2 = 0.3f;
            cameraShakeData = { shakeTimer2 , shakePower };

            Messenger::Instance().SendData(MessageData::CAMERASHAKE, &cameraShakeData);

            lightningAttack->Play(pPosition);
            angleCameraCheck = false;

            flashOn = true;

  
        }
    }
    if (ImGui::Button("ChargeSpeciale"))
    {
        // 攻撃チャージ
        specialAttackCharge = 1.2f;


        // 斬撃必殺技チャージ解消
        attackEnergyCharge = 3;
    }

    if (ImGui::Button("ChargeSlashSpeciale"))
    {
        // 一度発動すると初期化
        specialAttackCharge = specialAttackChargeMax;

        // 斬撃必殺技チャージ解消
        attackEnergyCharge = energyChargeMax;

    }

    if (ImGui::Button("ChargeFireSpeciale"))
    {
        // 一度発動すると初期化
        specialAttackCharge = specialAttackChargeMax;

        // 火必殺技チャージ解消
        fireEnergyCharge = energyChargeMax;
    }




    ImGui::SliderFloat("brigthness", &colorGradingData.brigthness,0.0f ,10.0f);
    ImGui::SliderFloat("hueShift", &colorGradingData.hueShift,0.0f ,10.0f);
    ImGui::SliderFloat("saturation", &colorGradingData.saturation,0.0f ,10.0f);


}
#endif // _DEBUG

// 移動入力処理
bool Player::InputMove(float elapsedTime)
{

    // 入力情報を取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    // 直進弾丸発射　rボタンを押したら
  
    if (ax + FLT_EPSILON != 0.0f + FLT_EPSILON &&
        GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage) &&
        GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Death))
    {
        
        return true;
    }

    if (ay + FLT_EPSILON != 0.0f + FLT_EPSILON &&
        GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage) &&
        GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Death))
    {

        return true;
    }

    //if (gamePad.GetButtonDown() & GamePad::BTN_DOWN &&
    //    GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage))
    //{

    //    return true;
    //}

    //if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT &&
    //    GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage))
    //{

    //    return true;
    //}

    //if (gamePad.GetButtonDown() & GamePad::BTN_LEFT &&
    //    GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage))
    //{

    //    return true;
    //}


    //// 進行ベクトル取得
    ////DirectX::XMFLOAT3 moveVec = GetMoveVec();
    //moveVec = GetMoveVec(elapsedTime);

    //
    //// 進行ベクトルがゼロベクトルでない場合は入力された
    //if(GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage))
    //return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;

    return false;
    

}
bool Player::InputRockOn()
{
   

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT_SHOULDER && !buttonRock && !rockCheck)
    {

        buttonRock = true;
        rockCheck = true;
        return true;
    }


    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT_SHOULDER && !buttonRock && rockCheck)
    {
        //cameraControlle->SetTarget(position);
        rockCheck = false;
        buttonRock = true;
        return true;
    }


    if (gamePad.GetButtonUp() & GamePad::BTN_RIGHT_SHOULDER)
        buttonRock = false;

    return false;
}
void Player::RockOnUI(ID3D11DeviceContext* dc,
    const DirectX::XMFLOAT4X4& view,
    const DirectX::XMFLOAT4X4& projection)
{
    //if (!rockCheck)return;
    // ビューポート 画面のサイズ等
    // ビューポートとは2Dの画面に描画範囲の指定(クリッピング指定も出来る)位置を指定
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    // ラスタライザーステートにバインドされているビューポート配列を取得
    dc->RSGetViewports(&numViewports, &viewport);

    // 変換行列
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
    // ローカルからワールドに行くときにいる奴相手のポジションを渡す。
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

    std::weak_ptr<Ui> uiIdSight;
    std::weak_ptr<TransForm2D> uiIdSightTransform;

    std::weak_ptr<Ui> uiIdSightMove;
    std::weak_ptr<TransForm2D> uiIdSightMoveTransform;

    int uiCount = UiManager::Instance().GetUiesCount();

    if (uiCount <= uiCountMax) return;
        uiIdSight = UiManager::Instance().GetUies((int)UiManager::UiCount::Sight)->GetComponent<Ui>();
        uiIdSightTransform = UiManager::Instance().GetUies((int)UiManager::UiCount::Sight)->GetComponent<TransForm2D>();
        uiIdSightMove = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<Ui>();
        uiIdSightMoveTransform = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<TransForm2D>();
    
    // 全ての敵の頭上にHPゲージを表示
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();

    for (int i = 0; i < enemyCount; ++i)
    {


        //Model::Node* characterWorld = EnemyManager::Instance().GetEnemy(i)->GetComponent<ModelControll>()->GetModel()->FindNode("body1");
        Model::Node* characterWorld = EnemyManager::Instance().GetEnemy(i)->GetComponent<ModelControll>()->GetModel()->FindNode("shoulder");

        //std::shared_ptr <Transform> enemy = enemyManager.GetEnemy(i)->GetComponent<Transform>();
        // エネミー頭上
        DirectX::XMFLOAT3 worldPosition =
        {
            
            characterWorld->worldTransform._41,
            characterWorld->worldTransform._42,
            characterWorld->worldTransform._43
        };

        // ワールドからスクリーン
        DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&worldPosition);






        // ゲージ描画 // ワールドからスクリーン

        DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
            WorldPosition,
            viewport.TopLeftX,
            viewport.TopLeftY,
            viewport.Width,
            viewport.Height,
            viewport.MinDepth,
            viewport.MaxDepth,
            Projection,
            View,
            World

        );
        // スクリーン座標
        DirectX::XMFLOAT3 scereenPosition;
        DirectX::XMStoreFloat3(&scereenPosition, ScreenPosition);

        // ゲージ長さ
        //const float gaugeWidth = 30.0f;
        //const float gaugeHeight = 5.0f;

        //float healthRate = enemy->GetHealth() / static_cast<float>(enemy->GetMaxHealth());

        if (rockCheck || !specialRockOff)
        {
            bool drawCheck = true;
            uiIdSight.lock()->SetDrawCheck(drawCheck);
        }

        if (scereenPosition.z < 0.0f || scereenPosition.z > 1.0f || !rockCheck || specialRockOff)
        {
            bool drawCheck = false;
            uiIdSight.lock()->SetDrawCheck(drawCheck);

            //uiIdSightMove->SetDrawCheck(drawCheck);

            continue;
        }

       // uiIdSightMove->SetDrawCheck(drawCheck);

        // 2Dスプライト描画
        {
            uiIdSightTransform.lock()->SetPosition(
                { 
                    scereenPosition.x,
                    scereenPosition.y
                });
            uiIdSightMoveTransform.lock()->SetPosition(
                {
                    scereenPosition.x - 34,
                    scereenPosition.y - 25
                });

            //// 描画
            //gauge->Render(dc,
            //    scereenPosition.x - gaugeWidth * 0.5f,
            //    scereenPosition.y - gaugeHeight
            //    , gaugeWidth * healthRate
            //    , gaugeHeight,
            //    0, 0,
            //    static_cast<float> (gauge->GetTextureWidth()),
            //    static_cast<float> (gauge->GetTextureHeight()),
            //    0.0f,
            //    1, 0, 0, 1);
            // {位置}{サイズ}{画像どこから}{画像何処まで}
            // dc , ｛範囲｝｛｝
        }



    }

}
// 攻撃方法選択
bool Player::InputSelectCheck()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_UP && !magicAction)
    {
        ++selectCheck;
    }
    if (gamePad.GetButtonDown() & GamePad::BTN_DOWN && !magicAction)
    {
        --selectCheck;
    }
    // コマンド操作階層下げ
    if (gamePad.GetButtonDown() & GamePad::BTN_B && selectCheck == (int)CommandAttack::Magic && !magicAction)
    {
        magicAction = true;

        gamePad.SetButtonDownCountinue(true);
        return true;
    }
    // 一度離すまでボタン効かない
    if (gamePad.GetButtonUp() & GamePad::BTN_B)
    {
        gamePad.SetButtonDownCountinue(false);
    }
    // コマンド操作階層下げ
    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT && selectCheck == (int)CommandAttack::Magic)
    {
        magicAction = true;
        return true;
    }
    // ループ操作 最大にいったら
    if (selectCheck > (int)CommandAttack::Magic)
    {
        selectCheck = (int)CommandAttack::Attack;
    }
    // ループ操作　最小に行ったら
    if (selectCheck < (int)CommandAttack::Attack)
    {
        selectCheck = (int)CommandAttack::Magic;
    }

    int uiCount = UiManager::Instance().GetUiesCount();
    // ui無かったら
    if (uiCount <= uiCountMax) return false;

    // UI設定 階層下がる前 選ぶ
    // 攻撃選ぶ
    if (selectCheck == (int)CommandAttack::Attack)
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttack)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttackCheck)->GetComponent<Ui>();

        
        uiIdAttack.lock()->SetDrawCheck(false);
        uiIdAttackCheck.lock()->SetDrawCheck(true);
    }
    // 魔法選んだ時
    else
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttack)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttackCheck)->GetComponent<Ui>();


        uiIdAttack.lock()->SetDrawCheck(true);
        uiIdAttackCheck.lock()->SetDrawCheck(false);
    }
    // 魔法選んだ時
    if (selectCheck == (int)CommandAttack::Magic)
    {
        std::weak_ptr<Ui> uiIdMagick = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagick)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdMagickCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagickCheck)->GetComponent<Ui>();


        uiIdMagick.lock()->SetDrawCheck(false);
        uiIdMagickCheck.lock()->SetDrawCheck(true);
    }
    // 攻撃選ぶ
    else
    {
        std::weak_ptr<Ui> uiIdMagick = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagick)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdMagickCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagickCheck)->GetComponent<Ui>();


        uiIdMagick.lock()->SetDrawCheck(true);
        uiIdMagickCheck.lock()->SetDrawCheck(false);
    }


    return false;
}

// 魔法のコマンド
bool Player::InputSelectMagicCheck()
{

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_UP && magicAction)
    {
        --selectMagicCheck;
    }
    if (gamePad.GetButtonDown() & GamePad::BTN_DOWN && magicAction)
    {
        ++selectMagicCheck;
    }

    if (gamePad.GetButtonDown() & GamePad::BTN_LEFT && magicAction)
    {
        magicAction = false;
        
    }
    if (selectMagicCheck <= (int)CommandMagic::Normal && magicAction)
    {
        selectMagicCheck = (int)CommandMagic::Fire;
    }

    if (selectMagicCheck > (int)CommandMagic::Heale && magicAction)
    {
        selectMagicCheck = (int)CommandMagic::Fire;
    }

    if (selectMagicCheck < (int)CommandMagic::Fire && magicAction)
    {
        selectMagicCheck = (int)CommandMagic::Heale;
    }

    ///////////////////////////
    // ショートカットキー 試し中


    if (gamePad.GetButtonUp())
    {
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<Ui>();

        uiIdAttackCheck.lock()->SetDrawCheck(false);
    }

    if (InputShortCutkeyMagic() &&
        gamePad.GetButtonDown() & GamePad::BTN_B )
    {
        selectMagicCheck = (int)CommandMagic::Fire;

        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<Ui>();


        std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = { 179, 340 };



        uiIdAttackCheck.lock()->SetDrawCheck(true);

        uiIdAttackCheckPos.lock()->SetPosition(pos);
    }


    if (InputShortCutkeyMagic() &&
        gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        selectMagicCheck = (int)CommandMagic::Thander;

        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<Ui>();


        std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = { 179, 440 };



        uiIdAttackCheck.lock()->SetDrawCheck(true);
        
        uiIdAttackCheckPos.lock()->SetPosition(pos);
    }

    if (InputShortCutkeyMagic() &&
        gamePad.GetButtonDown() & GamePad::BTN_Y )
    {
        selectMagicCheck = (int)CommandMagic::Heale;

        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<Ui>();

       
        std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = { 200, 640 };



        uiIdAttackCheck.lock()->SetDrawCheck(true);

        uiIdAttackCheckPos.lock()->SetPosition(pos);
    }
 


    if (InputShortCutkeyMagic() &&
        gamePad.GetButtonDown() & GamePad::BTN_A )
    {
        selectMagicCheck = (int)CommandMagic::Ice;

        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<Ui>();


        std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = { 179, 540 };



        uiIdAttackCheck.lock()->SetDrawCheck(true);

        uiIdAttackCheckPos.lock()->SetPosition(pos);
    }
    /////////////////////////

    if (!magicAction)
    {
        selectMagicCheck = (int)CommandMagic::Normal;
    }

    if (InputShortCutkeyMagic()) return false;

    int uiCount = UiManager::Instance().GetUiesCount();
    // ui無かったら
    if (uiCount <= uiCountMax) return false;

    // UI設定 炎
    if (selectMagicCheck == (int)CommandMagic::Fire && magicAction)
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFire)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFireCheck)->GetComponent<Ui>();


        uiIdAttack.lock()->SetDrawCheck(false);
        uiIdAttackCheck.lock()->SetDrawCheck(true);
    }
    else
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFire)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFireCheck)->GetComponent<Ui>();


        uiIdAttack.lock()->SetDrawCheck(true);
        uiIdAttackCheck.lock()->SetDrawCheck(false);
    }
    if (!magicAction)
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFire)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFireCheck)->GetComponent<Ui>();


        uiIdAttack.lock()->SetDrawCheck(false);
        uiIdAttackCheck.lock()->SetDrawCheck(false);
    }


    // UI設定 雷
    if (selectMagicCheck == (int)CommandMagic::Thander && magicAction)
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtning)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtningCheck)->GetComponent<Ui>();


        uiIdAttack.lock()->SetDrawCheck(false);
        uiIdAttackCheck.lock()->SetDrawCheck(true);
    }
    else
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtning)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtningCheck)->GetComponent<Ui>();


        uiIdAttack.lock()->SetDrawCheck(true);
        uiIdAttackCheck.lock()->SetDrawCheck(false);
    }
    if (!magicAction)
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtning)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtningCheck)->GetComponent<Ui>();


        uiIdAttack.lock()->SetDrawCheck(false);
        uiIdAttackCheck.lock()->SetDrawCheck(false);
    }


    // UI設定 氷
    if (selectMagicCheck == (int)CommandMagic::Ice && magicAction)
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIce)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIceCheck)->GetComponent<Ui>();


        uiIdAttack.lock()->SetDrawCheck(false);
        uiIdAttackCheck.lock()->SetDrawCheck(true);
    }
    else
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIce)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIceCheck)->GetComponent<Ui>();


        uiIdAttack.lock()->SetDrawCheck(true);
        uiIdAttackCheck.lock()->SetDrawCheck(false);
    }
    if (!magicAction)
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIce)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIceCheck)->GetComponent<Ui>();


        uiIdAttack.lock()->SetDrawCheck(false);
        uiIdAttackCheck.lock()->SetDrawCheck(false);
    }



    // UI設定 回復
    if (selectMagicCheck == (int)CommandMagic::Heale && magicAction)
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandHeale)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandHealeCheck)->GetComponent<Ui>();


        uiIdAttack.lock()->SetDrawCheck(false);
        uiIdAttackCheck.lock()->SetDrawCheck(true);
    }
    else
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandHeale)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandHealeCheck)->GetComponent<Ui>();


        uiIdAttack.lock()->SetDrawCheck(true);
        uiIdAttackCheck.lock()->SetDrawCheck(false);
    }
    if (!magicAction)
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandHeale)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandHealeCheck)->GetComponent<Ui>();


        uiIdAttack.lock()->SetDrawCheck(false);
        uiIdAttackCheck.lock()->SetDrawCheck(false);
    }


    return false;
}

// L1
bool Player::InputShortCutkeyMagic()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    // L1 ショートカット魔法
    if (gamePad.GetButtonDown() & GamePad::BTN_LEFT_SHOULDER)
    {
        selectCheck = (int)CommandAttack::Magic;
        selectMagicCheck = (int)CommandMagic::Fire;
        magicAction = true;


        return true;
    }
    // 押してる間選択
    if (gamePad.GetButton() & GamePad::BTN_LEFT_SHOULDER)
    {
        magicAction = true;

        // 炎
        std::weak_ptr<Ui> uiIdAttackCheckFire = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutFire)->GetComponent<Ui>();

        uiIdAttackCheckFire.lock()->SetDrawCheck(true);

        // 雷
        std::weak_ptr<Ui> uiIdAttackCheckSunder = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutSunder)->GetComponent<Ui>();

        uiIdAttackCheckSunder.lock()->SetDrawCheck(true);

        // 氷
        std::weak_ptr<Ui> uiIdAttackCheckIce = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutIce)->GetComponent<Ui>();

        uiIdAttackCheckIce.lock()->SetDrawCheck(true);

        // 回復
        std::weak_ptr<Ui> uiIdAttackCheckHeale = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutKeule)->GetComponent<Ui>();

        uiIdAttackCheckHeale.lock()->SetDrawCheck(true);

        return true;
    }
    // 離したら
    if (gamePad.GetButtonUp() & GamePad::BTN_LEFT_SHOULDER)
    {
        selectMagicCheck = (int)CommandMagic::Normal;
        selectCheck = (int)CommandAttack::Attack;
        magicAction = false;

        // 炎
        std::weak_ptr<Ui> uiIdAttackCheckFire = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutFire)->GetComponent<Ui>();

        uiIdAttackCheckFire.lock()->SetDrawCheck(false);

        // 雷
        std::weak_ptr<Ui> uiIdAttackCheckSunder = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutSunder)->GetComponent<Ui>();

        uiIdAttackCheckSunder.lock()->SetDrawCheck(false);

        // 氷
        std::weak_ptr<Ui> uiIdAttackCheckIce = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutIce)->GetComponent<Ui>();

        uiIdAttackCheckIce.lock()->SetDrawCheck(false);

        // 回復
        std::weak_ptr<Ui> uiIdAttackCheckHeale = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutKeule)->GetComponent<Ui>();

        uiIdAttackCheckHeale.lock()->SetDrawCheck(false);

    }
    return false;
}

bool Player::InputSpecialAttackCharge()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    int uiCount = UiManager::Instance().GetUiesCount();
    // ui無かったら
    if (uiCount <= uiCountMax) return false;

    if (specialAttackCharge >= specialAttackChargeMax)
    {
        std::weak_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdSpecialChargeSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<Ui>();

        // 必殺技たまった音
        PlaySpecialChargeCompleteSe();

        //// 技確定
        //std::shared_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
        //std::shared_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();


        //// 一度発動すると初期化
        specialAttackCharge = 0.0f;
        //specialAttack.push((int)SpecialAttack::Attack);

        bool drawCheck = false;
        uiIdSpecialChargeFurst.lock()->SetDrawCheck(drawCheck);

        uiIdSpecialChargeSecond.lock()->SetDrawCheck(drawCheck);

        uiIdSpecialChargeSerde.lock()->SetDrawCheck(drawCheck);

        //for (int i = 0; i < specialAttack.size(); ++i)
        //{
        //    specialAttack;
        //}

            // 剣攻撃を一定以上溜めたら
            if (attackEnergyCharge >= energyChargeMax)
            {
                // 技確定
                std::weak_ptr<Ui> uiIdAttackSpecial = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
                std::weak_ptr<TransForm2D> uiIdSpecialAttackTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();


                bool drawCheck = true;
                uiIdAttackSpecial.lock()->SetDrawCheck(drawCheck);


                // 必殺技何が登録されたか 斬撃
                specialAttack.push((int)SpecialAttack::Attack);


                DirectX::XMFLOAT2 pos;
                pos = { 94,240 };
                float add = 30;

                if (2 < (int)specialAttack.size())
                    pos.y = pos.y - (add * (float)specialAttack.size());
                uiIdSpecialAttackTransForm2D.lock()->SetPosition(pos);

                // 一度発動すると初期化
                specialAttackCharge = 0.0f;

                // 斬撃必殺技チャージ解消
                attackEnergyCharge = 0;
            }


            // 炎魔法を一定以上溜めたら
            if (fireEnergyCharge >= energyChargeMax)
            {
                // 技確定
                std::weak_ptr<Ui> uiIdSpecialFire = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<Ui>();
                std::weak_ptr<TransForm2D> uiIdSpecialFireTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<TransForm2D>();


                bool drawCheck = true;
                uiIdSpecialFire.lock()->SetDrawCheck(drawCheck);

                // 必殺技何が登録されたか 火
                specialAttack.push((int)SpecialAttack::MagicFire);

                DirectX::XMFLOAT2 pos;
                pos = { 94,240 };
                float add = 30;
                if (2 < (int)specialAttack.size())
                    pos.y = pos.y - (add * (float)specialAttack.size());
                uiIdSpecialFireTransForm2D.lock()->SetPosition(pos);

                // 一度発動すると初期化
                specialAttackCharge = 0.0f;


                // 火必殺技チャージ解消
                fireEnergyCharge = 0;
            }

            //// 雷魔法を一定以上溜めたら
            //if (ThanderEnergyCharge >= energyChargeMax)
            //{


            //    // 技確定
            //    std::weak_ptr<Ui> uiIdSpecialThander = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulThander)->GetComponent<Ui>();
            //    std::weak_ptr<TransForm2D> uiIdSpecialThanderTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulThander)->GetComponent<TransForm2D>();


            //    bool drawCheck = true;
            //    uiIdSpecialThander.lock()->SetDrawCheck(drawCheck);

            //    // 必殺技何が登録されたか 斬撃
            //    specialAttack.push((int)SpecialAttack::MagicThander);

            //    DirectX::XMFLOAT2 pos;
            //    pos = { 94,240 };
            //    float add = 30;
            //    if (2 < (int)specialAttack.size())
            //        pos.y = pos.y - (add * (float)specialAttack.size());
            //    uiIdSpecialThanderTransForm2D.lock()->SetPosition(pos);

            //    // 一度発動すると初期化
            //    specialAttackCharge = 0.0f;


            //    // 雷必殺技チャージ解消
            //    ThanderEnergyCharge = 0;
            //}

            //// 氷魔法を一定以上溜めたら
            //if (iceEnergyCharge >= energyChargeMax)
            //{
            //    // 技確定
            //    std::weak_ptr<Ui> uiIdSpecialIce = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulIce)->GetComponent<Ui>();
            //    std::weak_ptr<TransForm2D> uiIdSpecialIceTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulIce)->GetComponent<TransForm2D>();


            //    bool drawCheck = true;
            //    uiIdSpecialIce.lock()->SetDrawCheck(drawCheck);

            //    // 必殺技何が登録されたか 氷
            //    specialAttack.push((int)SpecialAttack::MagicIce);

            //    DirectX::XMFLOAT2 pos;
            //    pos = { 94,240 };
            //    float add = 30;
            //    if (2 < (int)specialAttack.size())
            //        pos.y = pos.y - (add * (float)specialAttack.size());
            //    uiIdSpecialIceTransForm2D.lock()->SetPosition(pos);


            //    // 一度発動すると初期化
            //    specialAttackCharge = 0.0f;


            //    // 氷必殺技チャージ解消
            //    iceEnergyCharge = 0;
            //}

       

        //drawCheck = true;
        //uiIdSpecialShurashu->SetDrawCheck(drawCheck);

        //DirectX::XMFLOAT2 pos;
        //pos = { 94,240 };
        //float add = 30;
        //if (2 < (int)specialAttack.size())
        //    pos.y = pos.y - (add * (float)specialAttack.size());
        //uiIdSpecialShurashuTransForm2D->SetPosition(pos);

    }
    //if (gamePad.GetButtonDown() & GamePad::BTN_Y && specialAttack.top() == (int)SpecialAttack::Attack && !specialAttackTime)
    // 技を放つ
    if (gamePad.GetButtonDown() & GamePad::BTN_Y &&  !specialAttackTime && specialAttack.size() > 0)
    {


        //// 技確定
        //std::shared_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
        //std::shared_ptr<Ui> uiIdSpecialShurashuPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<Ui>();
        //std::shared_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();
        //std::shared_ptr<TransForm2D> uiIdSpecialShurashuPushTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<TransForm2D>();

        //bool drawCheck = false;
        //uiIdSpecialShurashu->SetDrawCheck(drawCheck);
        //drawCheck = true;
        //uiIdSpecialShurashuPush->SetDrawCheck(drawCheck);

        //DirectX::XMFLOAT2 pos;
        //pos = { uiIdSpecialShurashuTransForm2D->GetPosition() };
        ////float add = 30;
        ////if (1 < (int)specialAttack.size())
        ////    pos.y = pos.y - (add * (float)specialAttack.size());
        //uiIdSpecialShurashuPushTransForm2D->SetPosition(pos);

        // エネミー呼ぶ奴
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyManagerCount = enemyManager.GetEnemyCount();

        // 動作させるかどうか
        if (enemyManagerCount > 0)
        {

            std::weak_ptr<EnemyBoss> enemy = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<EnemyBoss>();
            std::weak_ptr<Movement> enemyMove = enemyManager.GetEnemy(enemyManagerCount - 1)->GetComponent<Movement>();

            bool moveCheck = false;
            enemy.lock()->SetMoveCheck(moveCheck);

            // 速度停止
            bool stopVelocity = true;
            enemyMove.lock()->SetStopMove(stopVelocity);
            // 落ちるの停止
            bool stopFall = true;
            enemyMove.lock()->SetStopFall(stopFall);
        }

        switch (specialAttack.top())
        {
        // 斬撃
        case (int)SpecialAttack::Attack:
        {

            // 技確定
            std::weak_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
            std::weak_ptr<Ui> uiIdSpecialShurashuPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<Ui>();
            std::weak_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();
            std::weak_ptr<TransForm2D> uiIdSpecialShurashuPushTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<TransForm2D>();

            bool drawCheck = false;
            uiIdSpecialShurashu.lock()->SetDrawCheck(drawCheck);
            //drawCheck = true;
            //uiIdSpecialShurashuPush->SetDrawCheck(drawCheck);

            DirectX::XMFLOAT2 pos;
            pos = { uiIdSpecialShurashuTransForm2D.lock()->GetPosition() };
            uiIdSpecialShurashuPushTransForm2D.lock()->SetPosition(pos);

            specialAttack.pop();




            GetStateMachine()->ChangeState(static_cast<int>(Player::State::SpecialAttack));

            // もし地面なら何もしない
            bool noStart = false;
            // エフェクト再生
            areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;

            areWork->Play(position);

            break;
        }
        // 魔法火
        case (int)SpecialAttack::MagicFire:
        {
            // 技確定
            std::weak_ptr<Ui> uiIdSpecialFrame = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<Ui>();
            std::weak_ptr<Ui> uiIdSpecialFramePush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<Ui>();
            std::weak_ptr<TransForm2D> uiIdSpecialFrameTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<TransForm2D>();
            std::weak_ptr<TransForm2D> uiIdSpecialFramePushTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<TransForm2D>();

            bool drawCheck = false;
            uiIdSpecialFrame.lock()->SetDrawCheck(drawCheck);
            //drawCheck = true;
            //uiIdSpecialFramePush->SetDrawCheck(drawCheck);

            DirectX::XMFLOAT2 pos;
            pos = { uiIdSpecialFrameTransForm2D.lock()->GetPosition() };
            uiIdSpecialFramePushTransForm2D.lock()->SetPosition(pos);

            specialAttack.pop();


            GetStateMachine()->ChangeState(static_cast<int>(Player::State::SpecialMagic));

            // もし地面なら何もしない
            bool noStart = false;
            // エフェクト再生
            areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;

            areWork->Play(position);

            break;
        }
        // 魔法氷
        case (int)SpecialAttack::MagicIce:
        {
            // 技確定
            std::weak_ptr<Ui> uiIdSpecialIce = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulIce)->GetComponent<Ui>();
            std::weak_ptr<Ui> uiIdSpecialIceePush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<Ui>();
            std::weak_ptr<TransForm2D> uiIdSpecialIceTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulIce)->GetComponent<TransForm2D>();
            std::weak_ptr<TransForm2D> uiIdSpecialIcePushTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<TransForm2D>();

            bool drawCheck = false;
            uiIdSpecialIce.lock()->SetDrawCheck(drawCheck);
 /*           drawCheck = true;
            uiIdSpecialIceePush->SetDrawCheck(drawCheck);*/

            DirectX::XMFLOAT2 pos;
            pos = { uiIdSpecialIceTransForm2D.lock()->GetPosition() };
            uiIdSpecialIcePushTransForm2D.lock()->SetPosition(pos);


            specialAttack.pop();


            GetStateMachine()->ChangeState(static_cast<int>(Player::State::SpecialMagicIce));

            // もし地面なら何もしない
            bool noStart = false;
            // エフェクト再生
            areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;

            areWork->Play(position);

            break;
        }
        // 魔法雷
        case (int)SpecialAttack::MagicThander:
        {
            // 技確定
            std::weak_ptr<Ui> uiIdSpecialThander = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulThander)->GetComponent<Ui>();
            std::weak_ptr<Ui> uiIdSpecialThanderPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<Ui>();
            std::weak_ptr<TransForm2D> uiIdSpecialThanderTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulThander)->GetComponent<TransForm2D>();
            std::weak_ptr<TransForm2D> uiIdSpecialThanderPushTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<TransForm2D>();

            bool drawCheck = false;
            uiIdSpecialThander.lock()->SetDrawCheck(drawCheck);
            //drawCheck = true;
            //uiIdSpecialThanderPush->SetDrawCheck(drawCheck);

            DirectX::XMFLOAT2 pos;
            pos = { uiIdSpecialThanderTransForm2D.lock()->GetPosition() };
            uiIdSpecialThanderPushTransForm2D.lock()->SetPosition(pos);

            specialAttack.pop();


            GetStateMachine()->ChangeState(static_cast<int>(Player::State::SpecialMagicThander));

            // もし地面なら何もしない
            bool noStart = false;
            // エフェクト再生
            areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;

            areWork->Play(position);

            break;
        }
        default:
        {
            break;
        }
        }
   


        // 一度発動すると初期化
        //if (specialAttack.top() != (int)SpecialAttack::Normal)
        //    specialAttack.pop();
        specialAttackTime = true;

        return true;
    }
    else
    {
        specialAttackTime = false;

        // 技確定
        //std::shared_ptr<Ui> uiIdSpecialShurashuPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<Ui>();

        //bool drawCheck = false;
        //uiIdSpecialShurashuPush->SetDrawCheck(drawCheck);
    }


    // チャージを見やすく
    if (specialAttackCharge >= 0.4f && specialAttackCharge < 0.8f)
    {
        std::weak_ptr<TransForm2D> uiIdSpecialTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::weak_ptr<Ui> uiIdSpecialCharge = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialCharge.lock()->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = { 110,250 };
        uiIdSpecialTransForm2D.lock()->SetPosition(pos);
    }


    // チャージを見やすく
    if (specialAttackCharge >= 0.8f && specialAttackCharge < 1.2f)
    {
        std::weak_ptr<TransForm2D> uiIdSpecialTransForm2DFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::weak_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        std::weak_ptr<TransForm2D> uiIdSpecialTransForm2DSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<TransForm2D>();
        std::weak_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialChargeFurst.lock()->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = { 47,250 };
        uiIdSpecialTransForm2DFurst.lock()->SetPosition(pos);

        uiIdSpecialChargeSecond.lock()->SetDrawCheck(drawCheck);

        pos = { 158,250 };
        uiIdSpecialTransForm2DSecond.lock()->SetPosition(pos);
    }

    // チャージを見やすく
    if (specialAttackCharge >= 1.2f && specialAttackCharge < 1.5f)
    {
        std::weak_ptr<TransForm2D> uiIdSpecialTransForm2DFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::weak_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        std::weak_ptr<TransForm2D> uiIdSpecialTransForm2DSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<TransForm2D>();
        std::weak_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();

        std::weak_ptr<TransForm2D> uiIdSpecialTransForm2DSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<TransForm2D>();
        std::weak_ptr<Ui> uiIdSpecialChargeSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialChargeFurst.lock()->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = { 0,250 };
        uiIdSpecialTransForm2DFurst.lock()->SetPosition(pos);

        uiIdSpecialChargeSecond.lock()->SetDrawCheck(drawCheck);

        pos = { 94,250 };
        uiIdSpecialTransForm2DSecond.lock()->SetPosition(pos);

        uiIdSpecialChargeSerde.lock()->SetDrawCheck(drawCheck);

        pos = { 205,250 };
        uiIdSpecialTransForm2DSerde.lock()->SetPosition(pos);
    }



    //GamePad& gamePad = Input::Instance().GetGamePad();

    //if (specialAttackCharge >= 1.5f)
    //{
    //    std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();
    //    std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();
    //    std::shared_ptr<Ui> uiIdSpecialChargeSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<Ui>();

    //    // 技確定
    //    std::shared_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
    //    std::shared_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();


    //    // 一度発動すると初期化
    //    specialAttackCharge = 0.0f;
    //    specialAttack.push((int)SpecialAttack::Attack);

    //    bool drawCheck = false;
    //    uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

    //    uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

    //    uiIdSpecialChargeSerde->SetDrawCheck(drawCheck);

    //    drawCheck = true;
    //    uiIdSpecialShurashu->SetDrawCheck(drawCheck);
    //    
    //    DirectX::XMFLOAT2 pos;
    //    pos = {94,240 };
    //    float add = 30;
    //    if (2 < (int)specialAttack.size())
    //    pos.y = pos.y - (add * (float)specialAttack.size());
    //    uiIdSpecialShurashuTransForm2D->SetPosition(pos);

    //}
    //// 技を放つ
    //if (gamePad.GetButtonDown() & GamePad::BTN_Y && specialAttack.top() == (int)SpecialAttack::Attack && !specialAttackTime)
    //{
    // 

    //    // 技確定
    //    std::shared_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
    //    std::shared_ptr<Ui> uiIdSpecialShurashuPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<Ui>();
    //    std::shared_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();
    //    std::shared_ptr<TransForm2D> uiIdSpecialShurashuPushTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<TransForm2D>();

    //    bool drawCheck = false;
    //    uiIdSpecialShurashu->SetDrawCheck(drawCheck);
    //    drawCheck = true;
    //    uiIdSpecialShurashuPush->SetDrawCheck(drawCheck);

    //    DirectX::XMFLOAT2 pos;
    //    pos = { uiIdSpecialShurashuTransForm2D->GetPosition() };
    //    //float add = 30;
    //    //if (1 < (int)specialAttack.size())
    //    //    pos.y = pos.y - (add * (float)specialAttack.size());
    //    uiIdSpecialShurashuPushTransForm2D->SetPosition(pos);

    //    // 一度発動すると初期化
    //    if (specialAttack.top() != (int)SpecialAttack::Normal)
    //        specialAttack.pop();
    //    specialAttackTime = true;

    //    return true;
    //}
    //else 
    //{
    //    specialAttackTime = false;

    //    // 技確定
    //    std::shared_ptr<Ui> uiIdSpecialShurashuPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<Ui>();

    //    bool drawCheck = false;
    //    uiIdSpecialShurashuPush->SetDrawCheck(drawCheck);
    //}


    //// チャージを見やすく
    //if (specialAttackCharge >= 0.4f && specialAttackCharge < 0.8f)
    //{
    //    std::shared_ptr<TransForm2D> uiIdSpecialTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
    //    std::shared_ptr<Ui> uiIdSpecialCharge = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

    //    bool drawCheck = true;
    //    uiIdSpecialCharge->SetDrawCheck(drawCheck);

    //    DirectX::XMFLOAT2 pos = {110,250 };
    //    uiIdSpecialTransForm2D->SetPosition(pos);
    //}


    //// チャージを見やすく
    //if (specialAttackCharge >= 0.8f && specialAttackCharge < 1.2f)
    //{
    //    std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
    //    std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

    //    std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<TransForm2D>();
    //    std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();

    //    bool drawCheck = true;
    //    uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

    //    DirectX::XMFLOAT2 pos = { 47,250 };
    //    uiIdSpecialTransForm2DFurst->SetPosition(pos);

    //    uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

    //    pos = { 158,250 };
    //    uiIdSpecialTransForm2DSecond->SetPosition(pos);
    //}

    //// チャージを見やすく
    //if (specialAttackCharge >= 1.2f && specialAttackCharge < 1.5f)
    //{
    //    std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
    //    std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

    //    std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<TransForm2D>();
    //    std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();

    //    std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<TransForm2D>();
    //    std::shared_ptr<Ui> uiIdSpecialChargeSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<Ui>();

    //    bool drawCheck = true;
    //    uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

    //    DirectX::XMFLOAT2 pos = { 0,250 };
    //    uiIdSpecialTransForm2DFurst->SetPosition(pos);

    //    uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

    //    pos = { 94,250 };
    //    uiIdSpecialTransForm2DSecond->SetPosition(pos);

    //    uiIdSpecialChargeSerde->SetDrawCheck(drawCheck);

    //    pos = { 205,250 };
    //    uiIdSpecialTransForm2DSerde->SetPosition(pos);
    //}

    return false;
}

bool Player::InputSpecialShotCharge()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    int uiCount = UiManager::Instance().GetUiesCount();
    // ui無かったら
    if (uiCount <= uiCountMax) return false;

    if (specialShotCharge >= 1.5f)
    {
        std::weak_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdSpecialChargeSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<Ui>();

        // 技確定
        std::weak_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<Ui>();
        std::weak_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<TransForm2D>();


        // 一度発動すると初期化
        specialShotCharge = 0.0f;
        specialAttack.push((int)SpecialAttack::MagicFire);

        bool drawCheck = false;
        uiIdSpecialChargeFurst.lock()->SetDrawCheck(drawCheck);

        uiIdSpecialChargeSecond.lock()->SetDrawCheck(drawCheck);

        uiIdSpecialChargeSerde.lock()->SetDrawCheck(drawCheck);

        drawCheck = true;
        uiIdSpecialShurashu.lock()->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos;
        pos = { 94,240 };
        float add = 30;
        if (2 < (int)specialAttack.size())
            pos.y = pos.y - (add * (float)specialAttack.size());
        uiIdSpecialShurashuTransForm2D.lock()->SetPosition(pos);


    }


    if (gamePad.GetButtonDown() & GamePad::BTN_Y && specialAttack.top() == (int)SpecialAttack::MagicFire && !specialAttackTime)
    {
        // 技確定
        std::weak_ptr<Ui> uiIdSpecialMagic = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdSpecialMagicPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<Ui>();
        std::weak_ptr<TransForm2D> uiIdSpecialMagicTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<TransForm2D>();
        std::weak_ptr<TransForm2D> uiIdSpecialMagicPushTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<TransForm2D>();

        bool drawCheck = false;
        uiIdSpecialMagic.lock()->SetDrawCheck(drawCheck);
        drawCheck = true;
        uiIdSpecialMagicPush.lock()->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos;
        pos = { uiIdSpecialMagicTransForm2D.lock()->GetPosition() };
        //float add = 30;
        //if (1 > (int)specialAttack.size())
        //    pos.y = pos.y - (add * (float)specialAttack.size());
        uiIdSpecialMagicPushTransForm2D.lock()->SetPosition(pos);

        // 一度発動すると初期化
        if (specialAttack.top() != (int)SpecialAttack::Normal)
            specialAttack.pop();
        specialAttackTime = true;
        return true;
    }
    else 
    {
        specialAttackTime = false;

        // 技確定
        std::weak_ptr<Ui> uiIdSpecialMagicPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<Ui>();

        bool drawCheck = false;
        uiIdSpecialMagicPush.lock()->SetDrawCheck(drawCheck);
    }


    // チャージを見やすく
    if (specialShotCharge >= 0.4f && specialShotCharge < 0.8f)
    {
        std::weak_ptr<TransForm2D> uiIdSpecialTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::weak_ptr<Ui> uiIdSpecialCharge = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialCharge.lock()->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = { 110,250 };
        uiIdSpecialTransForm2D.lock()->SetPosition(pos);
    }


    // チャージを見やすく
    if (specialShotCharge >= 0.8f && specialShotCharge < 1.2f)
    {
        std::weak_ptr<TransForm2D> uiIdSpecialTransForm2DFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::weak_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        std::weak_ptr<TransForm2D> uiIdSpecialTransForm2DSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<TransForm2D>();
        std::weak_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialChargeFurst.lock()->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = { 47,250 };
        uiIdSpecialTransForm2DFurst.lock()->SetPosition(pos);

        uiIdSpecialChargeSecond.lock()->SetDrawCheck(drawCheck);

        pos = { 158,250 };
        uiIdSpecialTransForm2DSecond.lock()->SetPosition(pos);
    }

    // チャージを見やすく
    if (specialShotCharge >= 1.2f && specialShotCharge < 1.5f)
    {
        std::weak_ptr<TransForm2D> uiIdSpecialTransForm2DFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::weak_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        std::weak_ptr<TransForm2D> uiIdSpecialTransForm2DSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<TransForm2D>();
        std::weak_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();

        std::weak_ptr<TransForm2D> uiIdSpecialTransForm2DSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<TransForm2D>();
        std::weak_ptr<Ui> uiIdSpecialChargeSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialChargeFurst.lock()->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = { 0,250 };
        uiIdSpecialTransForm2DFurst.lock()->SetPosition(pos);

        uiIdSpecialChargeSecond.lock()->SetDrawCheck(drawCheck);

        pos = { 94,250 };
        uiIdSpecialTransForm2DSecond.lock()->SetPosition(pos);

        uiIdSpecialChargeSerde.lock()->SetDrawCheck(drawCheck);

        pos = { 205,250 };
        uiIdSpecialTransForm2DSerde.lock()->SetPosition(pos);
    }

    return false;
}
// 必殺技演出
void Player::SpecialPlayUlEffect(float elapsedTime)
{

    if (specialAttack.size() <= 0) return;
    switch (specialAttack.top())
    {
    case (int)SpecialAttack::Attack:
    {
        // 近接攻撃
        std::weak_ptr<Ui> uiIdAttackSpecial = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
        std::weak_ptr<TransForm2D> uiIdSpecialAttackTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();
        if (uiIdAttackSpecial.lock()->GetDrawCheck())
        {
            uiIdSpecialAttackTransForm2D.lock()->ShrinkTexture(uiShrinkValueMax, uiShrinkValueMin, elapsedTime);
        }

        break;
    }
    case (int)SpecialAttack::MagicFire:
    {
        // 炎
        std::weak_ptr<Ui> uiIdSpecialFire = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<Ui>();
        std::weak_ptr<TransForm2D> uiIdSpecialFireTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<TransForm2D>();
        if (uiIdSpecialFire.lock()->GetDrawCheck())
        {
            uiIdSpecialFireTransForm2D.lock()->ShrinkTexture(uiShrinkValueMax, uiShrinkValueMin, elapsedTime);
        }
        break;
    }
    case (int)SpecialAttack::MagicIce:
    {
        // 氷
        std::weak_ptr<Ui> uiIdSpecialIce = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulIce)->GetComponent<Ui>();
        std::weak_ptr<TransForm2D> uiIdSpecialIceTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulIce)->GetComponent<TransForm2D>();
        if (uiIdSpecialIce.lock()->GetDrawCheck())
        {
            uiIdSpecialIceTransForm2D.lock()->ShrinkTexture(uiShrinkValueMax, uiShrinkValueMin, elapsedTime);
        }
        break;
    }
    case (int)SpecialAttack::MagicThander:
    {
        // 雷
        std::weak_ptr<Ui> uiIdSpecialThander = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulThander)->GetComponent<Ui>();
        std::weak_ptr<TransForm2D> uiIdSpecialThanderTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulThander)->GetComponent<TransForm2D>();
        if (uiIdSpecialThander.lock()->GetDrawCheck())
        {
            uiIdSpecialThanderTransForm2D.lock()->ShrinkTexture(uiShrinkValueMax, uiShrinkValueMin, elapsedTime);
        }
        break;
    }
    }

}

void Player::ChargeSpecialEnergyMultiple()
{

    int uiCount = UiManager::Instance().GetUiesCount();
    // ui無かったら
    if (uiCount <= uiCountMax) return;

    // 炎魔法を一定以上溜めたら
    if (fireEnergyCharge >= energyChargeMax)
    {
        // 技確定
        std::weak_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
        std::weak_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();


        bool drawCheck = true;
        uiIdSpecialShurashu.lock()->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos;
        pos = { 94,240 };
        float add = 30;
        if (2 < (int)specialAttack.size())
            pos.y = pos.y - (add * (float)specialAttack.size());
        uiIdSpecialShurashuTransForm2D.lock()->SetPosition(pos);
    }

    // 雷魔法を一定以上溜めたら
    if (ThanderEnergyCharge >= energyChargeMax)
    {

    }

    // 氷魔法を一定以上溜めたら
    if (iceEnergyCharge >= energyChargeMax)
    {

    }


    // ここまで通ったら魔法がたまった事が分かるので
    isSpecialChargeComplete = false;

}

void Player::PlayPintchSe()
{
    Audio& Se = Audio::Instance();

    AudioParam audioParam;

    audioParam.filename = "Data/Audio/SE/HP危険.wav";

    audioParam.loop = false;


    audioParam.volume = 3.0f;

    Se.Play(audioParam);
}

void Player::StopPintchSe()
{
    Audio& bgm = Audio::Instance();


    AudioParam audioParam;

    audioParam.filename = "Data/Audio/SE/㏋危険.wav";


    bgm.Stop(audioParam);
}

void Player::PlaySpecialChargeCompleteSe()
{
    Audio& Se = Audio::Instance();

    AudioParam audioParam;

    audioParam.filename = "Data/Audio/SE/必殺技ため.wav";

    audioParam.loop = false;

    audioParam.volume = 3.0f;

    Se.Play(audioParam);
}

DirectX::XMFLOAT3 Player::GetMoveVec(float elapsedTime) const
{
    // 入力情報を取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    // カメラ方向とスティックの入力値によって進行方向を計算する
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    // 移動ベクトルはXZ平面に水平なベクトルになるようにする

    // カメラ右方向ベクトルはＸＺ平面に水平なベクトルに変換
    float cameraRightX =  cameraRight.x;
    float cameraRightZ =  cameraRight.z;
    // y成分を取らずに　矢印の長さを取得
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);

    // 何故Y方向を消してるか　右方向が斜めでも真っ直ぐ進んでほしいYを０
    //　にする少し距離が変わるだから単位ベクトルにする１．０に
    if (cameraRightLength > 0.0f)
    {
        // 単位ベクトル化
        // 右方向の単位ベクトル 
        cameraRightX = cameraRightX/cameraRightLength ;
        cameraRightZ = cameraRightZ / cameraRightLength;
        
    }



    // カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        // 単位ベクトル化
        cameraFrontX = cameraFrontX / cameraFrontLength;
        cameraFrontZ = cameraFrontZ / cameraFrontLength;
    
    }

    // スティックの水平入力値をカメラ右方向に反映し、
    // スティックの垂直入力値をカメラ前方向に反映し、
    // 進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;// 移動方向進むべき方向進行ベクトル
    vec.x = (cameraRightX* ax) + (cameraFrontX * ay);// 右方向
    vec.z = (cameraRightZ *ax) + (cameraFrontZ * ay);// ますっぐ
    // Y軸方向には移動しない
    vec.y = 0.0f;

    if (vec.x != 0 || vec.y != 0 || vec.z != 0)
    {
        movement.lock()->Move(vec,moveSpeed, elapsedTime);
        movement.lock()->Turn( vec ,turnSpeed, elapsedTime);
    }

   

    return vec;

}

DirectX::XMFLOAT3 Player::GetMagicMoveVec(float elapsedTime) const
{
    // 入力情報を取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    // カメラ方向とスティックの入力値によって進行方向を計算する
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    // 移動ベクトルはXZ平面に水平なベクトルになるようにする

    // カメラ右方向ベクトルはＸＺ平面に水平なベクトルに変換
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    // y成分を取らずに　矢印の長さを取得
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);

    // 何故Y方向を消してるか　右方向が斜めでも真っ直ぐ進んでほしいYを０
    //　にする少し距離が変わるだから単位ベクトルにする１．０に
    if (cameraRightLength > 0.0f)
    {
        // 単位ベクトル化
        // 右方向の単位ベクトル 
        cameraRightX = cameraRightX / cameraRightLength;
        cameraRightZ = cameraRightZ / cameraRightLength;

    }



    // カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        // 単位ベクトル化
        cameraFrontX = cameraFrontX / cameraFrontLength;
        cameraFrontZ = cameraFrontZ / cameraFrontLength;

    }

    // スティックの水平入力値をカメラ右方向に反映し、
    // スティックの垂直入力値をカメラ前方向に反映し、
    // 進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;// 移動方向進むべき方向進行ベクトル
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);// 右方向
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);// ますっぐ
    // Y軸方向には移動しない
    vec.y = 0.0f;

    if (vec.x != 0 || vec.y != 0 || vec.z != 0)
    {
        movement.lock()->Move(vec, moveSpeed, elapsedTime);
    }



    return vec;
}


// 弾丸と敵の衝突処理
void Player::CollisionProjectilesVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
   // Actor* enemyManager = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1);
   // EnemyManager& enemyManager = EnemyManager::Instance();
   
    projectileManager = ProjectileManager::Instance();

    // 全ての敵と総当たりで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();
    

    for (int i = 0; i < projectileCount; ++i)
    {

        int enemyCount = enemyManager.GetEnemyCount();
        // 指定のノードと全ての敵を総当たりで衝突処理
        for (int i = 0; i < enemyCount; ++i)
        {
            std::weak_ptr<Actor> enemy = enemyManager.GetEnemy(i);


            DirectX::XMFLOAT3 enemyPosition = enemy.lock()->GetComponent<Transform>()->GetPosition();
            float enemyRudius = enemy.lock()->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy.lock()->GetComponent<Transform>()->GetHeight();



            Model::Node* nodeHeart = enemy.lock()->GetComponent<ModelControll>()->GetModel()->FindNode("body2");
            Model::Node* nodeLeftArm = enemy.lock()->GetComponent<ModelControll>()->GetModel()->FindNode("boss_left_hand2");
            Model::Node* nodeRightArm = enemy.lock()->GetComponent<ModelControll>()->GetModel()->FindNode("boss_right_hand2");
            // 心臓位置
            DirectX::XMFLOAT3 nodeHeartPosition;
            nodeHeartPosition = {
            nodeHeart->worldTransform._41,
            nodeHeart->worldTransform._42,
            nodeHeart->worldTransform._43
            };

            //  左腕位置
            DirectX::XMFLOAT3 nodeLeftArmPosition;
            nodeLeftArmPosition = {
            nodeLeftArm->worldTransform._41,
            nodeLeftArm->worldTransform._42,
            nodeLeftArm->worldTransform._43
            };

            // 右腕位置
            DirectX::XMFLOAT3 nodeRightArmPosition;
            nodeRightArmPosition = {
            nodeRightArm->worldTransform._41,
            nodeRightArm->worldTransform._42,
            nodeRightArm->worldTransform._43
            };
            std::weak_ptr<Actor> projectile = projectileManager.GetProjectile(i);
            DirectX::XMFLOAT3 projectilePosition = projectile.lock()->GetComponent<Transform>()->GetPosition();
            float projectileRadius = projectile.lock()->GetComponent<Transform>()->GetRadius();
            


            //// 衝突処理
            DirectX::XMFLOAT3 outPositon;


            if (Collision::IntersectSphereVsCylinder(
                projectilePosition,
                leftHandRadius,
                {
                enemyPosition.x,
                enemyPosition.y,
                enemyPosition.z
                },
                enemyRudius,
                enemyHeight,
                outPositon) ||
                Collision::IntersectSpherVsSphere(
                    projectilePosition,
                    leftHandRadius,
                    {
                    nodeHeartPosition.x,
                    nodeHeartPosition.y,
                    nodeHeartPosition.z
                    },
                    enemyRudius,
                    outPositon) ||

                Collision::IntersectSphereVsCylinder(
                    projectilePosition,
                    leftHandRadius,
                    {
                    nodeLeftArmPosition.x,
                    nodeLeftArmPosition.y,
                    nodeLeftArmPosition.z
                    },
                    enemyRudius,
                    enemyHeight,
                    outPositon) ||
                Collision::IntersectSphereVsCylinder(
                    projectilePosition,
                    leftHandRadius,
                    {
                      nodeRightArmPosition.x,
                      nodeRightArmPosition.y,
                      nodeRightArmPosition.z
                    },
                    enemyRudius,
                    enemyHeight,
                    outPositon))
            {

                //for (int i = 0; i < projectileCount; ++i)
                //{
                //int enemyCount = EnemyManager::Instance().GetEnemyCount();
                //std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
                //    for (int j = 0; j < enemyCount; ++j)
                //    {
                //        std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(j);
                //        
                //        DirectX::XMFLOAT3 projectilePosition = projectile->GetComponent<Transform>()->GetPosition();
                //        float projectileRadius = projectile->GetComponent<Transform>()->GetRadius();
                //        
                //        DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
                //        float enemyRadius = enemy->GetComponent<Transform>()->GetRadius();
                //        float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();

                //        // 衝突処理
                //        DirectX::XMFLOAT3 outPositon;
                //      
                //        if (!projectile->GetComponent<ProjectileHoming>() && !projectile->GetComponent<ProjectileSunder>())return;

                //        // 円柱と円
                //        if (Collision::IntersectSphereVsCylinder(
                //            projectilePosition,
                //            projectileRadius,
                //            {
                //              enemyPosition.x,
                //              enemyPosition.y + enemyHeight / 2,
                //              enemyPosition.z
                //            } ,
                //            enemyRadius,
                //            enemyHeight / 2,
                //            outPositon))

                //        {
                if (!projectile.lock()->GetComponent<ProjectileHoming>() && !projectile.lock()->GetComponent<ProjectileSunder>())return;
                
                if (projectile.lock()->GetComponent<ProjectileSunder>())
                {
                    ++ThanderEnergyCharge;
                    hitThander->Play(projectilePosition);

                    // 雷ダメージ
                    applyDamageMagic = applyDamageThander;
                }
                else
                {
                    switch (projectile.lock()->GetComponent<ProjectileHoming>()->GetMagicNumber())
                    {
                    case (int)ProjectileHoming::MagicNumber::Fire:
                    {
                        ++fireEnergyCharge;
                        hitFire->Play(projectilePosition);
                        // 炎ダメージ
                        applyDamageMagic = applyDamageFire;
                        break;
                    }
                    case (int)ProjectileHoming::MagicNumber::Ice:
                    {
                        ++iceEnergyCharge;
                        hitIce->Play(projectilePosition);
                        // 氷ダメージ
                        applyDamageMagic = applyDamageIce;
                        break;
                    }
                    }

                }
                hitEffect->Play(projectilePosition);
                
                // ダメージを与える。
                if (enemy.lock()->GetComponent<HP>()->ApplyDamage(applyDamageMagic, 0.5f))
                {

                    if (enemy.lock()->GetComponent<EnemyBoss>()->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Wander &&
                        enemy.lock()->GetComponent<EnemyBoss>()->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Jump && 
                        enemy.lock()->GetComponent<EnemyBoss>()->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::IdleBattle &&
                        enemy.lock()->GetComponent<EnemyBoss>()->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Attack
                        )
                    {
                        // ダメージステートへ
                        enemy.lock()->GetComponent<EnemyBoss>()->GetStateMachine()->ChangeState((int)EnemyBoss::State::Damage);
                    }


                    //// ヒットエフェクト再生
                    //{
                    //    //DirectX::XMFLOAT3 e = enemyPosition;
                    //    //e.y += enemyHeight * 0.5f;
                    //    if (projectile.lock()->GetComponent<ProjectileSunder>())
                    //    {
                    //        ++ThanderEnergyCharge;
                    //        hitThander->Play(projectilePosition);
                    //    }
                    //    else
                    //    {
                    //        switch (projectile.lock()->GetComponent<ProjectileHoming>()->GetMagicNumber())
                    //        {
                    //        case (int)ProjectileHoming::MagicNumber::Fire:
                    //        {
                    //            ++fireEnergyCharge;
                    //            hitFire->Play(projectilePosition);
                    //            break;
                    //        }
                    //        case (int)ProjectileHoming::MagicNumber::Ice:
                    //        {
                    //            ++iceEnergyCharge;
                    //            hitIce->Play(projectilePosition);
                    //            break;
                    //        }
                    //        }
                    //    }

                    //    hitEffect->Play(projectilePosition);


                    //}
                    // 当たった時の副次的効果
                    {
                        specialAttackCharge += 0.2f;
                        //specialShotCharge += 0.1f;
                        //specialShotCharge += 1.5f;
                    }
                    if (projectile.lock()->GetComponent<ProjectileHoming>())
                        // 弾丸破棄
                        projectile.lock()->GetComponent<BulletFiring>()->Destroy();
                }
            }
        }
    }
}

void Player::CollisionRubyVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    projectileManager = ProjectileManager::Instance();

    // 全ての敵と総当たりで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();

    for (int i = 0; i < projectileCount; ++i)
    {
        int enemyCount = EnemyManager::Instance().GetEnemyCount();
        std::weak_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        for (int j = 0; j < enemyCount; ++j)
        {
            std::weak_ptr<Actor> enemy = enemyManager.GetEnemy(j);

            DirectX::XMFLOAT3 projectilePosition = projectile.lock()->GetComponent<Transform>()->GetPosition();
            float projectileRadius = projectile.lock()->GetComponent<Transform>()->GetRadius();

            DirectX::XMFLOAT3 enemyPosition = enemy.lock()->GetComponent<Transform>()->GetPosition();
            float enemyRadius = enemy.lock()->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy.lock()->GetComponent<Transform>()->GetHeight();

            // 衝突処理
            DirectX::XMFLOAT3 outPositon;

            if (!projectile.lock()->GetComponent<ProjectileThrowing>() )return;

            bool counterCheck;
            counterCheck = projectile.lock()->GetComponent<ProjectileThrowing>()->GetCounterCheck();

            // 円柱と円
            if (Collision::IntersectSphereVsCylinder(
                projectilePosition,
                projectileRadius,
                {
                  enemyPosition.x,
                  enemyPosition.y + enemyHeight / 2,
                  enemyPosition.z
                },
                enemyRadius,
                enemyHeight / 2,
                outPositon) && counterCheck)

            {

                // ダメージを与える。
                if (enemy.lock()->GetComponent<HP>()->ApplyDamage(1, 0.5f))
                {
                    // 吹き飛ばす
                    {
                      
                    }
                    // ヒットエフェクト再生
                    {
                        DirectX::XMFLOAT3 e = enemyPosition;
                        e.y += enemyHeight * 0.5f;


                        hitEffect->Play(e);
                    }
                    // 当たった時の副次的効果
                    {
                        specialShotCharge += 0.1f;
                    }
                    // 弾丸破棄
                    projectile.lock()->GetComponent<BulletFiring>()->Destroy();
                    enemy.lock()->GetComponent<EnemyBoss>()->GetStateMachine()->ChangeState
                    ((int)EnemyBoss::State::IdleBattle);
                   
                        
                        
               
                }
            }
        }
    }
}

// プレイヤーとエネミーとの衝突処理
void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    // 全ての敵と総当たりで衝突処理

    int enemyCount = enemyManager.GetEnemyCount();


        for (int i = 0; i < enemyCount; ++i)
        {
            std::weak_ptr<Actor> enemy = enemyManager.GetEnemy(i);


            //// 衝突処理
            DirectX::XMFLOAT3 outPositon;

            DirectX::XMFLOAT3 enemyPosition = enemy.lock()->GetComponent<Transform>()->GetPosition();
            float enemyRadius = enemy.lock()->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy.lock()->GetComponent<Transform>()->GetHeight();


            if (Collision::IntersectCylinderVsCylinder(
                enemyPosition,
                enemyRadius,
                enemyHeight,
                position, radius, height,
                outPositon))

            {
      
      
                    DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
                    DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemyPosition);
                    DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
                    DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
                    DirectX::XMFLOAT3 normal;
                    DirectX::XMStoreFloat3(&normal, N);

                    position = outPositon;
                    //position = {
                    //    position.x * -normal.x * power,
                    //    position.y,
                    //    position.z * -normal.z * power
                    //};

                    transform.lock()->SetPosition(position);

                    //if (normal.y > 0.8f)
                    //{

                    //    //小ジャンプ
                    //    //Jump(jumpSpeed * 0.5f);
                    //   // movement->JumpVelocity(jumpSpeed * 0.5f);


                    //}
                    //else
                    //{
                    //    
                    //    // 押し出し後の位置設定　
                    //    //enemy->GetComponent<Transform>()->SetPosition(outPositon);
                    //    //position = outPositon;
                    //    //position = outPositon;
                    //    const float power = 1.3f;

                    //    position = outPositon;
                    //    //position = {
                    //    //    position.x * -normal.x * power,
                    //    //    position.y,
                    //    //    position.z * -normal.z * power
                    //    //};

                    //    transform->SetPosition(position);
                    //    //DirectX::XMFLOAT3 impulse;
                    //    //impulse.x = normal.x * power;
                    //    //impulse.y = power * normal.y;
                    //    //impulse.z = normal.z * power;
   
                    //    //movement->AddImpulse(impulse);
                    //}
                
            }
            

        }
    
    


}

void Player::CollisionBornVsProjectile(const char* bornname)
{

    // ノード取得
    //Model::Node* nodehand = model->FindNode(bornname);

    EnemyManager& enemyManager = EnemyManager::Instance();

    // 全ての敵と総当たりで衝突処理

    int enemyCount = enemyManager.GetEnemyCount();


    for (int i = 0; i < enemyCount; ++i)
    {
        std::weak_ptr<Actor> enemy = enemyManager.GetEnemy(i);

        // ノード取得
        Model::Node* node = enemy.lock()->GetComponent<ModelControll>()->GetModel()->FindNode(bornname);

        // ノード位置取得
        DirectX::XMFLOAT3 nodePosition;
        nodePosition = {
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        };

        //// 衝突処理
        DirectX::XMFLOAT3 outPositon;

        DirectX::XMFLOAT3 enemyPosition = enemy.lock()->GetComponent<Transform>()->GetPosition();
        float enemyRadius = enemy.lock()->GetComponent<Transform>()->GetRadius();
        float enemyHeight = enemy.lock()->GetComponent<Transform>()->GetHeight();


        if (Collision::IntersectCylinderVsCylinder(
         
            { 
                nodePosition.x, 
                nodePosition.y - 0.1f, 
                nodePosition.z 
            }
            ,
            enemyRadius,
            enemyHeight,
            position, radius, height,
            outPositon))

        {
            

            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemyPosition);
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
            DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
            DirectX::XMFLOAT3 normal;
            DirectX::XMStoreFloat3(&normal, N);

            if (normal.y > 0.8f)
            {

                //小ジャンプ
                //Jump(jumpSpeed * 0.5f);
                //movement->JumpVelocity(jumpSpeed * 0.5f);


            }
            else
            {
                // 押し出し後の位置設定　
                //enemy->GetComponent<Transform>()->SetPosition(outPositon);
                //
                //const float power = 1.3f;

                //float velocityMax = 2;

                //DirectX::XMFLOAT3 impulse;
                //impulse.x = normal.x * power;
                //impulse.y = power * normal.y;
                //impulse.z = normal.z * power;
                //if(movement->GetVelocity().x <= velocityMax)
                //movement->AddImpulse(impulse);
                position = outPositon;
                transform.lock()->SetPosition(position);


            }

        }


    }
}

// ノードと敵の衝突判定
bool Player::CollisionNodeVsEnemies(
    const char* nodeName, float nodeRadius,
    const char* nodeHeartName,
    const char* nodeLeftArmName,
    const char* nodeRightArmName
)
{

    // ノード取得
    Model::Node* node = model->FindNode(nodeName);

    
    //worldTransform
    //localTransform
    // ノード位置取得
    DirectX::XMFLOAT3 nodePosition;
    nodePosition = {
        node->worldTransform._41,
        node->worldTransform._42,
        node->worldTransform._43
    };


    // マネージャー取得
    EnemyManager& enemyManager = EnemyManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();
    // 指定のノードと全ての敵を総当たりで衝突処理
    for (int i = 0; i < enemyCount; ++i)
    {
        std::weak_ptr<Actor> enemy = enemyManager.GetEnemy(i);


        DirectX::XMFLOAT3 enemyPosition = enemy.lock()->GetComponent<Transform>()->GetPosition();
        float enemyRudius = enemy.lock()->GetComponent<Transform>()->GetRadius();
        float enemyHeight = enemy.lock()->GetComponent<Transform>()->GetHeight();



        Model::Node* nodeHeart = enemy.lock()->GetComponent<ModelControll>()->GetModel()->FindNode(nodeHeartName);
        Model::Node* nodeLeftArm = enemy.lock()->GetComponent<ModelControll>()->GetModel()->FindNode(nodeLeftArmName);
        Model::Node* nodeRightArm = enemy.lock()->GetComponent<ModelControll>()->GetModel()->FindNode(nodeRightArmName);
        // 心臓位置
        DirectX::XMFLOAT3 nodeHeartPosition;
        nodeHeartPosition = {
        nodeHeart->worldTransform._41,
        nodeHeart->worldTransform._42,
        nodeHeart->worldTransform._43
        };

        //  左腕位置
        DirectX::XMFLOAT3 nodeLeftArmPosition;
        nodeLeftArmPosition = {
        nodeLeftArm->worldTransform._41,
        nodeLeftArm->worldTransform._42,
        nodeLeftArm->worldTransform._43
        };

        // 右腕位置
        DirectX::XMFLOAT3 nodeRightArmPosition;
        nodeRightArmPosition = {
        nodeRightArm->worldTransform._41,
        nodeRightArm->worldTransform._42,
        nodeRightArm->worldTransform._43
        };


        //// 衝突処理
        DirectX::XMFLOAT3 outPositon;


        if (Collision::IntersectSphereVsCylinder(
            nodePosition,
            leftHandRadius,
            {
            enemyPosition.x,
            enemyPosition.y,
            enemyPosition.z
            },
            enemyRudius,
            enemyHeight,
            outPositon) ||
            Collision::IntersectSpherVsSphere(
                nodePosition,
                leftHandRadius,
                {
                nodeHeartPosition.x,
                nodeHeartPosition.y,
                nodeHeartPosition.z
                },
                enemyRudius,
                outPositon) ||

            Collision::IntersectSphereVsCylinder(
                nodePosition,
                leftHandRadius,
                {
                nodeLeftArmPosition.x,
                nodeLeftArmPosition.y,
                nodeLeftArmPosition.z
                },
                enemyRudius,
                enemyHeight,
                outPositon) ||
            Collision::IntersectSphereVsCylinder(
                nodePosition,
                leftHandRadius,
                {
                  nodeRightArmPosition.x,
                  nodeRightArmPosition.y,
                  nodeRightArmPosition.z
                },
                enemyRudius,
                enemyHeight,
                outPositon))
        {
            if (enemy.lock()->GetComponent<HP>()->ApplyDamage(applyDamageNormal, 0.5f))
            {
                // 斬撃音
                InputAttackSlashSE();
                hitSlash->Play(nodePosition, slashScale);

                    if (enemy.lock()->GetComponent<EnemyBoss>()->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Wander &&
                        enemy.lock()->GetComponent<EnemyBoss>()->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Jump&&
                        enemy.lock()->GetComponent<EnemyBoss>()->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::IdleBattle
                        )
                    {
                        // ダメージステートへ
                        //enemy->GetComponent<EnemyBoss>()->GetStateMachine()->ChangeState((int)EnemyBoss::State::Damage);

                        // 再生ループ
                        //bool  loop = false;

                        //// 再生開始時間 
                        //float currentAnimationStartSeconds = 1.0f;

                        //// 再生時間加算分の値
                        //float currentAnimationAddSeconds = 0.00f;

                        //// キーフレームの終了
                        //float keyFrameEnd = 153.0f;

                        //// アニメーションブレンド
                        //float blendSeconds = 0.35f;

                        if (enemy.lock()->GetComponent<EnemyBoss>()->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Attack)
                        {

                            Model::ModelAnim modelAnim;

                            modelAnim.index = EnemyBoss::Animation::Anim_Movie;

                            modelAnim.currentanimationseconds = 1.0f;

                            modelAnim.keyFrameEnd = 153.0f;

                            // 通常
                            enemy.lock()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(modelAnim);


                        }


                        // 死んだとき
                        if (enemy.lock()->GetComponent<EnemyBoss>()->GetStateMachine()->GetStateIndex() == (int)EnemyBoss::State::IdleBattle)
                        {

                            //// 再生開始時間 
                            //currentAnimationStartSeconds = 0.3f;


                            //// キーフレームの終了
                            //keyFrameEnd = 55.0f;
                            
                            // model情報
                            Model::ModelAnim modelAnim;


                            modelAnim.index = EnemyBoss::Animation::Anim_Die;

                            modelAnim.currentanimationseconds = 0.3f;

                            modelAnim.keyFrameEnd = 55.0f;

                            enemy.lock()->GetComponent<ModelControll>()->GetModel()->PlayAnimation(modelAnim);
                        }

                        // 混乱状態
                        if (attackNumberSave >= attackNumberSaveMax && attackNumberSave <= attackNumberSaveMax &&
                            enemy.lock()->GetComponent<EnemyBoss>()->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::IdleBattle)
                        {
                            enemy.lock()->GetComponent<EnemyBoss>()->GetStateMachine()->ChangeState(
                                (int)EnemyBoss::State::IdleBattle);

                            // 攻撃連続ヒット停止
                            attackNumberSave = 0;
                        }



   
        
                    }

                    // 当たった時の副次的効果
                    specialAttackCharge += 0.1f;

                    // 斬撃チャージ
                    ++attackEnergyCharge;

                    // 攻撃ヒット回数
                    ++attackNumberSave;

             



                    return true;
          
            }
        }
    }
    return false;

        //// 下半身
        //// 円柱と円
        //if (Collision::IntersectSphereVsCylinder(
        //    nodePosition,
        //    leftHandRadius, 
        //    {
        //    enemyPosition.x,
        //    enemyPosition.y,
        //    enemyPosition.z 
        //    },
        //    enemyRudius,
        //    enemyHeight,
        //    outPositon))

        //{

        //    // ダメージを与える。
        //    //enemy->ApplyDamage(1);
        //// ダメージが通ったら消える。TRUEになるから
        //    if (enemy->GetComponent<HP>()->ApplyDamage(applyDamageNormal, 0.5f))
        //    {

     
        //        // ヒットエフェクト再生
        //        {
      

        //            hitEffect->Play(nodePosition);

        //            // ダメージステートへ
        //            enemy->GetComponent<EnemyBoss>()->GetStateMachine()->ChangeState((int)EnemyBoss::State::Damage);


        //        }
        //        // 当たった時の副次的効果
        //        {
        //            specialAttackCharge += 0.1f;
        //        }
        //   
        //    }
        //}
     
       // // 胸
       // // 円柱と円
       // if (Collision::IntersectSpherVsSphere(
       //     nodePosition,
       //     leftHandRadius,
       //     {
       //     nodeHeartPosition.x,
       //     nodeHeartPosition.y,
       //     nodeHeartPosition.z
       //     },
       //     enemyRudius,
       //     outPositon))

       // {

       //     // ダメージを与える。
       //     //enemy->ApplyDamage(1);
       // // ダメージが通ったら消える。TRUEになるから
       //     if (enemy->GetComponent<HP>()->ApplyDamage(applyDamageNormal, 0.5f))
       //     {


       //         // ヒットエフェクト再生
       //         {


       //             hitEffect->Play(nodePosition);
       //             // ダメージステートへ
       //             enemy->GetComponent<EnemyBoss>()->GetStateMachine()->ChangeState((int)EnemyBoss::State::Damage);

       //         }
       //         // 当たった時の副次的効果
       //         {
       //             specialAttackCharge += 0.1f;
       //         }
       //      
       //     }
       // }
       // // 左腕
       // // 円柱と円
       // if (Collision::IntersectSphereVsCylinder(
       //     nodePosition,
       //     leftHandRadius,
       //     {
       //     nodeLeftArmPosition.x,
       //     nodeLeftArmPosition.y,
       //     nodeLeftArmPosition.z
       //     },
       //     enemyRudius,
       //     enemyHeight,
       //     outPositon))

       // {

       //     // ダメージを与える。
       //     //enemy->ApplyDamage(1);
       // // ダメージが通ったら消える。TRUEになるから
       //     if (enemy->GetComponent<HP>()->ApplyDamage(applyDamageNormal, 0.5f))
       //     {


       //         // ヒットエフェクト再生
       //         {



       //             hitEffect->Play(nodePosition);
       //             // ダメージステートへ
       //             enemy->GetComponent<EnemyBoss>()->GetStateMachine()->ChangeState((int)EnemyBoss::State::Damage);

       //         }
       //         // 当たった時の副次的効果
       //         {
       //             specialAttackCharge += 0.1f;
       //         }
       //         
       //     }
       // }

       // // 右腕
       //// 円柱と円
       // if (Collision::IntersectSphereVsCylinder(
       //     nodePosition,
       //     leftHandRadius,
       //     {
       //       nodeRightArmPosition.x,
       //       nodeRightArmPosition.y,
       //       nodeRightArmPosition.z
       //     },
       //     enemyRudius,
       //     enemyHeight,
       //     outPositon))

       // {

       //     // ダメージを与える。
       //     //enemy->ApplyDamage(1);
       // // ダメージが通ったら消える。TRUEになるから
       //     if (enemy->GetComponent<HP>()->ApplyDamage(applyDamageNormal, 0.5f))
       //     {


       //         // ヒットエフェクト再生
       //         {


       //             hitEffect->Play(nodePosition);
       //             // ダメージステートへ
       //             enemy->GetComponent<EnemyBoss>()->GetStateMachine()->ChangeState((int)EnemyBoss::State::Damage);

       //         }
       //         // 当たった時の副次的効果
       //         {
       //             specialAttackCharge += 0.1f;
       //         }
       //         
       //     }
       // }
        //// ダメージ確認
        //if (enemy->GetComponent<HP>()->InvincibleTimerCheck())
        //{
        //    hitMortion = 0;
        //}
        //if (hitMortion < hitMortionMax)
        //{
        //    if (enemy->GetComponent<HP>()->DamageDrawCheck())
        //    {
        //        bool modelDrawCheck = false;
        //        enemy->GetComponent<EnemyBoss>()->SetModelDrawCheck(modelDrawCheck);
        //    }
        //    else
        //    {
        //        bool modelDrawCheck = true;
        //        enemy->GetComponent<EnemyBoss>()->SetModelDrawCheck(modelDrawCheck);
        //    }

        //}
        //else
        //{
        //    bool modelDrawCheck = true;
        //    enemy->GetComponent<EnemyBoss>()->SetModelDrawCheck(modelDrawCheck);
        //}

    }


 


void Player::CollisionNodeVsEnemiesCounter(const char* nodeName, float nodeRadius)
{
    // ノード取得
    Model::Node* node = model->FindNode(nodeName);
    //worldTransform
    //localTransform
    // ノード位置取得
    DirectX::XMFLOAT3 nodePosition;
    nodePosition = {
        node->worldTransform._41,
        node->worldTransform._42,
        node->worldTransform._43
    };
    // マネージャー取得
    EnemyManager& enemyManager = EnemyManager::Instance();
    ProjectileManager& projectileManager = ProjectileManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();
    int projectileCount = projectileManager.GetProjectileCount();
    // 指定のノードと全ての敵を総当たりで衝突処理
    for (int i = 0; i < projectileCount; ++i)
    {
        for (int j = 0; j < enemyCount; ++j)
        {
            std::weak_ptr<Actor> enemy = enemyManager.GetEnemy(j);
            std::weak_ptr<Actor> projectille = projectileManager.GetProjectile(i);

            //enemy->GetComponent<ModelControll>()->GetModel()->FindNode

            if (!projectille.lock()->GetComponent<ProjectileImpact>()) return;

            DirectX::XMFLOAT3 projectilePosition = projectille.lock()->GetComponent<Transform>()->GetPosition();
            float projectileInRudius = projectille.lock()->GetComponent<ProjectileImpact>()->GetRadiusInSide();
            float projectileOutRudius = projectille.lock()->GetComponent<ProjectileImpact>()->GetRadiusOutSide();
            float projectileHeight = projectille.lock()->GetComponent<Transform>()->GetHeight();


            /*DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRudius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();*/


            //// 衝突処理
            DirectX::XMFLOAT3 outPositon;

            std::weak_ptr<EnemyBoss> enemyBoss = enemy.lock()->GetComponent<EnemyBoss>();


            // 球と球
            if (Collision::IntersectSpherVsSphere(
                nodePosition,
                nodeRadius,
                projectilePosition,
                projectileOutRudius,
                outPositon) && 
                !Collision::IntersectSpherVsSphere(
                    nodePosition,
                    nodeRadius,
                    projectilePosition,
                    projectileInRudius,
                    outPositon) &&projectille.lock()->GetComponent<ProjectileImpact>()) /*&&
                EnemyBoss->GetCounterJudgment())*/
            {
                // 高さが一定以下なら通る
                if (projectilePosition.y + projectileHeight < nodePosition.y) return;


                //EnemyBoss->GetStateMachine()->ChangeState((int)EnemyBoss::State::Idle);
                // ヒットエフェクト再生
                {
                    //DirectX::XMFLOAT3 e = enemyPosition;
                    //e.y += enemyHeight * 0.5f;

                    projectille.lock()->GetComponent<ProjectileImpact>()->Destoroy();

                    ImpactEffect->Play(nodePosition);

                    // ヒット時少しスローに
                    hitCheck = true;

                    //desEffect->Play(e);

                    //EnemyBoss->SetCounterJudgment(false);
                }


            }
        }


    }
}

void Player::CollisionNodeVsRubyCounter(const char* nodeName, float nodeRadius)
{
    // ノード取得
    Model::Node* node = model->FindNode(nodeName);

    //worldTransform
    //localTransform
    // ノード位置取得
    DirectX::XMFLOAT3 nodePosition;
    nodePosition = {
        node->worldTransform._41,
        node->worldTransform._42,
        node->worldTransform._43
    };
    // マネージャー取得
    EnemyManager& enemyManager = EnemyManager::Instance();
    ProjectileManager& projectileManager = ProjectileManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();
    int projectileCount = projectileManager.GetProjectileCount();
    // 指定のノードと全ての敵を総当たりで衝突処理
    for (int i = 0; i < projectileCount; ++i)
    {
        for (int j = 0; j < enemyCount; ++j)
        {
            std::weak_ptr<Actor> enemy = enemyManager.GetEnemy(i);
            std::weak_ptr<Actor> projectille = projectileManager.GetProjectile(i);

            //enemy->GetComponent<ModelControll>()->GetModel()->FindNode

            if (!projectille.lock()->GetComponent<ProjectileRuby>()) return;
           


            DirectX::XMFLOAT3 projectilePosition = projectille.lock()->GetComponent<Transform>()->GetPosition();
            float projectileRudius = projectille.lock()->GetComponent<Transform>()->GetRadius();
            float projectileHeight = projectille.lock()->GetComponent<Transform>()->GetHeight();


            /*DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRudius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();*/


            //// 衝突処理
            DirectX::XMFLOAT3 outPositon;

            std::weak_ptr<EnemyBoss> enemyBoss = enemy.lock()->GetComponent<EnemyBoss>();


            // 球と球
            if (Collision::IntersectSpherVsSphere(
                nodePosition,
                nodeRadius,
                projectilePosition,
                projectileRudius,
                outPositon)) /*&&
                EnemyBoss->GetCounterJudgment())*/
            {
                // 高さが一定以下なら通る
                if (projectilePosition.y + projectileHeight < nodePosition.y) return;


                //EnemyBoss->GetStateMachine()->ChangeState((int)EnemyBoss::State::Idle);
                // ヒットエフェクト再生
                {
                    //DirectX::XMFLOAT3 e = enemyPosition;
                    //e.y += enemyHeight * 0.5f;

                    projectille.lock()->GetComponent<BulletFiring>()->Destroy();
                    

                    ImpactEffect->Play(nodePosition);

                    // ヒット時少しスローに
                    hitCheck = true;

                    //desEffect->Play(e);

                    //EnemyBoss->SetCounterJudgment(false);
                }


            }
        }


    }
}

void Player::CollisionNodeVsRubyCounterBulletFring(const char* nodeName, float nodeRadius)
{
    // ノード取得
    Model::Node* node = model->FindNode(nodeName);
    //worldTransform
    //localTransform
    // ノード位置取得
    DirectX::XMFLOAT3 nodePosition;
    nodePosition = {
        node->worldTransform._41,
        node->worldTransform._42,
        node->worldTransform._43
    };
    // マネージャー取得
    EnemyManager& enemyManager = EnemyManager::Instance();
    ProjectileManager& projectileManager = ProjectileManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();
    int projectileCount = projectileManager.GetProjectileCount();
    // 指定のノードと全ての敵を総当たりで衝突処理
    for (int j = 0; j < enemyCount; ++j)
    {
            std::weak_ptr<Actor> enemy = enemyManager.GetEnemy(j);
    for (int i = 0; i < projectileCount; ++i)
    {
      
            std::weak_ptr<Actor> projectille = projectileManager.GetProjectile(i);

            if (!projectille.lock()->GetComponent<ProjectileThrowing>()) return;

            bool counterCheck = projectille.lock()->GetComponent<ProjectileThrowing>()->GetCounterCheck();


            DirectX::XMFLOAT3 projectilePosition = projectille.lock()->GetComponent<Transform>()->GetPosition();
            float projectileRudius = projectille.lock()->GetComponent<Transform>()->GetRadius();
            float projectileHeight = projectille.lock()->GetComponent<Transform>()->GetHeight();


            /*DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRudius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();*/


            //// 衝突処理
            DirectX::XMFLOAT3 outPositon;

            std::weak_ptr<EnemyBoss> enemyBoss = enemy.lock()->GetComponent<EnemyBoss>();


            // 球と球
            if (Collision::IntersectSpherVsSphere(
                nodePosition,
                nodeRadius,
                projectilePosition,
                projectileRudius,
                outPositon) && !counterCheck) /*&&
                enemyBoss->GetCounterJudgment())*/
            {
                // 高さが一定以下なら通る
               /* if (projectilePosition.y + projectileHeight < nodePosition.y) return;*/


                //EnemyBoss->GetStateMachine()->ChangeState((int)EnemyBoss::State::Idle);
                // ヒットエフェクト再生
                {
                    //DirectX::XMFLOAT3 e = enemyPosition;
                    //e.y += enemyHeight * 0.5f;

                    //projectille->GetComponent<BulletFiring>()->Destroy();
                    projectille.lock()->GetComponent<ProjectileThrowing>()->SetTarget(
                        {
                            enemy.lock()->GetComponent<Transform>()->GetPosition().x,
                            enemy.lock()->GetComponent<Transform>()->GetPosition().y + enemy.lock()->GetComponent<Transform>()->GetHeight(),
                            enemy.lock()->GetComponent<Transform>()->GetPosition().z,
                        }
                    );

                    DirectX::XMFLOAT3 direction = GetForwerd(angle);

                    float lifeTimerCounter = 5.0f;
                    projectille.lock()->GetComponent<BulletFiring>()->Lanch(direction, projectilePosition, lifeTimerCounter);

                    ImpactEffect->Play(nodePosition);

                    counterCheck = true;
                    projectille.lock()->GetComponent<ProjectileThrowing>()->SetCounterCheck(counterCheck);

                    // ヒット時少しスローに
                    hitCheck = true;

                    //desEffect->Play(e);

                    //enemySlime->SetCounterJudgment(false);
                }


            }
        }


    }
}



// デバッグ用GUI描画
void Player::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
    // beginからendまでの内容が出来る
    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        // トランスフォーム
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // 位置　// 数値をいじる
            ImGui::InputFloat3("Position", &position.x);

            //ImGui::SliderFloat3("Velocity", &velocity.x,5,0);

            //ImGui::InputInt("jamplimit", &jumpCount);

            //ImGui::InputInt("hp", &health);

            ImGui::SliderFloat("frontVec", &frontVec.x,-0.37f,1.0);
            ImGui::SliderFloat("frontVecY", &frontVec.y,-0.37f,1.0);
            //ImGui::SliderFloat("frontVecX", &frontVec.y,0.0f,1.5);
        // 回転
        DirectX::XMFLOAT3 a;
        // XMConvertToDegrees普通の数字を何度にする
        a.x = DirectX::XMConvertToDegrees(angle.x);
        a.y = DirectX::XMConvertToDegrees(angle.y);
        a.z = DirectX::XMConvertToDegrees(angle.z);
        ImGui::InputFloat3("Angle", &a.x);
        angle.x = DirectX::XMConvertToRadians(a.x);
        angle.y = DirectX::XMConvertToRadians(a.y);
        angle.z = DirectX::XMConvertToRadians(a.z);
        // スケール
        //ImGui::InputFloat3("Scale", &scale.x);

       

    }
    }
    ImGui::End();
}

void Player::Destroy()
{
    PlayerManager::Instance().Remove(GetActor());
}

void Player::UpdateSwordeTraile()
{
    // 剣の原点から根本と先端までのオフセット値
    DirectX::XMVECTOR RootOffset = DirectX::XMVectorSet(0, 0, 0.5f, 0);
    DirectX::XMVECTOR TipOffset = DirectX::XMVectorSet(0, 0, 2.3f, 0);


    //Model::Node* SwordeRootName = model->FindNode("Maria_sword");
    Model::Node* SwordeRootName = model->FindNode("mixamorig:RightHand");
    //Model::Node* SwordeTipName = model->FindNode("mixamorig:RightHand");

    /*DirectX::XMFLOAT3 dir = GetForwerd(angle);*/

        // 剣の手元
    DirectX::XMFLOAT3 swordeRootPosition;
    //SwordeRootPosition = model->ConvertLocalToWorld(SwordeRootName);
    swordeRootPosition = model->ConvertLocalToWorld(SwordeRootName);

    // 前
    DirectX::XMFLOAT3 dir;
    dir.x = sinf(swordeRootPosition.y);// 三角を斜めにして位置を変えた
    dir.y = cosf(swordeRootPosition.x);
    dir.z = cosf(swordeRootPosition.y);

    DirectX::XMVECTOR dirVec = DirectX::XMLoadFloat3(&dir);

   


    DirectX::XMMATRIX swordeRootPositionVec = DirectX::XMLoadFloat4x4(&SwordeRootName->worldTransform);


    //SwordeRootPosition.x = (SwordeRootPosition.x * dir.x) * 1.3f;

   // DirectX::XMStoreFloat3(&SwordeRootPosition, DirectX::XMVector3Transform(RootOffset, swordeRootPositionVec));

    // 剣先
    DirectX::XMFLOAT3 swordeTipPosition = swordeRootPosition;
    //DirectX::XMVECTOR swordeTipPositionVec;
    //swordeTipPositionVec = DirectX::XMLoadFloat3(&swordeRootPosition);
    ////dirVec = DirectX::XMVector3Normalize(dirVec);
    //dirVec = DirectX::XMVectorScale(dirVec,1.5f);
    ////swordeTipPositionVec = DirectX::XMVectorMultiply(swordeTipPositionVec, dirVec);
    //swordeTipPositionVec = DirectX::XMVectorMultiply(swordeTipPositionVec , dirVec);
    //
    //DirectX::XMStoreFloat3(&swordeTipPosition, swordeTipPositionVec);


    swordeTipPosition.y *= (dir.y * 0.01f);

    //swordeTipPosition.x = swordeTipPosition.x * 1.3f;
    //swordeTipPosition.z = swordeTipPosition.z * 1.3f;
    //SwordeTipPosition = SwordeRootPosition;

    //DirectX::XMMATRIX swordeTipPositionVec = DirectX::XMLoadFloat4x4(&SwordeRootName->worldTransform);
    //DirectX::XMStoreFloat3(&SwordeTipPosition,DirectX::XMVector3Transform(TipOffset, swordeRootPositionVec));




   // DirectX::XMFLOAT3 positionSwordeTraile = position;

    //positionSwordeTraile.y += 1.0f;
    trailPositions[0][0] = swordeRootPosition;
    trailPositions[1][0] = swordeTipPosition;

    // ポリゴン作成
    PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();

    DirectX::XMFLOAT4 color = { 0,0,1,1 };
    //primitiveRenderer->AddVertex(position, color);

    //primitiveRenderer->AddVertex(positionSwordeTraile, color);

    // 保存していた頂点バッファでポリゴンを作る
    for (int i = 0; i < MAX_POLYGON; ++i)
    {
        primitiveRenderer->AddVertex(trailPositions[0][i], color);
        primitiveRenderer->AddVertex(trailPositions[1][i], color);
    }

    // 一つ後ろにずらす
    for (int i = MAX_POLYGON - 1; i > -1; --i)
    {
        trailPositions[0][i] = trailPositions[0][i - 1];
        trailPositions[1][i] = trailPositions[1][i - 1];
    }


}

void Player::PinchMode(float elapsedTime)
{
    if (hp.lock()->HealthPinch() && !hp.lock()->GetDead())
    {

        if (UpdateElapsedTime(timeElapsedHintMax, elapsedTime) && 
            !InputShortCutkeyMagic())
        {
            // Se再生
            PlayPintchSe();

            hintDrawCheck = hintDrawCheck ? false : true;

            UiManager::Instance().GetUies((int)UiManager::UiCount::PushShort)->
                GetComponent<Ui>()->SetDrawCheck(hintDrawCheck);
        }
        if (InputShortCutkeyMagic())
        {
            hintDrawCheck = true;

            UiManager::Instance().GetUies((int)UiManager::UiCount::PushShort)->
                GetComponent<Ui>()->SetDrawCheck(hintDrawCheck);
        }
            
        UiManager::Instance().GetUies((int)UiManager::UiCount::Push2)->
            GetComponent<Ui>()->SetDrawCheck(true);

        UiManager::Instance().GetUies((int)UiManager::UiCount::ShortCut)->
            GetComponent<Ui>()->SetDrawCheck(true);


        PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();

        vignetteData.color = { 1,0,0,1 };
        vignetteData.smoothness = 3.0f;
        vignetteData.intensity = 0.8;

        postprocessingRenderer.SetVignetteMaxData(vignetteData);
        postprocessingRenderer.SetVignetteDataColor(vignetteData.color);

        colorGradingData.brigthness = 1;

        postprocessingRenderer.SetColorGradingMinData(colorGradingData);

        // ピンチかどうか
        isPintch = true;
    }
    else
    {
        // ピンチかどうか
        isPintch = false;

        hintDrawCheck = false;

        UiManager::Instance().GetUies((int)UiManager::UiCount::PushShort)->
            GetComponent<Ui>()->SetDrawCheck(hintDrawCheck);

        UiManager::Instance().GetUies((int)UiManager::UiCount::Push2)->
            GetComponent<Ui>()->SetDrawCheck(hintDrawCheck);

        UiManager::Instance().GetUies((int)UiManager::UiCount::ShortCut)->
            GetComponent<Ui>()->SetDrawCheck(hintDrawCheck);

        PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();

        vignetteData.smoothness = 0.0f;
        vignetteData.intensity = 0.0;
        vignetteData.color = { 0,0,0,1 };

        postprocessingRenderer.SetVignetteData(vignetteData);


    }


    if (mp.lock()->GetMpEmpth())
    {
        DirectX::XMFLOAT2 pos = { 179, 640 };

        std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutKeule)->GetComponent<TransForm2D>();

        uiIdAttackCheckPos.lock()->SetPosition(pos);

        uiIdAttackCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandHeale)->GetComponent<TransForm2D>();

        pos = { 200, 525 };

        uiIdAttackCheckPos.lock()->SetPosition(pos);

        return;
    }


    // ピンチの時にヒントを出す
    if (InputShortCutkeyMagic() &&
        isPintch)
    {



        std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutKeule)->GetComponent<TransForm2D>();

        uiIdAttackCheckPos.lock()->Shake();

        //Graphics& graphics = Graphics::Instance();
        //Camera& camera = Camera::Instance();

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

        //std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
        //    (int)UiManager::UiCount::Push)->GetComponent<TransForm2D>();

        //// スクリーン座標位置
        //DirectX::XMVECTOR screenPositionV = DirectX::XMLoadFloat2(&uiIdAttackCheckPos.lock()->GetPosition());


        //// ワールド座標
        //DirectX::XMVECTOR WorldPositionV = DirectX::XMVector3Unproject(
        //    screenPositionV,
        //    viewport.TopLeftX,
        //    viewport.TopLeftY,
        //    viewport.Width,
        //    viewport.Height,
        //    viewport.MinDepth,
        //    viewport.MaxDepth,
        //    View,
        //    Projection,
        //    World);



        //DirectX::XMFLOAT3 pos;

        //DirectX::XMStoreFloat3(&pos, WorldPositionV);

        //effectFocus2D->Play(pos);
    }
    if (magicAction &&
        isPintch)
    {
        std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandHeale)->GetComponent<TransForm2D>();

        uiIdAttackCheckPos.lock()->Shake();
    }




}

//// 着地した時に呼ばれる
//void Player::OnLanding()
//{
//    // 着地したからリセット
//    jumpCount = 0;
//
//
//
//    //// 下方向の速力が一定以上なら着地ステートへ  十分な速度で落とす重力の５倍２、３秒後に着地モーションをする。
//    if (velocity.y < gravity * 5.0f)
//    {
//        if (state != State::Damage && state != State::Death)
//        {
//            // 着地ステートへ遷移
//            TransitionLandState();
//        }
//    }
//}
//
//void Player::OnDead()
//{
//
//
//    // 死亡ステート遷移
//    TransitionDeathState();
//}
//
//void Player::OnDamaged()
//{
//    
//        // ダメージステートへ遷移
//    TransitionDamageState();
//}

bool Player::InputJump()
{
    // ボタンで入力でジャンプ（ジャンプ回数制限つき）
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        // 値を小さくする
        if (movement.lock()->GetJumpCount() < jumpLimit) {
           
            
            movement.lock()->JumpVelocity(jumpSpeed);

            // ジャンプ入力した
           return true;
        }
    }

    return false;
}

bool Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();




    // 直進弾丸発射　cボタンを押したら
    if (gamePad.GetButtonDown() & GamePad::BTN_B)
    {
        // 前方向 sinの計算 角度の計算
        DirectX::XMFLOAT3 dir;
        dir = GetForwerd(angle);
        
        // 発射位置（プレイヤーの腰当たり)
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height*0.5f;// 身長÷位置のｙ
        pos.z = position.z;
        // 発射　ストレート弾丸を用意
        {
            // 弾丸初期化
            const char* filename = "Data/Model/Sword/Sword.mdl";

            std::weak_ptr<Actor> actor = ActorManager::Instance().Create();
            actor.lock()->AddComponent<ModelControll>();
            actor.lock()->GetComponent<ModelControll>()->LoadModel(filename);
            actor.lock()->SetName("ProjectileStraight");
            actor.lock()->AddComponent<Transform>();
            actor.lock()->GetComponent<Transform>()->SetPosition(position);
            actor.lock()->GetComponent<Transform>()->SetAngle(angle);
            actor.lock()->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f));
            actor.lock()->AddComponent<BulletFiring>();
            actor.lock()->AddComponent<ProjectileStraight>();
            //actor->AddComponent<Collision>();
            ProjectileManager::Instance().Register(actor.lock());
            //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
            //std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

            // これが２Dかの確認
            bool check2d = false;
            actor.lock()->SetCheck2d(check2d);

            // 飛ぶ時間
            float   lifeTimer = 3.0f;
            // 発射
            actor.lock()->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        }

        //projectile->Lanch(dir, pos);
        return true;
    }


    // 追尾弾丸発射
    if (gamePad.GetButtonDown() & GamePad::BTN_Y && specialAttackTime)
    {
        // 前方向 sinの計算
        DirectX::XMFLOAT3 dir;
        //dir.x = sinf(angle.y);// 三角を斜めにして位置を変えた
        //dir.y = 0;
        //dir.z = cosf(angle.y);

        dir = GetForwerd(angle);

        //sinf0度０　cosf0は１度
        //９０sin1,cos0返ってくる横
        //４５sin0.5,cos0.5斜め
        // 360度を上手く表現出来る。2dでも行ける。


        // 発射位置（プレイヤーの腰当たり)
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;// 身長÷位置のｙ
        pos.z = position.z;
        //ターゲット（デフォルトではプレイヤーの前方）
        DirectX::XMFLOAT3 target;
        // 敵がいなかった時のために　1000先まで飛んでくれ
        target.x = pos.x+dir.x * 1000.0f;
        target.y = pos.y+dir.y * 1000.0f;
        target.z = pos.z+dir.z * 1000.0f;

        // 一番近くの敵をターゲットにする
        float dist = FLT_MAX;// float の最大値float全体
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)//float 最大値ないにいる敵に向かう
        {
            // 敵との距離判定  敵の数も計測 全ての敵をてに入れる
            std::weak_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // 敵の位置
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy.lock()->GetComponent<Transform>()->GetPosition());
            // 自分から敵までの位置を計測
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            // ベクトルのながさを２乗する。√つけていない奴
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                // 距離が敵のものを入れる少なくする３０なら３０、１００なら１００入れる
                dist = d;
                target = enemy.lock()->GetComponent<Transform>()->GetPosition();// 位置を入れる
                target.y += enemy.lock()->GetComponent<Transform>()->GetHeight() * 0.5f;// 位置に身長分
            }
            
            

        }


        // 発射　ストレート弾丸を用意
        //ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
        //projectile->Lanch(dir, pos,target);

        // 弾丸初期化
        const char* filename = "Data/Model/Sword/Sword.mdl";

        std::weak_ptr<Actor> actor = ActorManager::Instance().Create();
        actor.lock()->AddComponent<ModelControll>();
        actor.lock()->GetComponent<ModelControll>()->LoadModel(filename);
        actor.lock()->SetName("ProjectileHoming");
        actor.lock()->AddComponent<Transform>();
        actor.lock()->GetComponent<Transform>()->SetPosition(position);
        actor.lock()->GetComponent<Transform>()->SetAngle(angle);
        actor.lock()->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(3.0f,3.0f,3.0f));
        actor.lock()->AddComponent<BulletFiring>();
        actor.lock()->AddComponent<ProjectileHoming>();

        // これが２Dかの確認
        bool check2d = false;
        actor.lock()->SetCheck2d(check2d);

        //actor->AddComponent<Collision>();
        ProjectileManager::Instance().Register(actor.lock());
        //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        std::weak_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

        // 飛ぶ時間
        float   lifeTimer = 3.0f;
        // 発射
        projectile.lock()->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        projectile.lock()->GetComponent<ProjectileHoming>()->SetTarget(target);

        //// 特殊攻撃の発生条件
        //specialAttackTime = false;
        return true;

    }
    return false;
}
// 回避入力orカウンター
bool Player::InputAvoidance()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    // 直進弾丸発射　rボタンを押したら
    //if (gamePad.GetButtonDown() & GamePad::BTN_B)
    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {

        return true;
    }


    return false;
}
// 攻撃入力
bool Player::InputAttack()
{
    // 攻撃入力処理

    GamePad& gamePad = Input::Instance().GetGamePad();

    // 直進弾丸発射　ｃボタンを押したら
    //if (gamePad.GetButtonDown() & GamePad::BTN_B)
    if (gamePad.GetButtonDown() & GamePad::BTN_B)
    {
  
        return true;
    }

    return false;
}

bool Player::InputMagick()
{

    GamePad& gamePad = Input::Instance().GetGamePad();


    if (!InputShortCutkeyMagic() && 
        gamePad.GetButtonDown() & GamePad::BTN_B) return true;

    switch (selectMagicCheck)
    {
    case (int)CommandMagic::Fire:
    {
        // 魔法
        if (gamePad.GetButtonDown() & GamePad::BTN_B)
        {

            return true;
        }
        break;
    }
    case (int)CommandMagic::Thander:
    {
        // 魔法
        if (gamePad.GetButtonDown() & GamePad::BTN_X)
        {

            return true;
        }
        break;
    }
    case (int)CommandMagic::Ice:
    {

        // 魔法
        if (gamePad.GetButtonDown() & GamePad::BTN_A)
        {

            return true;
        }
        break;
    }
    case (int)CommandMagic::Heale:
    {

        // 魔法
        if (gamePad.GetButtonDown() & GamePad::BTN_Y)
        {

            return true;
        }
        break;
    }


    default:
        break;
    }


    return false;
}

// メニュー開くボタン
bool Player::InputMenue()
{

    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_START)
    {
        return true;
    }
    return false;
}

// 待機ステートへ遷移
void Player::TransitionIdleState()
{
    state = State::Idle;

    afterimagemove = false;

    updateanim = UpAnim::Normal;

    // 待機アニメーション再生
    //model->PlayAnimation(Anim_Idle, true);

    
}

// 待機ステート更新処理
void Player::UpdateIdleState(float elapsedTime)
{
    // 移動入力処理
    if (InputMove(elapsedTime))
    {
       
        TransitionMoveState();
    }

    // ジャンプ入力処理
    if (InputJump())
    {
        
        TransitionJumpState();
    }


    InputSelectCheck();

    if (InputAttack() && selectCheck == (int)CommandAttack::Attack)
    {
        //stated = state;
        TransitionAttackState();
    }





    // 弾丸入力処理
    else if (InputMagicframe() && selectCheck == (int)CommandAttack::Magic && selectMagicCheck == (int)CommandMagic::Fire)
    {
        InputMagicframe();
        //InputProjectile();
        //TransitionAttackState();
    }

    // 特殊攻撃
    if (InputSpecialAttackCharge())
    {
        TransitionAttackState();
    }
    // 特殊魔法
    if (InputSpecialShotCharge())
    {
        InputProjectile();
    }
    //currentANimationSeconds = model->GetCurrentANimationSeconds();

    // 移動入力処理
    //InputMove(elapsedTime);


}

void Player::TransitionMoveState()
{

    afterimagemove = true;

    updateanim = UpAnim::Doble;

    //// 上半身
    //bornUpStartPoint = "mixamorig:Spine";
    //// 下半身
    //bornDownerEndPoint = "mixamorig:Spine";


    // 走りアニメーション再生
    //model->PlayAnimation(Anim_Running, true);

    //model->PlayUpeerBodyAnimation(Anim_Running, true);
    
    state = State::Move;
}

void Player::UpdateMoveState(float elapsedTime)
{
    // 位置情報を入れる。
    //position = GetActor()->GetPosition();


    // 移動入力処理
    if (!InputMove(elapsedTime))
    {
        
        TransitionIdleState();
    }

 

    // ジャンプ入力処理
    if (InputJump())
    {
        
        TransitionJumpState();
    }

    InputSelectCheck();

    if (InputAttack() && selectCheck == (int)CommandAttack::Attack)
    {
        //stated = state;
        TransitionAttackState();
    }



    // 弾丸入力処理
    else if (selectCheck == (int)CommandAttack::Magic)
    {
        InputProjectile();
    }

    // 特殊攻撃
    if (InputSpecialAttackCharge())
    {
        TransitionAttackState();
    }
    // 特殊魔法
    if (InputSpecialShotCharge())
    {
        InputProjectile();
    }
    // 落下着地
    Ground();

    // 残像姿勢用
    //currentANimationSeconds = model->GetCurrentANimationSeconds();



}




void Player::TransitionJumpState()
{

    state = State::Jump;

    // 上半身
    //bornUpStartPoint = "mixamorig:Spine";

    //model->PlayUpeerBodyAnimation(Anim_Jump, false);
    //// ジャンプアニメーション再生
    //model->PlayAnimation(Anim_Jump,false);

}

void Player::UpdateJumpState(float elapsedTime)
{

    //InputMove(elapsedTime);


    // ジャンプ入力処理
    if (InputJump()&&movement.lock()->GetJumpCount() >= 1)
    {
        TransitionJumpFlipState();
    }

    // 弾丸入力処理
    if (InputProjectile())
    {
        TransitionAttackState();
    }


    // 落下着地
    Ground();

    //if (movement->GetStepOffSet())
    //{
    //    TransitionLandState();
    //   
    //}

    


    //currentANimationSeconds = model->GetCurrentANimationSeconds();

}

void Player::TransitionLandState()
{
    state = State::Land;

    updateanim = UpAnim::Normal;
    // 着地アニメーション再生
    //model->PlayAnimation(Anim_Landing, false);

    movement.lock()->SetOnLadius(false);

    //movement->SetStepOffSet(false);
    
}

void Player::UpdateLandState(float elapsedTime)
{
   

    // もし終わったら待機に変更
    if (!model->IsPlayAnimation())
    {
        movement.lock()->SetOnLadius(false);
        TransitionIdleState();
    }

    //currentANimationSeconds = model->GetCurrentANimationSeconds();
 
}





void Player::TransitionJumpFlipState()
{


    state = State::JumpFlip;

    // 走りアニメーション再生
    //model->PlayAnimation(Anim_Jump_Flip, false);
    //model->PlayUpeerBodyAnimation(Anim_Jump_Flip, false);
}

void Player::UpdatejumpFlipState(float elapsedTime)
{


    if (InputMove(elapsedTime))
        bool afterimagemove = true;
    // ジャンプ入力処理
    if (!model->IsPlayAnimation())
    {
        TransitionJumpState();
    }

    //currentANimationSeconds = model->GetCurrentANimationSeconds();
}

void Player::TransitionAttackState()
{
    //if (updateanim == UpAnim::Doble)
    //{
    //    state = State::Attack;
    //    //updateanim = UpAnim::Doble;
    //    //上半身
    //    bornUpStartPoint = "mixamorig:Spine2";

    //   /* bornUpEndPoint = "mixamorig:Neck";*/
    //    // 下半身
    //    bornDownerEndPoint = "mixamorig:Spine";

    //    model->PlayUpeerBodyAnimation(Anim_Attack, false);
    //}
    //else
    //{
    //    state = State::Attack;

    //    updateanim = UpAnim::Normal;
    //    // 走りアニメーション再生
    //    model->PlayAnimation(Anim_Attack, false);
    //}

    //state = State::Attack;

    updateanim = UpAnim::Normal;
    // 走りアニメーション再生
    //model->PlayAnimation(Anim_Attack, false);
}

void Player::UpdateAttackState(float elapsedTime)
{

    // もし終わったら待機に変更
    if (updateanim == UpAnim::Doble && !model->IsPlayUpeerBodyAnimation())
    {
        attackCollisionFlag = false;
        TransitionMoveState();

       
    }
    else if (updateanim == UpAnim::Normal && !model->IsPlayAnimation())
    {
        attackCollisionFlag = false;
        TransitionMoveState();
    }
    if (updateanim == UpAnim::Doble && !InputMove(elapsedTime))
    {
        updateanim = UpAnim::Doble;
        //上半身
        //bornUpStartPoint = "mixamorig:Spine";
        //// 下半身
        //bornDownerEndPoint = "mixamorig:Spine";
       // model->PlayAnimation(Anim_Attack, false);
    }

        // 任意のアニメーション再生区間でのみ衝突判定処理をする
        float animationTime = model->GetCurrentANimationSeconds();
        // 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。
        attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
        if (updateanim == UpAnim::Doble)
        {
            // 任意のアニメーション再生区間でのみ衝突判定処理をする
            float animationTime = model->GetCurrentAnimationSecondsUpeer();
            // 上手く行けば敵が回避行動を取ってくれる行動を用意出来る。
            attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
        }
    if (attackCollisionFlag)
    {
        // 左手ノードとエネミーの衝突処理
        //CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    }

    Ground();
    
}

void Player::UpdateProjectile(float elapsedTime)
{
    DirectX::XMFLOAT3 target;
    projectileManager = ProjectileManager::Instance();

    // 全ての敵と総当たりで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();

    for (int i = 0; i < projectileCount; ++i)
    {
  
        std::weak_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        float dist = FLT_MAX;// float の最大値float全体
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)//float 最大値ないにいる敵に向かう
        {
            // 敵との距離判定  敵の数も計測 全ての敵をてに入れる
            std::weak_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);



            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // 敵の位置
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy.lock()->GetComponent<Transform>()->GetPosition());
            // 自分から敵までの位置を計測
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            // ベクトルのながさを２乗する。√つけていない奴
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                // 距離が敵のものを入れる少なくする３０なら３０、１００なら１００入れる
                dist = d;
                target = enemy.lock()->GetComponent<Transform>()->GetPosition();// 位置を入れる
                target.y += enemy.lock()->GetComponent<Transform>()->GetHeight() * 0.5f;// 位置に身長分
                //if (projectile->GetModel())
                //    projectile->GetComponent<ProjectileHoming>()->SetTarget(target);
            }


        }
    }

}

void Player::TransitionDamageState()
{
    state = State::Damage;

    // ダメージアニメーション再生
    //model->PlayAnimation(Anim_GetHit1, false);
}

void Player::UpdateDamageState(float elapsedTime)
{
    // ダメージアニメーション終わったら待機ステートへ
    if (!model->IsPlayAnimation())
    {

            TransitionIdleState();

    }
}

void Player::TransitionDeathState()
{
    state = State::Death;

    // ダメージアニメーション再生
    //model->PlayAnimation(Anim_Death, false);
}

void Player::UpdateDeathState(float elapsedTime)
{   
    
    if (!model->IsPlayAnimation())
    {

       // ボタンを押したら復活ステートへ遷移
        GamePad& gamePad = Input::Instance().GetGamePad();
        if (gamePad.GetButtonDown() & GamePad::BTN_A)
        {
            TransitionReviveState();
        }


    }

}
// 復活ステート遷移
void Player::TransitionReviveState()
{
    state = State::Revive;

    // 体力回復
    health = maxHealth;


    // ダメージアニメーション再生
    //model->PlayAnimation(Anim_Revive, false);
}

void Player::UpdateReviveState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();

    }
}

bool Player::InputMagicframe()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    //if (gamePad.GetButtonDown() & GamePad::BTN_B&& magicAction && !gamePad.GetButtonDownCountinue() && !mp->GetMpEmpth())
    //if ( !mp->GetMpEmpth())
    //{
        // mp消費
        mp.lock()->ApplyConsumption(magicConsumption);

        // 前方向 sinの計算
        DirectX::XMFLOAT3 dir;

        dir = GetForwerd(angle);

        //sinf0度０　cosf0は１度
        //９０sin1,cos0返ってくる横
        //４５sin0.5,cos0.5斜め
        // 360度を上手く表現出来る。2dでも行ける。


        // 発射位置（プレイヤーの腰当たり)
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;// 身長÷位置のｙ
        pos.z = position.z;
        //ターゲット（デフォルトではプレイヤーの前方）
        DirectX::XMFLOAT3 target;
        // 敵がいなかった時のために　1000先まで飛んでくれ
        target.x = pos.x + dir.x * 1000.0f;
        target.y = pos.y + dir.y * 1000.0f;
        target.z = pos.z + dir.z * 1000.0f;

        // 一番近くの敵をターゲットにする
        float dist = FLT_MAX;// float の最大値float全体
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)//float 最大値ないにいる敵に向かう
        {
            // 敵との距離判定  敵の数も計測 全ての敵をてに入れる
            std::weak_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // 敵の位置
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy.lock()->GetComponent<Transform>()->GetPosition());
            // 自分から敵までの位置を計測
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            // ベクトルのながさを２乗する。√つけていない奴
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                // 距離が敵のものを入れる少なくする３０なら３０、１００なら１００入れる
                dist = d;
                target = enemy.lock()->GetComponent<Transform>()->GetPosition();// 位置を入れる
                target.y += enemy.lock()->GetComponent<Transform>()->GetHeight() * 0.5f;// 位置に身長分
            }



        }

        // 弾丸初期化
        const char* filename = "Data/Model/Sword/Sword.mdl";

        std::weak_ptr<Actor> actor = ActorManager::Instance().Create();
        actor.lock()->AddComponent<ModelControll>();
        actor.lock()->GetComponent<ModelControll>()->LoadModel(filename);
        actor.lock()->SetName("ProjectileHoming");
        actor.lock()->AddComponent<Transform>();
        actor.lock()->GetComponent<Transform>()->SetPosition(position);
        actor.lock()->GetComponent<Transform>()->SetAngle(angle);
        actor.lock()->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
        actor.lock()->AddComponent<BulletFiring>();
        actor.lock()->AddComponent<ProjectileHoming>();
        const char* effectFilename = "Data/Effect/fire.efk";
        actor.lock()->GetComponent<ProjectileHoming>()->SetEffectProgress(effectFilename);
        int magicNumber = (int)ProjectileHoming::MagicNumber::Fire;
        actor.lock()->GetComponent<ProjectileHoming>()->SetMagicNumber(magicNumber);
        //actor->GetComponent<ProjectileHoming>()->EffectProgressPlay();
        // これが２Dかの確認
        bool check2d = false;
        actor.lock()->SetCheck2d(check2d);

        //actor->AddComponent<Collision>();
        ProjectileManager::Instance().Register(actor.lock());
        //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        std::weak_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

        // 飛ぶ時間
        float   lifeTimer = 3.0f;
        // 発射
        projectile.lock()->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        projectile.lock()->GetComponent<ProjectileHoming>()->SetTarget(target);

        // 攻撃解除
        //magicAction = false;
        //selectMagicCheck = (int)CommandMagic::Normal;
     
        return true;
    //}
    //else if (gamePad.GetButtonUp() & GamePad::BTN_B)
    //{
    //    gamePad.SetButtonDownCountinue(false);
    //}
    //return false;
}

bool Player::InputMagicIce()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    //if (gamePad.GetButtonDown() & GamePad::BTN_B && magicAction && !gamePad.GetButtonDownCountinue() && !mp->GetMpEmpth())
        //if ( !mp->GetMpEmpth())
        //{
        // mp消費
        mp.lock()->ApplyConsumption(magicConsumption);
        // 前方向 sinの計算
        DirectX::XMFLOAT3 dir;

        dir = GetForwerd(angle);

        //sinf0度０　cosf0は１度
        //９０sin1,cos0返ってくる横
        //４５sin0.5,cos0.5斜め
        // 360度を上手く表現出来る。2dでも行ける。


        // 発射位置（プレイヤーの腰当たり)
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;// 身長÷位置のｙ
        pos.z = position.z;
        //ターゲット（デフォルトではプレイヤーの前方）
        DirectX::XMFLOAT3 target;
        // 敵がいなかった時のために　1000先まで飛んでくれ
        target.x = pos.x + dir.x * 1000.0f;
        target.y = pos.y + dir.y * 1000.0f;
        target.z = pos.z + dir.z * 1000.0f;

        // 一番近くの敵をターゲットにする
        float dist = FLT_MAX;// float の最大値float全体
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)//float 最大値ないにいる敵に向かう
        {
            // 敵との距離判定  敵の数も計測 全ての敵をてに入れる
            std::weak_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // 敵の位置
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy.lock()->GetComponent<Transform>()->GetPosition());
            // 自分から敵までの位置を計測
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            // ベクトルのながさを２乗する。√つけていない奴
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                // 距離が敵のものを入れる少なくする３０なら３０、１００なら１００入れる
                dist = d;
                target = enemy.lock()->GetComponent<Transform>()->GetPosition();// 位置を入れる
                target.y += enemy.lock()->GetComponent<Transform>()->GetHeight() * 0.5f;// 位置に身長分
            }



        }

        // 弾丸初期化
        const char* filename = "Data/Model/Sword/Sword.mdl";

        std::weak_ptr<Actor> actor = ActorManager::Instance().Create();
        actor.lock()->AddComponent<ModelControll>();
        actor.lock()->GetComponent<ModelControll>()->LoadModel(filename);
        actor.lock()->SetName("ProjectileHoming");
        actor.lock()->AddComponent<Transform>();
        actor.lock()->GetComponent<Transform>()->SetPosition(position);
        actor.lock()->GetComponent<Transform>()->SetAngle(angle);
        actor.lock()->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
        actor.lock()->AddComponent<BulletFiring>();
        actor.lock()->AddComponent<ProjectileHoming>();
        const char* effectFilename = "Data/Effect/brezerd.efk";
        actor.lock()->GetComponent<ProjectileHoming>()->SetEffectProgress(effectFilename);
        //actor->GetComponent<ProjectileHoming>()->EffectProgressPlay();

        int magicNumber = (int)ProjectileHoming::MagicNumber::Ice;
        actor.lock()->GetComponent<ProjectileHoming>()->SetMagicNumber(magicNumber);

        // これが２Dかの確認
        bool check2d = false;
        actor.lock()->SetCheck2d(check2d);

        //actor->AddComponent<Collision>();
        ProjectileManager::Instance().Register(actor.lock());
        //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        std::weak_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);
        
        // 飛ぶ時間
        float   lifeTimer = 4.0f;
        // 発射
        projectile.lock()->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        projectile.lock()->GetComponent<ProjectileHoming>()->SetTarget(target);

        // 攻撃解除
        //magicAction = false;
        //selectMagicCheck = (int)CommandMagic::Normal;

        return true;
    //}
    //else if (gamePad.GetButtonUp() & GamePad::BTN_B)
    //{
    //    gamePad.SetButtonDownCountinue(false);
    //}
    //return false;
}

bool Player::InputMagicLightning()
{
   // GamePad& gamePad = Input::Instance().GetGamePad();
   // if (gamePad.GetButtonDown() & GamePad::BTN_B && magicAction && !gamePad.GetButtonDownCountinue() && !mp->GetMpEmpth())
    //if (!mp->GetMpEmpth())
    //{
        // mp消費
        mp.lock()->ApplyConsumption(magicConsumption);
        // 前方向 sinの計算
        DirectX::XMFLOAT3 dir;

        dir = GetForwerd(angle);

        //sinf0度０　cosf0は１度
        //９０sin1,cos0返ってくる横
        //４５sin0.5,cos0.5斜め
        // 360度を上手く表現出来る。2dでも行ける。


        // 発射位置（プレイヤーの腰当たり)
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;// 身長÷位置のｙ
        pos.z = position.z;
        //ターゲット（デフォルトではプレイヤーの前方）
        DirectX::XMFLOAT3 target;
        // 敵がいなかった時のために　1000先まで飛んでくれ
        target.x = pos.x + dir.x * 1000.0f;
        target.y = pos.y + dir.y * 1000.0f;
        target.z = pos.z + dir.z * 1000.0f;

        // 一番近くの敵をターゲットにする
        float dist = FLT_MAX;// float の最大値float全体
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)//float 最大値ないにいる敵に向かう
        {
            // 敵との距離判定  敵の数も計測 全ての敵をてに入れる
            std::weak_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // 敵の位置
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy.lock()->GetComponent<Transform>()->GetPosition());
            // 自分から敵までの位置を計測
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            // ベクトルのながさを２乗する。√つけていない奴
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                // 距離が敵のものを入れる少なくする３０なら３０、１００なら１００入れる
                dist = d;
                target = enemy.lock()->GetComponent<Transform>()->GetPosition();// 位置を入れる
                target.y += enemy.lock()->GetComponent<Transform>()->GetHeight() * 0.5f;// 位置に身長分
            }



        }

        // 弾丸初期化
        const char* filename = "Data/Model/Sword/Sword.mdl";

        std::weak_ptr<Actor> actor = ActorManager::Instance().Create();
        actor.lock()->AddComponent<ModelControll>();
        actor.lock()->GetComponent<ModelControll>()->LoadModel(filename);
        actor.lock()->SetName("ProjectileSunder");
        actor.lock()->AddComponent<Transform>();
        actor.lock()->GetComponent<Transform>()->SetPosition(pos);
        actor.lock()->GetComponent<Transform>()->SetAngle(angle);
        actor.lock()->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
        actor.lock()->AddComponent<BulletFiring>();
        actor.lock()->AddComponent<ProjectileSunder>();
        const char* effectFilename = "Data/Effect/lightningStrike.efk";
        actor.lock()->GetComponent<ProjectileSunder>()->SetEffectProgress(effectFilename);
        //actor->GetComponent<ProjectileHoming>()->EffectProgressPlay();


        // これが２Dかの確認
        bool check2d = false;
        actor.lock()->SetCheck2d(check2d);

        //actor->AddComponent<Collision>();
        ProjectileManager::Instance().Register(actor.lock());
        //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        std::weak_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

        float   lifeTimer = 0.5f;

        // 発射
        projectile.lock()->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        projectile.lock()->GetComponent<ProjectileSunder>()->SetTarget(target);

        // 攻撃解除
        //magicAction = false;
        //selectMagicCheck = (int)CommandMagic::Normal;

        return true;
    //}
    //else if (gamePad.GetButtonUp() & GamePad::BTN_B)
    //{
    //    gamePad.SetButtonDownCountinue(false);
    //}
    //return false;
}
// 回復魔法開始
bool Player::InputMagicHealing()
{

    // mp消費
    mp.lock()->ApplyConsumption(mp.lock()->GetMaxMagic());
    
    hp.lock()->AddHealth(healing);


    return true;
}

void Player::InputSpecialMagicframe()
{
    // mp消費
    //mp->ApplyConsumption(magicConsumption);
    // 前方向 sinの計算
    DirectX::XMFLOAT3 dir;

    dir = GetForwerd(angle);


    DirectX::XMFLOAT3 target;

    // 一番近くの敵をターゲットにする
    float dist = FLT_MAX;// float の最大値float全体
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)//float 最大値ないにいる敵に向かう
    {
        // 敵との距離判定  敵の数も計測 全ての敵をてに入れる
        std::weak_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);


        target = enemy.lock()->GetComponent<Transform>()->GetPosition();// 位置を入れる


    }

    // 弾丸初期化
    const char* filename = "Data/Model/Sword/Sword.mdl";

    std::weak_ptr<Actor> actor = ActorManager::Instance().Create();
    actor.lock()->AddComponent<ModelControll>();
    actor.lock()->GetComponent<ModelControll>()->LoadModel(filename);
    actor.lock()->SetName("MagicFrameTornade");
    actor.lock()->AddComponent<Transform>();
    actor.lock()->GetComponent<Transform>()->SetPosition(target);
    actor.lock()->GetComponent<Transform>()->SetAngle(angle);
    actor.lock()->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
    actor.lock()->AddComponent<BulletFiring>();
    actor.lock()->AddComponent<ProjectileTornade>();
    const char* effectFilename = "Data/Effect/fireTornade.efk";
    actor.lock()->GetComponent<ProjectileTornade>()->SetEffectProgress(effectFilename);
    //actor->GetComponent<ProjectileHoming>()->EffectProgressPlay();


    // これが２Dかの確認
    bool check2d = false;
    actor.lock()->SetCheck2d(check2d);

    //actor->AddComponent<Collision>();
    ProjectileManager::Instance().Register(actor.lock());
    //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
    std::weak_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

    float   lifeTimer = 0.5f;

    // 発射
    projectile.lock()->GetComponent<BulletFiring>()->Lanch(dir, target, lifeTimer);
    projectile.lock()->GetComponent<ProjectileTornade>()->SetTarget(target);

    //// 攻撃解除
    //magicAction = false;
    //selectMagicCheck = (int)CommandMagic::Normal;
}

void Player::AttackCheckUI()
{
    // UIカーソル
    //std::shared_ptr<Ui> uiSight = UiManager::Instance().GetUies((int)UiManager::UiCount::Sight)->GetComponent<Ui>();
    
    int uiCount = UiManager::Instance().GetUiesCount();
    // ui無かったら
    if (uiCount <= uiCountMax || !rockCheck || specialRockOff) return;

    EnemyManager& enemyManager = EnemyManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();
    switch (selectCheck)
    {
    case (int)CommandAttack::Attack:
    {
        for (int i = 0; i < enemyCount; ++i)
        {
            std::weak_ptr<Actor> enemy = enemyManager.GetEnemy(i);
            ////// 衝突処理
            //DirectX::XMFLOAT3 outPositon;

            //DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            //float enemyRadius = enemy->GetComponent<Transform>()->GetRadius();


            DirectX::XMFLOAT3 enemyPosition = enemy.lock()->GetComponent<Transform>()->GetPosition();
            float enemyRudius = enemy.lock()->GetComponent<Transform>()->GetRadius() + 1;
            float enemyHeight = enemy.lock()->GetComponent<Transform>()->GetHeight();

            //// 衝突処理
            DirectX::XMFLOAT3 outPositon;

            // 円柱と円
            if (Collision::IntersectSphereVsCylinder(
                position,
                radius,
                {
                enemyPosition.x,
                enemyPosition.y,
                enemyPosition.z
                },
                enemyRudius,
                enemyHeight,
                outPositon))

            {
                std::weak_ptr<Ui> uiSightAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<Ui>();

                bool drawCheck = true;
                uiSightAttackCheck.lock()->SetDrawCheck(drawCheck);
            }
            else
            {
                std::weak_ptr<Ui> uiSightAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<Ui>();

                bool drawCheck = false;
                uiSightAttackCheck.lock()->SetDrawCheck(drawCheck);
            }


            //if (Collision::IntersectSpherVsSphere(
            //    position, radius,
            //    enemyPosition,
            //    enemyRadius,
            //    outPositon))

            //{
            //    std::shared_ptr<Ui> uiSightAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<Ui>();

            //    bool drawCheck = true;
            //    uiSightAttackCheck->SetDrawCheck(drawCheck);
            //}
            //else
            //{
            //    std::shared_ptr<Ui> uiSightAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<Ui>();

            //    bool drawCheck = false;
            //    uiSightAttackCheck->SetDrawCheck(drawCheck);
            //}

        }


        break;
    }
    case (int)CommandAttack::Magic:
    {
        for (int i = 0; i < enemyCount; ++i)
        {
            std::weak_ptr<Actor> enemy = enemyManager.GetEnemy(i);


            DirectX::XMVECTOR playerPosition =
                DirectX::XMLoadFloat3(&position);
            DirectX::XMFLOAT3 enemyPosition = enemy.lock()->GetComponent<Transform>()->GetPosition();

            DirectX::XMVECTOR enemyPositionXM =
                DirectX::XMLoadFloat3(&enemyPosition);

            DirectX::XMVECTOR LengthSq =
                DirectX::XMVectorSubtract(playerPosition,enemyPositionXM);
            LengthSq = DirectX::XMVector3LengthSq(LengthSq);
            
            float lengthSq;
            DirectX::XMStoreFloat(&lengthSq, LengthSq);
           

            if (lengthSq < magicRangeLength)
            {
                std::weak_ptr<Ui> uiSightAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<Ui>();

                bool drawCheck = true;
                uiSightAttackCheck.lock()->SetDrawCheck(drawCheck);
            }
            else
            {
                std::weak_ptr<Ui> uiSightAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<Ui>();

                bool drawCheck = false;
                uiSightAttackCheck.lock()->SetDrawCheck(drawCheck);
            }
        }
        break;
    }
    default:
        break;
    }
   

    //    DirectX::XMFLOAT3 eposition;
    //    eposition = enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition();

    //    DirectX::XMFLOAT3 dir =
    //    {
    //    eposition.x - position.x,
    //    eposition.y - position.y,
    //    eposition.z - position.z
    //    };

    //    DirectX::XMVECTOR Length;
    //    float length;

    //    DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&dir);
    //    Length = DirectX::XMVector3LengthSq(Direction);
    //    DirectX::XMStoreFloat(&length, Length);

    //    length

    //}

}

//bool Player::InputMagic()
//{
//    GamePad& gamePad = Input::Instance().GetGamePad();
//    if (gamePad.GetButtonDown() & GamePad::BTN_B)
//    {
//
//        return true;
//    }
//    return false;
//}

//void Player::UpdateTransform()
//{
//    // 位置情報
//    position = GetActor()->GetPosition();
//
//    // 向き情報
//    inFloat3({
//        GetActor()->GetRotation().x,
//        GetActor()->GetRotation().y,
//        GetActor()->GetRotation().z
//        }, angle);
//    
//}

void Player::inFloat3(DirectX::XMFLOAT3 value, DirectX::XMFLOAT3 &inValue)
{
   inValue =
    {
         value.x,
         value.y,
         value.z
    };

    
}

DirectX::XMFLOAT3 Player::GetForwerd(DirectX::XMFLOAT3 angle)
{
    DirectX::XMFLOAT3 dir;
    dir.x = sinf(angle.y);// 三角を斜めにして位置を変えた
    dir.y = 0;
    dir.z = cosf(angle.y);

    return dir;
}

// 自分の当たり判定有無
void Player::DmageInvalidJudment(bool invalidJudgment)
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)//float 最大値ないにいる敵に向かう
    {
        // 敵との距離判定  敵の数も計測 全ての敵をてに入れる
        std::weak_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);

        enemy.lock()->GetComponent<EnemyBoss>()->SetInvalidJudgment(invalidJudgment);
    }
}

void Player::UiControlle(float elapsedTime)
{
    int uiCount = UiManager::Instance().GetUiesCount();
    // ui無かったら
    if (uiCount <= uiCountMax) return;

    float gaugeWidth = hp.lock()->GetMaxHealth() * hp.lock()->GetHealth() * 0.1f;
    // hpゲージ
    std::weak_ptr<TransForm2D> uiHp = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerHPBar)->GetComponent<TransForm2D>();
    std::weak_ptr<TransForm2D> uiHpBar = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerHp)->GetComponent<TransForm2D>();
    DirectX::XMFLOAT2 scale = { gaugeWidth, uiHp.lock()->GetScale().y };


    uiHp.lock()->SetScale(scale);

    gaugeWidth = mp.lock()->GetMaxMagic() * mp.lock()->GetMagic() * 0.1f;
    // mpゲージ
    std::weak_ptr<TransForm2D> uiMp = UiManager::Instance().GetUies((int)UiManager::UiCount::Mp)->GetComponent<TransForm2D>();
    std::weak_ptr<Ui> uiColor = UiManager::Instance().GetUies((int)UiManager::UiCount::Mp)->GetComponent<Ui>();
    scale = { gaugeWidth, uiMp.lock()->GetScale().y };

    uiMp.lock()->SetScale(scale);
    // mp色
   mpUiColor = { 1,1,1,1 };
    if (mp.lock()->GetMpEmpth())
    {
        mpUiColor = { 1,0,0,1 };
    }
   uiColor.lock()->SetColor(mpUiColor);



    // 揺れ
    if (shakeMode)
    {
        
        uiHp.lock()->Shake();
        uiHpBar.lock()->Shake();
        

        

    }
    //　初期化
    if (uiHp.lock()->GetShakeEnd())
    {
        float positionStandardBar = 593;
        float positionStandard = 421;
        shakeMode = false;
        uiHp.lock()->SetShakeTime(0);
        uiHpBar.lock()->SetShakeTime(0);
        uiHp.lock()->SetPosition({ uiHp.lock()->GetPosition().x ,positionStandardBar });
        uiHpBar.lock()->SetPosition({ uiHpBar.lock()->GetPosition().x ,positionStandard });
    
    }
}

void Player::MagicPointUpdate()
{
    
    
}

void Player::SpecialApplyDamageInRadius()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    
    int enemyCount = enemyManager.GetEnemyCount();
    // 指定のノードと全ての敵を総当たりで衝突処理
    for (int i = 0; i < enemyCount; ++i)
    {
        std::weak_ptr<Actor> enemy = enemyManager.GetEnemy(i);


        DirectX::XMFLOAT3 enemyPosition = enemy.lock()->GetComponent<Transform>()->GetPosition();
        float enemyRudius = enemy.lock()->GetComponent<Transform>()->GetRadius();
        float enemyHeight = enemy.lock()->GetComponent<Transform>()->GetHeight();
        ProjectileManager& projectileManager = ProjectileManager::Instance();
        for (int ii = 0; ii < projectileManager.GetProjectileCount(); ++ii)
        {
            std::weak_ptr<Actor> projectile = projectileManager.GetProjectile(ii);
            
            if (!projectile.lock()->GetComponent<ProjectileTornade>()) return;
            
            // 魔法位置
            DirectX::XMFLOAT3 magicPosition = projectile.lock()->GetComponent<Transform>()->GetPosition();

            float magicRadius = projectile.lock()->GetComponent<Transform>()->GetRadius();

            float magicHeight = projectile.lock()->GetComponent<Transform>()->GetHeight();

            //// 衝突処理
            DirectX::XMFLOAT3 outPositon;
            // 下半身
            // 円柱と円
            if (Collision::IntersectCylinderVsCylinder(
                magicPosition,
                magicRadius,
                magicHeight,
                enemyPosition,
                enemyRudius,
                enemyHeight,
                outPositon))

            {

                // ダメージを与える。
                //enemy->ApplyDamage(1);
            // ダメージが通ったら消える。TRUEになるから
                if (enemy.lock()->GetComponent<HP>()->ApplyDamage(applyDamageSpecial, 0.5f))
                {


                    // ヒットエフェクト再生
                    {


                        hitEffect->Play(enemyPosition);

                        hitFire->Play(enemyPosition);


                    }
                    // 当たった時の副次的効果
                    {
                        //specialAttackCharge += 0.1f;
                    }

                }
            }
        }
    }
}




bool Player::Ground()
{
    if (movement.lock()->GetOnLadius())
    {
        //jumpCount = 0;
        //TransitionLandState();
        return true;
    }
    return false;
    
}

void Player::AreAttackDecreaseAmount()
{
    // 空中行動制限
    --areAttackState;
}

// 経過時間
bool Player::UpdateElapsedTime(float timeMax, float elapsedTime)
{
    if (timeElapsed >= timeMax)
    {
        timeElapsed = 0.0f;
        return true;
    }
    timeElapsed += elapsedTime;
    return false;
}
//
//void Player::PintchMode()
//{
//
//    // ピンチの時にヒント
//    if (InputShortCutkeyMagic() &&
//        isPintch)
//    {
//
//       
//
//        std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
//          (int)UiManager::UiCount::PlayerCommandShortCutKeule)->GetComponent<TransForm2D>();
//
//        uiIdAttackCheckPos.lock()->Shake();
//
//        //Graphics& graphics = Graphics::Instance();
//        //Camera& camera = Camera::Instance();
//
//        //// ビューポート 画面のサイズ等
//        //// ビューポートとは2Dの画面に描画範囲の指定(クリッピング指定も出来る)位置を指定
//        //D3D11_VIEWPORT viewport;
//        //UINT numViewports = 1;
//        //// ラスタライザーステートにバインドされているビューポート配列を取得
//        //dc->RSGetViewports(&numViewports, &viewport);
//
//
//
//
//        //// 変換行列
//        //DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
//        //DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
//        //// ローカルからワールドに行くときにいる奴相手のポジションを渡す。
//        //DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();
//
//        //std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
//        //    (int)UiManager::UiCount::Push)->GetComponent<TransForm2D>();
//
//        //// スクリーン座標位置
//        //DirectX::XMVECTOR screenPositionV = DirectX::XMLoadFloat2(&uiIdAttackCheckPos.lock()->GetPosition());
//
//
//        //// ワールド座標
//        //DirectX::XMVECTOR WorldPositionV = DirectX::XMVector3Unproject(
//        //    screenPositionV,
//        //    viewport.TopLeftX,
//        //    viewport.TopLeftY,
//        //    viewport.Width,
//        //    viewport.Height,
//        //    viewport.MinDepth,
//        //    viewport.MaxDepth,
//        //    View,
//        //    Projection,
//        //    World);
//
//
//
//        //DirectX::XMFLOAT3 pos;
//
//        //DirectX::XMStoreFloat3(&pos, WorldPositionV);
//
//        //effectFocus2D->Play(pos);
//    }
//    if (magicAction &&
//        isPintch)
//    {
//        std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
//            (int)UiManager::UiCount::PlayerCommandHeale)->GetComponent<TransForm2D>();
//
//        uiIdAttackCheckPos.lock()->Shake();
//    }
//
//
//    if (mp.lock()->GetMpEmpth())
//    {
//        DirectX::XMFLOAT2 pos = { 179, 640 };
//
//        std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
//            (int)UiManager::UiCount::PlayerCommandShortCutKeule)->GetComponent<TransForm2D>();
//
//        uiIdAttackCheckPos.lock()->SetPosition(pos);
//
//        uiIdAttackCheckPos = UiManager::Instance().GetUies(
//            (int)UiManager::UiCount::PlayerCommandHeale)->GetComponent<TransForm2D>();
//
//        pos = { 200, 525 };
//
//        uiIdAttackCheckPos.lock()->SetPosition(pos);
//
//
//    }
//
//}








//更新処理
void PlayerManager::DeleteUpdate()
{

    //破棄処理
    for (std::shared_ptr<Actor> player : removes)
    {
        //std::vector要素を削除する場合はイテレータで削除する
        std::vector<std::shared_ptr<Actor>>::iterator it = std::find(players.begin(), players.end(), player);

        if (it != players.end())
        {
            players.erase(it);
        }

        //削除
        //delete player;
        
    }
    //破棄リストをクリア
    removes.clear();
}

void PlayerManager::Register(std::shared_ptr<Actor> actor)
{
    players.emplace_back(actor);
}



void PlayerManager::Remove(std::shared_ptr<Actor> player)
{
    // 削除登録
    removes.insert(player);
}

void PlayerManager::Clear()
{
    for (std::shared_ptr<Actor>& actor : players)// 
    {
        // 実体を消した管理している数はそのまま
       // delete projectile;
        //Remove(actor);
        actor.reset();
        //actor.reset();
    }

    players.clear();
}

