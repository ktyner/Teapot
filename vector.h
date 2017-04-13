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

void printVector(Vector3f vec)
{
	printf("(%f, %f, %f)\n", vec.x, vec.y, vec.z);
}

#endif
