#include "EnemyBullet.h"
#include <cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity){
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����̃f�[�^�������o�ϐ��ɑ��
	model_ = model;
	velocity_ = velocity;
	textureHandle_ = TextureManager::Load("black.png");

	//���[���h���W�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
}

void EnemyBullet::Update()
{
	worldTransform_.translation_ -= velocity_/5;

	worldTransform_.MatrixConvert();
	worldTransform_.TransferMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

Vector3 EnemyBullet::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void EnemyBullet::OnCollision(){
	isDead_ = true;
}
