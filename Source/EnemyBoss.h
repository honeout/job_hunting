#pragma once

#include "Graphics/Model.h"
#include "Component.h"
#include "Audio/AudioSource.h"
#include "Movement.h"
#include "HP.h"
#include "ModelControll.h"
#include "Transform.h"

#include "StateMachine.h"
#include "StageMain.h"
#include "Effect.h"
// ステート




// 宝石ボス
class EnemyBoss : public Component
{
public:
    EnemyBoss() {}
    ~EnemyBoss() override;

    // 名前取得
    const char* GetName() const override { return "EnemyBoss"; }

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


    // SE再生 切り裂き
    void InputSlashSe();
    void InputStopSlashSe();

    // SE再生 ジャンプ
    void InputJampSe();
    void InputStopJampSe();


    // SE再生 ダッシュ
    void InputDashSe();
    // SE再生 ダッシュ
    void InputStopDashSe();


    // SE叫び声
    void InputAwakeSe();

    // SE衝撃波
    void InputImpactSe();

    // SEダメージ音
    void InputDamageSe();

    // SEチャージオン
    void InputChargeSe();

    // 当たり判定衝撃波
    void CollisionImpactVsPlayer();

    // 当たり判定ルビー
    void CollisionRubyVsPlayer();

    // 当たり判定ルビー横
    void CollisionRubyWidthVsPlayer();

    // 当たり判定宝石と地面
    void CollisionRubyWidthVsOnGraound();

    // 当たり判定ボスとプレイヤー
    bool CollisionPlayerWithRushEnemy();

    // 衝撃波判定
    void CollisionInpact();

    // 敵覚醒管理
    void ManageAwakeTime(float elapsedTime);

    // 敵覚醒時間初期化
    void ResetAwakeTime();

    // 当たり判定右端
    void DetectHitByBodyPart(DirectX::XMFLOAT3 partBodyPosition, int applyDamage);

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
    //StateMachine* GetStateMachine() { return stateMachine; }
    StateMachine*GetStateMachine() { return stateMachine.get(); }


    // 外部で初期化するための処置
    void StateMachineCreate() { stateMachine = std::make_unique<StateMachine>(); }



public:
    enum class ClearState
    {
        Death,
    };

    enum class State
    {
        Wander,
        Idle,
        Pursuit,
        Jump,
        Attack,
        Shot,
        ShotThrowing,
        IdleBattle,
        Damage,
        Death,
        AwakeStart,
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

    enum class AttackMode
    {
        AssaultAttack,
        JumpStompAttack,
        DushAttack,
    };

    enum class ExitRoot
    {
        ExitJamp,
        Confusion
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
    
    // クリア確認
    bool GetClearCheck() { return clearCheck; }
    void SetClearCheck(bool clearCheck) { this->clearCheck = clearCheck; }

    void SetMoveCheck(bool moveCheck) 
    { this->moveCheck = moveCheck; }

    // 探す範囲
    float GetSearchRange() { return searchRange; }

    // ブラーをかける
    bool GetBlurCheck() { return blurCheck; }

    void SetBlurCheck(bool blurCheck) {  this->blurCheck = blurCheck; }

    // 覚醒有無
    bool GetIsEnemyAwakened() { return isEnemyAwakened; }

    // シェイク時間 パワー
    void StartDamageShake();

private:
    // モデル情報を確保
    Model* model = nullptr;
    
    
    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 angle = {};
    DirectX::XMFLOAT3 scale = {};

    std::weak_ptr<Movement>	movement;
    std::weak_ptr<HP>	hp;
    std::weak_ptr<Transform>	transform;





    // 覚醒有無
    bool isEnemyAwakened = false;
    // 覚醒時間
    float enemyAwakeningDuration = 0.0f;
    float enemyAwakeningDurationMax = 30;
    float enemyAwakeningDurationEnd = 0.0f;
    

    // プレイヤーに与ダメエフェ
    std::unique_ptr<Effect> moveAttackEffect;

    std::unique_ptr<Effect> inpactEffect;

    std::unique_ptr<Effect> awakeEffect;

    //// 衝撃波SE
    //std::unique_ptr<AudioSource> impactSe;
    //std::unique_ptr<AudioSource> moveAttackSe;

    // どのステートで動くか
    State state = State::Wander;
    // ターゲットの位置
    DirectX::XMFLOAT3 targetPosition = { 0,0,0 };
    // 縄張り
    DirectX::XMFLOAT3 territoryOrigin = { 0,0,0 };

    // アップデート再生上半身下半身別
    UpAnim  updateanim = UpAnim::Normal;
    
    //// 上半身更新開始位置
    //char* bornUpStartPoint;

    //// 下半身更新終了位置
    //char* bornDownerEndPoint = "";


    // 上半身スタート再生開始場所
    char* bornUpStartPoint = "";

    // 上半身エンド再生停止場所
    char* bornUpEndPoint = "";

    // 下半身スタート再生開始場所
    char* bornDownerStartPoint = "";

    // 下半身エンド再生停止場所
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
    float attackRange = 5.5f;


    // Hp
    int health = 50;

    // 最大HP
    int maxHealth = 50;

    // 半径
    float radius = 1.0f;

    // 高さ
    float height = 9.0f;

    float territoryarea = 10.0f;

    // ステートマシン用
    std::unique_ptr <StateMachine> stateMachine;
    //StateMachine* stateMachine;

    // 当たり判定無効判定
    bool invalidJudgment = true;

    // playerカウンター用
    bool counterJudgment = false;

    // 近距離攻撃時の当たり判定
    float attackRightFootRange = 0;

    // 衝撃波起こる範囲外側
    float radiusInpactOutSide = 0.3f;

    // 衝撃波起こる範囲内側
    float radiusInpactInSide = 0.3f;

    // 衝撃波高さ
    float radiusInpactHeight = 0.0f;

    // 衝撃波有無
    bool IsInpact = false;

    bool pushuThrow = false;

    // model描画チェック
    bool modelDrawCheck = true;



    // ダメージ経過時間
    int damageStateTime = 0;
    int damageStateTimeMax = 15;
    bool damageStateCheck = false;

    // クリア確認
    bool clearCheck = false;

    DirectX::XMFLOAT2 colorGB = { 1,1 };


    // 動作チェック
    bool moveCheck = true;

    // ブラーかける
    bool blurCheck = false;

    // uiCount最大値
    int uiCountMax = 3;


    // ダメージ
    int applyDamageSlash = 5;
    int applyDamageJamp = 4;
    int applyDamageStamp = 6;
    int applyDamageImpact = 5;
    int applyDamageDush = 7;


    // シェイクダメージ演出
    float shakePower = 0.6f;
    float shakeTimer = 0.2f;

    // シェイク用
    RadialBlurData damageDistortion;
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