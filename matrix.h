#ifndef MATRIX_H
#define MATRIX_H

typedef struct matrix {
	float m00, m01, m02, m10, m11, m12, m20, m21, m22;
} Matrix3f;

float dot(Vector3f left, Vector3f right)
{
	return left.x*right.x + left.y*right.y + left.z*right.z;
}

Vector3f cross(Vector3f left, Vector3f right) 
{
	Vector3f result = 
	{
		left.y*right.z - left.z*right.y,
		left.z*right.x - left.x*right.z,
		left.x*right.y - left.y*right.x
	};
	return result;
}

float getDeterminant(Matrix3f mat)
{
	Vector3f V0 = {mat.m00, mat.m01, mat.m02};
	Vector3f V1 = {mat.m10, mat.m11, mat.m12};
	Vector3f V2 = {mat.m20, mat.m21, mat.m22};

	return dot(V0, cross(V1, V2));
}

Matrix3f createMatrix(Vector3f a, Vector3f b, Vector3f c)
{
	Matrix3f mat;
	mat.m00 = a.x;
	mat.m01 = a.y;
	mat.m02 = a.z;
	mat.m10 = b.x;
	mat.m11 = b.y;
	mat.m12 = b.z;
	mat.m20 = c.x;
	mat.m21 = c.y;
	mat.m22 = c.z;
	return mat;
}

Vector3f multiplyVector(Matrix3f mat, Vector3f vec)
{
	Vector3f result = 
	{
		mat.m00*vec.x + mat.m01*vec.y + mat.m02*vec.z,
		mat.m10*vec.x + mat.m11*vec.y + mat.m12*vec.z,
		mat.m20*vec.x + mat.m21*vec.y + mat.m22*vec.z
	};
	return result;
}

Matrix3f invertAndTranspose(Matrix3f mat)
{
	Vector3f V0 = {mat.m00, mat.m01, mat.m02};
	Vector3f V1 = {mat.m10, mat.m11, mat.m12};
	Vector3f V2 = {mat.m20, mat.m21, mat.m22};

	float det = getDeterminant(mat);
	Vector3f top = mul(cross(V1, V2), 1.0/det);
	Vector3f mid = mul(mul(cross(V0, V2), -1), 1.0/det);
	Vector3f bot = mul(cross(V0, V1), 1.0/det);

	mat = createMatrix(top, mid, bot);
	return mat;
}

#endif
