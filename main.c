#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <GL/glext.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>

typedef struct vector3f {
	float x, y, z;
} Vector3f;

typedef struct vector2f {
	float x, y;
} Vector2f;

typedef struct face {
	int indices[12];
} Face;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glFlush();
}

void update()
{

}

void input(unsigned char key, int x, int y)
{
	switch(key) 
	{
		case 'q':
			exit(0);
		default:
			break;
	}
}

void loadTeapot(const char *objFile)
{
	int vertexCount = 0, texCount = 0;

	Vector3f *vertices, *normals, *tangents, *bitangents;
	Vector2f *textureCoords;

	Face *faces;

	char *data;

	FILE *file = fopen(objFile, "r");
	char line[256];

	// Obtain vertexCount and faceCount
	while (fgets(line, sizeof(line), file))
	{
		if (line[0]+line[1] == 'v'+' ')
		{
			vertexCount++;
		}
		else if (line[0]+line[1] == 'v'+'t') 
		{
			texCount++;
		}
	}
	rewind(file);

	// Allocate memory for storing all data from file
	vertices = malloc(sizeof(Vector3f) * vertexCount);
	normals = malloc(sizeof(Vector3f) * vertexCount);
	tangents = malloc(sizeof(Vector3f) * vertexCount);
	bitangents = malloc(sizeof(Vector3f) * vertexCount);
	faces = malloc(sizeof(Face) * vertexCount); // |faces| == |vertices|
	textureCoords = malloc(sizeof(Vector2f) * texCount);

	// Reset to use as index
	int currentVertex = 0, currentTexCoord = 0, currentNormal = 0;
	int currentTangent = 0, currentBiTangent = 0, currentFace = 0;
	
	// Parse File
	while (fgets(line, sizeof(line), file))
	{
		if (line[0] == '#') continue;

		if (line[0] == 'v')
		{
			switch (line[1])
			{
				case 't':
					data = strtok(line, " ");
					textureCoords[currentTexCoord].x = atof(strtok(NULL, " "));
					textureCoords[currentTexCoord++].y = atof(strtok(NULL, " "));
					break;
				case 'x':
					data = strtok(line, " ");
					tangents[currentTangent].x = atof(strtok(NULL, " "));
					tangents[currentTangent].y = atof(strtok(NULL, " "));
					tangents[currentTangent++].z = atof(strtok(NULL, " "));
					break;
				case 'y':
					data = strtok(line, " ");
					bitangents[currentBiTangent].x = atof(strtok(NULL, " "));
					bitangents[currentBiTangent].y = atof(strtok(NULL, " "));
					bitangents[currentBiTangent++].z = atof(strtok(NULL, " "));
					break;
				case 'n':
					data = strtok(line, " ");
					normals[currentNormal].x = atof(strtok(NULL, " "));
					normals[currentNormal].y = atof(strtok(NULL, " "));
					normals[currentNormal++].z = atof(strtok(NULL, " "));
					break;
				case ' ':
					data = strtok(line, " ");
					vertices[currentVertex].x = atof(strtok(NULL, " "));
					vertices[currentVertex].y = atof(strtok(NULL, " "));
					vertices[currentVertex++].z = atof(strtok(NULL, " "));
					break;
				default:
					break;
			}
		} 
		else if (line[0] == 'f') 
		{
			strtok(line, " ");
			int i = 0;			
			while (data = strtok(NULL, " /"))
			{
				faces[currentFace].indices[i++] = atof(data);
			}
			currentFace++;
		}
		else
		{
			char *mtlFile = strtok(line, " ");
			if (strstr(mtlFile, "mtllib"))
			{
				mtlFile = strtok(NULL, " \n");
				
				// Process mtlFile here
			}
		}
	}
	fclose(file);

	
}

int main(int argc, char **argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(768, 768);
	glutInitWindowPosition(1000, 200);
	glutCreateWindow("Hopefully a teapot");
	glClearColor(1.0, 0.0, 0.7, 1.0);

	loadTeapot("teapot.605.obj");
	
	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutKeyboardFunc(input);

	glutMainLoop();
}
