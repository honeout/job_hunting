#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

// スライム
class EnemySlime : public Enemy
{
public:
    EnemySlime();
    ~EnemySlime() override;

    //void Start() override;

    // 更新処理
    void Update(float elapsedTime) override;

    // 描画処理
    void Render(const RenderContext& rc, ModelShader* shader) override;

    // デバッグプリミティブ描画　デバッグ用
    void DrawDebugPrimitive();

    // 縄張り設定
    void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

private:
    // ターゲット位置をランダム設定
    void SetRandomTargetPosition();

    // 目標地点へ移動
    void MoveToTarget(float elapsedTime, float speedRate);

    // 徘徊ステートへ遷移
    void TransitionWanderState();

    // 徘徊ステート更新処理
    void UpdateWanderState(float elapsedTime);

    // ノードとプレイヤーの衝突処理
    void CollisitionNodeVsPlayer(const char* nodeName, float nodeRadius);

    // プレイヤー索敵
    bool SearchPlayer();

    // 待機ステートへ遷移
    void TransitionIdleState();

    // 待機ステート更新処理
    void UpdateIdleState(float elapsedTime);

    // 追跡ステートへ遷移
    void TransitionPursuitState();

    // 追跡ステート更新処理
    void UpdatePursuitState(float elapsedTime);

    // 攻撃ステートへ遷移
    void TransitionAttackState();

    // 攻撃ステート更新処理
    void UpdateAttackState(float elapsedTime);

    // 戦闘待機ステートへ遷移
    void TransitionIdleBattleState();

    // 戦闘待機ステート更新処理
    void UpdateIdleBattleState(float elapsedTime);

    // ダメージステートへ遷移
    void TransitionDamageState();

    // ダメージステートへ更新処理
    void UpdateDamageState(float elapsedTime);

    // 死亡ステートへ遷移
    void TransitionDeathState();

    // 死亡ステート更新処理
    void UpdateDeathState(float elapsedTime);

private:
    // ステート
    enum class State
    {
        Wander,
        Idle,
        Pursuit,
        Attack,
        IdleBattle,
        Damage,
        Death
    };

    // アニメーション
    enum Animation
    {
        Anim_IdleNormal,
        Anim_IdleBattle,
        Anim_Attack1,
        Anim_Attack2,
        Anim_WalkFWD,
        Anim_WalkBWD,
        Anim_WalkLeft,
        Anim_WalkRight,
        Anim_RunFWD,
        Anim_SceneSomthinST,
        Anim_SceneSomthinPRT,
        Anim_Taunt,
        Anim_Victory,
        Anim_GetHit,
        Anim_Dizzy,
        Anim_Die,
    };




protected:
    //// 死亡した時に呼ばれる
    //void OnDead() override;

    //// ダメージを受けた時に呼ばれる
    //void OnDamaged() override;

private:
    // モデル情報を確保
    Model* model = nullptr;

    //DirectX::XMFLOAT3 position = {};
    //DirectX::XMFLOAT3 angle = {};

    //std::shared_ptr<Movement>	movement;
    //std::shared_ptr<HP>	hp;

    // 速度
    /*float moveSpeed = 5;*/


    // どのステートで動くか
    State state = State::Wander;
    // ターゲットの位置
    DirectX::XMFLOAT3 targetPosition = { 0,0,0 };
    // 縄張り
    DirectX::XMFLOAT3 territoryOrigin = { 0,0,0 };
    // 縄張り半径
    float territoryRange = 10.0f;
    // 動く速度
    float moveSpeed = 3.0f;
    // 回転速度
    float turnSpeed = DirectX::XMConvertToRadians(360);
    // 追跡時間
    float stateTimer = 0.0f;

    
    // 探す半径
    float searchRange = 5.0f;
    // 攻撃半径
    float attackRange = 1.5f;


    //// Hp
    //int health = 5;

    //// 最大HP
    //int maxHealth = 5;


    //int radius = 5;

};