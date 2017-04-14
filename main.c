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
#include <math.h>

#include "global.h"

#define PI 3.14159265359

void init(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_MULTISAMPLE|GLUT_ACCUM|GLUT_DOUBLE);
	glutInitWindowSize(768, 768);
	glutInitWindowPosition(1000, 200);
	glutCreateWindow("Hopefully a teapot");
	glClearColor(1.0, 0.0, 0.7, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnableClientState(GL_VERTEX_ARRAY);
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

		//glVertexPointer(3, GL_FLOAT, 0, sceneData.teapot.vertices);
		//glNormalPointer(GL_FLOAT, 0, sceneData.teapot.normals);

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
		{	 dx, -dy, -dz	},
		{	 dx, -dy,  dz	},
		{	 dx,  dy,  dz	},
		// Top Face
		{	-dx,  dy,  dz	},
		{	-dx,  dy, -dz	},
		{	 dx,  dy, -dz	},
		{	 dx,  dy,  dz	},
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
	
	// Load box data into GPU
	glVertexPointer(3, GL_FLOAT, 0, sceneData.box.vertices);
	glColorPointer(3, GL_FLOAT, 0, sceneData.box.colors);
	glNormalPointer(GL_FLOAT, 0, sceneData.box.normals);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawElements(GL_QUADS, 20, GL_UNSIGNED_BYTE, sceneData.box.indices);

	// Load teapot data into GPU
	glVertexPointer(3, GL_FLOAT, 0, sceneData.teapot.vertices);
	glNormalPointer(GL_FLOAT, 0, sceneData.teapot.normals);
	glDisableClientState(GL_COLOR_ARRAY);

	// Stores the current matrix configuration to allow for local transformations
	glPushMatrix();
		glTranslatef(0.0, -1.0, 0.0);
		glRotatef(30.0, 0.0, 1.0, 0.0);
		glScalef(0.5, 0.5, 0.5);
		glDrawElements(GL_QUADS, sceneData.teapot.curIndex, GL_UNSIGNED_INT, sceneData.teapot.indices);
	glPopMatrix();
}
void update() {}
void input(unsigned char key, int x, int y) 
{
	if (key) exit(0);
}

int createLights(Vector3f pos, Vector3f color) {
  // Fill light
  float light0_ambient[] = { 0.3, 0.3, 0.3, 0.0 };
  float light0_diffuse[] = { color.x, color.y, color.z, 1.0 };
  float light0_specular[] = { color.x, color.y, color.z, 1.0 };
  float light0_position[] = { pos.x, pos.y, pos.z, 1.0 };
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
  
  gluLookAt(/*position*/0.0, 0.0, -3.0,/*viewpoint*/ 0.0, 0.0, 0.0,/*up*/ 0.0, 1.0, 0.0);

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
	float az, el;
	Vector3f result;
	
	az = 2.0*PI*phi(2, i);
	el = asin(phi(3, i));
	result.x = -sin(az)*cos(el);
	result.y = sin(el);
	result.z = cos(az)*cos(el);
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
		Vector3f null = {1000.0, 1000.0, 1000.0};
		return null;
	}

	result = add(add(mul(v0, (1.0 - result.y - result.z)), mul(v1, result.y)), mul(v2, result.z));
	
	return result;
}

void renderScene()
{
	int N = 100;
	glClear(GL_ACCUM_BUFFER_BIT);

	// Fill half of the accumulation buffer with the main light
	Vector3f lightPosition;
	Vector3f lightColor;
	
	// Fill the other half with the global illumination
	int i = 1;
	for (i = 0; i < N; i++)
	{
		Vector3f currentRay = normalize(halton(i));
		Vector3f color = {1.0, 1.0, 0.791};
		Vector3f source = {0.0, 0.8, 0.0};
		int j;
		for (j = 0; j < 20; j+=4)
		{
			Vector3f v0 = sceneData.box.vertices[j];
			Vector3f v1 = sceneData.box.vertices[j+1];
			Vector3f v2 = sceneData.box.vertices[j+2];
			Vector3f lightPosition = getRayTriangleIntersectionPoint(source, currentRay, v0, v1, v2);
			if (abs(lightPosition.x - 1000.0) > 0.1) {
				color = sceneData.box.colors[j];
				createLights(mul(lightPosition, 0.8), mul(color, 0.4));
				display();
				glAccum(GL_ACCUM, 1.0/(float)N);
			}


			v0 = sceneData.box.vertices[j+2];
			v1 = sceneData.box.vertices[j+3];
			v2 = sceneData.box.vertices[j];
			lightPosition = getRayTriangleIntersectionPoint(source, currentRay, v0, v1, v2);
			if (abs(lightPosition.x - 1000.0) > 0.1)
			{
				color = sceneData.box.colors[j];
				createLights(mul(lightPosition, 0.8), mul(color, 0.4));
				printVector(mul(lightPosition, 0.8));
				display();
				glAccum(GL_ACCUM, 1.0/(float)N);
			}
		}
	}
	glAccum(GL_RETURN, 1.0);
	glutSwapBuffers();
}

int main(int argc, char **argv) 
{
	init(argc, argv);

	printf("Creating View Volume...\n");
	createViewVolume();

	printf("Loading Teapot...\n");
	loadTeapot("teapot.605.obj");

	printf("Loading Box...\n");
	loadBox(2.0, 2.0, 2.0);

	printf("Loading Shaders...\n");
	boxShaderProgramID = loadShaders("boxShader.vert", "boxShader.frag");

	glutDisplayFunc(renderScene);
	glutIdleFunc(update);
	glutKeyboardFunc(input);

	glutMainLoop();
}

