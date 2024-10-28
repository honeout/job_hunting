#include <imgui.h>
#include "Player.h"
#include "Input/Input.h"
#include "Camera.h"
#include "Graphics/Graphics.h"
//#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "ProjectileRuby.h"
#include "ProjectileThrowing.h"
#include "Graphics/Model.h"
#include "EnemySlime.h"

#include "StateDerived.h"

#include "AfterimageManager.h"

#include "TransForm2D.h"

#include "ProjectileImpact.h"
#include "ProjectileSunder.h"
#include "BulletFiring.h"

#include "UiManager.h"
#include "Ui.h"







// デストラクタ
Player::~Player()
{

    if (movement)
        movement.reset();
    if (hp)
        hp.reset();
    if (transform)
        transform.reset();

    if (stateMachine)
    {
        delete stateMachine;
        stateMachine = nullptr;
    }

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
    if (fire != nullptr)
    {
        fire->Stop(fire->GetEfeHandle());
        delete fire;
        fire = nullptr;
    }

    if (cameraControlle != nullptr)
    {
        delete cameraControlle;
        cameraControlle = nullptr;
    }





    ProjectileManager::Instance().Clear();
  
}



void Player::Start()
{


    // ムーブメント関数を使えるように
    movement = GetActor()->GetComponent<Movement>();

    // 落下停止
    bool stopFall = false;
    movement->SetStopFall(stopFall);

    // 移動の停止
    bool stopMove = false;
    movement->SetStopMove(stopMove);

    // hp関数を使えるように
    hp = GetActor()->GetComponent<HP>();

    // トランスフォーム関数を呼び出し
    transform = GetActor()->GetComponent<Transform>();
    
    // 位置等
    position = transform->GetPosition();

    angle = transform->GetAngle();

    scale = transform->GetScale();

    // 重力設定
    movement->SetGravity(gravity);

    // モデルデータを入れる。
    model = GetActor()->GetComponent<ModelControll>()->GetModel();
    // カメラ初期化
    cameraControlle = new CameraController();

    // ヒットエフェクト読込 
    hitEffect = new Effect("Data/Effect/sunder.efk");
    ImpactEffect = new Effect("Data/Effect/rehleckte.efk");
    desEffect = new Effect("Data/Effect/F.efk");

    // エフェクト読み込み
    fire = new Effect("Data/Effect/fire.efk");

    // 上半身
    bornUpStartPoint = "mixamorig:Spine";
    // 下半身
    bornDownerEndPoint = "mixamorig:Spine";

   
    // hp設定
    hp->SetHealth(health);
    // hp最大値の設定
    hp->SetMaxHealth(maxHealth);
    // 半径
    transform->SetRadius(radius);
    // 身長
    transform->SetHeight(height);

    // コマンド操作用
    selectCheck = (int)CommandAttack::Attack;

    // 魔法選択用
    selectMagicCheck = (int)CommandMagic::Normal;

    // 特殊攻撃ため初期値
    specialAttackCharge = 0.0f;

    // ステートマシン
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
    stateMachine->RegisterState(new PlayerAvoidanceState(GetActor().get()));
    stateMachine->RegisterState(new PlayerReflectionState(GetActor().get()));

    // ステートセット
    stateMachine->SetState(static_cast<int>(State::Idle));

    // アニメーションルール
    updateanim = UpAnim::Normal;

    moveSpeedAnimation = 0.0f;

    // 特殊アクションの種類
    specialAttack.push((int)SpecialAttack::Normal);

    // 特殊アクション発動不
    specialAttackTime = false;

    // 揺れモード
    shakeMode = false;

    // 回転確認
    angleCheck = false;

    // 曲がる速度
    turnSpeedAdd = 0;



}

// 更新処理
// elapsedTime(経過時間)
void Player::Update(float elapsedTime)
{
    //// ステート毎の処理
    stateMachine->Update(elapsedTime);



    // コマンド操作
    {
        // 行動選択
        InputSelectCheck();
        // 魔法選択
        InputSelectMagicCheck();
        // 魔法選択ショートカットキー
        InputShortCutkeyMagic();




        // 攻撃の時にステートを変更
        if (
            InputAttack() && GetSelectCheck() ==
            (int)Player::CommandAttack::Attack && 
            GetStateMachine()->GetStateIndex() != static_cast<int>(Player::State::Attack)
            )
        {
            GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));

        }

        // 弾丸入力処理
        // 炎発射
        if (selectMagicCheck == (int)Player::CommandMagic::Fire && GetSelectCheck() == (int)Player::CommandAttack::Magic)
        {
            // 炎発射
            InputMagicframe();
            //TransitionAttackState();
        }
        // 雷
        if (selectMagicCheck == (int)Player::CommandMagic::Thander && GetSelectCheck() == (int)Player::CommandAttack::Magic)
        {
            // 雷発射
            InputMagicLightning();
            //TransitionAttackState();
        }
        // 氷
        if (selectMagicCheck == (int)Player::CommandMagic::Ice && GetSelectCheck() == (int)Player::CommandAttack::Magic)
        {
            // 氷発射
            InputMagicIce();
            //TransitionAttackState();
        }

        // 特殊攻撃
        if (InputSpecialAttackCharge())
        {
            
            //TransitionAttackState();
            GetStateMachine()->ChangeState(static_cast<int>(Player::State::Attack));
            // 特殊攻撃の発生条件 解消
            specialAttackTime = false;
                
        }
        // 特殊魔法
        if (InputSpecialShotCharge())
        {
            InputProjectile();
            // 特殊攻撃の発生条件 解消
            specialAttackTime = false;
        }

    }

    // 速力処理更新


    position = transform->GetPosition();

    angle = transform->GetAngle();

    scale = transform->GetScale();

    hp->UpdateInbincibleTimer(elapsedTime);

    // ロックオン
    InputRockOn();

    // カメラ設定
    if (rockCheck)
    {
        cameraControlle->RockUpdate(elapsedTime, position);

    }
    else
    {
        cameraControlle->SetTarget(
            { 
                position.x,
                position.y + height,
                position.z
            });

        cameraControlle->Update(elapsedTime);
    }

    // カメラ設定
    {


 //       cameraControlle->RockUpdate(elapsedTime);

 //       DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);

 //       DirectX::XMVECTOR EPosition =  DirectX::XMLoadFloat3(&EnemyManager::Instance().GetEnemy(EnemyManager::Instance().GetEnemyCount() - 1)->GetComponent<Transform>()->GetPosition());

 //       DirectX::XMVECTOR Pos = DirectX::XMVectorSubtract(EPosition, Position);

 //     

 //       Pos = DirectX::XMVector3Normalize(Pos);

 ///*       Pos = DirectX::XMVectorScale(Pos, 5.0f);

 //       Pos = DirectX::XMVectorAdd(Position, Pos);*/

 //       DirectX::XMFLOAT3 pos;

 //       DirectX::XMStoreFloat3(&pos, Pos);

 //     
 //       
 //       pos = { position.x /2, 0, position.z /2};


 //       cameraControlle->SetTarget(pos);
    }

    hitEffect->SetScale(hitEffect->GetEfeHandle(),{ 1,1,1 });
    // 加速度等
    movement->UpdateVelocity(elapsedTime);
    

    // プレイヤーと敵との衝突処理
    CollisionBornVsProjectile("shoulder");
    CollisionPlayerVsEnemies();
    // 弾丸当たり判定
    CollisionProjectilesVsEnemies();
    // ルビー当たり判定
    CollisionRubyVsEnemies();

    // 攻撃範囲内なのでUI描画
    AttackCheckUI();
    

    // ゲージ管理
    UiControlle(elapsedTime);

    // 位置更新
    transform->UpdateTransform();




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
        model->UpdateUpeerBodyAnimation(elapsedTime, bornUpStartPoint,true);
        model->UpdateLowerBodyAnimation(elapsedTime, bornDownerEndPoint, true);
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
    model->UpdateTransform(transform->GetTransform());
    //GetActor()->GetModel()->UpdateTransform(GetActor()->GetTransform());

    // モデル行列更新
    //model->UpdateTransform(transform);
}

void Player::Render(RenderContext& rc, ModelShader& shader)
{
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


    //// 弾丸デバッグプリミティブ描画
    //projectileManager.DrawDebugPrimitive();
    //for (int i = 0; i < projectileManager.GetProjectileCount(); ++i)
    //{

    //    projectileManager.GetProjectile(i)->GetComponent<ProjectileStraight>()->DrawDebugPrimitive();

    //}

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
    ImGui::InputInt("selectCheck", &selectCheck);
    ImGui::InputInt("selectMagicCheck", &selectMagicCheck);
    ImGui::InputInt("specialAttack.top", &specialAttack.top());
    ImGui::SliderFloat("specialAttackCharge", &specialAttackCharge,0,1.5f);
    ImGui::SliderFloat("specialShotCharge", &specialShotCharge,0,1.5f);

    ImGui::SliderFloat("dot", &dotfake, 0, 1);

    ImGui::SliderFloat("blend", &moveSpeedAnimation, 0.0f, 1.0f);

    

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
bool Player::InputRockOn()
{
   

    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT_SHOULDER && !buttonRock && !rockCheck)
    {

        buttonRock = true;
        rockCheck = true;
        return true;
    }

    if (rockCheck)
    {
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        DirectX::XMFLOAT3 epos = enemyManager.GetEnemy(0)->GetComponent<Transform>()->GetPosition();
        float eheight = enemyManager.GetEnemy(0)->GetComponent<Transform>()->GetHeight();


        DirectX::XMFLOAT3 pos;
        pos =
        {
            position.x - epos.x,
            position.y - epos.y,
            position.z - epos.z
        };
        //{
        //   epos.x - position.x,
        //   epos.y - position.y,
        //   epos.z - position.z,
        //};

        cameraControlle->SetTarget(
            {
                epos.x, 
                epos.y + eheight, 
                epos.z 
            });
        //cameraControlle->SetTarget(position);
        cameraControlle->SetFrontAngle(
            { 
                pos.x,
                pos.y,
                pos.z
            }
            //{
            //position.x,
            //-position.y,
            //position.z
            //}
        );
    }

    if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT_SHOULDER && !buttonRock && rockCheck)
    {
        cameraControlle->SetTarget(position);
        rockCheck = false;
        buttonRock = true;
        return true;
    }


    if (gamePad.GetButtonUp() & GamePad::BTN_RIGHT_SHOULDER)
        buttonRock = false;

    return false;
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
    // UI設定 階層下がる前 選ぶ
    // 攻撃選ぶ
    if (selectCheck == (int)CommandAttack::Attack)
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttack)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttackCheck)->GetComponent<Ui>();

        
        uiIdAttack->SetDrawCheck(false);
        uiIdAttackCheck->SetDrawCheck(true);
    }
    // 魔法選んだ時
    else
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttack)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttackCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(true);
        uiIdAttackCheck->SetDrawCheck(false);
    }
    // 魔法選んだ時
    if (selectCheck == (int)CommandAttack::Magic)
    {
        std::shared_ptr<Ui> uiIdMagick = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagick)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdMagickCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagickCheck)->GetComponent<Ui>();


        uiIdMagick->SetDrawCheck(false);
        uiIdMagickCheck->SetDrawCheck(true);
    }
    // 攻撃選ぶ
    else
    {
        std::shared_ptr<Ui> uiIdMagick = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagick)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdMagickCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagickCheck)->GetComponent<Ui>();


        uiIdMagick->SetDrawCheck(true);
        uiIdMagickCheck->SetDrawCheck(false);
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

    if (selectMagicCheck > (int)CommandMagic::Ice && magicAction)
    {
        selectMagicCheck = (int)CommandMagic::Fire;
    }

    if (selectMagicCheck < (int)CommandMagic::Fire && magicAction)
    {
        selectMagicCheck = (int)CommandMagic::Ice;
    }

    if (!magicAction)
    {
        selectMagicCheck = (int)CommandMagic::Normal;
    }

    // UI設定 炎
    if (selectMagicCheck == (int)CommandMagic::Fire && magicAction)
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFire)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFireCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(false);
        uiIdAttackCheck->SetDrawCheck(true);
    }
    else
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFire)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFireCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(true);
        uiIdAttackCheck->SetDrawCheck(false);
    }
    if (!magicAction)
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFire)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandFireCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(false);
        uiIdAttackCheck->SetDrawCheck(false);
    }


    // UI設定 雷
    if (selectMagicCheck == (int)CommandMagic::Thander && magicAction)
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtning)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtningCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(false);
        uiIdAttackCheck->SetDrawCheck(true);
    }
    else
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtning)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtningCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(true);
        uiIdAttackCheck->SetDrawCheck(false);
    }
    if (!magicAction)
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtning)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandRigtningCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(false);
        uiIdAttackCheck->SetDrawCheck(false);
    }


    // UI設定 氷
    if (selectMagicCheck == (int)CommandMagic::Ice && magicAction)
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIce)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIceCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(false);
        uiIdAttackCheck->SetDrawCheck(true);
    }
    else
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIce)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIceCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(true);
        uiIdAttackCheck->SetDrawCheck(false);
    }
    if (!magicAction)
    {
        std::shared_ptr<Ui> uiIdAttack = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIce)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandIceCheck)->GetComponent<Ui>();


        uiIdAttack->SetDrawCheck(false);
        uiIdAttackCheck->SetDrawCheck(false);
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
    }
    // 押してる間選択
    if (gamePad.GetButton() & GamePad::BTN_LEFT_SHOULDER)
    {
        magicAction = true;
    }
    // 離したら
    if (gamePad.GetButtonUp() & GamePad::BTN_LEFT_SHOULDER)
    {
        selectMagicCheck = (int)CommandMagic::Normal;
        magicAction = false;
    }
    return false;
}

bool Player::InputSpecialAttackCharge()
{

    GamePad& gamePad = Input::Instance().GetGamePad();

    if (specialAttackCharge >= 1.5f)
    {
        std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdSpecialChargeSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<Ui>();

        // 技確定
        std::shared_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
        std::shared_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();


        // 一度発動すると初期化
        specialAttackCharge = 0.0f;
        specialAttack.push((int)SpecialAttack::Attack);

        bool drawCheck = false;
        uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

        uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

        uiIdSpecialChargeSerde->SetDrawCheck(drawCheck);

        drawCheck = true;
        uiIdSpecialShurashu->SetDrawCheck(drawCheck);
        
        DirectX::XMFLOAT2 pos;
        pos = {94,240 };
        float add = 30;
        if (2 < (int)specialAttack.size())
        pos.y = pos.y - (add * (float)specialAttack.size());
        uiIdSpecialShurashuTransForm2D->SetPosition(pos);

    }

    if (gamePad.GetButtonDown() & GamePad::BTN_Y && specialAttack.top() == (int)SpecialAttack::Attack && !specialAttackTime)
    {
        //EnemyManager& enemyManager = EnemyManager::Instance();
        //int enemyCount = enemyManager.GetEnemyCount();
        //for (int i = 0; i < enemyCount; ++i)//float 最大値ないにいる敵に向かう
        //{
        //    // ターゲットまでのベクトルを算出
        //    DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);

        //    DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&enemyManager.GetEnemy(i)->GetComponent<Transform>()->GetPosition());
        //    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Position, Target);

        //    DirectX::XMFLOAT3 vec;
        //    DirectX::XMStoreFloat3(&vec, Vec);

        //    DirectX::XMFLOAT3 direction = GetForwerd(angle);

        //    // カメラ関係
        //    cameraControlle->SetAngle(vec);
        //}

        // 技確定
        std::shared_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdSpecialShurashuPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<Ui>();
        std::shared_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu)->GetComponent<TransForm2D>();
        std::shared_ptr<TransForm2D> uiIdSpecialShurashuPushTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<TransForm2D>();

        bool drawCheck = false;
        uiIdSpecialShurashu->SetDrawCheck(drawCheck);
        drawCheck = true;
        uiIdSpecialShurashuPush->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos;
        pos = { uiIdSpecialShurashuTransForm2D->GetPosition() };
        //float add = 30;
        //if (1 < (int)specialAttack.size())
        //    pos.y = pos.y - (add * (float)specialAttack.size());
        uiIdSpecialShurashuPushTransForm2D->SetPosition(pos);

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
        std::shared_ptr<Ui> uiIdSpecialShurashuPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashuPushu)->GetComponent<Ui>();

        bool drawCheck = false;
        uiIdSpecialShurashuPush->SetDrawCheck(drawCheck);
    }


    // チャージを見やすく
    if (specialAttackCharge >= 0.4f && specialAttackCharge < 0.8f)
    {
        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialCharge = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialCharge->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = {110,250 };
        uiIdSpecialTransForm2D->SetPosition(pos);
    }


    // チャージを見やすく
    if (specialAttackCharge >= 0.8f && specialAttackCharge < 1.2f)
    {
        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = { 47,250 };
        uiIdSpecialTransForm2DFurst->SetPosition(pos);

        uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

        pos = { 158,250 };
        uiIdSpecialTransForm2DSecond->SetPosition(pos);
    }

    // チャージを見やすく
    if (specialAttackCharge >= 1.2f && specialAttackCharge < 1.5f)
    {
        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();

        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = { 0,250 };
        uiIdSpecialTransForm2DFurst->SetPosition(pos);

        uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

        pos = { 94,250 };
        uiIdSpecialTransForm2DSecond->SetPosition(pos);

        uiIdSpecialChargeSerde->SetDrawCheck(drawCheck);

        pos = { 205,250 };
        uiIdSpecialTransForm2DSerde->SetPosition(pos);
    }

    return false;
}

bool Player::InputSpecialShotCharge()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (specialShotCharge >= 1.5f)
    {
        std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdSpecialChargeSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<Ui>();

        // 技確定
        std::shared_ptr<Ui> uiIdSpecialShurashu = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<Ui>();
        std::shared_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<TransForm2D>();


        // 一度発動すると初期化
        specialShotCharge = 0.0f;
        specialAttack.push((int)SpecialAttack::MagicFire);

        bool drawCheck = false;
        uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

        uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

        uiIdSpecialChargeSerde->SetDrawCheck(drawCheck);

        drawCheck = true;
        uiIdSpecialShurashu->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos;
        pos = { 94,240 };
        float add = 30;
        if (2 < (int)specialAttack.size())
            pos.y = pos.y - (add * (float)specialAttack.size());
        uiIdSpecialShurashuTransForm2D->SetPosition(pos);


    }


    if (gamePad.GetButtonDown() & GamePad::BTN_Y && specialAttack.top() == (int)SpecialAttack::MagicFire && !specialAttackTime)
    {
        // 技確定
        std::shared_ptr<Ui> uiIdSpecialMagic = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<Ui>();
        std::shared_ptr<Ui> uiIdSpecialMagicPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<Ui>();
        std::shared_ptr<TransForm2D> uiIdSpecialMagicTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame)->GetComponent<TransForm2D>();
        std::shared_ptr<TransForm2D> uiIdSpecialMagicPushTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<TransForm2D>();

        bool drawCheck = false;
        uiIdSpecialMagic->SetDrawCheck(drawCheck);
        drawCheck = true;
        uiIdSpecialMagicPush->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos;
        pos = { uiIdSpecialMagicTransForm2D->GetPosition() };
        //float add = 30;
        //if (1 > (int)specialAttack.size())
        //    pos.y = pos.y - (add * (float)specialAttack.size());
        uiIdSpecialMagicPushTransForm2D->SetPosition(pos);

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
        std::shared_ptr<Ui> uiIdSpecialMagicPush = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFramePushu)->GetComponent<Ui>();

        bool drawCheck = false;
        uiIdSpecialMagicPush->SetDrawCheck(drawCheck);
    }

    //if (gamePad.GetButtonDown() & GamePad::BTN_Y && specialAttack.top() == (int)SpecialAttack::MagicFire && !specialAttackTime)
    //{
    //    // 一度発動すると初期化
    //    if (specialAttack.top() != (int)SpecialAttack::Normal)
    //        specialAttack.pop();
    //    specialAttackTime = true;
    //    return true;
    //}

    // チャージを見やすく
    if (specialShotCharge >= 0.4f && specialShotCharge < 0.8f)
    {
        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2D = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialCharge = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialCharge->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = { 110,250 };
        uiIdSpecialTransForm2D->SetPosition(pos);
    }


    // チャージを見やすく
    if (specialShotCharge >= 0.8f && specialShotCharge < 1.2f)
    {
        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = { 47,250 };
        uiIdSpecialTransForm2DFurst->SetPosition(pos);

        uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

        pos = { 158,250 };
        uiIdSpecialTransForm2DSecond->SetPosition(pos);
    }

    // チャージを見やすく
    if (specialShotCharge >= 1.2f && specialShotCharge < 1.5f)
    {
        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeFurst = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge01)->GetComponent<Ui>();

        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeSecond = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge02)->GetComponent<Ui>();

        std::shared_ptr<TransForm2D> uiIdSpecialTransForm2DSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<TransForm2D>();
        std::shared_ptr<Ui> uiIdSpecialChargeSerde = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulCharge03)->GetComponent<Ui>();

        bool drawCheck = true;
        uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);

        DirectX::XMFLOAT2 pos = { 0,250 };
        uiIdSpecialTransForm2DFurst->SetPosition(pos);

        uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);

        pos = { 94,250 };
        uiIdSpecialTransForm2DSecond->SetPosition(pos);

        uiIdSpecialChargeSerde->SetDrawCheck(drawCheck);

        pos = { 205,250 };
        uiIdSpecialTransForm2DSerde->SetPosition(pos);
    }

    return false;
}


void Player::CameraControl(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    cameraControlle->SetAngleY(-angle.y);
    
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



//// 描画処理
//void Player::Render(RenderContext& rc, ModelShader* shader)
//{
//
//
//    shader->Begin(rc);
//    // 描画
//    shader->Draw(rc, model);
//
//    shader->End(rc);
//    // 弾丸描画処理
//    //projectileManager.Render(rc, shader);
//}





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
    std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        for (int j = 0; j < enemyCount; ++j)
        {
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(j);
            
            DirectX::XMFLOAT3 projectilePosition = projectile->GetComponent<Transform>()->GetPosition();
            float projectileRadius = projectile->GetComponent<Transform>()->GetRadius();
            
            DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRadius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();

            // 衝突処理
            DirectX::XMFLOAT3 outPositon;
          
            if (!projectile->GetComponent<ProjectileHoming>() && !projectile->GetComponent<ProjectileSunder>())return;

            // 円柱と円
            if (Collision::IntersectSphereVsCylinder(
                projectilePosition,
                projectileRadius,
                {
                  enemyPosition.x,
                  enemyPosition.y + enemyHeight / 2,
                  enemyPosition.z
                } ,
                enemyRadius,
                enemyHeight / 2,
                outPositon))

            {

                // ダメージを与える。
                if (enemy->GetComponent<HP>()->ApplyDamage(3, 0.5f))
                {
                    // 吹き飛ばす
                    {
                        // 衝動
        /*                DirectX::XMFLOAT3 impulse;
                        const float power = 10.0f;
                        const DirectX::XMFLOAT3& e = enemyPosition;
                        const DirectX::XMFLOAT3& p = projectilePosition;
                        float vx = e.x - p.x;
                        float vz = e.z - p.z;
                        float lengthXZ = sqrtf(vx * vx + vz * vz);
                        vx /= lengthXZ;
                        vz /= lengthXZ;*/

                       /* impulse.x = vx * power;
                        impulse.y = power * 0.5f;
                        impulse.z = vz * power;

                        enemy->GetComponent<Movement>()->AddImpulse(impulse);*/
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
                    if (projectile->GetComponent<ProjectileHoming>())
                    // 弾丸破棄
                    projectile->GetComponent<BulletFiring>()->Destroy();
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
        std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        for (int j = 0; j < enemyCount; ++j)
        {
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(j);

            DirectX::XMFLOAT3 projectilePosition = projectile->GetComponent<Transform>()->GetPosition();
            float projectileRadius = projectile->GetComponent<Transform>()->GetRadius();

            DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRadius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();

            // 衝突処理
            DirectX::XMFLOAT3 outPositon;

            if (!projectile->GetComponent<ProjectileThrowing>() )return;

            bool counterCheck;
            counterCheck = projectile->GetComponent<ProjectileThrowing>()->GetCounterCheck();

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
                if (enemy->GetComponent<HP>()->ApplyDamage(1, 0.5f))
                {
                    // 吹き飛ばす
                    {
                        //// 衝動
                        //DirectX::XMFLOAT3 impulse;
                        //const float power = 10.0f;
                        //const DirectX::XMFLOAT3& e = enemyPosition;
                        //const DirectX::XMFLOAT3& p = projectilePosition;
                        //float vx = e.x - p.x;
                        //float vz = e.z - p.z;
                        //float lengthXZ = sqrtf(vx * vx + vz * vz);
                        //vx /= lengthXZ;
                        //vz /= lengthXZ;

                        //impulse.x = vx * power;
                        //impulse.y = power * 0.5f;
                        //impulse.z = vz * power;

                        //enemy->GetComponent<Movement>()->AddImpulse(impulse);
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
                    projectile->GetComponent<BulletFiring>()->Destroy();
                    enemy->GetComponent<EnemySlime>()->GetStateMachine()->ChangeState
                    ((int)EnemySlime::State::IdleBattle);
                   
                        
                        
               
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
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(i);


            //// 衝突処理
            DirectX::XMFLOAT3 outPositon;

            DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRadius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();


            if (Collision::IntersectCylinderVsCylinder(
                position, radius, height,
                enemyPosition,
                enemyRadius,
                enemyHeight/2,
                
                outPositon))

            {
                //// 吹き飛ばす
                //{
                //    // 衝動
                //    DirectX::XMFLOAT3 impulse;
                //    const float power = 10.0f;
                //    const DirectX::XMFLOAT3& e = enemyPosition;
                //    const DirectX::XMFLOAT3& p = projectilePosition;
                //    float vx = e.x - p.x;
                //    float vz = e.z - p.z;
                //    float lengthXZ = sqrtf(vx * vx + vz * vz);
                //    vx /= lengthXZ;
                //    vz /= lengthXZ;

                //    impulse.x = vx * power;
                //    impulse.y = power * 0.5f;
                //    impulse.z = vz * power;
                //}

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
                        movement->JumpVelocity(jumpSpeed * 0.5f);


                    }
                    else
                    {
                        
                        // 押し出し後の位置設定　
                        //enemy->GetComponent<Transform>()->SetPosition(outPositon);
                        //position = outPositon;
                        const float power = 2.3f;

                        DirectX::XMFLOAT3 impulse;
                        impulse.x = normal.x * power;
                        impulse.y = power * normal.y;
                        impulse.z = normal.z * power;

                        movement->AddImpulse(impulse);
                    }
                
            }
            

        }
    
    


}

void Player::CollisionBornVsProjectile(const char* bornname)
{

    // ノード取得
    Model::Node* nodehand = model->FindNode(bornname);

    EnemyManager& enemyManager = EnemyManager::Instance();

    // 全ての敵と総当たりで衝突処理

    int enemyCount = enemyManager.GetEnemyCount();


    for (int i = 0; i < enemyCount; ++i)
    {
        std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(i);

        // ノード取得
        Model::Node* node = enemy->GetComponent<ModelControll>()->GetModel()->FindNode(bornname);

        // ノード位置取得
        DirectX::XMFLOAT3 nodePosition;
        nodePosition = {
            node->worldTransform._41,
            node->worldTransform._42,
            node->worldTransform._43
        };

        //// 衝突処理
        DirectX::XMFLOAT3 outPositon;

        DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
        float enemyRadius = enemy->GetComponent<Transform>()->GetRadius();
        float enemyHeight = enemy->GetComponent<Transform>()->GetRadius();


        if (Collision::IntersectCylinderVsCylinder(
            position, radius, height,
            nodePosition,
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

                //小ジャンプ
                //Jump(jumpSpeed * 0.5f);
                movement->JumpVelocity(jumpSpeed * 0.5f);


            }
            else
            {
                // 押し出し後の位置設定　
                //enemy->GetComponent<Transform>()->SetPosition(outPositon);
                
                const float power = 2.3f;

                float velocityMax = 2;

                DirectX::XMFLOAT3 impulse;
                impulse.x = normal.x * power;
                impulse.y = power * normal.y;
                impulse.z = normal.z * power;
                if(movement->GetVelocity().x <= velocityMax)
                movement->AddImpulse(impulse);


            }

        }


    }
}

// ノードと敵の衝突判定
void Player::CollisionNodeVsEnemies(const char* nodeName, float nodeRadius, const char* nodeNameSeconds)
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
        std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(i);

        DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
        float enemyRudius = enemy->GetComponent<Transform>()->GetRadius();
        float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();

        //// 衝突処理
        DirectX::XMFLOAT3 outPositon;

        // 円柱と円
        if (Collision::IntersectSphereVsCylinder(
            nodePosition,
            nodeRadius, 
            {
            enemyPosition.x,
            enemyPosition.y + enemyHeight / 2,
            enemyPosition.z 
            },
            enemyRudius,
            enemyHeight / 2,
            outPositon))

        {

            // ダメージを与える。
            //enemy->ApplyDamage(1);
        // ダメージが通ったら消える。TRUEになるから
            if (enemy->GetComponent<HP>()->ApplyDamage(5, 0.5f))
            {

                //// 吹き飛ばす
                //{
                //    // 衝動
                //    DirectX::XMFLOAT3 impulse;
                //    const float power = 10.0f;
                //    const DirectX::XMFLOAT3& e = enemyPosition;
                //    const DirectX::XMFLOAT3& p = nodePosition;
                //    float vx = e.x - p.x;
                //    float vz = e.z - p.z;
                //    float lengthXZ = sqrtf(vx * vx + vz * vz);
                //    vx /= lengthXZ;
                //    vz /= lengthXZ;

                //    impulse.x = vx * power;
                //    impulse.y = power * 0.5f;
                //    impulse.z = vz * power;

                //    enemy->GetComponent<Movement>()->AddImpulse(impulse);
                //}
                // ヒットエフェクト再生
                {
                    DirectX::XMFLOAT3 e = enemyPosition;
                    e.y += enemyHeight * 0.5f;


                    hitEffect->Play(e);


                    //desEffect->Play(e);

                }
                // 当たった時の副次的効果
                {
                    specialAttackCharge += 0.1f;
                }

            }
        }



    }
 
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
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(i);
            std::shared_ptr<Actor> projectille = projectileManager.GetProjectile(i);

            //enemy->GetComponent<ModelControll>()->GetModel()->FindNode

            if (!projectille->GetComponent<ProjectileImpact>()) return;

            DirectX::XMFLOAT3 projectilePosition = projectille->GetComponent<Transform>()->GetPosition();
            float projectileInRudius = projectille->GetComponent<ProjectileImpact>()->GetRadiusInSide();
            float projectileOutRudius = projectille->GetComponent<ProjectileImpact>()->GetRadiusOutSide();
            float projectileHeight = projectille->GetComponent<Transform>()->GetHeight();


            /*DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRudius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();*/


            //// 衝突処理
            DirectX::XMFLOAT3 outPositon;

            std::shared_ptr<EnemySlime> enemySlime = enemy->GetComponent<EnemySlime>();


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
                    outPositon) &&projectille->GetComponent<ProjectileImpact>()) /*&&
                enemySlime->GetCounterJudgment())*/
            {
                // 高さが一定以下なら通る
                if (projectilePosition.y + projectileHeight < nodePosition.y) return;


                //enemySlime->GetStateMachine()->ChangeState((int)EnemySlime::State::Idle);
                // ヒットエフェクト再生
                {
                    //DirectX::XMFLOAT3 e = enemyPosition;
                    //e.y += enemyHeight * 0.5f;

                    projectille->GetComponent<ProjectileImpact>()->Destoroy();

                    ImpactEffect->Play(nodePosition);


                    //desEffect->Play(e);

                    //enemySlime->SetCounterJudgment(false);
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
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(i);
            std::shared_ptr<Actor> projectille = projectileManager.GetProjectile(i);

            //enemy->GetComponent<ModelControll>()->GetModel()->FindNode

            if (!projectille->GetComponent<ProjectileRuby>()) return;
           


            DirectX::XMFLOAT3 projectilePosition = projectille->GetComponent<Transform>()->GetPosition();
            float projectileRudius = projectille->GetComponent<Transform>()->GetRadius();
            float projectileHeight = projectille->GetComponent<Transform>()->GetHeight();


            /*DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRudius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();*/


            //// 衝突処理
            DirectX::XMFLOAT3 outPositon;

            std::shared_ptr<EnemySlime> enemySlime = enemy->GetComponent<EnemySlime>();


            // 球と球
            if (Collision::IntersectSpherVsSphere(
                nodePosition,
                nodeRadius,
                projectilePosition,
                projectileRudius,
                outPositon)) /*&&
                enemySlime->GetCounterJudgment())*/
            {
                // 高さが一定以下なら通る
                if (projectilePosition.y + projectileHeight < nodePosition.y) return;


                //enemySlime->GetStateMachine()->ChangeState((int)EnemySlime::State::Idle);
                // ヒットエフェクト再生
                {
                    //DirectX::XMFLOAT3 e = enemyPosition;
                    //e.y += enemyHeight * 0.5f;

                    projectille->GetComponent<BulletFiring>()->Destroy();
                    

                    ImpactEffect->Play(nodePosition);


                    //desEffect->Play(e);

                    //enemySlime->SetCounterJudgment(false);
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
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(j);
    for (int i = 0; i < projectileCount; ++i)
    {
      
            std::shared_ptr<Actor> projectille = projectileManager.GetProjectile(i);

            if (!projectille->GetComponent<ProjectileThrowing>()) return;

            bool counterCheck = projectille->GetComponent<ProjectileThrowing>()->GetCounterCheck();


            DirectX::XMFLOAT3 projectilePosition = projectille->GetComponent<Transform>()->GetPosition();
            float projectileRudius = projectille->GetComponent<Transform>()->GetRadius();
            float projectileHeight = projectille->GetComponent<Transform>()->GetHeight();


            /*DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRudius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();*/


            //// 衝突処理
            DirectX::XMFLOAT3 outPositon;

            std::shared_ptr<EnemySlime> enemySlime = enemy->GetComponent<EnemySlime>();


            // 球と球
            if (Collision::IntersectSpherVsSphere(
                nodePosition,
                nodeRadius,
                projectilePosition,
                projectileRudius,
                outPositon) && !counterCheck) /*&&
                enemySlime->GetCounterJudgment())*/
            {
                // 高さが一定以下なら通る
               /* if (projectilePosition.y + projectileHeight < nodePosition.y) return;*/


                //enemySlime->GetStateMachine()->ChangeState((int)EnemySlime::State::Idle);
                // ヒットエフェクト再生
                {
                    //DirectX::XMFLOAT3 e = enemyPosition;
                    //e.y += enemyHeight * 0.5f;

                    //projectille->GetComponent<BulletFiring>()->Destroy();
                    projectille->GetComponent<ProjectileThrowing>()->SetTarget(
                        {
                            enemy->GetComponent<Transform>()->GetPosition().x,
                            enemy->GetComponent<Transform>()->GetPosition().y + enemy->GetComponent<Transform>()->GetHeight(),
                            enemy->GetComponent<Transform>()->GetPosition().z,
                        }
                    );

                    DirectX::XMFLOAT3 direction = GetForwerd(angle);

                    float lifeTimerCounter = 5.0f;
                    projectille->GetComponent<BulletFiring>()->Lanch(direction, projectilePosition, lifeTimerCounter);

                    ImpactEffect->Play(nodePosition);

                    counterCheck = true;
                    projectille->GetComponent<ProjectileThrowing>()->SetCounterCheck(counterCheck);

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
        if (movement->GetJumpCount() < jumpLimit) {
           
            
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

            // これが２Dかの確認
            bool check2d = false;
            actor->SetCheck2d(check2d);

            // 飛ぶ時間
            float   lifeTimer = 3.0f;
            // 発射
            actor->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
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
            std::shared_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // 敵の位置
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetComponent<Transform>()->GetPosition());
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
                target = enemy->GetComponent<Transform>()->GetPosition();// 位置を入れる
                target.y += enemy->GetComponent<Transform>()->GetHeight() * 0.5f;// 位置に身長分
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
        actor->AddComponent<Transform>();
        actor->GetComponent<Transform>()->SetPosition(position);
        actor->GetComponent<Transform>()->SetAngle(angle);
        actor->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(3.0f,3.0f,3.0f));
        actor->AddComponent<BulletFiring>();
        actor->AddComponent<ProjectileHoming>();

        // これが２Dかの確認
        bool check2d = false;
        actor->SetCheck2d(check2d);

        //actor->AddComponent<Collision>();
        ProjectileManager::Instance().Register(actor);
        //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

        // 飛ぶ時間
        float   lifeTimer = 3.0f;
        // 発射
        projectile->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        projectile->GetComponent<ProjectileHoming>()->SetTarget(target);

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

    // 上半身
    bornUpStartPoint = "mixamorig:Spine";
    // 下半身
    bornDownerEndPoint = "mixamorig:Spine";


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
    bornUpStartPoint = "mixamorig:Spine";

    //model->PlayUpeerBodyAnimation(Anim_Jump, false);
    //// ジャンプアニメーション再生
    //model->PlayAnimation(Anim_Jump,false);

}

void Player::UpdateJumpState(float elapsedTime)
{

    //InputMove(elapsedTime);


    // ジャンプ入力処理
    if (InputJump()&&movement->GetJumpCount() >= 1)
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

    state = State::Attack;

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
        bornUpStartPoint = "mixamorig:Spine";
        // 下半身
        bornDownerEndPoint = "mixamorig:Spine";
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
        CollisionNodeVsEnemies("mixamorig:LeftHand", leftHandRadius, "shoulder");
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
  
        std::shared_ptr<Actor> projectile = projectileManager.GetProjectile(i);
        float dist = FLT_MAX;// float の最大値float全体
        EnemyManager& enemyManager = EnemyManager::Instance();
        int enemyCount = enemyManager.GetEnemyCount();
        for (int i = 0; i < enemyCount; ++i)//float 最大値ないにいる敵に向かう
        {
            // 敵との距離判定  敵の数も計測 全ての敵をてに入れる
            std::shared_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);



            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // 敵の位置
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetComponent<Transform>()->GetPosition());
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
                target = enemy->GetComponent<Transform>()->GetPosition();// 位置を入れる
                target.y += enemy->GetComponent<Transform>()->GetHeight() * 0.5f;// 位置に身長分
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
    if (gamePad.GetButtonDown() & GamePad::BTN_B&& magicAction && !gamePad.GetButtonDownCountinue())
    {

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
            std::shared_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // 敵の位置
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetComponent<Transform>()->GetPosition());
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
                target = enemy->GetComponent<Transform>()->GetPosition();// 位置を入れる
                target.y += enemy->GetComponent<Transform>()->GetHeight() * 0.5f;// 位置に身長分
            }



        }

        // 弾丸初期化
        const char* filename = "Data/Model/Sword/Sword.mdl";

        std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
        actor->AddComponent<ModelControll>();
        actor->GetComponent<ModelControll>()->LoadModel(filename);
        actor->SetName("ProjectileHoming");
        actor->AddComponent<Transform>();
        actor->GetComponent<Transform>()->SetPosition(position);
        actor->GetComponent<Transform>()->SetAngle(angle);
        actor->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
        actor->AddComponent<BulletFiring>();
        actor->AddComponent<ProjectileHoming>();
        const char* effectFilename = "Data/Effect/fire.efk";
        actor->GetComponent<ProjectileHoming>()->SetEffectProgress(effectFilename);
        //actor->GetComponent<ProjectileHoming>()->EffectProgressPlay();
        // これが２Dかの確認
        bool check2d = false;
        actor->SetCheck2d(check2d);

        //actor->AddComponent<Collision>();
        ProjectileManager::Instance().Register(actor);
        //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

        // 飛ぶ時間
        float   lifeTimer = 4.0f;
        // 発射
        projectile->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        projectile->GetComponent<ProjectileHoming>()->SetTarget(target);

        // 攻撃解除
        magicAction = false;
        selectMagicCheck = (int)CommandMagic::Normal;
     
        return true;
    }
    else if (gamePad.GetButtonUp() & GamePad::BTN_B)
    {
        gamePad.SetButtonDownCountinue(false);
    }
    return false;
}

bool Player::InputMagicIce()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_B && magicAction && !gamePad.GetButtonDownCountinue())
    {

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
            std::shared_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // 敵の位置
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetComponent<Transform>()->GetPosition());
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
                target = enemy->GetComponent<Transform>()->GetPosition();// 位置を入れる
                target.y += enemy->GetComponent<Transform>()->GetHeight() * 0.5f;// 位置に身長分
            }



        }

        // 弾丸初期化
        const char* filename = "Data/Model/Sword/Sword.mdl";

        std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
        actor->AddComponent<ModelControll>();
        actor->GetComponent<ModelControll>()->LoadModel(filename);
        actor->SetName("ProjectileHoming");
        actor->AddComponent<Transform>();
        actor->GetComponent<Transform>()->SetPosition(position);
        actor->GetComponent<Transform>()->SetAngle(angle);
        actor->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
        actor->AddComponent<BulletFiring>();
        actor->AddComponent<ProjectileHoming>();
        const char* effectFilename = "Data/Effect/brezerd.efk";
        actor->GetComponent<ProjectileHoming>()->SetEffectProgress(effectFilename);
        //actor->GetComponent<ProjectileHoming>()->EffectProgressPlay();

        // これが２Dかの確認
        bool check2d = false;
        actor->SetCheck2d(check2d);

        //actor->AddComponent<Collision>();
        ProjectileManager::Instance().Register(actor);
        //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);
        
        // 飛ぶ時間
        float   lifeTimer = 4.0f;
        // 発射
        projectile->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        projectile->GetComponent<ProjectileHoming>()->SetTarget(target);

        // 攻撃解除
        magicAction = false;
        selectMagicCheck = (int)CommandMagic::Normal;

        return true;
    }
    else if (gamePad.GetButtonUp() & GamePad::BTN_B)
    {
        gamePad.SetButtonDownCountinue(false);
    }
    return false;
}

bool Player::InputMagicLightning()
{
    GamePad& gamePad = Input::Instance().GetGamePad();
    if (gamePad.GetButtonDown() & GamePad::BTN_B && magicAction && !gamePad.GetButtonDownCountinue())
    {

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
            std::shared_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);
            DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
            // 敵の位置
            DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetComponent<Transform>()->GetPosition());
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
                target = enemy->GetComponent<Transform>()->GetPosition();// 位置を入れる
                target.y += enemy->GetComponent<Transform>()->GetHeight() * 0.5f;// 位置に身長分
            }



        }

        // 弾丸初期化
        const char* filename = "Data/Model/Sword/Sword.mdl";

        std::shared_ptr<Actor> actor = ActorManager::Instance().Create();
        actor->AddComponent<ModelControll>();
        actor->GetComponent<ModelControll>()->LoadModel(filename);
        actor->SetName("ProjectileSunder");
        actor->AddComponent<Transform>();
        actor->GetComponent<Transform>()->SetPosition(pos);
        actor->GetComponent<Transform>()->SetAngle(angle);
        actor->GetComponent<Transform>()->SetScale(DirectX::XMFLOAT3(0.01f, 0.01f, 0.01f));
        actor->AddComponent<BulletFiring>();
        actor->AddComponent<ProjectileSunder>();
        const char* effectFilename = "Data/Effect/lightningStrike.efk";
        actor->GetComponent<ProjectileSunder>()->SetEffectProgress(effectFilename);
        //actor->GetComponent<ProjectileHoming>()->EffectProgressPlay();

        // これが２Dかの確認
        bool check2d = false;
        actor->SetCheck2d(check2d);

        //actor->AddComponent<Collision>();
        ProjectileManager::Instance().Register(actor);
        //ProjectileStraight* projectile = new ProjectileStraight(&projectileManager);
        std::shared_ptr<Actor> projectile = ProjectileManager::Instance().GetProjectile(ProjectileManager::Instance().GetProjectileCount() - 1);

        float   lifeTimer = 0.5f;

        // 発射
        projectile->GetComponent<BulletFiring>()->Lanch(dir, pos, lifeTimer);
        projectile->GetComponent<ProjectileSunder>()->SetTarget(target);

        // 攻撃解除
        magicAction = false;
        selectMagicCheck = (int)CommandMagic::Normal;

        return true;
    }
    else if (gamePad.GetButtonUp() & GamePad::BTN_B)
    {
        gamePad.SetButtonDownCountinue(false);
    }
    return false;
}

void Player::AttackCheckUI()
{
    // UIカーソル
    //std::shared_ptr<Ui> uiSight = UiManager::Instance().GetUies((int)UiManager::UiCount::Sight)->GetComponent<Ui>();
    

    EnemyManager& enemyManager = EnemyManager::Instance();

    int enemyCount = enemyManager.GetEnemyCount();
    switch (selectCheck)
    {
    case (int)CommandAttack::Attack:
    {
        for (int i = 0; i < enemyCount; ++i)
        {
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(i);
            ////// 衝突処理
            //DirectX::XMFLOAT3 outPositon;

            //DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            //float enemyRadius = enemy->GetComponent<Transform>()->GetRadius();


            DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
            float enemyRudius = enemy->GetComponent<Transform>()->GetRadius();
            float enemyHeight = enemy->GetComponent<Transform>()->GetHeight();

            //// 衝突処理
            DirectX::XMFLOAT3 outPositon;

            // 円柱と円
            if (Collision::IntersectSphereVsCylinder(
                position,
                radius,
                {
                enemyPosition.x,
                enemyPosition.y + enemyHeight / 2,
                enemyPosition.z
                },
                enemyRudius,
                enemyHeight / 2,
                outPositon))

            {
                std::shared_ptr<Ui> uiSightAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<Ui>();

                bool drawCheck = true;
                uiSightAttackCheck->SetDrawCheck(drawCheck);
            }
            else
            {
                std::shared_ptr<Ui> uiSightAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<Ui>();

                bool drawCheck = false;
                uiSightAttackCheck->SetDrawCheck(drawCheck);
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
            std::shared_ptr<Actor> enemy = enemyManager.GetEnemy(i);


            DirectX::XMVECTOR playerPosition =
                DirectX::XMLoadFloat3(&position);
            DirectX::XMFLOAT3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();

            DirectX::XMVECTOR enemyPositionXM =
                DirectX::XMLoadFloat3(&enemyPosition);

            DirectX::XMVECTOR LengthSq =
                DirectX::XMVectorSubtract(playerPosition,enemyPositionXM);
            LengthSq = DirectX::XMVector3LengthSq(LengthSq);
            
            float lengthSq;
            DirectX::XMStoreFloat(&lengthSq, LengthSq);
           

            if (lengthSq < magicRangeLength)
            {
                std::shared_ptr<Ui> uiSightAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<Ui>();

                bool drawCheck = true;
                uiSightAttackCheck->SetDrawCheck(drawCheck);
            }
            else
            {
                std::shared_ptr<Ui> uiSightAttackCheck = UiManager::Instance().GetUies((int)UiManager::UiCount::SightCheck)->GetComponent<Ui>();

                bool drawCheck = false;
                uiSightAttackCheck->SetDrawCheck(drawCheck);
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
        std::shared_ptr<Actor> enemy = EnemyManager::Instance().GetEnemy(i);

        enemy->GetComponent<EnemySlime>()->SetInvalidJudgment(invalidJudgment);
    }
}

void Player::UiControlle(float elapsedTime)
{

    float gaugeWidth = hp->GetMaxHealth() * hp->GetHealth() * 0.1f;
    // hpゲージ
    std::shared_ptr<TransForm2D> uiHp = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerHPBar)->GetComponent<TransForm2D>();
    std::shared_ptr<TransForm2D> uiHpBar = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerHp)->GetComponent<TransForm2D>();
    DirectX::XMFLOAT2 scale = { gaugeWidth, uiHp->GetScale().y };

    uiHp->SetScale(scale);

    
    // 揺れ
    if (shakeMode)
    {
        
        uiHp->Shake();
        uiHpBar->Shake();

        

    }
    //　初期化
    if (uiHp->GetShakeEnd())
    {
        float positionStandardBar = 593;
        float positionStandard = 421;
        shakeMode = false;
        uiHp->SetShakeTime(0);
        uiHpBar->SetShakeTime(0);
        uiHp->SetPosition({ uiHp->GetPosition().x ,positionStandardBar });
        uiHpBar->SetPosition({ uiHpBar->GetPosition().x ,positionStandard });
    }
}


bool Player::Ground()
{
    if (movement->GetOnLadius())
    {
        //jumpCount = 0;
        //TransitionLandState();
        return true;
    }
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

