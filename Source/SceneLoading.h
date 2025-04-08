#pragma once
#include "Graphics/Sprite.h"
#include "SpriteControll.h"
#include "TransForm2D.h"
#include "Ui.h"
#include "UiManager.h"
#include "Actor.h"
#include "Scene.h"
// クラス
#include <thread>

// ローディングシーン
class SceneLoading : public Scene
{
public:
    // 次のシーンをここのシーンに入れる
    SceneLoading(Scene* nextScene): nextScene(nextScene) {}
    ~SceneLoading() override {}

    // 初期化
    void Initialize() override;

    // スタート
    void Start() override;

    // 終了化
    void Finalize() override;

    // 更新処理
    void Update(float elapsedTime)override;

    // 描画処理
    void Render() override;

    // 描画２D
    void Render2D(std::shared_ptr<Sprite> sprite, float angle, ID3D11DeviceContext* dc, DirectX::XMFLOAT2 pos);

    // コンポネント登録
    void InitializeComponent();

    // icon回転
    void RotateLoadingIcon(float elapsedTime);

private:
    // ローディングスレッド
    // スタティックだからクラスの中身をいじれない
    static void LoadingThread(SceneLoading* scene);
private:
    std::shared_ptr<Sprite> spriteLoading;
    std::shared_ptr<Sprite> spriteControlManual;

    float angle = 0.0f;
    float angleLoading = 0.0f;

    Scene* nextScene = nullptr;
    std::thread* thread = nullptr;

    // 行動範囲
    DirectX::XMFLOAT3 minPos = { 0.0f,0.0f,0.0f };
    DirectX::XMFLOAT3 maxPos = { 0.0f,0.0f,0.0f };

    // actorManagerが１以上なら
    int sceneGameCountMin = 1;

    std::unique_ptr<Scene> currentScene;

    bool isFinalizeGame = false;
};
