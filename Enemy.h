#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Enemy{
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;


public:

	//初期化
	void Initialize(Model* model, const Vector3& position, uint32_t textureHandle);
	//更新処理
	void Update();
	//描画処理
	void Draw(const ViewProjection& viewProjection);
};

