#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "DebugText.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

class Player{
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Model* model2_ = nullptr;
	uint32_t textureHandle_ = 0;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	//キャラクターの移動ベクトル
	Vector3 move;
	//PlayerBullet* bullet_ = nullptr;
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	bool isChange = false;
	int state = 2;

public:
	int playerColor = 0;

	void Initialize(Model* model, Model* model2);
	void Update();
	void Draw(ViewProjection &viewProjection);
	void MatrixConvert();
	void Move();
	void Rotate();
	void Attack();

	Vector3 GetWorldPosition();
};