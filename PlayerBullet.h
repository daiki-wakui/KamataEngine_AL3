#pragma once
#include "WorldTransform.h"
#include "Model.h"

class PlayerBullet{
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;

public:

	//èâä˙âª
	void Initialize(Model *model,const Vector3& position);

	void Update();

	void Draw(const ViewProjection& viewProjection);
};

