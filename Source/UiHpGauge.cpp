#include "UiHpGauge.h"
#include "Player.h"
void UiHpGauge::Start()
{
}

void UiHpGauge::Update(float elapsedTime)
{
    // hp�̃Q�[�W�ݒ�
    HpGaugeUpdate();
}

void UiHpGauge::Render2D(RenderContext& rc, SpriteShader& shader)
{
    // �摜
    Sprite* sprite = nullptr;

    // �ʒu
    std::shared_ptr<TransForm2D> transForm2D;

    // �摜
    sprite = GetActor()->GetComponent<SpriteControll>()->GetSprite();

    // �ʒu��
    transForm2D = GetActor()->GetComponent<TransForm2D>();

    // �ϊ��s��
    DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&rc.view);
    DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&rc.projection);
    // 2D
    if (drawCheck)
    {
        // �`��
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
// �Q�[�W�̌v�Z
void UiHpGauge::HpGaugeUpdate()
{
    PlayerManager& playerManager = PlayerManager::Instance();
    int playerCount = playerManager.GetPlayerCount();//todo �O
    // player���S
    if (playerCount <= 0) return;

    // ���S�`�F�b�N
    std::shared_ptr<Actor> player = playerManager.GetPlayer((int)PlayerManager::PlayerType::Main);
    if (!player)
        return;
    std::shared_ptr<HP> playerHp = player->GetComponent<HP>();

    // �_���[�W
    if (!playerHp->OnDamaged()) return;
    // ���S�`�F�b�N
    auto sharedId = GetActor();
    if (!sharedId)return;
    // �ʒu
    std::shared_ptr<TransForm2D> transForm2D = sharedId->GetComponent<TransForm2D>();
    float gaugeWidth = playerHp->GetMaxHealth() * playerHp->GetHealth() * 0.08f;
    scale = { gaugeWidth ,transForm2D->GetScale().y };
    transForm2D->SetScale(scale);

    // �h��
    transForm2D->Shake();
}
