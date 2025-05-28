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
    stateMachine.reset(nullptr);
}

void Player::Start()
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // コンポーネントを使えるように
    std::shared_ptr movementId = sharedId->GetComponent<Movement>();
    std::shared_ptr hpId = sharedId->GetComponent<HP>();
    std::shared_ptr transformId = sharedId->GetComponent<Transform>();
    std::shared_ptr collisionId = sharedId->GetComponent<Collision>();
    std::shared_ptr mpId = sharedId->GetComponent<Mp>();

    // 落下停止
    bool stopFall = false;
    movementId->SetStopFall(stopFall);
    // 移動の停止
    bool stopMove = false;
    movementId->SetStopMove(stopMove);
    // 位置等
    position = transformId->GetPosition();
    angle = transformId->GetAngle();
    scale = transformId->GetScale();
    // 重力設定
    movementId->SetGravity(gravity);
    // モデルデータを入れる。
    model = sharedId->GetComponent<ModelControll>()->GetModel();
    // ヒットエフェクト読込 
    hitEffect = std::make_unique<Effect>("Data/Effect/Hit.efk");
    ImpactEffect = std::make_unique<Effect>("Data/Effect/rehleckte.efk");
    desEffect = std::make_unique<Effect>("Data/Effect/F.efk");
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
    hpId->SetHealth(health);
    // hp最大値の設定
    hpId->SetMaxHealth(maxHealth);
    // mp設定
    mpId->SetMagic(magicPoint);
    // mp最大値
    mpId->SetMaxMagic(magicPoint);
    // 半径
    collisionId->SetRadius(radius);
    // 身長
    collisionId->SetHeight(height);
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
    // 必殺技初期化斬撃
    SpecialAttack specialAttackInitialize;
    specialAttackInitialize.id = (int)SpecialAttackType::Attack;
    specialAttackInitialize.hasSkill = isSkillHave;
    specialAttack.push_back(specialAttackInitialize);
    // 必殺技初期化魔法火
    specialAttackInitialize.id = (int)SpecialAttackType::MagicFire;
    specialAttackInitialize.hasSkill = isSkillHave;
    specialAttack.push_back(specialAttackInitialize);
    // 揺れモード
    shakeMode = false;
    // 回転確認
    angleCheck = false;
    // 曲がる速度
    turnSpeedAdd = 0;
    // 攻撃ヒット回数初期化
    attackNumberSave = 0;
    // 遷移変更
    endState = false;
    // エネミーロックオン用ステート確認
    stateEnemyIndex = 0;
    // 経過時間測る用
    timeElapsed = 0.0f;
    // 空中行動制限
    areAttackTime = areAttackTimeMax;
    // エネミー接触判定
    isEnemyHit = false;
    // エネミー接触判定上半身
    isEnemyHitBody = false;
}

// 更新処理
// elapsedTime(経過時間)
void Player::Update(float elapsedTime)
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // コンポーネントを使えるように
    std::shared_ptr movementId = sharedId->GetComponent<Movement>();
    std::shared_ptr hpId = sharedId->GetComponent<HP>();
    std::shared_ptr mpId = sharedId->GetComponent<Mp>();
    std::shared_ptr transformId = sharedId->GetComponent<Transform>();

    GamePad& gamePad = Input::Instance().GetGamePad();
    //// ステート毎の処理
    stateMachine->Update(elapsedTime);
    // エフェクト位置更新
    if (areWork->GetEfeHandle())
    {
        areWork->SetPosition(areWork->GetEfeHandle(), position);
    }

    // 空中行動回数制限
    if (areAttackState <= areAttackStateEnd && !isAreAttack)
    {
        // 攻撃禁止
        isAreAttack = true;
    }

    // 地上
    if (movementId->GetOnLadius())
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
        if (movementId->GetOnLadius() || GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Death))
        {
            areWork->Stop(areWork->GetEfeHandle());
        }

        // 攻撃の時にステートを変更
        if (InputAttack() && GetSelectCheck() ==
            (int)Player::CommandAttack::Attack &&
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::QuickJab) &&
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::SideCut)&&
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::CycloneStrike)&&
                GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage) && 
                    GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Death) &&
                        !isAreAttack 
            )
        {
            // 空中行動回数
            --areAttackState;

            // ステート遷移
            GetStateMachine()->ChangeState(static_cast<int>(Player::State::QuickJab));
            // 音再生
            PlaySe("Data/Audio/SE/スラッシュ２回目.wav");
            // もし地面なら何もしない
            bool noStart = false;
            // エフェクト再生
            areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;
            areWork->Play(position);
        }

        //　魔法入力処理
        if (InputMagick() && GetSelectCheck() == (int)Player::CommandAttack::Magic &&
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::QuickJab) &&
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Magic) && 
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage) &&
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Death) && 
            !gamePad.GetButtonDownCountinue()
                )
        {
            // デバッグ
            debugInt++;

            // mpが０だったら
            if (mpId->GetMpEmpth())
            {
                // se再生
                seParam.filename = "Data/Audio/SE/魔法打てない.wav";
                seParam.volume = 1.0f;
                InputSe(seParam);
                magicAction = false;
                selectCheck = (int)CommandAttack::Attack;
                return;
            }
            // 魔法ステートに
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
        // 特殊技変更
        InputSpecialAttackChange();
        // UI必殺技演出
        SpecialPlayUlEffect(elapsedTime);
        // 攻撃範囲内なのでUI描画
        AttackCheckUI();
        // ゲージ管理
        UiControlleGauge(elapsedTime);
    }
    position = transformId->GetPosition();
    angle = transformId->GetAngle();
    scale = transformId->GetScale();
    // 無敵時間
    hpId->UpdateInbincibleTimer(elapsedTime);
    // マジック回復
    mpId->MpCharge(elapsedTime);
    //// ロックオン
    InputRockOn();
    // 与ダメエフェ
    hitEffect->SetScale(hitEffect->GetEfeHandle(),{ 1,1,1 });
    // 加速度等
    movementId->UpdateVelocity(elapsedTime);
    
    // プレイヤーと敵との衝突処理
    CollisionBornVsProjectile("body2");
    CollisionPlayerVsEnemies();
    // 弾丸当たり判定
    CollisionMagicFire();
    CollisionMagicSunder();
    CollisionMagicIce();

    // 位置更新
    transformId->UpdateTransform();
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
    // 逆再生
    case UpAnim::Reverseplayback:
    {
        // モデル逆再生アニメーション更新処理
        model->ReverseplaybackAnimation(elapsedTime, true);
        break;
    }
    }
    // 位置更新
    model->UpdateTransform(transformId->GetTransform());
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

// Se再生
void Player::InputSe(AudioParam param)
{
    Audio& Se = Audio::Instance();
    Se.Play(param);
}
// 音再生
void Player::PlaySe(const std::string& filename)
{
    Audio& Se = Audio::Instance();
    AudioParam audioParam;
    audioParam.filename = filename;
    audioParam.loop = isLoopDisabled;
    audioParam.volume = seVolume;
    Se.Play(audioParam);
}
// 音ループ再生
void Player::PlayLoopSe(const std::string& filename)
{
    Audio& Se = Audio::Instance();
    AudioParam audioParam;
    audioParam.filename = filename;
    audioParam.loop = isLoopAnim;
    audioParam.volume = seVolume;
    Se.Play(audioParam);
}
// se停止
void Player::StopSe(const std::string& filename)
{
    Audio& Se = Audio::Instance();
    // 種類停止
    Se.Stop(filename);
}
// カメラのステート管理
void Player::UpdateCameraState(float elapsedTime)
{
    CameraState oldLockonState = lockonState;
    DirectX::XMFLOAT3 oldLockonCharacter = lockonCharactor;
    lockonState = CameraState::NotLockOn;
    lockonCharactor = { 0,0,0 };

    // 通常カメラなら足元を送る。
    if (!rockCheck)
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
        return;
    }

    EnemyManager& enemyManager = EnemyManager::Instance();
    // エネミーの数
    int enemyCount = enemyManager.GetEnemyCount();
    // エネミーが一人でも生きていたら
    if (enemyCount <= 0) return;
    // エネミーの安全☑
    std::shared_ptr<Actor> enemyShader = enemyManager.GetEnemy((int)EnemyManager::EnemyType::Boss);
    if (!enemyShader) return;

    std::shared_ptr<EnemyBoss> enemyBoss = enemyShader->GetComponent<EnemyBoss>();
    std::shared_ptr<ModelControll> enemyModel = enemyShader->GetComponent<ModelControll>();

    // ロックオンモード
    Model::Node* PRock = model->FindNode("mixamorig:Spine1");
    DirectX::XMFLOAT3 pPosition;
    pPosition = enemyModel->GetModel()->ConvertLocalToWorld(PRock);
    pPosition.z *= 1.1f;
    DirectX::XMVECTOR p, t, v;
    switch (oldLockonState)
    {
        // ノーマルカメラ
    case	CameraState::NotLockOn:
    {
        // 一番近い距離のキャラクターを検索
        float	length1, length2;

        Model::Node* characterBorn = enemyModel->GetModel()->FindNode("shoulder");
        // 敵位置
        DirectX::XMFLOAT3 character;

        character = enemyModel->GetModel()->ConvertLocalToWorld(characterBorn);
        // ロックオン
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
        // ノーマル
        else
        {
            p = DirectX::XMLoadFloat3(&pPosition);
            t = DirectX::XMLoadFloat3(&character);
            v = DirectX::XMVectorSubtract(t, p);
            DirectX::XMStoreFloat(&length1, DirectX::XMVector3LengthSq(v));
            lockonCharactor = character;
            lockonState = CameraState::LockOn;
        }

        break;
    }
    // ロックオン
    case	CameraState::LockOn:
    {
        // ボーン名取得
        Model::Node* characterBorn = enemyModel->GetModel()->FindNode("body1");
        // 敵位置
        DirectX::XMFLOAT3 character;

        character = enemyModel->GetModel()->ConvertLocalToWorld(characterBorn);

        // エネミーステート
        stateEnemyIndex = enemyBoss->GetStateMachine()->GetStateIndex();
        // エネミーの特定ステートならカメラ更新しない
        if (stateEnemyIndex == (int)EnemyBoss::State::Jump ||
            stateEnemyIndex == (int)EnemyBoss::State::Attack ||
            stateEnemyIndex == (int)EnemyBoss::State::Wander
            )
        {
            lockonState = CameraState::AttackLock;
        }
        // 通常ロックオンに
        else
        {
            lockonState = CameraState::LockOn;
        }
        lockonCharactor = character;
        break;
    }
    // 敵の攻撃ロックオン
    case CameraState::AttackLock:
    {
        // 敵攻撃ロックオンステート
        lockonState = CameraState::AttackLock;

        // ボーン名取得
        Model::Node* characterBorn = enemyModel->GetModel()->FindNode("body1");

        // 敵位置
        lockonCharactor = enemyModel->GetModel()->ConvertLocalToWorld(characterBorn);
        // 敵ステート
        stateEnemyIndex = enemyBoss->GetStateMachine()->GetStateIndex();
        // 特定の行動以外なら通常ロックオンに
        if (stateEnemyIndex != (int)EnemyBoss::State::Jump &&
            stateEnemyIndex != (int)EnemyBoss::State::Attack)
        {
            lockonState = CameraState::LockOn;
        }

        break;
    }
    }
    // ステートがロックオンならカメラにモードを送る。
    if (lockonState == CameraState::LockOn)
    {
        MessageData::CAMERACHANGELOCKONMODEDATA	p = { pPosition, lockonCharactor };
        Messenger::Instance().SendData(MessageData::CAMERACHANGELOCKONMODE, &p);
    }
    // ステートが敵攻撃ロックオンならカメラにモードを送る。
    if (lockonState == CameraState::AttackLock)
    {
        MessageData::CAMERACHANGELOCKONHEIGHTMODEDATA	p = { pPosition, lockonCharactor };
        Messenger::Instance().SendData(MessageData::CAMERACHANGELOCKONTOPHEIGHTMODE, &p);
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
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;

    std::shared_ptr transformId = sharedId->GetComponent<Transform>();
    if (ImGui::Button("debugCamera"))
    {
        debugCameraTime = !debugCameraTime;

        MessageData::CAMERACHANGEMOTIONMODEDATA	p;
        float vx = sinf(angle.y) * 6;
        float vz = cosf(angle.y) * 6;
        float vx2 = sinf(angle.y) - 10;
        float vz2 = cosf(angle.y) * 7;
        float vx3 = sinf(angle.y);
        p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
        p.data.push_back({ 50, {position.x + vx, position.y + 3, position.z + vz }, position });
        p.data.push_back({ 150, {position.x - vx2, position.y + 5, position.z - vz2 }, position });
        p.data.push_back({ 200, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
        p.data.push_back({ 250, {position.x + vx3 , position.y + 1, (position.z + 0.1f) - vz2 }, position });

        Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);

    }
    if (debugCameraTime)
    {

        //MessageData::CAMERACHANGEMOTIONMODEDATA	p;
        //// モーション記録
        //float vx = sinf(angle.y) * 6;
        //float vz = cosf(angle.y) * 6;
        //float vx2 = sinf(angle.y) - 10;
        //float vz2 = cosf(angle.y) * 7;
        //float vx3 = sinf(angle.y);
        //p.data.push_back({ 0, {position.x + vx, position.y + 3, position.z + vz }, position });
        //p.data.push_back({ 50, {position.x + vx, position.y + 3, position.z + vz }, position });
        //p.data.push_back({ 100, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
        //p.data.push_back({ 150, {position.x - vx2, position.y + 5, position.z - vz2 }, position });
        //p.data.push_back({ 170, {position.x + vx, position.y + 3, position.z + vz }, position });
        //p.data.push_back({ 200, {position.x + vx2, position.y + 5, position.z - vz2 }, position });
        //p.data.push_back({ 250, {position.x + vx3 , position.y + 1, (position.z + 0.1f) - vz2 }, position });

        //Messenger::Instance().SendData(MessageData::CAMERACHANGEMOTIONMODE, &p);
    }
    if (ImGui::Button("debugShader"))
    {
        debugShaderFlash = !debugShaderFlash;
    } 
    if (ImGui::Button("debugShaderSeconde"))
    {
        debugShaderFlashSeconde = !debugShaderFlashSeconde;
    }
    if (debugShaderFlash)
    {
        PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
        ColorGradingData debugColorGradingData;
        colorGradingData.brigthness = 5.5f;
        postprocessingRenderer.SetColorGradingMaxData(colorGradingData);
    }
    if (debugShaderFlashSeconde)
    {
        PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
        ColorGradingData debugColorGradingData;
        debugColorGradingData.saturation = 0.0f;
        postprocessingRenderer.SetColorGradingMinData(debugColorGradingData);
    }
    if (ImGui::Button("Sound"))
    {
        // se再生
        Audio& Se = Audio::Instance();
        AudioParam audioParam;
        audioParam.filename = "Data/Audio/SE/必殺技雷.wav";
        audioParam.loop = isLoopDisabled;
        audioParam.volume = seVolume;
        Se.Play(audioParam);
    }

    ImGui::InputInt("areAttackState", &areAttackState);
    ImGui::InputFloat("Move Speed", &moveSpeed);
    ImGui::SliderFloat3("angle", &angle.x, -10, 10);
    transformId->SetAngle(angle);
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
        position = transformId->GetPosition();
        angle = transformId->GetAngle();
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
bool Player::InputMove()
{
    // 入力情報を取得
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();
    // x軸の移動感知  特定の動作中移動禁止
    if (ax + FLT_EPSILON != isInputEmpty + FLT_EPSILON &&
        GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage) &&
        GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Death))
    {
        
        return true;
    }
    // ｙ軸の移動感知　特定の動作中移動禁止
    if (ay + FLT_EPSILON != isInputEmpty + FLT_EPSILON &&
        GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Damage) &&
        GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Death))
    {

        return true;
    }
    return false;
}
// ロックオン状態の確認
bool Player::InputRockOn()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    // ロックオン中
    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT_SHOULDER  && !rockCheck)
    {
        rockCheck = true;
        return true;
    }
    // ロックオンオフ
    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT_SHOULDER && rockCheck)
    {
        rockCheck = false;
        return true;
    }
    return false;
}
// ロックオンUIを表示
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
    std::shared_ptr<Ui> uiIdSight;
    std::shared_ptr<TransForm2D> uiIdSightTransform;
    std::shared_ptr<Ui> uiIdSightMove;
    std::shared_ptr<TransForm2D> uiIdSightMoveTransform;
    int uiCount = UiManager::Instance().GetUiesCount();
    if (uiCount <= uiCountMax) return;
    // ロックオンしてるか 安全チェック
    auto sharedUiSightId = UiManager::Instance().GetUies((int)UiManager::UiCount::Sight);
    if (!sharedUiSightId)
        return;
    uiIdSight = sharedUiSightId->GetComponent<Ui>();
    uiIdSightTransform = sharedUiSightId->GetComponent<TransForm2D>();
    // 攻撃届くか 安全チェック
    auto sharedUiMoveId = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck);
    if (!sharedUiMoveId)
        return;

    // ロックオン有無
    uiIdSightMove = sharedUiMoveId->GetComponent<Ui>();
    uiIdSightMoveTransform = sharedUiMoveId->GetComponent<TransForm2D>();
    // 全ての敵の頭上にHPゲージを表示

    EnemyManager& enemyManager = EnemyManager::Instance();
    // エネミーの数
    int enemyCount = enemyManager.GetEnemyCount();
    // エネミーが一人でも生きていたら
    if (enemyCount <= 0) return;
    // エネミーの安全☑
    std::shared_ptr<Actor> enemyShader = enemyManager.GetEnemy((int)EnemyManager::EnemyType::Boss);
    if (!enemyShader) return;

    std::shared_ptr<EnemyBoss> enemyBoss = enemyShader->GetComponent<EnemyBoss>();
    std::shared_ptr<ModelControll> enemyModel = enemyShader->GetComponent<ModelControll>();

    Model::Node* characterBorn = enemyModel->GetModel()->FindNode("shoulder");
    // エネミー腰位置
    DirectX::XMFLOAT3 enemyPosition;

    enemyPosition = enemyModel->GetModel()->ConvertLocalToWorld(characterBorn);

    // ワールドからスクリーン
    DirectX::XMVECTOR enemyPositionVe = DirectX::XMLoadFloat3(&enemyPosition);
    // ゲージ描画 // ワールドからスクリーン
    DirectX::XMVECTOR screenPositionVe = DirectX::XMVector3Project(
        enemyPositionVe,
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
    DirectX::XMStoreFloat3(&scereenPosition, screenPositionVe);
    // 必殺技がでていなかったらロックオン
    if (rockCheck || !specialRockOff)
    {
        uiIdSight->SetDrawCheck(isDrawUi);
    }
    // 必殺技中ロックオン系UIを消す。
    if (scereenPosition.z < 0.0f || scereenPosition.z > 1.0f || !rockCheck || specialRockOff)
    {
        uiIdSight->SetDrawCheck(isDrawUiEmpth);
        uiIdSightMove->SetDrawCheck(isDrawUiEmpth);
        return;
    }
    // 2Dスプライト描画
    {
        uiIdSightTransform->SetPosition(
            {
                scereenPosition.x,
                scereenPosition.y
            });
        uiIdSightMoveTransform->SetPosition(
            {
                scereenPosition.x - scereenPositionOffset.x,
                scereenPosition.y - scereenPositionOffset.y
            });
    }
}

// コマンド　攻撃種類方法選択
bool Player::InputSelectCheck()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    // コマンド移動上 魔法選択していなくて　必殺技じゃ無かったら
    if (gamePad.GetButtonDown() & GamePad::BTN_UP && !magicAction && !specialAction)
    {
        --selectCheck;
    }
    // コマンド移動下 魔法選択していなくて　必殺技じゃ無かったら
    if (gamePad.GetButtonDown() & GamePad::BTN_DOWN && !magicAction && !specialAction)
    {
        ++selectCheck;
    }
    // コマンド操作 魔法選択
    if (gamePad.GetButtonUp() & GamePad::BTN_B && selectCheck == (int)CommandAttack::Magic && !magicAction)
    {
        magicAction = true;
        bool isPush = true;
        gamePad.SetButtonDownCountinue(isPush);
        return true;
    }
    // コマンド操作 必殺技
    if (gamePad.GetButtonDown() & GamePad::BTN_B && selectCheck == (int)CommandAttack::Special && !specialAction
        || gamePad.GetButtonDown() & GamePad::BTN_RIGHT && selectCheck == (int)CommandAttack::Special)
    {
        specialAction = true;
        bool isPush = true;
        gamePad.SetButtonDownCountinue(isPush);
        return true;
    }
    // コマンド操作を　全体に
    if (gamePad.GetButtonDown() & GamePad::BTN_LEFT && selectCheck == (int)CommandAttack::Special)
    {
        specialAction = false;
    }
    // コマンド選択 必殺技
    if (specialAction)
    {
        // 安全チェック コマンド斬撃
        auto sharedUiComandoAttackId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu);
        if (!sharedUiComandoAttackId)
            return false;
        // コマンド　必殺技斬撃表示するか
        std::shared_ptr<Ui> uiIdSpecialComandoAttack = sharedUiComandoAttackId->GetComponent<Ui>();
        // 安全チェック コマンド魔法炎
        auto sharedUiCommandSpecialFireId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame);
        if (!sharedUiCommandSpecialFireId)
            return false;
        // コマンド　必殺技魔法炎表示するか
        std::shared_ptr<Ui> uiIdSpecialComandoFire = sharedUiCommandSpecialFireId->GetComponent<Ui>();

        // 安全チェック 特殊技UI選択不可
        auto sharedUiCommandDisabled01Id = UiManager::Instance().GetUies((int)UiManager::UiCount::CommandDisabled01);
        if (!sharedUiCommandDisabled01Id)
            return false;
        // 特殊技UI選択不可 表示するか
        std::shared_ptr<Ui> uiIdIsCommandDisabledAttack = sharedUiCommandDisabled01Id->GetComponent<Ui>();
        // 安全チェック  特殊技UI選択不可
        auto sharedUiCommandDisabled02Id = UiManager::Instance().GetUies((int)UiManager::UiCount::CommandDisabled02);
        if (!sharedUiCommandDisabled02Id)
            return false;
        std::shared_ptr<Ui> uiIdIsCommandDisabledFire = sharedUiCommandDisabled02Id->GetComponent<Ui>();
        uiIdSpecialComandoAttack->SetDrawCheck(isDrawUi);
        uiIdSpecialComandoFire->SetDrawCheck(isDrawUi);

        // 特殊技UI選択不可 斬撃
        if (specialAttack.at((int)SpecialAttackType::Attack).hasSkill)
            uiIdIsCommandDisabledAttack->SetDrawCheck(isDrawUiEmpth);
        // 特殊技UI選択不可　斬撃　非表示
        else
            uiIdIsCommandDisabledAttack->SetDrawCheck(isDrawUi);
        // 特殊技UI選択不可 魔法炎
        if (specialAttack.at((int)SpecialAttackType::MagicFire).hasSkill)
            uiIdIsCommandDisabledFire->SetDrawCheck(isDrawUiEmpth);
        // 特殊技UI選択不可　魔法炎　非表示
        else
            uiIdIsCommandDisabledFire->SetDrawCheck(isDrawUi);
    }
    // コマンド非選択 必殺技
    else
    {
        // 安全チェック コマンド斬撃
        auto sharedUiComandoAttackId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu);
        if (!sharedUiComandoAttackId)
            return false;
        std::shared_ptr<Ui> uiIdAttack = sharedUiComandoAttackId->GetComponent<Ui>();
        // 安全チェック コマンド魔法炎
        auto sharedUiComandoFireId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame);
        if (!sharedUiComandoFireId)
            return false;
        std::shared_ptr<Ui> uiIdAttackCheck = sharedUiComandoFireId->GetComponent<Ui>();

        // 安全チェック 特殊技UI選択不可
        auto sharedUiCommandDisabled01Id = UiManager::Instance().GetUies((int)UiManager::UiCount::CommandDisabled01);
        if (!sharedUiCommandDisabled01Id)
            return false;
        std::shared_ptr<Ui> uiIdIsCommandDisabledAttack = sharedUiCommandDisabled01Id->GetComponent<Ui>();
        // 安全チェック 特殊技UI選択不可
        auto sharedUiCommandDisabled02Id = UiManager::Instance().GetUies((int)UiManager::UiCount::CommandDisabled02);
        if (!sharedUiCommandDisabled02Id)
            return false;
        std::shared_ptr<Ui> uiIdIsCommandDisabledFire = sharedUiCommandDisabled02Id->GetComponent<Ui>();
        uiIdAttack->SetDrawCheck(isDrawUiEmpth);
        uiIdAttackCheck->SetDrawCheck(isDrawUiEmpth);

        uiIdIsCommandDisabledAttack->SetDrawCheck(isDrawUiEmpth);
        uiIdIsCommandDisabledFire->SetDrawCheck(isDrawUiEmpth);
    }

    // 一度離すまでボタン効かない
    if (gamePad.GetButtonUp() & GamePad::BTN_B)
    {
        bool isPush = false;
        gamePad.SetButtonDownCountinue(isPush);
    }
    // コマンド操作 選択　魔法
    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT && selectCheck == (int)CommandAttack::Magic)
    {
        magicAction = true;
        return true;
    }
    // ループ操作 コマンド必殺技より下なら
    if (selectCheck > (int)CommandAttack::Special)
    {
        selectCheck = (int)CommandAttack::Attack;
    }
    // ループ操作　コマンド攻撃より上なら
    if (selectCheck < (int)CommandAttack::Attack)
    {
        selectCheck = (int)CommandAttack::Special;
    }
    int uiCount = UiManager::Instance().GetUiesCount();
    // ui無かったら
    if (uiCount <= uiCountMax) return false;
    
    // コマンドUI　選択中 攻撃選ぶ
    if (selectCheck == (int)CommandAttack::Attack)
    {
        // 安全チェック　コマンドUI　攻撃
        auto sharedUiComandoAttackId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttack);
        if (!sharedUiComandoAttackId)
            return false;
        std::shared_ptr<Ui> uiIdAttack = sharedUiComandoAttackId->GetComponent<Ui>();
        // 安全チェック コマンドUI　攻撃選択中
        auto sharedUiComandoAttackCheckId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttackCheck);
        if (!sharedUiComandoAttackCheckId)
            return false;
        std::shared_ptr<Ui> uiIdAttackCheck = sharedUiComandoAttackCheckId->GetComponent<Ui>();
        // 　コマンドUI　攻撃 非表示
        uiIdAttack->SetDrawCheck(isDrawUiEmpth);
        // コマンドUI　攻撃選択中　表示
        uiIdAttackCheck->SetDrawCheck(isDrawUi);
    }
    // コマンドUI　非選択 攻撃
    else
    {
        // 安全チェック　コマンドUI　攻撃
        auto sharedUiComandoAttackId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttack);
        if (!sharedUiComandoAttackId)
            return false;
        std::shared_ptr<Ui> uiIdAttack = sharedUiComandoAttackId->GetComponent<Ui>();
        // 安全チェック コマンドUI　攻撃選択中
        auto sharedUiComandoAttackCheckId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttackCheck);
        if (!sharedUiComandoAttackCheckId)
            return false;
        std::shared_ptr<Ui> uiIdAttackCheck = sharedUiComandoAttackCheckId->GetComponent<Ui>();
        // 　コマンドUI　攻撃 表示
        uiIdAttack->SetDrawCheck(isDrawUi);
        // コマンドUI　攻撃選択中　非表示
        uiIdAttackCheck->SetDrawCheck(isDrawUiEmpth);
    }
    //  コマンドUI　選択中 魔法選んだ時
    if (selectCheck == (int)CommandAttack::Magic)
    {
        // 安全チェック コマンドUI　魔法
        auto sharedUiComandoMagickId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagick);
        if (!sharedUiComandoMagickId)
            return false;
        std::shared_ptr<Ui> uiIdMagick = sharedUiComandoMagickId->GetComponent<Ui>();
        // 安全チェック コマンドUI　魔法選択中
        auto sharedUiComandoMagickCheckId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagickCheck);
        if (!sharedUiComandoMagickCheckId)
            return false;
        std::shared_ptr<Ui> uiIdMagickCheck = sharedUiComandoMagickCheckId->GetComponent<Ui>();
        // コマンドUI　魔法 非表示
        uiIdMagick->SetDrawCheck(isDrawUiEmpth);
        // コマンドUI　魔法 表示
        uiIdMagickCheck->SetDrawCheck(isDrawUi);
    }
    //  コマンドUI　非選択 魔法
    else
    {
        // 安全チェック コマンドUI　魔法
        auto sharedUiComandoMagickId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagick);
        if (!sharedUiComandoMagickId)
            return false;
        std::shared_ptr<Ui> uiIdMagick = sharedUiComandoMagickId->GetComponent<Ui>();
        // 安全チェック コマンドUI　魔法選択中
        auto sharedUiComandoMagickCheckId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagickCheck);
        if (!sharedUiComandoMagickCheckId)
            return false;
        std::shared_ptr<Ui> uiIdMagickCheck = sharedUiComandoMagickCheckId->GetComponent<Ui>();
        // コマンドUI　魔法 表示
        uiIdMagick->SetDrawCheck(isDrawUi);
        // コマンドUI　魔法 非表示
        uiIdMagickCheck->SetDrawCheck(isDrawUiEmpth);
    }
    // コマンドUI　選択中 必殺技選ぶ
    if (selectCheck == (int)CommandAttack::Special)
    {
        // 安全チェック コマンドUI　必殺技
        auto sharedUiSpecialId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpecial);
        if (!sharedUiSpecialId)
            return false;
        std::shared_ptr<Ui> uiIdSpecial = sharedUiSpecialId->GetComponent<Ui>();
        // 安全チェック コマンドUI　必殺技選択中
        auto sharedUiSpecialCheckId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpecialCheck);
        if (!sharedUiSpecialId)
            return false;
        std::shared_ptr<Ui> uiIdSpecialCheck = sharedUiSpecialCheckId->GetComponent<Ui>();
        // コマンドUI　必殺技 非表示
        uiIdSpecial->SetDrawCheck(isDrawUiEmpth);
        // コマンドUI　必殺技 表示
        uiIdSpecialCheck->SetDrawCheck(isDrawUi);
    }
    //  コマンドUI　非選択 必殺技
    else
    {
        // 安全チェック コマンドUI　必殺技
        auto sharedUiSpecialId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpecial);
        if (!sharedUiSpecialId)
            return false;
        std::shared_ptr<Ui> uiIdSpecial = sharedUiSpecialId->GetComponent<Ui>();
        // 安全チェック コマンドUI　必殺技選択中
        auto sharedUiSpecialCheckId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpecialCheck);
        if (!sharedUiSpecialCheckId)
            return false;
        std::shared_ptr<Ui> uiIdSpecialCheck = sharedUiSpecialCheckId->GetComponent<Ui>();
        // コマンドUI　必殺技 表示
        uiIdSpecial->SetDrawCheck(isDrawUi);
        // コマンドUI　必殺技 非表示
        uiIdSpecialCheck->SetDrawCheck(isDrawUiEmpth);
    }

    return false;
}

// 魔法のコマンド
bool Player::InputSelectMagicCheck()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    // 魔法コマンド　上に
    if (gamePad.GetButtonDown() & GamePad::BTN_UP && magicAction)
    {
        --selectMagicCheck;
    }
    // 魔法コマンド　下に
    if (gamePad.GetButtonDown() & GamePad::BTN_DOWN && magicAction)
    {
        ++selectMagicCheck;
    }
    // 魔法コマンド　全体コマンド操作に
    if (gamePad.GetButtonDown() & GamePad::BTN_LEFT && magicAction)
    {
        magicAction = false;
    }
    // コマンドに入ると必ず炎に
    if (selectMagicCheck <= (int)CommandMagic::Normal && magicAction)
    {

        selectMagicCheck = (int)CommandMagic::Fire;
    }
    // 魔法選択を　ケアルより下にすると
    if (selectMagicCheck > (int)CommandMagic::Heale && magicAction)
    {
        // 炎に
        selectMagicCheck = (int)CommandMagic::Fire;
    }
    // 魔法選択を　ファイアより上にすると
    if (selectMagicCheck < (int)CommandMagic::Fire && magicAction)
    {
        // 炎に
        selectMagicCheck = (int)CommandMagic::Heale;
    }
    ///////////////////////////
    // ショートカットキー

    // ボタンを離したら選択解除
    if (gamePad.GetButtonUp())
    {
        // 安全チェック UIコマンド選択
        auto sharedUiPushId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push)->GetComponent<Ui>();
        if (!sharedUiPushId)
            return false;
        std::shared_ptr<Ui> uiIdPush = sharedUiPushId;

        // UIコマンド選択　非表示
        uiIdPush->SetDrawCheck(isDrawUiEmpth);
    }
    // ショートカットキー炎選択
    if (InputShortCutkeyMagic() &&
        gamePad.GetButtonDown() & GamePad::BTN_B )
    {
        // 魔法の種類を炎に
        selectMagicCheck = (int)CommandMagic::Fire;
        // 安全チェック  UIコマンド選択
        auto sharedUiPushId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push);
        if (!sharedUiPushId)
            return false;
        std::shared_ptr<Ui> uiIdAttackCheck = sharedUiPushId->GetComponent<Ui>();
        //　UIコマンド　選択中　位置
        std::shared_ptr<TransForm2D> uiIdAttackCheckPos = sharedUiPushId->GetComponent<TransForm2D>();
        
        // 安全チェック　コマンドショートカット　魔法炎の位置
        auto sharedUiCommandShortCutFireId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutFire);
        if (!sharedUiCommandShortCutFireId)
            return false;

        std::shared_ptr<TransForm2D> uiIdFireShortCutCheckPos = sharedUiCommandShortCutFireId->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = uiIdFireShortCutCheckPos->GetPosition();
        // UIコマンド 選択 表示
        uiIdAttackCheck->SetDrawCheck(isDrawUi);
        // UIコマンド　魔法炎の位置に選択を移動
        uiIdAttackCheckPos->SetPosition(pos);
    }
    // ショートカットキー雷選択
    if (InputShortCutkeyMagic() &&
        gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        // 魔法の種類を雷に
        selectMagicCheck = (int)CommandMagic::Thander;
        // 安全チェック  UIコマンド選択
        auto sharedUiPushId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push);
        if (!sharedUiPushId)
            return false;

        std::shared_ptr<Ui> uiIdAttackCheck = sharedUiPushId->GetComponent<Ui>();
        std::shared_ptr<TransForm2D> uiIdAttackCheckPos = sharedUiPushId->GetComponent<TransForm2D>();
        // ショートカット魔法雷の位置
        // 安全チェック
        auto sharedUiCommandShortCutSunderId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutSunder);
        if (!sharedUiCommandShortCutSunderId)
            return false;
        std::shared_ptr<TransForm2D> uiIdSunderShortCutCheckPos = sharedUiCommandShortCutSunderId->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = uiIdSunderShortCutCheckPos->GetPosition();
        // UIコマンド 選択 表示
        uiIdAttackCheck->SetDrawCheck(isDrawUi);
        // UIコマンド　魔法雷の位置に選択を移動
        uiIdAttackCheckPos->SetPosition(pos);
    }
    // ショートカットキー回復選択
    if (InputShortCutkeyMagic() &&
        gamePad.GetButtonDown() & GamePad::BTN_Y )
    {
        // 魔法の種類を回復に
        selectMagicCheck = (int)CommandMagic::Heale;
        // 安全チェック  UIコマンド選択
        auto sharedUiPushId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push);
        if (!sharedUiPushId)
            return false;
        std::shared_ptr<Ui> uiIdAttackCheck = sharedUiPushId->GetComponent<Ui>();
        std::shared_ptr<TransForm2D> uiIdAttackCheckPos = sharedUiPushId->GetComponent<TransForm2D>();
        // 安全チェック ショートカット魔法回復
        auto sharedUiCommandShortCutKeuleId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutKeule);
        if (!sharedUiPushId)
            return false;
        std::shared_ptr<TransForm2D> uiIdHealeShortCutCheckPos = sharedUiCommandShortCutKeuleId->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = uiIdHealeShortCutCheckPos->GetPosition();
        // UIコマンド 選択 表示
        uiIdAttackCheck->SetDrawCheck(isDrawUi);
        // UIコマンド　魔法回復の位置に選択を移動
        uiIdAttackCheckPos->SetPosition(pos);
    }
    // ショートカットキー氷選択
    if (InputShortCutkeyMagic() &&
        gamePad.GetButtonDown() & GamePad::BTN_A )
    {
        // 魔法の種類を氷に
        selectMagicCheck = (int)CommandMagic::Ice;
        // 安全チェック  UIコマンド選択
        auto sharedUiPushId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::Push);
        if (!sharedUiPushId)
            return false;
        std::shared_ptr<Ui> uiIdAttackCheck = sharedUiPushId->GetComponent<Ui>();
        std::shared_ptr<TransForm2D> uiIdAttackCheckPos = sharedUiPushId->GetComponent<TransForm2D>();
       // 安全チェック ショートカット魔法氷
        auto sharedUiCommandShortCutIceId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutIce);
        if (!sharedUiCommandShortCutIceId)
            return false;
        std::shared_ptr<TransForm2D> uiIdIceShortCutCheckPos = sharedUiCommandShortCutIceId->GetComponent<TransForm2D>();
        DirectX::XMFLOAT2 pos = uiIdIceShortCutCheckPos->GetPosition();
        // UIコマンド 選択 表示
        uiIdAttackCheck->SetDrawCheck(isDrawUi);
        // UIコマンド　魔法氷の位置に選択を移動
        uiIdAttackCheckPos->SetPosition(pos);
    }
    /////////////////////////

    // コマンド魔法選択では無かったら
    if (!magicAction)
    {
        selectMagicCheck = (int)CommandMagic::Normal;
    }
    int uiCount = UiManager::Instance().GetUiesCount();
    // ui無かったら
    if (uiCount <= uiCountMax) return false;
    // UI設定 コマンド選択　魔法　炎 
    if (selectMagicCheck == (int)CommandMagic::Fire && magicAction&&
        !InputShortCutkeyMagic())
    {
        // 安全チェック　コマンド　魔法　火
        auto sharedUiCommandFireId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFire);
        // 安全チェック　コマンド選択　魔法　火
        auto sharedUiCommandFireCheckId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFireCheck);
        if (!sharedUiCommandFireId && !sharedUiCommandFireCheckId)
            return false;
        std::weak_ptr<Ui> uiCommandFire = sharedUiCommandFireId->GetComponent<Ui>();
        std::weak_ptr<Ui> uiCommandFireCheck = sharedUiCommandFireCheckId->GetComponent<Ui>();
        
        // UIコマンド 魔法　火 選択 非表示
        uiCommandFire.lock()->SetDrawCheck(isDrawUiEmpth);
        // UIコマンド選択　魔法　火 表示
        uiCommandFireCheck.lock()->SetDrawCheck(isDrawUi);
    }
    // UI設定 コマンド　魔法　炎 
    else
    {
        // 安全チェック　コマンド　魔法　火
        auto sharedUiCommandFireId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFire);
        // 安全チェック　コマンド選択　火　魔法　
        auto sharedUiCommandFireCheckId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFireCheck);
        if (!sharedUiCommandFireId && !sharedUiCommandFireCheckId)
            return false;
        std::shared_ptr<Ui> uiCommandFire = sharedUiCommandFireId->GetComponent<Ui>();
        std::shared_ptr<Ui> uiCommandFireCheck = sharedUiCommandFireCheckId->GetComponent<Ui>();
        // UIコマンド 魔法　火 選択 表示
        uiCommandFire->SetDrawCheck(isDrawUi);
        // UIコマンド選択　魔法　火 非表示
        uiCommandFireCheck->SetDrawCheck(isDrawUiEmpth);
    }
    // 魔法を発動していなかったら　ショートカットなら　十字キー操作UI解除
    if (!magicAction ||
        InputShortCutkeyMagic())
    {
        // 安全チェック　コマンド　魔法　火
        auto sharedUiCommandFireId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFire);
        // 安全チェック　コマンド選択　火　魔法　
        auto sharedUiCommandFireCheckId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFireCheck);
        if (!sharedUiCommandFireId && !sharedUiCommandFireCheckId)
            return false;
        std::shared_ptr<Ui> uiCommandFire = sharedUiCommandFireId->GetComponent<Ui>();
        std::shared_ptr<Ui> uiCommandFireCheck = sharedUiCommandFireCheckId->GetComponent<Ui>();
        // UIコマンド 魔法　火 選択 非表示
        uiCommandFire->SetDrawCheck(isDrawUiEmpth);
        // UIコマンド選択　魔法　火 非表示
        uiCommandFireCheck->SetDrawCheck(isDrawUiEmpth);
    }
    // UI設定 コマンド選択　魔法　雷 
    if (selectMagicCheck == (int)CommandMagic::Thander && magicAction &&
        !InputShortCutkeyMagic())
    {
        // 安全チェック　コマンド　魔法　雷
        auto sharedUiCommandRigtningId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtning);
        // 安全チェック　コマンド選択　魔法　雷
        auto sharedUiCommandRigtningCheckId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtningCheck);
        if (!sharedUiCommandRigtningId && !sharedUiCommandRigtningCheckId)
            return false;
        std::shared_ptr<Ui> uiIdCommandRightning = sharedUiCommandRigtningId->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdCommandRightningCheck = sharedUiCommandRigtningCheckId->GetComponent<Ui>();
        // UIコマンド 魔法　雷 選択 非表示
        uiIdCommandRightning->SetDrawCheck(isDrawUiEmpth);
        // UIコマンド選択　魔法　雷 表示
        uiIdCommandRightningCheck->SetDrawCheck(isDrawUi);
    }
    else
    {
        // 安全チェック　コマンド　魔法　雷
        auto sharedUiCommandRigtningId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtning);
        // 安全チェック　コマンド選択　魔法　雷
        auto sharedUiCommandRigtningCheckId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtningCheck);
        if (!sharedUiCommandRigtningId && !sharedUiCommandRigtningCheckId)
            return false;
        std::shared_ptr<Ui> uiIdCommandRightning = sharedUiCommandRigtningId->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdCommandRightningCheck = sharedUiCommandRigtningCheckId->GetComponent<Ui>();
        // UIコマンド 魔法　雷 選択 表示
        uiIdCommandRightning->SetDrawCheck(isDrawUi);
        // UIコマンド選択　魔法　雷 非表示
        uiIdCommandRightningCheck->SetDrawCheck(isDrawUiEmpth);
    }
    // 魔法を発動していなかったら　ショートカットなら　十字キー操作UI解除
    if (!magicAction || 
        InputShortCutkeyMagic())
    {
        // 安全チェック　コマンド　魔法　雷
        auto sharedUiCommandRigtningId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtning);
        // 安全チェック　コマンド選択　魔法　雷
        auto sharedUiCommandRigtningCheckId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtningCheck);
        if (!sharedUiCommandRigtningId && !sharedUiCommandRigtningCheckId)
            return false;
        std::shared_ptr<Ui> uiIdCommandRightning = sharedUiCommandRigtningId->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdCommandRightningCheck = sharedUiCommandRigtningCheckId->GetComponent<Ui>();
        // UIコマンド 魔法　雷 選択 非表示
        uiIdCommandRightning->SetDrawCheck(isDrawUiEmpth);
        // UIコマンド選択　魔法　雷 非表示
        uiIdCommandRightningCheck->SetDrawCheck(isDrawUiEmpth);
    }
    // UI設定 氷
    if (selectMagicCheck == (int)CommandMagic::Ice && magicAction &&
        !InputShortCutkeyMagic())
    {
        // 安全チェック　コマンド　魔法　氷
        auto sharedUiCommandIceId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIce);
        // 安全チェック　コマンド選択　魔法　氷
        auto sharedUiCommandIceCheckId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIceCheck);
        if (!sharedUiCommandIceId && !sharedUiCommandIceCheckId)
            return false;
        std::shared_ptr<Ui> uiIdCommandIce = sharedUiCommandIceId->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdCommandIceCheck = sharedUiCommandIceCheckId->GetComponent<Ui>();
        // UIコマンド 魔法　氷 選択 非表示
        uiIdCommandIce->SetDrawCheck(isDrawUiEmpth);
        // UIコマンド選択　魔法　氷 表示
        uiIdCommandIceCheck->SetDrawCheck(isDrawUi);
    }
    else
    {
        // 安全チェック　コマンド　魔法　氷
        auto sharedUiCommandIceId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIce);
        // 安全チェック　コマンド選択　魔法　氷
        auto sharedUiCommandIceCheckId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIceCheck);
        if (!sharedUiCommandIceId && !sharedUiCommandIceCheckId)
            return false;
        std::shared_ptr<Ui> uiIdCommandIce = sharedUiCommandIceId->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdCommandIceCheck = sharedUiCommandIceCheckId->GetComponent<Ui>();
        // UIコマンド 魔法　氷 選択 表示
        uiIdCommandIce->SetDrawCheck(isDrawUi);
        // UIコマンド選択　魔法　氷 非表示
        uiIdCommandIceCheck->SetDrawCheck(isDrawUiEmpth);
    }
    // 魔法を発動していなかったら　ショートカットなら　十字キー操作UI解除
    if (!magicAction || 
        InputShortCutkeyMagic())
    {
        // 安全チェック　コマンド　魔法　氷
        auto sharedUiCommandIceId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIce);
        // 安全チェック　コマンド選択　魔法　氷
        auto sharedUiCommandIceCheckId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIceCheck);
        if (!sharedUiCommandIceId && !sharedUiCommandIceCheckId)
            return false;
        std::shared_ptr<Ui> uiIdCommandIce = sharedUiCommandIceId->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdCommandIceCheck = sharedUiCommandIceCheckId->GetComponent<Ui>();
        // UIコマンド 魔法　氷 選択 非表示
        uiIdCommandIce->SetDrawCheck(isDrawUiEmpth);
        // UIコマンド選択　魔法　氷 非表示
        uiIdCommandIceCheck->SetDrawCheck(isDrawUiEmpth);
    }
    // UI設定 回復
    if (selectMagicCheck == (int)CommandMagic::Heale && magicAction &&
        !InputShortCutkeyMagic())
    {
        // 安全チェック　コマンド　魔法　回復
        auto sharedUiCommandHealeId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandHeale);
        // 安全チェック　コマンド選択　魔法　回復
        auto sharedUiCommandHealeCheckId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandHealeCheck);
        if (!sharedUiCommandHealeId && !sharedUiCommandHealeCheckId)
            return false;
        std::shared_ptr<Ui> uiIdCommandHeale = sharedUiCommandHealeId->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdCommandHealeCheck = sharedUiCommandHealeCheckId->GetComponent<Ui>();
        // UIコマンド 魔法　回復 選択 非表示
        uiIdCommandHeale->SetDrawCheck(isDrawUiEmpth);
        // UIコマンド選択　魔法　回復 表示
        uiIdCommandHealeCheck->SetDrawCheck(isDrawUi);
    }
    else
    {
        // 安全チェック　コマンド　魔法　回復
        auto sharedUiCommandHealeId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandHeale);
        // 安全チェック　コマンド選択　魔法　回復
        auto sharedUiCommandHealeCheckId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandHealeCheck);
        if (!sharedUiCommandHealeId && !sharedUiCommandHealeCheckId)
            return false;
        std::shared_ptr<Ui> uiIdCommandHeale = sharedUiCommandHealeId->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdCommandHealeCheck = sharedUiCommandHealeCheckId->GetComponent<Ui>();
        // UIコマンド 魔法　回復 選択 表示
        uiIdCommandHeale->SetDrawCheck(isDrawUi);
        // UIコマンド選択　魔法　回復 非表示
        uiIdCommandHealeCheck->SetDrawCheck(isDrawUiEmpth);
    }
    // 魔法を発動していなかったら　ショートカットなら　十字キー操作UI解除
    if (!magicAction ||
        InputShortCutkeyMagic())
    {
        // 安全チェック　コマンド　魔法　回復
        auto sharedUiCommandHealeId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandHeale);
        // 安全チェック　コマンド選択　魔法　回復
        auto sharedUiCommandHealeCheckId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandHealeCheck);
        if (!sharedUiCommandHealeId && !sharedUiCommandHealeCheckId)
            return false;
        std::shared_ptr<Ui> uiIdCommandHeale = sharedUiCommandHealeId->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdCommandHealeCheck = sharedUiCommandHealeCheckId->GetComponent<Ui>();
        // UIコマンド 魔法　回復 選択 非表示
        uiIdCommandHeale->SetDrawCheck(isDrawUiEmpth);
        // UIコマンド選択　魔法　回復 非表示
        uiIdCommandHealeCheck->SetDrawCheck(isDrawUiEmpth);
    }
    return false;
}

// L1 ショートカットキー
bool Player::InputShortCutkeyMagic()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    // L1 ショートカット魔法
    if (gamePad.GetButtonDown() & GamePad::BTN_LEFT_SHOULDER)
    {
        specialAction = false;
        selectCheck = (int)CommandAttack::Magic;
        selectMagicCheck = (int)CommandMagic::Fire;
        magicAction = true;
        return true;
    }
    // ショートカット押してる間 魔法コマンド内を見ます。
    if (gamePad.GetButton() & GamePad::BTN_LEFT_SHOULDER)
    {
        // 魔法コマンド入力
        magicAction = true;

        // 火コマンド
        {
            // 安全チェック コマンド火　非選択
            auto sharedUiCommandShortCutFireId = UiManager::Instance().GetUies(
                (int)UiManager::UiCount::PlayerCommandShortCutFire);
            if (!sharedUiCommandShortCutFireId)
                return false;

            std::shared_ptr<Ui> uiIdAttackCheckFire = sharedUiCommandShortCutFireId->GetComponent<Ui>();
            // 安全チェック コマンド火　非選択 表示
            uiIdAttackCheckFire->SetDrawCheck(isDrawUi);
        }

        // 雷コマンド
        {
            // 安全チェック コマンド雷　非選択
            auto sharedUiCommandShortCutSunderId = UiManager::Instance().GetUies(
                (int)UiManager::UiCount::PlayerCommandShortCutSunder);
            if (!sharedUiCommandShortCutSunderId)
                return false;

            std::shared_ptr<Ui> uiIdAttackCheckSunder = sharedUiCommandShortCutSunderId->GetComponent<Ui>();
            // 安全チェック コマンド雷　非選択 表示
            uiIdAttackCheckSunder->SetDrawCheck(isDrawUi);
        }

        // 氷コマンド
        {
            // 安全チェック コマンド氷　非選択
            auto sharedUiCommandShortCutIceId = UiManager::Instance().GetUies(
                (int)UiManager::UiCount::PlayerCommandShortCutIce);
            if (!sharedUiCommandShortCutIceId)
                return false;

            std::shared_ptr<Ui> uiIdAttackCheckIce = sharedUiCommandShortCutIceId->GetComponent<Ui>();
            // 安全チェック コマンド氷　非選択 表示
            uiIdAttackCheckIce->SetDrawCheck(isDrawUi);
        }

        // 回復コマンド
        {
            // 安全チェック コマンド回復　非選択
            auto sharedUiCommandShortCutHealeId = UiManager::Instance().GetUies(
                (int)UiManager::UiCount::PlayerCommandShortCutKeule);
            if (!sharedUiCommandShortCutHealeId)
                return false;

            std::shared_ptr<Ui> uiIdAttackCheckHeale = sharedUiCommandShortCutHealeId->GetComponent<Ui>();
            // 安全チェック コマンド回復　非選択 表示
            uiIdAttackCheckHeale->SetDrawCheck(isDrawUi);
        }
        return true;
    }
    
    // 後変更 いるのか分からない
    // 魔法打った後
    else if (gamePad.GetButton() == GamePad::BTN_LEFT_SHOULDER)
    {        
        // 炎
        // 安全チェック
        auto sharedUiCommandShortCutFireId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutFire);
        if (!sharedUiCommandShortCutFireId)
            return false;

        std::shared_ptr<Ui> uiIdAttackCheckFire = sharedUiCommandShortCutFireId->GetComponent<Ui>();
        uiIdAttackCheckFire->SetDrawCheck(isDrawUiEmpth);
        // 雷
        // 安全チェック
        auto sharedUiCommandShortCutSunderId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutSunder);
        if (!sharedUiCommandShortCutSunderId)
            return false;

        std::shared_ptr<Ui> uiIdAttackCheckSunder = sharedUiCommandShortCutSunderId->GetComponent<Ui>();
        uiIdAttackCheckSunder->SetDrawCheck(isDrawUiEmpth);
        // 氷
        // 安全チェック
        auto sharedUiCommandShortCutIceId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutIce);
        if (!sharedUiCommandShortCutIceId)
            return false;

        std::shared_ptr<Ui> uiIdAttackCheckIce = sharedUiCommandShortCutIceId->GetComponent<Ui>();
        uiIdAttackCheckIce->SetDrawCheck(isDrawUiEmpth);
        // 回復
        // 安全チェック
        auto sharedUiCommandShortCutHealeId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutKeule);
        if (!sharedUiCommandShortCutHealeId)
            return false;

        std::shared_ptr<Ui> uiIdAttackCheckHeale = sharedUiCommandShortCutHealeId->GetComponent<Ui>();
        uiIdAttackCheckHeale->SetDrawCheck(isDrawUiEmpth);
    }

    // ショートカットキー解除
    if (gamePad.GetButtonUp() & GamePad::BTN_LEFT_SHOULDER)
    {
        // 魔法選択UI解除
        RemoveUIMagic();

        // コマンド火
        {
            // 安全チェック コマンド火　非選択
            auto sharedUiCommandShortCutFireId = UiManager::Instance().GetUies(
                (int)UiManager::UiCount::PlayerCommandShortCutFire);
            if (!sharedUiCommandShortCutFireId)
                return false;

            std::shared_ptr<Ui> uiIdAttackCheckFire = sharedUiCommandShortCutFireId->GetComponent<Ui>();
            uiIdAttackCheckFire->SetDrawCheck(isDrawUiEmpth);
        }

        // コマンド雷
        {
            // 安全チェック コマンド雷　非選択
            auto sharedUiCommandShortCutSunderId = UiManager::Instance().GetUies(
                (int)UiManager::UiCount::PlayerCommandShortCutSunder);
            if (!sharedUiCommandShortCutSunderId)
                return false;

            std::shared_ptr<Ui> uiIdAttackCheckSunder = sharedUiCommandShortCutSunderId->GetComponent<Ui>();
            uiIdAttackCheckSunder->SetDrawCheck(isDrawUiEmpth);
        }

        // コマンド氷
        {
            // 安全チェック コマンド氷　非選択
            auto sharedUiCommandShortCutIceId = UiManager::Instance().GetUies(
                (int)UiManager::UiCount::PlayerCommandShortCutIce);
            if (!sharedUiCommandShortCutIceId)
                return false;

            std::shared_ptr<Ui> uiIdAttackCheckIce = sharedUiCommandShortCutIceId->GetComponent<Ui>();
            uiIdAttackCheckIce->SetDrawCheck(isDrawUiEmpth);
        }

        // コマンド回復
        {
            // 安全チェック コマンド回復　非選択
            auto sharedUiCommandShortCutHealeId = UiManager::Instance().GetUies(
                (int)UiManager::UiCount::PlayerCommandShortCutKeule);
            if (!sharedUiCommandShortCutHealeId)
                return false;

            std::shared_ptr<Ui> uiIdAttackCheckHeale = sharedUiCommandShortCutHealeId->GetComponent<Ui>();
            // 非表示
            uiIdAttackCheckHeale->SetDrawCheck(isDrawUiEmpth);
        }
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
// 特殊攻撃選択
bool Player::InputSpecialAttackCharge()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    int uiCount = UiManager::Instance().GetUiesCount();
    // ui無かったら
    if (uiCount <= uiCountMax) return false;
    // 必殺技チャージ
    if (specialAttackCharge >= specialAttackChargeMax)
    {
        // 安全チェック
        auto sharedUiSpecialChargeFurstId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandSpeciulCharge01);
        auto sharedUiSpecialChargeSecondId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandSpeciulCharge02);
        auto sharedUiSpecialChargeSerdeId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandSpeciulCharge03);
        if (!sharedUiSpecialChargeFurstId && !sharedUiSpecialChargeSecondId && !sharedUiSpecialChargeSerdeId)
            return false;

        std::shared_ptr<Ui> uiIdSpecialChargeFurst = sharedUiSpecialChargeFurstId->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdSpecialChargeSecond = sharedUiSpecialChargeSecondId->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdSpecialChargeSerde = sharedUiSpecialChargeSerdeId->GetComponent<Ui>();

        // 必殺技たまった音
        PlaySe("Data/Audio/SE/必殺技ため.wav");

        //// 一度発動すると初期化
        specialAttackCharge = 0.0f;
        bool drawCheck = false;
        uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);
        uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);
        uiIdSpecialChargeSerde->SetDrawCheck(drawCheck);
        // 剣攻撃を一定以上溜めたら
        if (attackEnergyCharge >= energyChargeMax)
        {
            // 技確定
            // 必殺技何が登録されたか 斬撃
            specialAttack.at((int)SpecialAttackType::Attack).hasSkill = true;
            // 一度発動すると初期化
            specialAttackCharge = specialAttackChargeMin;
            // 斬撃必殺技チャージ解消
            attackEnergyCharge = attackEnergyChargeMin;
        }
        // 炎魔法を一定以上溜めたら
        if (fireEnergyCharge >= energyChargeMax)
        {
            //// 技確定
            // 必殺技何が登録されたか 火
            specialAttack.at((int)SpecialAttackType::MagicFire).hasSkill = true;
            // 一度発動すると初期化
            specialAttackCharge = specialAttackChargeMin;
            // 火必殺技チャージ解消
            fireEnergyCharge = fireEnergyChargeMin;
        }
    }

    // 必殺技撃っていたら撃てない
    if (stateMachine->GetStateIndex() == (int)Player::State::SpecialAttack ||
        stateMachine->GetStateIndex() == (int)Player::State::SpecialMagic)return false;
    
    // 技を放つ
    if (gamePad.GetButtonDown() & GamePad::BTN_B &&
        !specialAttackTime && !magicAction && specialAction&&
        !gamePad.GetButtonDownCountinue())
    {

        switch (specialAttackNum)
        {
            // 斬撃
        case (int)SpecialAttackType::Attack:
        {
            if (!specialAttack.at(specialAttackNum).hasSkill)
            {
                // se再生
                seParam.filename = "Data/Audio/SE/魔法打てない.wav";
                seParam.volume = 1.0f;
                InputSe(seParam);
                // 必殺技UIを解除
                specialAction = false;
                return false;
            }
            // 安全チェック
            auto sharedUiSpecialShurashuId = UiManager::Instance().GetUies(
                (int)UiManager::UiCount::PlayerCommandSpeciulShurashu);
            if (!sharedUiSpecialShurashuId)
                return false;

            // 技確定
            std::shared_ptr<Ui> uiIdSpecialShurashu = sharedUiSpecialShurashuId->GetComponent<Ui>();
            std::shared_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = sharedUiSpecialShurashuId->GetComponent<TransForm2D>();
            bool drawCheck = false;
            uiIdSpecialShurashu->SetDrawCheck(drawCheck);
            DirectX::XMFLOAT2 pos;
            pos = { uiIdSpecialShurashuTransForm2D->GetPosition() };

            // 必殺技否所持
            specialAttack.at((int)SpecialAttackType::Attack).hasSkill = false;

            GetStateMachine()->ChangeState(static_cast<int>(Player::State::SpecialAttack));
            // もし地面なら何もしない
            bool noStart = false;
            // エフェクト再生
            areWork->GetEfeHandle() ? areWork->Stop(areWork->GetEfeHandle()) : noStart;
            areWork->Play(position);
            break;
        }
        // 魔法火
        case (int)SpecialAttackType::MagicFire:
        {
            if (!specialAttack.at(specialAttackNum).hasSkill)
            {
                // se再生
                seParam.filename = "Data/Audio/SE/魔法打てない.wav";
                seParam.volume = 1.0f;
                InputSe(seParam);
                // 必殺技UIを解除
                specialAction = false;
                return false;
            }
            // 安全チェック
            auto sharedUiSpecialFrameId = UiManager::Instance().GetUies(
                (int)UiManager::UiCount::PlayerCommandSpeciulFrame);
            if (!sharedUiSpecialFrameId)
                return false;

            // 技確定
            std::shared_ptr<Ui> uiIdSpecialFrame = sharedUiSpecialFrameId->GetComponent<Ui>();
            std::shared_ptr<TransForm2D> uiIdSpecialFrameTransForm2D = sharedUiSpecialFrameId->GetComponent<TransForm2D>();
            bool drawCheck = false;
            uiIdSpecialFrame->SetDrawCheck(drawCheck);
            DirectX::XMFLOAT2 pos;
            pos = { uiIdSpecialFrameTransForm2D->GetPosition() };
            // 必殺技否所持
            specialAttack.at((int)SpecialAttackType::MagicFire).hasSkill = false;
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

        // 技撃てる
        if (!specialAttack.at((int)SpecialAttackType::Attack).hasSkill &&
            specialAttackNum == (int)SpecialAttackType::Attack) return false;

        if (!specialAttack.at((int)SpecialAttackType::MagicFire).hasSkill &&
            specialAttackNum == (int)SpecialAttackType::MagicFire) return false;
        // エネミー呼ぶ奴
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyManagerCount = enemyManager.GetEnemyCount();
        // 動作させるかどうか
        if (enemyManagerCount > 0)
        {
            // 安全チェック
            auto sharedEnemyId = enemyManager.GetEnemy(enemyManagerCount - 1);
            if (!sharedEnemyId)
                return false;

            std::shared_ptr<EnemyBoss> enemy = sharedEnemyId->GetComponent<EnemyBoss>();
            std::shared_ptr<Movement> enemyMove = sharedEnemyId->GetComponent<Movement>();
            bool moveCheck = false;
            enemy->SetMoveCheck(moveCheck);
            // 速度停止
            bool stopVelocity = true;
            enemyMove->SetStopMove(stopVelocity);
            // 落ちるの停止
            bool stopFall = true;
            enemyMove->SetStopFall(stopFall);
        }
        
        specialAttackTime = true;
        return true;
    }
    else
    {
        specialAttackTime = false;
    }

    // チャージたまる
    // チャージを見やすく
    if (specialAttackCharge >= 0.4f)
    {
        // 安全チェック
        auto sharedUiCommandSpeciulCharge01Id = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandSpeciulCharge01);
        if (!sharedUiCommandSpeciulCharge01Id)
            return false;

        std::shared_ptr<Ui> uiIdSpecialCharge = sharedUiCommandSpeciulCharge01Id->GetComponent<Ui>();
        bool drawCheck = true;
        uiIdSpecialCharge->SetDrawCheck(drawCheck);
    }
    // チャージを見やすく
    if (specialAttackCharge >= 0.8f)
    {
        // 安全チェック
        auto sharedUiCommandSpeciulCharge02Id = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandSpeciulCharge02);
        if (!sharedUiCommandSpeciulCharge02Id)
            return false;

        std::shared_ptr<Ui> uiIdSpecialChargeSecond = sharedUiCommandSpeciulCharge02Id->GetComponent<Ui>();
        bool drawCheck = true;
        uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);
    }
    // チャージを見やすく
    if (specialAttackCharge >= 1.2f)
    {
        // 安全チェック
        auto sharedUiCommandSpeciulCharge03Id = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandSpeciulCharge03);
        if (!sharedUiCommandSpeciulCharge03Id)
            return false;

        std::shared_ptr<Ui> uiIdSpecialChargeSerde = sharedUiCommandSpeciulCharge03Id->GetComponent<Ui>();
        bool drawCheck = true;
        uiIdSpecialChargeSerde->SetDrawCheck(drawCheck);
    }
    return false;
}
// 特殊技順番交代
void Player::InputSpecialAttackChange()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    // コマンド操作 選択
    if (gamePad.GetButtonDown() & GamePad::BTN_UP && specialAction)
    {
        --specialAttackNum;
    }
    // コマンド操作 選択
    if (gamePad.GetButtonDown() & GamePad::BTN_DOWN && specialAction)
    {
        ++specialAttackNum;
    }

    // 最低値まで行ったら戻す
    if (specialAttackNum <= spCmdMoveLimitMin)
        specialAttackNum = spCmdMoveLimitMin;
    // 最大値まで行ったら戻す
    if (specialAttackNum >= spCmdMoveLimitMax)
        specialAttackNum = spCmdMoveLimitMax;
}
// UI必殺技演出
void Player::SpecialPlayUlEffect(float elapsedTime)
{
    // Ui必殺技選択してないとき コマンドUI点滅処理
    if (!specialAction)
    {
        auto uiId = UiManager::Instance().GetUies((int)UiManager::UiCount::ButtonY);
        if (!uiId) return;

        std::shared_ptr<Ui> uiIdSpecialButton = uiId->GetComponent<Ui>();
        // コマンド　ボタン　非表示
        uiIdSpecialButton->SetDrawCheck(isDrawUiEmpth);

        // 安全チェック UIコマンド　点滅用
        auto sharedUiCommandSpecialUnCheckId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandSpecialUnCheck);
        if (!sharedUiCommandSpecialUnCheckId)
            return;

        // UIコマンド　点滅用 表示、非表示切り替え用
        std::shared_ptr<Ui> uiIdSpecialUnCheck = sharedUiCommandSpecialUnCheckId->GetComponent<Ui>();

        // 特殊技溜まって無かったら コマンドUI点滅しない
        if (!specialAttack.at((int)SpecialAttackType::Attack).hasSkill &&
            !specialAttack.at((int)SpecialAttackType::MagicFire).hasSkill)
        {
            // UI非表示
            uiIdSpecialUnCheck->SetDrawCheck(isDrawUiEmpth);
            return;
        }

        // 特殊攻撃たまった 知らせるために コマンドUI点滅
        if (!UpdateElapsedTime(timeElapsedHintMax, elapsedTime))return;
        // 点滅
        isUiSpecilDrawCheck = isUiSpecilDrawCheck ? false : true;
        // 表示　非表示
        uiIdSpecialUnCheck->SetDrawCheck(isUiSpecilDrawCheck);
        return;
    }

    // コマンド　必殺技選択中
    switch (specialAttackNum)
    {
    case (int)SpecialAttackType::Attack:
    {
        // 必殺技切りつけ
        // 安全チェック
        auto sharedUiSpecialShurashuId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandSpeciulShurashu);
        if (!sharedUiSpecialShurashuId)
            return;

        // コマンドUI斬撃　トランスフォーム
        std::shared_ptr<TransForm2D> uiIdSpecialAttackTransForm2D = sharedUiSpecialShurashuId->GetComponent<TransForm2D>();
        
        // 必殺技ボタン
        // 安全チェック
        auto sharedUiButtonId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::ButtonY);
        if (!sharedUiButtonId)
            return;

        std::shared_ptr<Ui> uiIdSpecialButton = sharedUiButtonId->GetComponent<Ui>();
        std::shared_ptr<TransForm2D> uiIdSpecialButtonTransForm2D= sharedUiButtonId->GetComponent<TransForm2D>();
        
        float offset = 20.0f;
        float pos = uiIdSpecialAttackTransForm2D->GetPosition().y + offset;

        // 必殺技コマンド切りつけ
        uiIdSpecialButton->SetDrawCheck(isDrawUi);
        uiIdSpecialButtonTransForm2D->SetPositionY(pos);
        break;
    }
    case (int)SpecialAttackType::MagicFire:
    {
        // 必殺技炎
        // 安全チェック
        auto sharedUiCommandSpeciulFrameId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandSpeciulFrame);
        if (!sharedUiCommandSpeciulFrameId)
            return;

        // コマンドUI火　トランスフォーム
        std::shared_ptr<TransForm2D> uiIdSpecialFireTransForm2D = sharedUiCommandSpeciulFrameId->GetComponent<TransForm2D>();
        
        // 安全チェック
        auto sharedUiButtonId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::ButtonY);
        if (!sharedUiButtonId)
            return;

        std::shared_ptr<Ui> uiIdSpecialButton = sharedUiButtonId->GetComponent<Ui>();
        std::shared_ptr<TransForm2D> uiIdSpecialButtonTransForm2D = sharedUiButtonId->GetComponent<TransForm2D>();

        float offset = 20.0f;
        float pos = uiIdSpecialFireTransForm2D->GetPosition().y + offset;

        // 必殺技コマンド火
        uiIdSpecialButton->SetDrawCheck(isDrawUi);
        uiIdSpecialButtonTransForm2D->SetPositionY(pos);
        break;
    }
    case (int)SpecialAttackType::MagicIce:
    {
        // 氷
        // 安全チェック
        auto sharedUiCommandSpeciulIceId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandSpeciulIce);
        if (!sharedUiCommandSpeciulIceId)
            return;

        std::shared_ptr<Ui> uiIdSpecialIce = sharedUiCommandSpeciulIceId->GetComponent<Ui>();
        std::shared_ptr<TransForm2D> uiIdSpecialIceTransForm2D = sharedUiCommandSpeciulIceId->GetComponent<TransForm2D>();

        break;
    }
    case (int)SpecialAttackType::MagicThander:
    {
        // 雷
        // 安全チェック
        auto sharedUiCommandSpeciulThanderId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandSpeciulThander);
        if (!sharedUiCommandSpeciulThanderId)
            return;

        std::shared_ptr<Ui> uiIdSpecialThander = sharedUiCommandSpeciulThanderId->GetComponent<Ui>();
        std::shared_ptr<TransForm2D> uiIdSpecialThanderTransForm2D = sharedUiCommandSpeciulThanderId->GetComponent<TransForm2D>();

        break;
    }
    }
}

// 歩き　移動　方向
DirectX::XMFLOAT3 Player::GetMoveVec(float elapsedTime)
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return DirectX::XMFLOAT3(0,0,0);
    // 移動コンポーネント
    std::shared_ptr movementId = sharedId->GetComponent<Movement>();

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
        movementId->Move(vec,moveSpeed, elapsedTime);
        movementId->Turn( vec ,turnSpeed, elapsedTime);
    }
    return vec;
}
// 魔法　初動　向き
DirectX::XMFLOAT3 Player::GetMagicMoveVec(float elapsedTime)
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return DirectX::XMFLOAT3(0, 0, 0);
    // 移動コンポーネント
    std::shared_ptr movementId = sharedId->GetComponent<Movement>();

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
        movementId->Move(vec, moveSpeed, elapsedTime);
    }
    return vec;
}

// 当たり判定敵の五体すべて
bool Player::CheckAllPartsCollision(DirectX::XMFLOAT3 pos,float rudius)
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return false;
    // 移動コンポーネント
    std::shared_ptr collisionId = sharedId->GetComponent<Collision>();

    // 敵について
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();

    if (enemyCount <= 0) return false;

    std::shared_ptr<Actor> enemyShader = enemyManager.GetEnemy((int)EnemyManager::EnemyType::Boss);
    if (!enemyShader) return false;

    std::shared_ptr<EnemyBoss> enemyBoss = enemyShader->GetComponent<EnemyBoss>();
    std::shared_ptr<Transform> enemyTransform = enemyShader->GetComponent<Transform>();
    std::shared_ptr<ModelControll> enemyModel = enemyShader->GetComponent<ModelControll>();

    // パーツの種類
    Model::Node* nodePart;

    // 位置
    DirectX::XMFLOAT3 nodePosition;
    DirectX::XMFLOAT3 outPositon;

    //for (int i = 0; i < enemyBoss->GetBodyPartSize(); ++i)
    for (auto& part : enemyBoss->GetBodyPartSize())
    {
        auto bornPart = enemyBoss->GetBodyPartBorn(part.first);

        // パーツの種類
        nodePart = enemyModel->GetModel()->FindNode(bornPart);

        // 位置
        DirectX::XMFLOAT3 nodePosition;
        nodePosition = enemyModel->GetModel()->ConvertLocalToWorld(nodePart);
        
        if (!collisionId->IntersectSpherVsSphere(
            pos,
            rudius,
            nodePosition,
            enemyBoss->GetBodyPart(part.first),
            outPositon)) return false;

        return true;
    }
    return false;
}

// 魔法の種類チェック炎
bool Player::CheckMagicFire(std::shared_ptr<Actor> projectile)
{
    // 魔法炎単発
    if (projectile->GetComponent<ProjectileHoming>())
    {
        if (projectile->GetComponent<ProjectileHoming>()->GetMagicNumber() ==
            (int)ProjectileHoming::MagicNumber::Fire)
            return true;
    }

    // 魔法炎連発
    if (projectile->GetComponent<ProjectileFullHoming>())
    {
        if (projectile->GetComponent<ProjectileFullHoming>()->GetMagicNumber() ==
            (int)ProjectileFullHoming::MagicNumber::Fire)
            return true;
    }

    return false;
}
// 魔法種類確認
bool Player::CheckMagicIce(std::shared_ptr<Actor> projectile)
{
    // 魔法氷単発
    if (projectile->GetComponent<ProjectileHoming>())
    {
        if (projectile->GetComponent<ProjectileHoming>()->GetMagicNumber() ==
            (int)ProjectileHoming::MagicNumber::Ice)
            return true;
    }

    // 氷連射用チェック
    if (projectile->GetComponent<ProjectileStraight>())
        return true;

    return false;
}

// 魔法火と敵との当たり判定
void Player::CollisionMagicFire()
{    
    // 敵について
    EnemyManager& enemyManager = EnemyManager::Instance();
    projectileManager = ProjectileManager::Instance();

    // エネミーの数
    int enemyCount = enemyManager.GetEnemyCount();
    // エネミーが一人でも生きていたら
    if (enemyCount <= 0) return;
    // エネミーの安全☑
    std::shared_ptr<Actor> enemyShader = enemyManager.GetEnemy((int)EnemyManager::EnemyType::Boss);
    if (!enemyShader) return;

    std::shared_ptr<EnemyBoss> enemyBoss = enemyShader->GetComponent<EnemyBoss>();
    std::shared_ptr<HP> enemyHp = enemyShader->GetComponent<HP>();
    std::shared_ptr<ModelControll> enemyModel = enemyShader->GetComponent<ModelControll>();

    // 全ての敵と総当たりで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        // 安全チェック
        std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        if (!projectile)
            return;
        // 魔法位置
        DirectX::XMFLOAT3 projectilePosition = projectile->GetComponent<Transform>()->GetPosition();
        float projectileRadius = projectile->GetComponent<Collision>()->GetRadius();
        // 衝突処理
        if (!CheckAllPartsCollision(projectilePosition, leftHandRadius)) return;

        // 魔法の種類炎以外拒絶
        if (!CheckMagicFire(projectile)) return;

        // 炎ダメージ
        applyDamageMagic = applyDamageFire;

        // ダメージを与える。
        if (!enemyHp->ApplyDamage(applyDamageMagic, magicAttackInvincibleTime))
            return;

        // 炎関係
        ++fireEnergyCharge;
        hitFire->Play(projectilePosition);
        // 共通ダメエフェ
        hitEffect->Play(projectilePosition);

        // 遷移変更
        if (enemyBoss->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Wander &&
            enemyBoss->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Jump &&
            enemyBoss->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::IdleBattle &&
            enemyBoss->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Attack
            )
        {
            // ダメージステートへ
            enemyBoss->GetStateMachine()->ChangeState((int)EnemyBoss::State::Damage);
        }
        // 当たった時の副次的効果
        specialAttackCharge += specialAttackChargeMagicValue;

        // 弾丸破棄
        projectile->GetComponent<BulletFiring>()->Destroy();
    }
}

// 魔法雷と敵との当たり判定
void Player::CollisionMagicSunder()
{

    projectileManager = ProjectileManager::Instance();

    // 敵について
    EnemyManager& enemyManager = EnemyManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();

    if (enemyCount <= 0) return;

    std::shared_ptr<Actor> enemyShader = enemyManager.GetEnemy((int)EnemyManager::EnemyType::Boss);
    if (!enemyShader) return;

    std::shared_ptr<EnemyBoss> enemyBoss = enemyShader->GetComponent<EnemyBoss>();
    std::shared_ptr<HP> enemyHp = enemyShader->GetComponent<HP>();
    std::shared_ptr<ModelControll> enemyModel = enemyShader->GetComponent<ModelControll>();

    // 全ての敵と総当たりで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        // 安全チェック
        std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        if (!projectile)
            return;
        // 魔法位置
        DirectX::XMFLOAT3 projectilePosition = projectile->GetComponent<Transform>()->GetPosition();
        float projectileRadius = projectile->GetComponent<Collision>()->GetRadius();
        // 衝突処理
        if (!CheckAllPartsCollision(projectilePosition, leftHandRadius)) return;

        // 魔法の種類雷以外拒絶
        if (!projectile->GetComponent<ProjectileSunder>())
        return;

        // 雷ダメージ
        applyDamageMagic = applyDamageThander;

        // ダメージを与える。
        if (!enemyHp->ApplyDamage(applyDamageMagic, magicAttackInvincibleTime))
            return;

        // 雷関係
        ++ThanderEnergyCharge;
        hitThander->Play(projectilePosition);
        // 共通ダメエフェ
        hitEffect->Play(projectilePosition);


        // 遷移変更
        if (enemyBoss->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Wander &&
            enemyBoss->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Jump &&
            enemyBoss->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::IdleBattle &&
            enemyBoss->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Attack
            )
        {
            // ダメージステートへ
            enemyBoss->GetStateMachine()->ChangeState((int)EnemyBoss::State::Damage);
        }
        // 当たった時の副次的効果
        specialAttackCharge += specialAttackChargeMagicValue;

        // 弾丸破棄
        //projectile->GetComponent<BulletFiring>()->Destroy();
    }
}

// 魔法氷と敵との当たり判定
void Player::CollisionMagicIce()
{
    // 敵について
    EnemyManager& enemyManager = EnemyManager::Instance();
    projectileManager = ProjectileManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();

    if (enemyCount <= 0) return;

    std::shared_ptr<Actor> enemyShader = enemyManager.GetEnemy((int)EnemyManager::EnemyType::Boss);
    if (!enemyShader) return;

    std::shared_ptr<EnemyBoss> enemyBoss = enemyShader->GetComponent<EnemyBoss>();
    std::shared_ptr<HP> enemyHp = enemyShader->GetComponent<HP>();
    std::shared_ptr<ModelControll> enemyModel = enemyShader->GetComponent<ModelControll>();

    // 全ての敵と総当たりで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();
    for (int i = 0; i < projectileCount; ++i)
    {
        // 安全チェック
        std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        if (!projectile)
            return;
        // 魔法位置
        DirectX::XMFLOAT3 projectilePosition = projectile->GetComponent<Transform>()->GetPosition();
        float projectileRadius = projectile->GetComponent<Collision>()->GetRadius();
        // 衝突処理
        if (!CheckAllPartsCollision(projectilePosition, leftHandRadius)) 
            return;

        // 魔法の種類炎以外拒絶
        if (!CheckMagicIce(projectile)) 
            return;

        // 氷ダメージ
        applyDamageMagic = applyDamageIce;

        // ダメージを与える。
        if (!enemyHp->ApplyDamage(applyDamageMagic, magicAttackInvincibleTime))
            return;

        // 氷関係
        ++iceEnergyCharge;
        hitIce->Play(projectilePosition);
        // 共通ダメエフェ
        hitEffect->Play(projectilePosition);

        // 遷移変更
        if (enemyBoss->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Wander &&
            enemyBoss->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Jump &&
            enemyBoss->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::IdleBattle &&
            enemyBoss->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Attack
            )
        {
            // ダメージステートへ
            enemyBoss->GetStateMachine()->ChangeState((int)EnemyBoss::State::Damage);
        }
        // 当たった時の副次的効果
        specialAttackCharge += specialAttackChargeMagicValue;

        // 弾丸破棄
        projectile->GetComponent<BulletFiring>()->Destroy();
    }
}
// プレイヤーとエネミーとの衝突処理
void Player::CollisionPlayerVsEnemies()
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // 移動コンポーネント
    std::shared_ptr transformId = sharedId->GetComponent<Transform>();
    std::shared_ptr collisionId = sharedId->GetComponent<Collision>();

    EnemyManager& enemyManager = EnemyManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();

    if (enemyCount <= 0) return;
    // 安全チェック
    std::shared_ptr<Actor> enemyShader = enemyManager.GetEnemy((int)EnemyManager::EnemyType::Boss);
    if (!enemyShader) return;

    std::shared_ptr<EnemyBoss> enemyBoss = enemyShader->GetComponent<EnemyBoss>();
    std::shared_ptr<Transform> enemyTransform = enemyShader->GetComponent<Transform>();
    std::shared_ptr<Collision> enemyCollision = enemyShader->GetComponent<Collision>();
    std::shared_ptr<ModelControll> enemyModel = enemyShader->GetComponent<ModelControll>();

    //// 衝突処理
    DirectX::XMFLOAT3 outPositon;
    DirectX::XMFLOAT3 enemyPosition = enemyTransform->GetPosition();
    float enemyRadius = enemyCollision->GetRadius();
    // もし高さが一緒なら
    float enemyHeight = enemyBoss->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::IdleBattle ?
        enemyCollision->GetHeight() : enemyCollision->GetSecondesHeight();
    // エネミーとプレイヤーの当たり判定はじき
    if (collisionId->IntersectCylinderVsCylinder(
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
        transformId->SetPosition(position);
        // 接触
        isEnemyHit = true;
        return;
    }
    // 非接触
    isEnemyHit = false;
}
// 後変更Collision
// 敵の範囲内に入らないように 腰以上
void Player::CollisionBornVsProjectile(const char* bornname)
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // 移動コンポーネント
    std::shared_ptr transformId = sharedId->GetComponent<Transform>();
    std::shared_ptr collisionId = sharedId->GetComponent<Collision>();


    EnemyManager& enemyManager = EnemyManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();

    if (enemyCount <= 0) return;
    // 安全チェック
    std::shared_ptr<Actor> enemyShader = enemyManager.GetEnemy((int)EnemyManager::EnemyType::Boss);
    if (!enemyShader) return;

    std::shared_ptr<EnemyBoss> enemyBoss = enemyShader->GetComponent<EnemyBoss>();
    std::shared_ptr<Transform> enemyTransform = enemyShader->GetComponent<Transform>();
    std::shared_ptr<Collision> enemyCollision = enemyShader->GetComponent<Collision>();
    std::shared_ptr<HP> enemyHp = enemyShader->GetComponent<HP>();
    std::shared_ptr<ModelControll> enemyModel = enemyShader->GetComponent<ModelControll>();

    // ノード取得
    Model::Node* node = enemyModel->GetModel()->FindNode(bornname);
    // ノード位置取得
    DirectX::XMFLOAT3 nodePosition;
    nodePosition = {
        node->worldTransform._41,
        node->worldTransform._42,
        node->worldTransform._43
    };
    //// 衝突処理
    DirectX::XMFLOAT3 outPositon;
    DirectX::XMFLOAT3 enemyPosition = enemyTransform->GetPosition();
    float enemyRadius = enemyBoss->GetUpperRadius();
    // もし高さが一緒なら
    float enemyHeight = enemyBoss->GetStateMachine()->GetStateIndex() == (int)EnemyBoss::State::IdleBattle ?
        enemyCollision->GetHeight() : enemyCollision->GetSecondesHeight();
    // 当たり判定腰から上
    if (collisionId->IntersectCylinderVsCylinder(
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
        // 接触
        isEnemyHitBody = true;

        if (normal.y > 0.8f) return;

        position = outPositon;
        transformId->SetPosition(position);
        return;
    }
    // 非接触
    isEnemyHitBody = false;
}
// 切りつけ
bool Player::CollisionNodeVsEnemies()
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return false;
    // 移動コンポーネント
    std::shared_ptr<ModelControll> modelId = sharedId->GetComponent<ModelControll>();
    std::shared_ptr<Collision> collisionId = sharedId->GetComponent<Collision>();

    // マネージャー取得
    EnemyManager& enemyManager = EnemyManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();

    if (enemyCount <= 0) return false;
    // 安全チェック
    std::shared_ptr<Actor> enemyShader = enemyManager.GetEnemy((int)EnemyManager::EnemyType::Boss);
    if (!enemyShader) return false;

    std::shared_ptr<EnemyBoss> enemyBoss = enemyShader->GetComponent<EnemyBoss>();
    std::shared_ptr<HP> enemyHp = enemyShader->GetComponent<HP>();
    std::shared_ptr<ModelControll> enemyModelId = enemyShader->GetComponent<ModelControll>();

    // ノード取得
    Model::Node* node = model->FindNode("mixamorig:LeftHand");
    // ノード位置取得
    DirectX::XMFLOAT3 nodePosition;
    nodePosition = modelId->GetModel()->ConvertLocalToWorld(node);

    //// 衝突処理
    DirectX::XMFLOAT3 outPositon;
    if (!CheckAllPartsCollision(nodePosition, leftHandRadius)) return false;

    if (!enemyHp->ApplyDamage(applyDamageNormal, 0.5f)) return false;

    // 斬撃se再生
    PlaySe("Data/Audio/SE/スラッシュ２回目.wav");

    // 当たり判定
    hitSlash->Play(nodePosition, slashScale);

    if (enemyBoss->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Wander &&
        enemyBoss->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Jump &&
        enemyBoss->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::IdleBattle
        )
    {
        if (enemyBoss->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::Attack)
        {
            Model::ModelAnim modelAnim;
            modelAnim.index = EnemyBoss::Animation::Anim_Movie;
            modelAnim.currentanimationseconds = 1.0f;
            modelAnim.keyFrameEnd = 153.0f;
            // 通常
            enemyModelId->GetModel()->PlayAnimation(modelAnim);
        }
        // 死んだとき
        if (enemyBoss->GetStateMachine()->GetStateIndex() == (int)EnemyBoss::State::IdleBattle)
        {
            // model情報
            Model::ModelAnim modelAnim;
            modelAnim.index = EnemyBoss::Animation::Anim_Die;
            modelAnim.currentanimationseconds = 0.3f;
            modelAnim.keyFrameEnd = 55.0f;
            enemyModelId->GetModel()->PlayAnimation(modelAnim);
        }
        // 混乱状態
        if (attackNumberSave >= attackNumberSaveMax && attackNumberSave <= attackNumberSaveMax &&
            enemyBoss->GetStateMachine()->GetStateIndex() != (int)EnemyBoss::State::IdleBattle)
        {
            enemyBoss->GetStateMachine()->ChangeState(
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

// 後変更Collision
// カウンター用
void Player::CollisionNodeVsEnemiesCounter(const char* nodeName, float nodeRadius)
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;

    // 移動コンポーネント
    std::shared_ptr collisionId = sharedId->GetComponent<Collision>();

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
            if (collisionId->IntersectSpherVsSphere(
                nodePosition,
                nodeRadius,
                projectilePosition,
                projectileOutRudius,
                outPositon) && 
                !collisionId->IntersectSpherVsSphere(
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

// 後変更ソードトレイル
// ソードトレイル用
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

// hpのピンチ　ui描画も
void Player::PinchMode(float elapsedTime)
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // 移動コンポーネント
    std::shared_ptr hpId = sharedId->GetComponent<HP>();

    // hp が一定以下なら
    if (hpId->HealthPinch() && !hpId->GetDead())
    {
        // 一定時間で描画
        if (UpdateElapsedTime(timeElapsedHintMax, elapsedTime))
        {
            // Se再生
            PlaySe("Data/Audio/SE/HP危険.wav");

            hintDrawCheck = hintDrawCheck ? false : true;
            UiManager::Instance().GetUies((int)UiManager::UiCount::PushShort)->
                GetComponent<Ui>()->SetDrawCheck(hintDrawCheck);
        }
        // ショートカットキーが押されたら変わらない
        if (InputShortCutkeyMagic())
        {
            hintDrawCheck = true;
            UiManager::Instance().GetUies((int)UiManager::UiCount::PushShort)->
                GetComponent<Ui>()->SetDrawCheck(hintDrawCheck);
        }
        // ピンチ時常に描画
        UiManager::Instance().GetUies((int)UiManager::UiCount::Push2)->
            GetComponent<Ui>()->SetDrawCheck(isDrawUi);
        UiManager::Instance().GetUies((int)UiManager::UiCount::ShortCut)->
            GetComponent<Ui>()->SetDrawCheck(isDrawUi);
        PostprocessingRenderer& postprocessingRenderer = PostprocessingRenderer::Instance();
        // ポストエフェクト
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
    // 通常時
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
    // ピンチの時にヒントを出す　ショートカットキー
    if (InputShortCutkeyMagic() &&
        isPintch)
    {
        std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandShortCutKeule)->GetComponent<TransForm2D>();
        uiIdAttackCheckPos.lock()->Shake();
    }
    //  ピンチの時にヒントを出す　コマンド操作
    if (magicAction &&
        !InputShortCutkeyMagic()&&
        isPintch)
    {
        std::weak_ptr<TransForm2D> uiIdAttackCheckPos = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandHeale)->GetComponent<TransForm2D>();
        uiIdAttackCheckPos.lock()->Shake();
    }
}
// ジャンプ
bool Player::InputJump()
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return false;
    // 移動コンポーネント
    std::shared_ptr movementId = sharedId->GetComponent<Movement>();

    // ボタンで入力でジャンプ（ジャンプ回数制限つき）
    GamePad& gamePad = Input::Instance().GetGamePad();

    // 魔法コマンドを使っていたら
    if (magicAction) return false;

    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        // 値を小さくする
        if (movementId->GetJumpCount() < jumpLimit) {
            movementId->JumpVelocity(jumpSpeed);
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

    // 魔法コマンドを使っていたら
    if (magicAction) return false;

    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        return true;
    }
    return false;
}
// 攻撃入力
bool Player::InputAttack()
{
    // コマンド操作で魔法コマンド
    if (magicAction)
        return false;

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
// 魔法発射
bool Player::InputMagick()
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return false;
    // コンポーネントを使えるように
    std::shared_ptr mpId = sharedId->GetComponent<Mp>();

    GamePad& gamePad = Input::Instance().GetGamePad();

    if (GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::QuickJab) &&
        model->GetCurrentAnimationIndex() == Anim_Magic&&
        model->GetCurrentAnimationIndex() == Anim_MagicSeconde)
        return false;

    // 魔法コマンド選択中では無かったら
    if (!magicAction)
    {
        StartMagicUiFire();
        return false;
    }
    // 通常コマンド操作
    if (!InputShortCutkeyMagic() &&
        gamePad.GetButton() & GamePad::BTN_B)
    {
        switch (selectMagicCheck)
        {
        case (int)CommandMagic::Fire:
        {
            // 魔法
                std::weak_ptr<TransForm2D> transform2DPush
                    = UiManager::Instance().GetUies(
                        (int)UiManager::UiCount::PlayerCommandFireCheck)
                    ->GetComponent<TransForm2D>();

                DirectX::XMFLOAT2 pos;
                pos = transform2DPush.lock()->GetPosition();

                float sizeMax;
                sizeMax = transform2DPush.lock()->GetScale().x;
                StartMagicUiCharge(pos, sizeMax);

                transform2DPush.lock()->SetPosition(pos);
                transform2DPush.lock()->SetScale(
                    { sizeMax,transform2DPush.lock()->GetScale().y });
                return true;
          
            break;
        }
        case (int)CommandMagic::Thander:
        {
            // 魔法
                std::weak_ptr<TransForm2D> transform2DPush
                    = UiManager::Instance().GetUies(
                        (int)UiManager::UiCount::PlayerCommandRigtningCheck)
                    ->GetComponent<TransForm2D>();

                DirectX::XMFLOAT2 pos;
                pos = transform2DPush.lock()->GetPosition();

                float sizeMax;
                sizeMax = transform2DPush.lock()->GetScale().x;
                StartMagicUiCharge(pos, sizeMax);

                transform2DPush.lock()->SetPosition(pos);
                transform2DPush.lock()->SetScale(
                    { sizeMax,transform2DPush.lock()->GetScale().y });

                return true;
            
            break;
        }
        case (int)CommandMagic::Ice:
        {
            // 魔法
                std::weak_ptr<TransForm2D> transform2DPush
                    = UiManager::Instance().GetUies(
                        (int)UiManager::UiCount::PlayerCommandIceCheck)
                    ->GetComponent<TransForm2D>();

                DirectX::XMFLOAT2 pos;
                pos = transform2DPush.lock()->GetPosition();

                float sizeMax;
                sizeMax = transform2DPush.lock()->GetScale().x;
                StartMagicUiCharge(pos, sizeMax);

                transform2DPush.lock()->SetPosition(pos);
                transform2DPush.lock()->SetScale(
                    { sizeMax,transform2DPush.lock()->GetScale().y });

                return true;
            
            break;
        }
        case (int)CommandMagic::Heale:
        {
            // 魔法
            return true;
            break;
        }
        default:
            break;
        }

    }
    //// 続けて押せない
    //if ()return false;
    switch (selectMagicCheck)
    {
    case (int)CommandMagic::Fire:
    {
        // 魔法
        if (gamePad.GetButton() & GamePad::BTN_B &&
            !gamePad.GetButtonDownCountinue())
        {
            std::weak_ptr<TransForm2D> transform2DPush
                = UiManager::Instance().GetUies(
                    (int)UiManager::UiCount::PlayerCommandShortCutFire)
                ->GetComponent<TransForm2D>();

            DirectX::XMFLOAT2 pos;
            pos = transform2DPush.lock()->GetPosition();

            float sizeMax;
            sizeMax = transform2DPush.lock()->GetScale().x;
            StartMagicUiCharge(pos, sizeMax);

            transform2DPush.lock()->SetPosition(pos);
            transform2DPush.lock()->SetScale(
                { sizeMax,transform2DPush.lock()->GetScale().y });
            return true;
        }
        // 魔法UI初期化
        else
        {
            bool isPush = false;
            gamePad.SetButtonDownCountinue(isPush);
            StartMagicUiFire();
        }
        break;
    }
    case (int)CommandMagic::Thander:
    {
        // 魔法
        if (gamePad.GetButton() & GamePad::BTN_X &&
            !gamePad.GetButtonDownCountinue())
        {
            std::weak_ptr<TransForm2D> transform2DPush
                = UiManager::Instance().GetUies(
                    (int)UiManager::UiCount::PlayerCommandShortCutSunder)
                ->GetComponent<TransForm2D>();

            DirectX::XMFLOAT2 pos;
            pos = transform2DPush.lock()->GetPosition();

            float sizeMax;
            sizeMax = transform2DPush.lock()->GetScale().x;
            StartMagicUiCharge(pos, sizeMax);

            transform2DPush.lock()->SetPosition(pos);
            transform2DPush.lock()->SetScale(
                { sizeMax,transform2DPush.lock()->GetScale().y });

            return true;
        }
        // 魔法UI初期化
        else
        {
            bool isPush = false;
            gamePad.SetButtonDownCountinue(isPush);
            StartMagicUiFire();
        }
        break;
    }
    case (int)CommandMagic::Ice:
    {
        // 魔法
        if (gamePad.GetButton() & GamePad::BTN_A &&
            !gamePad.GetButtonDownCountinue())
        {
            std::weak_ptr<TransForm2D> transform2DPush
                = UiManager::Instance().GetUies(
                    (int)UiManager::UiCount::PlayerCommandShortCutIce)
                ->GetComponent<TransForm2D>();

            DirectX::XMFLOAT2 pos;
            pos = transform2DPush.lock()->GetPosition();

            float sizeMax;
            sizeMax = transform2DPush.lock()->GetScale().x;
            StartMagicUiCharge(pos, sizeMax);

            transform2DPush.lock()->SetPosition(pos);
            transform2DPush.lock()->SetScale(
                { sizeMax,transform2DPush.lock()->GetScale().y });

            return true;
        }
        // 魔法UI初期化
        else
        {
            bool isPush = false;
            gamePad.SetButtonDownCountinue(isPush);
            StartMagicUiFire();
        }
        break;
    }
    case (int)CommandMagic::Heale:
    {
        // 魔法
        if (gamePad.GetButton() & GamePad::BTN_Y &&
            !gamePad.GetButtonDownCountinue())
        {
            return true;
        }
        else
        {
            bool isPush = false;
            gamePad.SetButtonDownCountinue(isPush);
        }
        break;
    }
    default:
    {

        break;
    }
    }
  
    // ボタンから手を離したら
    StartMagicUiFire();
    return false;
}

// 魔法火発射
bool Player::InputMagicframe()
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return false;
    // 移動コンポーネント
    std::shared_ptr mpId = sharedId->GetComponent<Mp>();

    GamePad& gamePad = Input::Instance().GetGamePad();
        // mp消費
        mpId->ApplyConsumption(magicConsumption);
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
        }
        return true;
}
// 魔法氷発射
bool Player::InputMagicIce()
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return false;
    // 移動コンポーネント
    std::shared_ptr mpId = sharedId->GetComponent<Mp>();


    GamePad& gamePad = Input::Instance().GetGamePad();
   

    // mp消費
    mpId->ApplyConsumption(magicConsumption);
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
    }
    return true;
}
// 魔法雷発射
bool Player::InputMagicLightning()
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return false;
    // 移動コンポーネント
    std::shared_ptr mpId = sharedId->GetComponent<Mp>();

    // mp消費
    mpId->ApplyConsumption(magicConsumption);
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
    {
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
    }
    return true;
}

// 回復魔法開始
bool Player::InputMagicHealing()
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return false;
    // 移動コンポーネント
    std::shared_ptr mpId = sharedId->GetComponent<Mp>();
    std::shared_ptr hpId = sharedId->GetComponent<HP>();

    // mp消費
    mpId->ApplyConsumption(mpId->GetMaxMagic());
    hpId->AddHealth(healing);
    return true;
}

// 連射用
void Player::PushMagicFrame(DirectX::XMFLOAT3 angle)
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // 移動コンポーネント
    std::shared_ptr mpId = sharedId->GetComponent<Mp>();

    GamePad& gamePad = Input::Instance().GetGamePad();
    // mp消費
    mpId->ApplyConsumption(magicConsumption);
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
// 後変更連射用氷
// 連射用
void Player::PushMagicIce(DirectX::XMFLOAT3 angle)
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // 移動コンポーネント
    std::shared_ptr mpId = sharedId->GetComponent<Mp>();


    GamePad& gamePad = Input::Instance().GetGamePad();
    // mp消費
    mpId->ApplyConsumption(magicConsumption);
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
// 後変更必殺技炎
// 連射用
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
// 距離でUIを変えるロックオン中
void Player::AttackCheckUI()
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // 移動コンポーネント
    std::shared_ptr collisionId = sharedId->GetComponent<Collision>();

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
            if (collisionId->IntersectSphereVsCylinder(
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

// 後変更角度
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

// 後変更UI動作
void Player::UiControlleGauge(float elapsedTime)
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // 移動コンポーネント
    std::shared_ptr mpId = sharedId->GetComponent<Mp>();
    std::shared_ptr hpId = sharedId->GetComponent<HP>();


    int uiCount = UiManager::Instance().GetUiesCount();
    // ui無かったら
    if (uiCount <= uiCountMax) return;

    // hpゲージ操作用
    float gaugeWidth = hpId->GetMaxHealth() * hpId->GetHealth() * 0.08f;
    // hpゲージ
    std::shared_ptr<TransForm2D> uiHp = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerHPBar)->GetComponent<TransForm2D>();
    std::shared_ptr<TransForm2D> uiHpBar = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerHp)->GetComponent<TransForm2D>();
    
    // hpゲージUI　変える
    DirectX::XMFLOAT2 scale = { gaugeWidth, uiHp->GetScale().y };
    uiHp->SetScale(scale);
    gaugeWidth = mpId->GetMaxMagic() * mpId->GetMagic() * 0.1f;
    // mpゲージ
    std::shared_ptr<TransForm2D> uiMp = UiManager::Instance().GetUies((int)UiManager::UiCount::Mp)->GetComponent<TransForm2D>();
    std::shared_ptr<Ui> uiColor = UiManager::Instance().GetUies((int)UiManager::UiCount::Mp)->GetComponent<Ui>();
    
    // mpゲージUI　変える
    scale = { gaugeWidth, uiMp->GetScale().y };
    uiMp->SetScale(scale);
    // mp色
   mpUiColor = { 1,1,1,1 };
    if (mpId->GetMpEmpth())
    {
        mpUiColor = { 1,0.5f,0,1 };
    }
   uiColor->SetColor(mpUiColor);
    // 揺れ
    if (shakeMode)
    {
        uiHp->Shake();
        uiHpBar->Shake();
    }
    //　初期化
    if (uiHp->GetShakeEnd())
    {
        shakeMode = false; 
    }
}

// 後変更
// 特殊攻撃ダメージ判定
void Player::SpecialApplyDamageInRadius()
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // 移動コンポーネント
    std::shared_ptr collisionId = sharedId->GetComponent<Collision>();

    // 敵について
    EnemyManager& enemyManager = EnemyManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();

    if (enemyCount <= 0) return;

    std::shared_ptr<Actor> enemyShader = enemyManager.GetEnemy((int)EnemyManager::EnemyType::Boss);
    if (!enemyShader) return;

    std::shared_ptr<EnemyBoss> enemyBoss = enemyShader->GetComponent<EnemyBoss>();
    std::shared_ptr<Transform> enemyTransform = enemyShader->GetComponent<Transform>();
    std::shared_ptr<Collision> enemyCollision = enemyShader->GetComponent<Collision>();
    std::shared_ptr<HP> enemyHp = enemyShader->GetComponent<HP>();
    std::shared_ptr<ModelControll> enemyModel = enemyShader->GetComponent<ModelControll>();

    DirectX::XMFLOAT3 enemyPosition = enemyTransform->GetPosition();
    float enemyRudius = enemyCollision->GetRadius();
    float enemyHeight = enemyCollision->GetHeight();
    ProjectileManager& projectileManager = ProjectileManager::Instance();
    for (int i = 0; i < projectileManager.GetProjectileCount(); ++i)
    {
        std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        if (!projectile->GetComponent<ProjectileTornade>()) return;
        // 魔法位置
        DirectX::XMFLOAT3 magicPosition = projectile->GetComponent<Transform>()->GetPosition();
        float magicRadius = projectile->GetComponent<Collision>()->GetRadius();
        float magicHeight = projectile->GetComponent<Collision>()->GetHeight();
        //// 衝突処理
        DirectX::XMFLOAT3 outPositon;
        // 下半身
        // 円柱と円
        if (!collisionId->IntersectCylinderVsCylinder(
            magicPosition,
            magicRadius,
            magicHeight,
            enemyPosition,
            enemyRudius,
            enemyHeight,
            outPositon)) return;


        // ダメージが通ったら消える。TRUEになるから
        if (!enemyHp->ApplyDamage(applyDamageSpecial, 0.5f)) return;
        // ヒットエフェクト再生
        hitEffect->Play(enemyPosition);
        hitFire->Play(enemyPosition);

    }
}
// 地面に立っているか
bool Player::Ground()
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return false;
    // 移動コンポーネント
    std::shared_ptr movementId = sharedId->GetComponent<Movement>();

    // 地面の立つ
    if (movementId->GetOnLadius())
    {
        return true;
    }
    return false;
}

// 空中行動の経過時間
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

// Ui魔法チャージ動作開始
void Player::StartMagicUiCharge(DirectX::XMFLOAT2& pos, float& gaugeSizeMax)
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // 移動コンポーネント
    std::shared_ptr mpId = sharedId->GetComponent<Mp>();

    // mp切れ
    if (mpId->GetMpEmpth())
    {
        StartMagicUiFire();
        return;
    }

    // ゲージ溜め
    playerCommandPushUiChargeTime += commandChargeAddMin;

    // 後変更UIコマンド見にくい
    // 描画
    {
        std::weak_ptr<Ui> pushUi2D
            = UiManager::Instance().GetUies(
                (int)UiManager::UiCount::PlayerCommandPush)
            ->GetComponent<Ui>();
        pushUi2D.lock()->SetDrawCheck(isDrawUi);

        std::weak_ptr<Ui> pushNowUi
            = UiManager::Instance().GetUies(
                (int)UiManager::UiCount::PlayerCommandPushNow)
            ->GetComponent<Ui>();
        pushNowUi.lock()->SetDrawCheck(isDrawUi);

        std::weak_ptr<Ui> pushUiCharge
            = UiManager::Instance().GetUies(
                (int)UiManager::UiCount::PlayerCommandCharge)
            ->GetComponent<Ui>();
        pushUiCharge.lock()->SetDrawCheck(isDrawUi);

        // 位置大きさ
        std::weak_ptr<TransForm2D> transform2DPush
            = UiManager::Instance().GetUies(
                (int)UiManager::UiCount::PlayerCommandPush)
            ->GetComponent<TransForm2D>();
        transform2DPush.lock()->SetPosition(pos);
        transform2DPush.lock()->SetScale(
            { gaugeSizeMax,transform2DPush.lock()->GetScale().y });

        std::weak_ptr<TransForm2D> transform2DPushNow
            = UiManager::Instance().GetUies(
                (int)UiManager::UiCount::PlayerCommandPushNow)
            ->GetComponent<TransForm2D>();
        transform2DPushNow.lock()->SetPosition(pos);


        std::weak_ptr<TransForm2D> transform2DCharge
            = UiManager::Instance().GetUies(
                (int)UiManager::UiCount::PlayerCommandCharge)
            ->GetComponent<TransForm2D>();
        transform2DCharge.lock()->SetPosition(pos);
        // 溜め
        float gaugeWidth = gaugeSizeMax * playerCommandPushUiChargeTime * 0.08f;

        transform2DPushNow.lock()->SetScale(
            { gaugeWidth,transform2DPushNow.lock()->GetScale().y });
    }
}

// 後変更コマンド見にくい
// Ui魔法チャージ動作発射
void Player::StartMagicUiFire()
{
    // 初期化
    playerCommandPushUiChargeTime = 0.0f;

    std::weak_ptr<Ui> pushUi2D
        = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandPush)
        ->GetComponent<Ui>();
    pushUi2D.lock()->SetDrawCheck(isDrawUiEmpth);

    std::weak_ptr<Ui> pushNowUi
        = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandPushNow)
        ->GetComponent<Ui>();
    pushNowUi.lock()->SetDrawCheck(isDrawUiEmpth);

    std::weak_ptr<Ui> pushUiCharge
        = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandCharge)
        ->GetComponent<Ui>();
    pushUiCharge.lock()->SetDrawCheck(isDrawUiEmpth);

}

// 後変更デリートマネージャー
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

