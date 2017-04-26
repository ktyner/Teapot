#ifndef PARSE_H
#define PARSE_H

void loadTeapot(const char *objFile)
{
	FILE *file = fopen(objFile, "r");
	char line[256];
	char *lineData;

	int texCount = 0;

	while (fgets(line, sizeof(line), file))
		if (line[0]+line[1] == 'v'+' ')
			sceneData.vertexCount++;

	Vector3f *normals;
	int nindex = 0;

	normals = malloc(sizeof(Vector3f) * sceneData.vertexCount);
	sceneData.teapot.vertices = malloc(sizeof(Vector3f) * sceneData.vertexCount);
	sceneData.teapot.normals = malloc(sizeof(Vector3f) * sceneData.vertexCount);
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
					break;
				case 'x':
					break;
				case 'y':
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
				lineData = strtok(NULL, " /"); lineData = strtok(NULL, " /");
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

	Vector3f red = {0.49, 0.0, 1.0};
	Vector3f green = {0.49, 1.0, 0.0};
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
		{	4.0,  0.0	},
		{	4.0,  4.0	},
		{	0.0,  4.0	},

		{	0.0,  0.0	},
		{	4.0,  0.0	},
		{	4.0,  4.0	},
		{	0.0,  4.0	}
	};
	
	sceneData.box.vertices = malloc(sizeof(Vector3f) * 20);
	sceneData.box.colors = malloc(sizeof(Vector3f) * 20);
	sceneData.box.normals = malloc(sizeof(Vector3f) * 20);
	sceneData.box.indices = malloc(sizeof(GLubyte) * 20);
	sceneData.box.texCoords = malloc(sizeof(Vector2f) * 20);

	int i;
	for (i = 0; i < 20; i++)
	{
		sceneData.box.vertices[i] = vertices[i];
		sceneData.box.colors[i] = colors[i];
		sceneData.box.normals[i] = normals[i];
		sceneData.box.texCoords[i] = texCoords[i];
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
	
	sceneData.toplight.vertices = malloc(sizeof(Vector3f) * 4);
	sceneData.toplight.indices = malloc(sizeof(GLubyte) * 4);

	int i;
	for (i = 0; i < 4; i++)
	{
		sceneData.toplight.vertices[i] = vertices[i];
		sceneData.toplight.indices[i] = i;
	}
}

#endif