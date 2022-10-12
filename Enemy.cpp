#include "Enemy.h"
#include "Player.h"
#include <cassert>
#include <random>

#define PI 3.1415f

//����������
void Enemy::Initialize(int& phase,int color,Model* model, Model* model2, Model* model3, const Vector3& position, uint32_t textureHandle){
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

	debugText_ = DebugText::GetInstance();

	//�����̃f�[�^�������o�ϐ��ɑ��
	model_ = model;
	model2_ = model2;
	model3_ = model3;

	if (color == 0) {
		textureHandle_ = TextureManager::Load("black.png");
	}

	if (color == 1) {
		textureHandle_ = TextureManager::Load("white.png");
	}

	color_ = color;

	bullets_.clear();

	//���[���h���W�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.scale_ = { 3,3,3 };
	worldTransform_.translation_ = position;

	if (phase == 0) {
		moveSpeed = { 0.1f,0,0 };
	}
	if (phase == 1) {
		moveSpeed = { -0.1f,0,0 };
	}
	if (phase == 2) {
		addSpeed.y = 0.005f;
		if (color_ == 0) {
			moveSpeed = { -0.2f,0.2f,0 };
		}
		else if (color_ == 1) {
			moveSpeed = { 0.2f,-0.2f,0 };
		}
	}
	if (phase == 3) {
		addSpeed.y = 0.005f;
		moveSpeed = { 0.25f,0.3f,0.2f };
	}
	if (phase == 4) {
		addSpeed.y = 0.005f;
		if (color_ == 0) {
			moveSpeed = { 0.2f,0.3f,0.1f };
		}
		else if (color_ == 1) {
			moveSpeed = { -0.2f,-0.3f,-0.1f };
		}
	}
}

void Enemy::BossInitialize(int& phase,int color, Model* model, Model* model2, Model* model3, const Vector3& position, uint32_t textureHandle){
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����̃f�[�^�������o�ϐ��ɑ��
	model_ = model;
	model2_ = model2;
	model3_ = model3;

	if (color == 0) {
		textureHandle_ = TextureManager::Load("black.png");
	}

	if (color == 1) {
		textureHandle_ = TextureManager::Load("white.png");
	}

	color_ = color;
	debugText_ = DebugText::GetInstance();
	input_ = Input::GetInstance();

	//���[���h���W�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.scale_ = { 10,10,10 };
	worldTransform_.translation_ = position;

	hp = 75;
	isBoss = 1;

	/*addSpeed.x = 0.005f;
	addSpeed.y = 0.005f;*/

	addSpeed = { 0.015f,0.0f,0.0f };

	if (color_ == 0) {
		moveSpeed = { 0.7f,0.0f,0.0f };
	}
	else if (color_ == 1) {
		moveSpeed = { -0.7f,0.0f,0.0f };
	}
}

//�X�V����
void Enemy::Update(int& phase){

	//�f�X�t���O���������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
	});

	bulletTimer--;

	//�e����
	if (isBoss == 0) {
		if (bulletTimer < 0) {
			Fire();
			bulletTimer = 70;
		}
	}
	else {
		if (bulletTimer < 0) {
			/*if (bossAction != 0) {
				Fire();
				bulletTimer = 10;
			}*/
			if (bossAction == 1) {
				Fire();
				bulletTimer = 3;
			}
			if (bossAction == 2) {
				bossFire();
				bulletTimer = 15;
			}
		}
	}

	const float kMoveLimitX = 80.0f;
	const float kMoveLimitY = 20.0f;
	const float kMoveLimitZ = 60.0f;
	
//	moveSpeed.x = 1.0f;

	switch (phase) {
	//��3��
	case 1:
		worldTransform_.translation_.x += moveSpeed.x;

		if (worldTransform_.translation_.x >= kMoveLimitX) {
			moveSpeed.x = -moveSpeed.x;
		}
		else if(worldTransform_.translation_.x <= -kMoveLimitX) {
			moveSpeed.x = -moveSpeed.x;
		}
		break;
	//��3��
	case 2:
		worldTransform_.translation_.x += moveSpeed.x;

		if (worldTransform_.translation_.x >= kMoveLimitX) {
			moveSpeed.x = -moveSpeed.x;
		}
		else if (worldTransform_.translation_.x <= -kMoveLimitX) {
			moveSpeed.x = -moveSpeed.x;
		}
		break;
	//��2��2
	case 3:
		worldTransform_.translation_ += moveSpeed;

		//���ƍ��ŉ����l�ς���
		if (color_ == 1) {
			moveSpeed.y += addSpeed.y;
		}
		else {
			moveSpeed.y -= addSpeed.y;
		}
		

		if (moveSpeed.y >= 0.2f) {
			addSpeed.y = -addSpeed.y;
		}
		else if (moveSpeed.y <= -0.2f) {
			addSpeed.y = -addSpeed.y;
		}

		if (worldTransform_.translation_.x >= kMoveLimitX) {
			moveSpeed.x = -moveSpeed.x;
		}
		else if (worldTransform_.translation_.x <= -kMoveLimitX) {
			moveSpeed.x = -moveSpeed.x;
		}

		break;
	//����6��(3:3)
	case 4:
		worldTransform_.translation_ += moveSpeed;
		moveSpeed.y -= addSpeed.y;

		if (moveSpeed.y >= 0.3f) {
			addSpeed.y = -addSpeed.y;
		}
		else if (moveSpeed.y <= -0.3f) {
			addSpeed.y = -addSpeed.y;
		}

		if (worldTransform_.translation_.x >= kMoveLimitX) {
			moveSpeed.x = -moveSpeed.x;
		}
		else if (worldTransform_.translation_.x <= -kMoveLimitX) {
			moveSpeed.x = -moveSpeed.x;
		}
		if (worldTransform_.translation_.z >= kMoveLimitZ) {
			moveSpeed.z = -moveSpeed.z;
		}
		else if (worldTransform_.translation_.z <= -30) {
			moveSpeed.z = -moveSpeed.z;
		}

		break;
	//����10��(5:5)
	case 5:
		worldTransform_.translation_ += moveSpeed;
		if (color_ == 1) {
			moveSpeed.y += addSpeed.y;
		}
		else {
			moveSpeed.y -= addSpeed.y;
		}

		if (moveSpeed.y >= 0.3f) {
			addSpeed.y = -addSpeed.y;
		}
		else if (moveSpeed.y <= -0.3f) {
			addSpeed.y = -addSpeed.y;
		}

		if (worldTransform_.translation_.x >= kMoveLimitX) {
			moveSpeed.x = -moveSpeed.x;
		}
		else if (worldTransform_.translation_.x <= -kMoveLimitX) {
			moveSpeed.x = -moveSpeed.x;
		}
		if (worldTransform_.translation_.z >= kMoveLimitZ) {
			moveSpeed.z = -moveSpeed.z;
		}
		else if (worldTransform_.translation_.z <= -30) {
			moveSpeed.z = -moveSpeed.z;
		}

		break;
	//�{�X
	case 6:
		if (bossAction != 0) {
			switchTimer++;
		}
		//�o��
		if (bossAction == 0) {
			worldTransform_.translation_.z -= 0.5f;
			if (worldTransform_.translation_.z <= 90.0f) {
				bossAction = 2;
			}
		}
		//��]�U��
		if (bossAction == 1) {
			angle += 0.05f;
			angle = fmodf(angle, PI * 2.0f);

			if (color_ == 0) {
				angleTrans.x = cosf(angle)/2;
				angleTrans.y = sinf(angle)/2;
				worldTransform_.translation_.x += angleTrans.x;
				worldTransform_.translation_.y += angleTrans.y;
			}
			else if (color_ == 1) {
				angleTrans.x = -cosf(angle)/2;
				angleTrans.y = -sinf(angle)/2;
				worldTransform_.translation_.x += angleTrans.x;
				worldTransform_.translation_.y += angleTrans.y;
			}
			
			if (switchTimer >= 350) {
				bossAction = 2;
				switchTimer = 0;
			}
		}
		//���V
		if (bossAction == 2) {
			worldTransform_.translation_ += moveSpeed;

			if (color_ == 1) {
				moveSpeed += addSpeed;
			}
			else {
				moveSpeed -= addSpeed;
			}

			if (moveSpeed.x >= 0.7f) {
				addSpeed = -addSpeed;
			}
			else if (moveSpeed.x <= -0.7f) {
				addSpeed = -addSpeed;
			}

			/*actionTimer++;
			if (actionTimer >= 30) {
				if (color_ == 0) {
					worldTransform_.translation_.x -= changeSpeed;
				}
				else if (color_ == 1) {
					worldTransform_.translation_.x += changeSpeed;
				}
			}
			if (actionTimer >= 80) {
				actionTimer = 0;
				changeSpeed = -changeSpeed;
			}*/

			if (switchTimer >= 250) {
				bossAction = 1;
				switchTimer = 0;
			}
		}

		break;
	default:
		break;
	}

	if (isBoss == 0) {
		for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
			bullet->Update();
		}
	}
	else {
		for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
			bullet->bossUpdate();
		}
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

	if (color_ == 0) {
		//�e�̐����Ə�����
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(model2_, worldTransform_.translation_, velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
	if (color_ == 1) {
		//�e�̐����Ə�����
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(model3_, worldTransform_.translation_, velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}
	
}

void Enemy::bossFire(){
	playerPos = player_->GetWorldPosition();
	enemyPos = GetWorldPosition();
	differenceVec = enemyPos - playerPos;
	differenceVec.normalize();

	const float kBulletSpeed = -1.0f;
	Vector3 velocity(differenceVec);

	velocity.multiplyMat4(worldTransform_.matWorld_);

	if (color_ == 0) {
		//�e�̐����Ə�����
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->bossInitialize(model2_, worldTransform_.translation_, velocity);
		std::unique_ptr<EnemyBullet> newBullet2 = std::make_unique<EnemyBullet>();
		newBullet2->bossInitialize2(model2_, worldTransform_.translation_, velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
		bullets_.push_back(std::move(newBullet2));
	}
	if (color_ == 1) {
		//�e�̐����Ə�����
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->bossInitialize(model3_, worldTransform_.translation_, velocity);
		std::unique_ptr<EnemyBullet> newBullet2 = std::make_unique<EnemyBullet>();
		newBullet2->bossInitialize2(model3_, worldTransform_.translation_, velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
		bullets_.push_back(std::move(newBullet2));
	}
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
	hp--;

	if (hp <= 0) {
		isDead_ = true;
	}
}
