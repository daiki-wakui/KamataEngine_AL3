#pragma once
#include "WorldTransform.h"
#include "Model.h"

enum class Phase {
	Approach,	//接近する
	Leave,	//離脱する
};

class Enemy{
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;

	Phase phase_ = Phase::Approach;

public:

	//初期化
	void Initialize(Model* model, const Vector3& position, uint32_t textureHandle);
	//更新処理
	void Update();
	//描画処理
	void Draw(const ViewProjection& viewProjection);
};

