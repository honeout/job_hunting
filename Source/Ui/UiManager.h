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

    // -----コマンドアルファ-----
    // 透明度０
    constexpr float commandAlphaSelect = 1.0f;
    constexpr float commandAlphaUnSelect = 0.5f;
    
    // -----描画設定----- 
    // 描画
    constexpr bool draw = true;
    constexpr bool unDraw = false;

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

    // UI特殊技入力チャージ
    bool InputSpecialAttackCharge(float elapsedTime);

    // 特殊技溜まった
    void SpecialAttackCharge(float elapsedTime);

    // UI特殊技
    void SpecialUpdate(float elapsedTime);

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

    // あらゆる計算
    Mathf mathfPintch;

    // 経過時間ヒント最大値
    float timeElapsedHintMax = 1.0f;
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
};