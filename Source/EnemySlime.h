#pragma once

#include "Graphics/Model.h"
//#include "Enemy.h"
#include "Component.h"
#include "Movement.h"
#include "HP.h"
#include "ModelControll.h"
#include "Transform.h"

#include "StateMachine.h"
// ステート




// スライム
class EnemySlime : public Component
{
public:
    EnemySlime() {};
    ~EnemySlime() override;

    // 名前取得
    const char* GetName() const override { return "EnemySlime"; }

    void Start() override;

    //// 更新処理
    void Update(float elapsedTime) override;

    // 描画処理
    void Render(RenderContext& rc, ModelShader& shader) override;

    // シャドウマップ処理
    void RenderShadowmap(RenderContext& rc) override;

    //// 描画処理
    //void Render(const RenderContext& rc, ModelShader* shader) override;

    // デバッグプリミティブ描画　デバッグ用
    void DrawDebugPrimitive();

    // 縄張り設定
    void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

    // ターゲットポジション設定
    void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }

    // ターゲットポジション取得
    DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }


    // ステートマシーン取得
    StateMachine* GetStateMachine() { return stateMachine; }


    // アニメーション　ゲット
    int GetAnimationStateNormal() { return Animation::Anim_IdleNormal; }
    int GetAnimationStateAttack() { return Animation::Anim_Attack1; }
    int GetAnimationStateWalk() { return Animation::Anim_WalkFWD; }
    int GetAnimationStateHit() { return Animation::Anim_GetHit; }
    int GetAnimationStateDie() { return Animation::Anim_Die; }

    // ステートタイマー設定
    void SetStateTimer(float timer) {
        stateTimer = timer;
    }
    // ステートタイマー取得
    float GetStateTimer() { return stateTimer; }

    // ゲット当たり判定無効
    bool GetInvalidJudgment() const { return invalidJudgment; }
    // セット当たり判定無効
    void SetInvalidJudgment(bool Judgment) { invalidJudgment = Judgment; }

    // ゲットカウンター
    bool GetCounterJudgment() const { return counterJudgment; }
    // セットカウンター
    void SetCounterJudgment(bool counterJudgment) { this->counterJudgment = counterJudgment; }

    //  UI操作
    void UiControlle(float elapsedTime);

//private:
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

    // 破棄
    void Destroy();

    // ターゲットポジション取得
    //DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }

    // ポジション取得
    DirectX::XMFLOAT3 GetPosition() { return position; }

    // 半径
    float GetRadius() { return radius; }

public:
        enum class State
        {
            Wander,
            Idle,
            Pursuit,
            Attack,
            Damage,
            Death,
            IdleBattle,
        };

private:

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

    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 angle = {};
    DirectX::XMFLOAT3 scale = {};

    std::shared_ptr<Movement>	movement;
    std::shared_ptr<HP>	hp;
    std::shared_ptr<Transform>	transform;

    //std::unique_ptr<Model> model;
    // Model* model;

    // 速度
    //float moveSpeed = 5;


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


    // Hp
    int health = 50;

    // 最大HP
    int maxHealth = 50;

    // 半径
    float radius = 0.5f;

    // 高さ
    float height = 1.0f;

    float territoryarea = 10.0f;

    // ステートマシン用
    StateMachine* stateMachine = nullptr ;

    // 当たり判定無効判定
    bool invalidJudgment = true;

    // playerカウンター用
    bool counterJudgment = false;

    // ステート切り替え時間管理
   // float				stateTimer = 0.0f;

};

// エネミーマネージャー
class EnemyManager
{
private:
    EnemyManager() {}
    ~EnemyManager() {};

public:
    // インスタンス取得
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    // 描画
    //void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

    // 更新処理
    void DeleteUpdate(float elapsedTime);

    // 登録
    void Register(std::shared_ptr<Actor> actor);

    void Clear();

    // ステージ数取得
    int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

    // エネミー取得
    std::shared_ptr<Actor> GetEnemy(int index) { return enemies.at(index); }

    void Remove(std::shared_ptr<Actor> projectile);

private:
    std::vector<std::shared_ptr<Actor>> enemies;

    // 削除予約
    std::set<std::shared_ptr<Actor>>       removes;


};