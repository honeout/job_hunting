#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
//#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "Graphics/Model.h"
#include "EnemySlime.h"


#include "AfterimageManager.h"


static Player* instance = nullptr;

// インスタンス取得
Player& Player::Instance()
{
    return *instance;
}



// コンストラクタ
Player::Player()
{
    // インスタンスポインタ設定
    //instance = this;

    //model = new Model("Data/Model/Jammo/Jammo.mdl");
    //model = new Model("Data/Model/AimTest/AimTest.mdl");

    // モデルがおおきいのでスケーリング
    // キャラクターも1.1倍
    //scale.x = scale.y = scale.z = 0.01f;

    //// ヒットエフェクト読込 
    //hitEffect = new Effect("Data/Effect/sunder.efk");
    //desEffect = new Effect("Data/Effect/F.efk");

    //// 上下別アニメーション用
    //state = State::Idle;

    //// 上半身
    //bornUpStartPoint = "mixamorig:Spine";
    //// 下半身
    //bornDownerEndPoint = "mixamorig:Spine";

    //// 待機ステートへ遷移
    //TransitionIdleState();

}

// デストラクタ
Player::~Player()
{
    //if (hitEffect == nullptr)
    //delete hitEffect;
    //if (desEffect == nullptr)
    //delete desEffect;
    //if (model == nullptr)
    //delete model;
    if (hitEffect != nullptr)
    {
        delete hitEffect;
        hitEffect = nullptr;
    }

    if (desEffect != nullptr)
    {
        delete desEffect;
        desEffect = nullptr;
    }

    if (cameraControlle != nullptr)
    {
        delete cameraControlle;
        cameraControlle = nullptr;
    }

    //if (model != nullptr)
    //{
    //    delete model;
    //    //model = nullptr;
    //}
}




//void Player::UpdateTransform()
//{
//
//
//
//    
//}

void Player::Start()
{
    // インスタンス化
    instance = this;

    // ムーブメント関数を使えるように
    movement = GetActor()->GetComponent<Movement>();

    // hp関数を使えるように
    hp = GetActor()->GetComponent<HP>();

    // projectileStraight関数を使えるように
    //projectileStraight = GetActor()->GetComponent<>;
    
    // モデルデータを入れる。
    model = GetActor()->GetComponent<ModelControll>()->GetModel();
    
    cameraControlle = new CameraController();

    // ヒットエフェクト読込 
    hitEffect = new Effect("Data/Effect/sunder.efk");
    desEffect = new Effect("Data/Effect/F.efk");


    // 上半身
    bornUpStartPoint = "mixamorig:Spine";
    // 下半身
    bornDownerEndPoint = "mixamorig:Spine";

   

    hp->SetHealth(health);

    hp->SetMaxHealth(maxHealth);

    GetActor()->SetRadius(radius);
    GetActor()->SetHeight(height);
    // 待機ステートへ遷移
    TransitionIdleState();

}

// 更新処理
// elapsedTime(経過時間)
void Player::Update(float elapsedTime)
{
    // ステート毎の処理
    switch (state)
    {
    case State::Idle:
        UpdateIdleState(elapsedTime);
        break;
    case State::Move:
        UpdateMoveState(elapsedTime);
        break;

    case State::Jump:
        UpdateJumpState(elapsedTime);
        break;
    case State::Land:
        UpdateLandState(elapsedTime);
        break;

    case State::JumpFlip:
        UpdatejumpFlipState(elapsedTime);
        break;

    case State::Attack:
        UpdateAttackState(elapsedTime);
        break;
    case State::Damage:
        UpdateDamageState(elapsedTime);
        break;
    case State::Death:
        UpdateDeathState(elapsedTime);
        break;
    case State::Revive:
        UpdateReviveState(elapsedTime);
        break;
    }
    // 速力処理更新

    position = GetActor()->GetPosition();

    angle = { 
        GetActor()->GetRotation().x,
        GetActor()->GetRotation().y,
        GetActor()->GetRotation().z
    };

    //velocity = movement->GetVelocity();

    //movement->SetVelocity(velocity);

    //velocity = movement->GetVelocity();

    hp->UpdateInbincibleTimer(elapsedTime);

    cameraControlle->Update(elapsedTime);
    cameraControlle->SetTarget(position);

    hitEffect->SetScale(hitEffect->GetEfeHandle(),{ 1,1,1 });

    movement->UpdateVelocity(elapsedTime);



    //UpdateTransform();
    //CameraControl(elapsedTime);
   //CharacterControl(elapsedTime);

    // プレイヤーと敵との衝突処理
    CollisionPlayerVsEnemies();

    CollisionProjectilesVsEnemies();

    UpdateProjectile(elapsedTime);

    // 弾丸更新処理
    //projectileManager.Update(elapsedTime);

    // オブジェクト行列を更新
    //UpdateTransform();

    // モデルアニメーション更新処理
    //model->UpdateAnimation(elapsedTime, true);

    //switch (updateanim)
    //{
    //case UpAnim::Normal:
    //{
    //    // モデルアニメーション更新処理
    //    //model->UpdateAnimation(elapsedTime, true);
    //    break;
    //}
    //case UpAnim::Doble:
    //{
    //    // モデルアニメーション更新処理
    //    //model->UpdateUpeerBodyAnimation(elapsedTime, bornUpStartPoint,bornUpEndPoint, true);
    //    //model->UpdateLowerBodyAnimation(elapsedTime, bornDownerEndPoint, true);
    //    break;
    //}
    //}
    //model->Update_blend_animations(0.675f, frontVec.x,1.582f);
    //model->Update_blend_animations(elapsedTime, frontVec.x,36,60, true);
    //model->Update_blend_animations(elapsedTime, frontVec.y,40,80, true);
    // 位置更新
    GetActor()->UpdateTransform();
    // アニメーション再生
    model->UpdateAnimation(elapsedTime, true);
    // 位置更新
    model->UpdateTransform(GetActor()->GetTransform());
    //GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());

    // モデル行列更新
    //model->UpdateTransform(transform);
}

void Player::Render(RenderContext rc)
{
    Graphics& graphics = Graphics::Instance();
    //Shader* shader = graphics.GetShader();
    ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
    shader->Begin(rc);// シェーダーにカメラの情報を渡す


    shader->Draw(rc, model);
   
    shader->End(rc);
}



// デバッグプリミティブ描画
void Player::DrawDebugPrimitive()
{
    // もし立法体等ほしいなら自分で追加
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //// 衝突判定用のデバッグ球を描画
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    // 衝突判定用のデバッグ円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    // 弾丸デバッグプリミティブ描画
    //projectileManager.DrawDebugPrimitive();

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

void Player::OnGUI()
{
    ImGui::InputFloat("Move Speed", &moveSpeed);
    ImGui::InputInt("Jump max", &jumpCount);
}


// 移動入力処理
bool Player::InputMove(float elapsedTime)
{
    // 進行ベクトル取得
    //DirectX::XMFLOAT3 moveVec = GetMoveVec();
    moveVec = GetMoveVec(elapsedTime);

    

    // 進行ベクトルがゼロベクトルでない場合は入力された
    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}


void Player::CameraControl(float elapsedTime)
{
  /*  float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();

    float lengthSq = ax * ax + ay * ay;
    if (lengthSq > 0.1f)
    {
        float speed = cameraRollSpeed * elapsedTime;

        cameraAngle.x += ay * speed;
        cameraAngle.y += ax * speed;

        if (cameraAngle.x < cameraMinPitch)
        {
            cameraAngle.x = cameraMinPitch;
        }
        if (cameraAngle.x > cameraMaxPitch)
        {
            cameraAngle.x = cameraMaxPitch;
        }
        if (cameraAngle.y < -DirectX::XM_PI)
        {
            cameraAngle.y += DirectX::XM_2PI;
        }
        if (cameraAngle.y > DirectX::XM_PI)
        {
            cameraAngle.y -= DirectX::XM_2PI;
        }
    }

    DirectX::XMVECTOR Offset = DirectX::XMVectorSet(0.0f, characterHeight, 0.0f, 0.0f);
    DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&GetActor()->GetPosition());
    DirectX::XMVECTOR Focus = DirectX::XMVectorAdd(Target, Offset);
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(cameraAngle.x, cameraAngle.y, cameraAngle.z);
    DirectX::XMVECTOR Range = DirectX::XMVectorSet(0.0f, 0.0f, -cameraRange, 0.0f);
    DirectX::XMVECTOR Vec = DirectX::XMVector3TransformCoord(Range, Transform);
    DirectX::XMVECTOR Eye = DirectX::XMVectorAdd(Focus, Vec);

    DirectX::XMFLOAT3 eye, focus;
    DirectX::XMStoreFloat3(&eye, Eye);
    DirectX::XMStoreFloat3(&focus, Focus);

    Camera& camera = Camera::Instance();
    camera.SetLookAt(eye, focus, DirectX::XMFLOAT3(0, 1, 0));*/
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
        movement->Move(vec,moveSpeed, elapsedTime);
        movement->Turn( vec ,turnSpeed, elapsedTime);
    }

   

    return vec;

}

void Player::CharacterControl(float elapsedTime)
{
  /*  float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    float lengthSq = ax * ax + ay * ay;
    if (lengthSq > 0.1f)
    {
        Camera& camera = Camera::Instance();
        const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
        const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

        DirectX::XMFLOAT3 direction;
        direction.x = (cameraRight.x * ax) + (cameraFront.x * ay);
        direction.y = 0.0f;
        direction.z = (cameraRight.z * ax) + (cameraFront.z * ay);

        float length = sqrtf(direction.x * direction.x + direction.z * direction.z);
        direction.x /= length;
        direction.z /= length;

        movement->Turn(direction, elapsedTime);
        movement->Move(direction,5 ,elapsedTime);
    }*/
}



// 描画処理
void Player::Render(const RenderContext& rc, ModelShader* shader)
{
    // 描画
    shader->Draw(rc, model);
    // 弾丸描画処理
    //projectileManager.Render(rc, shader);
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
    int enemyCount = EnemyManager::Instance().GetEnemyCount();
        Actor* projectile = projectileManager.GetProjectile(i);
        for (int j = 0; j < enemyCount; ++j)
        {
            Actor* enemy = enemyManager.GetEnemy(j);

            
            // 衝突処理
            DirectX::XMFLOAT3 outPositon;
            // 円柱と円
            if (Collision::IntersectSphereVsCylinder(
                projectile->GetPosition(),
                projectile->GetRadius(),
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPositon))

            {

                // ダメージを与える。
                if (enemy->GetComponent<HP>()->ApplyDamage(1, 0.5f))
                {
                    // 吹き飛ばす
                    {
                        // 衝動
                        DirectX::XMFLOAT3 impulse;
                        const float power = 10.0f;
                        const DirectX::XMFLOAT3& e = enemy->GetPosition();
                        const DirectX::XMFLOAT3& p = projectile->GetPosition();
                        float vx = e.x - p.x;
                        float vz = e.z - p.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;

                        impulse.x = vx * power;
                        impulse.y = power * 0.5f;
                        impulse.z = vz * power;

                        enemy->GetComponent<Movement>()->AddImpulse(impulse);
                    }
                    // ヒットエフェクト再生
                    {
                        DirectX::XMFLOAT3 e = enemy->GetPosition();
                        e.y += enemy->GetHeight() * 0.5f;


                        hitEffect->Play(e);
                    }
                    // 弾丸破棄
                    //projectile->;
                    //ActorManager::Instance().Remove();
                    //if (projectile->GetName() == "ProjectileStraight")
                    //ActorManager::Instance().Remove(projectile->GetComponent<ProjectileStraight>()->GetActor());

                    //if (projectile->GetName() == "ProjectileHoming")
                    //    ActorManager::Instance().Remove(projectile->GetComponent<ProjectileHoming>()->GetActor());


                    projectile->GetComponent<BulletFiring>()->Destroy();
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
            Actor* enemy = enemyManager.GetEnemy(i);


            //// 衝突処理
            DirectX::XMFLOAT3 outPositon;



            if (Collision::IntersectCylinderVsCylinder(
                position, radius, height,
                enemy->GetPosition(),
                enemy->GetRadius(),
                enemy->GetHeight(),
                outPositon))

            {


                    DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
                    DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
                    DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
                    DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
                    DirectX::XMFLOAT3 normal;
                    DirectX::XMStoreFloat3(&normal, N);

                    if (normal.y > 0.8f)
                    {
                        if (enemy->GetComponent<HP>()->ApplyDamage(1, 0.1f))
                        {
                            //小ジャンプ
                            //Jump(jumpSpeed * 0.5f);
                            movement->JumpVelocity(jumpSpeed*0.5f);
                        }

                    }
                    else
                    {
                        // 押し出し後の位置設定　
                        enemy->SetPosition(outPositon);
                    }
                
            }
            

        }
    
    


}

// ノードと敵の衝突判定
void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
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
        Actor* enemy = enemyManager.GetEnemy(i);


        //// 衝突処理
        DirectX::XMFLOAT3 outPositon;

        // 円柱と円
        if (Collision::IntersectSphereVsCylinder(
            nodePosition,
            nodeRadius,
            enemy->GetPosition(),
            enemy->GetRadius(),
            enemy->GetHeight(),
            outPositon))

        {

            // ダメージを与える。
            //enemy->ApplyDamage(1);
        // ダメージが通ったら消える。TRUEになるから
            if (enemy->GetComponent<HP>()->ApplyDamage(1, 0.5f))
            {

                // 吹き飛ばす
                {
                    // 衝動
                    DirectX::XMFLOAT3 impulse;
                    const float power = 10.0f;
                    const DirectX::XMFLOAT3& e = enemy->GetPosition();
                    const DirectX::XMFLOAT3& p = nodePosition;
                    float vx = e.x - p.x;
                    float vz = e.z - p.z;
                    float lengthXZ = sqrtf(vx * vx + vz * vz);
                    vx /= lengthXZ;
                    vz /= lengthXZ;

                    impulse.x = vx * power;
                    impulse.y = power * 0.5f;
                    impulse.z = vz * power;

                    enemy->GetComponent<Movement>()->AddImpulse(impulse);
                }
                // ヒットエフェクト再生
                {
                    DirectX::XMFLOAT3 e = enemy->GetPosition();
                    e.y += enemy->GetHeight() * 0.5f;


                    hitEffect->Play(e);


                    //desEffect->Play(e);

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

            ImGui::InputInt("jamplimit", &jumpCount);

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
        if (jumpCount < jumpLimit) {
            // ジャンプの最大値
            ++jumpCount;
            movement->JumpVelocity(jumpSpeed);

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
    if (gamePad.GetButtonDown() & GamePad::BTN_X)
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

            std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
            actor->AddComponent<ModelControll>();
            actor->GetComponent<ModelControll>()->LoadModel(filename);
            actor->SetName("ProjectileStraight");
            actor->SetPosition(position);
            actor->SetRotation(GetActor()->GetRotation());
            actor->SetScale(DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f));
            actor->AddComponent<BulletFiring>();
            actor->AddComponent<ProjectileStraight>();
            //actor->AddComponent<Collision>();
            ProjectileManager::Instance().Register(actor.get());
            //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
            Actor* projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

            // 発射
            actor->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        }

        //projectile->Lanch(dir, pos);
        return true;
    }

    // 追尾弾丸発射
    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
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
            Actor* enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // 敵の位置
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
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
                target = enemy->GetPosition();// 位置を入れる
                target.y += enemy->GetHeight() * 0.5f;// 位置に身長分
            }
            
            

        }


        // 発射　ストレート弾丸を用意
        //ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
        //projectile->Lanch(dir, pos,target);

        // 弾丸初期化
        const char* filename = "Data/Model/Sword/Sword.mdl";

        std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
        actor->AddComponent<ModelControll>();
        actor->GetComponent<ModelControll>()->LoadModel(filename);
        actor->SetName("ProjectileHoming");
        actor->SetPosition(position);
        actor->SetRotation(GetActor()->GetRotation());
        actor->SetScale(DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f));
        actor->AddComponent<BulletFiring>();
        actor->AddComponent<ProjectileHoming>();
        //actor->AddComponent<Collision>();
        ProjectileManager::Instance().Register(actor.get());
        //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        Actor* projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

        // 発射
        projectile->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        projectile->GetComponent<ProjectileHoming>()->SetTarget(target);


        return true;

    }
    return false;
}

bool Player::InputAttack()
{
    // 攻撃入力処理

    GamePad& gamePad = Input::Instance().GetGamePad();

    // 直進弾丸発射　xボタンを押したら
    if (gamePad.GetButtonDown() & GamePad::BTN_B)
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
    model->PlayAnimation(Anim_Idle, true);

    
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

    if (InputAttack())
    {
        //stated = state;
        TransitionAttackState();
    }

    // 弾丸入力処理
    if (InputProjectile())
    {
        TransitionAttackState();
    }
    //currentANimationSeconds = model->GetCurrentANimationSeconds();

    // 移動入力処理
    //InputMove(elapsedTime);


}

void Player::TransitionMoveState()
{

    afterimagemove = true;

    updateanim = UpAnim::Doble;

    // 上半身
    bornUpStartPoint = "mixamorig:Spine";
    // 下半身
    bornDownerEndPoint = "mixamorig:Spine";


    // 走りアニメーション再生
    model->PlayAnimation(Anim_Running, true);

    model->PlayUpeerBodyAnimation(Anim_Running, true);
    
    state = State::Move;
}

void Player::UpdateMoveState(float elapsedTime)
{
    // 位置情報を入れる。
    position = GetActor()->GetPosition();


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

    if (InputAttack())
    {
        //stated = state;
        TransitionAttackState();
    }

    

    // 弾丸入力処理
    if (InputProjectile())
    {
        TransitionAttackState();
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
    bornUpStartPoint = "mixamorig:Spine";

    model->PlayUpeerBodyAnimation(Anim_Jump, false);
    // ジャンプアニメーション再生
    model->PlayAnimation(Anim_Jump,false);

}

void Player::UpdateJumpState(float elapsedTime)
{

    //InputMove(elapsedTime);


    // ジャンプ入力処理
    if (InputJump()&&jumpCount >= 1)
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
    model->PlayAnimation(Anim_Landing, false);

    movement->SetOnLadius(false);

    //movement->SetStepOffSet(false);
    
}

void Player::UpdateLandState(float elapsedTime)
{
   

    // もし終わったら待機に変更
    if (!model->IsPlayAnimation())
    {
        movement->SetOnLadius(false);
        TransitionIdleState();
    }

    //currentANimationSeconds = model->GetCurrentANimationSeconds();
 
}





void Player::TransitionJumpFlipState()
{


    state = State::JumpFlip;

    // 走りアニメーション再生
    model->PlayAnimation(Anim_Jump_Flip, false);
    model->PlayUpeerBodyAnimation(Anim_Jump_Flip, false);
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

    state = State::Attack;

    updateanim = UpAnim::Normal;
    // 走りアニメーション再生
    model->PlayAnimation(Anim_Attack, false);
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
        bornUpStartPoint = "mixamorig:Spine";
        // 下半身
        bornDownerEndPoint = "mixamorig:Spine";
        model->PlayAnimation(Anim_Attack, false);
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
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
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
  
        Actor* projectile = projectileManager.GetProjectile(i);
        float dist = FLT_MAX;// float の最大値float全体
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)//float 最大値ないにいる敵に向かう
        {
            // 敵との距離判定  敵の数も計測 全ての敵をてに入れる
            Actor* enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // 敵の位置
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
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
                target = enemy->GetPosition();// 位置を入れる
                target.y += enemy->GetHeight() * 0.5f;// 位置に身長分
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
    model->PlayAnimation(Anim_GetHit1, false);
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
    model->PlayAnimation(Anim_Death, false);
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
    model->PlayAnimation(Anim_Revive, false);
}

void Player::UpdateReviveState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleState();

    }
}

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


void Player::Ground()
{
    if (movement->GetOnLadius())
    {
        jumpCount = 0;
        TransitionLandState();
    }
    
}

void PlayerManager::Register(Actor* actor)
{
    players.emplace_back(actor);
}
