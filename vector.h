#ifndef VECTOR_H
#define VECTOR_H

typedef struct vector3f {
	GLfloat x, y, z;
} Vector3f;

typedef struct vector2f {
	GLfloat x, y;
} Vector2f;

Vector3f mul(Vector3f vector, float value)
{
	vector.x *= value;
	vector.y *= value;
	vector.z *= value;
	return vector;
}

Vector3f sub(Vector3f left, Vector3f right)
{
	Vector3f result = 
	{
		left.x - right.x,
		left.y - right.y,
		left.z - right.z
	};
	return result;
}

Vector3f add(Vector3f left, Vector3f right)
{
	Vector3f result = 
	{
		left.x + right.x,
		left.y + right.y,
		left.z + right.z
	};
	return result;
}

Vector3f normalize(Vector3f vec)
{
	float mag = sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
	vec.x /= mag;
	vec.y /= mag;
	vec.z /= mag;
	return vec;
}

#endif
