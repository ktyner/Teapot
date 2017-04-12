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

#include "global.h"

void init(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(768, 768);
	glutInitWindowPosition(1000, 200);
	glutCreateWindow("Hopefully a teapot");
	glClearColor(1.0, 0.0, 0.7, 1.0);
	glEnable(GL_DEPTH_TEST);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
}

void loadTeapot(const char *objFile)
{
	FILE *file = fopen(objFile, "r");
	char line[256];
	char *lineData;

	int vertexCount = 0, texCount = 0;

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

	Vector3f *normals;
	Vector2f *texCoords;
	int nindex = 0, texindex = 0;

	normals = malloc(sizeof(Vector3f) * vertexCount);
	texCoords = malloc(sizeof(Vector2f) * texCount);
	sceneData.teapot.vertices = malloc(sizeof(Vector3f) * vertexCount);
	sceneData.teapot.normals = malloc(sizeof(Vector3f) * vertexCount);
	sceneData.teapot.tangents = malloc(sizeof(Vector3f) * vertexCount);
	sceneData.teapot.bitangents = malloc(sizeof(Vector3f) * vertexCount);
	sceneData.teapot.texCoords = malloc(sizeof(Vector2f) * texCount);
	sceneData.teapot.indices = malloc(sizeof(GLuint) * vertexCount * 4);

	rewind(file);

	while (fgets(line, sizeof(line), file))
	{
		if (line[0] == '#') continue;
		if (line[0] == 'v') {
			switch(line[1]) {
				case ' ':
					lineData = strtok(line, " ");
					Vector3f vertex = {atof(strtok(NULL, " ")),
									   atof(strtok(NULL, " ")), 
									   atof(strtok(NULL, " "))};
					sceneData.teapot.vertices[sceneData.teapot.curV++] = vertex;
					break;
				case 'n':
					lineData = strtok(line, " ");
					Vector3f normal = {atof(strtok(NULL, " ")),
									   atof(strtok(NULL, " ")), 
									   atof(strtok(NULL, " "))};
					normals[nindex++] = normal;
					break;
				case 't':
					lineData = strtok(line, " ");
					Vector2f texCoord = {atof(strtok(NULL, " ")),
										 atof(strtok(NULL, " "))};
					texCoords[texindex++] = texCoord;
					break;
				case 'x':
					lineData = strtok(line, " ");
					Vector3f tangent = {atof(strtok(NULL, " ")),
									    atof(strtok(NULL, " ")), 
									    atof(strtok(NULL, " "))};
					sceneData.teapot.tangents[sceneData.teapot.curT++] = tangent;
					break;
				case 'y':
					lineData = strtok(line, " ");
					Vector3f bitangent = {atof(strtok(NULL, " ")),
									      atof(strtok(NULL, " ")), 
									      atof(strtok(NULL, " "))};
					sceneData.teapot.bitangents[sceneData.teapot.curB++] = bitangent;
					break;
				default:
					break;
			}	
		} else if (line[0] == 'f') {
			lineData = strtok(line, " ");
			int i;
			for (i = 0; i < 4; i++) {
				lineData = strtok(NULL, " /");
				int currentVertexPointer = atoi(lineData)-1;
				sceneData.teapot.indices[sceneData.teapot.curIndex++] = currentVertexPointer;
				lineData = strtok(NULL, " /");
				Vector2f currentTex = texCoords[atoi(lineData)-1];
				sceneData.teapot.texCoords[currentVertexPointer] = currentTex;
				lineData = strtok(NULL, " /");
				Vector3f currentNormal = normals[atoi(lineData)-1];
				sceneData.teapot.normals[currentVertexPointer] = currentNormal;
			}
		} else {

		}

		/* We don't need to free normals or texCoords because the O.S. handles
		   this automatically when using malloc. It causes a double free segfault
		   if we try to free here. */
	}
}

/*
	dx - width
	dy - height
	dz - depth
*/
void loadBox(float dx, float dy, float dz)
{
	dx /= 2.0;
	dy /= 2.0;
	dz /= 2.0;
	Vector3f vertices[] = 
	{
		// Back Face
		{	-dx,  dy,  dz	},
		{ 	 dx,  dy,  dz	},
		{	 dx, -dy,  dz	},
		{	-dx, -dy,  dz	},
		// Right Face
		{	-dx,  dy, -dz	},
		{	-dx,  dy,  dz	},
		{	-dx, -dy,  dz	},
		{	-dx, -dy, -dz	},
		// Left Face
		{	 dx,  dy, -dz	},
		{	 dx,  dy,  dz	},
		{	 dx, -dy,  dz	},
		{	 dx, -dy, -dz	},
		// Top Face
		{	-dx,  dy,  dz	},
		{	 dx,  dy,  dz	},
		{	 dx,  dy, -dz	},
		{	-dx,  dy, -dz	},
		// Bottom Face
		{	-dx, -dy,  dz	},
		{	 dx, -dy,  dz	},
		{	 dx, -dy, -dz	},
		{	-dx, -dy, -dz	}
	};

	Vector3f normals[] =
	{
		// Back Face
		{	 0.0,  0.0, -1.0	},
		{	 0.0,  0.0, -1.0	},
		{	 0.0,  0.0, -1.0	},
		{	 0.0,  0.0, -1.0	},
		// Right Face
		{	 1.0,  0.0,  0.0	},
		{	 1.0,  0.0,  0.0	},
		{	 1.0,  0.0,  0.0	},
		{	 1.0,  0.0,  0.0	},
		// Left Face
		{	-1.0,  0.0,  0.0	},
		{	-1.0,  0.0,  0.0	},
		{	-1.0,  0.0,  0.0	},
		{	-1.0,  0.0,  0.0	},
		// Top Face
		{	 0.0, -1.0,  0.0	},
		{	 0.0, -1.0,  0.0	},
		{	 0.0, -1.0,  0.0	},
		{	 0.0, -1.0,  0.0	},
		// Bottom Face
		{	 0.0,  1.0,  0.0	},
		{	 0.0,  1.0,  0.0	},
		{	 0.0,  1.0,  0.0	},
		{	 0.0,  1.0,  0.0	}
	};

	Vector3f colors[] = 
	{
		// Back Face
		{	 1.0,  1.0,  1.0	},
	 	{	 1.0,  1.0,  1.0	},
		{	 1.0,  1.0,  1.0	},
		{	 1.0,  1.0,  1.0	},
		// Right Face
		{	 0.0,  1.0,  0.0	},
	 	{	 0.0,  1.0,  0.0	},
		{	 0.0,  1.0,  0.0	},
		{	 0.0,  1.0,  0.0	},
		// Left Face
		{	 1.0,  0.0,  0.0	},
	 	{	 1.0,  0.0,  0.0	},
		{	 1.0,  0.0,  0.0	},
		{	 1.0,  0.0,  0.0	},
		// Top Face
		{	 1.0,  1.0,  1.0	},
	 	{	 1.0,  1.0,  1.0	},
		{	 1.0,  1.0,  1.0	},
		{	 1.0,  1.0,  1.0	},
		// Bottom Face
		{	 1.0,  1.0,  1.0	},
	 	{	 1.0,  1.0,  1.0	},
		{	 1.0,  1.0,  1.0	},
		{	 1.0,  1.0,  1.0	}
	};
	
	sceneData.box.vertices = malloc(sizeof(Vector3f) * 20);
	sceneData.box.colors = malloc(sizeof(Vector3f) * 20);
	sceneData.box.normals = malloc(sizeof(Vector3f) * 20);
	sceneData.box.indices = malloc(sizeof(GLubyte) * 20);

	int i;
	for (i = 0; i < 20; i++)
	{
		sceneData.box.vertices[i] = vertices[i];
		sceneData.box.colors[i] = colors[i];
		sceneData.box.normals[i] = normals[i];
		sceneData.box.indices[i] = i;
	}
}

void display() 
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vaoID[0]);
	glDrawElements(GL_QUADS, 20, GL_UNSIGNED_BYTE, sceneData.box.indices);
	glBindVertexArray(0);

	glFlush();
}
void update() {}
void input(unsigned char key, int x, int y) 
{
	if (key) exit(0);
}

int createLights() {
  // Fill light
  float light0_ambient[] = { 0.3, 0.3, 0.3, 0.0 };
  float light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  float light0_specular[] = { 1.0, 0.4, 1.3, 1.0 };
  float light0_position[] = { 0.0, 0.0, 0.0, 1.0 };
  float light0_direction[] = { 0.0, 0.0, 0.0, 1.0 };

  // Turn off scene default ambient.
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient);

  // Make specular correct for spots.
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);

  glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
  glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);
  glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,1.0);
  glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,180.0);
  glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,0.5);
  glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.001);
  glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.0001);
  glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
  glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light0_direction);

  glEnable(GL_LIGHT0);
  return 0;
}

int createViewVolume() {
  // Specify shape and size of the view volume.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); //load identity matrix into GL_PROJECTION
  gluPerspective(45.0, 1.0, 0.1, 20.0);

  // Specify the position for the view volume
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
gluLookAt(0.0, 0.0, -3.0,
		    0.0, 0.0, 0.0,
		    0.0, 1.0, 0.0);
/*
gluLookAt(2.0, 2.0, -4.0,
		    0.0, 0.0, 0.0,
		    0.0, 1.0, 0.0);
*/
  return 0;
}

char *readShader(char *filename)
{
	char *content = NULL;
	int fd, count;
	fd = open(filename, O_RDONLY);
	count = lseek(fd, 0, SEEK_END);
	content = (char *)calloc(1, (count+1));
	lseek(fd, 0, SEEK_SET);
	count = read(fd, content, count*sizeof(char));
	content[count] = '\0';
	close(fd);
	return content;
}

// Take this out before turning the assignment in
void debugShaderProgram(int id)
{
	int linkStatus = -1;
	glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);

	int compileStatus = -1;
	glGetProgramiv(id, GL_COMPILE_STATUS, &compileStatus);

	if (linkStatus == GL_FALSE || compileStatus == GL_FALSE)
	{
		int size;
		char *c;
		glGetProgramInfoLog(id, 1024, &size, &c[0]);
		printf("%s\n", c);
	} else {
		printf("Shaders loaded correctly.\n");
	}
}

unsigned int loadShaders(char *vertexShaderName, char *fragmentShaderName) 
{
	char *vs, *fs;
	GLuint p;
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	vs = readShader(vertexShaderName);
	fs = readShader(fragmentShaderName);

	glShaderSource(vertexShaderID, 1, (const char **)&vs, NULL);
	glShaderSource(fragmentShaderID, 1, (const char **)&fs, NULL);
	free(vs);
	free(fs);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	p = glCreateProgram();
	glAttachShader(p, fragmentShaderID);
	glAttachShader(p, vertexShaderID);
	glLinkProgram(p);
	glUseProgram(p);

	// Take this out before turning the assignment in
	debugShaderProgram(p);
	
	return p;
}

void createVAOs()
{
	glGenVertexArrays(2, vaoID);
	// Box
	glBindVertexArray(vaoID[0]);

	// Generate ALL VBOs that will be used.
	glGenBuffers(2, vertexVBO);
	glGenBuffers(2, normalVBO);
	glGenBuffers(2, indexVBO);
	glGenBuffers(1, &texCoordVBO);
	glGenBuffers(1, &tangentVBO);
	glGenBuffers(1, &bitangentVBO);
	glGenBuffers(1, &colorVBO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sceneData.box.vertices), sceneData.box.vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(boxShaderProgramID, 0, "in_position");
	
	glBindBuffer(GL_ARRAY_BUFFER, normalVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sceneData.box.normals), sceneData.box.normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, 0);
	glBindAttribLocation(boxShaderProgramID, 1, "in_normal");

	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sceneData.box.colors), sceneData.box.colors, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindAttribLocation(boxShaderProgramID, 2, "in_color");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sceneData.box.indices), 
		sceneData.box.indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

int main(int argc, char **argv) 
{
	init(argc, argv);

	printf("Creating View Volume...\n");
	createViewVolume();

	printf("Creating Lights...\n");
	createLights();
	
	printf("Loading Teapot...\n");
	loadTeapot("teapot.605.obj");

	printf("Loading Box...\n");
	loadBox(2.0, 2.0, 2.0);

	printf("Loading Shaders...\n");
	boxShaderProgramID = loadShaders("boxShader.vert", "boxShader.frag");

	printf("Creating VAOs...\n");
	createVAOs();

	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutKeyboardFunc(input);

	glutMainLoop();
}

