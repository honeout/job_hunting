#include "UiManager.h"
#include "UiTime.h"

#include "Character\Player.h"
#include "Component\Collision.h"
#include "Component\HP.h"
#include "Input/Input.h"

void UiManager::Update(float elapsedTime)
{
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

// UiTimeUpの有無
bool UiManager::GetTimeUp(int uiNumber)
{
    if (GetUiesCount() < 1)
        return false;

    // 安全チェック
    auto sharedUiSpecialShurashuId = GetUies(uiNumber);
    if (!sharedUiSpecialShurashuId)
        return false;

    // 技確定
    auto uiIdSpecialTime = sharedUiSpecialShurashuId->GetComponent<UiTime>();

    if (!uiIdSpecialTime)
        return false;

    if (uiIdSpecialTime->GetTimeUp())
        return true;

    return false;
}

// UI選択
void UiManager::SelectTex()
{
    // 安全チェック
    auto sharedUiSpecialShurashuId = GetUies(
        (int)UiManager::UiCount::PlayerCommandSpeciulShurashu);
    if (!sharedUiSpecialShurashuId)
        return;

    // 技確定
    std::shared_ptr<Ui> uiIdSpecialShurashu = sharedUiSpecialShurashuId->GetComponent<Ui>();
    std::shared_ptr<TransForm2D> uiIdSpecialShurashuTransForm2D = sharedUiSpecialShurashuId->GetComponent<TransForm2D>();
    bool drawCheck = false;
    uiIdSpecialShurashu->SetDrawCheck(drawCheck);
    DirectX::XMFLOAT2 pos;
    pos = { uiIdSpecialShurashuTransForm2D->GetPosition() };
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
        auto sharedUiComandoAttackId = GetUies((int)UiManager::UiCount::PlayerCommandAttack);
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
        auto sharedUiComandoAttackId = GetUies((int)UiManager::UiCount::PlayerCommandAttack);
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
        auto sharedUiComandoMagickId = GetUies((int)UiManager::UiCount::PlayerCommandMagick);
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
        auto sharedUiComandoMagickId = GetUies((int)UiManager::UiCount::PlayerCommandMagick);
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

// UI表示
void UiManager::SelectDrawUi(int uiNumber)
{
    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandUi)
        return;

    // 描画
    commandUi->SetDrawCheck(CommandConfig::draw);
    // 透明度100％
    commandUi->SetAlpha(CommandConfig::commandAlphaSelect);
}

// UI点滅
void UiManager::SelectDrawUi(int uiNumber, float TimeAlphaValue, float TimeAlphaMax, float elapsedTime)
{
    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandUi)
        return;

    // 描画
    commandUi->SetDrawCheck(CommandConfig::draw);

    // 点灯処理
    if (mathfblinking.UpdateElapsedTime(TimeAlphaMax, elapsedTime))
        TimeAlpha = TimeAlpha < TimeAlphaMax ? CommandConfig::alphaMax : CommandConfig::alphaMin;

    // 点滅
    commandUi->SetAlpha(TimeAlpha);
}

// UI非表示
void UiManager::SelectNotDrawUi(int uiNumber)
{
    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandUi)
        return;

    // 描画していない
    commandUi->SetDrawCheck(CommandConfig::unDraw);
}

// UI魔法の選択
void UiManager::SelectUi(int uiNumber)
{
    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandTransform = sharedUiCommandId->GetComponent<TransForm2D>();
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandTransform || !commandUi)
        return;
    // ショートカット
    commandTransform->SetTexPosition(CommandConfig::commandSelectTexPos);
    // 描画していない
    commandUi->SetDrawCheck(CommandConfig::draw);
    // 透かしオフ
    commandUi->SetAlpha(CommandConfig::commandAlphaSelect);
}

// UI非選択
void UiManager::UnSelectUi(int uiNumber)
{
    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandTransform = sharedUiCommandId->GetComponent<TransForm2D>();
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandTransform || !commandUi)
        return;
    // ショートカット
    commandTransform->SetTexPosition(CommandConfig::commandUnSelectTexPos);
    // 描画していない
    commandUi->SetDrawCheck(CommandConfig::draw);
    // 透かしオン
    commandUi->SetAlpha(CommandConfig::commandAlphaUnSelect);
}

// ショートカットキー選択
void UiManager::SelectShortCutUi(int uiNumber)
{
    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandTransform = sharedUiCommandId->GetComponent<TransForm2D>();
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandTransform || !commandUi)
        return;
    // ショートカット
    commandTransform->SetTexPosition(CommandConfig::commandShortCutTexScale);
    // 描画していない
    commandUi->SetDrawCheck(CommandConfig::draw);
    // 透かしオフ
    commandUi->SetAlpha(CommandConfig::commandAlphaSelect);
}

// ショートカットキー非選択
void UiManager::UnSelectShortCutUi(int uiNumber)
{
    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandTransform = sharedUiCommandId->GetComponent<TransForm2D>();
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandTransform || !commandUi)
        return;
    // ショートカット
    commandTransform->SetTexPosition(CommandConfig::commandShortCutTexScale);
    // 描画していない
    commandUi->SetDrawCheck(CommandConfig::draw);
    // 透かしオン
    commandUi->SetAlpha(CommandConfig::commandAlphaUnSelect);
}

// Ui魔法チャージ動作開始
void UiManager::StartMagicUiCharge(int uiCommandNumber ,int uiNumber, int uiFrameNumber, int chargeNumber,float elapsedTime)
{
    // 安全チェックプレイヤー
    auto playerid = PlayerManager::Instance().GetPlayer(0);
    
    if (!playerid) 
        return;
    auto playerMain = playerid->GetComponent<Player>();
    auto playerMp = playerid->GetComponent<Mp>();
    // 安全チェック
    if (!playerMain || !playerMp)
        return;

    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandMagicId = GetUies(uiCommandNumber);
    auto sharedUiCommandId = GetUies(uiNumber);
    auto sharedUiFrameCommandId = GetUies(uiFrameNumber);
    auto sharedUiChargeCommandId = GetUies(chargeNumber);

    if (!sharedUiCommandMagicId ||!sharedUiCommandId|| !sharedUiFrameCommandId || !sharedUiChargeCommandId)
        return;
    auto commandMagicTransform = sharedUiCommandMagicId->GetComponent<TransForm2D>();
    auto commandTransform = sharedUiCommandId->GetComponent<TransForm2D>();
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    auto commandFrameTransform = sharedUiFrameCommandId->GetComponent<TransForm2D>();
    auto commandFrameUi = sharedUiFrameCommandId->GetComponent<Ui>();
    auto commandChargeTransform = sharedUiChargeCommandId->GetComponent<TransForm2D>();
    auto commandChargeUi = sharedUiChargeCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandTransform || !commandUi || !commandFrameTransform || !commandFrameUi || !commandChargeTransform || !commandChargeUi)
        return;

    // mp切れ
    if (playerMp->GetMpEmpth())
    {
        // 魔法チャージオフ
        playerMain->SetIsMagicChageEnd(false);
        // 魔法
        StartMagicUiFire(uiNumber, uiFrameNumber, chargeNumber);
        return;
    }

    // ゲージ溜め
    commandPushUiChargeTime += PlayerConfig::commandChargeAdd * elapsedTime;

    // ゲージの横最大値
    float gaugeSize = commandMagicTransform->GetScale().x - CommandConfig::mpBarOffset;

    // 溜め
    float gaugeWidth = gaugeSize * commandPushUiChargeTime * CommandConfig::gaugeWidthMax;

    // コマンド位置
    DirectX::XMFLOAT2 pos = commandMagicTransform->GetPosition();

    // 位置補正
    commandTransform->SetPosition(pos);
    commandFrameTransform->SetPosition(pos);
    commandChargeTransform->SetPosition(pos);

    // スケール変更
    commandTransform->SetScaleX(gaugeSize);
    commandFrameTransform->SetScaleX(gaugeWidth);
    commandChargeTransform->SetScaleX(gaugeSize);

    // 描画
    commandUi->SetDrawCheck(CommandConfig::draw);
    commandFrameUi->SetDrawCheck(CommandConfig::draw);
    commandChargeUi->SetDrawCheck(CommandConfig::draw);

    // 透かしオフ
    commandFrameUi->SetAlpha(CommandConfig::commandAlphaSelect);

    // 魔法チャージ完了
    if (gaugeWidth >= CommandConfig::chargeMagicGaugeWidthMax)
    {
        playerMain->SetIsMagicChageEnd(true);
        gaugeWidth = CommandConfig::chargeMagicGaugeWidthMax;
    }
}

// Ui魔法チャージ動作発射
void UiManager::StartMagicUiFire(int uiNumber, int uiFrameNumber, int chargeNumber)
{
    // 初期化
    commandPushUiChargeTime = 0.0f;

    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);
    auto sharedUiFrameCommandId = GetUies(uiFrameNumber);
    auto sharedUiChargeCommandId = GetUies(chargeNumber);

    if (!sharedUiCommandId || !sharedUiFrameCommandId || !sharedUiChargeCommandId)
        return;
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    auto commandFrameUi = sharedUiFrameCommandId->GetComponent<Ui>();
    auto commandChargeUi = sharedUiChargeCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandUi ||!commandFrameUi || !commandChargeUi)
        return;

    commandUi->SetDrawCheck(CommandConfig::unDraw);
    commandFrameUi->SetDrawCheck(CommandConfig::unDraw);
    commandChargeUi->SetDrawCheck(CommandConfig::unDraw);
}

// UIシェイク
void UiManager::ShakeModeTyme(int uiNumber, bool& shakeMode)
{
    // シェイクの有無
    if (!shakeMode) return;

    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandTransform = sharedUiCommandId->GetComponent<TransForm2D>();
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandTransform || !commandUi)
        return;

    // 描画している
    commandUi->SetDrawCheck(CommandConfig::draw);
    // 透かしオン
    commandUi->SetAlpha(CommandConfig::commandAlphaSelect);

    // 揺れ
    if (shakeMode)
    {
        commandTransform->Shake();
    }
    //　初期化
    if (commandTransform->GetShakeEnd())
    {
        shakeMode = false;
    }
}

// UIポジション更新
void UiManager::PositionUpdate(int uiNumber, DirectX::XMFLOAT2 pos)
{
    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandTransform = sharedUiCommandId->GetComponent<TransForm2D>();
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandTransform || !commandUi)
        return;

    // ショートカット
    commandTransform->SetPosition(pos);
    // 描画していない
    commandUi->SetDrawCheck(CommandConfig::draw);
    // 透かしオン
    commandUi->SetAlpha(CommandConfig::commandAlphaUnSelect);
}

void UiManager::PositionXUpdate(int uiNumber, float posX)
{
    // 安全チェック
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandTransform = sharedUiCommandId->GetComponent<TransForm2D>();
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandTransform || !commandUi)
        return;

    // ショートカット
    commandTransform->SetPositionX(posX);
    // 描画していない
    commandUi->SetDrawCheck(CommandConfig::draw);
    // 透かしオン
    commandUi->SetAlpha(CommandConfig::commandAlphaUnSelect);
}

void UiManager::PositionYUpdate(int uiNumber, float posY)
{
    // 安全チェック
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandTransform = sharedUiCommandId->GetComponent<TransForm2D>();
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandTransform || !commandUi)
        return;

    // ショートカット
    commandTransform->SetPositionY(posY);
    // 描画していない
    commandUi->SetDrawCheck(CommandConfig::draw);
    // 透かしオン
    commandUi->SetAlpha(CommandConfig::commandAlphaUnSelect);
}

// UI大きさ更新
void UiManager::ScaleUpdate(int uiNumber, DirectX::XMFLOAT2 scale)
{
    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandTransform = sharedUiCommandId->GetComponent<TransForm2D>();
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandTransform || !commandUi)
        return;

    // ショートカット
    commandTransform->SetScale(scale);
    // 描画していない
    commandUi->SetDrawCheck(CommandConfig::draw);
}

// UI大きさX
void UiManager::ScaleUpdateX(int uiNumber, float scaleX)
{
    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandTransform = sharedUiCommandId->GetComponent<TransForm2D>();
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandTransform || !commandUi)
        return;

    // ショートカット
    commandTransform->SetScaleX(scaleX);
    // 描画していない
    commandUi->SetDrawCheck(CommandConfig::draw);
}

// UI大きさY
void UiManager::ScaleUpdateY(int uiNumber, float scaleY)
{
    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandTransform = sharedUiCommandId->GetComponent<TransForm2D>();
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandTransform || !commandUi)
        return;

    // ショートカット
    commandTransform->SetScaleY(scaleY);
    // 描画していない
    commandUi->SetDrawCheck(CommandConfig::draw);
}

// UI元画像の位置を変える。
void UiManager::TexPosUpdate(int uiNumber, DirectX::XMFLOAT2 texPos)
{
    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandTransform = sharedUiCommandId->GetComponent<TransForm2D>();
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandTransform || !commandUi)
        return;

    // 描画していない
    commandUi->SetDrawCheck(CommandConfig::draw);

    // 元画像位置
    commandTransform->SetTexPosition(texPos);
}

// フェードアウト
void UiManager::IncrementToAlpha(int uiNumber, float increment)
{
    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;

    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandUi)
        return;
    // フェードアウト
    commandUi->IncrementToAlpha(increment);
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
        auto sharedUiSpecialId = GetUies((int)UiManager::UiCount::PlayerCommandSpecial);
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
        auto sharedUiSpecialId = GetUies((int)UiManager::UiCount::PlayerCommandSpecial);
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
        auto sharedUiComandoAttackId = GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu);
        if (!sharedUiComandoAttackId)
            return;
        // コマンド　必殺技斬撃表示するか
        auto uiIdSpecialComandoAttack = sharedUiComandoAttackId->GetComponent<Ui>();
        // 安全チェック
        if (!uiIdSpecialComandoAttack) return;

        // 安全チェック コマンド魔法炎
        auto sharedUiCommandSpecialFireId = GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame);
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
        auto sharedUiComandoAttackId = GetUies((int)UiManager::UiCount::PlayerCommandSpeciulShurashu);
        if (!sharedUiComandoAttackId)
            return;
        auto uiIdAttack = sharedUiComandoAttackId->GetComponent<Ui>();

        // 安全チェック
        if (!uiIdAttack)
            return;

        // 安全チェック コマンド魔法炎
        auto sharedUiComandoFireId = GetUies((int)UiManager::UiCount::PlayerCommandSpeciulFrame);
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
    int uiCount = GetUiesCount();

    // uiCount最大値
    int uiCountMax = 5;

    // ui無かったら
    if (uiCount <= uiCountMax) return;
    // 安全チェック
    auto sharedUiSpecialChargeFurstId = GetUies(
        (int)UiManager::UiCount::PlayerCommandSpeciulCharge01);
    auto sharedUiSpecialChargeSecondId = GetUies(
        (int)UiManager::UiCount::PlayerCommandSpeciulCharge02);
    auto sharedUiSpecialChargeSerdeId = GetUies(
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
        auto sharedUiCommandSpecialUnCheckId = GetUies(
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
        if (!mathfSpecial.UpdateElapsedTime(timeElapsedHintMax, elapsedTime))return;

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
        auto sharedUiSpecialShurashuId = GetUies(
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
        auto sharedUiCommandSpeciulFrameId = GetUies(
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
        auto sharedUiCommandSpeciulFrameId = GetUies(
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
        auto sharedUiSpecialShurashuId = GetUies(
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
        auto sharedUiCommandSpeciulIceId = GetUies(
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
        auto sharedUiCommandSpeciulThanderId = GetUies(
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

// 魔法コマンド選択と入力
void UiManager::InputSelectMagicCommand()
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    int playerKinds = PlayerManager::Instance().GetPlayerCount() - 1;

    // 安全チェック プレイヤー
    auto playerId = PlayerManager::Instance().GetPlayer(playerKinds);
    if (!playerId)
        return;

    // 安全チェック プレイヤーの情報
    auto playerMain = playerId->GetComponent<Player>();
    if (!playerMain)
        return;
    ///////////////////////////
    // ショートカットキー

    // ショートカットキー炎選択
    if (playerMain->InputShortCutkeyMagic() &&
        gamePad.GetButton() & GamePad::BTN_B)
    {
        // コマンドショートカット火
        SelectShortCutUi((int)UiManager::UiCount::PlayerCommandFire);
    }
    // ショートカットキー雷選択
    if (playerMain->InputShortCutkeyMagic() &&
        gamePad.GetButton() & GamePad::BTN_X)
    {
        // コマンドショートカット雷
        SelectShortCutUi((int)UiManager::UiCount::PlayerCommandRigtning);
    }
    // ショートカットキー氷選択
    if (playerMain->InputShortCutkeyMagic() &&
        gamePad.GetButton() & GamePad::BTN_A)
    {
        // コマンドショートカット氷
        SelectShortCutUi((int)UiManager::UiCount::PlayerCommandIce);
    }
    // ショートカットキー回復選択
    if (playerMain->InputShortCutkeyMagic() &&
        gamePad.GetButton() & GamePad::BTN_Y)
    {
        // コマンドショートカット回復
        SelectShortCutUi((int)UiManager::UiCount::PlayerCommandHeale);
    }

    /////////////////////////

    // コマンド魔法選択では無かったら
    if (!playerMain->GetMagicAction())
    {
        playerMain->SetSelectMagicCheck((int)Player::CommandMagic::Normal);
    }
    int uiCount = GetUiesCount();

    // UI設定 コマンド選択　魔法　炎 
    if (playerMain->GetSelectCheck() == (int)Player::CommandMagic::Fire && 
        playerMain->GetMagicAction() &&
        !playerMain->InputShortCutkeyMagic())
    {
        SelectUi((int)UiManager::UiCount::PlayerCommandFire);
    }
    // UI設定 コマンド 非選択　魔法　炎 
    else if (!playerMain->InputShortCutkeyMagic())
    {
        UnSelectUi((int)UiManager::UiCount::PlayerCommandFire);
    }
    // 魔法を発動していなかったら　ショートカットなら　十字キー操作UI解除
    if (!playerMain->GetMagicAction())
    {
        SelectNotDrawUi((int)UiManager::UiCount::PlayerCommandFire);
    }
    // UI設定 コマンド選択　魔法　雷 
    if (playerMain->GetSelectMagicCheck() == (int)Player::CommandMagic::Thander 
        && playerMain->GetMagicAction() &&
        !playerMain->InputShortCutkeyMagic())
    {
        SelectUi((int)UiManager::UiCount::PlayerCommandRigtning);
    }
    // UI設定 コマンド 非選択　魔法　雷 
    else if (!playerMain->InputShortCutkeyMagic())
    {
        UnSelectUi((int)UiManager::UiCount::PlayerCommandRigtning);
    }
    // 魔法を発動していなかったら　ショートカットなら　十字キー操作UI解除
    if (!playerMain->GetMagicAction())
    {
        SelectNotDrawUi((int)UiManager::UiCount::PlayerCommandRigtning);
    }
    // UI設定 氷
    if (playerMain->GetSelectMagicCheck() == (int)Player::CommandMagic::Ice 
        && playerMain->GetMagicAction() &&
        !playerMain->InputShortCutkeyMagic())
    {
        SelectUi((int)UiManager::UiCount::PlayerCommandIce);
    }
    // UI設定 コマンド 非選択　魔法　氷
    else if (!playerMain->InputShortCutkeyMagic())
    {
        UnSelectUi((int)UiManager::UiCount::PlayerCommandIce);
    }
    // 魔法を発動していなかったら　ショートカットなら　十字キー操作UI解除
    if (!playerMain->GetMagicAction())
    {
        SelectNotDrawUi((int)UiManager::UiCount::PlayerCommandIce);
    }
    // UI設定 回復
    if (playerMain->GetSelectMagicCheck() == (int)Player::CommandMagic::Heale 
        && playerMain->GetMagicAction() &&
        !playerMain->InputShortCutkeyMagic())
    {
        SelectUi((int)UiManager::UiCount::PlayerCommandHeale);
    }
    // UI設定 コマンド 非選択　魔法　回復 
    else if (!playerMain->InputShortCutkeyMagic())
    {
        UnSelectUi((int)UiManager::UiCount::PlayerCommandHeale);
    }
    // 魔法を発動していなかったら　ショートカットなら　十字キー操作UI解除
    if (!playerMain->GetMagicAction())
    {
        SelectNotDrawUi((int)UiManager::UiCount::PlayerCommandHeale);
    }
}

// 必殺技チャージUI
void UiManager::SpecialChargeChack(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    int playerKinds = PlayerManager::Instance().GetPlayerCount() - 1;

    // 安全チェック プレイヤー
    auto playerId = PlayerManager::Instance().GetPlayer(playerKinds);
    if (!playerId)
        return;

    // 安全チェック プレイヤーの情報
    auto playerMain = playerId->GetComponent<Player>();
    if (!playerMain)
        return;

    // 消す
    if (playerMain->GetSpecialAttackCharge() < 0.4f)
    {
        // チャージ矢印1
        SelectNotDrawUi((int)UiManager::UiCount::PlayerCommandSpeciulCharge01);
        // チャージ矢印2
        SelectNotDrawUi((int)UiManager::UiCount::PlayerCommandSpeciulCharge02);
        // チャージ矢印3
        SelectNotDrawUi((int)UiManager::UiCount::PlayerCommandSpeciulCharge03);
    }

    // チャージたまる
    // チャージを見やすく
    if (playerMain->GetSpecialAttackCharge() >= 
        CommandConfig::specialAttackChargeStart && 
        playerMain->GetSpecialAttackCharge() <= CommandConfig::specialAttackChargeFirst)
    {
        // チャージ矢印
        SelectDrawUi((int)UiManager::UiCount::PlayerCommandSpeciulCharge01,
            CommandConfig::commandAlphaValue, CommandConfig::commandAlphaValueMax, elapsedTime);
    }
    // コマンド点灯　alpha100%
    else if (playerMain->GetSpecialAttackCharge() > CommandConfig::specialAttackChargeFirst)
    {
        // チャージ矢印
        SelectDrawUi((int)UiManager::UiCount::PlayerCommandSpeciulCharge01);
    }
    // チャージを見やすく
    if (playerMain->GetSpecialAttackCharge() > CommandConfig::specialAttackChargeFirst && playerMain->GetSpecialAttackCharge() <= CommandConfig::specialAttackChargeSecond)
    {
        // チャージ矢印
        SelectDrawUi((int)UiManager::UiCount::PlayerCommandSpeciulCharge02,
            CommandConfig::commandAlphaValue, CommandConfig::commandAlphaValueMax, elapsedTime);
    }
    // コマンド点灯　alpha100%
    else if (playerMain->GetSpecialAttackCharge() > CommandConfig::specialAttackChargeSecond)
    {
        // チャージ矢印
        SelectDrawUi((int)UiManager::UiCount::PlayerCommandSpeciulCharge02);
    }
    // チャージを見やすく
    if (playerMain->GetSpecialAttackCharge() > CommandConfig::specialAttackChargeSecond)
    {
        // チャージ矢印
        SelectDrawUi((int)UiManager::UiCount::PlayerCommandSpeciulCharge03,
            CommandConfig::commandAlphaValue, CommandConfig::commandAlphaValueMax, elapsedTime);
    }
    // コマンド点灯　alpha100%
    else if (playerMain->GetSpecialAttackCharge() >= PlayerConfig::specialAttackChargeMax)
    {
        // チャージ矢印
        SelectDrawUi((int)UiManager::UiCount::PlayerCommandSpeciulCharge03);
    }
}

// ピンチコマンドゲージ処理
bool UiManager::PinchModeCommandGage(int uiNumber,std::shared_ptr<HP> hpid, float elapsedTime)
{
    // HP変化色
    TexPosUpdate(uiNumber, CommandConfig::hpBarGreenwTexPos);
    // hp半分
    if (hpid->HealthHalf() && !hpid->GetDead())
    {
        // HP変化色
        TexPosUpdate(uiNumber, CommandConfig::hpBarYerowTexPos);
        return false;
    }

    // hp が一定以下なら
    if (hpid->HealthPinch() && !hpid->GetDead())
    {
        // HP変化色
        TexPosUpdate(uiNumber, CommandConfig::hpBarRedTexPos);

        return true;
    }
    return false;
}

void UiManager::UiHpControllePlayerGauge(int uiNumber, int uiNumberGage, DirectX::XMFLOAT2 texNoDamagePos, DirectX::XMFLOAT2 texDamagePos, std::shared_ptr<HP> hpId, float elapsedTime)
{
    // hpゲージ操作用
    float gaugeWidth;

    // ゲージの大きさ
    gaugeWidth = mathfPintch.LinearInterpolate((float)hpId->GetHealth(), (float)hpId->GetMaxHealth(), CommandConfig::lerpSpeed, elapsedTime);

    // 大きさ補正
    gaugeWidth *= CommandConfig::gaugeScaleMax;

    // hpバー最低値
    if (gaugeWidth <= CommandConfig::gaugeWidthMin)
        gaugeWidth = CommandConfig::gaugeWidthMin;

    // 経過時間
    if (this->timePlayerDamageValue > CommandConfig::timeMin)
    {
        // uiの周りをダメージように切り替える。
        TexPosUpdate(uiNumber, texDamagePos);
        // ダメージ食らってる時のUI
        this->timePlayerDamageValue -= CommandConfig::timeValue * elapsedTime;

        // 時間内常に揺らしたい
        bool shakeMode = true;

        // 揺れ
        ShakeModeTyme(uiNumber, shakeMode);
        ShakeModeTyme(uiNumberGage, shakeMode);
    }
    // ダメ終了
    else
    {
        // uiの周りをダメージ食らっていない時
        TexPosUpdate(uiNumber, texNoDamagePos);
        this->timePlayerDamageValue = CommandConfig::timeMin;
    }
    // ダメージ食らった瞬間
    if (hpId->OnDamaged())
    {
        // ダメージ食らった時間を継続
        this->timePlayerDamageValue = CommandConfig::timeMax;
    }

    // hpゲージUI　変える
    ScaleUpdateX(uiNumberGage, gaugeWidth);
}

void UiManager::UiHpControlleEnemyGauge(int uiNumber, int uiNumberGage, DirectX::XMFLOAT2 texNoDamagePos, DirectX::XMFLOAT2 texDamagePos, std::shared_ptr<HP> hpId, float elapsedTime)
{
    // hpゲージ操作用
    float gaugeWidth;

    // ゲージの大きさ
    gaugeWidth = mathfPintch.LinearInterpolate((float)hpId->GetHealth(), (float)hpId->GetMaxHealth(), CommandConfig::lerpSpeed, elapsedTime);

    // 大きさ補正
    gaugeWidth *= CommandConfig::gaugeScaleMax;

    // hpバー最低値
    if (gaugeWidth <= CommandConfig::gaugeWidthMin)
        gaugeWidth = CommandConfig::gaugeWidthMin;

    // 経過時間
    if (this->timeEnemyDamageValue > CommandConfig::timeMin)
    {
        // uiの周りをダメージように切り替える。
        TexPosUpdate(uiNumber, texDamagePos);
        // ダメージ食らってる時のUI
        this->timeEnemyDamageValue -= CommandConfig::timeValue * elapsedTime;

        // 時間内常に揺らしたい
        bool shakeMode = true;

        // 揺れ
        ShakeModeTyme(uiNumber, shakeMode);
        ShakeModeTyme(uiNumberGage, shakeMode);
    }
    // ダメ終了
    else
    {
        // uiの周りをダメージ食らっていない時
        TexPosUpdate(uiNumber, texNoDamagePos);
        this->timeEnemyDamageValue = CommandConfig::timeMin;
    }
    // ダメージ食らった瞬間
    if (hpId->OnDamaged())
    {
        // ダメージ食らった時間を継続
        this->timeEnemyDamageValue = CommandConfig::timeMax;
    }

    // hpゲージUI　変える
    ScaleUpdateX(uiNumberGage, gaugeWidth);
}

// UI MPゲージ
void UiManager::UiMpControlleGauge(int uiNumberGage, std::shared_ptr<Mp> mpId, float elapsedTime)
{
    // mpゲージ操作用
    float gaugeWidth;

    // ゲージの大きさ
    gaugeWidth = mathfMp.LinearInterpolate((float)mpId->GetMagic(), (float)mpId->GetMaxMagic(), CommandConfig::lerpSpeed, elapsedTime);
    // 大きさ補正
    gaugeWidth *= CommandConfig::gaugeScaleMax;
    
    // mpバー最低値
    if (gaugeWidth <= CommandConfig::gaugeWidthMin)
        gaugeWidth = CommandConfig::gaugeWidthMin;

    // mpゲージUI　変える
    ScaleUpdateX((int)UiManager::UiCount::Mp, gaugeWidth);

    // mp透けていない
    SelectDrawUi((int)UiManager::UiCount::Mp);

    // mp切れ
    if (mpId->GetMpEmpth())
    {
        // mp透けてる
        SelectNotDrawUi((int)UiManager::UiCount::Mp);
    }

    // 透明度設定
    SelectDrawUi((int)UiManager::UiCount::Mp);
}

// ロックオンUI処理
void UiManager::RockOnUI(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
    // ビューポート 画面のサイズ等
    // ビューポートとは2Dの画面に描画範囲の指定(クリッピング指定も出来る)位置を指定
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    // ラスタライザーステートにバインドされているビューポート配列を取得
    dc->RSGetViewports(&numViewports, &viewport);
    // 変換行列
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
    // ローカルからワールドに行くときにいる奴相手のポジションを渡す。
    DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

    int playerKinds = PlayerManager::Instance().GetPlayerCount() - 1;

    // 安全チェック プレイヤー
    auto playerId = PlayerManager::Instance().GetPlayer(playerKinds);
    if (!playerId)
        return;

    // 安全チェック プレイヤーの情報
    auto playerMain = playerId->GetComponent<Player>();
    if (!playerMain)
        return;

    // 全ての敵の頭上にHPゲージを表示
    EnemyManager& enemyManager = EnemyManager::Instance();
    // エネミーの数
    int enemyCount = enemyManager.GetEnemyCount();
    // エネミーが一人でも生きていたら
    if (enemyCount <= 0) return;
    // エネミーの安全☑
    auto enemyShader = enemyManager.GetEnemy((int)EnemyManager::EnemyType::Boss);
    if (!enemyShader) return;

    auto enemyBoss = enemyShader->GetComponent<EnemyBoss>();
    auto enemyModel = enemyShader->GetComponent<ModelControll>();
    if (!enemyBoss || !enemyModel) return;

    Model::Node* characterBorn = enemyModel->GetModel()->FindNode("boss_right_eye");
    // エネミー腰位置
    DirectX::XMFLOAT3 enemyPosition;

    enemyPosition = enemyModel->GetModel()->ConvertLocalToWorld(characterBorn);

    // ワールドからスクリーン
    DirectX::XMVECTOR enemyPositionVe = DirectX::XMLoadFloat3(&enemyPosition);
    // ゲージ描画 // ワールドからスクリーン座標に
    DirectX::XMVECTOR screenPositionVe = DirectX::XMVector3Project(
        enemyPositionVe,
        viewport.TopLeftX,
        viewport.TopLeftY,
        viewport.Width,
        viewport.Height,
        viewport.MinDepth,
        viewport.MaxDepth,
        Projection,
        View,
        World
    );
    // スクリーン座標
    DirectX::XMFLOAT3 scereenPosition;
    DirectX::XMStoreFloat3(&scereenPosition, screenPositionVe);
    // 必殺技がでていなかったらロックオン
    if (playerMain->GetRockCheck() || !playerMain->GetSpecialRockOff())
    {
        PositionUpdate((int)UiManager::UiCount::Sight,
            {
                scereenPosition.x,
                scereenPosition.y
            });

        PositionUpdate((int)UiManager::UiCount::SightCheck,
            {
                scereenPosition.x - CommandConfig::scereenPositionOffset.x,
                scereenPosition.y - CommandConfig::scereenPositionOffset.y
            });
    }
    // 必殺技中ロックオン系UIを消す。
    if (scereenPosition.z < 0.0f || scereenPosition.z > 1.0f || !playerMain->GetRockCheck()|| playerMain->GetSpecialRockOff())
    {
        SelectNotDrawUi((int)UiManager::UiCount::Sight);
        SelectNotDrawUi((int)UiManager::UiCount::SightCheck);
        return;
    }
}

// 距離でUIを変えるロックオン中
void UiManager::AttackCheckUI()
{
    int playerKinds = PlayerManager::Instance().GetPlayerCount() - 1;

    // 安全チェック プレイヤー
    auto playerId = PlayerManager::Instance().GetPlayer(playerKinds);
    if (!playerId)
        return;

    // 安全チェック プレイヤーの情報
    auto playerMain = playerId->GetComponent<Player>();
    auto playerTransform = playerId->GetComponent<Transform>();
    if (!playerMain || !playerTransform)
        return;

    // ui無かったら
    if ( !playerMain->GetRockCheck() || playerMain->GetSpecialRockOff()) return;
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; ++i)
    {
        auto enemy = enemyManager.GetEnemy(i);
        // 安全チェック
        if (!enemy) return;

        auto enemyTransform = enemy->GetComponent<Transform>();
        // 安全チェック
        if (!enemyTransform)return;
        DirectX::XMVECTOR playerPosition =
            DirectX::XMLoadFloat3(&playerTransform->GetPosition());
        DirectX::XMFLOAT3 enemyPosition = enemyTransform->GetPosition();
        DirectX::XMVECTOR enemyPositionXM =
            DirectX::XMLoadFloat3(&enemyPosition);
        DirectX::XMVECTOR LengthSq =
            DirectX::XMVectorSubtract(playerPosition, enemyPositionXM);
        LengthSq = DirectX::XMVector3LengthSq(LengthSq);
        float lengthSq;
        DirectX::XMStoreFloat(&lengthSq, LengthSq);
        switch (playerMain->GetSelectCheck())
        {
            // 近距離の時の射程距離によるUI表示
        case (int)Player::CommandAttack::Attack:
        {
            // 当たり判定距離
            if (lengthSq < PlayerConfig::attackCheckRange)
            {
                // 描画ロックオン
                SelectDrawUi((int)UiManager::UiCount::SightCheck);
            }
            else
            {
                // 非表示ロックオン
                SelectNotDrawUi((int)UiManager::UiCount::SightCheck);
            }

            break;
        }
        // 魔法の時の射程距離によるUI表示
        case (int)Player::CommandAttack::Magic:
        {
            if (lengthSq < PlayerConfig::magicRangeLength)
            {
                // 描画ロックオン
                SelectDrawUi((int)UiManager::UiCount::SightCheck);
            }
            else
            {
                // 非表示ロックオン
                SelectNotDrawUi((int)UiManager::UiCount::SightCheck);
            }

            break;
        }
        default:
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

void UiLoadingManager::SelectDrawUi(int uiNumber)
{
    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandUi)
        return;

    // 描画
    commandUi->SetDrawCheck(CommandConfig::draw);
    // 透明度100％
    commandUi->SetAlpha(CommandConfig::commandAlphaSelect);
}

void UiLoadingManager::SelectDrawUi(int uiNumber, float TimeAlpha, float TimeAlphaMax, float elapsedTime)
{
    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandUi)
        return;

    // 描画
    commandUi->SetDrawCheck(CommandConfig::draw);

    // 点灯処理
    if (mathfblinking.UpdateElapsedTime(TimeAlphaMax, elapsedTime))
        TimeAlpha = TimeAlpha < TimeAlphaMax ? 1.0f : 0.0f;

    // 点滅
    commandUi->SetAlpha(TimeAlpha);
}

void UiLoadingManager::SelectNotDrawUi(int uiNumber)
{
    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandUi)
        return;

    // 描画していない
    commandUi->SetDrawCheck(CommandConfig::unDraw);
}

void UiLoadingManager::SelectUi(int uiNumber)
{
    // 安全チェック　コマンド シ
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandTransform = sharedUiCommandId->GetComponent<TransForm2D>();
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandTransform || !commandUi)
        return;
    // ショートカット
    commandTransform->SetTexPosition(CommandConfig::commandSelectTexPos);
    // 描画していない
    commandUi->SetDrawCheck(CommandConfig::draw);
    // 透かしオフ
    commandUi->SetAlpha(CommandConfig::commandAlphaSelect);
}

void UiLoadingManager::UnSelectUi(int uiNumber)
{
    // 安全チェック　コマンド ショートカット　魔法　火
    auto sharedUiCommandId = GetUies(uiNumber);

    if (!sharedUiCommandId)
        return;
    auto commandTransform = sharedUiCommandId->GetComponent<TransForm2D>();
    auto commandUi = sharedUiCommandId->GetComponent<Ui>();
    // 安全チェック
    if (!commandTransform || !commandUi)
        return;
    // ショートカット
    commandTransform->SetTexPosition(CommandConfig::commandUnSelectTexPos);
    // 描画していない
    commandUi->SetDrawCheck(CommandConfig::draw);
    // 透かしオン
    commandUi->SetAlpha(CommandConfig::commandAlphaUnSelect);
}

