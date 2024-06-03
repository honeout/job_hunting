#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Player.h"
#include "AfterimageManager.h"
//#include "Collision.h"
//#include "ProjectileManager.h"


// プレイヤー
class PlayerAfterimage : public Afterimage
{
public:
    PlayerAfterimage();
    ~PlayerAfterimage() override;

    //// インスタンス取得
    //static PlayerAfterimage& Instance();


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




    // 更新処理
    void Update(float elapsedTime) override;

    // デバッグプリミティブ描画　デバッグ用
    void DrawDebugPrimitive() override;


    // 描画処理
    void Render(RenderContext& rc, ModelShader* shade) override;


    // 弾丸と敵の衝突処理
    void CollisionProjectilesVsEnemies();

    //// プレイヤーとエネミーとの衝突処理
    //void CollisionPlayerVsEnemies();

    // デバッグ用GUI描画
    void DrawDebugGUI();
protected:
    // 着地した時に呼ばれる
    void OnLanding() override;

    // 死亡した時に呼ばれる
    void OnDead() override;

    // ダメージを受けた時に呼ばれる
    void OnDamaged() override;

private:
    // スティック入力値から移動ベクトルを取得 進行ベクトルを取る進むべき方向
    DirectX::XMFLOAT3 GetMoveVec() const;


    // 移動入力処理
    bool InputMove(float elapsedTime);



    // プレイヤーとエネミーとの衝突処理
    void CollisionPlayerVsEnemies();

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
public:

    Model* GetModel() const { return model; }


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
        void SetModel(Model* model) { this->model = model; }



private:
    Model* model = nullptr;
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

    // 着地場所までの距離　 十分な速度で落とす重力の５倍２、３秒後に着地モーションをする。
    int jumpfliptime = grabity * 5;

    float            leftHandRadius = 0.4f;

    bool             attackCollisionFlag = false;

    int   healse = 10;


    bool             first = false;

    float            timeremove = 0.5f;
};
