
varying vec3 ec_vnormal, ec_vposition;
varying vec4 colorAttribute;
varying vec4 tcoords;

void main()
{
	colorAttribute = gl_Color;
	ec_vnormal = gl_NormalMatrix*gl_Normal;
	ec_vposition = gl_ModelViewMatrix*gl_Vertex;
	gl_Position = gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex;
	tcoords = gl_TextureMatrix[7]*gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
