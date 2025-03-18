#pragma once
#include <vector>
#include "Actor.h"
#include <set>


// uiマネージャー
class UiManager// 複数の弾がん球数
{
public:

    UiManager() {};
    // 誰が持つか分からないから複数の奴が別々に持つかもしれない
    ~UiManager() {};

    // インスタンス取得
    static UiManager& Instance()
    {
        static UiManager instance;
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
    enum class UiCount
    {
        PlayerHp = 0,
        EnemyHp,
        PlayerCommandAttack,
        PlayerCommandMagick,
        PlayerCommandAttackCheck,
        PlayerCommandMagickCheck,
        PlayerCommandFire,
        PlayerCommandFireCheck,
        PlayerCommandRigtning,
        PlayerCommandRigtningCheck,
        PlayerCommandIce,
        PlayerCommandIceCheck,
        PlayerCommandHeale,
        PlayerCommandHealeCheck,
        PlayerCommandSpeciulCharge01,
        PlayerCommandSpeciulCharge02,
        PlayerCommandSpeciulCharge03,
        PlayerCommandSpeciulShurashu,
        PlayerCommandSpeciulShurashuPushu,
        PlayerCommandSpeciulFrame,
        PlayerCommandSpeciulFramePushu,
        PlayerCommandSpeciulIce,
        PlayerCommandSpeciulThander,
        PlayerHPBar,
        EnemyHPBar,
        EnemyHPLife01,
        EnemyHPLife02,
        Time,
        Sight,
        SightCheck,
        Mp,
        PlayerCommandShortCutFire,
        PlayerCommandShortCutSunder,
        PlayerCommandShortCutIce,
        PlayerCommandShortCutKeule,
        Push,
        Push2,
        PushShort,
        ShortCut,
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
};