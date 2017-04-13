varying vec3 ec_vnormal, ec_vposition;
varying vec4 colorAttribute;

void main()
{
	
	vec3 P, N, L, V, H;
	vec4 diffuse_color = gl_FrontMaterial.diffuse * colorAttribute * gl_LightSource[0].diffuse; 
	vec4 specular_color = gl_FrontMaterial.specular * gl_LightSource[0].specular; 
	float shininess = gl_FrontMaterial.shininess;
	float pi = 3.14159265;

	P = ec_vposition;
	N = normalize(ec_vnormal);
	L = normalize(gl_LightSource[0].position - P);
	V = normalize(-P);				// eye position is (0,0,0)!
	H = normalize(L+V);
		
	diffuse_color *= max(dot(N,L),0.0);
	specular_color *= ((shininess+2.0)/(8.0*pi))*pow(max(dot(H,N),0.0),shininess);

	gl_FragColor = (diffuse_color + specular_color);
}
