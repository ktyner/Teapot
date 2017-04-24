varying vec3 ec_vnormal, ec_vposition;
varying vec4 colorAttribute;

void main()
{
	
	vec3 P, N, L, V, H;
	vec4 diffuse_color = colorAttribute * gl_LightSource[0].diffuse;
	vec4 specular_color = gl_LightSource[0].specular; 
	float shininess = gl_FrontMaterial.shininess;
	float pi = 3.14159265;

	P = ec_vposition;
	N = normalize(ec_vnormal);
	L = normalize(gl_LightSource[0].position - P);
	V = normalize(-P);				// eye position is (0,0,0)!
	H = normalize(L+V);
	
	diffuse_color *= max(dot(N,L),0.2);
	specular_color *= ((shininess+2.0)/(8.0*pi))*pow(max(dot(H,N),0.0),shininess);

	float kc = gl_LightSource[0].constantAttenuation;
	float kl = gl_LightSource[0].linearAttenuation;
	float kq = gl_LightSource[0].quadraticAttenuation;
	float d = length(gl_LightSource[0].position);
	float atten = 1.0 / (kc + kl*d + kq*d*d);

	gl_FragColor = atten*(diffuse_color + specular_color);
}
