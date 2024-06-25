#include <imgui.h>
#include "EnemySlime.h"
//#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
//#include "Collision.h"

//// コンストラクタ
EnemySlime::EnemySlime()
{
//    model = new Model("Data/Model/Slime/Slime.mdl");
//
//    // モデルがおおきいのでスケーリング
//    scale.x = scale.y = scale.z = 0.01f;
//
//    // 幅、高さ設定(円柱)
//    radius = 0.5f;
//    height = 1.0f;
//    
//        // 徘徊ステートへ遷移
//    TransitionWanderState();
}

// デストラクタ
EnemySlime::~EnemySlime()
{
   // delete model;
}

void EnemySlime::Start()
{
    // ムーブメント関数を使えるように
    movement = GetActor()->GetComponent<Movement>();

    // hp関数を使えるように
    hp = GetActor()->GetComponent<HP>();

    // モデルデータを入れる。
    model = GetActor()->GetModel();

    hp->SetHealth(health);

    hp->SetMaxHealth(maxHealth);

    GetActor()->SetRadius(radius);

    GetActor()->SetHeight(height);

    // 徘徊ステートへ遷移
    TransitionWanderState();

    SetTerritory(position, territoryarea);
}

// 更新処理
void EnemySlime::Update(float elapsedTime)
{
    // ステート毎の処理
    switch (state)
    {
    case State::Wander:
        UpdateWanderState(elapsedTime);
        break;
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Pursuit:
        UpdatePursuitState(elapsedTime);
        break;
    case State::Attack:
        UpdateAttackState(elapsedTime);
        break;
    case State::IdleBattle:
        UpdateIdleBattleState(elapsedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;
    }

    // 位置
    position = GetActor()->GetPosition();

    // 速力処理更新
    movement->UpdateVelocity(elapsedTime);
    // 無敵時間更新
    hp->UpdateInbincibleTimer(elapsedTime);

    GetActor()->UpdateTransform();
    //GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());
    
   // UpdateVelocity(elapsedTime);

    //UpdateInbincibleTimer(elapsedTime);
    // オブジェクト行列を更新
    //UpdateTransform();

    //// モデルアニメーション更新
    //model->UpdateAnimation(elapsedTime);

    //// モデル行列更新
    //model->UpdateTransform(transform);
}

// 描画処理
//void EnemySlime::Render(const RenderContext& rc, ModelShader* shader)
//{
//    shader->Draw(rc, model);
//}

// デバッグプリミティブ描画
void EnemySlime::DrawDebugPrimitive()
{
    // 基底クラスのデバッグプリミティブ描画
    //Enemy::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // 縄張り範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f,
        DirectX::XMFLOAT4(0, 1, 0, 1));

    // ターゲット位置をデバッグ球描画
    debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1, 1, 0, 1));
    
    // 索敵範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0, 0, 1, 1));

    // 攻撃範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(position, attackRange, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
}

// 縄張り設定
void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;

}

void EnemySlime::SetRandomTargetPosition()
{

    float theta = Mathf::RandomRange(-DirectX::XM_PI, DirectX::XM_PI);
    float range = Mathf::RandomRange(0.0f, territoryRange);
    targetPosition.x = territoryOrigin.x + sinf(theta)*range;
    targetPosition.y = territoryOrigin.y;
    targetPosition.z = territoryOrigin.z + cosf(theta) * range;
}

void EnemySlime::MoveToTarget(float elapsedTime, float speedRate)
{
    // ターゲット方向への進行ベクトルを算出
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;

    // 移動処理
    movement->Move({ vx,0.0f ,vz }, moveSpeed * speedRate, elapsedTime);
    movement->Turn({ vx,0.0f ,vz } ,turnSpeed,elapsedTime);
   // Move( vx,vz  ,moveSpeed * speedRate);
    //Turn(elapsedTime, vx, vz ,turnSpeed * speedRate);
    //movement->Turn({ vx,0.0f ,vz } ,turnSpeed * speedRate,elapsedTime);
}

// 徘徊ステートへ遷移
void EnemySlime::TransitionWanderState()
{
    state = State::Wander;

    // 目標地点設定
    SetRandomTargetPosition();

    // 歩きアニメーション再生
    model->PlayAnimation(Anim_WalkFWD, true);
}

// 徘徊ステート更新処理
void EnemySlime::UpdateWanderState(float elapsedTime)
{
    // 目標地点までXZ平面での距離判定
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z- position.z;
    float distSq = vx * vx + vz * vz;
    if (distSq < radius * radius)
    {

        //// 待機ステートへ
        TransitionIdleState();
    }

    // 目標地点へ移動
    MoveToTarget(elapsedTime, 0.5f);

    // プレイヤー索敵
    if (SearchPlayer())
    {
        // 見つかったら
        TransitionPursuitState();
    }
}

void EnemySlime::CollisitionNodeVsPlayer(const char* nodeName, float nodeRadius)
{
    // ノード位置と当たり判定を行う
    Model::Node* node = model->FindNode(nodeName);
    if (node != nullptr)
    {
        // ノードのワールド座標
        DirectX::XMFLOAT3 nodePosition(
        node->worldTransform._41,
        node->worldTransform._42,
        node->worldTransform._43
            );

        // 当たり判定表示
        Graphics::Instance().GetDebugRenderer()->DrawSphere(
            nodePosition, nodeRadius, DirectX::XMFLOAT4(0, 0, 1, 1)
        );

        // プレイヤーと当たり判定
        //Player& player = Player::Instance();

        int playercount = PlayerManager::Instance().GetPlayerCount();
        
        for (int i = 0; i < playercount; ++i)
        {
            // プレイヤー取得
            Actor* playerid = PlayerManager::Instance().GetPlayer(i);

            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                nodePosition,
                nodeRadius,
                playerid->GetPosition(),
                playerid->GetRadius(),
                playerid->GetHeight(),
                outPosition))
            {
                // ダメージを与える
                if (playerid->GetComponent<HP>()->ApplyDamage(1, 0.5f))
                {

                    // 衝動
                    DirectX::XMFLOAT3 impulse;
                    const float power = 10.0f;
                    const DirectX::XMFLOAT3& p = playerid->GetPosition();
                    const DirectX::XMFLOAT3& e = nodePosition;
                    float vx = p.x - e.x;
                    float vz = p.z - e.z;
                    float lengthXZ = sqrtf(vx * vx + vz * vz);
                    vx /= lengthXZ;
                    vz /= lengthXZ;

                    impulse.x = vx * power;
                    impulse.y = power * 0.5f;
                    impulse.z = vz * power;



                    // 吹っ飛ばす
                    playerid->GetComponent<Movement>()->AddImpulse(impulse);
                }
            }
        }
    }
}

bool EnemySlime::SearchPlayer()
{
    // プレイヤー取得
    Actor* playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount()-1);
    // プレイヤーと高低差を考慮して３Dでの距離判定をする
    const DirectX::XMFLOAT3& playerPosition = playerid->GetPosition();
    //const DirectX::XMFLOAT3& playerPosition = DirectX::XMFLOAT3{0, 0, 0};
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

void EnemySlime::TransitionIdleState()
{
    state = State::Idle;

    // タイマーをランダムを設定　待機時間
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    // 待機アニメーション再生
    model->PlayAnimation(Anim_IdleNormal, true);
}

void EnemySlime::UpdateIdleState(float elapsedTime)
{

    // タイマー処理 待機時間
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        // 徘徊ステートへ遷移
        TransitionWanderState();
    }
}

void EnemySlime::TransitionPursuitState()
{
    state = State::Pursuit;

    // 数秒間追尾するタイマーをランダム設定
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    // 歩きアニメーション再生
    model->PlayAnimation(Anim_RunFWD, true);
}

// 追尾ステート更新処理
void EnemySlime::UpdatePursuitState(float elapsedTime)
{
    // プレイヤーid
    Actor* playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
    // 目標地点ををプレイヤー位置に設定
    targetPosition = playerid->GetPosition();

    // 目標地点へ移動
    MoveToTarget(elapsedTime, 1.0f);

    // タイマー処理
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        // 待機ステートへ遷移
        TransitionIdleState();
    }

    // プレイヤーの近づくと攻撃ステートへ遷移
    float vx = targetPosition.x - position.x;
    float vy = targetPosition.y - position.y;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);
    if (dist < attackRange)
    {
        // 攻撃ステートへ遷移
        TransitionAttackState();
    }



}

void EnemySlime::TransitionAttackState()
{
    state = State::Attack;

    // 攻撃アニメーション再生
    model->PlayAnimation(Anim_Attack1, false);
}

void EnemySlime::UpdateAttackState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
    // 任意のアニメーション再生区間でのみ衝突判定処理をする
    float animationTime = model->GetCurrentANimationSeconds();
    if (animationTime >= 0.2f && animationTime <= 0.35f)
    {
        // 目玉ノードとプレイヤーの衝突処理
        CollisitionNodeVsPlayer("EyeBall", 0.2f);
    }
}

void EnemySlime::TransitionIdleBattleState()
{
    state = State::IdleBattle;

    // 攻撃アニメーション再生
    model->PlayAnimation(Anim_IdleBattle, true);
}

void EnemySlime::UpdateIdleBattleState(float elapsedTime)
{
    // プレイヤーid
    Actor* playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
    // 目標地点ををプレイヤー位置に設定
    targetPosition = playerid->GetPosition();

    // タイマー処理
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        // プレイヤーが攻撃範囲にいた場合は攻撃ステートへ遷移
        float vx = targetPosition.x - position.x;
        float vy = targetPosition.y - position.y;
        float vz = targetPosition.z - position.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);
        if (dist < attackRange)
        {
            // 攻撃ステートへ遷移
            TransitionAttackState();
        }
        else
        {
            // 徘徊ステートへ遷移
            TransitionWanderState();
        }
    }
    MoveToTarget(elapsedTime, 0.0f);
}
// ダメージステートへ遷移
void EnemySlime::TransitionDamageState()
{
    state = State::Damage;

    // ダメージアニメーション再生
    model->PlayAnimation(Anim_GetHit, false);
}

void EnemySlime::UpdateDamageState(float elapsedTime)
{
    // ダメージアニメーションが終わったら戦闘待機ステートへ遷移
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

// 死亡ステートへ遷移
void EnemySlime::TransitionDeathState()
{
    state = State::Death;

    // ダメージアニメーション再生
    model->PlayAnimation(Anim_Die, false);
}

void EnemySlime::UpdateDeathState(float elapsedTime)
{
    // ダメージアニメーションが終わったら自分を破棄
    if (!model->IsPlayAnimation())
    {
        hp->OnDead();
        
          
    }
}

void EnemySlime::Destroy()
{
    Actor* enemyId = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1);
    //ActorManager::Instance().Remove();
}

//// 死亡した時に呼ばれる
//void EnemySlime::OnDead()
//{
//    // 死亡ステートへ遷移
//    TransitionDeathState();
//}
//
//void EnemySlime::OnDamaged()
//{
//    // ダメージステートへ遷移
//    TransitionDamageState();
//}



void EnemyManager::Register(Actor* actor)
{
    enemies.emplace_back(actor);
}
