#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle){
	//NULLポインタチェック
	assert(model);

	//引数のデータをメンバ変数に代入
	model_ = model;
	textureHandle_ = textureHandle;

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.scale_ = { 1,1,10 };

	//ワールド座標変換の初期化
	worldTransform_.Initialize();
}

void Player::Update(){
	//移動限界座標
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 18.5f;

	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	if (input_->PushKey(DIK_RIGHT)) {
		move = { 0.5f,0,0 };
	}
	else if (input_->PushKey(DIK_LEFT)) {
		move = { -0.5f,0,0 };
	}
	else if (input_->PushKey(DIK_UP)) {
		move = { 0,0,0.5f };
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move = { 0,0,-0.5f };
	}

	worldTransform_.translation_ += move;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//行列変換
	MatrixConvert(worldTransform_);

	//行列を送信
	worldTransform_.TransferMatrix();

	//デバッグ
	debugText_->SetPos(20, 20);
	debugText_->Printf(
		"ray_Pos %f,%f,%f",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}

void Player::Draw(ViewProjection &viewProjection){
	//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void MatrixConvert(WorldTransform& worldTransform_){
	Matrix4 matScale;
	Matrix4 matRotX, matRotY, matRotZ;
	Matrix4 matRot;
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	Matrix4 matTrans2 = MathUtility::Matrix4Identity();
	matScale.ScaleSet(worldTransform_.scale_);
	matRotZ.RotZSet(worldTransform_.rotation_.z);
	matRotX.RotXSet(worldTransform_.rotation_.x);
	matRotY.RotYSet(worldTransform_.rotation_.y);
	matTrans.TransSet(worldTransform_.translation_);
	matRot = matRotZ * matRotX * matRotY;
	//合成
	worldTransform_.matWorld_ = matScale * matRot * matTrans;
}
