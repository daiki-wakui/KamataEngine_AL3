#include "Object.h"
#include <cassert>

void Object::Initialize(Model* model, uint32_t textureHandle){
	//NULLポインタチェック
	assert(model);

	//引数のデータをメンバ変数に代入
	model_ = model;
	textureHandle_ = textureHandle;

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.scale_ = { 2,2,2 };
	worldTransform_.translation_ = { 0,0,0 };

	//ワールド座標変換の初期化
	worldTransform_.Initialize();
}

void Object::Update()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	if (input_->PushKey(DIK_D)) {
		move = { 0.5f,0,0 };
	}
	else if (input_->PushKey(DIK_A)) {
		move = { -0.5f,0,0 };
	}

	worldTransform_.translation_ += move;


	//行列変換
	MatrixConvert(worldTransform_);

	//行列を送信
	worldTransform_.TransferMatrix();
}

void Object::Draw(ViewProjection& viewProjection)
{
	//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

