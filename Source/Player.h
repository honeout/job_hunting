#pragma once

#include <DirectXMath.h>
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
//#include "Character.h"
//#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileManager.h"
#include "Component.h"
#include "Movement.h"
#include "HP.h"
#include "Input\GamePad.h"
#include "CameraController.h"
#include "Effect.h"
#include "ModelControll.h"
#include "Transform.h"
#include "StateMachine.h"

enum class UpAnim
{
    Normal,
    Doble,
    Blend,

};

// プレイヤー
class Player : public Component
{
public:
    Player() {};
    ~Player() override;



    //// インスタンス取得
    //static Player& Instance();


    // アニメーション
    enum Animation
    {
        Anim_Attack,
        Anim_Death,
        Anim_Falling,
        Anim_GetHit1,
        Anim_GetHit2,
        Anim_Idle,
        Anim_Jump,
        Anim_Jump_Flip,
        Anim_Landing,
        Anim_Revive,
        Anim_Running,
        Anim_Walking
    };

    // 名前取得
    const char* GetName() const override { return "Player"; }

    //void UpdateTransform();

    // 開始処理
    void Start() override;

    // 更新処理
    void Update(float elapsedTime) override;

    // 描画処理
    //void Render(RenderContext rc) override;

    // デバッグプリミティブ描画　デバッグ用
    void DrawDebugPrimitive();
    // GUI描画
    void OnGUI() override;

    // 描画処理
    void Render(RenderContext& rc, ModelShader& shader) override;

    // シャドウマップ処理
    void RenderShadowmap(RenderContext& rc) override;


    // 弾丸と敵の衝突処理
    void CollisionProjectilesVsEnemies();

    // プレイヤーとエネミーとの衝突処理
    void CollisionPlayerVsEnemies();

    // デバッグ用GUI描画
    void DrawDebugGUI();

    // 破棄
    void Destroy();

protected:
    //// 着地した時に呼ばれる
    //void OnLanding() override;

    //// 死亡した時に呼ばれる
    //void OnDead() override;

    //// ダメージを受けた時に呼ばれる
    //void OnDamaged() override;

public:

    // カメラ操作
    void CameraControl(float elapsedTime);

    // スティック入力値から移動ベクトルを取得 進行ベクトルを取る進むべき方向
    DirectX::XMFLOAT3 GetMoveVec(float elapsedTime) const;

    //// キャラクター操作
    void CharacterControl(float elapsedTime);


    // 移動入力処理
    bool InputMove(float elapsedTime);

    // 攻撃方法選択
    bool InputSelectCheck();

    // 特殊攻撃選択
    bool InputSpecialAttackCharge();
    bool InputSpecialShotCharge();
 


    // プレイヤーとエネミーとの衝突処理
    //void CollisionPlayerVsEnemies();

    // ノードとエネミーの衝突処理
    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

    // ノードとエネミーの衝突処理
    void CollisionNodeVsEnemiesCounter(const char* nodeName, float nodeRadius);


    // ジャンプ入力処理
    bool InputJump();

    // 弾丸入力
    bool InputProjectile();

    // 回避入力
    bool InputAvoidance();

    // 攻撃入力
    bool InputAttack();

    // 待機ステートへ遷移
    void TransitionIdleState();
    // 待機ステート更新
    void UpdateIdleState(float elapsedTime);

    // 移動ステートへ遷移    
    void TransitionMoveState();

    // 移動ステートへ更新
    void UpdateMoveState(float elapsedTime);


    // ジャンプステートへ遷移
    void TransitionJumpState();

    // ジャンプステート更新処理
    void UpdateJumpState(float elapsedTime);

    // 着地ステートへ遷移
    void TransitionLandState();

    // 着地ステージ更新処理
    void UpdateLandState(float elapsedTime);

    // 二段ジャンプステートへ遷移
    void TransitionJumpFlipState();

    // 二段ジャンプステート更新
    void UpdatejumpFlipState(float elapsedTime);
    

    // 攻撃ステートへ遷移
    void TransitionAttackState();

    // 攻撃ステージ更新処理
    void UpdateAttackState(float elapsedTime);

    void UpdateProjectile(float elapsedTime);

    // ダメージステートへ遷移
    void TransitionDamageState();

    // ダメージステートへ更新処理
    void UpdateDamageState(float elapsedTime);

    // 死亡ステートへ遷移
    void TransitionDeathState();

    // 死亡ステート更新処理
    void UpdateDeathState(float elapsedTime);



    // 復活ステートへ遷移
    void TransitionReviveState();

    // 復活ステート更新処理
    void UpdateReviveState(float elapsedTime);

   // void Update();

    void inFloat3(DirectX::XMFLOAT3 value, DirectX::XMFLOAT3& inValue);

    DirectX::XMFLOAT3 GetForwerd(DirectX::XMFLOAT3 angle);

    // ステートマシーン取得
    StateMachine* GetStateMachine() { return stateMachine; }

    // 攻撃方法変更
    void SetSelectCheck(bool selectCheck) { this->selectCheck = selectCheck; }
    // 攻撃方法変更
    bool GetSelectCheck() const { return selectCheck; }

    // 特殊魔法発動ため
    void SetSpecialAttackCharge(float specialAttackCharge) { this->specialAttackCharge = specialAttackCharge; }

    // 特殊アクション発動ため
    float GetSpecialAttackCharge() const { return specialAttackCharge; }

    // ジャンプの最大値
    int GetJumpCount() { return jumpCount; }

    // 攻撃開始
    void SetAttackCollisionFlag(bool attackCollisionFlag) { this->attackCollisionFlag = attackCollisionFlag; }

    // 攻撃開始
    bool GetAttackCollisionFlag() const { return attackCollisionFlag; }

    // 左手の当たり判定
    float GetLeftHandRadius() const { return leftHandRadius; }

    // ゲット当たり判定無効
    bool GetInvalidJudgment() const { return invalidJudgment;}
    // セット当たり判定無効
    void SetInvalidJudgment(bool invalidJudgment) { this->invalidJudgment = invalidJudgment;}

    // 自分の当たり判定有無
    void DmageInvalidJudment(bool invalidJudgment);

public:
    // ステート
    enum class State
    {
        // ステート入れるのは後ろから
        Idle,
        Move,
        Jump,
        Land,
        JumpFlip,
        Attack,
        Damage,
        Death,
        Revive,
        Avoidance,
        Reflection,
    };

private:
    // 構造体
    struct SpecialMode
    {
        bool specialAttack;
        bool specialShot;
    };


public:

    bool GetAfterimageMove() const { return this->afterimagemove; }

   // Model* GetModel() const { return model; }

    bool Ground();

    State GetStateTuren() const { return this->state; }

    void SetStateTuren(State state)  { this->state = state; }

    // 再生方法
    void  SetUpdateAnim(UpAnim  updateanim) { this->updateanim = updateanim; }

    UpAnim  GetUpdateAnim() const  { return this->updateanim; }

    float GetMoveSpeedAnimation() const { return moveSpeedAnimation; }

private:
    std::shared_ptr<Movement>	movement;
    std::shared_ptr<HP>	hp;
    std::shared_ptr<Transform> transform;

    
    DirectX::XMFLOAT3 velocity = { 0,0,0 };
 
    Model* model = nullptr;

    GamePad        gamePad;

    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 angle = {};
    DirectX::XMFLOAT3 scale = {};

    // 身長
    float height = 1.0f;
    
    

    float          moveSpeed = 5.0f;

    float         turnSpeed = DirectX::XMConvertToRadians(720);

    float          jumpSpeed = 20.0f;
    int                     jumpCount = 0;
    int                     jumpLimit = 2;

    ProjectileManager projectileManager;

    Effect* hitEffect = nullptr;
    Effect* desEffect = nullptr;

    State                   state = State::Idle;
    State                   stated;
    
    float gravity =-1.0f;

    // 着地場所までの距離　 十分な速度で落とす重力の５倍２、３秒後に着地モーションをする。
    float jumpfliptime = gravity * 5;

    float            leftHandRadius = 0.4f;

    bool             attackCollisionFlag = false;
  
    //int   health = 10;

    // 歩いている判定
    bool afterimagemove = false;

    bool blend = false;

    // アップデート再生上半身下半身別
    UpAnim  updateanim ;

    DirectX::XMFLOAT3 moveVec;

    DirectX::XMFLOAT3 frontVec = { 0 ,0,0 };

    // 上半身更新開始位置
    char* bornUpStartPoint;

    // 上半身の何処まで更新するか
    char* bornUpEndPoint = "";

    // 下半身更新終了位置
    char* bornDownerEndPoint = "";

    // 当たり判定半径
    float radius = 0.5f;

    // Hp
    int health = 5;

    // 最大HP
    int maxHealth = 5;



    // アニメーションの時間 
    float currentANimationSeconds = 0.0f;


    // 弾丸生命時間
    float   lifeTimer = 3.0f;

    /////////////////// カメラ関係

    CameraController* cameraControlle;

    DirectX::XMFLOAT3	cameraAngle = DirectX::XMFLOAT3(0, 0, 0);
    float				cameraRollSpeed = DirectX::XMConvertToRadians(90);
    float				cameraMaxPitch = DirectX::XMConvertToRadians(45);
    float				cameraMinPitch = DirectX::XMConvertToRadians(-45);
    float				cameraRange = 100.0f;
    float				characterHeight = 10.0f;

    // 攻撃方法の選択
    int selectCheck = false;

    // 特殊攻撃出るまで
    float specialAttackCharge = 0.0f;
    float specialShotCharge = 0.0f;

    // ステートマシン用
    StateMachine* stateMachine = nullptr;

    // 移動傾き
    float moveSpeedAnimation;

    // 当たり判定無効判定
    bool invalidJudgment = true;
};

// プレイヤーマネージャー
class PlayerManager
{
private:
    PlayerManager() {}
    ~PlayerManager() {};

public:
    // インスタンス取得
    static PlayerManager& Instance()
    {
        static PlayerManager instance;
        return instance;
    }

    // 更新
    void DeleteUpdate();

    // 登録
    void Register(std::shared_ptr<Actor> actor);


    // ステージ数取得
    int GetPlayerCount() const { return static_cast<int>(players.size()); }

    // エネミー取得
    std::shared_ptr<Actor> GetPlayer(int index) { return players.at(index); }

    // 削除
    void Remove(std::shared_ptr<Actor> player);

    void Clear();
private:
    std::vector<std::shared_ptr<Actor>> players;
    // 削除予約
    std::set<std::shared_ptr<Actor>>       removes;
};