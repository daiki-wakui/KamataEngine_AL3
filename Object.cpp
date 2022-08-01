#include "Object.h"
#include <cassert>

void Object::Initialize(Model* model, uint32_t textureHandle){
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����̃f�[�^�������o�ϐ��ɑ��
	model_ = model;
	textureHandle_ = textureHandle;

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.scale_ = { 2,2,2 };
	worldTransform_.translation_ = { 0,0,0 };

	//���[���h���W�ϊ��̏�����
	worldTransform_.Initialize();
}

void Object::Update()
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	if (input_->PushKey(DIK_D)) {
		move = { 0.5f,0,0 };
	}
	else if (input_->PushKey(DIK_A)) {
		move = { -0.5f,0,0 };
	}

	worldTransform_.translation_ += move;


	//�s��ϊ�
	MatrixConvert(worldTransform_);

	//�s��𑗐M
	worldTransform_.TransferMatrix();
}

void Object::Draw(ViewProjection& viewProjection)
{
	//3D���f���`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

