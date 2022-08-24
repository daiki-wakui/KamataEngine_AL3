#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "DebugText.h"

class Skydome{
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
public:

	void Initialize(Model* model);
	void Update();
	void UpdateN();
	void Draw(ViewProjection& viewProjection);
};

