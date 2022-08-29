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
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

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
	Skydome* newSkydome = new Skydome();
	newSkydome->Initialize(modelSkydome_);
	skydome_.reset(newSkydome);

	//天球(外側)
	Skydome* newWorlddome = new Skydome();
	newWorlddome->Initialize(modelWorlddome_);
	worlddome_.reset(newWorlddome);

	//playerの生成
	newPlayer->Initialize(playerModel_,model_);
	player_.reset(newPlayer);

	//敵の生成
	//Enemy* newEnemy = new Enemy();
	//newEnemy->Initialize(model_, worldTransform_.translation_, textureHandle_);
	//enemy_.reset(newEnemy);

	//enemy_->SetPlayer(newPlayer);

	//敵の生成と初期化
	for (int i = 0; i < 3; i++) {
		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
		newEnemy->Initialize(model_, worldTransform_.translation_, textureHandle_);
		newEnemy->SetPlayer(newPlayer);

		//敵を登録する
		enemys_.push_back(std::move(newEnemy));
	}
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

	//ゲームスタート
	if (input_->TriggerKey(DIK_S)) {
		isStart = 1;
	}
	//天球(外側)
	worlddome_->UpdateN();
	//player更新処理
	player_->Update();

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
		frame++;

		if (frame >= 50) {
			popEnemyTimer++;

			if (popEnemyTimer >= 50) {
				popEnemyTimer = 0;
			}
			
		}

		//天球(内側)
		skydome_->Update();
		
		/*if (enemy_) {
			enemy_->Update();
		}*/
		for (std::unique_ptr<Enemy>& enemy : enemys_) {
			enemy->Update();
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

	player_->Draw(viewProjection_);

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
			float r1 = enemy->worldTransform_.scale_.x;
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
