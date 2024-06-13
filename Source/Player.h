#pragma once

#include <DirectXMath.h>
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileManager.h"
#include "Component.h"
#include "Movement.h"
#include "HP.h"
#include "Input\GamePad.h"
#include "CameraController.h"

enum class UpAnim
{
    Normal,
    Doble,

};

// プレイヤー
class Player : public Component
{
public:
    Player();
    ~Player() override;



    // インスタンス取得
    static Player& Instance();


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

    void UpdateTransform();

    // 開始処理
    void Start() override;

    // 更新処理
    void Update(float elapsedTime);

    // デバッグプリミティブ描画　デバッグ用
    void DrawDebugPrimitive();


    // 描画処理
    void Render(const RenderContext& rc, ModelShader* shader);


    // 弾丸と敵の衝突処理
    void CollisionProjectilesVsEnemies();

    // プレイヤーとエネミーとの衝突処理
    void CollisionPlayerVsEnemies();

    // デバッグ用GUI描画
    void DrawDebugGUI();



protected:
    //// 着地した時に呼ばれる
    //void OnLanding() override;

    //// 死亡した時に呼ばれる
    //void OnDead() override;

    //// ダメージを受けた時に呼ばれる
    //void OnDamaged() override;

private:

    // カメラ操作
    void CameraControl(float elapsedTime);

    // スティック入力値から移動ベクトルを取得 進行ベクトルを取る進むべき方向
    DirectX::XMFLOAT3 GetMoveVec(float elapsedTime) const;

    //// キャラクター操作
    void CharacterControl(float elapsedTime);


    // 移動入力処理
    bool InputMove(float elapsedTime);
 


    // プレイヤーとエネミーとの衝突処理
    //void CollisionPlayerVsEnemies();

    // ノードとエネミーの衝突処理
    void CollisionNodeVsEnemies(const char* nodeName, float nodeRadius);

    // ジャンプ入力処理
    bool InputJump();

    // 弾丸入力
    bool InputProjectile();

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

private:
    // ステート
    enum class State
    {
        Idle,
        Move,
        Jump,
        Land,
        JumpFlip,
        Attack,
        Damage,
        Death,
        Revive
    };

    public:

    bool GetAfterimageMove() const { return this->afterimagemove; }

   // Model* GetModel() const { return model; }



private:
    std::shared_ptr<Movement>	movement;
    std::shared_ptr<HP>	hp;

    
    DirectX::XMFLOAT3 velocity = { 0,0,0 };
 
   Model* model = nullptr;

    GamePad        gamePad;

    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 angle = {};

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
    
    float gravity = 1;

    // 着地場所までの距離　 十分な速度で落とす重力の５倍２、３秒後に着地モーションをする。
    int jumpfliptime = gravity * 5;

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
    char* bornDownerEndPoint;

    // 当たり判定半径
    float radius = 0.5f;

    // Hp
    int health = 5;

    // 最大HP
    int maxHealth = 5;



    // アニメーションの時間 
    float currentANimationSeconds = 0.0f;

    /////////////////// カメラ関係

    CameraController* cameraControlle;

    DirectX::XMFLOAT3	cameraAngle = DirectX::XMFLOAT3(0, 0, 0);
    float				cameraRollSpeed = DirectX::XMConvertToRadians(90);
    float				cameraMaxPitch = DirectX::XMConvertToRadians(45);
    float				cameraMinPitch = DirectX::XMConvertToRadians(-45);
    float				cameraRange = 100.0f;
    float				characterHeight = 10.0f;


};