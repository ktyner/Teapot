
in vec3 in_color;
in vec3 in_position;
in vec3 in_normal;

varying vec3 ec_vnormal, ec_vposition;
varying vec4 colorAttribute;

void main()
{
	colorAttribute = in_color;
	ec_vnormal = gl_NormalMatrix*in_normal;
	ec_vposition = gl_ModelViewMatrix*in_position;
	gl_Position = gl_ProjectionMatrix*gl_ModelViewMatrix*in_position;
}
