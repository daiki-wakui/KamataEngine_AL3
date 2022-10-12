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
	Model* model3_ = nullptr;
	Model* model4_ = nullptr;

	uint32_t textureHandle_ = 0;

	//Model* bulletModel_b = nullptr;
	//Model* bulletModel_w = nullptr;

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

	int colorCoolTime = 0;

	
	bool isDead_ = false;

	//3Dレティクル用ワールド座標
	WorldTransform worldTransform3DReticle_;

public:
	std::unique_ptr<Sprite> sprite2DReticle_;

	WorldTransform worldTransform_;
	int playerColor = 0;

	void Initialize(Model* model, Model* model2, Model* model3, Model* model4);
	void Update(int start);
	void Draw(ViewProjection &viewProjection);
	void MatrixConvert();
	void Move();
	void Rotate();
	void Attack();

	Vector3 GetWorldPosition();

	//当たり判定
	void OnCollision();
	bool IsDead() const { return isDead_; }

	void DrawUI();

	int GetCoolTime() { return coolTime; }

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }
};