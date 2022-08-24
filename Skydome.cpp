#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model){
	//NULLポインタチェック
	assert(model);

	//引数のデータをメンバ変数に代入
	model_ = model;

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = { 0,0,0 };

	worldTransform_.scale_ = { 0,0,0 };
	//ワールド座標変換の初期化
	worldTransform_.Initialize();
}

void Skydome::Update(){
	Vector3 addSize = { 3,3,3 };

	if (worldTransform_.scale_.x >= 50) {
		addSize = { 2,2,2 };
		
	}
	if (worldTransform_.scale_.x >= 100) {
		addSize = { 1.5,1.5,1.5 };
		
	}
	if (worldTransform_.scale_.x >= 150) {
		addSize = { 0,0,0 };
		
	}

	worldTransform_.rotation_.y += 0.0002f;

	worldTransform_.scale_ += addSize;

	//行列変換
	worldTransform_.MatrixConvert();
	//行列を送信
	worldTransform_.TransferMatrix();
}

void Skydome::UpdateN()
{
	worldTransform_.scale_ = { 200,200,200 };
	//行列変換
	worldTransform_.MatrixConvert();
	//行列を送信
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(ViewProjection& viewProjection){
	model_->Draw(worldTransform_, viewProjection);
}
