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
#define XRES 1280
#define YRES 1024
#define RAYNOTHIT 1000000

void init(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_MULTISAMPLE|GLUT_ACCUM|GLUT_DOUBLE);
	glutInitWindowSize(XRES, YRES);
	glutInitWindowPosition(1000, 200);
	glutCreateWindow("Hopefully a teapot");
	glClearColor(1.0, 0.0, 0.7, 1.0);
	glEnable(GL_DEPTH_TEST);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
}

void loadTeapot(const char *objFile)
{
	FILE *file = fopen(objFile, "r");
	char line[256];
	char *lineData;

	int texCount = 0;

	while (fgets(line, sizeof(line), file))
	{
		if (line[0]+line[1] == 'v'+' ')
		{
			sceneData.vertexCount++;
		}
		else if (line[0]+line[1] == 'v'+'t') 
		{
			texCount++;
		}
	}

	Vector3f *normals;
	Vector2f *texCoords;
	int nindex = 0, texindex = 0;

	normals = malloc(sizeof(Vector3f) * sceneData.vertexCount);
	texCoords = malloc(sizeof(Vector2f) * texCount);
	sceneData.teapot.vertices = malloc(sizeof(Vector3f) * sceneData.vertexCount);
	sceneData.teapot.normals = malloc(sizeof(Vector3f) * sceneData.vertexCount);
	sceneData.teapot.tangents = malloc(sizeof(Vector3f) * sceneData.vertexCount);
	sceneData.teapot.bitangents = malloc(sizeof(Vector3f) * sceneData.vertexCount);
	sceneData.teapot.texCoords = malloc(sizeof(Vector2f) * texCount);
	sceneData.teapot.indices = malloc(sizeof(GLuint) * sceneData.vertexCount * 4);

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
			// Material file stuff here
		}
	}
}

void loadBox(float dx, float dy, float dz)
{
	dx /= 2.0;
	dy /= 2.0;
	dz /= 2.0;

	Vector3f red = {1.0, 0.0, 0.0};
	Vector3f green = {0.0, 1.0, 0.0};
	Vector3f white = {1.0, 1.0, 1.0};

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
		white, white, white, white,
		// Right Face
		green, green, green, green,
		// Left Face
		red, red, red, red,
		// Top Face
		white, white, white, white,
		// Bottom Face
		white, white, white, white
	};

	Vector2f texCoords[] =
	{
		{	0.0,  0.0	},
		{	1.0,  0.0	},
		{	1.0,  1.0	},
		{	0.0,  1.0	},

		{	0.0,  0.0	},
		{	1.0,  0.0	},
		{	1.0,  1.0	},
		{	0.0,  1.0	},

		{	0.0,  0.0	},
		{	1.0,  0.0	},
		{	1.0,  1.0	},
		{	0.0,  1.0	},

		{	0.0,  0.0	},
		{	1.0,  0.0	},
		{	1.0,  1.0	},
		{	0.0,  1.0	},

		{	0.0,  0.0	},
		{	1.0,  0.0	},
		{	1.0,  1.0	},
		{	0.0,  1.0	},

		{	0.0,  0.0	},
		{	1.0,  0.0	},
		{	1.0,  1.0	},
		{	0.0,  1.0	}
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

void loadTopLight(float dx, float dz)
{
	dx /= 2.0;
	dz /= 2.0;
	Vector3f vertices[] = 
	{
		// Top Light
		{	-dx/3.0,  0.0,  dz/3.0	},
		{	-dx/3.0,  0.0, -dz/3.0	},
		{	 dx/3.0,  0.0, -dz/3.0	},
		{	 dx/3.0,  0.0,  dz/3.0	}
	};

	Vector3f normals[] =
	{
		// Top Light
		{	 0.0, -1.0,  0.0	},
		{	 0.0, -1.0,  0.0	},
		{	 0.0, -1.0,  0.0	},
		{	 0.0, -1.0,  0.0	}
	};
	
	sceneData.toplight.vertices = malloc(sizeof(Vector3f) * 4);
	sceneData.toplight.normals = malloc(sizeof(Vector3f) * 4);
	sceneData.toplight.indices = malloc(sizeof(GLubyte) * 4);

	int i;
	for (i = 0; i < 4; i++)
	{
		sceneData.toplight.vertices[i] = vertices[i];
		sceneData.toplight.normals[i] = normals[i];
		sceneData.toplight.indices[i] = i;
	}
}

float myrand() { return ((float)(rand() % 100000)/100000.0 - 0.5); }

// flag - 0 = render with shadows, 1 = render without shadows
void display() 
{
	Vector3f lightJitter = {myrand()*0.4, myrand()*0.07, myrand()*0.07};
	Vector3f aaJitter = {myrand(), myrand(), myrand()};
	aaJitter = mul(aaJitter, 0.003);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Render Light	
	glUseProgram(0);
	glVertexPointer(3, GL_FLOAT, 0, sceneData.toplight.vertices);
	glNormalPointer(GL_FLOAT, 0, sceneData.toplight.normals);
	glPushMatrix();
		glTranslatef(lightJitter.y, 0.9999, lightJitter.z);
		glScalef(1.0-lightJitter.x, 1.0-lightJitter.x, 1.0-lightJitter.x);
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, sceneData.toplight.indices);
	glPopMatrix();

	glUseProgram(phongProgramID);

	// Render Box
	glUseProgram(boxShaderProgramID);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, sceneData.box.vertices);
	glColorPointer(3, GL_FLOAT, 0, sceneData.box.colors);
	glNormalPointer(GL_FLOAT, 0, sceneData.box.normals);
	glPushMatrix();
		glTranslatef(aaJitter.x, aaJitter.y, aaJitter.z);
		glDrawElements(GL_QUADS, 20, GL_UNSIGNED_BYTE, sceneData.box.indices);
	glPopMatrix();

	// Render Teapot
	glUseProgram(teapotShaderProgramID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floorTextureID);
	glVertexPointer(3, GL_FLOAT, 0, sceneData.teapot.vertices);
	glNormalPointer(GL_FLOAT, 0, sceneData.teapot.normals);
	glTexCoordPointer(2, GL_FLOAT, 0, sceneData.teapot.texCoords);
	glDisableClientState(GL_COLOR_ARRAY);
	glPushMatrix();
		glTranslatef(aaJitter.x, aaJitter.y-1.0, aaJitter.z);
		glRotatef(50.0, 0.0, 1.0, 0.0);
		glScalef(0.5, 0.5, 0.5);
		glDrawElements(GL_QUADS, sceneData.teapot.curIndex, GL_UNSIGNED_INT, sceneData.teapot.indices);
	glPopMatrix();

	glFlush();
}

void input(unsigned char key, int x, int y) { if (key=='q') exit(0); }

void createLights(Vector3f pos, Vector3f color) {
	light0_diffuse[0] = color.x; 
	light0_diffuse[1] = color.y; 
	light0_diffuse[2] = color.z; 
	light0_diffuse[3] = 1.0;

	light0_position[0] = pos.x;
	light0_position[1] = pos.y; 
	light0_position[2] = pos.z; 
	light0_position[3] = 1.0;

	float light0_specular[] = { color.x, color.y, color.z, 1.0 };

	glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);
	glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,0.5);
	glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.001);
	glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.0001);
	glLightfv(GL_LIGHT0,GL_POSITION,light0_position);

	glEnable(GL_LIGHT0);
}

void createViewVolume(Vector3f ep, Vector3f vp) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)(XRES)/(float)(YRES), 0.1, 20.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(ep.x, ep.y, ep.z, vp.x, vp.y, vp.z, 0.0, 1.0, 0.0);
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
		Vector3f null = {RAYNOTHIT, RAYNOTHIT, RAYNOTHIT};
		return null;
	}

	result = add(add(mul(v0, (1.0 - result.y - result.z)), mul(v1, result.y)), mul(v2, result.z));
	
	return result;
}

void save_matrix(float *ep, float *vp)
{
	glMatrixMode(GL_TEXTURE); 
	// This must match the unit used in the vertex shader.
	glActiveTexture(GL_TEXTURE7);
	glLoadIdentity();
	glTranslatef(0.0,0.0,-0.005);
	glScalef(0.5,0.5,0.5);
	glTranslatef(1.0,1.0,1.0);
	gluPerspective(45.0,(float)(XRES)/(float)(YRES),0.1,20.0);
	gluLookAt(ep[0], ep[1], ep[2], vp[0], vp[1], vp[2], 0.0,1.0,0.0);
}

void set_uniform(int p)
{
	int location;
	location = glGetUniformLocation(p,"depthTexture");
	glUniform1i(location,6);
}

void renderWithShadows(Vector3f lightPosition, Vector3f lightColor)
{
	float eyepoint[3], viewpoint[3];
	int k;

	glEnable(GL_MULTISAMPLE_ARB);

	// Fill the framebuffer with depth data from the point of view of the light
	glBindFramebufferEXT(GL_FRAMEBUFFER, 1);
		glUseProgram(0);

		light0_position[0] = lightPosition.x, light0_position[1] = lightPosition.y, light0_position[2] = lightPosition.z;
		light0_direction[0] = -light0_position[0];
		light0_direction[1] = -1.0-light0_position[1];
		light0_direction[2] = -light0_position[2];

		for (k = 0; k < 3; k++) {
			eyepoint[k] = light0_position[k];
			viewpoint[k] = light0_position[k] + light0_direction[k];
		}

		Vector3f ep = {eyepoint[0], eyepoint[1], eyepoint[2]};
		Vector3f vp = {viewpoint[0], viewpoint[1], viewpoint[2]};
		createViewVolume(ep, vp);

		Vector3f l0_pos = {light0_position[0], light0_position[1], light0_position[2]};
		createLights(l0_pos, lightColor);
		display();
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

	// Store this viewpoint matrix into texture matrix 7
	save_matrix(eyepoint, viewpoint);

	glUseProgram(boxShaderProgramID);
	set_uniform(boxShaderProgramID);
	set_uniform(teapotShaderProgramID);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, 1);

	eyepoint[0] = 0.0, eyepoint[1] = 0.0, eyepoint[2] = -2.9;
	viewpoint[0] = 0.0, viewpoint[1] = 0.0, viewpoint[2] = 0.0;

	ep.x = eyepoint[0], ep.y = eyepoint[1], ep.z = eyepoint[2];
	vp.x = viewpoint[0], vp.y = viewpoint[1], vp.z = viewpoint[2];
	createViewVolume(ep, vp);
	createLights(l0_pos, lightColor);
	display();
}

int trace(int j, int N, Vector3f currentRay, Vector3f v0, Vector3f v1, Vector3f v2)
{
	Vector3f color = {1.0, 1.0, 0.791};
	Vector3f source = {0.0, 0.9, 0.0};
	Vector3f lightPosition = getRayTriangleIntersectionPoint(source, currentRay, v0, v1, v2);
	if (lightPosition.x != RAYNOTHIT)
	{
		lightPosition = mul(lightPosition, 0.8);
		if (lightPosition.y <= 0.) lightPosition.y = 0.5;
		color = mul(sceneData.box.colors[j], 0.5);
		renderWithShadows(lightPosition, color);

		glAccum(GL_ACCUM, 1.0/(float)N);
		return 1;
	}
	return 0;
}

void renderScene()
{
	int N = 1000;
	glClear(GL_ACCUM_BUFFER_BIT);

	int i;
	for (i = 0; i < N; i++)
	{
		Vector3f currentRay = normalize(halton(i));

		Vector3f v0, v1, v2;

		int j;
		for (j = 0; j < 20; j+=4)
		{
			v0 = sceneData.box.vertices[j];
			v1 = sceneData.box.vertices[j+1];
			v2 = sceneData.box.vertices[j+2];
			trace(j, N, currentRay, v0, v1, v2);

			v0 = sceneData.box.vertices[j+2];
			v1 = sceneData.box.vertices[j+3];
			v2 = sceneData.box.vertices[j];
			trace(j, N, currentRay, v0, v1, v2);
		}
	}
	glAccum(GL_RETURN, 1.0);
	glutSwapBuffers();
}

void prepareFramebuffer()
{
	// Set properties of texture id #1.
	glBindTexture(GL_TEXTURE_2D,1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, XRES, YRES, 0, 
		GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint frameBufferID;
	glGenFramebuffers(1, &frameBufferID);
	glBindFramebufferEXT(GL_FRAMEBUFFER,frameBufferID);
	glDrawBuffer(GL_NONE);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,1,0);
	glBindFramebufferEXT(GL_FRAMEBUFFER,0);
}

int loadTexture(const char *filename) {
	FILE *fopen(), *fptr;
	char buf[512];
	int im_size, im_width, im_height, max_color;
	unsigned char *texture_bytes, *parse;

	fptr=fopen(filename,"r");
	if (!fptr) return -1;
	fgets(buf,512,fptr);
	do{
	fgets(buf,512,fptr);
	} while(buf[0]=='#');

	parse = strtok(buf," \t");
	im_width = atoi(parse);

	parse = strtok(NULL," \n");
	im_height = atoi(parse);

	fgets(buf,512,fptr);
	parse = strtok(buf," \n");
	max_color = atoi(parse);

	im_size = im_width*im_height;
	texture_bytes = (unsigned char *)calloc(3,im_size);
	fread(texture_bytes,3,im_size,fptr);
	fclose(fptr);

	glGenTextures(1, &floorTextureID);
	glBindTexture(GL_TEXTURE_2D, floorTextureID);

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,im_width,im_height,0,GL_RGB,
	           GL_UNSIGNED_BYTE,texture_bytes);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	cfree(texture_bytes);

	int location = glGetUniformLocation(boxShaderProgramID, "floorTexture");
	glUniform1i(location, 0);

	return 0;
}

int main(int argc, char **argv) 
{
	init(argc, argv);

	printf("Preparing Framebuffer...\n");
	prepareFramebuffer();

	printf("Loading environment...\n");
	loadTopLight(2.0, 2.0);
	loadTeapot("teapot.605.obj");
	loadBox(2.0, 2.0, 2.0);

	printf("Loading Shaders...\n");
	boxShaderProgramID = loadShaders("boxShader.vert", "boxShader.frag");
	teapotShaderProgramID = loadShaders("teapotShader.vert", "teapotShader.frag");

	printf("Loading textures...\n");
	loadTexture("white_marble_75.ppm");

	glutDisplayFunc(renderScene);
	glutKeyboardFunc(input);

	glutMainLoop();
}

