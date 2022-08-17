#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "EnemyBullet.h"
#include "Input.h"
#include <memory>
#include <list>

class Player;

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

	Input* input_ = nullptr;

	int bulletTimer = 50;

	Player* player_ = nullptr;

	Vector3 playerPos;
	Vector3 enemyPos;
	Vector3	differenceVec;
public:

	//‰Šú‰»
	void Initialize(Model* model, const Vector3& position, uint32_t textureHandle);
	//XVˆ—
	void Update();
	//•`‰æˆ—
	void Draw(const ViewProjection& viewProjection);

	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	void Fire();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();
};

