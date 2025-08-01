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


// 定数定義
namespace LoadingConfig
{
    // 数字の元画像の位置
    constexpr float selectNumbarOneTexPositionX = 20;
    constexpr float selectNumbarTwoTexPositionX = 40;
    constexpr float selectNumbarThreeTexPositionX = 60;
};


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

    // コマンド選択用
    void Select(float elapsedTime);

    // 不透明度点滅
    void FadeAlphaPulse(float elapsedTime);
private:
    // ローディングスレッド
    // スタティックだからクラスの中身をいじれない
    static void LoadingThread(SceneLoading* scene);
private:
    enum class Select
    {
        Xbox,
        KeyBord        
    };

private:
    std::shared_ptr<Sprite> spriteLoading;
    std::shared_ptr<Sprite> spriteControlManual;

    // 画面サイズ
    float screenWidth = 1280;
    float screenHeight = 720;

    // 画面の比率
    DirectX::XMFLOAT2 scaleScreen;

    // 非選択状態透明度
    float commandAlphaUnSelect = 0.5f;
    float commandAlphaSelect = 1.0f;

    // 選択コマンド元画像位置
    DirectX::XMFLOAT2 commandSelectTexPos = { .0f,92.0f };
    // 選択コマンド元画像大きさ
    DirectX::XMFLOAT2 commandSelectTexScale = { .0f,92.0f };
    // 非選択コマンド基画像位置
    DirectX::XMFLOAT2 commandUnSelectTexPos = { .0f,0.0f };
    // 非選択コマンド基画像大きさ
    DirectX::XMFLOAT2 commandSUnelectTexScale = { .0f,92.0f };

    // ロード中アイコン
    DirectX::XMFLOAT2 loadingIconPos = { 1280.0f,677 };

    // ロード点
    DirectX::XMFLOAT2 nomLoadingIconPos = { 903.0f,927 };

    // ロードの位置
    DirectX::XMFLOAT2 nowLoadingIconPos = { 903.0f,927 };

    // コマンドの大きさ
    DirectX::XMFLOAT2 commandScale = { 181,104 };

    // ゲームスタートコマンド
    DirectX::XMFLOAT2 startCommandPos = { 903.0f,977 };

    // 操作説明xbx
    DirectX::XMFLOAT2 operationXbxPos = { 300.0f,0.0f };

    // 操作説明キーボード
    DirectX::XMFLOAT2 operationPos = { 380.0f,0.0f };

    // ボタンの位置
    DirectX::XMFLOAT2 buttonPos = { 0,0 };

    // ボタン用位置の大きさ分
    float buttonOffset = 20.0f;

    // ページ数
    DirectX::XMFLOAT2 selectNumbar = { 1430.0f, 33.0f };
    // フォントの元画像位置
    DirectX::XMFLOAT2 selectNumbarTexPosition = { 0.0f, 60.0f };
    // フォントの元画像サイズ
    DirectX::XMFLOAT2 selectNumbarTexScale = { 20.0f, 20.0f };

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

    float stateTime = 0.0f;
    DirectX::XMFLOAT4 stateTimeMax = { 1.0f,3.0f,5.0f,8.0f };

    // 選択コマンド元画像大きさ
    DirectX::XMFLOAT2 LoadingSelectTexScale = { 887.0f,270.0f };

    float texScaleLoadMin = 887.0f;
    float texScaleLoad1 = 970.0f;
    float texScaleLoad2 = 1100.0f;
    float texScaleLoad3 = 1200.0f;

    DirectX::XMFLOAT2 scaleLoad = { 300,200 };
    float scaleAdd = 50.0f;

    int selectPush = 0;
    int selectPushMin = 0;
    int selectPushMax = 0;

    // 入力保持用（グローバル or クラス内に保持）
    float stickHoldTimerX = 0.0f;
    float stickHoldTimerXStart = 0.0f;
    float stickHoldTime = 0.1f;
    // ２D描画
    bool draw2d = true;
    bool draw2dEmpty = false;


    // ページ数の不透明度
    float numbarAlpha = 1.0f;
    float numbarAlphaMax = 1.0f;
    float numbarAlphaMin = 0.3f;
    float numbarAlphaValue = 0.5f;
};
