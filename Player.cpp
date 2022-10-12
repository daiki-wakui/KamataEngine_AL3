#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, Model* model2, Model* model3, Model* model4){
	uint32_t textureReticle = TextureManager::Load("ret.png");

	sprite2DReticle_.reset(Sprite::Create(textureReticle, Vector2(1280/2, 720/2), Vector4(1, 1, 1, 1), { 0.5f,0.5f }));


	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����̃f�[�^�������o�ϐ��ɑ��
	model_ = model;
	model2_ = model2;
	model3_ = model3;
	model4_ = model4;

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.scale_ = { 3,3,3 };
	worldTransform_.rotation_ = { 0,0,0 };
	worldTransform_.translation_ = { 0,-18,0 };

	//���[���h���W�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform3DReticle_.Initialize();
	playerColor = 0;
	bullets_.clear();
}

void Player::Update(int start){

	//�t�@�C�������w�肵�ăe�N�X�`����ǂݍ���
	if (playerColor == 0) {
		textureHandle_ = TextureManager::Load("black.png");
	}
	else if (playerColor == 1) {
		textureHandle_ = TextureManager::Load("white.png");
	}

	//�f�X�t���O���������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
	});

	//�ړ����E���W
	const float kMoveLimitX = 85.0f;
	const float kMoveLimitY = 40.0f;
	const float kMoveLimitZ = 70.0f;

	if (start == 1) {
		//�ړ��֐�
		Move();
		//��]�֐�
		Rotate();

		//�e���o��
		Attack();

		if (input_->TriggerKey(DIK_C)) {

			if (isChange == false) {
				worldTransform_.rotation_.z = 0.0f;
				dashSpeed = 4.0f;
				isChange = true;
			}

			if (playerColor == 0) {
				playerColor = 1;
			}
			else if (playerColor == 1) {
				playerColor = 0;
			}
		}

		if (isChange == true) {

			if (state == 1) {
				worldTransform_.rotation_.z -= 0.75f;
				worldTransform_.translation_.x += dashSpeed;
				dashSpeed -= 0.5f;
			}
			if (state == 2) {
				worldTransform_.rotation_.z += 0.75f;
				worldTransform_.translation_.x -= dashSpeed;
				dashSpeed -= 0.5f;
			}


			if (worldTransform_.rotation_.z >= 6.5 || worldTransform_.rotation_.z <= -6.5) {
				isChange = false;
				worldTransform_.rotation_.z = 0;
				
			}
		}

		//�e�̍X�V����
		for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
			bullet->Update();
		}
	}
	

	//�ړ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
	worldTransform_.translation_.z = max(worldTransform_.translation_.z, -kMoveLimitZ);
	worldTransform_.translation_.z = min(worldTransform_.translation_.z, +kMoveLimitZ);

	//�s��ϊ�
	MatrixConvert();

	//�s��𑗐M
	worldTransform_.TransferMatrix();

	const float kDistancePlayerTo3DReticle = 50.0f;

	Vector3 offset = { 0,0,1.0f };
	offset.multiplyMat4(worldTransform_.matWorld_);
	offset.normalize();
	offset *= kDistancePlayerTo3DReticle;

	worldTransform3DReticle_.translation_ = worldTransform_.translation_;
	worldTransform3DReticle_.translation_.z += kDistancePlayerTo3DReticle;
	worldTransform3DReticle_.translation_.y += 7.0f;

	//�s��ϊ�
	worldTransform3DReticle_.MatrixConvert();
	worldTransform3DReticle_.TransferMatrix();

}

//�`��֐�
void Player::Draw(ViewProjection &viewProjection){
	//3D���f���`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//model2_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_){
		bullet->Draw(viewProjection);
	}
}

//�s��ϊ��֐�
void Player::MatrixConvert(){
	Matrix4 matScale;
	Matrix4 matRotX, matRotY, matRotZ;
	Matrix4 matRot;
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	Matrix4 matTrans2 = MathUtility::Matrix4Identity();
	matScale.ScaleSet(worldTransform_.scale_);
	matRotZ.RotZSet(worldTransform_.rotation_.z);
	matRotX.RotXSet(worldTransform_.rotation_.x);
	matRotY.RotYSet(worldTransform_.rotation_.y);
	matTrans.TransSet(worldTransform_.translation_);
	matRot = matRotZ * matRotX * matRotY;
	//����
	worldTransform_.matWorld_ = matScale * matRot * matTrans;
}

//�ړ��֐�
void Player::Move(){
	//�L�����N�^�[�̈ړ��x�N�g��
	move = { 0,0,0 };

	if (input_->PushKey(DIK_RIGHT)) {
		move += { 0.5f, 0, 0 };
		if (isChange == false) {
			state = 1;
			worldTransform_.rotation_.z -= 0.025f;

			if (worldTransform_.rotation_.z <= -0.25f) {
				worldTransform_.rotation_.z = -0.25f;
			}
		}
	}
	else if (isChange == false && worldTransform_.rotation_.z <= 0.0f) {
		worldTransform_.rotation_.z += 0.025f;
		if (worldTransform_.rotation_.z >= 0.0f) {
			worldTransform_.rotation_.z = 0.0f;
		}
	}

	if (input_->PushKey(DIK_LEFT)) {
		move += { -0.5f,0,0 };
		if (isChange == false) {
			state = 2;

			worldTransform_.rotation_.z += 0.025f;

			if (worldTransform_.rotation_.z >= 0.25f) {
				worldTransform_.rotation_.z = 0.25f;
			}
		}
	}
	else if (isChange == false && worldTransform_.rotation_.z >= 0.0f) {
		worldTransform_.rotation_.z -= 0.025f;
		if (worldTransform_.rotation_.z <= 0.0f) {
			worldTransform_.rotation_.z = 0.0f;
		}
	}

	if (input_->PushKey(DIK_UP)) {
		move += { 0, 0.5f, 0 };
	}
	if (input_->PushKey(DIK_DOWN)) {
		move += { 0, -0.5f, 0 };
	}
	if (input_->PushKey(DIK_X)) {
		move += { 0, 0, 0.5f};

		worldTransform_.rotation_.x += 0.02f;

		if (worldTransform_.rotation_.x >= 0.2f) {
			worldTransform_.rotation_.x = 0.2f;
		}
	}
	else if (isChange == false && worldTransform_.rotation_.x >= 0.0f) {
		worldTransform_.rotation_.x -= 0.02f;
		if (worldTransform_.rotation_.x <= 0.0f) {
			worldTransform_.rotation_.x = 0.0f;
		}
	}

	if (input_->PushKey(DIK_Z)) {
		move += { 0, 0, -0.5f};

		worldTransform_.rotation_.x -= 0.02f;

		if (worldTransform_.rotation_.x <= -0.2f) {
			worldTransform_.rotation_.x = -0.2f;
		}
	}
	else if (isChange == false && worldTransform_.rotation_.x <= 0.0f) {
		worldTransform_.rotation_.x += 0.02f;
		if (worldTransform_.rotation_.x >= 0.0f) {
			worldTransform_.rotation_.x = 0.0f;
		}
	}

	worldTransform_.translation_ += move;
}

//��]�֐�
void Player::Rotate(){
}

//�e�̍U��
void Player::Attack()
{
	coolTime--;

	if (input_->PushKey(DIK_SPACE)) {
		//�e�̑��x
		const float kBulletSpeed = 2.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = worldTransform3DReticle_.translation_ - worldTransform_.translation_;
		velocity.y -= 10.0f;
		velocity.multiplyMat4(worldTransform3DReticle_.matWorld_);
		velocity.normalize();
		velocity *= kBulletSpeed;

		if (coolTime < 0) {
			if (playerColor == 0) {
				//�e�̐����Ə�����
				std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
				newBullet->Initialize(model3_, worldTransform_.translation_, velocity, playerColor);
				std::unique_ptr<PlayerBullet> newBullet2 = std::make_unique<PlayerBullet>();
				newBullet2->Initialize2(model3_, worldTransform_.translation_, velocity, playerColor);
				//�e��o�^����
				bullets_.push_back(std::move(newBullet));
				bullets_.push_back(std::move(newBullet2));
			}
			else if (playerColor == 1) {
				//�e�̐����Ə�����
				std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
				newBullet->Initialize(model4_, worldTransform_.translation_, velocity, playerColor);
				std::unique_ptr<PlayerBullet> newBullet2 = std::make_unique<PlayerBullet>();
				newBullet2->Initialize2(model4_, worldTransform_.translation_, velocity, playerColor);
				//�e��o�^����
				bullets_.push_back(std::move(newBullet));
				bullets_.push_back(std::move(newBullet2));
			}
			

			coolTime = 7;
		}
	}
}

//���[���h���W���擾
Vector3 Player::GetWorldPosition()
{
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Player::OnCollision()
{
}

void Player::DrawUI()
{
	sprite2DReticle_->Draw();
}
