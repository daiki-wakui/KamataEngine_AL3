#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

#define Radian(angle)angle*3.1415f/180

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

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

	//ビュープロダクションの初期化
	viewProjection_.Initialize();

	//軸方向表示の表示を無効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//ライン描画が参照するビュープロジェクションを指定する
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	//スケール
	worldTransform_.scale_ = { 5.0f,5.0f,5.0f };

	//worldTransform_.rotation_ = { 0.0f,0.0f,Radian(45.0f) };
	//worldTransform_.rotation_ = { Radian(45.0f),0.0f,0.0f };
	worldTransform_.rotation_ = { Radian(45.0f),Radian(45.0f),Radian(45.0f) };

	worldTransform_.translation_ = { 10,10,10 };

	Matrix4 matScale;
	Matrix4 matRot;
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	//回転行列
	Matrix4 matRotX, matRotY, matRotZ;

	matScale.ScaleSet(worldTransform_.scale_);

	matRotZ.RotZSet(worldTransform_.rotation_.z);
	matRotX.RotXSet(worldTransform_.rotation_.x);
	matRotY.RotYSet(worldTransform_.rotation_.y);

	//スケール
	/*matScale.m[0][0] = worldTransform_.scale_.x;
	matScale.m[1][1] = worldTransform_.scale_.y;
	matScale.m[2][2] = worldTransform_.scale_.z;
	matScale.m[3][3] = 1.0f;*/

	/*matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform_.rotation_.x);
	matRotX.m[0][0] = 1.0f;
	matRotX.m[3][3] = 1.0f;*/

	//matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
	//matRotY.m[0][2] = -sin(worldTransform_.rotation_.y);
	//matRotY.m[2][0] = sin(worldTransform_.rotation_.y);
	//matRotY.m[2][2] = cos(worldTransform_.rotation_.y);
	//matRotY.m[1][1] = 1.0f;
	//matRotY.m[3][3] = 1.0f;

	//matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
	//matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
	//matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
	//matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);
	//matRotZ.m[2][2] = 1.0f;
	//matRotZ.m[3][3] = 1.0f;

	matTrans.m[3][0] = worldTransform_.translation_.x;
	matTrans.m[3][1] = worldTransform_.translation_.y;
	matTrans.m[3][2] = worldTransform_.translation_.z;
	matTrans.m[3][3] = 1.0f;

	matRot = matRotZ * matRotX * matRotY;

	//単位行列の設定
	for (int i = 0; i < 4; i++) {
		worldTransform_.matWorld_.m[i][i] = 1.0f;
	}
	//worldTransform_.matWorld_ *= matScale;
	worldTransform_.matWorld_ *= matRot ;
	//worldTransform_.matWorld_ *= matTrans;

	//worldTransform_.matWorld_ = matScale * matRot * matTrans;

	//行列の転送
	worldTransform_.TransferMatrix();
}


void GameScene::Update() {

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
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

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
