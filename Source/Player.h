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
#include <stack>

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
    // SE再生 歩き
    void InputWalkSE();
    void InputStopWalkSE();
    // SE再生 ジャンプ
    void InputJampSE();
    void InputStopJampSE();
    // SE再生 空中
    void InputAreWalkSE();
    // SE再生 ダッシュ
    void InputDashSE();
    // SE再生 斬撃
    void InputAttackSlashSE();
    // SE再生 魔法　炎
    void InputAttackFlameSE();
    // SE再生 魔法　雷
    void InputAttackThanderSE();
    void InputStopAttackThanderSE();
    // SE再生 魔法　氷
    void InputAttackIceSE();
    // SE再生 魔法　回復
    void InputAttackHealeSE();
    // SE再生 必殺技斬撃
    void InputAttackSlashSpecileLightningStrikeSE();
    // SE再生 必殺技魔法　炎
    void InputAttackFlameSpecileSE();
    // カメラ切り替え処理
    void UpdateCameraState(float elapsedTime);
    // 弾丸と敵の衝突処理
    void CollisionMagicVsEnemies();
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
    // 曲再生
    void PlayPintchSe();
    void StopPintchSe();
    // 必殺技ため終了
    void PlaySpecialChargeCompleteSe();

    // テレポート音
    void PlayTellePortSe();

    // スティック入力値から移動ベクトルを取得 進行ベクトルを取る進むべき方向
    DirectX::XMFLOAT3 GetMoveVec(float elapsedTime) const;

    DirectX::XMFLOAT3 GetMagicMoveVec(float elapsedTime) const;

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
    bool CollisionNodeVsEnemies(
        const char* nodeName,
        float nodeRadius,
        const char* nodeHeartName,
        const char* nodeLeftArmName,
        const char* nodeRightArmName

    );

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

    // メニュー開く入力
    bool InputMenue();

    // デバッグ用距離
    void DebugLength();

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

    void inFloat3(DirectX::XMFLOAT3 value, DirectX::XMFLOAT3& inValue);

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
    void UiControlle(float elapsedTime);

    // ダメージ判定
    void SpecialApplyDamageInRadius();
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

private:
        //// 構造体
        //struct SpecialMode
        //{
        //    bool specialAttack;
        //    bool specialShot;
        //};
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

    float GetGravity() const { return gravity; }

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

    DirectX::XMFLOAT3 velocity = { 0,0,0 };

    // 空中行動許可
    bool isAreAttack = false;

    // 空中行動許可間隔
    int areAttackState = 0;
    int areAttackStateEnd = 0;
    int areAttackStateMax = 1;

    // 空中行動間隔時間
    float areAttackTime = 0.0f;
    float areAttackTimeEnd = 0.0f;
    float areAttackTimeMax = 1.0f;
    float areAttackTimeValue = 0.1f;

    // 経過時間を測る
    float timeElapsed = 0.0f;
    float timeElapsedMin = 0.0f;
    float timeElapsedHintMax = 1.0f;

    // UI処理を出すかチェック
    bool             uiControlleCheck = false;

    Model* model = nullptr;

    GamePad        gamePad;

    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 angle = {};
    DirectX::XMFLOAT3 scale = {};

    // 身長
    float height = 1.0f;

    // 斬撃エフェクト　スケール
    float slashScale = 0.4f;

    float          moveSpeed = 5.0f;

    float          turnSpeed = DirectX::XMConvertToRadians(720);

    float          turnSpeedAdd = 0.0f;

    // 回転速度　攻撃時
    float          turnSpeedAttack = DirectX::XMConvertToRadians(2600);

    float          jumpSpeed = 13.0f;

    int                     jumpLimit = 2;

    ProjectileManager projectileManager;

    Effect* hitEffect = nullptr;
    Effect* magicFireHitEffect = nullptr;
    Effect* magicThenderHitEffect = nullptr;
    Effect* magicIceHitEffect = nullptr;
    Effect* ImpactEffect = nullptr;
    Effect* desEffect = nullptr;
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

    // 重力
    float gravity = -0.5f;

    // 着地場所までの距離　 十分な速度で落とす重力の５倍２、３秒後に着地モーションをする。
    float jumpfliptime = gravity * 5;

    float            leftHandRadius = 1.0f;

    bool             attackCollisionFlag = false;

    // 歩いている判定
    bool afterimagemove = false;

    bool blend = false;

    // アップデート再生種類
    UpAnim  updateanim = UpAnim::Normal;

    DirectX::XMFLOAT3 moveVec = { 0.0f,0.0f,0.0f };

    DirectX::XMFLOAT3 frontVec = { 0 ,0,0 };

    // 上半身スタート再生開始場所
    char* bornUpStartPoint = "";

    // 上半身エンド再生停止場所
    char* bornUpEndPoint = "";

    // 下半身スタート再生開始場所
    char* bornDownerStartPoint = "";

    // 下半身エンド再生停止場所
    char* bornDownerEndPoint = "";

    // 当たり判定半径
    float radius = 1.0f;

    // Hp
    int health = 50;

    // 最大HP
    int maxHealth = 50;

    // Mp
    int magicPoint = 50;

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
    float specialAttackChargeSlashValue = 0.1f;
    float specialAttackChargeMagicValue = 0.2f;
    float specialAttackChargeMin = 0.0f;
    float specialAttackChargeMax = 1.5f;
    float specialShotCharge = 0.0f;

    // 攻撃ヒット回数
    int attackNumberSave;
    int attackNumberSaveMax = 4;

    // 特殊技のチャージ
    int attackEnergyCharge = 0;
    int attackEnergyChargeMin = 0;
    int fireEnergyCharge = 0;
    int fireEnergyChargeMin = 0;
    int ThanderEnergyCharge = 0;
    int iceEnergyCharge = 0;
    int energyChargeMax = 4;

    // 特殊技のチャージ完了
    bool isSpecialChargeComplete = false;

    // 特殊攻撃をためる奴
    std::stack<int> chargedSpecialMoves;

    // seの音の大きさ
    float seVolume = 0.5f;

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

    // 特殊攻撃をためる奴
    //std::stack<int> specialAttack;
    //std::vector<int> specialAttack;
    //bool specialAttack[specialAttackNumMax];
    std::vector<SpecialAttack> specialAttack;

    bool isSkillHave = false;

    int spCmdMoveLimitMin = 0;
    int spCmdMoveLimitMax = 1;

    // 特殊攻撃のサイズが最低値ならやめる。
    int specialAttackNumMin = 0;

    // 特殊アクション確認
    bool specialAttackTime = false;

    // 魔法攻撃を打つショートカット用
    bool magicAction = false;

    // 特殊技コマンド開く
    bool specialAction = false;

    // 特殊技斬撃がたまったか
    bool isSpecialAttack = false;

    // 特殊技炎がたまったか
    bool isSpecialMagicFire = false;

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

    // デバッグ
    float debugLength;

    // 押し続けるか
    bool ButtonDownCountinue = true;

    // デバッグ
    int debugInt = 0;
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