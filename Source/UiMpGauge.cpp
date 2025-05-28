#include "UiMpGauge.h"
#include "Player.h"
void UiMpGauge::Start()
{
}

void UiMpGauge::Update(float elapsedTime)
{
    // mpのゲージ設定
    MpGaugeUpdate();
}

void UiMpGauge::Render2D(RenderContext& rc, SpriteShader& shader)
{
    // 画像
    Sprite* sprite = nullptr;

    // 位置
    std::shared_ptr<TransForm2D> transForm2D;

    // 画像
    sprite = GetActor()->GetComponent<SpriteControll>()->GetSprite();
    // 位置等
    transForm2D = GetActor()->GetComponent<TransForm2D>();


    // 変換行列
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&rc.view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&rc.projection);
    // 2D
    if (drawCheck)
    {
        // 描画
        sprite->Render(rc.deviceContext,
            transForm2D->GetPosition().x,
            transForm2D->GetPosition().y
            , transForm2D->GetScale().x
            , transForm2D->GetScale().y,
            transForm2D->GetTexPosition().x, transForm2D->GetTexPosition().y,
            transForm2D->GetTexScale().x > 0 ? transForm2D->GetTexScale().x : static_cast<float> (sprite->GetTextureWidth()),
            transForm2D->GetTexScale().y > 0 ? transForm2D->GetTexScale().y : static_cast<float> (sprite->GetTextureHeight()),
            transForm2D->GetAngle(),
            color.x, color.y, color.z, color.w);
    }
}
// ゲージの計算
void UiMpGauge::MpGaugeUpdate()
{
    PlayerManager& playerManager = PlayerManager::Instance();
    int playerCount = playerManager.GetPlayerCount();//todo 外
    // player死亡
    if (playerCount <= 0) return;

    // 安全チェック
    std::shared_ptr<Actor> player = playerManager.GetPlayer((int)PlayerManager::PlayerType::Main);
    if (!player)
        return;
    std::shared_ptr<Mp> playerMp = player->GetComponent<Mp>();
    // ｍｐ消費
    if (!playerMp->GetIsConsumption()) return;
    // 安全チェック
    auto sharedId = GetActor();
    if (!sharedId)return;
    // 位置
    std::shared_ptr<TransForm2D> transForm2D = sharedId->GetComponent<TransForm2D>();
    float gaugeWidth = playerMp->GetMaxMagic() * playerMp->GetMagic() * 0.08f;
    scale = { gaugeWidth ,transForm2D->GetScale().y };
    transForm2D->SetScale(scale);
}
