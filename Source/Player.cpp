#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
#include "Collision.h"
#include "Audio\Audio.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "ProjectileFullHoming.h"
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
    stateMachine.reset(nullptr);
}

void Player::Start()
{
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
    // collisionを使えるように
    collision = GetActor()->GetComponent<Collision>();
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
    collision.lock()->SetRadius(radius);
    // 身長
    collision.lock()->SetHeight(height);
    // コマンド操作用
    selectCheck = (int)CommandAttack::Attack;
    // 魔法選択用
    selectMagicCheck = (int)CommandMagic::Normal;
    // 特殊攻撃ため初期値
    specialAttackCharge = 0.0f;
    // アニメーションルール
    updateanim = UpAnim::Normal;
    moveSpeedAnimation = 0.0f;
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
    GamePad& gamePad = Input::Instance().GetGamePad();
    //// ステート毎の処理
    stateMachine->Update(elapsedTime);
    // エフェクト位置更新
    if (areWork->GetEfeHandle())
    {
        areWork->SetPosition(areWork->GetEfeHandle(), position);
    }

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

    if (areAttackState <= areAttackStateEnd && isAreAttack)
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
            // もし地面なら何もしない
            bool noStart = false;
            // エフェクト再生
            areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;
            areWork->Play(position);
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
        else
        {

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
    }
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
    CollisionBornVsProjectile("body2");
    CollisionPlayerVsEnemies();
    // 弾丸当たり判定
    CollisionMagicVsEnemies();

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
        model->UpdateUpeerBodyAnimation(elapsedTime, bornUpStartPoint, bornUpEndPoint, true);
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
    rc.isSpecular = isSpecular;
    // 影オンオフ
    rc.isRimRightning = isRimRightning;
    // modelオンオフ
    rc.StencilRef = StencilRef;
    if (isPlayerDrawCheck == skipDraw) return;
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
    audioParam.loop = isLoopSe;
    audioParam.volume = seVolume;
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
    audioParam.loop = isLoopDisabled;
    audioParam.volume = seVolume;
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
    audioParam.loop = isLoopDisabled;
    audioParam.volume = seVolume;
    Se.Play(audioParam);
}

void Player::InputDashSE()
{
    Audio& Se = Audio::Instance();
    AudioParam audioParam;
    audioParam.filename = "Data/Audio/SE/ヒットストップ.wav";
    audioParam.keyName = "BGM";
    audioParam.loop = isLoopDisabled;
    audioParam.volume = seVolume;
    Se.Play(audioParam);
}

// SE再生 斬撃
void Player::InputAttackSlashSE()
{
    Audio& Se = Audio::Instance();
    AudioParam audioParam;
    audioParam.filename = "Data/Audio/SE/スラッシュ２回目.wav";
    audioParam.keyName = "BGM";
    audioParam.loop = isLoopDisabled;
    audioParam.volume = seVolume;
    Se.Play(audioParam);
}

void Player::InputAttackFlameSE()
{
    Audio& Se = Audio::Instance();
    AudioParam audioParam;
    audioParam.filename = "Data/Audio/SE/炎飛行時.wav";
    audioParam.keyName = "BGM";
    audioParam.loop = isLoopDisabled;
    audioParam.volume = seVolume;
    Se.Play(audioParam);
}

void Player::InputAttackThanderSE()
{
    Audio& Se = Audio::Instance();
    AudioParam audioParam;
    audioParam.filename = "Data/Audio/SE/雷.wav";
    audioParam.keyName = "BGM";
    audioParam.loop = isLoopDisabled;
    audioParam.volume = seVolume;
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
    audioParam.loop = isLoopDisabled;
    audioParam.volume = seVolume;
    Se.Play(audioParam);
}

void Player::InputAttackHealeSE()
{
    Audio& Se = Audio::Instance();
    AudioParam audioParam;
    audioParam.filename = "Data/Audio/SE/maou_se_magical11.wav";
    audioParam.keyName = "BGM";
    audioParam.loop = isLoopDisabled;
    audioParam.volume = seVolume;
    Se.Play(audioParam);
}

void Player::InputAttackSlashSpecileLightningStrikeSE()
{
    Audio& Se = Audio::Instance();
    AudioParam audioParam;
    audioParam.filename = "Data/Audio/SE/必殺技雷.wav";
    audioParam.keyName = "BGM";
    audioParam.loop = isLoopDisabled;
    audioParam.volume = seVolume;
    Se.Play(audioParam);
}

void Player::InputAttackFlameSpecileSE()
{
    Audio& Se = Audio::Instance();
    AudioParam audioParam;
    audioParam.filename = "Data/Audio/SE/必殺技炎.wav";
    audioParam.keyName = "BGM";
    audioParam.loop = isLoopDisabled;
    audioParam.volume = seVolume;
    Se.Play(audioParam);
}

void Player::UpdateCameraState(float elapsedTime)
{
    CameraState oldLockonState = lockonState;
    DirectX::XMFLOAT3 oldLockonCharacter = lockonCharactor;
    lockonState = CameraState::NotLockOn;
    lockonCharactor = {0,0,0};
        // ロックオンモード
    if (rockCheck)
    {
        Model::Node* PRock = model->FindNode("mixamorig:Spine1");
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
                stateEnemyIndex = EnemyManager::Instance().GetEnemy(ii)->GetComponent<EnemyBoss>()->GetStateMachine()->
                    GetStateIndex();
                if (stateEnemyIndex == (int)EnemyBoss::State::Jump ||
                    stateEnemyIndex == (int)EnemyBoss::State::Attack ||
                    stateEnemyIndex == (int)EnemyBoss::State::Wander
                    )
                {
                    lockonState = CameraState::AttackLock;
                }
                else
                {
                    lockonState = CameraState::LockOn;
                }
                p = DirectX::XMLoadFloat3(&pPosition);
                t = DirectX::XMLoadFloat3(&character);
                v = DirectX::XMVectorSubtract(t, p);
                lockonCharactor = character;
            }
            // 右スティックでロックオン対象を変更する処理
            GamePad& gamePad = Input::Instance().GetGamePad();
            float ax = gamePad.GetAxisRX();	// 水平のみ
            // 垂直方向は使わないでおく
            lockonTargetChangeTime -= flame * elapsedTime;
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
                    Model::Node* characterWorld = EnemyManager::Instance().GetEnemy(ii)->GetComponent<ModelControll>()->GetModel()->FindNode("body1");
                    DirectX::XMFLOAT3 character =
                    {
                        characterWorld->worldTransform._41,
                        characterWorld->worldTransform._42,
                        characterWorld->worldTransform._43
                    };
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
                lockonCharactor =
                    EnemyManager::Instance().GetEnemy(ii)->
                    GetComponent<ModelControll>()->GetModel()->
                    ConvertLocalToWorld(characterWorld);
                stateEnemyIndex = EnemyManager::Instance().GetEnemy(ii)->GetComponent<EnemyBoss>()->GetStateMachine()->
                    GetStateIndex();
                if (stateEnemyIndex != (int)EnemyBoss::State::Jump &&
                    stateEnemyIndex != (int)EnemyBoss::State::Attack)
                {
                    lockonState = CameraState::LockOn;
                }
            }
            break;
        }
        }
        if (lockonState == CameraState::LockOn)
        {
            MessageData::CAMERACHANGELOCKONMODEDATA	p = { pPosition, lockonCharactor };
            Messenger::Instance().SendData(MessageData::CAMERACHANGELOCKONMODE, &p);
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
    DebugLength();
    ImGui::InputFloat("Move Speed", &moveSpeed);
    ImGui::SliderFloat3("angle", &angle.x, -10, 10);
    ImGui::SliderFloat("debugLength", &debugLength, -10, 10);
    transform.lock()->SetAngle(angle);
    ImGui::InputInt("selectCheck", &selectCheck);
    ImGui::InputInt("isPlayerDrawCheck", &isPlayerDrawCheck);
    ImGui::InputInt("selectMagicCheck", &selectMagicCheck);
    ImGui::SliderFloat("specialAttackCharge", &specialAttackCharge,0,1.5f);
    ImGui::SliderFloat("specialShotCharge", &specialShotCharge,0,1.5f);
    ImGui::SliderFloat("dot", &dotfake, 0, 1);
    ImGui::SliderFloat("blend", &moveSpeedAnimation, 0.0f, 1.0f);
    if (ImGui::Button("RockCamera"))
    {
        specialCameraRock = specialCameraRock ? false : true;
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
    if (specialCameraRock)
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
        MessageData::CAMERACHANGEMOTIONMODEDATA	p;
        position = transform.lock()->GetPosition();
        angle = transform.lock()->GetAngle();
        float vx = sinf(angle.y) * 6;
        float vz = cosf(angle.y) * 6;
        float vx2 = sinf(angle.y) - 10;
        float vx3 = sinf(angle.y);
        float vz2 = cosf(angle.y) * 7;
        p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
        p.data.push_back({ 50, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
        p.data.push_back({ 90, {position.x + vx3 , position.y + 2, (position.z + 0.1f) - vz2 }, position });
        Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);
        Model::ModelAnim modelAnim;
        modelAnim.index = Player::Animation::Anim_SpecialAttack;
        // アニメーション再生
        model->PlayAnimation(modelAnim);
        angleCameraCheck = true;
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
    if (ax + FLT_EPSILON != isInputEmpty + FLT_EPSILON &&
        GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage) &&
        GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Death))
    {
        
        return true;
    }
    if (ay + FLT_EPSILON != isInputEmpty + FLT_EPSILON &&
        GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage) &&
        GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Death))
    {

        return true;
    }
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
        Model::Node* characterWorld = EnemyManager::Instance().GetEnemy(i)->GetComponent<ModelControll>()->GetModel()->FindNode("shoulder");
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
        if (rockCheck || !specialRockOff)
        {
            uiIdSight.lock()->SetDrawCheck(isDrawUi);
        }
        if (scereenPosition.z < 0.0f || scereenPosition.z > 1.0f || !rockCheck || specialRockOff)
        {
            uiIdSight.lock()->SetDrawCheck(isDrawUiEmpth);
            continue;
        }
        // 2Dスプライト描画
        {
            uiIdSightTransform.lock()->SetPosition(
                { 
                    scereenPosition.x,
                    scereenPosition.y
                });
            uiIdSightMoveTransform.lock()->SetPosition(
                {
                    scereenPosition.x - scereenPositionOffset.x,
                    scereenPosition.y - scereenPositionOffset.y
                });
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
        bool isPush = true;
        gamePad.SetButtonDownCountinue(isPush);
        return true;
    }
    // 一度離すまでボタン効かない
    if (gamePad.GetButtonUp() & GamePad::BTN_B)
    {
        bool isPush = false;
        gamePad.SetButtonDownCountinue(isPush);
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
        uiIdAttack.lock()->SetDrawCheck(isDrawUiEmpth);
        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUi);
    }
    // 魔法選んだ時
    else
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttack)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttackCheck)->GetComponent<Ui>();
        uiIdAttack.lock()->SetDrawCheck(isDrawUi);
        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUiEmpth);
    }
    // 魔法選んだ時
    if (selectCheck == (int)CommandAttack::Magic)
    {
        std::weak_ptr<Ui> uiIdMagick = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagick)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdMagickCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagickCheck)->GetComponent<Ui>();
        uiIdMagick.lock()->SetDrawCheck(isDrawUiEmpth);
        uiIdMagickCheck.lock()->SetDrawCheck(isDrawUi);
    }
    // 攻撃選ぶ
    else
    {
        std::weak_ptr<Ui> uiIdMagick = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagick)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdMagickCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagickCheck)->GetComponent<Ui>();
        uiIdMagick.lock()->SetDrawCheck(isDrawUi);
        uiIdMagickCheck.lock()->SetDrawCheck(isDrawUiEmpth);
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
    // ショートカットキー
    if (gamePad.GetButtonUp())
    {
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<Ui>();

        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUiEmpth);
    }
    // ショートカットキー炎選択
    if (InputShortCutkeyMagic() &&
        gamePad.GetButtonDown() & GamePad::BTN_B )
    {
        selectMagicCheck = (int)CommandMagic::Fire;
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<Ui>();
        // 選択Icon表示と位置の調整を想定して変えておく
        std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<TransForm2D>();
        // ショートカット魔法炎の位置
        std::weak_ptr<TransForm2D> uiIdFireShortCutCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutFire)->GetComponent<TransForm2D>();
        //DirectX::XMFLOAT2 pos = { 179, 340 };
        DirectX::XMFLOAT2 pos = uiIdFireShortCutCheckPos.lock()->GetPosition();
        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUi);
        uiIdAttackCheckPos.lock()->SetPosition(pos);
    }
    // ショートカットキー雷選択
    if (InputShortCutkeyMagic() &&
        gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        selectMagicCheck = (int)CommandMagic::Thander;
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<Ui>();
        std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<TransForm2D>();
        // ショートカット魔法雷の位置
        std::weak_ptr<TransForm2D> uiIdSunderShortCutCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutSunder)->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = uiIdSunderShortCutCheckPos.lock()->GetPosition();
        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUi);
        uiIdAttackCheckPos.lock()->SetPosition(pos);
    }
    // ショートカットキー回復選択
    if (InputShortCutkeyMagic() &&
        gamePad.GetButtonDown() & GamePad::BTN_Y )
    {
        selectMagicCheck = (int)CommandMagic::Heale;
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<Ui>();
        std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<TransForm2D>();
        // ショートカット魔法回復の位置
        std::weak_ptr<TransForm2D> uiIdHealeShortCutCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutKeule)->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = uiIdHealeShortCutCheckPos.lock()->GetPosition();
        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUi);
        uiIdAttackCheckPos.lock()->SetPosition(pos);
    }
    // ショートカットキー氷選択
    if (InputShortCutkeyMagic() &&
        gamePad.GetButtonDown() & GamePad::BTN_A )
    {
        selectMagicCheck = (int)CommandMagic::Ice;
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<Ui>();
        std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<TransForm2D>();
        // ショートカット魔法氷の位置
        std::weak_ptr<TransForm2D> uiIdIceShortCutCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutIce)->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = uiIdIceShortCutCheckPos.lock()->GetPosition();
        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUi);
        uiIdAttackCheckPos.lock()->SetPosition(pos);
    }
    /////////////////////////
    if (!magicAction)
    {
        selectMagicCheck = (int)CommandMagic::Normal;
    }
    /*if (InputShortCutkeyMagic()) return false;*/
    int uiCount = UiManager::Instance().GetUiesCount();
    // ui無かったら
    if (uiCount <= uiCountMax) return false;
    // UI設定 炎
    if (selectMagicCheck == (int)CommandMagic::Fire && magicAction&&
        !InputShortCutkeyMagic())
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFire)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFireCheck)->GetComponent<Ui>();
        uiIdAttack.lock()->SetDrawCheck(isDrawUiEmpth);
        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUi);
    }
    else
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFire)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFireCheck)->GetComponent<Ui>();
        uiIdAttack.lock()->SetDrawCheck(isDrawUi);
        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUiEmpth);
    }
    // 魔法を発動していなかったら　ショートカットなら　十字キー操作UI解除
    if (!magicAction ||
        InputShortCutkeyMagic())
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFire)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFireCheck)->GetComponent<Ui>();
        uiIdAttack.lock()->SetDrawCheck(isDrawUiEmpth);
        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUiEmpth);
    }
    // UI設定 雷
    if (selectMagicCheck == (int)CommandMagic::Thander && magicAction &&
        !InputShortCutkeyMagic())
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtning)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtningCheck)->GetComponent<Ui>();
        uiIdAttack.lock()->SetDrawCheck(isDrawUiEmpth);
        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUi);
    }
    else
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtning)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtningCheck)->GetComponent<Ui>();
        uiIdAttack.lock()->SetDrawCheck(isDrawUi);
        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUiEmpth);
    }
    // 魔法を発動していなかったら　ショートカットなら　十字キー操作UI解除
    if (!magicAction || 
        InputShortCutkeyMagic())
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtning)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtningCheck)->GetComponent<Ui>();
        uiIdAttack.lock()->SetDrawCheck(isDrawUiEmpth);
        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUiEmpth);
    }
    // UI設定 氷
    if (selectMagicCheck == (int)CommandMagic::Ice && magicAction &&
        !InputShortCutkeyMagic())
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIce)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIceCheck)->GetComponent<Ui>();
        uiIdAttack.lock()->SetDrawCheck(isDrawUiEmpth);
        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUi);
    }
    else
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIce)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIceCheck)->GetComponent<Ui>();
        uiIdAttack.lock()->SetDrawCheck(isDrawUi);
        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUiEmpth);
    }
    // 魔法を発動していなかったら　ショートカットなら　十字キー操作UI解除
    if (!magicAction || 
        InputShortCutkeyMagic())
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIce)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIceCheck)->GetComponent<Ui>();
        uiIdAttack.lock()->SetDrawCheck(isDrawUiEmpth);
        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUiEmpth);
    }
    // UI設定 回復
    if (selectMagicCheck == (int)CommandMagic::Heale && magicAction &&
        !InputShortCutkeyMagic())
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandHeale)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandHealeCheck)->GetComponent<Ui>();
        uiIdAttack.lock()->SetDrawCheck(isDrawUiEmpth);
        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUi);
    }
    else
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandHeale)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandHealeCheck)->GetComponent<Ui>();
        uiIdAttack.lock()->SetDrawCheck(isDrawUi);
        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUiEmpth);
    }
    // 魔法を発動していなかったら　ショートカットなら　十字キー操作UI解除
    if (!magicAction ||
        InputShortCutkeyMagic())
    {
        std::weak_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandHeale)->GetComponent<Ui>();
        std::weak_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandHealeCheck)->GetComponent<Ui>();
        uiIdAttack.lock()->SetDrawCheck(isDrawUiEmpth);
        uiIdAttackCheck.lock()->SetDrawCheck(isDrawUiEmpth);
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
        uiIdAttackCheckFire.lock()->SetDrawCheck(isDrawUi);
        // 雷
        std::weak_ptr<Ui> uiIdAttackCheckSunder = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutSunder)->GetComponent<Ui>();
        uiIdAttackCheckSunder.lock()->SetDrawCheck(isDrawUi);
        // 氷
        std::weak_ptr<Ui> uiIdAttackCheckIce = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutIce)->GetComponent<Ui>();
        uiIdAttackCheckIce.lock()->SetDrawCheck(isDrawUi);
        // 回復
        std::weak_ptr<Ui> uiIdAttackCheckHeale = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutKeule)->GetComponent<Ui>();
        uiIdAttackCheckHeale.lock()->SetDrawCheck(isDrawUi);
        return true;
    }
    //// 魔法打ってる途中にショートカット離しても意味なし
    //if (stateMachine->GetStateIndex() == static_cast<int>(Player::State::Magic) &&
    //    gamePad.GetButton() != GamePad::BTN_LEFT_SHOULDER)
    //{
    //    return true;
    //}
    // 魔法打った後
    else if (gamePad.GetButton() == GamePad::BTN_LEFT_SHOULDER)
    {
        //selectMagicCheck = (int)CommandMagic::Normal;
        ////selectCheck = (int)CommandAttack::Attack;
        //magicAction = false;
        
        // 炎
        std::weak_ptr<Ui> uiIdAttackCheckFire = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutFire)->GetComponent<Ui>();
        uiIdAttackCheckFire.lock()->SetDrawCheck(isDrawUiEmpth);
        // 雷
        std::weak_ptr<Ui> uiIdAttackCheckSunder = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutSunder)->GetComponent<Ui>();
        uiIdAttackCheckSunder.lock()->SetDrawCheck(isDrawUiEmpth);
        // 氷
        std::weak_ptr<Ui> uiIdAttackCheckIce = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutIce)->GetComponent<Ui>();
        uiIdAttackCheckIce.lock()->SetDrawCheck(isDrawUiEmpth);
        // 回復
        std::weak_ptr<Ui> uiIdAttackCheckHeale = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutKeule)->GetComponent<Ui>();
        uiIdAttackCheckHeale.lock()->SetDrawCheck(isDrawUiEmpth);
    }

    // 離したら
    if (gamePad.GetButtonUp() & GamePad::BTN_LEFT_SHOULDER)
    {
        // 魔法選択UI解除
        RemoveUIMagic();
        // 炎
        std::weak_ptr<Ui> uiIdAttackCheckFire = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutFire)->GetComponent<Ui>();
        uiIdAttackCheckFire.lock()->SetDrawCheck(isDrawUiEmpth);
        // 雷
        std::weak_ptr<Ui> uiIdAttackCheckSunder = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutSunder)->GetComponent<Ui>();
        uiIdAttackCheckSunder.lock()->SetDrawCheck(isDrawUiEmpth);
        // 氷
        std::weak_ptr<Ui> uiIdAttackCheckIce = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutIce)->GetComponent<Ui>();
        uiIdAttackCheckIce.lock()->SetDrawCheck(isDrawUiEmpth);
        // 回復
        std::weak_ptr<Ui> uiIdAttackCheckHeale = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutKeule)->GetComponent<Ui>();
        uiIdAttackCheckHeale.lock()->SetDrawCheck(isDrawUiEmpth);
    }
    return false;
}
// 魔法選択UI解除
void Player::RemoveUIMagic()
{
    selectMagicCheck = (int)CommandMagic::Normal;
    selectCheck = (int)CommandAttack::Attack;
    magicAction = false;
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
        //// 一度発動すると初期化
        specialAttackCharge = 0.0f;
        bool drawCheck = false;
        uiIdSpecialChargeFurst.lock()->SetDrawCheck(drawCheck);
        uiIdSpecialChargeSecond.lock()->SetDrawCheck(drawCheck);
        uiIdSpecialChargeSerde.lock()->SetDrawCheck(drawCheck);
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
                specialAttackCharge = specialAttackChargeMin;
                // 斬撃必殺技チャージ解消
                attackEnergyCharge = attackEnergyChargeMin;
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
                specialAttackCharge = specialAttackChargeMin;
                // 火必殺技チャージ解消
                fireEnergyCharge = fireEnergyChargeMin;
            }
    }
    // 技を放つ
    if (gamePad.GetButtonDown() & GamePad::BTN_Y &&  !specialAttackTime && specialAttack.size() > 0&& !magicAction)
    {
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
        default:
        {
            break;
        }
        }
        specialAttackTime = true;
        return true;
    }
    else
    {
        specialAttackTime = false;
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

void Player::PlayPintchSe()
{
    Audio& Se = Audio::Instance();
    AudioParam audioParam;
    audioParam.filename = "Data/Audio/SE/HP危険.wav";
    audioParam.loop = isLoopDisabled;
    audioParam.volume = seVolume;
    Se.Play(audioParam);
}

void Player::StopPintchSe()
{
    Audio& bgm = Audio::Instance();
    AudioParam audioParam;
    audioParam.filename = "Data/Audio/SE/HP危険.wav";
    bgm.Stop(audioParam);
}

void Player::PlaySpecialChargeCompleteSe()
{
    Audio& Se = Audio::Instance();
    AudioParam audioParam;
    audioParam.filename = "Data/Audio/SE/必殺技ため.wav";
    audioParam.loop = isLoopDisabled;
    audioParam.volume = seVolume;
    Se.Play(audioParam);
}

void Player::PlayTellePortSe()
{
    Audio& Se = Audio::Instance();
    AudioParam audioParam;
    audioParam.filename = "Data/Audio/SE/telleport.wav";
    audioParam.loop = isLoopDisabled;
    audioParam.volume = seVolume;
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
    vec.y = isInputEmpty;
    if (vec.x != isInputEmpty || vec.y != isInputEmpty || vec.z != isInputEmpty)
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
    vec.y = isInputEmpty;

    if (vec.x != isInputEmpty || vec.y != isInputEmpty || vec.z != isInputEmpty)
    {
        movement.lock()->Move(vec, moveSpeed, elapsedTime);
    }
    return vec;
}

// 魔法と敵の衝突処理
void Player::CollisionMagicVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
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
            float enemyRudius = enemy.lock()->GetComponent<Collision>()->GetRadius();
            float enemyHeight = enemy.lock()->GetComponent<Collision>()->GetHeight();
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
            float projectileRadius = projectile.lock()->GetComponent<Collision>()->GetRadius();
            //// 衝突処理
            DirectX::XMFLOAT3 outPositon;
            if (collision.lock()->IntersectSphereVsCylinder(
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
                collision.lock()->IntersectSpherVsSphere(
                    projectilePosition,
                    leftHandRadius,
                    {
                    nodeHeartPosition.x,
                    nodeHeartPosition.y,
                    nodeHeartPosition.z
                    },
                    enemyRudius,
                    outPositon) ||

                collision.lock()->IntersectSphereVsCylinder(
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
                collision.lock()->IntersectSphereVsCylinder(
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
                if (!projectile.lock()->GetComponent<ProjectileHoming>() && !projectile.lock()->GetComponent<ProjectileSunder>() &&
                    !projectile.lock()->GetComponent<ProjectileFullHoming>() && 
                    !projectile.lock()->GetComponent<ProjectileStraight>())return;
                if (projectile.lock()->GetComponent<ProjectileSunder>())
                {
                    ++ThanderEnergyCharge;
                    hitThander->Play(projectilePosition);
                    // 雷ダメージ
                    applyDamageMagic = applyDamageThander;
                }
                else if (projectile.lock()->GetComponent<ProjectileHoming>())
                {
                    // 通常魔法
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
                else if (projectile.lock()->GetComponent<ProjectileStraight>())
                {
                    ++iceEnergyCharge;
                    hitIce->Play(projectilePosition);
                    // 氷ダメージ
                    applyDamageMagic = applyDamageIce;           
                }
                else
                {
                    // 弧の時型の魔法
                    switch (projectile.lock()->GetComponent<ProjectileFullHoming>()->GetMagicNumber())
                    {
                    case (int)ProjectileFullHoming::MagicNumber::Fire:
                    {
                        ++fireEnergyCharge;
                        hitFire->Play(projectilePosition);
                        // 炎ダメージ
                        applyDamageMagic = applyDamageFire;
                        break;
                    }
                    //case (int)ProjectileFullHoming::MagicNumber::Ice:
                    //{
                    //    ++iceEnergyCharge;
                    //    hitIce->Play(projectilePosition);
                    //    // 氷ダメージ
                    //    applyDamageMagic = applyDamageIce;
                    //    break;
                    //}
                    }
                }
                hitEffect->Play(projectilePosition);
                // ダメージを与える。
                if (enemy.lock()->GetComponent<HP>()->ApplyDamage(applyDamageMagic, magicAttackInvincibleTime))
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
                    // 当たった時の副次的効果
                    {
                        specialAttackCharge += specialAttackChargeMagicValue;
                    }
                    if (projectile.lock()->GetComponent<ProjectileHoming>() || 
                        projectile.lock()->GetComponent<ProjectileFullHoming>() ||
                        projectile.lock()->GetComponent<ProjectileStraight>())
                        // 弾丸破棄
                        projectile.lock()->GetComponent<BulletFiring>()->Destroy();
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
        float enemyRadius = enemy.lock()->GetComponent<Collision>()->GetRadius();
        float enemyHeight = enemy.lock()->GetComponent<Collision>()->GetHeight();
        if (collision.lock()->IntersectCylinderVsCylinder(
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
            transform.lock()->SetPosition(position);
        }
    }
}

void Player::CollisionBornVsProjectile(const char* bornname)
{
    EnemyManager& enemyManager = EnemyManager::Instance();
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
        //float enemyRadius = enemy.lock()->GetComponent<Transform>()->GetRadius();
        float enemyRadius = enemy.lock()->GetComponent<EnemyBoss>()->GetUpperRadius();
        float enemyHeight = enemy.lock()->GetComponent<Collision>()->GetHeight();
        if (collision.lock()->IntersectCylinderVsCylinder(
            {
                nodePosition.x,
                nodePosition.y - 0.1f,
                nodePosition.z
            },
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
            }
            else
            {
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
        float enemyRudius = enemy.lock()->GetComponent<Collision>()->GetRadius();
        float enemyHeight = enemy.lock()->GetComponent<Collision>()->GetHeight();
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
        if (collision.lock()->IntersectSphereVsCylinder(
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
            collision.lock()->IntersectSpherVsSphere(
                nodePosition,
                leftHandRadius,
                {
                nodeHeartPosition.x,
                nodeHeartPosition.y,
                nodeHeartPosition.z
                },
                enemyRudius,
                outPositon) ||

            collision.lock()->IntersectSphereVsCylinder(
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
            collision.lock()->IntersectSphereVsCylinder(
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
                    enemy.lock()->GetComponent<EnemyBoss>()->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Jump &&
                    enemy.lock()->GetComponent<EnemyBoss>()->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::IdleBattle
                    )
                {
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
                specialAttackCharge += specialAttackChargeSlashValue;
                // 斬撃チャージ
                ++attackEnergyCharge;
                // 攻撃ヒット回数
                ++attackNumberSave;
                return true;
            }
        }
    }
    return false;
}

void Player::CollisionNodeVsEnemiesCounter(const char* nodeName, float nodeRadius)
{
    // ノード取得
    Model::Node* node = model->FindNode(nodeName);
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
            if (!projectille.lock()->GetComponent<ProjectileImpact>()) return;
            DirectX::XMFLOAT3 projectilePosition = projectille.lock()->GetComponent<Transform>()->GetPosition();
            float projectileInRudius = projectille.lock()->GetComponent<ProjectileImpact>()->GetRadiusInSide();
            float projectileOutRudius = projectille.lock()->GetComponent<ProjectileImpact>()->GetRadiusOutSide();
            float projectileHeight = projectille.lock()->GetComponent<Collision>()->GetHeight();
            //// 衝突処理
            DirectX::XMFLOAT3 outPositon;
            std::weak_ptr<EnemyBoss> enemyBoss = enemy.lock()->GetComponent<EnemyBoss>();
            // 球と球
            if (collision.lock()->IntersectSpherVsSphere(
                nodePosition,
                nodeRadius,
                projectilePosition,
                projectileOutRudius,
                outPositon) && 
                !collision.lock()->IntersectSpherVsSphere(
                    nodePosition,
                    nodeRadius,
                    projectilePosition,
                    projectileInRudius,
                    outPositon) &&projectille.lock()->GetComponent<ProjectileImpact>())
            {
                // 高さが一定以下なら通る
                if (projectilePosition.y + projectileHeight < nodePosition.y) return;
                // ヒットエフェクト再生
                {
                    projectille.lock()->GetComponent<ProjectileImpact>()->Destoroy();
                    ImpactEffect->Play(nodePosition);
                    // ヒット時少しスローに
                    hitCheck = true;
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
            ImGui::SliderFloat("frontVec", &frontVec.x,-0.37f,1.0);
            ImGui::SliderFloat("frontVecY", &frontVec.y,-0.37f,1.0);
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
    Model::Node* SwordeRootName = model->FindNode("mixamorig:RightHand");
    // 剣の手元
    DirectX::XMFLOAT3 swordeRootPosition;
    swordeRootPosition = model->ConvertLocalToWorld(SwordeRootName);
    // 前
    DirectX::XMFLOAT3 dir;
    dir.x = sinf(swordeRootPosition.y);// 三角を斜めにして位置を変えた
    dir.y = cosf(swordeRootPosition.x);
    dir.z = cosf(swordeRootPosition.y);
    DirectX::XMVECTOR dirVec = DirectX::XMLoadFloat3(&dir);
    DirectX::XMMATRIX swordeRootPositionVec = DirectX::XMLoadFloat4x4(&SwordeRootName->worldTransform);
    // 剣先
    DirectX::XMFLOAT3 swordeTipPosition = swordeRootPosition;
    swordeTipPosition.y *= (dir.y * 0.01f);
    trailPositions[0][0] = swordeRootPosition;
    trailPositions[1][0] = swordeTipPosition;
    // ポリゴン作成
    PrimitiveRenderer* primitiveRenderer = Graphics::Instance().GetPrimitiveRenderer();
    DirectX::XMFLOAT4 color = { 0,0,1,1 };
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
        if (UpdateElapsedTime(timeElapsedHintMax, elapsedTime))
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
            GetComponent<Ui>()->SetDrawCheck(isDrawUi);
        UiManager::Instance().GetUies((int)UiManager::UiCount::ShortCut)->
            GetComponent<Ui>()->SetDrawCheck(isDrawUi);
        PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
        vignetteData.color = vignettePinchColor;
        vignetteData.smoothness = vignettePinchSmoothness;
        vignetteData.intensity = vignettePinchIntensity;
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
        vignetteData.smoothness = vignetteNormalSmoothness;
        vignetteData.intensity = vignetteNormalIntensity;
        vignetteData.color = vignetteNormalColor;
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
    }
    if (magicAction &&
        isPintch)
    {
        std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandHeale)->GetComponent<TransForm2D>();
        uiIdAttackCheckPos.lock()->Shake();
    }
}

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

// 回避入力orカウンター
bool Player::InputAvoidance()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
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
    if (gamePad.GetButtonDown() & GamePad::BTN_B)
    {
        return true;
    }
    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT)
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
        if (gamePad.GetButton() & GamePad::BTN_B)
        {
            return true;
        }
        break;
    }
    case (int)CommandMagic::Thander:
    {
        // 魔法
        if (gamePad.GetButton() & GamePad::BTN_X)
        {
            return true;
        }
        break;
    }
    case (int)CommandMagic::Ice:
    {
        // 魔法
        if (gamePad.GetButton() & GamePad::BTN_A)
        {
            return true;
        }
        break;
    }
    case (int)CommandMagic::Heale:
    {
        // 魔法
        if (gamePad.GetButton() & GamePad::BTN_Y)
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

void Player::DebugLength()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();

    DirectX::XMFLOAT2 playerPos = {transform.lock()->GetPosition().x
        ,transform.lock()->GetPosition().z};
    DirectX::XMVECTOR playerPositionXZ = DirectX::XMLoadFloat2(&playerPos);
    DirectX::XMFLOAT2 enemyPosXZ;
    DirectX::XMVECTOR VectorXZ;
    DirectX::XMVECTOR LengthSqDebug;

    for (int i = 0; i < enemyCount; ++i)//float 最大値ないにいる敵に向かう
    {
        enemyPosXZ.x = enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition().x;
        enemyPosXZ.y = enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition().z;
        DirectX::XMVECTOR enemyPositionXZ = DirectX::XMLoadFloat2(&enemyPosXZ);

        VectorXZ = DirectX::XMVectorSubtract(enemyPositionXZ, playerPositionXZ);

        LengthSqDebug = DirectX::XMVector2Length(VectorXZ);


        DirectX::XMStoreFloat(&debugLength, LengthSqDebug);
    }
}

bool Player::InputMagicframe()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
        // mp消費
        mp.lock()->ApplyConsumption(magicConsumption);
        // 前方向 sinの計算
        DirectX::XMFLOAT3 dir;
        dir = GetForwerd(angle);
        // 発射位置（プレイヤーの腰当たり)
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;// 身長÷位置のｙ
        pos.z = position.z;
        //ターゲット（デフォルトではプレイヤーの前方）
        DirectX::XMFLOAT3 target;
        // 敵がいなかった時のために　1000先まで飛んでくれ
        target.x = pos.x + dir.x * magicMoveLength;
        target.y = pos.y + dir.y * magicMoveLength;
        target.z = pos.z + dir.z * magicMoveLength;
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
                target.y += enemy.lock()->GetComponent<Collision>()->GetHeight() * 0.5f;// 位置に身長分
            }
        }
        // 弾丸初期化
        const char* filename = "Data/Model/Magic/gun.mdl";
        std::weak_ptr<Actor> actor = ActorManager::Instance().Create();
        actor.lock()->AddComponent<ModelControll>();
        actor.lock()->GetComponent<ModelControll>()->LoadModel(filename);
        actor.lock()->SetName("ProjectileHoming");
        actor.lock()->AddComponent<Transform>();
        actor.lock()->GetComponent<Transform>()->SetPosition(position);
        actor.lock()->GetComponent<Transform>()->SetAngle(angle);
        actor.lock()->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
        actor.lock()->AddComponent<Collision>();
        actor.lock()->GetComponent<Collision>()->SetRadius(0.3f);
        actor.lock()->AddComponent<BulletFiring>();
        actor.lock()->AddComponent<ProjectileHoming>();
        const char* effectFilename = "Data/Effect/fire.efk";
        actor.lock()->GetComponent<ProjectileHoming>()->SetEffectProgress(effectFilename);
        int magicNumber = (int)ProjectileHoming::MagicNumber::Fire;
        actor.lock()->GetComponent<ProjectileHoming>()->SetMagicNumber(magicNumber);
        // これが２Dかの確認
        bool check2d = false;
        actor.lock()->SetCheck2d(check2d);
        ProjectileManager::Instance().Register(actor.lock());
        std::weak_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);
        // 飛ぶ時間
        float   lifeTimer = 3.0f;
        // 発射
        projectile.lock()->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        projectile.lock()->GetComponent<ProjectileHoming>()->SetTarget(target);
        return true;
}

bool Player::InputMagicIce()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
   

    // mp消費
    mp.lock()->ApplyConsumption(magicConsumption);
    // 前方向 sinの計算
    DirectX::XMFLOAT3 dir;
    dir = GetForwerd(angle);
    // 発射位置（プレイヤーの腰当たり)
    DirectX::XMFLOAT3 pos;
    pos.x = position.x;
    pos.y = position.y + height * 0.5f;// 身長÷位置のｙ
    pos.z = position.z;
    //ターゲット（デフォルトではプレイヤーの前方）
    DirectX::XMFLOAT3 target;
    // 敵がいなかった時のために　1000先まで飛んでくれ
    target.x = pos.x + dir.x * magicMoveLength;
    target.y = pos.y + dir.y * magicMoveLength;
    target.z = pos.z + dir.z * magicMoveLength;
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
            target.y += enemy.lock()->GetComponent<Collision>()->GetHeight() * 0.5f;// 位置に身長分
        }
    }
    // 弾丸初期化
    const char* filename = "Data/Model/Magic/gun.mdl";
    std::weak_ptr<Actor> actor = ActorManager::Instance().Create();
    actor.lock()->AddComponent<ModelControll>();
    actor.lock()->GetComponent<ModelControll>()->LoadModel(filename);
    actor.lock()->SetName("ProjectileHoming");
    actor.lock()->AddComponent<Transform>();
    actor.lock()->GetComponent<Transform>()->SetPosition(position);
    actor.lock()->GetComponent<Transform>()->SetAngle(angle);
    actor.lock()->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
    actor.lock()->AddComponent<Collision>();
    actor.lock()->GetComponent<Collision>()->SetRadius(0.3f);
    actor.lock()->AddComponent<BulletFiring>();
    actor.lock()->AddComponent<ProjectileStraight>();
    const char* effectFilename = "Data/Effect/brezerd.efk";
    actor.lock()->GetComponent<ProjectileStraight>()->SetEffectProgress(effectFilename);
    
    //int magicNumber = (int)ProjectileHoming::MagicNumber::Ice;
    //actor.lock()->GetComponent<ProjectileHoming>()->SetMagicNumber(magicNumber);
    // これが２Dかの確認
    bool check2d = false;
    actor.lock()->SetCheck2d(check2d);
    ProjectileManager::Instance().Register(actor.lock());
    std::weak_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);
    // 飛ぶ時間
    float   lifeTimer = 4.0f;



    // 発射
    projectile.lock()->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
    //projectile.lock()->GetComponent<ProjectileHoming>()->SetTarget(target);
    return true;
}

bool Player::InputMagicLightning()
{
    // mp消費
    mp.lock()->ApplyConsumption(magicConsumption);
    // 前方向 sinの計算
    DirectX::XMFLOAT3 dir;
    dir = GetForwerd(angle);
    // 発射位置（プレイヤーの腰当たり)
    DirectX::XMFLOAT3 pos;
    pos.x = position.x;
    pos.y = position.y + height * 0.5f;// 身長÷位置のｙ
    pos.z = position.z;
    //ターゲット（デフォルトではプレイヤーの前方）
    DirectX::XMFLOAT3 target;
    // 敵がいなかった時のために　1000先まで飛んでくれ
    target.x = pos.x + dir.x * magicMoveLength;
    target.y = pos.y + dir.y * magicMoveLength;
    target.z = pos.z + dir.z * magicMoveLength;
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
            target.y += enemy.lock()->GetComponent<Collision>()->GetHeight() * 0.5f;// 位置に身長分
        }
    }
    // 弾丸初期化
    const char* filename = "Data/Model/Magic/gun.mdl";
    std::weak_ptr<Actor> actor = ActorManager::Instance().Create();
    actor.lock()->AddComponent<ModelControll>();
    actor.lock()->GetComponent<ModelControll>()->LoadModel(filename);
    actor.lock()->SetName("ProjectileSunder");
    actor.lock()->AddComponent<Transform>();
    actor.lock()->GetComponent<Transform>()->SetPosition(pos);
    actor.lock()->GetComponent<Transform>()->SetAngle(angle);
    actor.lock()->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
    actor.lock()->AddComponent<Collision>();
    actor.lock()->GetComponent<Collision>()->SetRadius(0.3f);
    actor.lock()->AddComponent<BulletFiring>();
    actor.lock()->AddComponent<ProjectileSunder>();
    const char* effectFilename = "Data/Effect/lightningStrike.efk";
    actor.lock()->GetComponent<ProjectileSunder>()->SetEffectProgress(effectFilename);
    // これが２Dかの確認
    bool check2d = false;
    actor.lock()->SetCheck2d(check2d);
    ProjectileManager::Instance().Register(actor.lock());
    std::weak_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);
    float   lifeTimer = 0.5f;
    // 発射
    projectile.lock()->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
    projectile.lock()->GetComponent<ProjectileSunder>()->SetTarget(target);
    return true;
}
// 回復魔法開始
bool Player::InputMagicHealing()
{
    // mp消費
    mp.lock()->ApplyConsumption(mp.lock()->GetMaxMagic());
    
    hp.lock()->AddHealth(healing);
    return true;
}

void Player::PushMagicFrame(DirectX::XMFLOAT3 angle)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    // mp消費
    mp.lock()->ApplyConsumption(magicConsumption);
    // 前方向 sinの計算
    DirectX::XMFLOAT3 dir;
    //dir = GetForwerd(angle);
    dir.x = sinf(angle.y);
    dir.y = cosf(angle.x);
    dir.z = cosf(angle.y);
    // 発射位置（プレイヤーの腰当たり)
    DirectX::XMFLOAT3 pos;
    pos.x = position.x;
    pos.y = position.y + height * 0.5f;// 身長÷位置のｙ
    pos.z = position.z;
    //ターゲット（デフォルトではプレイヤーの前方）
    DirectX::XMFLOAT3 target;
    // 敵がいなかった時のために　1000先まで飛んでくれ
    target.x = pos.x + dir.x * magicMoveLength;
    target.y = pos.y + dir.y * magicMoveLength;
    target.z = pos.z + dir.z * magicMoveLength;
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
            target.y += enemy.lock()->GetComponent<Collision>()->GetHeight() * 0.5f;// 位置に身長分
        }
    }
    // 初期化
    {
        const char* filename = "Data/Model/Magic/gun.mdl";
        std::weak_ptr<Actor> actor = ActorManager::Instance().Create();
        actor.lock()->AddComponent<ModelControll>();
        actor.lock()->GetComponent<ModelControll>()->LoadModel(filename);
        actor.lock()->SetName("ProjectileHoming");
        actor.lock()->AddComponent<Transform>();
        actor.lock()->GetComponent<Transform>()->SetPosition(position);
        actor.lock()->GetComponent<Transform>()->SetAngle(angle);
        actor.lock()->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
        actor.lock()->AddComponent<Collision>();
        actor.lock()->GetComponent<Collision>()->SetRadius(0.3f);
        actor.lock()->AddComponent<BulletFiring>();
        actor.lock()->AddComponent<ProjectileFullHoming>();
        const char* effectFilename = "Data/Effect/fire.efk";
        actor.lock()->GetComponent<ProjectileFullHoming>()->SetEffectProgress(effectFilename);
        int magicNumber = (int)ProjectileFullHoming::MagicNumber::Fire;
        actor.lock()->GetComponent<ProjectileFullHoming>()->SetMagicNumber(magicNumber);
        // これが２Dかの確認
        bool check2d = false;
        actor.lock()->SetCheck2d(check2d);
        ProjectileManager::Instance().Register(actor.lock());
        std::weak_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);
        // 飛ぶ時間
        float   lifeTimer = 3.0f;
        // 発射
        projectile.lock()->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        projectile.lock()->GetComponent<ProjectileFullHoming>()->SetTarget(target);
    }
    return;
}

void Player::PushMagicIce(DirectX::XMFLOAT3 angle)
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    // mp消費
    mp.lock()->ApplyConsumption(magicConsumption);
    // 前方向 sinの計算
    DirectX::XMFLOAT3 dir;
   /* dir.x = sinf(angle.y);
    dir.y = cosf(angle.x);
    dir.z = cosf(angle.y);*/
    dir = GetForwerd(angle);
    // 発射位置（プレイヤーの腰当たり)
    DirectX::XMFLOAT3 pos;
    pos.x = position.x;
    pos.y = position.y + height * 0.5f;// 身長÷位置のｙ
    pos.z = position.z;
    //ターゲット（デフォルトではプレイヤーの前方）
    DirectX::XMFLOAT3 target;
    // 敵がいなかった時のために　1000先まで飛んでくれ
    target.x = pos.x + dir.x * magicMoveLength;
    target.y = pos.y + dir.y * magicMoveLength;
    target.z = pos.z + dir.z * magicMoveLength;
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

        // 距離が敵のものを入れる少なくする３０なら３０、１００なら１００入れる
        dist = d;
        target = enemy.lock()->GetComponent<Transform>()->GetPosition();// 位置を入れる
        target.y += enemy.lock()->GetComponent<Collision>()->GetHeight() * 0.5f;// 位置に身長分

    }
    // 弾丸初期化
    const char* filename = "Data/Model/Magic/gun.mdl";
    std::weak_ptr<Actor> actor = ActorManager::Instance().Create();
    actor.lock()->AddComponent<ModelControll>();
    actor.lock()->GetComponent<ModelControll>()->LoadModel(filename);
    actor.lock()->SetName("ProjectileIce");
    actor.lock()->AddComponent<Transform>();
    actor.lock()->GetComponent<Transform>()->SetPosition(pos);
    actor.lock()->GetComponent<Transform>()->SpawnRandomInArea(randomPosMax, randomPosMin);
    actor.lock()->GetComponent<Transform>()->SetAngle(angle);
    actor.lock()->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
    pos = actor.lock()->GetComponent<Transform>()->GetPosition();
    actor.lock()->AddComponent<Collision>();
    actor.lock()->GetComponent<Collision>()->SetRadius(0.3f);
    actor.lock()->AddComponent<BulletFiring>();
    actor.lock()->AddComponent<ProjectileStraight>();
    const char* effectFilename = "Data/Effect/brezerd.efk";
    actor.lock()->GetComponent<ProjectileStraight>()->SetEffectProgress(effectFilename);


    effectFilename = "Data/Effect/flozeTime.efk";
    actor.lock()->GetComponent<ProjectileStraight>()->SetEffectSpawned(effectFilename);
    //int magicNumber = (int)ProjectileStraight::MagicNumber::Ice;
    //actor.lock()->GetComponent<ProjectileFullHoming>()->SetMagicNumber(magicNumber);
    //// 回転速度変更
    //float migicIceTurnSpeed = DirectX::XMConvertToRadians(200);
    //actor.lock()->GetComponent<ProjectileFullHoming>()->SetTurnSpeed(migicIceTurnSpeed);

    //// 一時停止
    //actor.lock()->GetComponent<ProjectileFullHoming>()->SetMovementCheck(iceMagicMoveCheck);

    // これが２Dかの確認
    bool check2d = false;
    actor.lock()->SetCheck2d(check2d);
    ProjectileManager::Instance().Register(actor.lock());
    std::weak_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);
    // 飛ぶ時間
    float   lifeTimer = 4.0f;
    // 発射
    projectile.lock()->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
   // projectile.lock()->GetComponent<ProjectileFullHoming>()->SetTarget(target);
}

void Player::InputSpecialMagicframe()
{
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
    const char* filename = "Data/Model/Magic/gun.mdl";
    std::weak_ptr<Actor> actor = ActorManager::Instance().Create();
    actor.lock()->AddComponent<ModelControll>();
    actor.lock()->GetComponent<ModelControll>()->LoadModel(filename);
    actor.lock()->SetName("MagicFrameTornade");
    actor.lock()->AddComponent<Transform>();
    actor.lock()->GetComponent<Transform>()->SetPosition(target);
    actor.lock()->GetComponent<Transform>()->SetAngle(angle);
    actor.lock()->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
    actor.lock()->AddComponent<Collision>();
    actor.lock()->GetComponent<Collision>()->SetRadius(0.3f);
    actor.lock()->AddComponent<BulletFiring>();
    actor.lock()->AddComponent<ProjectileTornade>();
    const char* effectFilename = "Data/Effect/fireTornade.efk";
    actor.lock()->GetComponent<ProjectileTornade>()->SetEffectProgress(effectFilename);
    // これが２Dかの確認
    bool check2d = false;
    actor.lock()->SetCheck2d(check2d);
    ProjectileManager::Instance().Register(actor.lock());
    std::weak_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);
    float   lifeTimer = 0.5f;
    // 発射
    projectile.lock()->GetComponent<BulletFiring>()->Lanch(dir, target, lifeTimer);
    projectile.lock()->GetComponent<ProjectileTornade>()->SetTarget(target);
}

void Player::AttackCheckUI()
{
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
            DirectX::XMFLOAT3 enemyPosition = enemy.lock()->GetComponent<Transform>()->GetPosition();
            float enemyRudius = enemy.lock()->GetComponent<Collision>()->GetRadius() + 1;
            float enemyHeight = enemy.lock()->GetComponent<Collision>()->GetHeight();
            //// 衝突処理
            DirectX::XMFLOAT3 outPositon;
            // 円柱と円
            if (collision.lock()->IntersectSphereVsCylinder(
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
}

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
    float gaugeWidth = hp.lock()->GetMaxHealth() * hp.lock()->GetHealth() * 0.08f;
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
        //float positionStandardBar = 508;
        //float positionStandard = 421;
        shakeMode = false;
        //int shakeTime = 0;
        //uiHp.lock()->SetShakeTime(shakeTime);
        //uiHpBar.lock()->SetShakeTime(shakeTime);
        //uiHp.lock()->SetPosition({ uiHp.lock()->GetPosition().x ,positionStandardBar });
        //uiHpBar.lock()->SetPosition({ uiHpBar.lock()->GetPosition().x ,positionStandard });
    
    }
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
        float enemyRudius = enemy.lock()->GetComponent<Collision>()->GetRadius();
        float enemyHeight = enemy.lock()->GetComponent<Collision>()->GetHeight();
        ProjectileManager& projectileManager = ProjectileManager::Instance();
        for (int ii = 0; ii < projectileManager.GetProjectileCount(); ++ii)
        {
            std::weak_ptr<Actor> projectile = projectileManager.GetProjectile(ii);
            if (!projectile.lock()->GetComponent<ProjectileTornade>()) return;
            // 魔法位置
            DirectX::XMFLOAT3 magicPosition = projectile.lock()->GetComponent<Transform>()->GetPosition();
            float magicRadius = projectile.lock()->GetComponent<Collision>()->GetRadius();
            float magicHeight = projectile.lock()->GetComponent<Collision>()->GetHeight();
            //// 衝突処理
            DirectX::XMFLOAT3 outPositon;
            // 下半身
            // 円柱と円
            if (collision.lock()->IntersectCylinderVsCylinder(
                magicPosition,
                magicRadius,
                magicHeight,
                enemyPosition,
                enemyRudius,
                enemyHeight,
                outPositon))

            {
            // ダメージが通ったら消える。TRUEになるから
                if (enemy.lock()->GetComponent<HP>()->ApplyDamage(applyDamageSpecial, 0.5f))
                {
                    // ヒットエフェクト再生
                    {
                        hitEffect->Play(enemyPosition);
                        hitFire->Play(enemyPosition);
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
        timeElapsed = timeElapsedMin;
        return true;
    }
    timeElapsed += elapsedTime;
    return false;
}

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
        actor.reset();
    }
    players.clear();
}

