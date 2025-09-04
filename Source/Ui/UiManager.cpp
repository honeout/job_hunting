#include "UiManager.h"
#include "Character\Player.h"

void UiManager::Update(float elapsedTime)
{
    //for (auto ui : uies)
    //{
    //    if (ui->GetComponent<Ui>())
    //    {

    //    }
    //}
}

void UiManager::Register(std::shared_ptr<Actor> ui)
{
    uies.emplace_back(ui);
}

void UiManager::Clear()
{
    uies.clear();
}

void UiManager::Remove(std::shared_ptr<Actor> ui)
{
    removes.insert(ui);
}

// 入力UI
void UiManager::InputAttack()
{
    int playerKinds = PlayerManager::Instance().GetPlayerCount() - 1;

    // 安全チェック プレイヤー
    auto playerId = PlayerManager::Instance().GetPlayer(playerKinds);
    if (!playerId)
        return;

    // 安全チェック プレイヤーの情報
    auto playerMain = playerId->GetComponent<Player>();
    if (!playerMain)
        return;

    // コマンドUI　選択中 攻撃選ぶ
    if (playerMain->GetSelectCheck() == (int)Player::CommandAttack::Attack)
    {
        // 安全チェック　コマンドUI　攻撃
        auto sharedUiComandoAttackId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttack);
        if (!sharedUiComandoAttackId)
            return;
        auto uiIdAttackUi = sharedUiComandoAttackId->GetComponent<Ui>();
        auto uiIdAttackTransform2D = sharedUiComandoAttackId->GetComponent<TransForm2D>();

        // 安全チェック
        if (!uiIdAttackUi && !uiIdAttackTransform2D) return;

        // 元画像の選択状態の場所
        uiIdAttackTransform2D->SetTexPosition(CommandConfig::commandSelectTexScale);

        // 非選択用アルファ値
        uiIdAttackUi->SetAlpha(CommandConfig::commandAlphaSelect);
    }
    // コマンドUI　非選択 攻撃
    else
    {
        // 安全チェック　コマンドUI　攻撃
        auto sharedUiComandoAttackId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandAttack);
        if (!sharedUiComandoAttackId)
            return;
        auto uiIdAttackUi = sharedUiComandoAttackId->GetComponent<Ui>();
        auto uiIdAttackTransform2D = sharedUiComandoAttackId->GetComponent<TransForm2D>();

        // 安全チェック
        if (!uiIdAttackUi && !uiIdAttackTransform2D) return;

        // 元画像の非選択状態の場所
        uiIdAttackTransform2D->SetTexPosition(CommandConfig::commandUnSelectTexScale);

        // 非選択用アルファ値
        uiIdAttackUi->SetAlpha(CommandConfig::commandAlphaUnSelect);
    }
}

// UI魔法入力時
void UiManager::InputMagic()
{
    int playerKinds = PlayerManager::Instance().GetPlayerCount() - 1;

    // 安全チェック プレイヤー
    auto playerId = PlayerManager::Instance().GetPlayer(playerKinds);
    if (!playerId)
        return;

    // 安全チェック プレイヤーの情報
    auto playerMain = playerId->GetComponent<Player>();
    if (!playerMain)
        return;

    //  コマンドUI　選択中 魔法選んだ時
    if (playerMain->GetSelectCheck() == (int)Player::CommandAttack::Magic)
    {
        // 安全チェック コマンドUI　魔法
        auto sharedUiComandoMagickId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagick);
        if (!sharedUiComandoMagickId)
            return;
        auto uiIdMagicUi = sharedUiComandoMagickId->GetComponent<Ui>();
        auto uiIdMagickTransform2D = sharedUiComandoMagickId->GetComponent<TransForm2D>();

        // 安全チェック
        if (!uiIdMagicUi && !uiIdMagickTransform2D) return;

        // 元画像の選択状態の場所
        uiIdMagickTransform2D->SetTexPosition(CommandConfig::commandSelectTexScale);

        // 非選択用アルファ値
        uiIdMagicUi->SetAlpha(CommandConfig::commandAlphaSelect);
    }
    //  コマンドUI　非選択 魔法
    else
    {
        // 安全チェック コマンドUI　魔法
        auto sharedUiComandoMagickId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandMagick);
        if (!sharedUiComandoMagickId)
            return;
        auto uiIdMagicUi = sharedUiComandoMagickId->GetComponent<Ui>();
        auto uiIdMagickTransform2D = sharedUiComandoMagickId->GetComponent<TransForm2D>();

        // 安全チェック
        if (!uiIdMagicUi && !uiIdMagickTransform2D) return;

        // 元画像の非選択状態の場所
        uiIdMagickTransform2D->SetTexPosition(CommandConfig::commandUnSelectTexScale);

        // 非選択用アルファ値
        uiIdMagicUi->SetAlpha(CommandConfig::commandAlphaUnSelect);
    }
}

// コマンドUI　選択中 必殺技選ぶ
void UiManager::InputSpecialAttack()
{
    int playerKinds = PlayerManager::Instance().GetPlayerCount() - 1;

    // 安全チェック プレイヤー
    auto playerId = PlayerManager::Instance().GetPlayer(playerKinds);
    if (!playerId)
        return;

    // 安全チェック プレイヤーの情報
    auto playerMain = playerId->GetComponent<Player>();
    if (!playerMain)
        return;

    // コマンドUI　選択中 必殺技選ぶ
    if (playerMain->GetSelectCheck() == (int)Player::CommandAttack::Special)
    {
        // 安全チェック コマンドUI　必殺技
        auto sharedUiSpecialId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpecial);
        if (!sharedUiSpecialId)
            return;
        auto uiIdSpecialUi = sharedUiSpecialId->GetComponent<Ui>();
        auto uiIdSpecialTransform2D = sharedUiSpecialId->GetComponent<TransForm2D>();
        if (!uiIdSpecialUi && !uiIdSpecialTransform2D) return;

        // 選択
        uiIdSpecialTransform2D->SetTexPosition(CommandConfig::commandSelectTexScale);

        // コマンド選択判断　透明度
        float selectAlpha = CommandConfig::commandAlphaSelect;

        // 特殊技UI選択不可 斬撃
        if (!playerMain->GetSpecialAttack((int)Player::SpecialAttackType::Attack) &&
            !playerMain->GetSpecialAttack((int)Player::SpecialAttackType::MagicFire))
            selectAlpha = CommandConfig::commandAlphaUnSelect;

        // 選択不可かどうか
        uiIdSpecialUi->SetAlpha(selectAlpha);
    }
    //  コマンドUI　非選択 必殺技
    else
    {
        // 安全チェック コマンドUI　必殺技
        auto sharedUiSpecialId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpecial);
        if (!sharedUiSpecialId)
            return;
        auto uiIdSpecialUi = sharedUiSpecialId->GetComponent<Ui>();
        auto uiIdSpecialTransform2D = sharedUiSpecialId->GetComponent<TransForm2D>();
        if (!uiIdSpecialUi && !uiIdSpecialTransform2D) return;

        // 選択
        uiIdSpecialTransform2D->SetTexPosition(CommandConfig::commandUnSelectTexScale);

        // 選択不可かどうか
        uiIdSpecialUi->SetAlpha(CommandConfig::commandAlphaUnSelect);
    }
}

// コマンド選択 必殺技
void UiManager::SelectSpecialAttack()
{
    int playerKinds = PlayerManager::Instance().GetPlayerCount() - 1;

    // 安全チェック プレイヤー
    auto playerId = PlayerManager::Instance().GetPlayer(playerKinds);
    if (!playerId)
        return;

    // 安全チェック プレイヤーの情報
    auto playerMain = playerId->GetComponent<Player>();
    if (!playerMain)
        return;

    // コマンド選択 必殺技
    if (playerMain->GetSpecialAction())
    {
        // 安全チェック コマンド斬撃
        auto sharedUiComandoAttackId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu);
        if (!sharedUiComandoAttackId)
            return;
        // コマンド　必殺技斬撃表示するか
        auto uiIdSpecialComandoAttack = sharedUiComandoAttackId->GetComponent<Ui>();
        // 安全チェック
        if (!uiIdSpecialComandoAttack) return;

        // 安全チェック コマンド魔法炎
        auto sharedUiCommandSpecialFireId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame);
        if (!sharedUiCommandSpecialFireId)
            return;
        // コマンド　必殺技魔法炎表示するか
        auto uiIdSpecialComandoFire = sharedUiCommandSpecialFireId->GetComponent<Ui>();
        // 安全チェック
        if (!uiIdSpecialComandoFire) return;

        uiIdSpecialComandoAttack->SetDrawCheck(CommandConfig::draw);
        uiIdSpecialComandoFire->SetDrawCheck(CommandConfig::draw);
    }
    // コマンド非選択 必殺技
    else
    {
        // 安全チェック コマンド斬撃
        auto sharedUiComandoAttackId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu);
        if (!sharedUiComandoAttackId)
            return;
        auto uiIdAttack = sharedUiComandoAttackId->GetComponent<Ui>();

        // 安全チェック
        if (!uiIdAttack)
            return;

        // 安全チェック コマンド魔法炎
        auto sharedUiComandoFireId = UiManager::Instance().GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame);
        if (!sharedUiComandoFireId)
            return;
        auto uiIdAttackCheck = sharedUiComandoFireId->GetComponent<Ui>();

        // 安全チェック
        if (!uiIdAttackCheck)
            return;

        uiIdAttack->SetDrawCheck(CommandConfig::unDraw);
        uiIdAttackCheck->SetDrawCheck(CommandConfig::unDraw);
    }
}

// 特殊技溜まった
void UiManager::SpecialAttackCharge(float elapsedTime)
{
    int playerKinds = PlayerManager::Instance().GetPlayerCount() - 1;

    // 安全チェック プレイヤー
    auto playerId = PlayerManager::Instance().GetPlayer(playerKinds);
    if (!playerId)
        return;

    // 安全チェック プレイヤーの情報
    auto playerMain = playerId->GetComponent<Player>();
    if (!playerMain)
        return;

    int uiCount = UiManager::Instance().GetUiesCount();

    // uiCount最大値
    int uiCountMax = 5;

    // ui無かったら
    if (uiCount <= uiCountMax) return;
    // 安全チェック
    auto sharedUiSpecialChargeFurstId = UiManager::Instance().GetUies(
        (int)UiManager::UiCount::PlayerCommandSpeciulCharge01);
    auto sharedUiSpecialChargeSecondId = UiManager::Instance().GetUies(
        (int)UiManager::UiCount::PlayerCommandSpeciulCharge02);
    auto sharedUiSpecialChargeSerdeId = UiManager::Instance().GetUies(
        (int)UiManager::UiCount::PlayerCommandSpeciulCharge03);
    if (!sharedUiSpecialChargeFurstId && !sharedUiSpecialChargeSecondId && !sharedUiSpecialChargeSerdeId)
        return;

    auto uiIdSpecialChargeFurst = sharedUiSpecialChargeFurstId->GetComponent<Ui>();
    auto uiIdSpecialChargeSecond = sharedUiSpecialChargeSecondId->GetComponent<Ui>();
    auto uiIdSpecialChargeSerde = sharedUiSpecialChargeSerdeId->GetComponent<Ui>();

    //// 一度発動すると初期化
    bool drawCheck = CommandConfig::unDraw;
    uiIdSpecialChargeFurst->SetDrawCheck(drawCheck);
    uiIdSpecialChargeSecond->SetDrawCheck(drawCheck);
    uiIdSpecialChargeSerde->SetDrawCheck(drawCheck);

}

// UI特殊技
void UiManager::SpecialUpdate(float elapsedTime)
{
    int playerKinds = PlayerManager::Instance().GetPlayerCount() - 1;

    // 安全チェック プレイヤー
    auto playerId = PlayerManager::Instance().GetPlayer(playerKinds);
    if (!playerId)
        return;

    // 安全チェック プレイヤーの情報
    auto playerMain = playerId->GetComponent<Player>();
    if (!playerMain)
        return;

    // 必殺技を選択していないとき
    if (!playerMain->GetSpecialAction())
    {
        // 安全チェック UIコマンド　点滅用
        auto sharedUiCommandSpecialUnCheckId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandSpecial);
        if (!sharedUiCommandSpecialUnCheckId)
            return;

        // UIコマンド　点滅用 表示、非表示切り替え用
        auto uiIdSpecialUnCheckUi = sharedUiCommandSpecialUnCheckId->GetComponent<Ui>();
        auto uiIdSpecialUnCheckTransform2D = sharedUiCommandSpecialUnCheckId->GetComponent<TransForm2D>();

        // 安全チェック
        if (!uiIdSpecialUnCheckUi && !uiIdSpecialUnCheckTransform2D)
            return;

        // 特殊技溜まって無かったら コマンドUI点滅しない
        if (!playerMain->GetSpecialAttack((int)Player::SpecialAttackType::Attack) &&
            !playerMain->GetSpecialAttack((int)Player::SpecialAttackType::MagicFire))
        {
            // UI非表示
            uiIdSpecialUnCheckUi->SetAlpha(PlayerConfig::halfAlpha);
            return;
        }
        
        // 特殊攻撃たまった 知らせるために コマンドUI点滅
        if (!mathfPintch.UpdateElapsedTime(timeElapsedHintMax, elapsedTime))return;

        // 点滅
        isUiSpecilDrawCheck = isUiSpecilDrawCheck ? false : true;

        // 点滅用　コマンド変換値 元画像
        DirectX::XMFLOAT2 blinkTexPos = isUiSpecilDrawCheck ? CommandConfig::commandSelectTexScale : CommandConfig::commandUnSelectTexScale;

        // 選択、非選択
        uiIdSpecialUnCheckTransform2D->SetTexPosition(blinkTexPos);
        return;
    }


    // コマンド　必殺技選択中
    switch (playerMain->GetSpecialAttackNum())
    {
    case (int)Player::SpecialAttackType::Attack:
    {
        // 必殺技切りつけ
        auto sharedUiSpecialShurashuId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandSpeciulShurashu);
        // 安全チェック
        if (!sharedUiSpecialShurashuId)
            return;

        // コマンドUI斬撃　トランスフォーム
        auto uiIdSpecialAttackTransForm2D = sharedUiSpecialShurashuId->GetComponent<TransForm2D>();
        auto uiIdSpecialAttack = sharedUiSpecialShurashuId->GetComponent<Ui>();
        // 安全チェック
        if (!uiIdSpecialAttack || !uiIdSpecialAttackTransForm2D)
            return;

        // 必殺技炎
        // 安全チェック
        auto sharedUiCommandSpeciulFrameId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandSpeciulFrame);
        if (!sharedUiCommandSpeciulFrameId)
            return;

        // コマンドUI火　トランスフォーム
        auto uiIdSpecialFireTransForm2D = sharedUiCommandSpeciulFrameId->GetComponent<TransForm2D>();
        auto uiIdSpecialFire = sharedUiCommandSpeciulFrameId->GetComponent<Ui>();
        // 安全チェック
        if (!uiIdSpecialFireTransForm2D || !uiIdSpecialFire)
            return;

        float pos = uiIdSpecialAttackTransForm2D->GetPosition().y + PlayerConfig::offset;
        
        // コマンド選択判断　透明度
        float selectAlpha = CommandConfig::commandAlphaSelect;
        // コマンド非選択判断　透明度
        if (!playerMain->GetSpecialAttack((int)Player::SpecialAttackType::Attack))
            selectAlpha = CommandConfig::commandAlphaUnSelect;

        // 必殺技コマンド切りつけ 選択コマンドに変更
        uiIdSpecialAttackTransForm2D->SetTexPosition(CommandConfig::commandSelectTexPos);

        // 非選択コマンドに変更 炎
        uiIdSpecialFireTransForm2D->SetTexPosition(CommandConfig::commandUnSelectTexPos);

        // 非選択状態半透明
        uiIdSpecialFire->SetAlpha(CommandConfig::commandAlphaUnSelect);
        // 選択状態不透明
        uiIdSpecialAttack->SetAlpha(selectAlpha);
        break;
    }
    case (int)Player::SpecialAttackType::MagicFire:
    {
        // 必殺技炎
        auto sharedUiCommandSpeciulFrameId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandSpeciulFrame);
        // 安全チェック
        if (!sharedUiCommandSpeciulFrameId)
            return;

        // コマンドUI火　トランスフォーム
        auto uiIdSpecialFireTransForm2D = sharedUiCommandSpeciulFrameId->GetComponent<TransForm2D>();
        auto uiIdSpecialFire = sharedUiCommandSpeciulFrameId->GetComponent<Ui>();
        // 安全チェック
        if (!uiIdSpecialFireTransForm2D || !uiIdSpecialFire)
            return;

        // 必殺技切りつけ
        // 安全チェック
        auto sharedUiSpecialShurashuId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandSpeciulShurashu);
        if (!sharedUiSpecialShurashuId)
            return;

        // コマンドUI斬撃　トランスフォーム
        auto uiIdSpecialAttackTransForm2D = sharedUiSpecialShurashuId->GetComponent<TransForm2D>();
        auto uiIdSpecialAttack = sharedUiSpecialShurashuId->GetComponent<Ui>();
        // 安全チェック
        if (!uiIdSpecialAttackTransForm2D || !uiIdSpecialAttack)
            return;

        float pos = uiIdSpecialFireTransForm2D->GetPosition().y + PlayerConfig::offset;

        // コマンド選択判断　透明度
        float selectAlpha = CommandConfig::commandAlphaSelect;
        // コマンド非選択判断　透明度
        if (!playerMain->GetSpecialAttack((int)Player::SpecialAttackType::MagicFire))
            selectAlpha = CommandConfig::commandAlphaUnSelect;

        // 非選択選択コマンドに変更
        uiIdSpecialAttackTransForm2D->SetTexPosition(CommandConfig::commandUnSelectTexPos);

        // 選択コマンドに変更 炎
        uiIdSpecialFireTransForm2D->SetTexPosition(CommandConfig::commandSelectTexPos);

        // 選択状態不透明
        uiIdSpecialFire->SetAlpha(selectAlpha);
        // 非選択状態半透明
        uiIdSpecialAttack->SetAlpha(CommandConfig::commandAlphaUnSelect);
        break;
    }
    case (int)Player::SpecialAttackType::MagicIce:
    {
        // 実装前

        // 氷
        // 安全チェック
        auto sharedUiCommandSpeciulIceId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandSpeciulIce);
        if (!sharedUiCommandSpeciulIceId)
            return;

        auto uiIdSpecialIce = sharedUiCommandSpeciulIceId->GetComponent<Ui>();
        auto uiIdSpecialIceTransForm2D = sharedUiCommandSpeciulIceId->GetComponent<TransForm2D>();
        // 安全チェック
        if (!uiIdSpecialIce || !uiIdSpecialIceTransForm2D)
            return;

        break;
    }
    case (int)Player::SpecialAttackType::MagicThander:
    {
        // 実装前

        // 雷
        // 安全チェック
        auto sharedUiCommandSpeciulThanderId = UiManager::Instance().GetUies(
            (int)UiManager::UiCount::PlayerCommandSpeciulThander);
        if (!sharedUiCommandSpeciulThanderId)
            return;

        auto uiIdSpecialThander = sharedUiCommandSpeciulThanderId->GetComponent<Ui>();
        auto uiIdSpecialThanderTransForm2D = sharedUiCommandSpeciulThanderId->GetComponent<TransForm2D>();
        // 安全チェック
        if (!uiIdSpecialThander || !uiIdSpecialThanderTransForm2D)
            return;

        break;
    }
    }
}

void UiLoadingManager::Register(std::shared_ptr<Actor> ui)
{
    uies.emplace_back(ui);
}

void UiLoadingManager::Clear()
{
    uies.clear();
}

void UiLoadingManager::Remove(std::shared_ptr<Actor> ui)
{
    removes.insert(ui);
}
