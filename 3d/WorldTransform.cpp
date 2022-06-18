#include "WorldTransform.h"
#include <math.h>

void WorldTransform::MatrixConvert()
{
	Matrix4 matScale;
	Matrix4 matRotX, matRotY, matRotZ;
	Matrix4 matRot;
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	Matrix4 matTrans2 = MathUtility::Matrix4Identity();
	matScale.ScaleSet(scale_);
	matRotZ.RotZSet(rotation_.z);
	matRotX.RotXSet(rotation_.x);
	matRotY.RotYSet(rotation_.y);
	matTrans.TransSet(translation_);
	matRot = matRotZ * matRotX * matRotY;
	//合成
	matWorld_ = matScale * matRot * matTrans;
}