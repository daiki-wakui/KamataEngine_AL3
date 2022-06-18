#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& position)
{
	//NULLポインタチェック
	assert(model);

	//引数のデータをメンバ変数に代入
	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");

	//ワールド座標変換の初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
}

void PlayerBullet::Update()
{
	worldTransform_.MatrixConvert();
	worldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
