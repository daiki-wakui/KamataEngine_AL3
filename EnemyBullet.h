#pragma once
#include "WorldTransform.h"
#include "Model.h"

class EnemyBullet{
private:
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	Vector3 velocity_;

	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
public:
	WorldTransform worldTransform_;

	//初期化
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	//更新処理
	void Update();
	//描画処理
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	Vector3 GetWorldPosition();

	//当たり判定
	void OnCollision();
};

