#include "Enemy.h"
#include <cassert>

//����������
void Enemy::Initialize(Model* model, const Vector3& position, uint32_t textureHandle){
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����̃f�[�^�������o�ϐ��ɑ��
	model_ = model;
	textureHandle_ = TextureManager::Load("enemy.png");

	//���[���h���W�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.translation_ = {0.0f,0.0f,100.0f};
}

//�X�V����
void Enemy::Update(){
	Vector3 move = { 0.0f,0.0f,-0.25f };

	worldTransform_.translation_ += move;

	worldTransform_.MatrixConvert();
	worldTransform_.TransferMatrix();
}

//�`�揈��
void Enemy::Draw(const ViewProjection& viewProjection){
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
