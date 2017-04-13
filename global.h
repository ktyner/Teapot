#ifndef GLOBAL_H
#define GLOBAL_H
typedef struct vector3f {
	GLfloat x, y, z;
} Vector3f;

typedef struct vector2f {
	GLfloat x, y;
} Vector2f;

typedef struct teapot {
	int curV, curN, curT, curB, curTex, curIndex;
	Vector3f *vertices, *normals, *tangents, *bitangents;
	Vector2f *texCoords;
	GLuint *indices;
} TeapotData;

typedef struct box {
	Vector3f *vertices, *colors, *normals;
	GLubyte *indices;
} BoxData;

typedef struct data {
	BoxData box;
	TeapotData teapot;
} SceneData;

Vector3f normalize(Vector3f vec)
{
	float mag = sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
	vec.x /= mag;
	vec.y /= mag;
	vec.z /= mag;
	return vec;
}

SceneData sceneData;
unsigned int boxShaderProgramID;
float teapotRotation = 0.0;

#endif
