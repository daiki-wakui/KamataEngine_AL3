#pragma once
#include "WorldTransform.h"
#include "Model.h"

class PlayerBullet{
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	Vector3 velocity_;

	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
public:

	//初期化
	void Initialize(Model *model,const Vector3& position,const Vector3 &velocity,int color);
	//更新処理
	void Update();
	//描画処理
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }
};

