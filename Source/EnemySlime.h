#pragma once

#include "Graphics/Model.h"
#include "Component.h"
#include "Movement.h"
#include "HP.h"
#include "ModelControll.h"
#include "Transform.h"

#include "StateMachine.h"
#include "StageMain.h"
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

#ifdef _DEBUG
    // GUI描画
    void OnGUI() override;
#endif // _DEBUG

    // デバッグプリミティブ描画　デバッグ用
    void DrawDebugPrimitive();

    // 当たり判定衝撃波
    void CollisionImpactVsPlayer();

    // 当たり判定ルビー
    void CollisionRubyVsPlayer();

    // 当たり判定ルビー横
    void CollisionRubyWidthVsPlayer();

    // 当たり判定宝石と地面
    void CollisionRubyWidthVsOnGraound();

    void InputImpact(DirectX::XMFLOAT3 pos);

    // 宝石飛び出し
    void InputProjectile();

    // 宝石弧のじで飛ぶ
    void InputThrowingRuby(DirectX::XMFLOAT3 target);
    
    // 縄張り設定
    void SetTerritory(const DirectX::XMFLOAT3& origin, float range);

    // ターゲットポジション設定
    void SetTargetPosition(DirectX::XMFLOAT3 position) { targetPosition = position; }

    // ターゲットポジション取得
    DirectX::XMFLOAT3 GetTargetPosition() { return targetPosition; }


    // ステートマシーン取得
    StateMachine* GetStateMachine() { return stateMachine; }



public:
    enum class State
    {
        Wander,
        Idle,
        Pursuit,
        Attack,
        Shot,
        ShotThrowing,
        IdleBattle,
        Damage,
        Death,
    };



    // アニメーション
    enum Animation
    {

        Anim_Walk,
        Anim_Standby,
        Anim_jewelattack,
        Anim_Attack,
        Anim_CloseEye,
        Anim_Shot,
        Anim_Die,
        Anim_Movie
    };

    // アニメーション再生の種類
    enum class UpAnim
    {
        Stop = -1,
        Normal,
        Doble,
        Blend,
        Reverseplayback,

    };
public:
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

    // HP現象時点滅
    void OnHit(float elapsedTime);

//private:
    // ターゲット位置をランダム設定
    void SetRandomTargetPosition();

    // 目標地点へ移動
    void MoveToTarget(float elapsedTime, float speedRate);
    // 目的方向への回転
    void TurnToTarget(float elapsedTime, float speedRate);

    // ジャンプ設定
    void InputJump();

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

    // ポジション取得
    DirectX::XMFLOAT3 GetPosition() { return position; }

    // 半径
    float GetRadius() { return radius; }

    // 近距離攻撃時の当たり判定
    void SetAttackRightFootRange(float attackRightFootRange) { this->attackRightFootRange = attackRightFootRange; }

    // 近距離攻撃時の当たり判定
    float GetAttackRightFootRange() const { return attackRightFootRange; }

    // 再生方法セット
    void  SetUpdateAnim(UpAnim  updateanim) { this->updateanim = updateanim; }
    // 再生方法ゲット
    UpAnim  GetUpdateAnim() const { return this->updateanim; }

    // Todo現在おかしいので使っていないが投げてプレイヤーが跳ね返せる攻撃の一時的地面判定消去
    // 地面判定復活
    void SetPushuThrow(bool pushuThrow) { this->pushuThrow = pushuThrow; }
    // 地面判定復活
    bool GetPushuThrow() const { return pushuThrow; }

    void SetModelDrawCheck(bool modelDrawCheck) {this->modelDrawCheck = modelDrawCheck;}

private:
    // モデル情報を確保
    Model* model = nullptr;
    
    
    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 angle = {};
    DirectX::XMFLOAT3 scale = {};

    std::shared_ptr<Movement>	movement;
    std::shared_ptr<HP>	hp;
    std::shared_ptr<Transform>	transform;



    // どのステートで動くか
    State state = State::Wander;
    // ターゲットの位置
    DirectX::XMFLOAT3 targetPosition = { 0,0,0 };
    // 縄張り
    DirectX::XMFLOAT3 territoryOrigin = { 0,0,0 };

    // アップデート再生上半身下半身別
    UpAnim  updateanim;

    // 上半身更新開始位置
    char* bornUpStartPoint;

    // 下半身更新終了位置
    char* bornDownerEndPoint = "";

    // 縄張り半径
    float territoryRange = 10.0f;
    // 動く速度
    float moveSpeed = 3.0f;
    // 回転速度
    float turnSpeed = DirectX::XMConvertToRadians(360);

    // ジャンプの値
    float jumpSpeed = 0;

    // ジャンプのプラス分
    float jumpSpeedMin = 20.0f;

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
    float radius = 3.0f;

    // 高さ
    float height = 10.0f;

    float territoryarea = 10.0f;

    // ステートマシン用
    StateMachine* stateMachine = nullptr ;

    // 当たり判定無効判定
    bool invalidJudgment = true;

    // playerカウンター用
    bool counterJudgment = false;

    // 近距離攻撃時の当たり判定
    float attackRightFootRange = 0;

    bool pushuThrow = false;

    // model描画チェック
    bool modelDrawCheck = true;

    // ダメージ経過時間
    int damageStateTime = 0;
    int damageStateTimeMax = 15;
    bool damageStateCheck = false;

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