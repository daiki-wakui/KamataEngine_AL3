#include "PlayerBullet.h"
#include <cassert>

//playerの初期化
void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
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

//更新処理
void PlayerBullet::Update()
{
	worldTransform_.translation_ += velocity_;

	worldTransform_.MatrixConvert();
	worldTransform_.TransferMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

//描画処理
void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
