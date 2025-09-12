#pragma once
#include <string>


namespace AudioConfig
{
	// -----   移動   -----
	// プレイヤー移動 
	constexpr const char* audioMove = "Data/Audio/SE/footsteps.wav";
	
	// ダッシュ音
	constexpr const char* audioDush = "Data/Audio/SE/hit stop.wav";

	// -----   魔法   -----
	// 魔法溜め
	constexpr const char* audioMagicCharge = "Data/Audio/SE/charge sound enemy.wav";

	// 魔法切れ、必殺技溜まっていない
	constexpr const char* audioMagicCant = "Data/Audio/SE/charge sound enemy.wav";

	// 炎
	constexpr const char* audioFire = "Data/Audio/SE/flame flight.wav";
	// 飛ぶ炎
	constexpr const char* audioFireMove = "Data/Audio/SE/Special Move Flame.wav";

	// 雷魔法音
	constexpr const char* audioThunder = "Data/Audio/SE/thunder.wav";

	// 氷
	constexpr const char* audioIce = "Data/Audio/SE/ice launch.wav";
	// 回復
	constexpr const char* audioHeale = "Data/Audio/SE/maou_se_magical11.wav";



	// -----   HP     -----
	// サイレン音
	constexpr const char* audioSiren = "Data/Audio/SE/siren.wav";

	// -----   ダメ    -----
	// 与ダメージ音　敵味方共有
	constexpr const char* audioDealtDamage = "Data/Audio/SE/slash.wav";
	// 被ダメ敵
	constexpr const char* audioCoveredDamage = "Data/Audio/SE/blow.wav";

	// ----- ジャンプ  -----
	// ジャンプ音
	constexpr const char* audioJump = "Data/Audio/SE/Enemy walking attackk hit.wav";
	// 着地、足音音
	constexpr const char* audioLanding = "Data/Audio/SE/Enemy Landing.wav";

	// -----   攻撃    -----
	// インパクト発射
	constexpr const char* audioImpactPush = "Data/Audio/SE/shockwave hit.wav";
	// 敵溜め
	constexpr const char* audioCharge = "Data/Audio/SE/charge sound enemy.wav";

	// テレポート音
	constexpr const char* audioTelleport = "Data/Audio/SE/telleport.wav";

	// -----   必殺技   -----
	// 必殺技溜まった音
	constexpr const char* audioSpecialCharge = "Data/Audio/SE/For the killer move.wav";
	// 雷必殺技
	constexpr const char* audioThunderSpecial = "Data/Audio/SE/Special Move Thunder.wav";

	// ------音調整------------
	// bgmや繰り返すseの音の大きさ
	constexpr float bgmVolume = 1.0f;
	// seの音の大きさ
	constexpr float seVolume = 0.7f;
	// 音のフェードアウト速度
	constexpr float fadeVolume = 0.01f;
	// 音の最大値
	constexpr float volumeMax = 1.0f;
	// 音の最低値
	constexpr float volumeMin = 0.0f;
	// 音のループ
	constexpr bool audioLoop = true;
	// 音の非ループ
	constexpr bool audioNotLoop = false;
};

struct AudioParam
{
    // 音の名前
    std::string filename{};
    // 音の大きさ
    float volume;
    // 音の変化速度
    float fadeSpeed;
    // 音の最大値
    float volumeMax;
    // 音の最低値
    float volumeMin;
    
    // ループするか
    bool loop = false;


	static AudioParam Bgm()
	{
		return { {},AudioConfig::bgmVolume, 
			AudioConfig::fadeVolume,
			AudioConfig::volumeMax,
			AudioConfig::volumeMin,
			AudioConfig::audioLoop
		};
	};

	static AudioParam Se()
	{
		return { {},AudioConfig::seVolume,
			AudioConfig::fadeVolume,
			AudioConfig::volumeMax,
			AudioConfig::volumeMin,
			AudioConfig::audioNotLoop
		};
	};
};
