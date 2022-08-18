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

	worldTransform_.scale_ = { 70,70,70 };
	//ワールド座標変換の初期化
	worldTransform_.Initialize();
}

void Skydome::Update(){
	//行列変換
	worldTransform_.MatrixConvert();
	//行列を送信
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(ViewProjection& viewProjection){
	model_->Draw(worldTransform_, viewProjection);
}
