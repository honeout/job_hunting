#pragma once
#include <DirectXMath.h>
#include "Component.h"

// 移動
class Mp : public Component
{
public:
	Mp();
	virtual ~Mp();

	// 名前取得
	const char* GetName() const override { return "Mp"; }

#ifdef _DEBUG
	// GUI描画
	void OnGUI() override;
#endif // _DEBUG

	// 無敵
	bool UpdateInbincibleTimer(float elapsedTime);

	// ダメージを与える ダメージを受けたら他に渡す。
	bool ApplyConsumption(int damage);

	bool MpCharge(float elapsedTime);

	// HP状態を取得
	int GetMagic() const { return magic; }

	// HPを突っ込む
	void SetMagic(int magic) {
		this->magic = magic;
	}

	// 最大HPを取り出す
	int GetMaxMagic() const { return maxMagic; }

	// 最大HPを突っ込む
	void SetMaxMagic(int maxMagic) { this->maxMagic = maxMagic; }

	// 死亡した時に呼ばれる
	bool OnDead();

	bool GetMpEmpth() const { return mpEmpth; }

	void SetMpEmpth(bool mpEmpth) { this->mpEmpth = mpEmpth; }

private:
	// 最大値HP
	int          maxMagic = 5;

	// Mp
	int          magic = maxMagic;

	// 無敵時間
	float   invincibleTimer = 0.3f;
	float   invincibleTimerMin = 0.0f;

	float   imvincibleTimerMax = 0.3f;

	int     mpValueMin = 0;
	int     mpMin = 0;

	int     mpValue = 1;

	// 判定
	bool mpEmpth = false;
};

