#ifndef GLOBAL_H
#define GLOBAL_H
#include "vector.h"
#include "matrix.h"

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

typedef struct toplight {
	Vector3f *vertices, *colors, *normals;
	GLubyte *indices;
} TopLightData;

typedef struct data {
	BoxData box;
	TeapotData teapot;
	TopLightData toplight;
} SceneData;

SceneData sceneData;
unsigned int boxShaderProgramID;
float teapotRotation = 0.0;

#endif
