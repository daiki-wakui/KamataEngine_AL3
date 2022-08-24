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

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	uint32_t UItextureHandle_ = 0;
	uint32_t UItextureHandleBlack_ = 0;
	uint32_t UItextureHandleTitle_ = 0;

	Sprite* sprite_ = nullptr;
	Sprite* spriteBlack_ = nullptr;
	Sprite* spriteTitle_ = nullptr;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//3Dモデル
	Model* model_ = nullptr;

	Model* modelSkydome_ = nullptr;

	Model* modelWorlddome_ = nullptr;
	Model* playerModel_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//自キャラ
	//Player* player_ = nullptr;
	std::unique_ptr<Player> player_;

	std::unique_ptr<Enemy> enemy_;

	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Skydome> worlddome_;
	//Skydome* skydome_ = nullptr;

	//std::list<std::unique_ptr<Enemy>> enemys_;

	bool isDebugCameraActive_ = false;

	//ゲーム開始フラグ
	int isStart = 0;
	//プレイヤーの色判別
	int playerColor = 0;

	//衝突判定
	void CheakAllCollisions();
};

void MatrixSynthetic(WorldTransform& worldTransform);
