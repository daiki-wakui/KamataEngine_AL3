#include "Enemy.h"
#include "Player.h"
#include <cassert>
#include <random>

//初期化処理
void Enemy::Initialize(int color,Model* model, Model* model2, Model* model3, const Vector3& position, uint32_t textureHandle){
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//乱数範囲の指定
	// 
	

	//座標
	std::uniform_real_distribution<float> posDistX(10.0f, 50.0f);
	std::uniform_real_distribution<float> posDist(-50.0f, 50.0f);

	//NULLポインタチェック
	assert(model);

	//引数のデータをメンバ変数に代入
	model_ = model;
	model2_ = model2;
	model3_ = model3;

	if (color == 0) {
		textureHandle_ = TextureManager::Load("black.png");
	}

	if (color == 1) {
		textureHandle_ = TextureManager::Load("white.png");
	}

	color_ = color;

	//ワールド座標変換の初期化
	worldTransform_.Initialize();

	worldTransform_.scale_ = { 3,3,3 };
	worldTransform_.translation_ = { posDist(engine),posDist(engine),80.0f };

	
}

//更新処理
void Enemy::Update(){
	//デスフラグが立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
	});

	bulletTimer--;

	if (bulletTimer < 0) {
		Fire();
		bulletTimer = 70;
	}

	Vector3 move = { 0.0f,0.0f,0.0f };

	switch (phase_) {
	case Phase::Approach:
		move = { 0.0f,0.0f,-0.25f };
		//worldTransform_.translation_ += move;

		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}

		break;
	case Phase::Leave:
		move = { -0.25f,0.25f,-0.25f };
		worldTransform_.translation_ += move;


		break;
	default:
		break;
	}

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	worldTransform_.MatrixConvert();
	worldTransform_.TransferMatrix();
}

//描画処理
void Enemy::Draw(const ViewProjection& viewProjection){
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire()
{
	playerPos = player_->GetWorldPosition();
	enemyPos = GetWorldPosition();
	differenceVec = enemyPos - playerPos;
	differenceVec.normalize();
	
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(differenceVec);

	velocity.multiplyMat4(worldTransform_.matWorld_);

	if (color_ == 0) {
		//弾の生成と初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(model2_, worldTransform_.translation_, velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
	if (color_ == 1) {
		//弾の生成と初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(model3_, worldTransform_.translation_, velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}
	
}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::OnCollision()
{
}
