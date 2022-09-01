#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "EnemyBullet.h"
#include "Input.h"
#include <memory>
#include <list>
#include "DebugText.h"

class Player;

class Enemy{
private:
	DebugText* debugText_ = nullptr;

	Model* model_ = nullptr;
	Model* model2_ = nullptr;
	Model* model3_ = nullptr;


	uint32_t textureHandle_ = 0;
	Input* input_ = nullptr;

	int bulletTimer = 50;
	Player* player_ = nullptr;
	Vector3 playerPos;
	Vector3 enemyPos;
	Vector3	differenceVec;

	int color_ = 0;
	int hp = 1;

	int isBoss = 0;
	int bossAction = 0;
	int actionTimer = 0;
	int switchTimer = 0;
	int changeSpeed = 1.0f;
	float angle = 0;

	bool isDead_ = false;

	Vector3 moveSpeed = { 0,0,0 };
	Vector3 addSpeed = { 0,0,0 };
	Vector3 angleTrans = { 0,0,0 };
	int addTimer = 0;

public:
	WorldTransform worldTransform_;

	//初期化
	void Initialize(int& phase,int color,Model* model, Model* model2, Model* model3, const Vector3& position, uint32_t textureHandle);
	void BossInitialize(int& phase,int color, Model* model, Model* model2, Model* model3, const Vector3& position, uint32_t textureHandle);

	//更新処理
	void Update(int& phase);
	//描画処理
	void Draw(const ViewProjection& viewProjection);

	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	void Fire();
	void bossFire();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	//当たり判定
	void OnCollision();

	bool IsDead() const { return isDead_; }

	int GetEnemyColor() { return color_; }

	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }
};

