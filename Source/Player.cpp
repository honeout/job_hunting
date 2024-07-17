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

#include "StateDerived.h"

#include "AfterimageManager.h"
#include "Graphics\PhongShader.h"


//static Player* instance = nullptr;
//
//// ƒCƒ“ƒXƒ^ƒ“ƒXæ“¾
//Player& Player::Instance()
//{
//    return *instance;
//}





// ƒfƒXƒgƒ‰ƒNƒ^
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


}



void Player::Start()
{

    // ƒ€[ƒuƒƒ“ƒgŠÖ”‚ğg‚¦‚é‚æ‚¤‚É
    movement = GetActor()->GetComponent<Movement>();

    // hpŠÖ”‚ğg‚¦‚é‚æ‚¤‚É
    hp = GetActor()->GetComponent<HP>();

    // ƒgƒ‰ƒ“ƒXƒtƒH[ƒ€ŠÖ”‚ğŒÄ‚Ño‚µ
    transform = GetActor()->GetComponent<Transform>();
    
    // ƒ‚ƒfƒ‹ƒf[ƒ^‚ğ“ü‚ê‚éB
    model = GetActor()->GetComponent<ModelControll>()->GetModel();
    // ƒJƒƒ‰‰Šú‰»
    cameraControlle = new CameraController();

    // ƒqƒbƒgƒGƒtƒFƒNƒg“Ç 
    hitEffect = new Effect("Data/Effect/sunder.efk");
    desEffect = new Effect("Data/Effect/F.efk");


    // ã”¼g
    bornUpStartPoint = "mixamorig:Spine";
    // ‰º”¼g
    bornDownerEndPoint = "mixamorig:Spine";

   
    // hpİ’è
    hp->SetHealth(health);
    // hpÅ‘å’l‚Ìİ’è
    hp->SetMaxHealth(maxHealth);
    // ”¼Œa
    transform->SetRadius(radius);
    // g’·
    transform->SetHeight(height);
    // ‘Ò‹@ƒXƒe[ƒg‚Ö‘JˆÚ
    TransitionIdleState();

    // ƒRƒ}ƒ“ƒh‘€ì—p
    selectCheck = 0;

    // “ÁêUŒ‚‚½‚ß‰Šú’l
    specialAttackCharge = 0.0f;

    // ƒXƒe[ƒgƒ}ƒVƒ“
    stateMachine = new StateMachine();

    stateMachine->RegisterState(new PlayerIdleState(GetActor().get()));
    stateMachine->RegisterState(new PlayerMovestate(GetActor().get()));
    stateMachine->RegisterState(new PlayerJumpState(GetActor().get()));
    stateMachine->RegisterState(new PlayerLandState(GetActor().get()));
    stateMachine->RegisterState(new PlayerJumpFlipState(GetActor().get()));
    stateMachine->RegisterState(new PlayerAttackState(GetActor().get()));
    stateMachine->RegisterState(new PlayerDamageState(GetActor().get()));
    stateMachine->RegisterState(new PlayerDeathState(GetActor().get()));
    stateMachine->RegisterState(new PlayerReviveState(GetActor().get()));

    // ƒXƒe[ƒgƒZƒbƒg
    stateMachine->SetState(static_cast<int>(State::Idle));
}

// XVˆ—
// elapsedTime(Œo‰ßŠÔ)
void Player::Update(float elapsedTime)
{
    //// ƒXƒe[ƒg–ˆ‚Ìˆ—
    stateMachine->Update(elapsedTime);

    //switch (state)
    //{
    //case State::Idle:
    //    UpdateIdleState(elapsedTime);
    //    break;
    //case State::Move:
    //    UpdateMoveState(elapsedTime);
    //    break;

    //case State::Jump:
    //    UpdateJumpState(elapsedTime);
    //    break;
    //case State::Land:
    //    UpdateLandState(elapsedTime);
    //    break;

    //case State::JumpFlip:
    //    UpdatejumpFlipState(elapsedTime);
    //    break;

    //case State::Attack:
    //    UpdateAttackState(elapsedTime);
    //    break;
    //case State::Damage:
    //    UpdateDamageState(elapsedTime);
    //    break;
    //case State::Death:
    //    UpdateDeathState(elapsedTime);
    //    break;
    //case State::Revive:
    //    UpdateReviveState(elapsedTime);
    //    break;
    //}
    
    // ‘¬—Íˆ—XV

    position = transform->GetPosition();

    angle = transform->GetAngle();

    scale = transform->GetScale();

    //velocity = movement->GetVelocity();

    //movement->SetVelocity(velocity);

    //velocity = movement->GetVelocity();

    hp->UpdateInbincibleTimer(elapsedTime);

    DrawDebugPrimitive();

    // ƒJƒƒ‰İ’è
    {
        cameraControlle->Update(elapsedTime);



        cameraControlle->SetTarget(position);



        //DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);

        //DirectX::XMVECTOR EPosition =  DirectX::XMLoadFloat3(&EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<Transform>()->GetPosition());

        //DirectX::XMVECTOR Pos = DirectX::XMVectorSubtract(Position,EPosition);

      

        //Pos = DirectX::XMVector3Normalize(Pos);

        //Pos = DirectX::XMVectorScale(Pos, 5.0f);

        //Pos = DirectX::XMVectorAdd(Position, Pos);

        //DirectX::XMFLOAT3 pos;

        //DirectX::XMStoreFloat3(&pos, Pos);

      
        //
        //pos = { position.x /2, 0, position.z /2};


        //cameraControlle->SetTarget(pos);
    }

    hitEffect->SetScale(hitEffect->GetEfeHandle(),{ 1,1,1 });
    // ‰Á‘¬“x“™
    movement->UpdateVelocity(elapsedTime);

    // ƒvƒŒƒCƒ„[‚Æ“G‚Æ‚ÌÕ“Ëˆ—
    CollisionPlayerVsEnemies();
    // ’eŠÛ“–‚½‚è”»’è
    CollisionProjectilesVsEnemies();

    //switch (updateanim)
    //{
    //case UpAnim::Normal:
    //{
    //    // ƒ‚ƒfƒ‹ƒAƒjƒ[ƒVƒ‡ƒ“XVˆ—
    //    //model->UpdateAnimation(elapsedTime, true);
    //    break;
    //}
    //case UpAnim::Doble:
    //{
    //    // ƒ‚ƒfƒ‹ƒAƒjƒ[ƒVƒ‡ƒ“XVˆ—
    //    //model->UpdateUpeerBodyAnimation(elapsedTime, bornUpStartPoint,bornUpEndPoint, true);
    //    //model->UpdateLowerBodyAnimation(elapsedTime, bornDownerEndPoint, true);
    //    break;
    //}
    //}
    //model->Update_blend_animations(0.675f, frontVec.x,1.582f);
    //model->Update_blend_animations(elapsedTime, frontVec.x,36,60, true);
    //model->Update_blend_animations(elapsedTime, frontVec.y,40,80, true);
    // ˆÊ’uXV
    transform->UpdateTransform();
    // ƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
    model->UpdateAnimation(elapsedTime, true);
    // ˆÊ’uXV
    model->UpdateTransform(transform->GetTransform());
    //GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());

    // ƒ‚ƒfƒ‹s—ñXV
    //model->UpdateTransform(transform);
}

void Player::Render(RenderContext& rc)
{
    Graphics& graphics = Graphics::Instance();
    //Shader* shader = graphics.GetShader();
    ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);
    shader->Begin(rc);// ƒVƒF[ƒ_[‚ÉƒJƒƒ‰‚Ìî•ñ‚ğ“n‚·


    shader->Draw(rc, model);
   
    shader->End(rc);
<<<<<<< HEAD

}

// ƒVƒƒƒhƒEƒ}ƒbƒv
void Player::RenderShadowmap(RenderContext& rc)
{
    Graphics& graphics = Graphics::Instance();
    ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);
    shader->Begin(rc);// ƒVƒF[ƒ_[‚ÉƒJƒƒ‰‚Ìî•ñ‚ğ“n‚·


    shader->Draw(rc, model);

    shader->End(rc);
    
=======
>>>>>>> parent of 8a0ff20 (ã¨ã‚Šã‚ãˆãšã‚·ã‚§ãƒ¼ãƒ€ãƒ¼ç¶ºéº—ãªå¥´å…¥ã£ãŸ)
}



// ƒfƒoƒbƒOƒvƒŠƒ~ƒeƒBƒu•`‰æ
void Player::DrawDebugPrimitive()
{
    // ‚à‚µ—§–@‘Ì“™‚Ù‚µ‚¢‚È‚ç©•ª‚Å’Ç‰Á
    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //// Õ“Ë”»’è—p‚ÌƒfƒoƒbƒO‹…‚ğ•`‰æ
    //debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

    // Õ“Ë”»’è—p‚ÌƒfƒoƒbƒO‰~’Œ‚ğ•`‰æ
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    // ’eŠÛƒfƒoƒbƒOƒvƒŠƒ~ƒeƒBƒu•`‰æ
    //projectileManager.DrawDebugPrimitive();

    if (attackCollisionFlag)
    {


        // UŒ‚Õ“Ë—p‚Ì¶èƒm[ƒhƒfƒoƒbƒO‹…‚ğ•`‰æ
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
    ImGui::InputInt("selectCheck", &selectCheck);
    ImGui::InputFloat("specialAttackCharge", &specialAttackCharge);
    ImGui::InputFloat("specialShotCharge", &specialShotCharge);

}


// ˆÚ“®“ü—Íˆ—
bool Player::InputMove(float elapsedTime)
{
    // isƒxƒNƒgƒ‹æ“¾
    //DirectX::XMFLOAT3 moveVec = GetMoveVec();
    moveVec = GetMoveVec(elapsedTime);

    

    // isƒxƒNƒgƒ‹‚ªƒ[ƒƒxƒNƒgƒ‹‚Å‚È‚¢ê‡‚Í“ü—Í‚³‚ê‚½
    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}
// UŒ‚•û–@‘I‘ğ
bool Player::InputSelectCheck()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_B)
    {
        ++selectCheck;
    }

    if (selectCheck >= 2)
    {
        selectCheck = 0;
    }
    return false;
}

bool Player::InputSpecialAttackCharge()
{

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_Y && specialAttackCharge >= 1.5f)
    {
        // ˆê“x”­“®‚·‚é‚Æ‰Šú‰»
        specialAttackCharge = 0.0f;
        return true;
    }
    return false;
}

bool Player::InputSpecialShotCharge()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_Y && specialShotCharge >= 1.5f)
    {
        // ˆê“x”­“®‚·‚é‚Æ‰Šú‰»
        specialShotCharge = 0.0f;
        return true;
    }
    return false;
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
    // “ü—Íî•ñ‚ğæ“¾
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    // ƒJƒƒ‰•ûŒü‚ÆƒXƒeƒBƒbƒN‚Ì“ü—Í’l‚É‚æ‚Á‚Äis•ûŒü‚ğŒvZ‚·‚é
    Camera& camera = Camera::Instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

    // ˆÚ“®ƒxƒNƒgƒ‹‚ÍXZ•½–Ê‚É…•½‚ÈƒxƒNƒgƒ‹‚É‚È‚é‚æ‚¤‚É‚·‚é

    // ƒJƒƒ‰‰E•ûŒüƒxƒNƒgƒ‹‚Í‚w‚y•½–Ê‚É…•½‚ÈƒxƒNƒgƒ‹‚É•ÏŠ·
    float cameraRightX =  cameraRight.x;
    float cameraRightZ =  cameraRight.z;
    // y¬•ª‚ğæ‚ç‚¸‚É@–îˆó‚Ì’·‚³‚ğæ“¾
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);

    // ‰½ŒÌY•ûŒü‚ğÁ‚µ‚Ä‚é‚©@‰E•ûŒü‚ªÎ‚ß‚Å‚à^‚Á’¼‚®i‚ñ‚Å‚Ù‚µ‚¢Y‚ğ‚O
    //@‚É‚·‚é­‚µ‹——£‚ª•Ï‚í‚é‚¾‚©‚ç’PˆÊƒxƒNƒgƒ‹‚É‚·‚é‚PD‚O‚É
    if (cameraRightLength > 0.0f)
    {
        // ’PˆÊƒxƒNƒgƒ‹‰»
        // ‰E•ûŒü‚Ì’PˆÊƒxƒNƒgƒ‹ 
        cameraRightX = cameraRightX/cameraRightLength ;
        cameraRightZ = cameraRightZ / cameraRightLength;
        
    }



    // ƒJƒƒ‰‘O•ûŒüƒxƒNƒgƒ‹‚ğXZ’PˆÊƒxƒNƒgƒ‹‚É•ÏŠ·
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        // ’PˆÊƒxƒNƒgƒ‹‰»
        cameraFrontX = cameraFrontX / cameraFrontLength;
        cameraFrontZ = cameraFrontZ / cameraFrontLength;
    
    }

    // ƒXƒeƒBƒbƒN‚Ì…•½“ü—Í’l‚ğƒJƒƒ‰‰E•ûŒü‚É”½‰f‚µA
    // ƒXƒeƒBƒbƒN‚Ì‚’¼“ü—Í’l‚ğƒJƒƒ‰‘O•ûŒü‚É”½‰f‚µA
    // isƒxƒNƒgƒ‹‚ğŒvZ‚·‚é
    DirectX::XMFLOAT3 vec;// ˆÚ“®•ûŒüi‚Ş‚×‚«•ûŒüisƒxƒNƒgƒ‹
    vec.x = (cameraRightX* ax) + (cameraFrontX * ay);// ‰E•ûŒü
    vec.z = (cameraRightZ *ax) + (cameraFrontZ * ay);// ‚Ü‚·‚Á‚®
    // Y²•ûŒü‚É‚ÍˆÚ“®‚µ‚È‚¢
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



//// •`‰æˆ—
//void Player::Render(RenderContext& rc, ModelShader* shader)
//{
//
//
//    shader->Begin(rc);
//    // •`‰æ
//    shader->Draw(rc, model);
//
//    shader->End(rc);
//    // ’eŠÛ•`‰æˆ—
//    //projectileManager.Render(rc, shader);
//}





// ’eŠÛ‚Æ“G‚ÌÕ“Ëˆ—
void Player::CollisionProjectilesVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();
   // Actor* enemyManager = EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1);
   // EnemyManager& enemyManager = EnemyManager::Instance();
   
    projectileManager = ProjectileManager::Instance();

    // ‘S‚Ä‚Ì“G‚Æ‘“–‚½‚è‚ÅÕ“Ëˆ—
    int projectileCount = projectileManager.GetProjectileCount();
    
    for (int i = 0; i < projectileCount; ++i)
    {
    int enemyCount = EnemyManager::Instance().GetEnemyCount();
    std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        for (int j = 0; j < enemyCount; ++j)
        {
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(j);
            
            DirectX::XMFLOAT3 projectilePosition = projectile->GetComponent<Transform>()->GetPosition();
            float projectileRadius = projectile->GetComponent<Transform>()->GetRadius();
            
            DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRadius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();

            // Õ“Ëˆ—
            DirectX::XMFLOAT3 outPositon;
            // ‰~’Œ‚Æ‰~
            if (Collision::IntersectSphereVsCylinder(
                projectilePosition,
                projectileRadius,
                enemyPosition,
                enemyRadius,
                enemyHeight,
                outPositon))

            {

                // ƒ_ƒ[ƒW‚ğ—^‚¦‚éB
                if (enemy->GetComponent<HP>()->ApplyDamage(1, 0.5f))
                {
                    // ‚«”ò‚Î‚·
                    {
                        // Õ“®
                        DirectX::XMFLOAT3 impulse;
                        const float power = 10.0f;
                        const DirectX::XMFLOAT3& e = enemyPosition;
                        const DirectX::XMFLOAT3& p = projectilePosition;
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
                    // ƒqƒbƒgƒGƒtƒFƒNƒgÄ¶
                    {
                        DirectX::XMFLOAT3 e = enemyPosition;
                        e.y += enemyHeight * 0.5f;


                        hitEffect->Play(e);
                    }
                    // “–‚½‚Á‚½‚Ì•›Ÿ“IŒø‰Ê
                    {
                        specialShotCharge += 0.1f;
                    }

                    // ’eŠÛ”jŠü
                    projectile->GetComponent<BulletFiring>()->Destroy();
                }
            }
        }
    }
}

// ƒvƒŒƒCƒ„[‚ÆƒGƒlƒ~[‚Æ‚ÌÕ“Ëˆ—
void Player::CollisionPlayerVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    // ‘S‚Ä‚Ì“G‚Æ‘“–‚½‚è‚ÅÕ“Ëˆ—

    int enemyCount = enemyManager.GetEnemyCount();


        for (int i = 0; i < enemyCount; ++i)
        {
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(i);


            //// Õ“Ëˆ—
            DirectX::XMFLOAT3 outPositon;

            DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRadius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetRadius();


            if (Collision::IntersectCylinderVsCylinder(
                position, radius, height,
                enemyPosition,
                enemyRadius,
                enemyHeight,
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
                        if (enemy->GetComponent<HP>()->ApplyDamage(1, 0.1f))
                        {
                            //¬ƒWƒƒƒ“ƒv
                            //Jump(jumpSpeed * 0.5f);
                            movement->JumpVelocity(jumpSpeed*0.5f);
                        }

                    }
                    else
                    {
                        // ‰Ÿ‚µo‚µŒã‚ÌˆÊ’uİ’è@
                        enemy->GetComponent<Transform>()->SetPosition(outPositon);
                    }
                
            }
            

        }
    
    


}

// ƒm[ƒh‚Æ“G‚ÌÕ“Ë”»’è
void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius)
{

    // ƒm[ƒhæ“¾
    Model::Node* node = model->FindNode(nodeName);
    //worldTransform
    //localTransform
    // ƒm[ƒhˆÊ’uæ“¾
    DirectX::XMFLOAT3 nodePosition;
    nodePosition = {
        node->worldTransform._41,
        node->worldTransform._42,
        node->worldTransform._43
    };
    // ƒ}ƒl[ƒWƒƒ[æ“¾
    EnemyManager& enemyManager = EnemyManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();
    // w’è‚Ìƒm[ƒh‚Æ‘S‚Ä‚Ì“G‚ğ‘“–‚½‚è‚ÅÕ“Ëˆ—
    for (int i = 0; i < enemyCount; ++i)
    {
        std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(i);

        DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
        float enemyRudius = enemy->GetComponent<Transform>()->GetRadius();
        float enemyHeight = enemy->GetComponent<Transform>()->GetRadius();

        //// Õ“Ëˆ—
        DirectX::XMFLOAT3 outPositon;

        // ‰~’Œ‚Æ‰~
        if (Collision::IntersectSphereVsCylinder(
            nodePosition,
            nodeRadius,
            enemyPosition,
            enemyRudius,
            enemyHeight,
            outPositon))

        {

            // ƒ_ƒ[ƒW‚ğ—^‚¦‚éB
            //enemy->ApplyDamage(1);
        // ƒ_ƒ[ƒW‚ª’Ê‚Á‚½‚çÁ‚¦‚éBTRUE‚É‚È‚é‚©‚ç
            if (enemy->GetComponent<HP>()->ApplyDamage(1, 0.5f))
            {

                // ‚«”ò‚Î‚·
                {
                    // Õ“®
                    DirectX::XMFLOAT3 impulse;
                    const float power = 10.0f;
                    const DirectX::XMFLOAT3& e = enemyPosition;
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
                // ƒqƒbƒgƒGƒtƒFƒNƒgÄ¶
                {
                    DirectX::XMFLOAT3 e = enemyPosition;
                    e.y += enemyHeight * 0.5f;


                    hitEffect->Play(e);


                    //desEffect->Play(e);

                }
                // “–‚½‚Á‚½‚Ì•›Ÿ“IŒø‰Ê
                {
                    specialAttackCharge += 0.1f;
                }

            }
        }



    }
 
}



// ƒfƒoƒbƒO—pGUI•`‰æ
void Player::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
    // begin‚©‚çend‚Ü‚Å‚Ì“à—e‚ªo—ˆ‚é
    if (ImGui::Begin("Player", nullptr, ImGuiWindowFlags_None))
    {
        // ƒgƒ‰ƒ“ƒXƒtƒH[ƒ€
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // ˆÊ’u@// ”’l‚ğ‚¢‚¶‚é
            ImGui::InputFloat3("Position", &position.x);

            //ImGui::SliderFloat3("Velocity", &velocity.x,5,0);

            ImGui::InputInt("jamplimit", &jumpCount);

            //ImGui::InputInt("hp", &health);

            ImGui::SliderFloat("frontVec", &frontVec.x,-0.37f,1.0);
            ImGui::SliderFloat("frontVecY", &frontVec.y,-0.37f,1.0);
            //ImGui::SliderFloat("frontVecX", &frontVec.y,0.0f,1.5);
        // ‰ñ“]
        DirectX::XMFLOAT3 a;
        // XMConvertToDegrees•’Ê‚Ì”š‚ğ‰½“x‚É‚·‚é
        a.x = DirectX::XMConvertToDegrees(angle.x);
        a.y = DirectX::XMConvertToDegrees(angle.y);
        a.z = DirectX::XMConvertToDegrees(angle.z);
        ImGui::InputFloat3("Angle", &a.x);
        angle.x = DirectX::XMConvertToRadians(a.x);
        angle.y = DirectX::XMConvertToRadians(a.y);
        angle.z = DirectX::XMConvertToRadians(a.z);
        // ƒXƒP[ƒ‹
        //ImGui::InputFloat3("Scale", &scale.x);

       

    }
    }
    ImGui::End();
}

void Player::Destroy()
{
    PlayerManager::Instance().Remove(GetActor());
}

//// ’…’n‚µ‚½‚ÉŒÄ‚Î‚ê‚é
//void Player::OnLanding()
//{
//    // ’…’n‚µ‚½‚©‚çƒŠƒZƒbƒg
//    jumpCount = 0;
//
//
//
//    //// ‰º•ûŒü‚Ì‘¬—Í‚ªˆê’èˆÈã‚È‚ç’…’nƒXƒe[ƒg‚Ö  \•ª‚È‘¬“x‚Å—‚Æ‚·d—Í‚Ì‚T”{‚QA‚R•bŒã‚É’…’nƒ‚[ƒVƒ‡ƒ“‚ğ‚·‚éB
//    if (velocity.y < gravity * 5.0f)
//    {
//        if (state != State::Damage && state != State::Death)
//        {
//            // ’…’nƒXƒe[ƒg‚Ö‘JˆÚ
//            TransitionLandState();
//        }
//    }
//}
//
//void Player::OnDead()
//{
//
//
//    // €–SƒXƒe[ƒg‘JˆÚ
//    TransitionDeathState();
//}
//
//void Player::OnDamaged()
//{
//    
//        // ƒ_ƒ[ƒWƒXƒe[ƒg‚Ö‘JˆÚ
//    TransitionDamageState();
//}

bool Player::InputJump()
{
    // ƒ{ƒ^ƒ“‚Å“ü—Í‚ÅƒWƒƒƒ“ƒviƒWƒƒƒ“ƒv‰ñ”§ŒÀ‚Â‚«j
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_A)
    {
        // ’l‚ğ¬‚³‚­‚·‚é
        if (jumpCount < jumpLimit) {
            // ƒWƒƒƒ“ƒv‚ÌÅ‘å’l
            ++jumpCount;
            movement->JumpVelocity(jumpSpeed);

            // ƒWƒƒƒ“ƒv“ü—Í‚µ‚½
           return true;
        }
    }

    return false;
}

bool Player::InputProjectile()
{
    GamePad& gamePad = Input::Instance().GetGamePad();




    // ’¼i’eŠÛ”­Ë@cƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚½‚ç
    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {
        // ‘O•ûŒü sin‚ÌŒvZ Šp“x‚ÌŒvZ
        DirectX::XMFLOAT3 dir;
        dir = GetForwerd(angle);
        
        // ”­ËˆÊ’uiƒvƒŒƒCƒ„[‚Ì˜“–‚½‚è)
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height*0.5f;// g’·€ˆÊ’u‚Ì‚™
        pos.z = position.z;
        // ”­Ë@ƒXƒgƒŒ[ƒg’eŠÛ‚ğ—pˆÓ
        {
            // ’eŠÛ‰Šú‰»
            const char* filename = "Data/Model/Sword/Sword.mdl";

            std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
            actor->AddComponent<ModelControll>();
            actor->GetComponent<ModelControll>()->LoadModel(filename);
            actor->SetName("ProjectileStraight");
            actor->AddComponent<Transform>();
            actor->GetComponent<Transform>()->SetPosition(position);
            actor->GetComponent<Transform>()->SetAngle(angle);
            actor->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f));
            actor->AddComponent<BulletFiring>();
            actor->AddComponent<ProjectileStraight>();
            //actor->AddComponent<Collision>();
            ProjectileManager::Instance().Register(actor);
            //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
            //std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

            // ”­Ë
            actor->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        }

        //projectile->Lanch(dir, pos);
        return true;
    }

    // ’Ç”ö’eŠÛ”­Ë
    if (gamePad.GetButtonDown() & GamePad::BTN_Y)
    {
        // ‘O•ûŒü sin‚ÌŒvZ
        DirectX::XMFLOAT3 dir;
        //dir.x = sinf(angle.y);// OŠp‚ğÎ‚ß‚É‚µ‚ÄˆÊ’u‚ğ•Ï‚¦‚½
        //dir.y = 0;
        //dir.z = cosf(angle.y);

        dir = GetForwerd(angle);

        //sinf0“x‚O@cosf0‚Í‚P“x
        //‚X‚Osin1,cos0•Ô‚Á‚Ä‚­‚é‰¡
        //‚S‚Tsin0.5,cos0.5Î‚ß
        // 360“x‚ğãè‚­•\Œ»o—ˆ‚éB2d‚Å‚às‚¯‚éB


        // ”­ËˆÊ’uiƒvƒŒƒCƒ„[‚Ì˜“–‚½‚è)
        DirectX::XMFLOAT3 pos;
        pos.x = position.x;
        pos.y = position.y + height * 0.5f;// g’·€ˆÊ’u‚Ì‚™
        pos.z = position.z;
        //ƒ^[ƒQƒbƒgiƒfƒtƒHƒ‹ƒg‚Å‚ÍƒvƒŒƒCƒ„[‚Ì‘O•ûj
        DirectX::XMFLOAT3 target;
        // “G‚ª‚¢‚È‚©‚Á‚½‚Ì‚½‚ß‚É@1000æ‚Ü‚Å”ò‚ñ‚Å‚­‚ê
        target.x = pos.x+dir.x * 1000.0f;
        target.y = pos.y+dir.y * 1000.0f;
        target.z = pos.z+dir.z * 1000.0f;

        // ˆê”Ô‹ß‚­‚Ì“G‚ğƒ^[ƒQƒbƒg‚É‚·‚é
        float dist = FLT_MAX;// float ‚ÌÅ‘å’lfloat‘S‘Ì
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)//float Å‘å’l‚È‚¢‚É‚¢‚é“G‚ÉŒü‚©‚¤
        {
            // “G‚Æ‚Ì‹——£”»’è  “G‚Ì”‚àŒv‘ª ‘S‚Ä‚Ì“G‚ğ‚Ä‚É“ü‚ê‚é
            std::shared_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // “G‚ÌˆÊ’u
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetComponent<Transform>()->GetPosition());
            // ©•ª‚©‚ç“G‚Ü‚Å‚ÌˆÊ’u‚ğŒv‘ª
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            // ƒxƒNƒgƒ‹‚Ì‚È‚ª‚³‚ğ‚Qæ‚·‚éBã‚Â‚¯‚Ä‚¢‚È‚¢“z
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                // ‹——£‚ª“G‚Ì‚à‚Ì‚ğ“ü‚ê‚é­‚È‚­‚·‚é‚R‚O‚È‚ç‚R‚OA‚P‚O‚O‚È‚ç‚P‚O‚O“ü‚ê‚é
                dist = d;
                target = enemy->GetComponent<Transform>()->GetPosition();// ˆÊ’u‚ğ“ü‚ê‚é
                target.y += enemy->GetComponent<Transform>()->GetHeight() * 0.5f;// ˆÊ’u‚Ég’·•ª
            }
            
            

        }


        // ”­Ë@ƒXƒgƒŒ[ƒg’eŠÛ‚ğ—pˆÓ
        //ProjectileHoming* projectile = new ProjectileHoming(&projectileManager);
        //projectile->Lanch(dir, pos,target);

        // ’eŠÛ‰Šú‰»
        const char* filename = "Data/Model/Sword/Sword.mdl";

        std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
        actor->AddComponent<ModelControll>();
        actor->GetComponent<ModelControll>()->LoadModel(filename);
        actor->SetName("ProjectileHoming");
        actor->AddComponent<Transform>();
        actor->GetComponent<Transform>()->SetPosition(position);
        actor->GetComponent<Transform>()->SetAngle(angle);
        actor->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(3.0f,3.0f,3.0f));
        actor->AddComponent<BulletFiring>();
        actor->AddComponent<ProjectileHoming>();
        //actor->AddComponent<Collision>();
        ProjectileManager::Instance().Register(actor);
        //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

        // ”­Ë
        projectile->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        projectile->GetComponent<ProjectileHoming>()->SetTarget(target);


        return true;

    }
    return false;
}

bool Player::InputAttack()
{
    // UŒ‚“ü—Íˆ—

    GamePad& gamePad = Input::Instance().GetGamePad();

    // ’¼i’eŠÛ”­Ë@xƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚½‚ç
    //if (gamePad.GetButtonDown() & GamePad::BTN_B)
    if (gamePad.GetButtonDown() & GamePad::BTN_X)
    {
  
        return true;
    }

    return false;
}

// ‘Ò‹@ƒXƒe[ƒg‚Ö‘JˆÚ
void Player::TransitionIdleState()
{
    state = State::Idle;

    afterimagemove = false;

    updateanim = UpAnim::Normal;

    // ‘Ò‹@ƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
    model->PlayAnimation(Anim_Idle, true);

    
}

// ‘Ò‹@ƒXƒe[ƒgXVˆ—
void Player::UpdateIdleState(float elapsedTime)
{
    // ˆÚ“®“ü—Íˆ—
    if (InputMove(elapsedTime))
    {
       
        TransitionMoveState();
    }

    // ƒWƒƒƒ“ƒv“ü—Íˆ—
    if (InputJump())
    {
        
        TransitionJumpState();
    }


    InputSelectCheck();

    if (InputAttack() && selectCheck == 0)
    {
        //stated = state;
        TransitionAttackState();
    }





    // ’eŠÛ“ü—Íˆ—
    else if (selectCheck == 1)
    {
        InputProjectile();
        //TransitionAttackState();
    }

    // “ÁêUŒ‚
    if (InputSpecialAttackCharge())
    {
        TransitionAttackState();
    }
    // “Áê–‚–@
    if (InputSpecialShotCharge())
    {
        InputProjectile();
    }
    //currentANimationSeconds = model->GetCurrentANimationSeconds();

    // ˆÚ“®“ü—Íˆ—
    //InputMove(elapsedTime);


}

void Player::TransitionMoveState()
{

    afterimagemove = true;

    updateanim = UpAnim::Doble;

    // ã”¼g
    bornUpStartPoint = "mixamorig:Spine";
    // ‰º”¼g
    bornDownerEndPoint = "mixamorig:Spine";


    // ‘–‚èƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
    model->PlayAnimation(Anim_Running, true);

    model->PlayUpeerBodyAnimation(Anim_Running, true);
    
    state = State::Move;
}

void Player::UpdateMoveState(float elapsedTime)
{
    // ˆÊ’uî•ñ‚ğ“ü‚ê‚éB
    //position = GetActor()->GetPosition();


    // ˆÚ“®“ü—Íˆ—
    if (!InputMove(elapsedTime))
    {
        
        TransitionIdleState();
    }

 

    // ƒWƒƒƒ“ƒv“ü—Íˆ—
    if (InputJump())
    {
        
        TransitionJumpState();
    }

    InputSelectCheck();

    if (InputAttack() && selectCheck == 0)
    {
        //stated = state;
        TransitionAttackState();
    }



    // ’eŠÛ“ü—Íˆ—
    else if (selectCheck == 1)
    {
        InputProjectile();
    }

    // “ÁêUŒ‚
    if (InputSpecialAttackCharge())
    {
        TransitionAttackState();
    }
    // “Áê–‚–@
    if (InputSpecialShotCharge())
    {
        InputProjectile();
    }
    // —‰º’…’n
    Ground();

    // c‘œp¨—p
    //currentANimationSeconds = model->GetCurrentANimationSeconds();



}




void Player::TransitionJumpState()
{

    state = State::Jump;

    // ã”¼g
    bornUpStartPoint = "mixamorig:Spine";

    model->PlayUpeerBodyAnimation(Anim_Jump, false);
    // ƒWƒƒƒ“ƒvƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
    model->PlayAnimation(Anim_Jump,false);

}

void Player::UpdateJumpState(float elapsedTime)
{

    //InputMove(elapsedTime);


    // ƒWƒƒƒ“ƒv“ü—Íˆ—
    if (InputJump()&&jumpCount >= 1)
    {
        TransitionJumpFlipState();
    }

    // ’eŠÛ“ü—Íˆ—
    if (InputProjectile())
    {
        TransitionAttackState();
    }


    // —‰º’…’n
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
    // ’…’nƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
    model->PlayAnimation(Anim_Landing, false);

    movement->SetOnLadius(false);

    //movement->SetStepOffSet(false);
    
}

void Player::UpdateLandState(float elapsedTime)
{
   

    // ‚à‚µI‚í‚Á‚½‚ç‘Ò‹@‚É•ÏX
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

    // ‘–‚èƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
    model->PlayAnimation(Anim_Jump_Flip, false);
    model->PlayUpeerBodyAnimation(Anim_Jump_Flip, false);
}

void Player::UpdatejumpFlipState(float elapsedTime)
{


    if (InputMove(elapsedTime))
        bool afterimagemove = true;
    // ƒWƒƒƒ“ƒv“ü—Íˆ—
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
    //    //ã”¼g
    //    bornUpStartPoint = "mixamorig:Spine2";

    //   /* bornUpEndPoint = "mixamorig:Neck";*/
    //    // ‰º”¼g
    //    bornDownerEndPoint = "mixamorig:Spine";

    //    model->PlayUpeerBodyAnimation(Anim_Attack, false);
    //}
    //else
    //{
    //    state = State::Attack;

    //    updateanim = UpAnim::Normal;
    //    // ‘–‚èƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
    //    model->PlayAnimation(Anim_Attack, false);
    //}

    state = State::Attack;

    updateanim = UpAnim::Normal;
    // ‘–‚èƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
    model->PlayAnimation(Anim_Attack, false);
}

void Player::UpdateAttackState(float elapsedTime)
{

    // ‚à‚µI‚í‚Á‚½‚ç‘Ò‹@‚É•ÏX
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
        //ã”¼g
        bornUpStartPoint = "mixamorig:Spine";
        // ‰º”¼g
        bornDownerEndPoint = "mixamorig:Spine";
        model->PlayAnimation(Anim_Attack, false);
    }

        // ”CˆÓ‚ÌƒAƒjƒ[ƒVƒ‡ƒ“Ä¶‹æŠÔ‚Å‚Ì‚İÕ“Ë”»’èˆ—‚ğ‚·‚é
        float animationTime = model->GetCurrentANimationSeconds();
        // ãè‚­s‚¯‚Î“G‚ª‰ñ”ğs“®‚ğæ‚Á‚Ä‚­‚ê‚és“®‚ğ—pˆÓo—ˆ‚éB
        attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
        if (updateanim == UpAnim::Doble)
        {
            // ”CˆÓ‚ÌƒAƒjƒ[ƒVƒ‡ƒ“Ä¶‹æŠÔ‚Å‚Ì‚İÕ“Ë”»’èˆ—‚ğ‚·‚é
            float animationTime = model->GetCurrentAnimationSecondsUpeer();
            // ãè‚­s‚¯‚Î“G‚ª‰ñ”ğs“®‚ğæ‚Á‚Ä‚­‚ê‚és“®‚ğ—pˆÓo—ˆ‚éB
            attackCollisionFlag = animationTime >= 0.3f && animationTime <= 0.4f;
        }
    if (attackCollisionFlag)
    {
        // ¶èƒm[ƒh‚ÆƒGƒlƒ~[‚ÌÕ“Ëˆ—
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius);
    }

    Ground();
    
}

void Player::UpdateProjectile(float elapsedTime)
{
    DirectX::XMFLOAT3 target;
    projectileManager = ProjectileManager::Instance();

    // ‘S‚Ä‚Ì“G‚Æ‘“–‚½‚è‚ÅÕ“Ëˆ—
    int projectileCount = projectileManager.GetProjectileCount();

    for (int i = 0; i < projectileCount; ++i)
    {
  
        std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        float dist = FLT_MAX;// float ‚ÌÅ‘å’lfloat‘S‘Ì
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)//float Å‘å’l‚È‚¢‚É‚¢‚é“G‚ÉŒü‚©‚¤
        {
            // “G‚Æ‚Ì‹——£”»’è  “G‚Ì”‚àŒv‘ª ‘S‚Ä‚Ì“G‚ğ‚Ä‚É“ü‚ê‚é
            std::shared_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);



            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // “G‚ÌˆÊ’u
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetComponent<Transform>()->GetPosition());
            // ©•ª‚©‚ç“G‚Ü‚Å‚ÌˆÊ’u‚ğŒv‘ª
            DirectX::XMVECTOR V = DirectX::XMVectorSubtract(E, P);
            // ƒxƒNƒgƒ‹‚Ì‚È‚ª‚³‚ğ‚Qæ‚·‚éBã‚Â‚¯‚Ä‚¢‚È‚¢“z
            DirectX::XMVECTOR D = DirectX::XMVector3LengthSq(V);
            float d;
            DirectX::XMStoreFloat(&d, D);
            if (d < dist)
            {
                // ‹——£‚ª“G‚Ì‚à‚Ì‚ğ“ü‚ê‚é­‚È‚­‚·‚é‚R‚O‚È‚ç‚R‚OA‚P‚O‚O‚È‚ç‚P‚O‚O“ü‚ê‚é
                dist = d;
                target = enemy->GetComponent<Transform>()->GetPosition();// ˆÊ’u‚ğ“ü‚ê‚é
                target.y += enemy->GetComponent<Transform>()->GetHeight() * 0.5f;// ˆÊ’u‚Ég’·•ª
                //if (projectile->GetModel())
                //    projectile->GetComponent<ProjectileHoming>()->SetTarget(target);
            }


        }
    }

}

void Player::TransitionDamageState()
{
    state = State::Damage;

    // ƒ_ƒ[ƒWƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
    model->PlayAnimation(Anim_GetHit1, false);
}

void Player::UpdateDamageState(float elapsedTime)
{
    // ƒ_ƒ[ƒWƒAƒjƒ[ƒVƒ‡ƒ“I‚í‚Á‚½‚ç‘Ò‹@ƒXƒe[ƒg‚Ö
    if (!model->IsPlayAnimation())
    {

            TransitionIdleState();

    }
}

void Player::TransitionDeathState()
{
    state = State::Death;

    // ƒ_ƒ[ƒWƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
    model->PlayAnimation(Anim_Death, false);
}

void Player::UpdateDeathState(float elapsedTime)
{   
    
    if (!model->IsPlayAnimation())
    {

       // ƒ{ƒ^ƒ“‚ğ‰Ÿ‚µ‚½‚ç•œŠˆƒXƒe[ƒg‚Ö‘JˆÚ
        GamePad& gamePad = Input::Instance().GetGamePad();
        if (gamePad.GetButtonDown() & GamePad::BTN_A)
        {
            TransitionReviveState();
        }


    }

}
// •œŠˆƒXƒe[ƒg‘JˆÚ
void Player::TransitionReviveState()
{
    state = State::Revive;

    // ‘Ì—Í‰ñ•œ
    health = maxHealth;


    // ƒ_ƒ[ƒWƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
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
//    // ˆÊ’uî•ñ
//    position = GetActor()->GetPosition();
//
//    // Œü‚«î•ñ
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
    dir.x = sinf(angle.y);// OŠp‚ğÎ‚ß‚É‚µ‚ÄˆÊ’u‚ğ•Ï‚¦‚½
    dir.y = 0;
    dir.z = cosf(angle.y);

    return dir;
}


bool Player::Ground()
{
    if (movement->GetOnLadius())
    {
        jumpCount = 0;
        //TransitionLandState();
        return true;
    }
    return false;
    
}




void PlayerManager::Register(std::shared_ptr<Actor> actor)
{
    players.emplace_back(actor);
}



void PlayerManager::Remove(std::shared_ptr<Actor> player)
{
    // íœ“o˜^
    removes.insert(player);
}
