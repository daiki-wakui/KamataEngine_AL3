#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "DebugCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include <memory>
#include <list>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	//サウンドデータ
	uint32_t BGMHandle_ = 0;
	uint32_t BossBGMHandle_ = 0;
	uint32_t titleBGMHandle_ = 0;

	uint32_t voiceBGMHandle_ = 0;
	uint32_t voiceBossBGMHandle_ = 0;
	uint32_t voiceTitleHandle_ = 0;
	int isTitleBGM = 0;

	uint32_t SEWindowHandle_ = 0;
	uint32_t SEStartHandle_ = 0;
	uint32_t SEHitHandle_ = 0;
	uint32_t SEShotHandle_ = 0;
	uint32_t SEDamageHandle_ = 0;
	uint32_t SEChangeHandle_ = 0;
	uint32_t SEGuradHandle_ = 0;
	uint32_t SEWinHandle_ = 0;


	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//UI
	uint32_t UIHPHandle_ = 0;
	uint32_t UIReticleHandle_ = 0;
	uint32_t UISideReticleHandle_ = 0;
	uint32_t UItextureHandleTitle_ = 0;
	uint32_t UIWhiteHandle_ = 0;
	uint32_t UIBlackHandle_ = 0;
	uint32_t UIHPFontHandle_[11] = {};
	uint32_t UIDamageHandle_ = 0;
	uint32_t UIGuardBlackHandle_ = 0;
	uint32_t UIGuardWhiteHandle_ = 0;
	uint32_t UIClearHandle_ = 0;
	uint32_t UIGameOverHandle_ = 0;
	uint32_t UIEx1Handle_ = 0;
	uint32_t UIEx2Handle_ = 0;

	Sprite* spriteTitle_ = nullptr;
	Sprite* spriteHP_ = nullptr;
	Sprite* spriteReticle_ = nullptr;
	Sprite* spriteSideReticle_ = nullptr;
	Sprite* spriteWhite_ = nullptr;
	Sprite* spriteBlack_ = nullptr;
	Sprite* spriteHP0_ = nullptr;
	Sprite* spriteHP10_ = nullptr;
	Sprite* spriteHP20_ = nullptr;
	Sprite* spriteHP30_ = nullptr;
	Sprite* spriteHP40_ = nullptr;
	Sprite* spriteHP50_ = nullptr;
	Sprite* spriteHP60_ = nullptr;
	Sprite* spriteHP70_ = nullptr;
	Sprite* spriteHP80_ = nullptr;
	Sprite* spriteHP90_ = nullptr;
	Sprite* spriteHP100_ = nullptr;
	Sprite* spriteDamage_ = nullptr;
	Sprite* spriteGuardBlack_ = nullptr;
	Sprite* spriteGuardWhite_ = nullptr;
	Sprite* spriteClear_ = nullptr;
	Sprite* spriteGameOver_ = nullptr;
	Sprite* spriteEx1_ = nullptr;
	Sprite* spriteEx2_ = nullptr;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//3Dモデル
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelWorlddome_ = nullptr;
	Model* playerModel_ = nullptr;
	Model* bulletModel_b = nullptr;
	Model* bulletModel_w = nullptr;
	Model* enemyModel_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//自キャラ
	//Player* player_ = nullptr;
	std::unique_ptr<Player> player_;

	Player* newPlayer = new Player();
	Skydome* newSkydome = new Skydome();

	//std::unique_ptr<Enemy> enemy_;

	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Skydome> worlddome_;
	//Skydome* skydome_ = nullptr;

	std::list<std::unique_ptr<Enemy>> enemys_;

	bool isDebugCameraActive_ = false;

	//ゲーム開始フラグ
	int isStart = 0;
	//プレイヤーの色判別
	int playerColor = 0;
	int playerHp = 100;
	bool playerisDead = false;
	int coolTime = 5;

	int isPlayerDamage = 0;
	int isPlayerGuardBlack = 0;
	int isPlayerGuardWhite = 0;
	int invincibleTime = 0;
	int playerFlash = 0;
	int GuardTime = 0;

	int phase = 0;
	int isPop = 0;
	int frame = 0;
	int popEnemyTimer = 0;
	int popPos = 0;
	int enemyColor = 0;

	int isClear = 0;

	int isShake = 0;
	float randX = 0.5f;
	int timer = 0;
	int shakeTime = 0;
	int tutorial = 0;
	int tutorialPoint[3] = {0,0,0};

	//衝突判定
	void CheakAllCollisions();

	//カメラワーク
	void CameraUpdate();
};

void MatrixSynthetic(WorldTransform& worldTransform);
