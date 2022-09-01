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
	delete sprite_;
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

	SEStartHandle_ = audio_->LoadWave("Sound/window.wav");
	

	uint32_t textureReticle = TextureManager::Load("ret.png");
	UItextureHandle_ = TextureManager::Load("aim.png");
	UItextureHandleBlack_= TextureManager::Load("UI.png");
	UItextureHandleTitle_ = TextureManager::Load("title.png");
	sprite_ = Sprite::Create(UItextureHandle_,{1280/2,710/2},{1,1,1,1},{0.5f,0.5f});
	spriteBlack_ = Sprite::Create(UItextureHandleBlack_, { 0,0 });
	spriteTitle_ = Sprite::Create(UItextureHandleTitle_, { 0,0 });

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

	//敵の生成
	//Enemy* newEnemy = new Enemy();
	//newEnemy->Initialize(model_, worldTransform_.translation_, textureHandle_);
	//enemy_.reset(newEnemy);

	//enemy_->SetPlayer(newPlayer);

	//敵の生成と初期化
	
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
	

	if (input_->TriggerKey(DIK_L)) {
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

	if (isTitleBGM == 0) {
		voiceTitleHandle_ = audio_->PlayWave(titleBGMHandle_, true,(0.4f));
		isTitleBGM = 1;
	}
	

	//ゲームスタート
	if (input_->TriggerKey(DIK_S)) {
		audio_->PlayWave(SEStartHandle_);
		voiceBGMHandle_ = audio_->PlayWave(BGMHandle_,true,(0.4f));
		//voiceBossBGMHandle_ = audio_->PlayWave(BossBGMHandle_);
		//audio_->PlayWave(BGMHandle_);
		//audio_->PlayWave(BossBGMHandle_);
		isStart = 1;
		audio_->StopWave(voiceTitleHandle_);
	}
	//天球(外側)
	worlddome_->UpdateN();
	//player更新処理
	player_->Update(isStart);

	//カメラワーク更新処理
	CameraUpdate();

	//playerの色変え
	if (input_->TriggerKey(DIK_C)) {
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
		#pragma region	敵発生

		if (isPop == 0) {
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
			frame++;
			popPos = 0;
			audio_->StopWave(voiceBossBGMHandle_);
		}

		#pragma endregion
		

		//enemyの死亡フラグ
		enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {
			return enemy->IsDead();
		});

		if (playerHp <= 0) {
			playerisDead = true;
		}

		//天球(内側)
		skydome_->Update();
		
		/*if (enemy_) {
			enemy_->Update();
		}*/

		//敵の動き
		for (std::unique_ptr<Enemy>& enemy : enemys_) {
			enemy->Update(phase);
		}

	}

	//UI

	
	Vector2 size = sprite_->GetSize();

	if (input_->PushKey(DIK_G)) {
		size.x += 15.0f;
	}

	if (input_->PushKey(DIK_F)) {
		size.x -= 15.0f;
	}

	sprite_->SetSize(size);

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

	//デバッグ
	debugText_->SetPos(20, 20);
	debugText_->Printf("frame %d", frame);
	debugText_->SetPos(20, 40);
	debugText_->Printf("pop %d", popEnemyTimer);
	debugText_->SetPos(20, 60);
	debugText_->Printf("enemy %d", enemys_.size());
	debugText_->SetPos(20, 80);
	debugText_->Printf("hp %d", playerHp);
	debugText_->SetPos(20, 100);
	debugText_->Printf("isDead %d", playerisDead);
	debugText_->SetPos(20, 120);
	debugText_->Printf("ispop %d", isPop);
	debugText_->SetPos(20, 140);
	debugText_->Printf("poppos %d", popPos);
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
		

		/*if (enemy_) {
			enemy_->Draw(viewProjection_);
		}*/

		for (std::unique_ptr<Enemy>& enemy : enemys_) {
			enemy->Draw(viewProjection_);
		}
	}

	if (playerisDead == false) {
		player_->Draw(viewProjection_);
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
		if (playerColor == 0) {
			spriteBlack_->Draw();
		}
		else {
			sprite_->Draw();
		}

		player_->DrawUI();
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


				player_->OnCollision();
				bullet->OnCollision();

				if (playerColor == 0 && enemy->GetEnemyColor() == 1) {
					playerHp -= 10;
				}
				else if (playerColor == 1 && enemy->GetEnemyColor()==0) {
					playerHp -= 10;
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
			float r1 = enemy->worldTransform_.scale_.x*3;
			float r2 = bullet->worldTransform_.scale_.x;
			float r = r1 + r2;

			Vector3 dis = posB - posA;

			if ((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z) <= (r * r)) {
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
