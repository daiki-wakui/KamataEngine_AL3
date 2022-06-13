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
}

void GameScene::Initialize() {
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//乱数範囲の指定
	//回転
	std::uniform_real_distribution<float> rotDist(0.0f, RADIAN(180.0f));
	//座標
	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//3Dモデル
	model_ = Model::Create();

	//ワールドトランスフォームの初期化
	
	for (i = 0; i < PartId::kNumPartId; i++) {
		worldTransforms_[i].Initialize();
	}

	//キャラクターの大元

	//脊髄
	worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[kRoot];
	worldTransforms_[PartId::kSpine].translation_ = { 0,4.5f,0 };
	
	//上半身
	//胸
	worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[kSpine];
	worldTransforms_[PartId::kChest].translation_ = { 0,0,0 };

	//頭
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[kChest];
	worldTransforms_[PartId::kHead].translation_ = { 0,4.5f,0 };

	//左上
	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[kChest];
	worldTransforms_[PartId::kArmL].translation_ = { -4.5f,0,0 };

	//右上
	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[kChest];
	worldTransforms_[PartId::kArmR].translation_ = { 4.5f,0,0 };

	//下半身
	//腰
	worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[kSpine];
	worldTransforms_[PartId::kHip].translation_ = { 0,-4.5f,0 };

	//左足
	worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[kHip];
	worldTransforms_[PartId::kLegL].translation_ = { -4.5f,-4.5f,0 };

	//右足
	worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[kHip];
	worldTransforms_[PartId::kLegR].translation_ = { 4.5f,-4.5f,0 };

	for (i = 0; i < PartId::kNumPartId; i++) {
		MatrixSynthetic(worldTransforms_[i]);
	}

	//行列の転送
	for (i = 0; i < PartId::kNumPartId; i++) {
		worldTransforms_[i].TransferMatrix();
	}

	//ビュープロダクションの初期化
	viewProjection_.Initialize();

	//軸方向表示の表示を無効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());


}


void GameScene::Update() {

	Vector3 move = { 0,0,0 };

	if (input_->PushKey(DIK_RIGHT)) {
		move = { 0.5f,0,0 };
	}
	else if (input_->PushKey(DIK_LEFT)) {
		move = { -0.5f,0,0 };
	}

	//上半身回転
	if (input_->PushKey(DIK_U)) {
		worldTransforms_[PartId::kChest].rotation_.y -= 0.05f;
	}
	else if (input_->PushKey(DIK_I)) {
		worldTransforms_[PartId::kChest].rotation_.y += 0.05f;
	}

	//下半身回転
	if (input_->PushKey(DIK_J)) {
		worldTransforms_[PartId::kHip].rotation_.y -= 0.05f;
	}
	else if (input_->PushKey(DIK_K)) {
		worldTransforms_[PartId::kHip].rotation_.y += 0.05f;
	}

	worldTransforms_[PartId::kRoot].translation_.x += move.x;
	
	//全パーツの更新
	for (i = 0; i < PartId::kNumPartId; i++) {
		MatrixSynthetic(worldTransforms_[i]);
	}

	worldTransforms_[PartId::kSpine].matWorld_ *= worldTransforms_[PartId::kRoot].matWorld_;
	worldTransforms_[PartId::kChest].matWorld_ *= worldTransforms_[PartId::kSpine].matWorld_;
	worldTransforms_[PartId::kHead].matWorld_ *= worldTransforms_[PartId::kChest].matWorld_;
	worldTransforms_[PartId::kArmL].matWorld_ *= worldTransforms_[PartId::kChest].matWorld_;
	worldTransforms_[PartId::kArmR].matWorld_ *= worldTransforms_[PartId::kChest].matWorld_;
	worldTransforms_[PartId::kHip].matWorld_ *= worldTransforms_[PartId::kSpine].matWorld_;
	worldTransforms_[PartId::kLegL].matWorld_ *= worldTransforms_[PartId::kHip].matWorld_;
	worldTransforms_[PartId::kLegR].matWorld_ *= worldTransforms_[PartId::kHip].matWorld_;

	for (i = 0; i < PartId::kNumPartId; i++) {
		worldTransforms_[i].TransferMatrix();
	}

	//行列の再計算
	viewProjection_.UpdateMatrix();

	//デバッグ表示
	debugText_->SetPos(50, 10);
	debugText_->Printf("pos %f", worldTransforms_[0].translation_.x);

	debugText_->SetPos(50, 30);
	debugText_->Printf("U/I key : Chest kaitenn");

	debugText_->SetPos(50, 50);
	debugText_->Printf("J/K key : Hip kaitenn");

	//デバッグカメラの更新
	debugCamera_->Update();
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
	/*for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
	}*/
	/*model_->Draw(worldTransforms_[0], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[1], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[2], viewProjection_, textureHandle_);*/
	

	for (i = 0; i < PartId::kNumPartId; i++) {
		if (i == PartId::kRoot || i == PartId::kSpine) {
			continue;
		}
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
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

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
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
