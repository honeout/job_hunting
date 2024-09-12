#include <imgui.h>
#include "EnemySlime.h"
//#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
//#include "Collision.h"
#include "StateDerived.h"

#include "TransForm2D.h"

#include "ProjectileImpact.h"
#include "ProjectileRuby.h"

#include "UiManager.h"
#include "Ui.h"


// デストラクタ
EnemySlime::~EnemySlime()
{
   // delete model;
    if (stateMachine != nullptr)
    {
        delete stateMachine;
        stateMachine = nullptr;
    }
}

void EnemySlime::Start()
{
    // ムーブメント関数を使えるように
    movement = GetActor()->GetComponent<Movement>();

    // hp関数を使えるように
    hp = GetActor()->GetComponent<HP>();

    // transform関数を使えるように
    transform = GetActor()->GetComponent<Transform>();

    // モデルデータを入れる。
    model = GetActor()->GetComponent<ModelControll>()->GetModel();
    

    hp->SetHealth(health);

    hp->SetMaxHealth(maxHealth);

    transform->SetRadius(radius);

    transform->SetHeight(height);

    // 徘徊ステートへ遷移
    //TransitionWanderState();

    //SetTerritory(position, territoryarea);

    //stateMachine = std::make_unique<StateMachine>();
    stateMachine = new StateMachine();

    // ステートマシンにステート登録
    stateMachine->RegisterState(new WanderState(GetActor().get()));
    stateMachine->RegisterState(new IdleState(GetActor().get()));
    stateMachine->RegisterState(new PursuitState(GetActor().get()));
    stateMachine->RegisterState(new AttackState(GetActor().get()));
    stateMachine->RegisterState(new AttackShotState(GetActor().get()));
    stateMachine->RegisterState(new DamageState(GetActor().get()));

    // ステートセット
    stateMachine->SetState(static_cast<int>(State::Attack));

    // 当たり判定無効判定
    invalidJudgment = true;

    // playerカウンター用
    counterJudgment = false;
}

// 更新処理
void EnemySlime::Update(float elapsedTime)
{
    // ステート毎の処理
   /* switch (state)
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
    }*/

    stateMachine->Update(elapsedTime);

    // 位置
    position = transform->GetPosition();
    // 向き
    angle = transform->GetAngle();
    // 大きさ
    scale = transform->GetScale();

    // 速力処理更新
    movement->UpdateVelocity(elapsedTime);
    // 無敵時間更新
    hp->UpdateInbincibleTimer(elapsedTime);

    DrawDebugPrimitive();

    // 当たり判定衝撃波とプレイヤー
    CollisionImpactVsPlayer();

    // 当たり判定ルビーとプレイヤー
    CollisionRubyVsPlayer();

    // 削除
    ProjectileManager::Instance().DeleteUpdate(elapsedTime);

    transform->UpdateTransform();

    model->UpdateAnimation(elapsedTime,true);

    //model->UpdateAnimation(elapsedTime, true);

    model->UpdateTransform(transform->GetTransform());
    //GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());
    
   // UpdateVelocity(elapsedTime);

    //UpdateInbincibleTimer(elapsedTime);
    // オブジェクト行列を更新
    //UpdateTransform();

    //// モデルアニメーション更新
    //model->UpdateAnimation(elapsedTime);

    //// モデル行列更新
    //model->UpdateTransform(transform);

    // ゲージ管理
    UiControlle(elapsedTime);
}

// 描画処理
//void EnemySlime::Render(const RenderContext& rc, ModelShader* shader)
//{
//    shader->Draw(rc, model);
//}

void EnemySlime::Render(RenderContext& rc, ModelShader& shader)
{
    Graphics& graphics = Graphics::Instance();
    //Shader* shader = graphics.GetShader();
    //ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
    shader.Begin(rc);// シェーダーにカメラの情報を渡す


    shader.Draw(rc, model);

    shader.End(rc);


}
// シャドウマップ
void EnemySlime::RenderShadowmap(RenderContext& rc)
{
    Graphics& graphics = Graphics::Instance();
    ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);
    shader->Begin(rc);// シェーダーにカメラの情報を渡す


    shader->Draw(rc, model);

    shader->End(rc);
}

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

// 足踏み(衝撃波)の当たり判定
void EnemySlime::CollisionImpactVsPlayer()
{
    PlayerManager& playerManager = PlayerManager::Instance();

    ProjectileManager& projectileManager = ProjectileManager::Instance();

    // 全ての敵と総当たりで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();

    for (int i = 0; i < projectileCount; ++i)
    {
        int playerCount = PlayerManager::Instance().GetPlayerCount();
        std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        for (int j = 0; j < playerCount; ++j)
        {
            if (projectile->GetComponent<ProjectileImpact>())
            {
                std::shared_ptr<Actor> player = playerManager.GetPlayer(j);

                DirectX::XMFLOAT3 projectilePosition = projectile->GetComponent<Transform>()->GetPosition();
                
                // 身長
                float height = 1.0f;
                projectile->GetComponent<Transform>()->SetHeight(height);
                float projectileHeight = projectile->GetComponent<Transform>()->GetHeight();

                float projectileRadiusOutLine = projectile->GetComponent<ProjectileImpact>()->GetRadiusOutSide();
                float projectileRadiusInLine = projectile->GetComponent<ProjectileImpact>()->GetRadiusInSide();

                DirectX::XMFLOAT3 playerPosition = player->GetComponent<Transform>()->GetPosition();
                float playerRadius = player->GetComponent<Transform>()->GetRadius();
                float playerHeight = player->GetComponent<Transform>()->GetHeight();

                // 衝突処理
                DirectX::XMFLOAT3 outPositon;
                // 円柱と円
                if (Collision::IntersectSphereVsCylinder(
                    projectilePosition,
                    projectileRadiusOutLine,
                    playerPosition,
                    playerRadius,
                    playerHeight,
                    outPositon) &&
                    !Collision::IntersectSphereVsCylinder(
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
                    if (player->GetComponent<HP>()->ApplyDamage(3, 0.5f))
                    {
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

                            player->GetComponent<Movement>()->AddImpulse(impulse);
                        }
                        // ヒットエフェクト再生
                        {
                            playerPosition.y += playerHeight * 0.5f;


                            //hitEffect->Play(e);
                        }

                        // 弾丸破棄
                        //projectile->GetComponent<ProjectileImpact>()->Destoroy();
                        
                    }

                }
            }
        }
    }
}

void EnemySlime::CollisionRubyVsPlayer()
{
    PlayerManager& playerManager = PlayerManager::Instance();

    ProjectileManager& projectileManager = ProjectileManager::Instance();

    // 全ての敵と総当たりで衝突処理
    int projectileCount = projectileManager.GetProjectileCount();

    for (int i = 0; i < projectileCount; ++i)
    {
        int playerCount = PlayerManager::Instance().GetPlayerCount();
        std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        for (int j = 0; j < playerCount; ++j)
        {
            if (projectile->GetComponent<ProjectileRuby>())
            {
                std::shared_ptr<Actor> player = playerManager.GetPlayer(j);

                DirectX::XMFLOAT3 projectilePosition = projectile->GetComponent<Transform>()->GetPosition();
                float projectileRadius = projectile->GetComponent<Transform>()->GetRadius();
                float projectileHeight = projectile->GetComponent<Transform>()->GetHeight();

                DirectX::XMFLOAT3 playerPosition = player->GetComponent<Transform>()->GetPosition();
                float playerRadius = player->GetComponent<Transform>()->GetRadius();
                float playerHeight = player->GetComponent<Transform>()->GetHeight();

                // 衝突処理
                DirectX::XMFLOAT3 outPositon;
                // 円柱と円柱
                if (Collision::IntersectCylinderVsCylinder(
                    projectilePosition,
                    projectileRadius,
                    projectileHeight,
                    playerPosition,
                    playerRadius,
                    playerHeight,
                    outPositon))

                {
                    // ダメージを与える。
                    if (player->GetComponent<HP>()->ApplyDamage(3, 0.5f))
                    {

                        DirectX::XMVECTOR ProjectileP = DirectX::XMLoadFloat3(&projectilePosition);
                        DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&playerPosition);
                        DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P , ProjectileP);
                        DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
                        DirectX::XMFLOAT3 normal;
                        DirectX::XMStoreFloat3(&normal, N);

                        if (normal.y > 0.8f)
                        {
                            float jumpSpeed = 20.0f;
                            // 衝撃
                            const float power = 10.0f;
                            DirectX::XMFLOAT3 impulse;
                            impulse.y = power * 0.5f;
                            player->GetComponent<Movement>()->JumpVelocity(jumpSpeed);

                        }
                        //// 吹き飛ばす
                        else
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

                            player->GetComponent<Movement>()->AddImpulse(impulse);
                            // ヒットエフェクト再生

                            playerPosition.y += playerHeight * 0.5f;

                            //hitEffect->Play(e);

                        }
                      



                        // 弾丸破棄
                        //projectile->GetComponent<ProjectileImpact>()->Destoroy();

                    }
                    else
                    {
                        DirectX::XMVECTOR ProjectileP = DirectX::XMLoadFloat3(&projectilePosition);
                        DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&playerPosition);
                        DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, ProjectileP);
                        DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
                        DirectX::XMFLOAT3 normal;
                        DirectX::XMStoreFloat3(&normal, N);

                        if (normal.y > 0.8f)
                        {
                            float jumpSpeed = 20.0f;
                            // 衝撃
                            const float power = 10.0f;
                            DirectX::XMFLOAT3 impulse;
                            impulse.y = power * 0.5f;
                            player->GetComponent<Movement>()->JumpVelocity(jumpSpeed);

                        }
                        //// 吹き飛ばす
                        else
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

                            player->GetComponent<Movement>()->AddImpulse(impulse);
                            // ヒットエフェクト再生

                            playerPosition.y += playerHeight * 0.5f;

                            //hitEffect->Play(e);

                        }
                    }

                }
            }
        }
    }
}

void EnemySlime::InputImpact(DirectX::XMFLOAT3 pos)
{
    {

        //// 前方向 sinの計算
        //DirectX::XMFLOAT3 dir;


        //dir.x = sinf(angle.y);// 三角を斜めにして位置を変えた
        //dir.y = 0;
        //dir.z = cosf(angle.y);

        ////sinf0度０　cosf0は１度
        ////９０sin1,cos0返ってくる横
        ////４５sin0.5,cos0.5斜め
        //// 360度を上手く表現出来る。2dでも行ける。


        //// 発射位置（プレイヤーの腰当たり)
        //DirectX::XMFLOAT3 pos;
        //pos.x = position.x;
        //pos.y = position.y + height * 0.5f;// 身長÷位置のｙ
        //pos.z = position.z;
        ////ターゲット（デフォルトではプレイヤーの前方）
        //DirectX::XMFLOAT3 target;
        //// 敵がいなかった時のために　1000先まで飛んでくれ
        //target.x = pos.x + dir.x * 1000.0f;
        //target.y = pos.y + dir.y * 1000.0f;
        //target.z = pos.z + dir.z * 1000.0f;

        //// 一番近くの敵をターゲットにする
        //float dist = FLT_MAX;// float の最大値float全体
        //PlayerManager& playerManager = PlayerManager::Instance();
        //int playerCount = playerManager.GetPlayerCount();
        //for (int i = 0; i < playerCount; ++i)//float 最大値ないにいる敵に向かう
        //{
        //    // 敵との距離判定  敵の数も計測 全ての敵をてに入れる
        //    std::shared_ptr<Actor> player = PlayerManager::Instance().GetPlayer(i);
        //    DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
        //    // 敵の位置
        //    DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&player->GetComponent<Transform>()->GetPosition());
        //    // 自分から敵までの位置を計測
        //    DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
        //    // ベクトルのながさを２乗する。√つけていない奴
        //    DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
        //    float d;
        //    DirectX::XMStoreFloat(&d, D);
        //    if (d < dist)
        //    {
        //        // 距離が敵のものを入れる少なくする３０なら３０、１００なら１００入れる
        //        dist = d;
        //        target = player->GetComponent<Transform>()->GetPosition();// 位置を入れる
        //        target.y += player->GetComponent<Transform>()->GetHeight() * 0.5f;// 位置に身長分
        //    }



        //}

        // 弾丸初期化
        const char* filename = "Data/Model/Sword/Sword.mdl";

        std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
        actor->AddComponent<ModelControll>();
        actor->GetComponent<ModelControll>()->LoadModel(filename);
        actor->SetName("ProjectileImpact");
        actor->AddComponent<Transform>();
        actor->GetComponent<Transform>()->SetPosition(pos);
        actor->GetComponent<Transform>()->SetAngle(angle);
        actor->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f));



        //actor->AddComponent<BulletFiring>();
        actor->AddComponent<ProjectileImpact>();
        const char* effectFilename = "Data/Effect/inpact.efk";
        actor->GetComponent<ProjectileImpact>()->SetEffectProgress(effectFilename);
        // 生存時間
        float lifeTimer = 50.0f;
        actor->GetComponent<ProjectileImpact>()->SetLifeTimer(lifeTimer);

        
        //actor->GetComponent<ProjectileImpact>()->EffectProgressPlay();
        // これが２Dかの確認
        bool check2d = false;
        actor->SetCheck2d(check2d);

        //actor->AddComponent<Collision>();
        ProjectileManager::Instance().Register(actor);
        //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        //std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

        // 発射
        //projectile->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        //projectile->GetComponent<ProjectileImpact>()->SetTarget(target);

        // 攻撃解除
        //magicAction = false;
        //selectMagicCheck = (int)CommandMagic::Normal;

       
    }

}

void EnemySlime::InputProjectile()
{
    // 前方向 sinの計算
    DirectX::XMFLOAT3 dir;


    dir.x = 0;// 三角を斜めにして位置を変えた
    dir.y = cosf(angle.x);
    dir.z = 0;

    DirectX::XMFLOAT3 pos;
    PlayerManager& playerManager = PlayerManager::Instance();
int playerCount = playerManager.GetPlayerCount();
for (int i = 0; i < playerCount; ++i)//float 最大値ないにいる敵に向かう
{
    std::shared_ptr<Actor> player = PlayerManager::Instance().GetPlayer(i);
    // 位置

    pos = player->GetComponent<Transform>()->GetPosition();
    pos.y -= 15;

}

    // 弾丸初期化
    const char* filename = "Data/Model/Bullet/Bullet.mdl";
    //const char* filename = "Data/Model/Sword/Sword.mdl";

    std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
    actor->AddComponent<ModelControll>();
    actor->GetComponent<ModelControll>()->LoadModel(filename);
    actor->SetName("ProjectileRuby");
    actor->AddComponent<Transform>();
    actor->GetComponent<Transform>()->SetPosition(pos);
    actor->GetComponent<Transform>()->SetAngle(angle);
    actor->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
    
    float height = 5;
    actor->GetComponent<Transform>()->SetHeight(height);
    float lifetimer = 3;
    
    actor->AddComponent<BulletFiring>();
    actor->GetComponent<BulletFiring>()->Lanch(dir,pos, lifetimer);


    actor->AddComponent<ProjectileRuby>();
    const char* effectFilename = "Data/Effect/smorker.efk";
    actor->GetComponent<ProjectileRuby>()->SetEffectProgress(effectFilename);
    //actor->GetComponent<ProjectileHoming>()->EffectProgressPlay();
    // これが２Dかの確認
    bool check2d = false;
    actor->SetCheck2d(check2d);
    // 移動処理の有無
    bool movementCheck = true;
    actor->GetComponent<ProjectileRuby>()->SetMovementCheck(movementCheck);
    // 生存時間
    //float lifeTimer = 50.0f;
    //actor->GetComponent<ProjectileHoming>()->SetLifeTimer(lifeTimer);

    //actor->AddComponent<Collision>();
    ProjectileManager::Instance().Register(actor);
    //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
    //std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

    // 発射
    //projectile->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
    //projectile->GetComponent<ProjectileImpact>()->SetTarget(target);

    // 攻撃解除
    //magicAction = false;
    //selectMagicCheck = (int)CommandMagic::Normal;



}

// 縄張り設定
void EnemySlime::SetTerritory(const DirectX::XMFLOAT3& origin, float range)
{
    territoryOrigin = origin;
    territoryRange = range;

}

void EnemySlime::UiControlle(float elapsedTime)
{
    float gaugeWidth = hp->GetMaxHealth() * hp->GetHealth() * 0.12f;
    std::shared_ptr<TransForm2D> uiHp = UiManager::Instance().GetUies((int)UiManager::UiCount::EnemyHPBar)->GetComponent<TransForm2D>();
    DirectX::XMFLOAT2 scale = { gaugeWidth, uiHp->GetScale().y };

    uiHp->SetScale(scale);

    std::shared_ptr<Ui> uiHpLife1 = UiManager::Instance().GetUies((int)UiManager::UiCount::EnemyHPLife01)->GetComponent<Ui>();
    std::shared_ptr<Ui> uiHpLife2 = UiManager::Instance().GetUies((int)UiManager::UiCount::EnemyHPLife02)->GetComponent<Ui>();

    bool checkDraw = false;
    switch (hp->GetLife())
    {
    case 1:
    {
        uiHpLife2->SetDrawCheck(checkDraw);
        break;
    }
    case 0:
    {
        uiHpLife1->SetDrawCheck(checkDraw);
        break;
    }
    default:
        break;
    }
    
    

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
    model->PlayAnimation(EnemySlime::Anim_Walk, true);
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


    if (node != nullptr && invalidJudgment)
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
            std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(i);

            //　トランスフォーム分解
            DirectX::XMFLOAT3 playerPosition = playerid->GetComponent<Transform>()->GetPosition();
            float playerRadius = playerid->GetComponent<Transform>()->GetRadius();
            float playerHeight = playerid->GetComponent<Transform>()->GetRadius();


            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectSphereVsCylinder(
                nodePosition,
                nodeRadius,
                playerPosition,
                playerRadius,
                playerHeight,
                outPosition))
            {
                // ダメージを与える
                if (playerid->GetComponent<HP>()->ApplyDamage(1, 0.5f))
                {

                    // 衝動
                    DirectX::XMFLOAT3 impulse;
                    const float power = 10.0f;
                    const DirectX::XMFLOAT3& p = playerPosition;
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

                    // Ui 揺らし開始
                    playerid->GetComponent<Player>()->SetShakeMode(true);
                }
            }
        }
    }
}

bool EnemySlime::SearchPlayer()
{
    // プレイヤー取得
    std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount()-1);
    
    //　トランスフォーム分解
    DirectX::XMFLOAT3 playerPosition = playerid->GetComponent<Transform>()->GetPosition();
    float playerRadius = playerid->GetComponent<Transform>()->GetRadius();
    float playerHeight = playerid->GetComponent<Transform>()->GetRadius();


    
    // プレイヤーと高低差を考慮して３Dでの距離判定をする
    //const DirectX::XMFLOAT3& playerPosition = playerid->GetPosition();
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
   // model->PlayAnimation(Anim_IdleNormal, true);
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
    // model->PlayAnimation(Anim_RunFWD, true);
}

// 追尾ステート更新処理
void EnemySlime::UpdatePursuitState(float elapsedTime)
{
    // プレイヤーid
    std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
    // 目標地点ををプレイヤー位置に設定
    targetPosition = playerid->GetComponent<Transform>()->GetPosition();

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
    //model->PlayAnimation(Anim_Attack1, false);
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
    //model->PlayAnimation(Anim_IdleBattle, true);
}

void EnemySlime::UpdateIdleBattleState(float elapsedTime)
{
    // プレイヤーid
    std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
    // 目標地点ををプレイヤー位置に設定
    targetPosition = playerid->GetComponent<Transform>()->GetPosition();

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
    //model->PlayAnimation(Anim_GetHit, false);
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
        EnemyManager::Instance().Remove(GetActor());
        EnemyManager::Instance().DeleteUpdate(elapsedTime);
    }
}

void EnemySlime::Destroy()
{
    //Actor* enemyId = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1);
    EnemyManager::Instance().Remove(GetActor());
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



void EnemyManager::DeleteUpdate(float elapsedTime)
{
    // 破棄処理 毎フレームここで一気に消す。
    for (std::shared_ptr<Actor> enemy : removes)// 殺しますよリストを殺す
    {
        std::vector<std::shared_ptr<Actor>>::iterator it = std::find(enemies.begin(), enemies.end(),
            enemy);
        if (it != enemies.end())
        {
            enemies.erase(it);// 削除
        }

        // 弾丸の破棄処理
        //delete projectile;// 入れ物破棄

    }
    // 破棄リストをクリア
    removes.clear();
}

void EnemyManager::Register(std::shared_ptr<Actor> actor)
{
    enemies.emplace_back(actor);
}

void EnemyManager::Clear()
{
    for (std::shared_ptr<Actor>& actor : enemies)// 
    {
        // 実体を消した管理している数はそのまま
       // delete projectile;
       // Remove(actor);
        //actor.reset();
        actor.reset();
    }
    enemies.clear();
}

void EnemyManager::Remove(std::shared_ptr<Actor> actor)
{
    // 削除登録
    removes.insert(actor);
}
