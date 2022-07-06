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
	worldTransform_.Initialize();
	MatrixSynthetic(worldTransform_);
	worldTransform_.TransferMatrix();
	
	//ビュープロダクションの初期化
	for (int i = 0; i < 3; i++) {
		viewProjection_[i].Initialize();
		viewProjection_[i].eye = { posDist(engine),posDist(engine),posDist(engine) };
	}

	

	//軸方向表示の表示を無効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_[0]);

	//ライン描画が参照するビュープロジェクションを指定する
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}


void GameScene::Update() {

	//スペースキーでカメラ切り替え
	if (input_->TriggerKey(DIK_SPACE)) {
		viewCameraState++;
		if (viewCameraState > 2) {
			viewCameraState = 0;
		}
	}

	//行列の合成
	MatrixSynthetic(worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();
	

	//ビュー行列の再計算
	for (int i = 0; i < 3; i++) {
		viewProjection_[i].UpdateMatrix();
	}
	
	//デバッグ表示
	debugText_->SetPos(50, 10);
	debugText_->Printf("pos %f", worldTransform_.translation_.x);

	debugText_->SetPos(50, 30);
	debugText_->Printf("Camera1 %f,%f,%f",
		viewProjection_[0].eye.x,
		viewProjection_[0].eye.y,
		viewProjection_[0].eye.z);

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
	if (viewCameraState == 0) {
		model_->Draw(worldTransform_, viewProjection_[0], textureHandle_);
	}
	else if (viewCameraState == 1) {
		model_->Draw(worldTransform_, viewProjection_[1], textureHandle_);
	}
	else if (viewCameraState == 2) {
		model_->Draw(worldTransform_, viewProjection_[2], textureHandle_);
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