#pragma once
#include <vector>
#include "Actor.h"
#include <set>


// ui�}�l�[�W���[
class UiManager// �����̒e���񋅐�
{
public:

    UiManager() {};
    // �N������������Ȃ����畡���̓z���ʁX�Ɏ���������Ȃ�
    ~UiManager() {};

    // �C���X�^���X�擾
    static UiManager& Instance()
    {
        static UiManager instance;
        return instance;
    }

    // ui�o�^
    void Register(std::shared_ptr<Actor> ui);

    // ui�S�폜
    void Clear();

    // ui���擾
    int GetUiesCount() const { return static_cast<int> (uies.size()); }

    // ui�擾
    std::shared_ptr<Actor> GetUies(int index) { return uies.at(index); }

    // ui�폜
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
    // ui�擾
    std::vector<std::shared_ptr<Actor>>   uies;
    // ����ui�폜�o�^
    std::set<std::shared_ptr<Actor>>       removes;
};