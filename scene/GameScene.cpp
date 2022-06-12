#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

#define RADIAN(angle)angle*3.1415f/180
#define PAI 3.1415f

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
	Matrix4 matScale;
	Matrix4 matRotX, matRotY, matRotZ;
	Matrix4 matRot;
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	worldTransform_.Initialize();

	//x,y,z軸方向のスケーリングを設定
	worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };
	// x,y,z軸周りの回転角を設定
	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };
	// x,y,z軸周りに平行移動を設定
	worldTransform_.translation_ = { 0.0f,0.0f,0.0f };

	matScale.ScaleSet(worldTransform_.scale_);
	matRotZ.RotZSet(worldTransform_.rotation_.z);
	matRotX.RotXSet(worldTransform_.rotation_.x);
	matRotY.RotYSet(worldTransform_.rotation_.y);
	matTrans.TransSet(worldTransform_.translation_);

	matRot = matRotZ * matRotX * matRotY;

	//単位行列の設定
	for (int i = 0; i < 4; i++) {
		worldTransform_.matWorld_.m[i][i] = 1.0f;
	}
	//合成
	worldTransform_.matWorld_ = matScale * matRot * matTrans;
	//行列の転送
	worldTransform_.TransferMatrix();


	//カメラ視点座標を設定
	viewProjection_.eye = { 0.0f,15.0f,-25.0f };

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
	//正面ベクトルの長さの成分
	Vector3 length = { 0, 0, 10 };
	//正面ベクトルの長さ(大きさ)
	float frontVecLength = 0;
	//正面ベクトル
	Vector3 frontVec = { 0, 0, 0 };
	//始点
	Vector3 start = { worldTransform_.translation_ };
	//終点
	Vector3 end = { 0, 0, 0 };
	//オブジェクトの移動ベクトル
	Vector3 move = { 0, 0, 0 };
	//オブジェクトの回転ベクトル
	Vector3 rotaMove = { 0, 0, 0 };

	Vector3 tmpVecY = { 0,1,0 };

	Vector3 moveXVec = { 0,0,0 };

	Vector3 eyeMove = { 0,0,0 };
	float kEyeSpeed = 0.5f;

	//オブジェクトの回転スピード
	const float rotaSpeed = 0.05f;

	//モード切り替え
	if (input_->TriggerKey(DIK_Q)) {
		if (isMode == 0) {
			isMode = 1;
		}
		else {
			isMode = 0;
		}
	}

	if (isMode == 0) {
		frontVec.x = worldTransform_.translation_.x - viewProjection_.eye.x;
		frontVec.z = worldTransform_.translation_.z - viewProjection_.eye.z;

		frontVec.normalize();

		if (input_->PushKey(DIK_W)) {
			worldTransform_.translation_ += frontVec/5;
		}
		else if (input_->PushKey(DIK_S)) {
			worldTransform_.translation_ -= frontVec/5;
		}

		moveXVec = tmpVecY.cross(frontVec);

		if (input_->PushKey(DIK_D)) {
			worldTransform_.translation_.x += moveXVec.x/5;
		}
		else if (input_->PushKey(DIK_A)) {
			worldTransform_.translation_.x -= moveXVec.x/5;
		}
	}

	//　バイオ歩き　//
	if (isMode == 1) {
		//オブジェクトの回転
		if (input_->PushKey(DIK_D)) {
			rotaMove = { 0, rotaSpeed, 0 };
		}
		else if (input_->PushKey(DIK_A)) {
			rotaMove = { 0, -rotaSpeed, 0 };
		}

		//終点座標を設定
		end.x = start.x + length.x;
		end.y = start.y + length.y;
		end.z = start.z + length.z;

		//回転を考慮した座標を設定
		end.x = start.x + sinf(worldTransform_.rotation_.y);
		end.z = start.z + cosf(worldTransform_.rotation_.y);

		//始点と終点から正面ベクトルを求める
		frontVec.x = end.x - start.x;
		frontVec.y = end.y - start.y;
		frontVec.z = end.z - start.z;

		//正面ベクトルの長さ
		frontVecLength = sqrtf(
			(length.x * length.x) +
			(length.y * length.y) +
			(length.z * length.z));

		//正面ベクトルの正規化
		frontVec.x /= frontVecLength;
		frontVec.y /= frontVecLength;
		frontVec.z /= frontVecLength;

		//始点座標に正面ベクトルの値を加算or減算
		if (input_->PushKey(DIK_W)) {
			start.x += frontVec.x;
			start.y += frontVec.y;
			start.z += frontVec.z;
		}
		else if (input_->PushKey(DIK_S)) {
			start.x -= frontVec.x;
			start.y -= frontVec.y;
			start.z -= frontVec.z;
		}

		//ベクトルの加算
		worldTransform_.translation_.x = start.x;
		worldTransform_.translation_.z = start.z;

		//回転
		worldTransform_.rotation_.y += rotaMove.y;
	}//　バイオ歩き　//
	

	Matrix4 matScale;
	Matrix4 matRotX, matRotY, matRotZ;
	Matrix4 matRot;
	Matrix4 matTrans = MathUtility::Matrix4Identity();

	matScale.ScaleSet(worldTransform_.scale_);
	matRotZ.RotZSet(worldTransform_.rotation_.z);
	matRotX.RotXSet(worldTransform_.rotation_.x);
	matRotY.RotYSet(worldTransform_.rotation_.y);
	matTrans.TransSet(worldTransform_.translation_);

	matRot = matRotZ * matRotX * matRotY;

	//単位行列の設定
	for (int i = 0; i < 4; i++) {
		worldTransform_.matWorld_.m[i][i] = 1.0f;
	}
	//合成
	worldTransform_.matWorld_ = matScale * matRot * matTrans;

	//行列の送信
	worldTransform_.TransferMatrix();
	viewProjection_.UpdateMatrix();

	//デバッグ表示
	debugText_->SetPos(10, 10);
	if (isMode == 0) {
		debugText_->Printf("CameraMove Mode", isMode);
	}
	else {
		debugText_->Printf("BioWalking Mode", isMode);
	}
	debugText_->SetPos(10, 30);
	debugText_->Printf("Q Key : ModeSwitch");
	debugText_->SetPos(10, 50);
	debugText_->Printf("WASD Key : PlayerMove");
	debugText_->SetPos(10, 70);
	debugText_->Printf("Mouse : DebugCamera");
	debugText_->SetPos(10, 90);
	debugText_->Printf(
		"pos:(%f,%f,%f)",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

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
	
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	

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
