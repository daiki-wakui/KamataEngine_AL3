#include "EnemyBullet.h"
#include <cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity){
	//NULLポインタチェック
	assert(model);

	//引数のデータをメンバ変数に代入
	model_ = model;
	velocity_ = velocity;
	textureHandle_ = TextureManager::Load("black.png");

	//ワールド座標変換の初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
}

void EnemyBullet::Update()
{
	worldTransform_.translation_ -= velocity_/2;

	worldTransform_.MatrixConvert();
	worldTransform_.TransferMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
