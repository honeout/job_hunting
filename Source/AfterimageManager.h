#pragma once

#include <vector>
#include "Afterimage.h"
#include <set>

// 残像_マネージャー
class AfterimageManager
{
private:
    AfterimageManager() {}
    ~AfterimageManager() {}

    //// 残像_同士の衝突処理
    //void CollisiionEnemyVsEnemies();

public:
    // 唯一のスタンス取得
    // 自分じゃ作れないからシングルトンで作る
    static AfterimageManager& Instance()
    {
        static AfterimageManager instance;
        return instance;
    }



    // 更新処理
    void Update(float elapsedTime);

    // デバッグプリミティブ描画
    void DrawDebugPrimitive();

    // 描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader);

    // 残像_削除
    void Remove(Afterimage* enemy);

    // 管理する為の奴
    // 残像_登録
    void Register(Afterimage* enemy);

    void DrawDebugGUI();

    // 残像_全削除
    void Clear();

    // 何番なのかの確認
    // 最大数値を取り出す
    // 残像_数取得
    int GetAfterimageCount() const { return static_cast<int>(afterimages.size()); }

    // 残像_取得
    // 引数の番号の場所に入っている物を出す。
    Afterimage* GetAfterimage(int index) { return afterimages.at(index); }


private:
    // 更新　描画を複数する為のもの
    std::vector<Afterimage*> afterimages;
    // 殺す奴セット
    std::set<Afterimage*>  removes;



};
