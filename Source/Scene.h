#pragma once

// 定数定義
namespace SceneConfig
{
    constexpr const char* sceneTitleName = "SceneTitle";
    constexpr const char* sceneGameName = "SceneGame";
    constexpr const char* sceneGameOverName = "SceneGameOver";
    constexpr const char* sceneGameClearName = "SceneGameClear";
};

// シーン
class Scene
{
public:
    Scene() {}
    virtual ~Scene() {}

    // 初期化
    virtual void Initialize() = 0;

    // 始め
    virtual void Start() = 0;

    // 終了化
    virtual void Finalize() = 0;

    // 更新処理
    virtual void Update(float elapsedTime) = 0;

    // 描画処理
    virtual void Render() = 0;

    // 準備完了しているか
    bool IsReady() const { return ready; }

    // 準備完了設定
    void SetReady() { ready = true; }

    // シーンの名前変更
    void SetSceneName(char* sceneName) { this->sceneName = sceneName; }
    // シーンの名前確認
    char* GetSceneName() { return sceneName; }

private:
    bool  ready = false;

    char* sceneName;
};