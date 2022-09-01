#pragma once
#include "WorldTransform.h"
#include "Model.h"

class EnemyBullet{
private:
	Model* model_ = nullptr;
	

	uint32_t textureHandle_ = 0;
	Vector3 velocity_;

	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
public:
	WorldTransform worldTransform_;

	//������
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void bossInitialize(Model* model, const Vector3& position, const Vector3& velocity);
	void bossInitialize2(Model* model, const Vector3& position, const Vector3& velocity);
	//�X�V����
	void Update();
	void bossUpdate();
	//�`�揈��
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	Vector3 GetWorldPosition();

	//�����蔻��
	void OnCollision();
};

