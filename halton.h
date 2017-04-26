#ifndef HALTON_H
#define HALTON_H
#define RAYNOTHIT 1000000
#define PI 3.14159265359

float phi(int b, int i)
{
	float x, f;
	x = 0.0;
	f = 1.0/(float)b;
	while (i) {
		x += f * (float)(i%b);
		i /= b;
		f *= 1.0 / (float)b;
	}
	return x;
}

Vector3f halton(int i)
{
	float az = 2.0*PI*phi(2, i);
	float el = asin(phi(3, i));
	Vector3f result = { -sin(az)*cos(el), sin(el), cos(az)*cos(el) };
	return result;
}

Vector3f getRayTriangleIntersectionPoint(Vector3f p, Vector3f d, Vector3f v0, Vector3f v1, Vector3f v2)
{
	Vector3f result;
	Vector3f top = sub(p, d);
	Vector3f mid = sub(v1, v0);
	Vector3f bot = sub(v2, v0);
	Vector3f right = sub(p, v0);

	Matrix3f mat = createMatrix(top, mid, bot);

	mat = invertAndTranspose(mat);
	result = multiplyVector(mat, right);

	if (result.x < 0.0) {
		Vector3f null = {RAYNOTHIT, RAYNOTHIT, RAYNOTHIT};
		return null;
	}

	result = add(add(mul(v0, (1.0 - result.y - result.z)), mul(v1, result.y)), mul(v2, result.z));
	
	return result;
}

#endif