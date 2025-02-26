#pragma once

#include <DirectXMath.h>
#include "Graphics/Shader.h"
#include "Graphics/Model.h"
//#include "Character.h"
//#include "EnemyManager.h"
#include "Collision.h"
#include "ProjectileManager.h"
#include "Audio/AudioSource.h"
#include "Component.h"
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



    //// インスタンス取得
    //static Player& Instance();


    // アニメーション
    //enum Animation
    //{
    //    Anim_Attack,
    //    Anim_Death,
    //    Anim_Falling,
    //    Anim_GetHit1,
    //    Anim_GetHit2,
    //    Anim_Idle,
    //    Anim_Jump,
    //    Anim_Jump_Flip,
    //    Anim_Landing,
    //    Anim_Revive,
    //    Anim_Running,
    //    Anim_Walking
    //};
    //enum Animation
    //{
    //    Anim_Tpose,
    //    Anim_Idle,
    //    Anim_Move,
    //    Anim_Back,
    //    Anim_Beside,
    //    Anim_BesideSrow,
    //    Anim_Jump,
    //    Anim_Slash,
    //    Anim_SlashBeside,
    //    Anim_SlashThree,
    //    Anim_Deth,
    //    Anim_Pain
    //    
    //};

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
    void CollisionProjectilesVsEnemies();

    void CollisionRubyVsEnemies();

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

    // HP管理

protected:
    //// 着地した時に呼ばれる
    //void OnLanding() override;

    //// 死亡した時に呼ばれる
    //void OnDead() override;

    //// ダメージを受けた時に呼ばれる
    //void OnDamaged() override;

public:



    // スティック入力値から移動ベクトルを取得 進行ベクトルを取る進むべき方向
    DirectX::XMFLOAT3 GetMoveVec(float elapsedTime) const;

    DirectX::XMFLOAT3 GetMagicMoveVec(float elapsedTime) const;




    // 移動入力処理
    bool InputMove(float elapsedTime);

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

    // 特殊攻撃選択
    bool InputSpecialAttackCharge();
    bool InputSpecialShotCharge();

    // UIの演出
    void SpecialPlayUlEffect(float elapsedTime);

    // 特殊技チャージ出来ました。
    void ChargeSpecialEnergyMultiple();
 
    bool GetSpecialAttackTime() const { return specialAttackTime; }
    void SetSpecialAttackTime(bool specialAttackTime) { this->specialAttackTime = specialAttackTime; }

    // プレイヤーとエネミーとの衝突処理
    //void CollisionPlayerVsEnemies();

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
    
    // ノードとルビー反射
    void CollisionNodeVsRubyCounter(const char* nodeName, float nodeRadius);

    // ノードとルビー反射返し
   void CollisionNodeVsRubyCounterBulletFring(const char* nodeName, float nodeRadius);

    // ジャンプ入力処理
    bool InputJump();

    // 弾丸入力
    bool InputProjectile();

    // 回避orカウンター入力
    bool InputAvoidance();

    // 攻撃入力
    bool InputAttack();

    // 魔法入力
    bool InputMagick();


    // メニュー開く入力
    bool InputMenue();


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

    // 魔法入力
    bool InputMagicframe();
    bool InputMagicIce();
    bool InputMagicLightning();
    bool InputMagicHealing();

    void InputSpecialMagicframe();

    void AttackCheckUI();

   // void Update();

    void inFloat3(DirectX::XMFLOAT3 value, DirectX::XMFLOAT3& inValue);

    DirectX::XMFLOAT3 GetForwerd(DirectX::XMFLOAT3 angle);

    // ステートマシーン取得
    //StateMachine* GetStateMachine() { return stateMachine; }
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
    bool GetInvalidJudgment() const { return invalidJudgment;}
    // セット当たり判定無効
    void SetInvalidJudgment(bool invalidJudgment) { this->invalidJudgment = invalidJudgment;}

    // 自分の当たり判定有無
    void DmageInvalidJudment(bool invalidJudgment);

    //  UI操作
    void UiControlle(float elapsedTime);

    // 魔法の量
    void MagicPointUpdate();

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
        SpecialMagicIce,
        SpecialMagicThander,
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
        //Anim_Attack,
        //Anim_Death,
        //Anim_Falling,
        //Anim_GetHit1,
        //Anim_GetHit2,
        //Anim_Idle,
        //Anim_Jump,
        //Anim_Jump_Flip,
        //Anim_Landing,
        //Anim_Revive,
        //Anim_Running,
        //Anim_Walking

        //Anim_Tpose,
        //Anim_Idle,
        //Anim_Move,
        //Anim_Back,
        //Anim_Beside,
        //Anim_BesideSrow,
        //Anim_Jump,
        //Anim_Slash,
        //Anim_SlashBeside,
        //Anim_SlashThree,
        //Anim_Deth,
        //Anim_Pain,
        //Anim_Counter


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
        Anim_Roll

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
        Magic
    };

    enum class CommandMagic
    {
        Normal = -2,
        Fire = 0,
        Thander,
        Ice,
        Heale
    };

    enum class SpecialAttack
    {
        Normal = -1,
        Attack,
        MagicFire,
        MagicThander,
        MagicIce,
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



    bool Ground();

    State GetStateTuren() const { return this->state; }

    void SetStateTuren(State state)  { this->state = state; }

    // 再生方法
    void  SetUpdateAnim(UpAnim  updateanim) { this->updateanim = updateanim; }

    UpAnim  GetUpdateAnim() const  { return this->updateanim; }

    float GetMoveSpeedAnimation() const { return moveSpeedAnimation; }

    // UI揺らし状態
    void SetShakeMode(bool shakeMode) { this->shakeMode = shakeMode;   }

    // 回転確認
    void SetAngleCheck(bool angleCheck) { this->angleCheck = angleCheck; }

    //// 反射後の当たり判定敵に反映
    //void SetCounterCheck(bool counterCheck) { this->counterCheck = counterCheck; }

    //// 反射後の当たり判定敵に反映
    //bool GetCounterCheck() const { return counterCheck; }

    float GetGravity() const { return gravity; }

    void SetLockOnState(CameraState lockonState) { this->lockonState = lockonState; }

    CameraState GetLockOnState() const { return lockonState; }
    
    void Hit();

    // ロックオン入力確認
    void SetRockCheck(bool rockCheck) { this->rockCheck = rockCheck; }
    bool GetRockCheck() { return this->rockCheck; }

    void SetFreeCameraCheck(bool freeCameraCheck) { this->freeCameraCheck = freeCameraCheck; }

    // 色
    void SetColorGradingData(ColorGradingData colorGradingData) { this->colorGradingData = colorGradingData; }

    // fashu
    void SetFlashOn(bool flashOn) { this->flashOn = flashOn; }
    bool GetFlashOn() { return flashOn; }

    void SetHitCheck(bool hitCheck) {  this->hitCheck = hitCheck; }
    bool GetHitCheck() {  return hitCheck; }

   // AudioSeSource* PlaySE() { return seSouce.get(); }

    void SetAttackNumberSave(int attackNumberSave) { this->attackNumberSave = attackNumberSave; }

    // UIがないときはfalse
    void SetUiControlleCheck(bool uiControlleCheck) { this->uiControlleCheck = uiControlleCheck; }

    // ステート変換
    void SetEndState(bool endState) { this->endState = endState; }
    bool GetEndState() { return endState; }

    // 必殺技中のロックオフ
    void SetSpecialRockOff(bool specialRockOff) { this->specialRockOff = specialRockOff; }

private:
    //std::shared_ptr<Movement>	movement;
    //std::shared_ptr<HP>	hp;
    //std::shared_ptr<Mp> mp;
    //std::shared_ptr<Transform> transform;
    std::weak_ptr<Movement>	movement;
    std::weak_ptr<HP> hp;
    std::weak_ptr<Mp> mp;
    std::weak_ptr<Transform> transform;

    
    DirectX::XMFLOAT3 velocity = { 0,0,0 };

    // 空中行動許可
    bool isAreAttack = false;

    // 空中行動許可間隔
    float areAttackState = .0;
    float areAttackStateEnd = .0;
    float areAttackStateMax = 2.0f;

    // UI処理を出すかチェック
    bool             uiControlleCheck = false;

    // 炎発射
   // std::unique_ptr<AudioSeSource> seSouce;

    //std::unique_ptr<Model> model;
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


    //////// 音関係 //////////
    
    // BGM
    //std::unique_ptr<AudioSource> Bgm;

    // SE
    //std::unique_ptr<AudioSeSource> SePlayer;

    //// １斬撃
    //std::unique_ptr<AudioSource> slashFustSe;
    //// ２斬撃
    //std::unique_ptr<AudioSource> slasSecondeSe;
    //// 打撃
    //std::unique_ptr<AudioSource> buttonTherdeSe;

    //// 歩行
    //std::unique_ptr<AudioSource> walkSe;

    //// ダッシュ
    //std::unique_ptr<AudioSource> DushSe;

    //// ジャンプ
    //std::unique_ptr<AudioSource> janpSe;

    //// 着地
    //std::unique_ptr<AudioSource> landSe;

    //// 被ダメージ
    //std::unique_ptr<AudioSource> damageSe;

    //// 混乱敵
    //std::unique_ptr<AudioSource> confusionSe;

    //// 死亡音声敵
    //std::unique_ptr<AudioSource> deathSe;


    //// ヒットストップ
    //std::unique_ptr<AudioSource> hitStop;

    //// 必殺技炎
    //std::unique_ptr<AudioSource> flameSpecialStarteSe;
    //// 必殺技炎ため
    //std::unique_ptr<AudioSource> flameSpecialSaveSe;

    //// 必殺技雷
    //std::unique_ptr<AudioSource> lighteningStrikeSpecialSe;
    //// 必殺技雷ため
    //std::unique_ptr<AudioSource> lighteningStrikeSpecialSaveSe;


    //// 炎発射
    //std::unique_ptr<AudioSource> flameStarteSe;
    //// 炎持続
    //std::unique_ptr<AudioSource> flameDurationSe;
    //// 炎着弾時
    //std::unique_ptr<AudioSource> flameimpactSe;
    //

    //// 氷発射
    //std::unique_ptr<AudioSource> iceStarteSe;
    //// 氷持続
    //std::unique_ptr<AudioSource> iceDurationSe;
    //// 氷着弾時
    //std::unique_ptr<AudioSource> iceimpactSe;

    //// 雷ヒット時
    //std::unique_ptr<AudioSource> lightningSe;



    /////////////////////////



    std::unique_ptr<Effect> hitSlash;

    std::unique_ptr<Effect> lightningAttack;

    std::unique_ptr<Effect> hitFire;
    std::unique_ptr<Effect> hitThander;
    std::unique_ptr<Effect> hitIce;

    // 足もとに竜巻
    std::unique_ptr<Effect> areWork;

    State                   state = State::Idle;
    State                   stated = State::Idle;
    
    CameraState            cameraState = CameraState::Normal;
    CameraState			lockonState = CameraState::NotLockOn;

    DirectX::XMFLOAT3 lockonCharactor = {0.0f,0.0f,0.0f};


    float				lockonTargetChangeTime = 0;
    float				lockonTargetChangeTimeMax = 8;

    // 重力
    float gravity =-0.5f;

    // 着地場所までの距離　 十分な速度で落とす重力の５倍２、３秒後に着地モーションをする。
    float jumpfliptime = gravity * 5;

    float            leftHandRadius = 1.0f;

    bool             attackCollisionFlag = false;
  
    //int   health = 10;

    // 歩いている判定
    bool afterimagemove = false;

    bool blend = false;

    // アップデート再生種類
    UpAnim  updateanim = UpAnim::Normal;

    DirectX::XMFLOAT3 moveVec = {0.0f,0.0f,0.0f};

    DirectX::XMFLOAT3 frontVec = { 0 ,0,0 };

    // 上半身更新開始位置
    //char* bornUpStartPoint;

    //// 上半身の何処まで更新するか
    //char* bornUpEndPoint = "";

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


    // 当たり判定半径
    //float radius = 0.7f;
    float radius = 1.0f;

    // Hp
    int health = 50;

    // 最大HP
    int maxHealth = 50;

    // Mp
    int magicPoint = 50;




    // アニメーションの時間 
    float currentANimationSeconds = 0.0f;




    // 弾丸生命時間
    //float   lifeTimer = 3.0f;

    /////////////////// カメラ関係



    CameraController* cameraControlle = nullptr;
    //std::unique_ptr<CameraController> cameraControlle;

    DirectX::XMFLOAT3	cameraAngle = DirectX::XMFLOAT3(0, 0, 0);
    float				cameraRollSpeed = DirectX::XMConvertToRadians(90);
    float				cameraMaxPitch = DirectX::XMConvertToRadians(45);
    float				cameraMinPitch = DirectX::XMConvertToRadians(-45);
    float				cameraRange = 100.0f;
    float				characterHeight = 10.0f;



    // 攻撃方法の選択
    int selectCheck = 0;

    // 魔法選択
    int selectMagicCheck = 0;

    // 特殊攻撃出るまで
    float specialAttackCharge = 0.0f;
    float specialAttackChargeMax = 1.5f;
    float specialShotCharge = 0.0f;

    // 攻撃ヒット回数
    int attackNumberSave;
    int attackNumberSaveMax = 4;

    // 特殊技のチャージ
    int attackEnergyCharge = 0;
    int fireEnergyCharge = 0;
    int ThanderEnergyCharge = 0;
    int iceEnergyCharge = 0;
    int energyChargeMax = 4;

    // 特殊技のチャージ完了
    bool isSpecialChargeComplete = false;

    // 特殊攻撃をためる奴
    std::stack<int> chargedSpecialMoves;


    // ステートマシン用
    //StateMachine* stateMachine = nullptr;
    std::unique_ptr<StateMachine> stateMachine;


    // 移動傾き
    float moveSpeedAnimation = 0.0f;

    // 当たり判定無効判定
    bool invalidJudgment = true;

    // 特殊攻撃をためる奴
    std::stack<int> specialAttack;
    //std::vector<int> specialAttack2;


    // 特殊アクション確認
    bool specialAttackTime = false;

    // 魔法攻撃を打つショートカット用
    bool magicAction = false;

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

    // 反射後の当たり判定敵に反映
    //bool counterCheck = false;

    // 魔法の射程当たる距離
    float magicRangeLength = 470;

    // 魔法消費量
    int magicConsumption = 5;


    // 描画の色をどうするか
    DirectX::XMFLOAT4 mpUiColor = {1,1,1,1};


    // ダメージ量
    int applyDamageNormal = 3;

    int applyDamageFire = 4;
    int applyDamageThander = 2;
    int applyDamageIce = 3;

    // マジックダメージ
    int applyDamageMagic = 0;



    // 必殺技ダメージ量
    int applyDamageSpecial = 2;

    int hitMortion = 0;
    int hitMortionMax = 180;

    // デバッグ用必殺技のカメラアングルを確認
    bool SpecialCameraRock = false;


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

    int stateSize = 2;

    // uiCount最大値
    int uiCountMax = 3;

    // 終了タイミング
    bool endState = false;

    //float uiTimeIntervalMax = 5.0f; 
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