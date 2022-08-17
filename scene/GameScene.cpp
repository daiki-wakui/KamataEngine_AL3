#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>
#include <DirectXMath.h>

#define RADIAN(angle)angle*3.1415f/180
#define PAI 3.1415f

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
	delete player_;
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


	viewProjection_.eye = { 0.0f,15.0f,-25.0f };

	//ビュープロダクションの初期化
	viewProjection_.Initialize();



	worldObject_.Initialize();

	//軸方向表示の表示を無効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	player_ = new Player();
	player_->Initialize(model_,textureHandle_);

	object_ = new Object();
	object_->Initialize(model_, textureHandle_);
}


void GameScene::Update() {
	//原点
	Vector3 origin = { 0,0,0 };

	//レイ始点
	Vector3 start = { 0,0,1 };
	//レイ終点
	Vector3 end = { 0,0,1 };
	//レイ方向
	Vector3 dir = { 0,0,1 };

	//平面座標
	Vector3 distance = { 0,0,0 };
	float dis = 0.0f;

	//面法線
	Vector3 plane = { 0,0,1 };

	//始点と平面の距離
	float dist = 0.0f;

	float t;

	bool rayHit = false;

	start = player_->worldTransform_.translation_;
	start.z -= player_->worldTransform_.scale_.z;
	end = player_->worldTransform_.translation_;
	end.z += player_->worldTransform_.scale_.z;
	distance = object_->worldTransform_.translation_;

	float rayAndObjectDis;

	rayAndObjectDis = start.x - distance.x;
	
	//レイの方向Vec
	dir = start;
	dir.normalize();

	//面法線とレイの方向ベクトルの内積
	float d1 = plane.dot(dir);
	//面法線とレイの始点座標の内積
	float d2 = plane.dot(start);

	dist = d2 - dis;
	t = dist / -d1;

	if (t < 0) {
		rayHit = false;
	}
	else {
		rayHit = true;

		if (end.z < -object_->worldTransform_.scale_.z) {
			rayHit = false;
		}
		if (rayAndObjectDis > object_->worldTransform_.scale_.x) {
			rayHit = false;
		}
		if (rayAndObjectDis < -object_->worldTransform_.scale_.x) {
			rayHit = false;
		}
	}

	
	

	//オブジェクトの方向

	

	player_->Update();
	object_->Update();

	//行列の再計算
	viewProjection_.UpdateMatrix();

	//デバッグカメラの更新
	debugCamera_->Update();

	//デバッグ
	/*debugText_->SetPos(20, 40);
	debugText_->Printf(
		"ray_start %f,%f,%f",
		start.x,
		start.y,
		start.z);

	デバッグ
	debugText_->SetPos(20, 60);
	debugText_->Printf(
		"ray_end %f,%f,%f",
		end.x,
		end.y,
		end.z);

	debugText_->SetPos(20, 80);
	debugText_->Printf(
		"object %f,%f,%f",
		distance.x,
		distance.y,
		distance.z);

	debugText_->SetPos(20, 100);
	debugText_->Printf(
		"ray_dir %f,%f,%f",
		dir.x,
		dir.y,
		dir.z);*/

	if (rayHit == true) {
		debugText_->SetPos(600, 120);
		debugText_->Printf(
			"!!Ray!!");
	}
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
	player_->Draw(viewProjection_);
	object_->Draw(viewProjection_);
	

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
