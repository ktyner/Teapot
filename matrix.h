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

	printf("Determinant %f\n", dot(V0, cross(V1, V2)));

	return dot(V0, cross(V1, V2));
}

Matrix3f invertAndTranspose(Matrix3f mat)
{
	Vector3f V0 = {mat.m00, mat.m01, mat.m02};
	Vector3f V1 = {mat.m10, mat.m11, mat.m12};
	Vector3f V2 = {mat.m20, mat.m21, mat.m22};

	float det = getDeterminant(mat);
	Vector3f top = cross(V1, V2);
	Vector3f mid = mul(cross(V0, V2), -1);
	Vector3f bot = cross(V0, V1);

	mat.m00 = top.x/det;
	mat.m01 = top.y/det;
	mat.m02 = top.z/det;
	mat.m10 = mid.x/det;
	mat.m11 = mid.y/det;
	mat.m12 = mid.z/det;
	mat.m20 = bot.x/det;
	mat.m21 = bot.y/det;
	mat.m22 = bot.z/det;
	return mat;
}

Vector3f normalize(Vector3f vec)
{
	float mag = sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
	vec.x /= mag;
	vec.y /= mag;
	vec.z /= mag;
	return vec;
}

void printMatrix(Matrix3f mat)
{
	printf("%f %f %f\n", mat.m00, mat.m01, mat.m02);
	printf("%f %f %f\n", mat.m10, mat.m11, mat.m12);
	printf("%f %f %f\n", mat.m20, mat.m21, mat.m22);
}

#endif
