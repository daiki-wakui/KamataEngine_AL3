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
