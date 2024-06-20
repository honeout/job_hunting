#include <imgui.h>
#include "BulletFiring.h"

void BulletFiring::Move(float speed,float elapsedTime)
{
    //　寿命処理 
    lifeTimer -= elapsedTime;
    if (lifeTimer <= 0.0f)// 寿命が尽きたら自害
    {
        // 自分を削除
        Destroy();
    }
    // 移動　　秒じゃなくとフレームに
    float bulletspeed = speed * elapsedTime;

    // 　だからかけ算で向きにこれだけ進だから位置に入れた
    position.x += bulletspeed * direction.x;
    position.y += bulletspeed * direction.y;
    position.z += bulletspeed * direction.z;

    GetActor()->SetPosition(position);
    

    // 反射
    //UpdateReflection(elapsedTime);

    // オブジェクト行列を更新　変わった位置の行列を
    //UpdateTransform();

    // モデル行列更新　かわったからここに言って
    //model->UpdateTransform(transform);
}

void BulletFiring::Lanch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float   lifeTimer)
{
    // direction 方向　正規化して入れる差もなきゃスピード変わる
    this->direction = direction;
    this->position = position;
    this->lifeTimer = lifeTimer;
}

void BulletFiring::Destroy()
{
    ActorManager::Instance().Remove(GetActor());
}

void BulletFiring::OnGUI()
{
    ImGui::InputFloat3("Move Speed", &position.x);
    ImGui::InputFloat3("Turn Speed", &direction.x);
    ImGui::InputFloat("lifeTimer", &lifeTimer);

}
