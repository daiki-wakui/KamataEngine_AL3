#include "Enemy.h"
#include <cassert>

//初期化処理
void Enemy::Initialize(Model* model, const Vector3& position, uint32_t textureHandle){
	//NULLポインタチェック
	assert(model);

	//引数のデータをメンバ変数に代入
	model_ = model;
	textureHandle_ = TextureManager::Load("enemy.png");

	//ワールド座標変換の初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = {0.0f,0.0f,100.0f};
}

//更新処理
void Enemy::Update(){
	Vector3 move = { 0.0f,0.0f,-0.25f };

	worldTransform_.translation_ += move;

	worldTransform_.MatrixConvert();
	worldTransform_.TransferMatrix();
}

//描画処理
void Enemy::Draw(const ViewProjection& viewProjection){
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
