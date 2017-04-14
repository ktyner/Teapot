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

void printVector(Vector3f vec)
{
	printf("(%f, %f, %f)\n", vec.x, vec.y, vec.z);
}

#endif
