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

SceneData sceneData;
int vaoID[2];
int vertexVBO[2];
int normalVBO[2];
int indexVBO[2];
int texCoordVBO;
int tangentVBO;
int bitangentVBO;
int colorVBO;
unsigned int boxShaderProgramID;

#endif
