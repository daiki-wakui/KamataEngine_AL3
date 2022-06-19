#include "Matrix4.h"
#include "Vector3.h"

void Vector3::multiplyMat4(Matrix4& mat)
{
	x = x * mat.m[0][0] + y * mat.m[1][0] + z * mat.m[2][0];
	y = x * mat.m[0][1] + y * mat.m[1][1] + z * mat.m[2][1];
	z = x * mat.m[0][2] + y * mat.m[1][2] + z * mat.m[2][2];
}
