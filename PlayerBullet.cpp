#include "PlayerBullet.h"
#include <cassert>

//player�̏�����
void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity ,int color)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����̃f�[�^�������o�ϐ��ɑ��
	model_ = model;
	velocity_ = velocity;

	if (color == 0) {
		textureHandle_ = TextureManager::Load("black.png");
	}
	if (color == 1) {
		textureHandle_ = TextureManager::Load("white.png");
	}
	

	//���[���h���W�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	worldTransform_.translation_.x -= 7;
	worldTransform_.translation_.y += 5;
}

void PlayerBullet::Initialize2(Model* model, const Vector3& position, const Vector3& velocity, int color){
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����̃f�[�^�������o�ϐ��ɑ��
	model_ = model;
	velocity_ = velocity;

	if (color == 0) {
		textureHandle_ = TextureManager::Load("black.png");
	}
	if (color == 1) {
		textureHandle_ = TextureManager::Load("white.png");
	}


	//���[���h���W�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	worldTransform_.translation_.x += 7;
	worldTransform_.translation_.y += 5;
}

//�X�V����
void PlayerBullet::Update()
{
	worldTransform_.translation_ += velocity_;

	worldTransform_.MatrixConvert();
	worldTransform_.TransferMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

//�`�揈��
void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 PlayerBullet::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}
