#include "SceneManager.h"

// 更新処理
void SceneManager::Update(float elapsedTime)
{
    if (this->nextScene)
    {
        // 古いシーン終了処理
        this->Clear();


        // 新しいシーンを設定
        this->currentScene = this->nextScene;
        this->nextScene = nullptr;

        // シーン初期化処理
        // 違う場所で呼ばれてるからもし呼ばれていたら通らない風にする。
        if (!this->currentScene->IsReady())
            this->currentScene->Initialize();

    }

    if (currentScene )
    {
        // 毎フレーム更新nextの中身の奴
        
        currentScene->Update(elapsedTime);
    }
}
// 描画処理
void SceneManager::Render()
{
    // 今動いてるものをする
    if (this->currentScene)
    {
        this->currentScene->Render();
    }
}
// シーンクリア
void SceneManager::Clear()
{
    if (this->currentScene)
    {
        currentScene->Finalize();
        delete currentScene;
        currentScene = nullptr;
    }
}
// シーン切り替え
void SceneManager::ChangeScene(Scene* scene)
{
    // 新しいシーンを設定
    this->nextScene = scene;
}
