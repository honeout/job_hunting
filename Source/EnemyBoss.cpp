#include <imgui.h>
#include "EnemyBoss.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
#include "StateDerived.h"
#include "Audio/Audio.h"
#include "TransForm2D.h"
#include "ProjectileImpact.h"
#include "UiManager.h"
#include "Ui.h"

//

// デストラクタ
EnemyBoss::~EnemyBoss()
{
   
}

void EnemyBoss::Start()
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

    // モデルデータを入れる。
    model = GetActor()->GetComponent<ModelControll>()->GetModel();
    hpId->SetHealth(health);
    hpId->SetMaxHealth(maxHealth);
    collisionId->SetRadius(radius);
    collisionId->SetPartRadius(partRadius);
    collisionId->SetHeight(height);
    collisionId->SetSecondesHeight(confusionHeight);

    // エフェクト
    moveAttackEffect = std::make_unique<Effect>("Data/Effect/enemyMoveAttackHit.efk");
    awakeEffect = std::make_unique<Effect>("Data/Effect/awake.efk");
    inpactEffect = std::make_unique<Effect>("Data/Effect/hit fire.efk");

    // アニメーションルール
    updateanim = UpAnim::Normal;
    // 上半身スタート再生開始場所
    bornUpStartPoint = "body3";
    // 上半身エンド再生停止場所
    bornUpEndPoint = "body2";
    // 下半身スタート再生開始場所
    bornDownerStartPoint = "body2";
    // 下半身エンド再生停止場所
    bornDownerEndPoint = "boss_left_eye";

    // 当たり判定無効判定
    invalidJudgment = true;

    // playerカウンター用
    counterJudgment = false;

    // 攻撃右足するかどうか
    attackRightFootRange = 1.5f;

    // 動作チェック
    moveCheck = true;

    // 衝撃波起こる範囲外側
    radiusInpactOutSide = 0.3f;

    // 衝撃波起こる範囲内側
    radiusInpactInSide = 0.3f;

    // 衝撃波高さ
    radiusInpactHeight = 0.3f;
}

// 更新処理
void EnemyBoss::Update(float elapsedTime)
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;

    // コンポーネントを使えるように
    std::shared_ptr transformId = sharedId->GetComponent<Transform>();
    std::shared_ptr movementId = sharedId->GetComponent<Movement>();
    std::shared_ptr hpId = sharedId->GetComponent<HP>();

    // 動作するかどうか
    if (moveCheck)
    // ステート毎の処理
    stateMachine->Update(elapsedTime);

    // 敵覚醒管理
    ManageAwakeTime(elapsedTime);

    // 位置
    position = transformId->GetPosition();
    // 向き
    angle = transformId->GetAngle();
    // 大きさ
    scale = transformId->GetScale();

    // 速力処理更新
    movementId->UpdateVelocity(elapsedTime);

    // 無敵時間更新
    hpId->UpdateInbincibleTimer(elapsedTime);

    // 当たり判定衝撃波とプレイヤー
    CollisionImpactVsPlayer();

    // 削除
    ProjectileManager::Instance().DeleteUpdate(elapsedTime);//todo いるの？

    // ワールド位置
    transformId->UpdateTransform();

    // ダメージ点滅
    OnHit(elapsedTime);

    // 動作するかどうか
    if (moveCheck)
    // モーション更新処理
    switch (updateanim)
    {
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
            model->ReverseplaybackAnimation(elapsedTime, true);
            break;
        }
    }

    // 姿勢
    model->UpdateTransform(transformId->GetTransform());
    
    // ゲージ管理
    UiControlle(elapsedTime);
}

void EnemyBoss::Render(RenderContext& rc, ModelShader& shader)
{
        Graphics& graphics = Graphics::Instance();
        // 赤色
        rc.color = colorGB;
        // スペキュラー無効化
        rc.isSpecular = isSpecular;
        // 影オンオフ
        rc.isRimRightning = isRimRightning;
        // 描画オンオフ
        rc.StencilRef = StencilRef;
        shader.Begin(rc);// シェーダーにカメラの情報を渡す
        shader.Draw(rc, model);
        shader.End(rc);
}
#ifdef _DEBUG
void EnemyBoss::OnGUI()
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;

    std::shared_ptr hpId = sharedId->GetComponent<HP>();

    if(ImGui::Button("drawtrue"))
    {
        modelDrawCheck = true;
    }
    if (ImGui::Button("drawfalse"))
    {
        modelDrawCheck = false;
    }
    if (ImGui::Button("isEnemyAwakened"))
    {
        ResetAwakeTime();
        bool check = isEnemyAwakened ? true : false;
        // 耐久力追加
        hpId->SetIsBonusHpActive(check);
    }
    if (ImGui::Button("StartAwake"))
    {
        stateMachine->ChangeState(static_cast<int>(State::AwakeStart));
    }
    if (ImGui::Button("stateStop"))
    {
        moveCheck = !moveCheck;
    }
}
#endif // _DEBUG

// デバッグプリミティブ描画
void EnemyBoss::DrawDebugPrimitive()
{
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // 縄張り範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f,
        DirectX::XMFLOAT4(0, 1, 0, 1));

    debugRenderer->DrawCylinder(
        {
            position.x,
            position.y + height / 2,
            position.z,
        }, radius, height / 2, DirectX::XMFLOAT4(1, 0, 0, 1));

    // ターゲット位置をデバッグ球描画
    debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1, 1, 0, 1));
    
    // 攻撃右足するかどうか
    debugRenderer->DrawSphere(position, attackRightFootRange, DirectX::XMFLOAT4(1, 0, 1, 1));
    
    // 索敵範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0, 0, 1, 1));

    // 攻撃範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(position, attackRange, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));

    debugRenderer->DrawSphere(position, 10, DirectX::XMFLOAT4(0, 1, 0, 1));

    debugRenderer->DrawSphere(position, 3, DirectX::XMFLOAT4(0, 1, 0, 1));

    debugRenderer->DrawCylinder(targetPosition, radius, height, DirectX::XMFLOAT4(1, 1, 0, 1));
}
// se再生
void EnemyBoss::InputSe(AudioParam param)
{
    Audio& Se = Audio::Instance();
    Se.Play(param);
}
void EnemyBoss::PlaySe(const std::string& filename)
{
    Audio& Se = Audio::Instance();
    AudioParam audioParam;
    audioParam.filename = filename;
    audioParam.loop = isLoopDisabled;
    audioParam.volume = seVolume;
    Se.Play(audioParam);
}
// se停止
void EnemyBoss::StopSe(const std::string& filename)
{
    Audio& Se = Audio::Instance();
    // 種類停止
    Se.Stop(filename);
}

// 後変更Collision
// 足踏み(衝撃波)の当たり判定
void EnemyBoss::CollisionImpactVsPlayer()
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    std::shared_ptr collisionId = sharedId->GetComponent<Collision>();
    PlayerManager& playerManager = PlayerManager::Instance();

    ProjectileManager& projectileManager = ProjectileManager::Instance();

    // 全ての敵と総当たりで衝突処理
    int playerCount = playerManager.GetPlayerCount();//todo 外
    int projectileCount = projectileManager.GetProjectileCount();

    // player所持無し
    if (playerCount <= 0) return;

    // 安全チェック
    std::shared_ptr<Actor> player = playerManager.GetPlayer((int)PlayerManager::PlayerType::Main);
    if (!player)
        return;
    // player関係
    std::shared_ptr<Player> playerMain = player->GetComponent<Player>();
    std::shared_ptr<Transform> playerTransform = player->GetComponent<Transform>();
    std::shared_ptr<Movement> playerMovement = player->GetComponent<Movement>();
    std::shared_ptr<Collision> playerCollision = player->GetComponent<Collision>();
    std::shared_ptr<HP> playerHp = player->GetComponent<HP>();
    // 位置、半径、高さ
    DirectX::XMFLOAT3 playerPosition = playerTransform->GetPosition();
    float playerRadius = playerCollision->GetRadius();
    float playerHeight = playerCollision->GetHeight();

    for (int i = 0; i < projectileCount; ++i)
    {
        std::weak_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        if (!projectile.lock()->GetComponent<ProjectileImpact>()) return;

        DirectX::XMFLOAT3 projectilePosition =
            projectile.lock()->GetComponent<Transform>()->GetPosition();

        // 身長
        float height = 1.0f;
        projectile.lock()->GetComponent<Collision>()->SetHeight(height);
        float projectileHeight = projectile.lock()->GetComponent<Collision>()->GetHeight();
        float projectileRadiusOutLine = projectile.lock()->GetComponent<ProjectileImpact>()->GetRadiusOutSide();
        float projectileRadiusInLine = projectile.lock()->GetComponent<ProjectileImpact>()->GetRadiusInSide();
        // 衝突処理
        DirectX::XMFLOAT3 outPositon;

        // インパクトの疑似的判定
        // 円柱と円
        if (collisionId->IntersectSphereVsCylinder(
            projectilePosition,
            projectileRadiusOutLine,
            playerPosition,
            playerRadius,
            playerHeight,
            outPositon) &&
            !collisionId->IntersectSphereVsCylinder(
                projectilePosition,
                projectileRadiusInLine,
                playerPosition,
                playerRadius,
                playerHeight,
                outPositon))
        {

            // 高さが一定以下なら通る
            if (projectilePosition.y + projectileHeight < playerPosition.y) return;
            // ダメージを与える。
            if (!playerHp->ApplyDamage(applyDamageImpact, impactInvincibleTime)) return;

            playerMain->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Damage));

            // 衝撃波音
            PlaySe("Data/Audio/SE/衝撃波ヒット.wav");

            // 吹き飛ばす
            {
                // 衝動
                DirectX::XMFLOAT3 impulse;
                // 衝撃
                const float power = 10.0f;

                float vx = playerPosition.x - projectilePosition.x;
                float vz = playerPosition.z - projectilePosition.z;
                float lengthXZ = sqrtf(vx * vx + vz * vz);
                vx /= lengthXZ;
                vz /= lengthXZ;

                impulse.x = vx * power;
                impulse.y = power * 0.5f;
                impulse.z = vz * power;

                playerMovement->AddImpulse(impulse);
            }
            // ヒットエフェクト再生
            {
                playerPosition.y += playerHeight * 0.5f;

                bool loopSe = false;
            }
            // UI揺れ
            playerMain->SetShakeMode(true);

            // 振動
            StartDamageShake();
        }
    }
}

// 後変更Collision
// 衝撃波
void EnemyBoss::CollisionInpact()
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;

    std::shared_ptr collisionId = sharedId->GetComponent<Collision>();

    // 衝撃波の有無
    if (!IsInpact) return;

    ProjectileManager& projectileManager = ProjectileManager::Instance();
    // 左足のボーン名
    Model::Node* bossLeftFoot = model->FindNode("boss_left_foot1");
    // ノード位置取得
    // 左足
    DirectX::XMFLOAT3 bossLeftFootPosition;
    bossLeftFootPosition = model->ConvertLocalToWorld(bossLeftFoot);

    DetectHitByBodyPart(bossLeftFootPosition, applyDamageStamp);

    // 当たり判定増大
    radiusInpactInSide += 0.3f;

    // 当たり判定増大
    radiusInpactOutSide += 0.3f;

    // 当たり判定増大高さ
    radiusInpactHeight += 0.3f;

    PlayerManager& playerManager = PlayerManager::Instance();

    // 全ての敵と総当たりで衝突処理
    int playerCount = playerManager.GetPlayerCount();

    // player所持無し
    if (playerCount <= 0) return;

    // 安全チェック
    std::shared_ptr<Actor> player = playerManager.GetPlayer((int)PlayerManager::PlayerType::Main);
    if (!player)
        return;
    // player関係
    std::shared_ptr<Player> playerMain = player->GetComponent<Player>();
    std::shared_ptr<Transform> playerTransform = player->GetComponent<Transform>();
    std::shared_ptr<Movement> playerMovement = player->GetComponent<Movement>();
    std::shared_ptr<Collision> playerCollision = player->GetComponent<Collision>();
    std::shared_ptr<HP> playerHp = player->GetComponent<HP>();
    // 位置、半径、高さ
    DirectX::XMFLOAT3 playerPosition = playerTransform->GetPosition();
    float playerRadius = playerCollision->GetRadius();
    float playerHeight = playerCollision->GetHeight();

    // 衝突処理
    DirectX::XMFLOAT3 outPositon;
    // 円柱と円
    if (collisionId->IntersectSphereVsCylinder(
        bossLeftFootPosition,
        radiusInpactOutSide,
        playerPosition,
        playerRadius,
        playerHeight,
        outPositon) &&
        !collisionId->IntersectSphereVsCylinder(
            bossLeftFootPosition,
            radiusInpactInSide,
            playerPosition,
            playerRadius,
            playerHeight,
            outPositon))

    {
        // 高さが一定以下なら通る
        if (bossLeftFootPosition.y + radiusInpactHeight < playerPosition.y) return;
        // ダメージを与える。
        if (!playerHp->ApplyDamage(applyDamageImpact, impactInvincibleTime)) return;

        playerMain->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Damage));

        // 吹き飛ばす
        {
            // 衝動
            DirectX::XMFLOAT3 impulse;
            // 衝撃
            const float power = 10.0f;
            float vx = playerPosition.x - bossLeftFootPosition.x;
            float vz = playerPosition.z - bossLeftFootPosition.z;
            float lengthXZ = sqrtf(vx * vx + vz * vz);
            vx /= lengthXZ;
            vz /= lengthXZ;
            impulse.x = vx * power;
            impulse.y = power * 0.5f;
            impulse.z = vz * power;
            playerMovement->AddImpulse(impulse);
        }
        // ヒットエフェクト再生
        {
            playerPosition.y += playerHeight * 0.5f;
        }
    }
}

// 後変更いるかどうか
// 敵覚醒管理
void EnemyBoss::ManageAwakeTime(float elapsedTime)
{
    // 覚醒中
    if (enemyAwakeningDuration >= enemyAwakeningDurationEnd)
    {
        enemyAwakeningDuration -= elapsedTime;
        // 暴走状態
        isEnemyAwakened = true;
    }
    // 覚醒終了
    else
    {
        // 暴走状態
        isEnemyAwakened = false;
    }
    // 覚醒エフェクト位置更新
    if (awakeEffect->GetEfeHandle())
    {
        awakeEffect->SetPosition(awakeEffect->GetEfeHandle(), position);
    }
}

// 後変更いるかどうか
// 敵覚醒時間初期化
void EnemyBoss::ResetAwakeTime()
{
    // 時間初期化
    enemyAwakeningDuration = enemyAwakeningDurationMax;
    // 覚醒中のエフェクト
    awakeEffect->Play(position);
}

// 後変更Collision
// パーツごとの当たり判定
void EnemyBoss::DetectHitByBodyPart(DirectX::XMFLOAT3 partBodyPosition, int applyDamage)
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;

    std::shared_ptr collisionId = sharedId->GetComponent<Collision>();

    PlayerManager& playerManager = PlayerManager::Instance();

    // 全ての敵と総当たりで衝突処理
    int playerCount = playerManager.GetPlayerCount();

    // 安全チェック
    std::shared_ptr<Actor> player = playerManager.GetPlayer((int)PlayerManager::PlayerType::Main);
    if (!player)
        return;
    // player関係
    std::shared_ptr<Player> playerMain = player->GetComponent<Player>();
    std::shared_ptr<Transform> playerTransform = player->GetComponent<Transform>();
    std::shared_ptr<Movement> playerMovement = player->GetComponent<Movement>();
    std::shared_ptr<Collision> playerCollision = player->GetComponent<Collision>();
    std::shared_ptr<HP> playerHp = player->GetComponent<HP>();
    // 位置、半径、高さ
    DirectX::XMFLOAT3 playerPosition = playerTransform->GetPosition();
    float playerRadius = playerCollision->GetRadius();
    float playerHeight = playerCollision->GetHeight();
    // 衝突処理
    DirectX::XMFLOAT3 outPositon;
    // 球と球
    if (collisionId->IntersectSphereVsCylinder(
        partBodyPosition,
        attackRightFootRange,
        playerPosition,
        playerRadius,
        playerHeight,
        outPositon))
    {
        // ダメージを与える。
        if (!playerHp->ApplyDamage(applyDamage, nuckleInvincibleTime)) return;

        // 斬撃音
        PlaySe("Data/Audio/SE/スラッシュ２回目.wav");

        // se再生
        AudioParam audioParam;
        audioParam.filename = "Data/Audio/SE/スラッシュ２回目.wav";
        audioParam.loop = isLoopDisabled;
        audioParam.volume = seVolume;
        InputSe(audioParam);


        DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&playerPosition);
        DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
        DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
        DirectX::XMFLOAT3 normal;
        DirectX::XMStoreFloat3(&normal, N);
        float jumpSpeed = 5.0f;
        //// 吹き飛ばす
        {
            // 衝動
            DirectX::XMFLOAT3 impulse;
            // 衝撃
            const float power = 10.0f;
            float vx = outPositon.x - playerPosition.x;
            float vz = outPositon.z - playerPosition.z;
            float lengthXZ = sqrtf(vx * vx + vz * vz);
            vx /= lengthXZ;
            vz /= lengthXZ;
            impulse.x = vx * power;
            impulse.y = power * 0.5f;
            impulse.z = vz * power;
            playerMain->GetStateMachine()->ChangeState((int)Player::State::Damage);
            playerMovement->AddImpulse(impulse);
            // エフェクト発生位置
            DirectX::XMFLOAT3 efcPos = playerPosition;
            efcPos.y += playerCollision->GetHeight();
            // ヒットエフェクト再生
            moveAttackEffect->Play(playerPosition);
            //SE
            bool loopSe = false;
            // UI揺れ
            playerMain->SetShakeMode(true);
            // 振動
            StartDamageShake();
        }
    }
}

void EnemyBoss::DetectHitByBodyAllPart(int applyDamage)
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;

    std::shared_ptr collisionId = sharedId->GetComponent<Collision>();
    std::shared_ptr modelId = sharedId->GetComponent<ModelControll>();

    PlayerManager& playerManager = PlayerManager::Instance();

    // 全ての敵と総当たりで衝突処理
    int playerCount = playerManager.GetPlayerCount();

    // 安全チェック
    std::shared_ptr<Actor> player = playerManager.GetPlayer((int)PlayerManager::PlayerType::Main);
    if (!player)
        return;
    // player関係
    std::shared_ptr<Player> playerMain = player->GetComponent<Player>();
    std::shared_ptr<Transform> playerTransform = player->GetComponent<Transform>();
    std::shared_ptr<Movement> playerMovement = player->GetComponent<Movement>();
    std::shared_ptr<Collision> playerCollision = player->GetComponent<Collision>();
    std::shared_ptr<HP> playerHp = player->GetComponent<HP>();
    // 位置、半径、高さ
    DirectX::XMFLOAT3 playerPosition = playerTransform->GetPosition();
    float playerRadius = playerCollision->GetRadius();
    float playerHeight = playerCollision->GetHeight();

    // パーツの種類
    Model::Node* nodePart;

    // パーツ全確認
    for (auto& part : hitSizes)
    {
        // パーツの種類
        nodePart = modelId->GetModel()->FindNode(bornPart.at(part.first));

        // 位置
        DirectX::XMFLOAT3 nodePosition;
        nodePosition = modelId->GetModel()->ConvertLocalToWorld(nodePart);

        // 衝突処理
        DirectX::XMFLOAT3 outPositon;
        // 球と球
        if (!collisionId->IntersectSphereVsCylinder(
            nodePosition,
            hitSizes.at(part.first),
            playerPosition,
            playerRadius,
            playerHeight,
            outPositon)) return;

        // ダメージを与える。
        if (!playerHp->ApplyDamage(applyDamage, nuckleInvincibleTime)) return;

        // 斬撃音
        PlaySe("Data/Audio/SE/スラッシュ２回目.wav");
        DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&playerPosition);
        DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
        DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
        DirectX::XMFLOAT3 normal;
        DirectX::XMStoreFloat3(&normal, N);
        float jumpSpeed = 5.0f;
        //// 吹き飛ばす
        {
            // 衝動
            DirectX::XMFLOAT3 impulse;
            // 衝撃
            const float power = 10.0f;
            float vx = outPositon.x - playerPosition.x;
            float vz = outPositon.z - playerPosition.z;
            float lengthXZ = sqrtf(vx * vx + vz * vz);
            vx /= lengthXZ;
            vz /= lengthXZ;
            impulse.x = vx * power;
            impulse.y = power * 0.5f;
            impulse.z = vz * power;
            playerMain->GetStateMachine()->ChangeState((int)Player::State::Damage);
            playerMovement->AddImpulse(impulse);
            // エフェクト発生位置
            DirectX::XMFLOAT3 efcPos = playerPosition;
            efcPos.y += playerCollision->GetHeight();
            // ヒットエフェクト再生
            moveAttackEffect->Play(playerPosition);
            //SE
            bool loopSe = false;
            // UI揺れ
            playerMain->SetShakeMode(true);
            // 振動
            StartDamageShake();
        }
    }
}

// 後変更衝撃波飛ばす
// 衝撃波発射
void EnemyBoss::InputImpact(DirectX::XMFLOAT3 pos)
{
    // 弾丸初期化
    const char* filename = "Data/Model/SpikeBall/SpikeBall.mdl";
    std::weak_ptr<Actor> actor = ActorManager::Instance().Create();
    actor.lock()->AddComponent<ModelControll>();
    actor.lock()->GetComponent<ModelControll>()->LoadModel(filename);
    actor.lock()->SetName("ProjectileImpact");
    actor.lock()->AddComponent<Transform>();
    actor.lock()->GetComponent<Transform>()->SetPosition(pos);
    actor.lock()->GetComponent<Transform>()->SetAngle(angle);
    actor.lock()->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f));
    actor.lock()->AddComponent<Collision>();
    actor.lock()->AddComponent<ProjectileImpact>();
    const char* effectFilename = "Data/Effect/inpact.efk";
    actor.lock()->GetComponent<ProjectileImpact>()->SetEffectProgress(effectFilename);
    // 生存時間
    float lifeTimer = 50.0f;
    actor.lock()->GetComponent<ProjectileImpact>()->SetLifeTimer(lifeTimer);
    // これが２Dかの確認
    bool check2d = false;
    actor.lock()->SetCheck2d(check2d);
    ProjectileManager::Instance().Register(actor.lock());
}

// 後変更 縄張り設定
// 縄張り設定
void EnemyBoss::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;
}

// 後変更 UIControlle
// 敵HPのUI
void EnemyBoss::UiControlle(float elapsedTime)
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // コンポーネントを使えるように
    std::shared_ptr hpId = sharedId->GetComponent<HP>();

    if (UiManager::Instance().GetUiesCount() <= uiCountMax)return;
    float gaugeWidth = hpId->GetMaxHealth() * hpId->GetHealth() * 0.12f;
    std::weak_ptr<TransForm2D> uiHp = UiManager::Instance().GetUies((int)UiManager::UiCount::EnemyHPBar)->GetComponent<TransForm2D>();
    DirectX::XMFLOAT2 scale = { gaugeWidth, uiHp.lock()->GetScale().y };
    uiHp.lock()->SetScale(scale);
    std::weak_ptr<Ui> uiHpLife1 = UiManager::Instance().GetUies((int)UiManager::UiCount::EnemyHPLife01)->GetComponent<Ui>();
    std::weak_ptr<Ui> uiHpLife2 = UiManager::Instance().GetUies((int)UiManager::UiCount::EnemyHPLife02)->GetComponent<Ui>();
    bool checkDraw = false;
    switch (hpId->GetLife())
    {
    case 1:
    {
        uiHpLife2.lock()->SetDrawCheck(checkDraw);
        break;
    }
    case 0:
    {
        uiHpLife1.lock()->SetDrawCheck(checkDraw);
        break;
    }
    default:
        break;
    }
}

// 後変更 エネミー点滅
// エネミー点滅
void EnemyBoss::OnHit(float elapsedTime)
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // コンポーネントを使えるように
    std::shared_ptr hpId = sharedId->GetComponent<HP>();

    // 点滅時間
    if (hpId->FlashTime(elapsedTime))
    {
        ++damageStateTime;
        if (damageStateTime >= damageStateTimeMax)
        {
            damageStateCheck = damageStateCheck ? false : true;
            damageStateTime = 0;
        }
        // 通常色
        if (damageStateCheck)
        {
            bool onHit = false;
            modelDrawCheck = onHit;
            colorGB.x += 0.1f;
            colorGB.y += 0.1f;
        }
        // 赤
        else
        {
            bool onHit = true;
            modelDrawCheck = onHit;
            colorGB.x -= 0.1f;
            colorGB.y -= 0.1f;
        }
    }
    else
    {
        damageStateTime = 0;
        bool onHit = true;
        modelDrawCheck = onHit;
        colorGB = { 1,1 };
    }
}

// 移動位置
void EnemyBoss::SetRandomTargetPosition()
{
    float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
    float range = Mathf::RandomRange(territoryRangeMin, territoryRange);
    targetPosition.x = territoryOrigin.x + sinf(theta)*range;
    targetPosition.y = territoryOrigin.y;
    targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

// 後変更　ターゲット位置まで移動
// ターゲット位置までの移動
void EnemyBoss::MoveToTarget(float elapsedTime, float speedRate)
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // コンポーネントを使えるように
    std::shared_ptr movementId = sharedId->GetComponent<Movement>();

    // ターゲット方向への進行ベクトルを算出
    float vx = targetPosition.x - position.x;
    float vy = 0.0f;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;
    // 移動処理
    movementId->Move({ vx,vy ,vz }, moveSpeed * speedRate, elapsedTime);
    movementId->Turn({ vx,vy ,vz } ,turnSpeed,elapsedTime);
}

// 後変更　回転
// 目的方向への回転
void EnemyBoss::TurnToTarget(float elapsedTime, float speedRate)
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // コンポーネントを使えるように
    std::shared_ptr movementId = sharedId->GetComponent<Movement>();

    float vx = targetPosition.x - position.x;
    float vy = 0.0f;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;
    // 回転
    movementId->Turn({ vx,vy ,vz }, turnSpeed, elapsedTime);
}

// ジャンプ
void EnemyBoss::InputJump()
{
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)
        return;
    // コンポーネントを使えるように
    std::shared_ptr movementId = sharedId->GetComponent<Movement>();

    // 強制停止
    if (position.y >= jumpLimit)
    {
        jumpSpeed = 0;
        movementId->JumpVelocity(jumpSpeed);
    }

    if (movementId->GetOnLadius())
    {
        movementId->JumpVelocity(jumpSpeedMin);
    }
}

// プレイヤーの位置を探す。
bool EnemyBoss::SearchPlayer()
{
    // プレイヤー取得
    std::weak_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount()-1);
    //　トランスフォーム分解
    DirectX::XMFLOAT3 playerPosition = playerid.lock()->GetComponent<Transform>()->GetPosition();
    float playerRadius = playerid.lock()->GetComponent<Collision>()->GetRadius();
    float playerHeight = playerid.lock()->GetComponent<Collision>()->GetRadius();
    float vx = playerPosition.x - position.x;
    float vy = playerPosition.y - position.y;
    float vz = playerPosition.z - position.z;
    // ルート
    float dist = sqrtf(vx * vx + vz * vz);
    if (dist < searchRange)
    {
        float distXZ = sqrtf(vx * vx + vz * vz);
        // 単位ベクトル化
        vx /= distXZ;
        vz /= distXZ;
        // 前方ベクトル
        float frontX = sinf(angle.y);
        float frontZ = cosf(angle.y);
        // ２つのベクトルの内積値で前後判定
        float dot = (frontX * vx) + (frontZ * vz);
        //0.070；90度
        //0.0f；半分以上
        if (dot > 0.0f)
        {
            return true;
        }
    }
    return false;
}

// エネミー削除
void EnemyBoss::Destroy()
{
    EnemyManager::Instance().Remove(GetActor());
}

// 画面を揺らす
void EnemyBoss::StartDamageShake()
{
    // シェイク時間 パワー
    MessageData::CAMERASHAKEDATA p;
    p.shakePower = shakePower;
    p.shakeTimer = shakeTimer;
    Messenger::Instance().SendData(MessageData::CAMERASHAKE, &p);
    PostprocessingRenderer postprocessingRenderer;
    damageDistortion.radius = 300.0f;
    damageDistortion.mask_radius = 200.0f;
    postprocessingRenderer.SetRadialBlurMaxData(damageDistortion);
}

// 削除更新
void EnemyManager::DeleteUpdate(float elapsedTime)
{
    // 破棄処理 毎フレームここで一気に消す。
    for (std::shared_ptr<Actor> enemy : removes)// リストを消す
    {
        std::vector<std::shared_ptr<Actor>>::iterator it = std::find(enemies.begin(), enemies.end(),
            enemy);
        if (it != enemies.end())
        {
            enemies.erase(it);// 削除
        }
    }
    // 破棄リストをクリア
    removes.clear();
}

// エネミー作成
void EnemyManager::Register(std::shared_ptr<Actor> actor)
{
    enemies.emplace_back(actor);
}

// 全削除
void EnemyManager::Clear()
{
    for (std::shared_ptr<Actor>& actor : enemies)
    {
        // 実体を消した管理している数はそのまま
        actor.reset();
    }
    enemies.clear();
}

// 削除登録
void EnemyManager::Remove(std::shared_ptr<Actor> actor)
{
    // 削除登録
    removes.insert(actor);
}
