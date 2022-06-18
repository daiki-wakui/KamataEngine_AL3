#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& position)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����̃f�[�^�������o�ϐ��ɑ��
	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");

	//���[���h���W�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
}

void PlayerBullet::Update()
{
	worldTransform_.MatrixConvert();
	worldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
