#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model){
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����̃f�[�^�������o�ϐ��ɑ��
	model_ = model;

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.translation_ = { 0,0,0 };

	worldTransform_.scale_ = { 70,70,70 };
	//���[���h���W�ϊ��̏�����
	worldTransform_.Initialize();
}

void Skydome::Update(){
	//�s��ϊ�
	worldTransform_.MatrixConvert();
	//�s��𑗐M
	worldTransform_.TransferMatrix();
}

void Skydome::Draw(ViewProjection& viewProjection){
	model_->Draw(worldTransform_, viewProjection);
}
