#pragma once
#include "WorldTransform.h"
#include "Model.h"

enum class Phase {
	Approach,	//Ú‹ß‚·‚é
	Leave,	//—£’E‚·‚é
};

class Enemy{
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;

	Phase phase_ = Phase::Approach;

public:

	//‰Šú‰»
	void Initialize(Model* model, const Vector3& position, uint32_t textureHandle);
	//XVˆ—
	void Update();
	//•`‰æˆ—
	void Draw(const ViewProjection& viewProjection);
};

