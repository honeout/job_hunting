#include <imgui.h>
#include "EnemySlime.h"
//#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Mathf.h"
#include "Player.h"
//#include "Collision.h"
#include "StateDerived.h"


// ƒfƒXƒgƒ‰ƒNƒ^
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
    // ƒ€[ƒuƒƒ“ƒgŠÖ”‚ğg‚¦‚é‚æ‚¤‚É
    movement = GetActor()->GetComponent<Movement>();

    // hpŠÖ”‚ğg‚¦‚é‚æ‚¤‚É
    hp = GetActor()->GetComponent<HP>();

    // transformŠÖ”‚ğg‚¦‚é‚æ‚¤‚É
    transform = GetActor()->GetComponent<Transform>();

    // ƒ‚ƒfƒ‹ƒf[ƒ^‚ğ“ü‚ê‚éB
    model = GetActor()->GetComponent<ModelControll>()->GetModel();
    

    hp->SetHealth(health);

    hp->SetMaxHealth(maxHealth);

    transform->SetRadius(radius);

    transform->SetHeight(height);

    // œpœjƒXƒe[ƒg‚Ö‘JˆÚ
    //TransitionWanderState();

    //SetTerritory(position, territoryarea);

    //stateMachine = std::make_unique<StateMachine>();
    stateMachine = new StateMachine();

    // ƒXƒe[ƒgƒ}ƒVƒ“‚ÉƒXƒe[ƒg“o˜^
    stateMachine->RegisterState(new WanderState(GetActor().get()));
    stateMachine->RegisterState(new IdleState(GetActor().get()));
    stateMachine->RegisterState(new PursuitState(GetActor().get()));
    stateMachine->RegisterState(new AttackState(GetActor().get()));

    // ƒXƒe[ƒgƒZƒbƒg
    stateMachine->SetState(static_cast<int>(State::Idle));

}

// XVˆ—
void EnemySlime::Update(float elapsedTime)
{
    // ƒXƒe[ƒg–ˆ‚Ìˆ—
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

    // ˆÊ’u
    position = transform->GetPosition();
    // Œü‚«
    angle = transform->GetAngle();
    // ‘å‚«‚³
    scale = transform->GetScale();

    // ‘¬—Íˆ—XV
    movement->UpdateVelocity(elapsedTime);
    // –³“GŠÔXV
    hp->UpdateInbincibleTimer(elapsedTime);

    DrawDebugPrimitive();

    transform->UpdateTransform();

    model->UpdateAnimation(elapsedTime,true);

    //model->UpdateAnimation(elapsedTime, true);

    model->UpdateTransform(transform->GetTransform());
    //GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());
    
   // UpdateVelocity(elapsedTime);

    //UpdateInbincibleTimer(elapsedTime);
    // ƒIƒuƒWƒFƒNƒgs—ñ‚ğXV
    //UpdateTransform();

    //// ƒ‚ƒfƒ‹ƒAƒjƒ[ƒVƒ‡ƒ“XV
    //model->UpdateAnimation(elapsedTime);

    //// ƒ‚ƒfƒ‹s—ñXV
    //model->UpdateTransform(transform);
}

// •`‰æˆ—
//void EnemySlime::Render(const RenderContext& rc, ModelShader* shader)
//{
//    shader->Draw(rc, model);
//}

void EnemySlime::Render(RenderContext& rc)
{
    Graphics& graphics = Graphics::Instance();
    //Shader* shader = graphics.GetShader();
<<<<<<< HEAD
<<<<<<< HEAD
    ModelShader* shader = graphics.GetShader(ModelShaderId::Phong);
=======
    ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
>>>>>>> parent of 8a0ff20 (ã¨ã‚Šã‚ãˆãšã‚·ã‚§ãƒ¼ãƒ€ãƒ¼ç¶ºéº—ãªå¥´å…¥ã£ãŸ)
=======
    ModelShader* shader = graphics.GetShader(ModelShaderId::Lanbert);
    shader->Begin(rc);// ƒVƒF[ƒ_[‚ÉƒJƒƒ‰‚Ìî•ñ‚ğ“n‚·


    shader->Draw(rc, model);

    shader->End(rc);

}
// ƒVƒƒƒhƒEƒ}ƒbƒv
void EnemySlime::RenderShadowmap(RenderContext& rc)
{
    Graphics& graphics = Graphics::Instance();
    ModelShader* shader = graphics.GetShader(ModelShaderId::ShadowmapCaster);
>>>>>>> parent of 0db33c3 (ä¸€åº¦ä¿å­˜ãƒ¯ãƒ¼ãƒ‹ãƒ³ã‚°å¤§é‡)
    shader->Begin(rc);// ƒVƒF[ƒ_[‚ÉƒJƒƒ‰‚Ìî•ñ‚ğ“n‚·


    shader->Draw(rc, model);

    shader->End(rc);
}

// ƒfƒoƒbƒOƒvƒŠƒ~ƒeƒBƒu•`‰æ
void EnemySlime::DrawDebugPrimitive()
{
    // Šî’êƒNƒ‰ƒX‚ÌƒfƒoƒbƒOƒvƒŠƒ~ƒeƒBƒu•`‰æ
    //Enemy::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // “ê’£‚è”ÍˆÍ‚ğƒfƒoƒbƒO‰~’Œ•`‰æ
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f,
        DirectX::XMFLOAT4(0, 1, 0, 1));

    // ƒ^[ƒQƒbƒgˆÊ’u‚ğƒfƒoƒbƒO‹…•`‰æ
    debugRenderer->DrawSphere(targetPosition, radius, DirectX::XMFLOAT4(1, 1, 0, 1));
    
    // õ“G”ÍˆÍ‚ğƒfƒoƒbƒO‰~’Œ•`‰æ
    debugRenderer->DrawCylinder(position, searchRange, 1.0f, DirectX::XMFLOAT4(0, 0, 1, 1));

    // UŒ‚”ÍˆÍ‚ğƒfƒoƒbƒO‰~’Œ•`‰æ
    debugRenderer->DrawCylinder(position, attackRange, 1.0f, DirectX::XMFLOAT4(1, 0, 0, 1));
}

// “ê’£‚èİ’è
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
    // ƒ^[ƒQƒbƒg•ûŒü‚Ö‚ÌisƒxƒNƒgƒ‹‚ğZo
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;

    // ˆÚ“®ˆ—
    movement->Move({ vx,0.0f ,vz }, moveSpeed * speedRate, elapsedTime);
    movement->Turn({ vx,0.0f ,vz } ,turnSpeed,elapsedTime);
   // Move( vx,vz  ,moveSpeed * speedRate);
    //Turn(elapsedTime, vx, vz ,turnSpeed * speedRate);
    //movement->Turn({ vx,0.0f ,vz } ,turnSpeed * speedRate,elapsedTime);
}

// œpœjƒXƒe[ƒg‚Ö‘JˆÚ
void EnemySlime::TransitionWanderState()
{
    state = State::Wander;

    // –Ú•W’n“_İ’è
    SetRandomTargetPosition();

    // •à‚«ƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
    model->PlayAnimation(Anim_WalkFWD, true);
}

// œpœjƒXƒe[ƒgXVˆ—
void EnemySlime::UpdateWanderState(float elapsedTime)
{
    // –Ú•W’n“_‚Ü‚ÅXZ•½–Ê‚Å‚Ì‹——£”»’è
    float vx = targetPosition.x - position.x;
    float vz = targetPosition.z- position.z;
    float distSq = vx * vx + vz * vz;
    if (distSq < radius * radius)
    {

        //// ‘Ò‹@ƒXƒe[ƒg‚Ö
        TransitionIdleState();
    }

    // –Ú•W’n“_‚ÖˆÚ“®
    MoveToTarget(elapsedTime, 0.5f);

    // ƒvƒŒƒCƒ„[õ“G
    if (SearchPlayer())
    {
        // Œ©‚Â‚©‚Á‚½‚ç
        TransitionPursuitState();
    }
}

void EnemySlime::CollisitionNodeVsPlayer(const char* nodeName, float nodeRadius)
{
    // ƒm[ƒhˆÊ’u‚Æ“–‚½‚è”»’è‚ğs‚¤
    Model::Node* node = model->FindNode(nodeName);
    if (node != nullptr)
    {
        // ƒm[ƒh‚Ìƒ[ƒ‹ƒhÀ•W
        DirectX::XMFLOAT3 nodePosition(
        node->worldTransform._41,
        node->worldTransform._42,
        node->worldTransform._43
            );

        // “–‚½‚è”»’è•\¦
        Graphics::Instance().GetDebugRenderer()->DrawSphere(
            nodePosition, nodeRadius, DirectX::XMFLOAT4(0, 0, 1, 1)
        );

        // ƒvƒŒƒCƒ„[‚Æ“–‚½‚è”»’è
        //Player& player = Player::Instance();

        int playercount = PlayerManager::Instance().GetPlayerCount();
        
        for (int i = 0; i < playercount; ++i)
        {
            // ƒvƒŒƒCƒ„[æ“¾
            std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(i);

            //@ƒgƒ‰ƒ“ƒXƒtƒH[ƒ€•ª‰ğ
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
                // ƒ_ƒ[ƒW‚ğ—^‚¦‚é
                if (playerid->GetComponent<HP>()->ApplyDamage(1, 0.5f))
                {

                    // Õ“®
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



                    // ‚Á”ò‚Î‚·
                    playerid->GetComponent<Movement>()->AddImpulse(impulse);
                }
            }
        }
    }
}

bool EnemySlime::SearchPlayer()
{
    // ƒvƒŒƒCƒ„[æ“¾
    std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount()-1);
    
    //@ƒgƒ‰ƒ“ƒXƒtƒH[ƒ€•ª‰ğ
    DirectX::XMFLOAT3 playerPosition = playerid->GetComponent<Transform>()->GetPosition();
    float playerRadius = playerid->GetComponent<Transform>()->GetRadius();
    float playerHeight = playerid->GetComponent<Transform>()->GetRadius();


    
    // ƒvƒŒƒCƒ„[‚Æ‚’á·‚ğl—¶‚µ‚Ä‚RD‚Å‚Ì‹——£”»’è‚ğ‚·‚é
    //const DirectX::XMFLOAT3& playerPosition = playerid->GetPosition();
    //const DirectX::XMFLOAT3& playerPosition = DirectX::XMFLOAT3{0, 0, 0};
    float vx = playerPosition.x - position.x;
    float vy = playerPosition.y - position.y;
    float vz = playerPosition.z - position.z;
    // ƒ‹[ƒg
    float dist = sqrtf(vx * vx + vz * vz);
    if (dist < searchRange)
    {
        float distXZ = sqrtf(vx * vx + vz * vz);
        // ’PˆÊƒxƒNƒgƒ‹‰»
        vx /= distXZ;
        vz /= distXZ;
        // ‘O•ûƒxƒNƒgƒ‹
        float frontX = sinf(angle.y);
        float frontZ = cosf(angle.y);
        // ‚Q‚Â‚ÌƒxƒNƒgƒ‹‚Ì“àÏ’l‚Å‘OŒã”»’è
        float dot = (frontX * vx) + (frontZ * vz);
        //0.070G90“x
        //0.0fG”¼•ªˆÈã
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

    // ƒ^ƒCƒ}[‚ğƒ‰ƒ“ƒ_ƒ€‚ğİ’è@‘Ò‹@ŠÔ
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    // ‘Ò‹@ƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
    model->PlayAnimation(Anim_IdleNormal, true);
}

void EnemySlime::UpdateIdleState(float elapsedTime)
{

    // ƒ^ƒCƒ}[ˆ— ‘Ò‹@ŠÔ
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        // œpœjƒXƒe[ƒg‚Ö‘JˆÚ
        TransitionWanderState();
    }
}

void EnemySlime::TransitionPursuitState()
{
    state = State::Pursuit;

    // ”•bŠÔ’Ç”ö‚·‚éƒ^ƒCƒ}[‚ğƒ‰ƒ“ƒ_ƒ€İ’è
    stateTimer = Mathf::RandomRange(3.0f, 5.0f);

    // •à‚«ƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
    model->PlayAnimation(Anim_RunFWD, true);
}

// ’Ç”öƒXƒe[ƒgXVˆ—
void EnemySlime::UpdatePursuitState(float elapsedTime)
{
    // ƒvƒŒƒCƒ„[id
    std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
    // –Ú•W’n“_‚ğ‚ğƒvƒŒƒCƒ„[ˆÊ’u‚Éİ’è
    targetPosition = playerid->GetComponent<Transform>()->GetPosition();

    // –Ú•W’n“_‚ÖˆÚ“®
    MoveToTarget(elapsedTime, 1.0f);

    // ƒ^ƒCƒ}[ˆ—
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        // ‘Ò‹@ƒXƒe[ƒg‚Ö‘JˆÚ
        TransitionIdleState();
    }

    // ƒvƒŒƒCƒ„[‚Ì‹ß‚Ã‚­‚ÆUŒ‚ƒXƒe[ƒg‚Ö‘JˆÚ
    float vx = targetPosition.x - position.x;
    float vy = targetPosition.y - position.y;
    float vz = targetPosition.z - position.z;
    float dist = sqrtf(vx * vx + vy * vy + vz * vz);
    if (dist < attackRange)
    {
        // UŒ‚ƒXƒe[ƒg‚Ö‘JˆÚ
        TransitionAttackState();
    }



}

void EnemySlime::TransitionAttackState()
{
    state = State::Attack;

    // UŒ‚ƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
    model->PlayAnimation(Anim_Attack1, false);
}

void EnemySlime::UpdateAttackState(float elapsedTime)
{
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
    // ”CˆÓ‚ÌƒAƒjƒ[ƒVƒ‡ƒ“Ä¶‹æŠÔ‚Å‚Ì‚İÕ“Ë”»’èˆ—‚ğ‚·‚é
    float animationTime = model->GetCurrentANimationSeconds();
    if (animationTime >= 0.2f && animationTime <= 0.35f)
    {
        // –Ú‹Êƒm[ƒh‚ÆƒvƒŒƒCƒ„[‚ÌÕ“Ëˆ—
        CollisitionNodeVsPlayer("EyeBall", 0.2f);
    }
}

void EnemySlime::TransitionIdleBattleState()
{
    state = State::IdleBattle;

    // UŒ‚ƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
    model->PlayAnimation(Anim_IdleBattle, true);
}

void EnemySlime::UpdateIdleBattleState(float elapsedTime)
{
    // ƒvƒŒƒCƒ„[id
    std::shared_ptr<Actor> playerid = PlayerManager::Instance().GetPlayer(PlayerManager::Instance().GetPlayerCount() - 1);
    // –Ú•W’n“_‚ğ‚ğƒvƒŒƒCƒ„[ˆÊ’u‚Éİ’è
    targetPosition = playerid->GetComponent<Transform>()->GetPosition();

    // ƒ^ƒCƒ}[ˆ—
    stateTimer -= elapsedTime;
    if (stateTimer < 0.0f)
    {
        // ƒvƒŒƒCƒ„[‚ªUŒ‚”ÍˆÍ‚É‚¢‚½ê‡‚ÍUŒ‚ƒXƒe[ƒg‚Ö‘JˆÚ
        float vx = targetPosition.x - position.x;
        float vy = targetPosition.y - position.y;
        float vz = targetPosition.z - position.z;
        float dist = sqrtf(vx * vx + vy * vy + vz * vz);
        if (dist < attackRange)
        {
            // UŒ‚ƒXƒe[ƒg‚Ö‘JˆÚ
            TransitionAttackState();
        }
        else
        {
            // œpœjƒXƒe[ƒg‚Ö‘JˆÚ
            TransitionWanderState();
        }
    }
    MoveToTarget(elapsedTime, 0.0f);
}
// ƒ_ƒ[ƒWƒXƒe[ƒg‚Ö‘JˆÚ
void EnemySlime::TransitionDamageState()
{
    state = State::Damage;

    // ƒ_ƒ[ƒWƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
    model->PlayAnimation(Anim_GetHit, false);
}

void EnemySlime::UpdateDamageState(float elapsedTime)
{
    // ƒ_ƒ[ƒWƒAƒjƒ[ƒVƒ‡ƒ“‚ªI‚í‚Á‚½‚çí“¬‘Ò‹@ƒXƒe[ƒg‚Ö‘JˆÚ
    if (!model->IsPlayAnimation())
    {
        TransitionIdleBattleState();
    }
}

// €–SƒXƒe[ƒg‚Ö‘JˆÚ
void EnemySlime::TransitionDeathState()
{
    state = State::Death;

    // ƒ_ƒ[ƒWƒAƒjƒ[ƒVƒ‡ƒ“Ä¶
    model->PlayAnimation(Anim_Die, false);
}

void EnemySlime::UpdateDeathState(float elapsedTime)
{
    // ƒ_ƒ[ƒWƒAƒjƒ[ƒVƒ‡ƒ“‚ªI‚í‚Á‚½‚ç©•ª‚ğ”jŠü
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

//// €–S‚µ‚½‚ÉŒÄ‚Î‚ê‚é
//void EnemySlime::OnDead()
//{
//    // €–SƒXƒe[ƒg‚Ö‘JˆÚ
//    TransitionDeathState();
//}
//
//void EnemySlime::OnDamaged()
//{
//    // ƒ_ƒ[ƒWƒXƒe[ƒg‚Ö‘JˆÚ
//    TransitionDamageState();
//}



void EnemyManager::DeleteUpdate(float elapsedTime)
{
    // ”jŠüˆ— –ˆƒtƒŒ[ƒ€‚±‚±‚Åˆê‹C‚ÉÁ‚·B
    for (std::shared_ptr<Actor> enemy : removes)// E‚µ‚Ü‚·‚æƒŠƒXƒg‚ğE‚·
    {
        std::vector<std::shared_ptr<Actor>>::iterator it = std::find(enemies.begin(), enemies.end(),
            enemy);
        if (it != enemies.end())
        {
            enemies.erase(it);// íœ
        }

        // ’eŠÛ‚Ì”jŠüˆ—
        //delete projectile;// “ü‚ê•¨”jŠü

    }
    // ”jŠüƒŠƒXƒg‚ğƒNƒŠƒA
    removes.clear();
}

void EnemyManager::Register(std::shared_ptr<Actor> actor)
{
    enemies.emplace_back(actor);
}

void EnemyManager::Remove(std::shared_ptr<Actor> actor)
{
    // íœ“o˜^
    removes.insert(actor);
}
