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
	int coolTime = 5;
	float dashSpeed = 4.0f;

public:
	WorldTransform worldTransform_;
	int playerColor = 0;

	void Initialize(Model* model, Model* model2);
	void Update();
	void Draw(ViewProjection &viewProjection);
	void MatrixConvert();
	void Move();
	void Rotate();
	void Attack();

	Vector3 GetWorldPosition();

	//当たり判定
	void OnCollision();

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }
};