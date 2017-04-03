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

int vertexCount, texCount, faceCount;
GLuint *faces;

int createViewVolume() {
  glEnable(GL_DEPTH_TEST);

  // Specify shape and size of the view volume.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); //load identity matrix into GL_PROJECTION
  gluPerspective(45.0, 1.0, 0.1, 20.0);

  // Specify the position for the view volume
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.5, -3.0,  // eye xyz,
            0.0, 0.5, 0.0,  // view xyz,
            0.0, 1.0, 0.0); // up xyz
  return 0;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glDrawElements(GL_QUADS, faceCount*12, GL_UNSIGNED_INT, faces);
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
	GLfloat *vertices, *normals, *tangents, *bitangents, *textureCoords;

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
		} else if (line[0] == 'f') {
			faceCount++;
		}
	}

	rewind(file);

	// Allocate memory for storing all data from file
	vertices = (GLfloat *)malloc(sizeof(GLfloat) * vertexCount * 3);
	normals = (GLfloat *)malloc(sizeof(GLfloat) * vertexCount * 3);
	tangents = (GLfloat *)malloc(sizeof(GLfloat) * vertexCount * 3);
	bitangents = (GLfloat *)malloc(sizeof(GLfloat) * vertexCount * 3);
	faces = (GLuint *)malloc(sizeof(GLuint) * faceCount * 12); // |faces| == |vertices|
	textureCoords = (GLfloat *)malloc(sizeof(GLfloat) * texCount * 2);

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
					textureCoords[currentTexCoord++] = atof(strtok(NULL, " "));
					textureCoords[currentTexCoord++] = atof(strtok(NULL, " "));
					break;
				case 'x':
					data = strtok(line, " ");
					tangents[currentTangent++] = atof(strtok(NULL, " "));
					tangents[currentTangent++] = atof(strtok(NULL, " "));
					tangents[currentTangent++] = atof(strtok(NULL, " "));
					break;
				case 'y':
					data = strtok(line, " ");
					bitangents[currentBiTangent++] = atof(strtok(NULL, " "));
					bitangents[currentBiTangent++] = atof(strtok(NULL, " "));
					bitangents[currentBiTangent++] = atof(strtok(NULL, " "));
					break;
				case 'n':
					data = strtok(line, " ");
					normals[currentNormal++] = atof(strtok(NULL, " "));
					normals[currentNormal++] = atof(strtok(NULL, " "));
					normals[currentNormal++] = atof(strtok(NULL, " "));
					break;
				case ' ':
					data = strtok(line, " ");
					vertices[currentVertex++] = atof(strtok(NULL, " "));
					vertices[currentVertex++] = atof(strtok(NULL, " "));
					vertices[currentVertex++] = atof(strtok(NULL, " "));
					break;
				default:
					break;
			}
		} 
		else if (line[0] == 'f') 
		{
			strtok(line, " ");
			int i = 0;			
			while (i < 12)
			{
				data = strtok(NULL, " /");
				faces[currentFace++] = atof(data);
				i++;
			}
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
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	glEnableClientState(GL_VERTEX_ARRAY);
}

int main(int argc, char **argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(768, 768);
	glutInitWindowPosition(1000, 200);
	glutCreateWindow("Hopefully a teapot");
	glClearColor(1.0, 0.0, 0.7, 1.0);
	
	printf("Creating view volume\n");
	createViewVolume();
	printf("Loading teapot\n");
	loadTeapot("teapot.605.obj");
	
	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutKeyboardFunc(input);

	glutMainLoop();
}
