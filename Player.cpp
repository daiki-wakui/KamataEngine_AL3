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

	//���[���h���W�ϊ��̏�����
	worldTransform_.Initialize();
}

void Player::Update(){
	//�ړ����E���W
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 18.5f;

	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };
	Vector3 rote = { 0,0,0 };
	Vector3 scale = { 0,0,0 };

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
	else if (input_->PushKey(DIK_Z)) {
		move = { 0,0,0.5f };
	}
	else if (input_->PushKey(DIK_X)) {
		move = { 0,0,-0.5f };
	}

	if (input_->PushKey(DIK_W)) {
		rote = { 0.3f,0,0 };
	}
	else if (input_->PushKey(DIK_S)) {
		rote = { -0.3f,0,0 };
	}
	else if (input_->PushKey(DIK_D)) {
		rote = { 0,0.3f,0 };
	}
	else if (input_->PushKey(DIK_A)) {
		rote = { 0,-0.3f,0 };
	}
	else if (input_->PushKey(DIK_E)) {
		rote = { 0,0,0.3f };
	}
	else if (input_->PushKey(DIK_Q)) {
		rote = { 0,0,-0.3f };
	}

	if (input_->PushKey(DIK_U)) {
		scale = { 0.05f,0,0 };
	}
	else if (input_->PushKey(DIK_J)) {
		scale = { -0.05f,0,0 };
	}
	else if (input_->PushKey(DIK_I)) {
		scale = { 0,0.05f,0 };
	}
	else if (input_->PushKey(DIK_K)) {
		scale = { 0,-0.05f,0 };
	}
	else if (input_->PushKey(DIK_O)) {
		scale = { 0,0,0.05f };
	}
	else if (input_->PushKey(DIK_L)) {
		scale = { 0,0,-0.05f };
	}


	worldTransform_.translation_ += move;
	worldTransform_.rotation_ += rote;
	worldTransform_.scale_ += scale;


	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//�s��ϊ�
	MatrixConvert(worldTransform_);

	//�s��𑗐M
	worldTransform_.TransferMatrix();

	//�f�o�b�O
	debugText_->SetPos(20, 20);
	debugText_->Printf(
		"PlayerPos %f,%f,%f",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}

void Player::Draw(ViewProjection &viewProjection){
	//3D���f���`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void MatrixConvert(WorldTransform& worldTransform_){
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
