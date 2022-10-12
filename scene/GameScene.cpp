#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>

#define RADIAN(angle)angle*3.1415f/180
#define PAI 3.1415f

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
	delete modelSkydome_;
	delete playerModel_;
	delete modelWorlddome_;
	delete bulletModel_b;
	delete bulletModel_w;
	delete enemyModel_;
}

void GameScene::Initialize() {


	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	BGMHandle_ = audio_->LoadWave("Sound/Electric Wild.wav");
	BossBGMHandle_ = audio_->LoadWave("Sound/Alone.wav");
	titleBGMHandle_ = audio_->LoadWave("Sound/PerituneMaterial.wav");

	SEWindowHandle_ = audio_->LoadWave("Sound/window.wav");
	SEShotHandle_ = audio_->LoadWave("Sound/Shot.wav");
	SEStartHandle_ = audio_->LoadWave("Sound/Start.wav");
	SEHitHandle_ = audio_->LoadWave("Sound/Hit.wav");
	SEDamageHandle_ = audio_->LoadWave("Sound/damage.wav");
	SEChangeHandle_ = audio_->LoadWave("Sound/change.wav");
	SEGuradHandle_ = audio_->LoadWave("Sound/gurad.wav");
	SEWinHandle_ = audio_->LoadWave("Sound/Victory.wav");

	uint32_t textureReticle = TextureManager::Load("ret.png");


	UItextureHandleTitle_ = TextureManager::Load("title.png");
	UIHPHandle_ = TextureManager::Load("UI/HP.png");
	UIReticleHandle_ = TextureManager::Load("UI/reticle.png");
	UISideReticleHandle_ = TextureManager::Load("UI/sideReticle.png");
	UIWhiteHandle_ = TextureManager::Load("UI/UI_Wihte.png");
	UIBlackHandle_ = TextureManager::Load("UI/UI_Black.png");
	UIHPFontHandle_[0] = TextureManager::Load("UI/0.png");
	UIHPFontHandle_[1] = TextureManager::Load("UI/10.png");
	UIHPFontHandle_[2] = TextureManager::Load("UI/20.png");
	UIHPFontHandle_[3] = TextureManager::Load("UI/30.png");
	UIHPFontHandle_[4] = TextureManager::Load("UI/40.png");
	UIHPFontHandle_[5] = TextureManager::Load("UI/50.png");
	UIHPFontHandle_[6] = TextureManager::Load("UI/60.png");
	UIHPFontHandle_[7] = TextureManager::Load("UI/70.png");
	UIHPFontHandle_[8] = TextureManager::Load("UI/80.png");
	UIHPFontHandle_[9] = TextureManager::Load("UI/90.png");
	UIHPFontHandle_[10] = TextureManager::Load("UI/100.png");
	UIDamageHandle_ = TextureManager::Load("UI/Damage.png");
	UIGuardBlackHandle_ = TextureManager::Load("UI/guard_Black.png");
	UIGuardWhiteHandle_ = TextureManager::Load("UI/guard_White.png");
	UIClearHandle_ = TextureManager::Load("UI/clear.png");
	UIGameOverHandle_ = TextureManager::Load("UI/gameover.png");
	UIEx1Handle_ = TextureManager::Load("UI/explan1.png");
	UIEx2Handle_ = TextureManager::Load("UI/expaln2.png");
	

	//sprite_ = Sprite::Create(UItextureHandle_,{1280/2,710/2},{1,1,1,1},{0.5f,0.5f});
	spriteTitle_ = Sprite::Create(UItextureHandleTitle_, { 0,0 });
	spriteHP_ = Sprite::Create(UIHPHandle_, { 1280 / 2,200 }, { 1,1,1,1 }, { 0.5f,0.5f });
	spriteReticle_ = Sprite::Create(UIReticleHandle_, { 1280 / 2,720 / 2 }, { 1,1,1,1 }, { 0.5f,0.5f });
	spriteSideReticle_ = Sprite::Create(UISideReticleHandle_, { 1280 / 2,720/2 }, { 1,1,1,1 }, { 0.5f,0.5f });
	spriteWhite_ = Sprite::Create(UIWhiteHandle_, { 1075,575 }, { 1,1,1,1 }, { 0.5f,0.5f });
	spriteBlack_= Sprite::Create(UIBlackHandle_, { 1075,575 }, { 1,1,1,1 }, { 0.5f,0.5f });
	spriteHP0_ = Sprite::Create(UIHPFontHandle_[0], { -5,0 });
	spriteHP10_ = Sprite::Create(UIHPFontHandle_[1], { -5,0 });
	spriteHP20_ = Sprite::Create(UIHPFontHandle_[2], { -5,0 });
	spriteHP30_ = Sprite::Create(UIHPFontHandle_[3], { -5,0 });
	spriteHP40_ = Sprite::Create(UIHPFontHandle_[4], { -5,0 });
	spriteHP50_ = Sprite::Create(UIHPFontHandle_[5], { -5,0 });
	spriteHP60_ = Sprite::Create(UIHPFontHandle_[6], { -5,0 });
	spriteHP70_ = Sprite::Create(UIHPFontHandle_[7], { -5,0 });
	spriteHP80_ = Sprite::Create(UIHPFontHandle_[8], { -5,0 });
	spriteHP90_ = Sprite::Create(UIHPFontHandle_[9], { -5,0 });
	spriteHP100_ = Sprite::Create(UIHPFontHandle_[10], { -5,0 });
	spriteDamage_ = Sprite::Create(UIDamageHandle_, { 0,0 });
	spriteGuardBlack_ = Sprite::Create(UIGuardBlackHandle_, { 0,0 });
	spriteGuardWhite_ = Sprite::Create(UIGuardWhiteHandle_, { 0,0 });
	spriteClear_ = Sprite::Create(UIClearHandle_, { 0,0 });
	spriteGameOver_ = Sprite::Create(UIGameOverHandle_, { 0,0 });
	spriteEx1_ = Sprite::Create(UIEx1Handle_, { 0,0 });
	spriteEx2_= Sprite::Create(UIEx2Handle_, { 0,0 });


	//3Dモデル
	model_ = Model::Create();

	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("tennkyuu", true);
	modelWorlddome_ = Model::CreateFromOBJ("Map", true);
	playerModel_ = Model::CreateFromOBJ("playerModelB", true);

	bulletModel_b = Model::CreateFromOBJ("playerBullet_Black", true);
	bulletModel_w = Model::CreateFromOBJ("playerBullet_White", true);

	enemyModel_ = Model::CreateFromOBJ("enemy", true);

	viewProjection_.eye = { 0,10,-50 };

	//ビュープロダクションの初期化
	viewProjection_.Initialize();

	//軸方向表示の表示を無効にする
	//AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//天球(中側)
	newSkydome->Initialize(modelSkydome_);
	skydome_.reset(newSkydome);

	//天球(外側)
	Skydome* newWorlddome = new Skydome();
	newWorlddome->Initialize(modelWorlddome_);
	worlddome_.reset(newWorlddome);

	//playerの生成
	newPlayer->Initialize(playerModel_,model_,bulletModel_b,bulletModel_w);
	player_.reset(newPlayer);
}


void GameScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = true;
	}

	if (input_->TriggerKey(DIK_9)) {
		isDebugCameraActive_ = false;
	}
#endif // _DEBUG

	if (isTitleBGM == 0) {

		if (audio_->IsPlaying(voiceTitleHandle_) == false) {
			voiceTitleHandle_ = audio_->PlayWave(titleBGMHandle_, true, (0.3f));
		}
		
		isTitleBGM = 1;
	}
	

	//ゲームスタート
	if (input_->TriggerKey(DIK_S)) {
		
		if (isStart == 0) {
			tutorial = 0;
			audio_->PlayWave(SEStartHandle_);
			voiceBGMHandle_ = audio_->PlayWave(BGMHandle_, true, (0.4f));
		}
		isStart = 1;
		audio_->StopWave(voiceTitleHandle_);
	}
	//天球(外側)
	worlddome_->UpdateN();

	if (playerHp > 0) {
		//player更新処理
		player_->Update(isStart);
	}
	
	coolTime--;
	if (input_->PushKey(DIK_SPACE)) {
		if (coolTime < 0) {
			audio_->PlayWave(SEShotHandle_, false, (0.4f));
			coolTime = 7;
		}
	}

	//カメラワーク更新処理
	CameraUpdate();

	//playerの色変え
	if (input_->TriggerKey(DIK_C)) {

		audio_->PlayWave(SEChangeHandle_);
		//黒なら白に
		if (playerColor == 0) {
			playerColor = 1;
		}
		//白なら黒に
		else if (playerColor == 1) {
			playerColor = 0;
		}
	}

	//始まったら
	if (isStart == 1) {
		//チュートリアル
		if (tutorial == 0) {
			if (input_->PushKey(DIK_RIGHT) || input_->PushKey(DIK_LEFT) ||
				input_->PushKey(DIK_UP) || input_->PushKey(DIK_DOWN)) {

				if (tutorialPoint[0] == 0) {
					tutorialPoint[0] = 1;
				}
			}

			if (input_->PushKey(DIK_Z)) {
				if (tutorialPoint[1] == 0) {
					tutorialPoint[1] = 1;
				}
			}

			if (input_->PushKey(DIK_X)) {
				if (tutorialPoint[2] == 0) {
					tutorialPoint[2] = 1;
				}
			}

			if (tutorialPoint[0] == 1 && tutorialPoint[1] == 1&& tutorialPoint[2] == 1) {
				tutorial = 1;
			}
		}

		#pragma region	敵発生

		if (isPop == 0 && tutorial == 1) {
			for (int i = 0; i < 3; i++) {
				std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
				newEnemy->Initialize(isPop,0, enemyModel_, bulletModel_b, bulletModel_w, Vector3{ -20.0f * (i + 1),0,50.0f }, textureHandle_);
				newEnemy->SetPlayer(newPlayer);

				//敵を登録する
				enemys_.push_back(std::move(newEnemy));
			}
			phase = 1;
			isPop++;
		}

		if (isPop == 1 && frame >= 200) {
			tutorial = 2;

			for (int i = 0; i < 3; i++) {
				std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
				newEnemy->Initialize(isPop, 1, enemyModel_, bulletModel_b, bulletModel_w, Vector3{ 60.0f - 20.0f * (i + 1),0,50.0f }, textureHandle_);
				newEnemy->SetPlayer(newPlayer);

				//敵を登録する
				enemys_.push_back(std::move(newEnemy));
			}
			phase = 2;
			isPop++;
		}

		if (isPop == 2 && frame >= 400) {
			for (int i = 0; i < 2; i++) {
				std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
				newEnemy->Initialize(isPop, 0, enemyModel_, bulletModel_b, bulletModel_w, Vector3{ 60.0f - 20.0f * (i + 1),10.0f,50.0f }, textureHandle_);
				newEnemy->SetPlayer(newPlayer);

				//敵を登録する
				enemys_.push_back(std::move(newEnemy));
			}

			for (int i = 0; i < 2; i++) {
				std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
				newEnemy->Initialize(isPop, 1, enemyModel_, bulletModel_b, bulletModel_w, Vector3{ -20.0f * (i + 1),-10.0f,50.0f }, textureHandle_);
				newEnemy->SetPlayer(newPlayer);

				//敵を登録する
				enemys_.push_back(std::move(newEnemy));
			}
			phase = 3;
			isPop++;
		}

		if (isPop == 3 && frame >= 600) {
			popEnemyTimer++;

			if (popEnemyTimer >= 50) {
				std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
				newEnemy->Initialize(isPop, 1, enemyModel_, bulletModel_b, bulletModel_w, Vector3{ 20.0f - 20.0f * popPos,10.0f,50.0f }, textureHandle_);
				newEnemy->SetPlayer(newPlayer);

				//敵を登録する
				enemys_.push_back(std::move(newEnemy));

				popEnemyTimer = 0;
				popPos++;
			}

			phase = 4;

			if (popPos == 6) {
				isPop++;
			}
		}

		if (isPop == 4 && frame >= 800) {
			popEnemyTimer++;

			if (popEnemyTimer >= 75) {
				std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
				newEnemy->Initialize(isPop, 0, enemyModel_, bulletModel_b, bulletModel_w, Vector3{ 30.0f - 20.0f * popPos,10.0f,50.0f }, textureHandle_);
				newEnemy->SetPlayer(newPlayer);
				//敵を登録する
				enemys_.push_back(std::move(newEnemy));

				std::unique_ptr<Enemy> newEnemy2 = std::make_unique<Enemy>();
				newEnemy2->Initialize(isPop, 1, enemyModel_, bulletModel_b, bulletModel_w, Vector3{ -30.0f + 20.0f * popPos,-10.0f,50.0f }, textureHandle_);
				newEnemy2->SetPlayer(newPlayer);
				//敵を登録する
				enemys_.push_back(std::move(newEnemy2));
			
				popEnemyTimer = 0;
				popPos++;
			}

			phase = 5;

			if (popPos == 5) {
				isPop++;
			}
		}

		if (isPop == 5 && frame >= 1200) {
			audio_->StopWave(voiceBGMHandle_);
			voiceBossBGMHandle_ = audio_->PlayWave(BossBGMHandle_);

			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->BossInitialize(isPop, 0, enemyModel_, bulletModel_b, bulletModel_w, Vector3{ 0.0f,15.0f,200.0f }, textureHandle_);
			newEnemy->SetPlayer(newPlayer);
			//敵を登録する
			enemys_.push_back(std::move(newEnemy));

			std::unique_ptr<Enemy> newEnemy2 = std::make_unique<Enemy>();
			newEnemy2->BossInitialize(isPop, 1, enemyModel_, bulletModel_b, bulletModel_w, Vector3{ 0.0f,-15.0f,200.0f }, textureHandle_);
			newEnemy2->SetPlayer(newPlayer);
			//敵を登録する
			enemys_.push_back(std::move(newEnemy2));

			phase = 6;
			if (enemys_.size() == 2) {
				isPop++;
			}
		}

		if (enemys_.size() == 0) {
			if (tutorial != 0) {
				frame++;
			}
			
			popPos = 0;

			if (phase == 6) {
				isClear = 1;
				audio_->PlayWave(SEWinHandle_);
				audio_->StopWave(voiceBossBGMHandle_);
				phase = 7;
			}
		}

		#pragma endregion
		
		if (isClear == 1) {
			if (input_->TriggerKey(DIK_T)) {
				tutorialPoint[0] = 0;
				tutorialPoint[1] = 0;
				tutorialPoint[2] = 0;
				tutorial = 0;
				isStart = 0;
				isPop = 0;
				enemys_.clear();
				frame = 0;
				skydome_.release();
				newSkydome->Initialize(modelSkydome_);
				skydome_.reset(newSkydome);
				player_.release();
				newPlayer->Initialize(playerModel_, model_, bulletModel_b, bulletModel_w);
				player_.reset(newPlayer);
				playerColor = 0;
				playerHp = 100;
				playerisDead = 0;
				isTitleBGM = 0;
				audio_->StopWave(voiceBGMHandle_);
				isClear = 0;
			}
		}

		if (playerHp <= 0) {
			if (input_->TriggerKey(DIK_T)) {
				tutorialPoint[0] = 0;
				tutorialPoint[1] = 0;
				tutorialPoint[2] = 0;
				tutorial = 0;
				isStart = 0;
				isPop = 0;
				enemys_.clear();
				frame = 0;
				skydome_.release();
				newSkydome->Initialize(modelSkydome_);
				skydome_.reset(newSkydome);
				player_.release();
				newPlayer->Initialize(playerModel_, model_, bulletModel_b, bulletModel_w);
				player_.reset(newPlayer);
				playerColor = 0;
				playerHp = 100;
				playerisDead = 0;
				isTitleBGM = 0;
				audio_->StopWave(voiceBGMHandle_);
			}

			if (input_->TriggerKey(DIK_R)) {
				tutorial = 1;
				isPop = 0;
				enemys_.clear();
				frame = 0;
				player_.release();
				newPlayer->Initialize(playerModel_, model_, bulletModel_b, bulletModel_w);
				player_.reset(newPlayer);
				playerColor = 0;
				playerHp = 100;
				playerisDead = 0;
				audio_->StopWave(voiceBGMHandle_);
				voiceBGMHandle_ = audio_->PlayWave(BGMHandle_, true, (0.4f));
			}
			
		}


		//enemyの死亡フラグ
		enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
			return enemy->IsDead();
		});

		if (playerHp <= 0) {
			playerisDead = true;
		}

		//天球(内側)
		skydome_->Update();

		//敵の動き
		for (std::unique_ptr<Enemy>& enemy : enemys_) {
			enemy->Update(phase);
		}

		//無敵時間
		if (isPlayerDamage == 1) {
			invincibleTime++;

			if (invincibleTime >= 100) {
				isPlayerDamage = 0;
				invincibleTime = 0;
				playerFlash = 0;
			}
		}

		if (isPlayerGuardWhite == 1 || isPlayerGuardBlack == 1) {
			GuardTime++;
			if (GuardTime > 2) {
				isPlayerGuardBlack = 0;
				isPlayerGuardWhite = 0;
				GuardTime = 0;
			}
		}

		//UI

		Vector2 HpSize = spriteHP_->GetSize();

		HpSize.x = HpSize.x / 2 + playerHp * 2;
		if (playerHp <= 0) {
			HpSize.x = 0.0f;
		}

		spriteHP_->SetSize(HpSize);
	}

	

	#pragma region	カメラ座標更新

		//カメラ座標更新
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
		}
		else {
			//行列の再計算
			viewProjection_.UpdateMatrix();
			viewProjection_.TransferMatrix();
		}
		
	#pragma endregion

	//当たり判定
	CheakAllCollisions();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
		//ライン描画が参照するビュープロジェクションを指定する
	


	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	//モデル描画
	worlddome_->Draw(viewProjection_);

	if (isStart == 1) {
		skydome_->Draw(viewProjection_);

		for (std::unique_ptr<Enemy>& enemy : enemys_) {
			enemy->Draw(viewProjection_);
		}
	}

	if (playerFlash == 0) {
	
		if (playerisDead == false) {
			player_->Draw(viewProjection_);
		}
	}
	

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	
	if (isStart == 0) {
		spriteTitle_->Draw();
	}

	if (isStart == 1) {

		//UI描画
		if (playerColor == 0) {
			//spriteBlack_->Draw();
			spriteBlack_->Draw();
		}
		else {
			//sprite_->Draw();
			spriteWhite_->Draw();
		}

		if (playerHp > 0 || isClear == 0) {
			spriteReticle_->Draw();
			spriteHP_->Draw();
			spriteSideReticle_->Draw();
		}
		
		//player_->DrawUI();
		if (playerHp == 100) { spriteHP100_->Draw(); }
		if (playerHp == 90) { spriteHP90_->Draw(); }
		if (playerHp == 80) { spriteHP80_->Draw(); }
		if (playerHp == 70) { spriteHP70_->Draw(); }
		if (playerHp == 60) { spriteHP60_->Draw(); }
		if (playerHp == 50) { spriteHP50_->Draw(); }
		if (playerHp == 40) { spriteHP40_->Draw(); }
		if (playerHp == 30) { spriteHP30_->Draw(); }
		if (playerHp == 20) { spriteHP20_->Draw(); }
		if (playerHp == 10) { spriteHP10_->Draw(); }
		if (playerHp <= 0) { spriteHP0_->Draw(); }

		if (isPlayerGuardBlack == true) {spriteGuardBlack_->Draw();}
		if (isPlayerGuardWhite == true) {spriteGuardWhite_->Draw();}
		if (isPlayerDamage == true&&invincibleTime<=10) {spriteDamage_->Draw();}
		if (isClear == true) {spriteClear_->Draw();}
		if (playerHp <= 0) { spriteGameOver_->Draw(); }

		if (tutorial == 0 && isStart == 1) {
			spriteEx1_->Draw();
		}
		if (tutorial == 1 && isStart == 1) {
			spriteEx2_->Draw();
		}

	}
	
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

//衝突判定
void GameScene::CheakAllCollisions(){
	//衝突判定AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	/*for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();
	}*/

	#pragma region	自キャラと被弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();
	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		for (const std::unique_ptr<EnemyBullet>& bullet : enemy->GetBullets()) {
			//敵弾の座標
			posB = bullet->GetWorldPosition();

			//AとBの距離
			float r1 = player_->worldTransform_.scale_.x;
			float r2 = bullet->worldTransform_.scale_.x;
			float r = r1 + r2;

			Vector3 dis = posB - posA;

			if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r)) {

				if (playerHp > 0) {
					player_->OnCollision();
					bullet->OnCollision();

					if (playerColor == 0 && enemy->GetEnemyColor() == 1) {
						if (isPlayerDamage == 0) {
							isPlayerDamage = 1;
							playerHp -= 10;
							isShake = 1;
							randX = 0.5f;
							audio_->PlayWave(SEDamageHandle_);
						}
					}
					else if (playerColor == 1 && enemy->GetEnemyColor() == 0) {
						if (isPlayerDamage == 0) {
							isPlayerDamage = 1;
							playerHp -= 10;
							isShake = 1;
							randX = 0.5f;
							audio_->PlayWave(SEDamageHandle_);
						}
					}
					else if (playerColor == 0 && enemy->GetEnemyColor() == 0) {
						isPlayerGuardBlack = 1;
						audio_->PlayWave(SEGuradHandle_);
					}
					else if (playerColor == 1 && enemy->GetEnemyColor() == 1) {
						isPlayerGuardWhite = 1;
						audio_->PlayWave(SEGuradHandle_);
					}
				}
			}
		}
	}
	
	#pragma endregion

	#pragma region	自弾と敵キャラの当たり判定
	//自キャラの座標
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		posA = enemy->GetWorldPosition();

		//自キャラと敵弾全ての当たり判定
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			//敵弾の座標
			posB = bullet->GetWorldPosition();

			//AとBの距離
			float r1 = enemy->worldTransform_.scale_.x*2.5;

			if (enemy->GetIsBoss() == true) {
				r1 = enemy->worldTransform_.scale_.x;
			}

			float r2 = bullet->worldTransform_.scale_.x;
			float r = r1 + r2;

			Vector3 dis = posB - posA;

			if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r)) {
				audio_->PlayWave(SEHitHandle_,false,(0.2f));
				enemy->OnCollision();
				bullet->OnCollision();
			}
		}
	}
	
	

	#pragma endregion

	#pragma region	自弾と敵弾の当たり判定
	#pragma endregion
}

//カメラワーク更新処理
void GameScene::CameraUpdate(){
	Vector3 cameraVec = { 0,0,0 };
	cameraVec = player_->GetWorldPosition();
	viewProjection_.eye = cameraVec;
	viewProjection_.eye.y += 30.0f;
	viewProjection_.eye.z -= 40.0f;
	viewProjection_.target = cameraVec;
	viewProjection_.target.y += 20.0f;

	if (isShake == 1) {
		shakeTime++;
		if (shakeTime >= 2) {
			shakeTime = 0;
			randX -= 0.05f;
		}
		if (shakeTime == 0) {
			viewProjection_.eye.x += randX;
			viewProjection_.target.x += randX;
		}
		else if (shakeTime == 1) {
			viewProjection_.eye.x -= randX;
			viewProjection_.target.x -= randX;
		}
		if (randX <= 0) {
			isShake = 0;
		}
	}

	

	if (input_->PushKey(DIK_K)) {
		viewProjection_.eye.x += 0.5f;
	}
}

void MatrixSynthetic(WorldTransform& worldTransform){
	Matrix4 matScale;
	Matrix4 matRotX, matRotY, matRotZ;
	Matrix4 matRot;
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	Matrix4 matTrans2 = MathUtility::Matrix4Identity();
	matScale.ScaleSet(worldTransform.scale_);
	matRotZ.RotZSet(worldTransform.rotation_.z);
	matRotX.RotXSet(worldTransform.rotation_.x);
	matRotY.RotYSet(worldTransform.rotation_.y);
	matTrans.TransSet(worldTransform.translation_);
	matRot = matRotZ * matRotX * matRotY;
	//合成
	worldTransform.matWorld_ = matScale * matRot * matTrans;
}
