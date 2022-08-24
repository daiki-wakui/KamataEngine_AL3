#include "Enemy.h"
#include "Player.h"
#include <cassert>
#include <random>

//����������
void Enemy::Initialize(Model* model, const Vector3& position, uint32_t textureHandle){
	//�����V�[�h������
	std::random_device seed_gen;
	//�����Z���k�E�c�C�X�^�[�̗����G���W��
	std::mt19937_64 engine(seed_gen());
	//�����͈͂̎w��
	// 
	

	//���W
	std::uniform_real_distribution<float> posDistX(10.0f, 50.0f);
	std::uniform_real_distribution<float> posDist(-50.0f, 50.0f);

	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����̃f�[�^�������o�ϐ��ɑ��
	model_ = model;
	textureHandle_ = TextureManager::Load("enemy.png");

	//���[���h���W�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.scale_ = { 3,3,3 };
	worldTransform_.translation_ = { posDist(engine),0.0f,80.0f };

	
}

//�X�V����
void Enemy::Update(){
	//�f�X�t���O���������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
	});

	bulletTimer--;

	if (bulletTimer < 0) {
		Fire();
		bulletTimer = 70;
	}

	Vector3 move = { 0.0f,0.0f,0.0f };

	switch (phase_) {
	case Phase::Approach:
		move = { 0.0f,0.0f,-0.25f };
		//worldTransform_.translation_ += move;

		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}

		break;
	case Phase::Leave:
		move = { -0.25f,0.25f,-0.25f };
		worldTransform_.translation_ += move;


		break;
	default:
		break;
	}

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	worldTransform_.MatrixConvert();
	worldTransform_.TransferMatrix();
}

//�`�揈��
void Enemy::Draw(const ViewProjection& viewProjection){
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire()
{
	playerPos = player_->GetWorldPosition();
	enemyPos = GetWorldPosition();
	differenceVec = enemyPos - playerPos;
	differenceVec.normalize();
	
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(differenceVec);

	velocity.multiplyMat4(worldTransform_.matWorld_);

	//�e�̐����Ə�����
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//�e��o�^����
	bullets_.push_back(std::move(newBullet));
}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::OnCollision()
{
}
