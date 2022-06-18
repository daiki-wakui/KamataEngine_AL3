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
	Matrix4 matScale;
	Matrix4 matRotX, matRotY, matRotZ;
	Matrix4 matRot;
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	for (int i = 0; i < 9; i++) {
		worldTransforms_Under_[i].Initialize();
		worldTransforms_Top_[i].Initialize();

		//x,y,z軸方向のスケーリングを設定
		worldTransforms_Under_[i].scale_ = { 5.0f, 5.0f, 5.0f };
		worldTransforms_Top_[i].scale_ = { 5.0f, 5.0f, 5.0f };

		// x,y,z軸周りに平行移動を設定
		worldTransforms_Under_[i].translation_ = { 40.0f - (10.0f * i), -20.0f , 0 };
		worldTransforms_Top_[i].translation_ = { 40.0f - (10.0f * i), 20.0f , 0 };

		//合成
		worldTransforms_Under_[i].MatrixConvert();
		worldTransforms_Top_[i].MatrixConvert();

		//行列の転送
		worldTransforms_Under_[i].TransferMatrix();
		worldTransforms_Top_[i].TransferMatrix();
	}

	//カメラ視点座標を設定
	viewProjection_.eye = { 0.0f,10.0f,-50.0f };

	//カメラ注視点座標を設定
	viewProjection_.target = { 0,0,0 };

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

	//行列の再計算
	viewProjection_.UpdateMatrix();

	//デバッグ表示
	/*debugText_->SetPos(50, 30);
	debugText_->Printf(
		"eye W/S:(%f,%f,%f)",
		viewProjection_.eye.x,
		viewProjection_.eye.y,
		viewProjection_.eye.z);*/

	

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

	for (int i = 0; i < 9; i++) {
		model_->Draw(worldTransforms_Under_[i], viewProjection_, textureHandle_);
		model_->Draw(worldTransforms_Top_[i], viewProjection_, textureHandle_);
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
