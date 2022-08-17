#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle){
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����̃f�[�^�������o�ϐ��ɑ��
	model_ = model;
	textureHandle_ = textureHandle;

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//worldTransform_.scale_ = { 1,1,1 };

	//���[���h���W�ϊ��̏�����
	worldTransform_.Initialize();
}

void Player::Update(){

	//�f�X�t���O���������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
	});

	//�ړ����E���W
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 18.5f;

	//�ړ��֐�
	Move();
	//��]�֐�
	Rotate();

	//�e���o��
	Attack();
	//�e�̍X�V����
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//�ړ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//�s��ϊ�
	MatrixConvert();

	//�s��𑗐M
	worldTransform_.TransferMatrix();

	//�f�o�b�O
	debugText_->SetPos(20, 20);
	debugText_->Printf(
		"PlayerPos %f,%f,%f",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
	debugText_->SetPos(20, 40);
	debugText_->Printf("U/I Key PlayerRote");

	debugText_->SetPos(20, 60);
	debugText_->Printf("UP/DOWN/RIGHT/LEFT Key PlayerMove");

	debugText_->SetPos(20, 80);
	debugText_->Printf("SPACE Key PlayerBullet");
}

//�`��֐�
void Player::Draw(ViewProjection &viewProjection){
	//3D���f���`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

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
		move = { 0.5f,0,0 };
	}
	else if (input_->PushKey(DIK_LEFT)) {
		move = { -0.5f,0,0 };
	}
	else if (input_->PushKey(DIK_UP)) {
		move = { 0,0.5f,0 };
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move = { 0,-0.5f,0 };
	}

	worldTransform_.translation_ += move;
}

//��]�֐�
void Player::Rotate(){
	if (input_->PushKey(DIK_U)) {
		worldTransform_.rotation_.y -= 0.05f;
	}
	else if (input_->PushKey(DIK_I)) {
		worldTransform_.rotation_.y += 0.05f;
	}
}

//�e�̍U��
void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity.multiplyMat4(worldTransform_.matWorld_);

		//�e�̐����Ə�����
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
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
