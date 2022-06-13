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

	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.Initialize();

		//x,y,z軸方向のスケーリングを設定
		worldTransform.scale_ = {1.0f, 1.0f, 1.0f};

		matScale.ScaleSet(worldTransform.scale_);
		matRotZ.RotZSet(worldTransform.rotation_.z);
		matRotX.RotXSet(worldTransform.rotation_.x);
		matRotY.RotYSet(worldTransform.rotation_.y);
		matTrans.TransSet(worldTransform.translation_);

		matRot = matRotZ * matRotX * matRotY;

		//単位行列の設定
		for (int i = 0; i < 4; i++) {
			worldTransform.matWorld_.m[i][i] = 1.0f;
		}
		
		//合成
		worldTransform.matWorld_ = matScale * matRot * matTrans;

		//行列の転送
		worldTransform.TransferMatrix();
	}
	//カメラ視点座標を設定
	//viewProjection_.eye = { 0.0f,0.0f,-50.0f };

	////カメラ注視点座標を設定
	//viewProjection_.target = { 0,0,0 };

	////カメラ上方向ベクトルを設定(右上45度指定)
	//viewProjection_.up = { cosf(RADIAN(180.0f) / 4.0f),sinf(RADIAN(180.0f) / 4.0f),0.0f};

	//viewProjection_.fovAngleY = RADIAN(10.0f);

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
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	Vector3 move = { 0,0,0 };

	if (input_->PushKey(DIK_RIGHT)) {
		move = { 0.05f,0,0 };
	}
	else if (input_->PushKey(DIK_LEFT)) {
		move = { -0.05f,0,0 };
	}
	else {
		move = { 0,0,0 };
	}

	worldTransforms_[0].translation_.x += move.x;
	matTrans.TransSet(worldTransforms_[0].translation_);

	for (int i = 0; i < 4; i++) {
		worldTransforms_->matWorld_.m[i][i] = 1.0f;
	}
	worldTransforms_->matWorld_ *= matTrans;
	worldTransforms_[0].TransferMatrix();

	//行列の再計算
	viewProjection_.UpdateMatrix();

	//デバッグ表示
	debugText_->SetPos(50, 30);
	debugText_->Printf("move %f",move.x);

	debugText_->SetPos(50, 50);
	debugText_->Printf("pos %f",worldTransforms_[0].translation_.x);

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
	model_->Draw(worldTransforms_[0], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[1], viewProjection_, textureHandle_);

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
