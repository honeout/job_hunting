#include <imgui.h>
#include "EnemyBoss.h"
#include "Graphics/Graphics.h"
#include "Math\Mathf.h"
#include "Player.h"
#include "State\StateDerived.h"
#include "Audio/Audio.h"
#include "Component\TransForm2D.h"
#include "Projectile\ProjectileImpact.h"
#include "Ui\UiManager.h"
#include "Ui\Ui.h"
#include "Projectile\MagicConfig.h"

// デストラクタ
EnemyBoss::~EnemyBoss()
{
}

// 初期化
void EnemyBoss::Start()
{
    // コンポーネント初期化
    InitComponents();
    // エフェクト初期化
    InitEffects();
    // ステータス初期化
    InitStats();
}

// 更新処理
void EnemyBoss::Update(float elapsedTime)
{
    // ステート
    UpdateStateMachine(elapsedTime);
    // 状態
    UpdateStatus(elapsedTime);
    // 物理挙動  
    UpdatePhysics(elapsedTime);
    // 当たり判定
    HandleCollisions(elapsedTime);
    // アニメーション更新
    UpdateAnimation(elapsedTime);
}

void EnemyBoss::Render(RenderContext& rc, ModelShader& shader)
{
    // Lockとして実体を使う
    auto modelId = model.lock();

    // 有効性チェック
    if (!modelId)
        return;

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
    shader.Draw(rc, modelId->GetModel());
    shader.End(rc);
}
#ifdef _DEBUG
void EnemyBoss::OnGUI()
{
    auto hpId = hp.lock();
    // 安全☑
    if (!hpId) return;

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
    // デバッグ用当たり判定半径
    const float kRadius = EnemyConfig::kBaseBodyRadius;
    const float kHeight = EnemyConfig::kHeight;


    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // 縄張り範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, EnemyConfig::kDebugCylinderHeight,
        EnemyConfig::kColorGreen);

    debugRenderer->DrawCylinder(
        {
            position.x,
            position.y + kHeight / 2,
            position.z,
        }, kRadius, kHeight / 2, EnemyConfig::kColorRed);

    // ターゲット位置をデバッグ球描画
    debugRenderer->DrawSphere(targetPosition, kRadius, EnemyConfig::kColorYellow);
    
    // 攻撃右足するかどうか
    debugRenderer->DrawSphere(position, attackRightFootRange, EnemyConfig::kColorMagenta);
    
    // 索敵範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(position, searchRange, EnemyConfig::kDebugCylinderHeight, EnemyConfig::kColorBlue);

    // 攻撃範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(position, attackRange, EnemyConfig::kDebugCylinderHeight, EnemyConfig::kColorRed);

    debugRenderer->DrawSphere(position, EnemyConfig::kDebugSphereLargeRadius, EnemyConfig::kColorGreen);

    debugRenderer->DrawSphere(position, EnemyConfig::kDebugSphereSmallRadius, EnemyConfig::kColorGreen);

    debugRenderer->DrawCylinder(targetPosition, kRadius, EnemyConfig::kHeight, EnemyConfig::kColorYellow);
}

// コンポーネント初期化
void EnemyBoss::InitComponents()
{
    // コンポーネントを使えるように
    movement = GetActor()->GetComponent<Movement>();
    hp = GetActor()->GetComponent<HP>();
    transform = GetActor()->GetComponent<Transform>();
    collision = GetActor()->GetComponent<Collision>();
    model = GetActor()->GetComponent<ModelControll>();

    // Lockとして実体を使う
    auto movementId = movement.lock();
    auto hpId = hp.lock();
    auto transformId = transform.lock();
    auto collisionId = collision.lock();
    auto modelId = model.lock();


    // 有効性チェック
    if (!movementId || !hpId || !transformId || !collisionId || !modelId)
        return;
}

// エフェクト初期化
void EnemyBoss::InitEffects()
{
    // エフェクト
    moveAttackEffect = std::make_unique<Effect>("Data/Effect/enemyMoveAttackHit.efk");
    awakeEffect = std::make_unique<Effect>("Data/Effect/awake.efk");
    inpactEffect = std::make_unique<Effect>("Data/Effect/hit fire.efk");
    // 混乱エフェクト
    confusionEffect = std::make_unique<Effect>("Data/Effect/confusion.efk");
}

// ステータス初期化
void EnemyBoss::InitStats()
{
    // Lockとして実体を使う
    auto hpId = hp.lock();
    auto collisionId = collision.lock();
    auto modelId = model.lock();

    // 有効性チェック
    if (!hpId || !collisionId || !modelId)
        return;

    int health = EnemyConfig::kHealth;

    hpId->SetHealth(health);
    hpId->SetMaxHealth(health);

    // アニメーションルール
    updateanim = UpAnim::Normal;

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

    // ダメージ食らった時
    onDamageTime = EnemyConfig::onDamageTimeMin;

    // Tpos対策アニメーション初期化を適当なモーションに
    modelId->GetModel()->UpdateAnimation(0.0f, false);

    // 死亡確認
    isDead = false;

    // クリアシーンに
    isSceneChange = false;

    // audioの個々設定
    // 初期化
    paramSe = AudioParam::Se();
}

// ステート更新まとめ
void EnemyBoss::UpdateStateMachine(float elapsedTime)
{
    // 動作するかどうか
    if (moveCheck)
        // ステート毎の処理
        stateMachine->Update(elapsedTime);

    // 敵覚醒管理
    ManageAwakeTime(elapsedTime);
}

// プレイヤー状態制御
void EnemyBoss::UpdateStatus(float elapsedTime)
{
    // Lockとして実体を使う
    auto hpId = hp.lock();

    // 有効性チェック
    if (!hpId)
        return;

    // 無敵時間更新
    hpId->UpdateInbincibleTimer(elapsedTime);

    // hpが無くなったら、ライフで復活
    if (hpId->GetHealth() <= EnemyConfig::healthMin && hpId->GetLife() >= EnemyConfig::lifeMin)
    {
        // hpを回復
        hpId->SetHealth(hpId->GetMaxHealth());
    }

    // 削除
    ProjectileManager::Instance().DeleteUpdate(elapsedTime);

    // ゲージ管理
    UiControlle(elapsedTime);

    // クリアシーンへ行く
    if (hpId->DeadStart())
    {
        // 死亡状態
        isDead = true;
        stateMachine->ChangeState(static_cast<int>(State::Death));
    }
}

// 物理挙動
void EnemyBoss::UpdatePhysics(float elapsedTime)
{
    // Lockとして実体を使う
    auto movementId = movement.lock();
    auto transformId = transform.lock();

    // 有効性チェック
    if (!movementId || !transformId)
        return;

    // 位置
    position = transformId->GetPosition();

    // 向き
    angle = transformId->GetAngle();

    // 大きさ
    scale = transformId->GetScale();

    // 速力処理更新
    movementId->UpdateVelocity(elapsedTime);
}

// 当たり判定処理
void EnemyBoss::HandleCollisions(float elapsedTime)
{
    // 当たり判定衝撃波とプレイヤー
    CollisionImpactVsPlayer();

    // ダメージ点滅
    OnHit(elapsedTime);
}

// アニメーションの再生や状態切り替え
void EnemyBoss::UpdateAnimation(float elapsedTime)
{
    // Lockとして実体を使う
    auto movementId = movement.lock();
    auto transformId = transform.lock();
    auto modelId = model.lock();

    // 有効性チェック
    if (!movementId || !transformId || !modelId)
        return;

    // ワールド位置
    transformId->UpdateTransform();

    // 動作するかどうか
    if (moveCheck)
        // モーション更新処理
        switch (updateanim)
        {
            // 通常アニメーション
        case UpAnim::Normal:
        {
            // アニメーション再生
            modelId->GetModel()->UpdateAnimation(elapsedTime, true);
            break;
        }
        // 部分再生
        case UpAnim::Doble:
        {
            // モデル部分アニメーション更新処理
            modelId->GetModel()->UpdateUpeerBodyAnimation(elapsedTime, EnemyConfig::kBornUpStartPoint, EnemyConfig::kBornUpEndPoint, true);
            modelId->GetModel()->UpdateLowerBodyAnimation(elapsedTime, EnemyConfig::kBornDownerStartPoint, EnemyConfig::kBornDownerEndPoint, true);
            break;
        }
        // 複数ブレンド再生
        case UpAnim::Blend:
        {
            // モデル複数ブレンドアニメーション更新処理
            modelId->GetModel()->Update_blend_animations(elapsedTime, true);
            break;
        }
        // 逆再生
        case UpAnim::Reverseplayback:
        {
            modelId->GetModel()->ReverseplaybackAnimation(elapsedTime, true);
            break;
        }
        }

    // 姿勢
    modelId->GetModel()->UpdateTransform(transformId->GetTransform());
}

// 後変更Collision
// 足踏み(衝撃波)の当たり判定
void EnemyBoss::CollisionImpactVsPlayer()
{
    auto collisionId = collision.lock();
    // 安全☑
    if (!collisionId) return;
    PlayerManager& playerManager = PlayerManager::Instance();

    ProjectileManager& projectileManager = ProjectileManager::Instance();

    // 全ての敵と総当たりで衝突処理
    int playerCount = playerManager.GetPlayerCount();
    int projectileCount = projectileManager.GetProjectileCount();

    // player所持無し
    if (playerCount <= 0) return;

    auto playerId = playerManager.GetPlayer((int)PlayerManager::PlayerType::Main);
    // 安全☑
    if (!playerId) return;

    // player関係
    auto playerMain = playerId->GetComponent<Player>();
    auto playerTransform = playerId->GetComponent<Transform>();
    auto playerMovement = playerId->GetComponent<Movement>();
    auto playerCollision = playerId->GetComponent<Collision>();
    auto playerHp = playerId->GetComponent<HP>();

    // 安全チェック
    if (!playerMain || !playerTransform || !playerMovement || !playerCollision || !playerHp)
        return;

    // 位置、半径、高さ
    DirectX::XMFLOAT3 playerPosition = playerTransform->GetPosition();
    float playerRadius = PlayerConfig::radius;
    float playerHeight = PlayerConfig::height;

    for (int i = 0; i < projectileCount; ++i)
    {
        auto projectile = projectileManager.GetProjectile(i);
        if (!projectile->GetComponent<ProjectileImpact>() || !projectile) return;

        DirectX::XMFLOAT3 projectilePosition =
            projectile->GetComponent<Transform>()->GetPosition();

        // 身長
        float height = 1.0f;
        float projectileHeight = MagicConfig::kHeight;
        float projectileRadiusOutLine = projectile->GetComponent<ProjectileImpact>()->GetRadiusOutSide();
        float projectileRadiusInLine = projectile->GetComponent<ProjectileImpact>()->GetRadiusInSide();
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
            if (!playerHp->ApplyDamage(EnemyConfig::kApplyDamageStamp, EnemyConfig::kImpactInvincibleTime)) return;

            playerMain->changeState(static_cast<int>(Player::State::Damage));

            // 衝撃波音
            paramSe.filename = AudioConfig::audioImpactPush;
            Audio::Instance().Play(paramSe);

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
    // Lockとして実体を使う
    auto collisionId = collision.lock();
    auto modelId = model.lock();

    // 有効性チェック
    if (!collisionId || !modelId)
        return;

    // 衝撃波の有無
    if (!IsInpact) return;

    ProjectileManager& projectileManager = ProjectileManager::Instance();
    // 左足のボーン名
    Model::Node* bossLeftFoot = modelId->GetModel()->FindNode("boss_left_foot1");
    // ノード位置取得
    // 左足
    DirectX::XMFLOAT3 bossLeftFootPosition;
    bossLeftFootPosition = modelId->GetModel()->ConvertLocalToWorld(bossLeftFoot);

    // 体の一部とplayerの当たり判定
    DetectHitByBodyPart(bossLeftFootPosition, EnemyConfig::kApplyDamageStamp);

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

    auto playerId = playerManager.GetPlayer((int)PlayerManager::PlayerType::Main);
    // 安全☑
    if (!playerId) return;

    // player関係
    auto playerMain = playerId->GetComponent<Player>();
    auto playerTransform = playerId->GetComponent<Transform>();
    auto playerMovement = playerId->GetComponent<Movement>();
    auto playerCollision = playerId->GetComponent<Collision>();
    auto playerHp = playerId->GetComponent<HP>();

    // 安全チェック
    if (!playerMain || !playerTransform || !playerMovement || !playerCollision || !playerHp)
        return;

    // 位置、半径、高さ
    DirectX::XMFLOAT3 playerPosition = playerTransform->GetPosition();
    float playerRadius = PlayerConfig::radius;
    float playerHeight = PlayerConfig::height;

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
        if (!playerHp->ApplyDamage(EnemyConfig::kApplyDamageImpact, EnemyConfig::kImpactInvincibleTime)) return;

        playerMain->changeState(static_cast<int>(Player::State::Damage));

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
    auto collisionId = collision.lock();
    if (!collisionId) return;

    PlayerManager& playerManager = PlayerManager::Instance();

    // 全ての敵と総当たりで衝突処理
    int playerCount = playerManager.GetPlayerCount();

    auto playerId = playerManager.GetPlayer((int)PlayerManager::PlayerType::Main);
    // 安全☑
    if (!playerId) return;

    // player関係
    auto playerMain = playerId->GetComponent<Player>();
    auto playerTransform = playerId->GetComponent<Transform>();
    auto playerMovement = playerId->GetComponent<Movement>();
    auto playerCollision = playerId->GetComponent<Collision>();
    auto playerHp = playerId->GetComponent<HP>();

    // 安全チェック
    if (!playerMain || !playerTransform || !playerMovement || !playerCollision || !playerHp) 
        return;

    // 位置、半径、高さ
    DirectX::XMFLOAT3 playerPosition = playerTransform->GetPosition();
    float playerRadius = PlayerConfig::radius;
    float playerHeight = PlayerConfig::height;
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
        if (!playerHp->ApplyDamage(applyDamage, EnemyConfig::kNuckleInvincibleTime)) return;

        // 斬撃音
        paramSe.filename = AudioConfig::audioDealtDamage;
        Audio::Instance().Play(paramSe);

        DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&playerPosition);
        DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
        DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
        DirectX::XMFLOAT3 normal;
        DirectX::XMStoreFloat3(&normal, N);

        // 吹き飛ばす
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
            playerMain->changeState((int)Player::State::Damage);
            playerMovement->AddImpulse(impulse);
            // エフェクト発生位置
            DirectX::XMFLOAT3 efcPos = playerPosition;
            efcPos.y += PlayerConfig::height;
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

// 部位ごとの当たり判定
void EnemyBoss::DetectHitByBodyAllPart(int applyDamage)
{
    auto collisionId = collision.lock();
    auto modelId = model.lock();
    // 安全☑
    if (!collisionId || !modelId) return;

    PlayerManager& playerManager = PlayerManager::Instance();

    // 全ての敵と総当たりで衝突処理
    int playerCount = playerManager.GetPlayerCount();

    auto playerId = playerManager.GetPlayer((int)PlayerManager::PlayerType::Main);
    // 安全☑
    if (!playerId) return;

    // player関係
    auto playerMain = playerId->GetComponent<Player>();
    auto playerTransform = playerId->GetComponent<Transform>();
    auto playerMovement = playerId->GetComponent<Movement>();
    auto playerCollision = playerId->GetComponent<Collision>();
    auto playerHp = playerId->GetComponent<HP>();

    // 安全チェック
    if (!playerMain || !playerTransform || !playerMovement || !playerCollision || !playerHp) return;

    // 位置、半径、高さ
    DirectX::XMFLOAT3 playerPosition = playerTransform->GetPosition();
    float playerRadius = PlayerConfig::radius;
    float playerHeight = PlayerConfig::height;

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
        if (!playerHp->ApplyDamage(applyDamage, EnemyConfig::kNuckleInvincibleTime)) return;

        // 斬撃音
        paramSe.filename = AudioConfig::audioDealtDamage;
        Audio::Instance().Play(paramSe);

        DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&position);
        DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&playerPosition);
        DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
        DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
        DirectX::XMFLOAT3 normal;
        DirectX::XMStoreFloat3(&normal, N);

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
            playerMain->changeState((int)Player::State::Damage);
            playerMovement->AddImpulse(impulse);
            // エフェクト発生位置
            DirectX::XMFLOAT3 efcPos = playerPosition;
            efcPos.y += PlayerConfig::height;
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
    auto hpId = hp.lock();

    if (!hpId) return;

    if (UiManager::Instance().GetUiesCount() <= uiCountMax)return;

    // hpゲージ処理
    UiManager::Instance().UiHpControlleGauge(
        (int)UiManager::UiCount::EnemyHp,
        (int)UiManager::UiCount::EnemyHPBar,
        CommandConfig::texEnemyNoDamagePos,
        CommandConfig::texEnemyDamagePos,
        hpId,
        elapsedTime);

    bool checkDraw = false;
    switch (hpId->GetLife())
    {
    case 1:
    {
        UiManager::Instance().SelectNotDrawUi((int)UiManager::UiCount::EnemyHPLife02);
        break;
    }
    case 0:
    {
        UiManager::Instance().SelectNotDrawUi((int)UiManager::UiCount::EnemyHPLife01);
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
    auto hpId = hp.lock();

    if (!hpId) return;

    // 点滅時間
    if (hpId->FlashTime(elapsedTime))
    {
        ++damageStateTime;
        // 点滅のオンオフ
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
    // 点滅しない
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
    auto movementId = movement.lock();

    if (!movementId) return;

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
    auto movementId = movement.lock();
    if (!movementId) return;

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
    auto movementId = movement.lock();
    if (!movementId) return;

    // 強制停止
    if (position.y >= EnemyConfig::kLimit)
    {
        movementId->JumpVelocity(EnemyConfig::kMinSpeed);
    }

    // ジャンプ
    if (movementId->GetOnLadius())
    {
        movementId->JumpVelocity(EnemyConfig::kSpeed);
    }
}

// プレイヤーの位置を探す。
bool EnemyBoss::SearchPlayer()
{
    // プレイヤー取得
    auto playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount()-1);
    //　トランスフォーム分解
    DirectX::XMFLOAT3 playerPosition = playerid->GetComponent<Transform>()->GetPosition();
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

// 混乱エフェクト再生
void EnemyBoss::StartConfusion()
{
    // Lockとして実体を使う
    auto modelId = model.lock();

    // 有効性チェック
    if (!modelId)
        return;

    Model::Node* characterBorn = modelId->GetModel()->FindNode("boss_right_hand4");
    // エネミー腰位置
    DirectX::XMFLOAT3 enemyHeadPosition;

    enemyHeadPosition = modelId->GetModel()->ConvertLocalToWorld(characterBorn);

    confusionEffect->Play(enemyHeadPosition);
}

// 削除更新
void EnemyManager::DeleteUpdate(float elapsedTime)
{
    // 破棄処理 毎フレームここで一気に消す。
    for (auto enemy : removes)// リストを消す
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
