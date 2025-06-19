#pragma once
#include <DirectXMath.h>
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Collision.h"
#include "ProjectileManager.h"
#include "Audio/AudioSource.h"
#include "Component.h"
#include "Collision.h"
#include "Movement.h"
#include "HP.h"
#include "Mp.h"
#include "Input\GamePad.h"
#include "CameraController.h"
#include "Effect.h"
#include "ModelControll.h"
#include "Transform.h"
#include "StateMachine.h"
#include "PostprocessingRenderer.h"
#include "EnemyBoss.h"
#include <stack>

// 定数定義
namespace PlayerConfig
{
    // 上半身スタート再生開始場所
    constexpr const char* bornUpStartPoint = "mixamorig:Hips";
    // 上半身エンド再生停止場所
    constexpr const char* bornUpEndPoint = "mixamorig:LeftUpLeg";
    // 下半身スタート再生開始場所
    constexpr const char* bornDownerStartPoint = "mixamorig:LeftUpLeg";
    // 下半身エンド再生停止場所
    constexpr const char* bornDownerEndPoint = "mixamorig:RightToe_End";

    // 身長
    constexpr float height = 1.0f;

    // 当たり判定半径
    constexpr float radius = 1.0f;

    // Hp
    constexpr int health = 50;

    // 歩く速度
    constexpr float   moveSpeed = 5.0f;

    // ジャンプ速度
    constexpr float   jumpSpeed = 13.0f;

    // ジャンプの最大回数
    constexpr int     jumpLimit = 2;

    // 回転速度
    constexpr float   turnSpeed = DirectX::XMConvertToRadians(720);

    // 重力
    constexpr float gravity = -0.5f;

    // 最大HP
    constexpr int maxHealth = 50;

    // Mp
    constexpr int magicPoint = 50;

    // 魔法チャージ加算
    constexpr float commandChargeAdd = 0.065f;

    // コマンド余白文
    constexpr float offset = 20.0f;

    // 連続攻撃回数
    constexpr int attackNumberSaveMax = 4;

    // 空中移動制限限界
    constexpr int areAttackStateEnd = 0;

    // 空中移動限界最大
    constexpr int areAttackStateMax = 3;

    // 経過時間関数の最低値
    constexpr float timeElapsedMin = 0.0f;

    // 経過時間関数の最大値
    constexpr float timeElapsedHintMax = 1.0f;

    // 特殊技チャージゲージ　斬撃
    constexpr float specialAttackChargeSlashValue = 0.1f;
    // 特殊技チャージゲージ　魔法
    constexpr float specialAttackChargeMagicValue = 0.2f;
    // 特殊技チャージゲージ　初期値
    constexpr float specialAttackChargeMin = 0.0f;
    // 特殊技チャージゲージ　最大値　共有
    constexpr float specialAttackChargeMax = 1.5f;

    // 特殊技最大値 それぞれの技のチャージ最大値
    constexpr int energyChargeMax = 4;
    // 特殊技最大値 それぞれの技のチャージ初期化
    constexpr int energyChargeMin = 0;

    // コマンド特殊技選択最低値　
    constexpr int spCmdMoveLimitMin = 0;
    // コマンド特殊技選択最大値
    constexpr int spCmdMoveLimitMax = 1;
};

// プレイヤー
class Player : public Component
{
public:
    Player() {};
    ~Player() override;

    // 名前取得
    const char* GetName() const override { return "Player"; }
    // 開始処理
    void Start() override;
    // 更新処理
    void Update(float elapsedTime) override;
    // デバッグプリミティブ描画　デバッグ用
    void DrawDebugPrimitive();
#ifdef _DEBUG
    // GUI描画
    void OnGUI() override;
#endif // _DEBUG
    // 描画処理
    void Render(RenderContext& rc, ModelShader& shader) override;
    // シャドウマップ処理
    void RenderShadowmap(RenderContext& rc) override;

    // コンポーネント初期化
    void InitComponents();

    // エフェクト初期化
    void InitEffects();

    // ステータス初期化
    void InitStats();

    // コマンド初期化
    void InitCommands();

    // ステート更新まとめ
    void UpdateStateMachine(float elapsedTime);

    // 入力受付と行動への変換
    void HandleInput(float elapsedTime);

    // プレイヤー状態制御
    void UpdateStatus(float elapsedTime);

    // ヒットエフェクト等の移動更新用
    void UpdateEffects(float elapsedTime);

    // 当たり判定処理
    void HandleCollisions();

    // アニメーションの再生や状態切り替え
    void UpdateAnimation(float elapsedTime);
 
    // SE再生
    void InputSe(AudioParam param);
    // SE再生同じ
    void PlaySe(const std::string& filename);

    // SE再生ループ
    void PlayLoopSe(const std::string& filename);

    // se停止
    void StopSe(const std::string& filename);

    // カメラ切り替え処理
    void UpdateCameraState(float elapsedTime);

    // 当たり判定パーツ事
    bool CheckAllPartsCollision(DirectX::XMFLOAT3 pos, float rudius);

    // 魔法の種類チェック炎
    bool CheckMagicFire(std::shared_ptr<Actor> projectile);

    // 魔法の種類チェック氷
    bool CheckMagicIce(std::shared_ptr<Actor> projectile);

    // 魔法火と敵との衝突判定
    void CollisionMagicFire();

    // 魔法雷と敵との衝突判定
    void CollisionMagicSunder();

    // 魔法氷と敵との衝突判定
    void CollisionMagicIce();

    // プレイヤーとエネミーとの衝突処理
    void CollisionPlayerVsEnemies();
    // ボーンと攻撃の衝突判定
    void CollisionBornVsProjectile(const char* bornname);
    // デバッグ用GUI描画
    void DrawDebugGUI();
    // 破棄
    void Destroy();
    // ソードトレイル
    void UpdateSwordeTraile();
    // hpピンチ
    void PinchMode(float elapsedTime);
public:
    // スティック入力値から移動ベクトルを取得 進行ベクトルを取る進むべき方向
    DirectX::XMFLOAT3 GetMoveVec(float elapsedTime);

    DirectX::XMFLOAT3 GetMagicMoveVec(float elapsedTime);

    // 移動入力処理
    bool InputMove();

    // ロックオン入力
    bool InputRockOn();

    // ロックオンのUIを出す
    void RockOnUI(ID3D11DeviceContext* dc,
        const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection);

    // 攻撃方法選択
    bool InputSelectCheck();

    // 魔法選択
    bool InputSelectMagicCheck();

    // ショートカットキー
    bool InputShortCutkeyMagic();

    // 魔法選択UI解除
    void RemoveUIMagic();

    // 特殊攻撃選択
    bool InputSpecialAttackCharge();

    // 特殊技順番交代
    void InputSpecialAttackChange();

    // UIの演出
    void SpecialPlayUlEffect(float elapsedTime);

    bool GetSpecialAttackTime() const { return specialAttackTime; }
    void SetSpecialAttackTime(bool specialAttackTime) { this->specialAttackTime = specialAttackTime; }

    // ノードとエネミーの衝突処理
    bool CollisionNodeVsEnemies();

    // ノードと弾丸の衝突処理
    void CollisionNodeVsEnemiesCounter(const char* nodeName, float nodeRadius);

    // ジャンプ入力処理
    bool InputJump();

    // 回避orカウンター入力
    bool InputAvoidance();

    // 攻撃入力
    bool InputAttack();

    // 魔法入力
    bool InputMagick();

    // 魔法入力
    bool InputMagicframe();
    bool InputMagicIce();
    bool InputMagicLightning();
    bool InputMagicHealing();

    // 魔法を撃つ
    void PushMagicFrame(DirectX::XMFLOAT3 angle);
    void PushMagicIce(DirectX::XMFLOAT3 angle);

    void InputSpecialMagicframe();

    void AttackCheckUI();

    DirectX::XMFLOAT3 GetForwerd(DirectX::XMFLOAT3 angle);

    StateMachine* GetStateMachine() { return stateMachine.get(); }
    // 外部で初期化するための処置
    void StateMachineCreate() { stateMachine = std::make_unique<StateMachine>(); }

    // 攻撃方法変更
    void SetSelectCheck(int selectCheck) { this->selectCheck = selectCheck; }
    // 攻撃方法変更
    int GetSelectCheck() const { return selectCheck; }

    // 魔法変更
    void SetSelectMagicCheck(int selectMagicCheck) { this->selectMagicCheck = selectMagicCheck; }
    // 魔法変更
    int GetSelectMagicCheck() const { return selectMagicCheck; }

    // 魔法選択
    void SetMagicAction(bool magicAction) { this->magicAction = magicAction; }

    // 魔法選択
    bool GetMagicAction() { return magicAction; }

    // 特殊魔法発動ため
    void SetSpecialAttackCharge(float specialAttackCharge) { this->specialAttackCharge = specialAttackCharge; }

    // 特殊アクション発動ため
    float GetSpecialAttackCharge() const { return specialAttackCharge; }

    // 攻撃開始
    void SetAttackCollisionFlag(bool attackCollisionFlag) { this->attackCollisionFlag = attackCollisionFlag; }

    // 攻撃開始
    bool GetAttackCollisionFlag() const { return attackCollisionFlag; }

    // 左手の当たり判定
    float GetLeftHandRadius() const { return leftHandRadius; }

    // ゲット当たり判定無効
    bool GetInvalidJudgment() const { return invalidJudgment; }
    // セット当たり判定無効
    void SetInvalidJudgment(bool invalidJudgment) { this->invalidJudgment = invalidJudgment; }

    // 自分の当たり判定有無
    void DmageInvalidJudment(bool invalidJudgment);

    //  UI操作
    void UiControlleGauge(float elapsedTime);

    // ダメージ判定
    void SpecialApplyDamageInRadius();

    // エネミー接触判定
    bool GetIsEnemyHit() { return isEnemyHit; }
    // エネミー接触判定上半身
    bool GetIsEnemyHitBody() { return isEnemyHitBody; }
public:
    // ゲームステート
    enum class State
    {
        // ステート入れるのは後ろから
        Idle,
        Move,
        Jump,
        Land,
        JumpFlip,
        QuickJab,
        SideCut,
        CycloneStrike,
        SpecialAttack,
        Magic,
        SpecialMagic,
        Damage,
        Death,
        Revive,
        Avoidance,
        Reflection,
    };

    // タイトルステート
    enum class StateTitle
    {
        // ステート入れるのは後ろから
        Idle,
        Push,
    };

    // クリアステート
    enum class StateClear
    {
        // ステート入れるのは後ろから
        Idle,
    };

    // ゲームオーバーステート
    enum class StateOver
    {
        // ステート入れるのは後ろから
        Idle,
        Revive,

    };

    // カメラステート
    enum class CameraState
    {
        Normal,
        NotLockOn,
        LockOn,
        AttackLock
    };

    // アニメ
    enum Animation
    {
        Anim_Tpose,
        Anim_Idle,
        Anim_Jump,
        Anim_Slash,
        Anim_SlashBeside,
        Anim_Deth,
        Anim_Pain,
        Anim_Counter,
        Anim_Running,
        Anim_SlashThree,
        Anim_Dush,
        Anim_Magic,
        Anim_MagicSeconde,
        Anim_SpecialAttack,
        Anim_Land,
        Anim_Roll,
        Anim_MagicArch
    };

    // アニメーション再生方法
    enum class UpAnim
    {
        Stop = -1,
        Normal,
        Doble,
        Blend,
        Reverseplayback,
    };


    enum class CommandAttack
    {
        Attack,
        Magic,
        Special
    };

    enum class CommandMagic
    {
        Normal = -2,
        Fire = 0,
        Thander,
        Ice,
        Heale
    };

    enum class SpecialAttackType
    {
        Normal = -1,
        Attack,
        MagicFire,
        MagicThander,
        MagicIce,
    };

    // エネミーに与える
    enum AttackType {

    };

public:
    bool GetAfterimageMove() const { return this->afterimagemove; }

    bool Ground();

    State GetStateTuren() const { return this->state; }

    void SetStateTuren(State state) { this->state = state; }

    // 再生方法
    void  SetUpdateAnim(UpAnim  updateanim) { this->updateanim = updateanim; }

    UpAnim  GetUpdateAnim() const { return this->updateanim; }

    float GetMoveSpeedAnimation() const { return moveSpeedAnimation; }

    // UI揺らし状態
    void SetShakeMode(bool shakeMode) { this->shakeMode = shakeMode; }

    // 回転確認
    void SetAngleCheck(bool angleCheck) { this->angleCheck = angleCheck; }

    void SetLockOnState(CameraState lockonState) { this->lockonState = lockonState; }

    CameraState GetLockOnState() const { return lockonState; }

    // ロックオン入力確認
    void SetRockCheck(bool rockCheck) { this->rockCheck = rockCheck; }
    bool GetRockCheck() { return this->rockCheck; }

    void SetFreeCameraCheck(bool freeCameraCheck) { this->freeCameraCheck = freeCameraCheck; }

    // 色
    void SetColorGradingData(ColorGradingData colorGradingData) { this->colorGradingData = colorGradingData; }

    // fashu
    void SetFlashOn(bool flashOn) { this->flashOn = flashOn; }
    bool GetFlashOn() { return flashOn; }

    void SetHitCheck(bool hitCheck) { this->hitCheck = hitCheck; }
    bool GetHitCheck() { return hitCheck; }

    void SetAttackNumberSave(int attackNumberSave) { this->attackNumberSave = attackNumberSave; }

    // UIがないときはfalse
    void SetUiControlleCheck(bool uiControlleCheck) { this->uiControlleCheck = uiControlleCheck; }

    // ステート変換
    void SetEndState(bool endState) { this->endState = endState; }
    bool GetEndState() { return endState; }

    // 必殺技中のロックオフ
    void SetSpecialRockOff(bool specialRockOff) { this->specialRockOff = specialRockOff; }

    // 空中行動許可確認
    void SetAreAttackState(int areAttackState) { this->areAttackState = areAttackState; }

    void AreAttackDecreaseAmount();

    // 経過時間
    bool UpdateElapsedTime(float timeMax, float elapsedTime);

    // 描画するかどうか
    void SetPlayeDrawCheck(int isPlayerDrawCheck) { this->isPlayerDrawCheck = isPlayerDrawCheck; }

    // Ui魔法チャージ動作開始
    void StartMagicUiCharge(DirectX::XMFLOAT2& pos, float& gaugeSizeMax);
    // Ui魔法チャージ動作発射
    void StartMagicUiFire();

    // デバッグ用
    bool GetDebugCameraTime() { return debugCameraTime; }
private:



    // 構造体
    struct SpecialAttack
    {
        int id;
        bool hasSkill;
    };
private:
    std::weak_ptr<Movement>	movement;
    std::weak_ptr<HP> hp;
    std::weak_ptr<Collision> collision;
    std::weak_ptr<Mp> mp;
    std::weak_ptr<Transform> transform;
    std::weak_ptr<ModelControll> modelControll;

    // 位置
    DirectX::XMFLOAT3 position = {};
    // 角度
    DirectX::XMFLOAT3 angle = {};
    // 大きさ
    DirectX::XMFLOAT3 scale = {};

    // 速度
    DirectX::XMFLOAT3 velocity = { 0,0,0 };

    // 空中行動許可
    bool isAreAttack = false;

    // 空中行動許可間隔
    int areAttackState = 0;

    // 経過時間を測る
    float timeElapsed = 0.0f;
    float timeElapsedHintMax = 0.0f;

    
    // UI処理を出すかチェック
    bool             uiControlleCheck = false;

    // 斬撃エフェクト　スケール
    float slashScale = 0.4f;

    // 当たり判定半径
    float radius;
    // 当たり判定高さ
    float height;

    // 移動速度
    float          moveSpeed = 0.0f;

    // 弾丸マネージャー
    ProjectileManager projectileManager;

    // エフェクト
    std::unique_ptr<Effect> hitEffect;
    std::unique_ptr<Effect> magicFireHitEffect;
    std::unique_ptr<Effect> magicThenderHitEffect;
    std::unique_ptr<Effect> magicIceHitEffect;
    std::unique_ptr<Effect> ImpactEffect;
    std::unique_ptr<Effect> desEffect;
    std::unique_ptr<Effect> hitSlash;
    std::unique_ptr<Effect> lightningAttack;
    std::unique_ptr<Effect> hitFire;
    std::unique_ptr<Effect> hitThander;
    std::unique_ptr<Effect> hitIce;
    // 足もとに竜巻
    std::unique_ptr<Effect> areWork;
    std::unique_ptr<Effect> effectFocus2D;


    State                   state = State::Idle;
    State                   stated = State::Idle;

    CameraState            cameraState = CameraState::Normal;
    CameraState			lockonState = CameraState::NotLockOn;

    DirectX::XMFLOAT3 lockonCharactor = { 0.0f,0.0f,0.0f };

    float				lockonTargetChangeTime = 0;
    float				lockonTargetChangeTimeMax = 8;

    // 位置情報のランダム
    DirectX::XMFLOAT3 randomPosMax = {1.5f,3.0f,1.5f};
    DirectX::XMFLOAT3 randomPosMin = {1.0f,0.1f,1.0f};

    float            leftHandRadius = 1.8f;

    bool             attackCollisionFlag = false;

    // 歩いている判定
    bool afterimagemove = false;

    bool blend = false;

    // アップデート再生種類
    UpAnim  updateanim = UpAnim::Normal;

    DirectX::XMFLOAT3 moveVec = { 0.0f,0.0f,0.0f };

    DirectX::XMFLOAT3 frontVec = { 0 ,0,0 };

    // アニメーションの時間 
    float currentANimationSeconds = 0.0f;

    CameraController* cameraControlle = nullptr;

    DirectX::XMFLOAT3	cameraAngle = DirectX::XMFLOAT3(0, 0, 0);
    float				cameraRollSpeed = DirectX::XMConvertToRadians(90);
    float				cameraMaxPitch = DirectX::XMConvertToRadians(45);
    float				cameraMinPitch = DirectX::XMConvertToRadians(-45);
    float				cameraRange = 100.0f;
    float				characterHeight = 10.0f;

    float               flame = 60.0f;
    float               isInputEmpty = 0.0f;

    bool                isDrawUi = true;
    bool                isDrawUiEmpth = false;

    DirectX::XMFLOAT2   scereenPositionOffset = { 34.0f,25.0f };

    float               slashAttackInvincibleTime = 0.5f;
    float               magicAttackInvincibleTime = 0.1f;

    // 攻撃方法の選択
    int selectCheck = 0;

    // 魔法選択
    int selectMagicCheck = 0;

    // 特殊攻撃出るまで
    float specialAttackCharge = 0.0f;
    // 特殊技チャージゲージ　初期値
    float specialAttackChargeMin = 0.0f;
    // 特殊技チャージゲージ　魔法
    float specialAttackChargeMagicValue = 0.0f;


    // 攻撃ヒット回数
    int attackNumberSave;
    int attackNumberSaveMax;

    // 特殊技のチャージ
    int attackEnergyCharge = 0;
    int fireEnergyCharge = 0;
    int thanderEnergyCharge = 0;
    int iceEnergyCharge = 0;
    int energyChargeMax = 0;
    int energyChargeMin = 0;



    // ステートマシン用
    std::unique_ptr<StateMachine> stateMachine;

    // 移動傾き
    float moveSpeedAnimation = 0.0f;

    // 移動魔法の
    float magicMoveLength = 1000.0f;

    // 当たり判定無効判定
    bool invalidJudgment = true;

    // 特殊攻撃
    static const int specialAttackNumMax = 4;

    int specialAttackNum = 0;

    // 特殊攻撃　所持
    std::vector<SpecialAttack> specialAttack;

    bool isSkillHave = false;

    // 特殊攻撃のサイズが最低値ならやめる。
    int specialAttackNumMin = 0;

    // 特殊アクション確認
    bool specialAttackTime = false;

    // 魔法攻撃を打つショートカット用
    bool magicAction = false;

    // 特殊技コマンド開く
    bool specialAction = false;

    // 特殊技たまった時の反応
    bool isUiSpecilDrawCheck = false;

    // 揺れ
    bool shakeMode = false;

    // 回転確認
    bool angleCheck = false;

    // 角度確認
    float dotfake = 0.0f;

    // ロックオンチェック
    bool rockCheck = false;

    // ボタン押したかのチェック
    bool buttonRock = false;

    // 魔法の射程当たる距離
    float magicRangeLength = 470;

    // 魔法消費量
    int magicConsumption = 5;

    // 描画の色をどうするか
    DirectX::XMFLOAT4 mpUiColor = {1,1,1,1};

    // スペキュラー無効
    int isSpecular = 0;
    // 影オン
    int isRimRightning = 1;
    // modelオンオフ
    int StencilRef = 1;

    // ダメージ量
    int applyDamageNormal = 3;
    int applyDamageFire = 3;
    int applyDamageThander = 2;
    int applyDamageIce = 4;
    // マジックダメージ
    int applyDamageMagic = 0;
    // 必殺技ダメージ量
    int applyDamageSpecial = 2;

    int hitMortion = 0;
    int hitMortionMax = 180;

    // デバッグ用必殺技のカメラアングルを確認
    bool specialCameraRock = false;

    // デバッグ用カメラシェイク用
    float shakeTimer = 0.5f;
    float shakePower = 0.3f;

    // ロックオン以外の狙いをプレイヤー中心かどうか
    bool freeCameraCheck = true;
    // デバッグ
    bool angleCameraCheck = false;

    // 画面の色
    ColorGradingData       colorGradingData;
    ColorGradingData       colorGradingPostData;

    bool flashOn = false;

    bool hitCheck = false;

    static const int MAX_POLYGON = 32;
    DirectX::XMFLOAT3					trailPositions[2][MAX_POLYGON];

    int stateSize = 5;

    // uiCount最大値
    int uiCountMax = 5;

    // 終了タイミング
    bool endState = false;

    float uiShrinkValueMax = 250; 
    float uiShrinkValueMin = 100;

    // メニューのオンオフ
    bool isMenue = false;
    bool isMenueOn = true;
    bool isMenueOf = false;

    VignetteData					vignetteData;

    bool   checkVignette = false;

    bool testcolor = false;
    // 回復値
    int healing = 30;

    // 必殺技中ロック外す
    bool specialRockOff = false;

    // ターゲットの一定以上高い
    float               topHeight = 3.0f;
    float               minHeight = 2.0f;

    // shortecut pos
    DirectX::XMFLOAT2 fireShorteCutPos = { 179, 200 };
    DirectX::XMFLOAT2 thunderShorteCutPos = { 179, 285 };
    DirectX::XMFLOAT2 iceShorteCutPos = { 179, 370 };
    DirectX::XMFLOAT2 healeShorteCutPos = { 200, 525 };

    // ステートエネミーロックオン用
    int stateEnemyIndex = 0;

    // ヒント描画するかどうか
    bool hintDrawCheck = false;

    // ピンチかどうか
    bool isPintch = false;

    // 描画するかどうか
    int isPlayerDrawCheck = 1;
    int skipDraw = 0;

    // Se関係の値
    bool isLoopSe = true;
    bool isLoopDisabled = false;


    DirectX::XMFLOAT4 vignettePinchColor = { 1,0,0,1 };
    float vignettePinchSmoothness = 3.0f;
    float vignettePinchIntensity = 0.8f;


    DirectX::XMFLOAT4 vignetteNormalColor = { 0,0,0,1 };
    float vignetteNormalSmoothness = 0.0f;
    float vignetteNormalIntensity = 0.0f;


    // 氷連射一時停止
    bool iceMagicMoveCheck = false;

    // 押し続けるか
    bool ButtonDownCountinue = true;

    // デバッグ
    int debugInt = 0;

    // UI操作用
    float playerCommandPushUiChargeTime = 0.0f;

    // エネミー接触判定
    bool isEnemyHit;
    // エネミー接触判定上半身
    bool isEnemyHitBody;

    bool debugShaderFlash = false;
    bool debugShaderFlashSeconde = false;
    bool debugCameraTime = false;

    /////////////////////// SE 再生関係
    
    // se再生情報
    AudioParam seParam;
    
    // アニメーションのループ再生
    bool isLoopAnim = true;

    // seの音の大きさ
    float seVolume = 0.8f;
    /////////////////////// 
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

public:

    // プレイヤーの種類
    enum class PlayerType
    {
        Main = 0,
    };

private:
    std::vector<std::shared_ptr<Actor>> players;
    // 削除予約
    std::set<std::shared_ptr<Actor>>       removes;
};