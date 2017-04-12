
in vec3 in_color;
in vec3 in_position;
in vec3 in_normal;

varying vec3 ec_vnormal, ec_vposition;
varying vec4 colorAttribute;

void main()
{
	colorAttribute = gl_Color;
	ec_vnormal = gl_NormalMatrix*gl_Normal;
	ec_vposition = gl_ModelViewMatrix*gl_Position;
	gl_Position = gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Position;
}
