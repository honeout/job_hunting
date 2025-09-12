#pragma once
#include <vector>
#include "Component\Actor.h"
#include "Ui.h"
#include "Math\Mathf.h"
#include <set>

namespace CommandConfig
{
    // -----コマンド元画像----- 

    // コマンド基画像大きさ

    // コマンドHP元画像の位置　通常
    constexpr DirectX::XMFLOAT2 texNoDamagePos = { 0.0f, 0.0f };
    // コマンドHP元画像の位置　ダメージ
    constexpr DirectX::XMFLOAT2 texDamagePos = { 0.0f, 103.0f };

    // 非選択
    constexpr DirectX::XMFLOAT2 commandUnSelectTexScale = { .0f,.0f };
    // 選択
    constexpr DirectX::XMFLOAT2 commandSelectTexScale = { .0f,86.0f };
    // ショートカット
    constexpr DirectX::XMFLOAT2 commandShortCutTexScale = { .0f,172.0f };

    // 選択コマンド元画像位置
    constexpr DirectX::XMFLOAT2 commandSelectTexPos = { .0f,86.0f };

    // 非選択コマンド基画像位置
    constexpr DirectX::XMFLOAT2 commandUnSelectTexPos = { .0f,0.0f };

    // MPバー
    constexpr float mpBarOffset = 50.0f;
    //constexpr float mpBarOffset = 45.0f;

    // 魔法チャージ値
    constexpr float commandChargeAdd = 5.0f;

    // チャージ最大値
    constexpr float chargeMagicGaugeWidthMax = 260.0f;
    // -----コマンドアルファ-----
    // 透明度０
    constexpr float commandAlphaSelect = 1.0f;
    constexpr float commandAlphaUnSelect = 0.5f;

    // 値更新用
    constexpr float commandAlphaValue = 1.0f;
    constexpr float commandAlphaValueMax = 1.0f;
    constexpr float commandAlphaValueMin = 0.0f;
    
    // -----描画設定----- 
    // 描画
    constexpr bool draw = true;
    constexpr bool unDraw = false;

    // -----矢印描画-----
    // 特殊技チャージ矢印一個目開始
    constexpr float specialAttackChargeStart = 0.4f;
    // 特殊技チャージ矢印2個目開始
    constexpr float specialAttackChargeFirst = 0.8f;
    // 特殊技チャージ矢印3個目開始
    constexpr float specialAttackChargeSecond = 1.2f;

    // -----ターゲット-----
    // ターゲット位置
    constexpr DirectX::XMFLOAT2   scereenPositionOffset = { 34.0f,25.0f };



};

// uiマネージャー
class UiManager// 複数の弾がん球数
{
public:

    UiManager() {};
    // 誰が持つか分からないから複数の奴が別々に持つかもしれない
    ~UiManager() {};

    // インスタンス取得
#if defined(_DEBUG)
    inline static UiManager* debug_instance;
#endif
    static UiManager& Instance()
    {
        static UiManager instance;
#if defined(_DEBUG)
        debug_instance = &instance;
#endif
        return instance;
    }

    // 更新処理
    void Update(float elapsedTime);

    // ui登録
    void Register(std::shared_ptr<Actor> ui);

    // ui全削除
    void Clear();

    // ui数取得
    int GetUiesCount() const { return static_cast<int> (uies.size()); }

    // ui取得
    std::shared_ptr<Actor> GetUies(int index) { return uies.at(index); }

    // ui削除
    void Remove(std::shared_ptr<Actor> ui);
    
    // UiTimeUpの有無
    bool GetTimeUp(int uiNumber);

    // UI選択
    void SelectTex();

    // UI攻撃入力時
    void InputAttack();

    // UI魔法入力時
    void InputMagic();

    // UI表示
    void SelectDrawUi(int uiNumber);

    // UI点滅
    void SelectDrawUi(int uiNumber, float TimeAlpha, float TimeAlphaMax, float elapsedTime);

    // UI非表示
    void SelectNotDrawUi(int uiNumber);

    // UI魔法の選択
    void SelectUi(int uiNumber);

    // UI魔法の非選択
    void UnSelectUi(int uiNumber);

    // UIショートカットキー選択
    void SelectShortCutUi(int uiNumber);

    // UIショートカットキー非選択
    void UnSelectShortCutUi(int uiNumber);

    // Ui魔法チャージ動作開始
    void StartMagicUiCharge(int uiCommandNumber, int uiNumber , int uiFrameNumber, int chageNumber, float elapsedTime);

    // Ui魔法チャージ動作発射
    void StartMagicUiFire(int uiNumber, int uiFrameNumber, int chargeNumber);

    // UIシェイク
    void ShakeModeTyme(int uiNumber, bool& shakeMode);

    // UIポジション更新
    void PositionUpdate(int uiNumber, DirectX::XMFLOAT2 pos);
    void PositionXUpdate(int uiNumber, float posX);
    void PositionYUpdate(int uiNumber, float posY);

    // UI大きさ更新
    void ScaleUpdate(int uiNumber, DirectX::XMFLOAT2 scale);
    void ScaleUpdateX(int uiNumber, float scaleX);
    void ScaleUpdateY(int uiNumber, float scaleY);

    // UI元画像の位置を変える。
    void TexPosUpdate(int uiNumber, DirectX::XMFLOAT2 texPos);

    // フェードアウト
    void IncrementToAlpha(int uiNumber, float increment);

    // UI特殊技入力時
    void InputSpecialAttack();

    // コマンド選択 必殺技
    void SelectSpecialAttack();

    // 特殊技溜まった
    void SpecialAttackCharge(float elapsedTime);

    // UI特殊技
    void SpecialUpdate(float elapsedTime);

    // ロックオンUI処理
    void RockOnUI(ID3D11DeviceContext* dc,
        const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection);

    // 距離でUIを変えるロックオン中
    void AttackCheckUI();
public:
    enum class UiCount
    {
        PlayerHp = 0,
        EnemyHp,
        //SpecialBox,
        PlayerCommandAttack,
        PlayerCommandMagick,
        PlayerCommandSpecial,
        PlayerCommandFire,
        PlayerCommandRigtning,
        PlayerCommandIce,
        PlayerCommandHeale,
        PlayerComandSpeciulChargeBox,
        PlayerCommandSpeciulCharge01,
        PlayerCommandSpeciulCharge02,
        PlayerCommandSpeciulCharge03,
        PlayerCommandSpeciulShurashu,
        PlayerCommandSpeciulFrame,
        PlayerCommandSpeciulIce,
        PlayerCommandSpeciulThander,
        PlayerHPBar,
        PlayerMPBar,
        EnemyHPBar,
        EnemyHPLife01,
        EnemyHPLife02,
        TimeIrast,
        Time,
        Sight,
        SightCheck,
        Mp,
        PlayerCommandPush,
        PlayerCommandPushNow,
        PlayerCommandCharge,
        //PlayerCommandChargeFire,
        ShortCut,
        PushShort,
        OperationInstructionsSelect,
        OperationInstructionsButton,
    };

    enum class UiCountTitle
    {
        TitleSprite = 0,
        Start,
        Exit,
        Push,
        Select,
    };

    enum class UiCountGameOver
    {
        GameOverSprite = 0,
        Title,
        ReStart,
        Push,
        Select,
    };

    enum class UiCountGameCler
    {
        GameClerSprite = 0,
        Start,
        Title,
        Push,
        Select,
    };



private:
    // ui取得
    std::vector<std::shared_ptr<Actor>>   uies;
    // 特定ui削除登録
    std::set<std::shared_ptr<Actor>>       removes;

    // UIの描画について
    bool isUiSpecilDrawCheck = false;

    // 特殊技経過時間計算
    Mathf mathfSpecial;
    // ライト経過時間計算
    Mathf mathfblinking;

    // 経過時間ヒント最大値
    float timeElapsedHintMax = 1.0f;

    // 点滅時間
    float TimeAlpha = 0.0f;

    // UI操作用
    float commandPushUiChargeTime = 0.0f;
};


// uiマネージャー
class UiLoadingManager// 複数の弾がん球数
{
public:

    UiLoadingManager() {};
    // 誰が持つか分からないから複数の奴が別々に持つかもしれない
    ~UiLoadingManager() {};

    // インスタンス取得
    static UiLoadingManager& Instance()
    {
        static UiLoadingManager instance;
        return instance;
    }

    // ui登録
    void Register(std::shared_ptr<Actor> ui);

    // ui全削除
    void Clear();

    // ui数取得
    int GetUiesCount() const { return static_cast<int> (uies.size()); }

    // ui取得
    std::shared_ptr<Actor> GetUies(int index) { return uies.at(index); }

    // ui削除
    void Remove(std::shared_ptr<Actor> ui);

    // UI表示
    void SelectDrawUi(int uiNumber);

    // UI点滅
    void SelectDrawUi(int uiNumber, float TimeAlpha, float TimeAlphaMax, float elapsedTime);

    // UI非表示
    void SelectNotDrawUi(int uiNumber);

    // UI魔法の選択
    void SelectUi(int uiNumber);

    // UI魔法の非選択
    void UnSelectUi(int uiNumber);
public:
    enum class UiCountLoading
    {
        LodingIcon = 0,
        NowLoading1,
        NowLoading,
        GameStart,
        Control,
        ControlKeyBoard,
        SelectNumbar,
        Start,
        Button,
    };

private:
    // ui取得
    std::vector<std::shared_ptr<Actor>>   uies;
    // 特定ui削除登録
    std::set<std::shared_ptr<Actor>>       removes;

    // 経過時間計算
    Mathf mathfblinking;
};