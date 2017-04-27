varying vec3 ec_vnormal, ec_vposition;
varying vec4 colorAttribute;
varying vec4 tcoords;

uniform sampler2D depthTexture;
uniform sampler2D floorTexture;

void main()
{
	
	vec3 P, N, L, V, H;
	vec4 pccoords;
	vec4 diffuse_color = gl_FrontMaterial.diffuse * colorAttribute * gl_LightSource[0].diffuse;
	vec4 specular_color = gl_FrontMaterial.specular * gl_LightSource[0].specular; 
	float shininess = gl_FrontMaterial.shininess;
	float pi = 3.14159265;
	float depthsample;
	vec4 clarity = vec4(1.0,1.0,1.0,0.0);
	

	P = ec_vposition;
	N = normalize(ec_vnormal);
	L = normalize(gl_LightSource[0].position - P);
	V = normalize(-P);				// eye position is (0,0,0)!
	H = normalize(L+V);
	
	pccoords = tcoords/tcoords.w;
	depthsample = texture2D(depthTexture, pccoords.st);
	if (abs(depthsample - pccoords.z) < 0.1 && depthsample < pccoords.z) {
		clarity = vec4(0.3, 0.3, 0.0, 0.0);
	}

	vec4 floorcolor = texture2D(floorTexture, gl_TexCoord[0]);

	// Add a small amount of contrast to the floor for a more appealing texture
	float contrast = 0.2;
	float factor = (1.01568*(contrast + 1.0)) / (1.01568 - contrast);
	floorcolor.x = factor * (floorcolor.x - 0.5) + 0.5;
	floorcolor.y = factor * (floorcolor.y - 0.5) + 0.5;
	floorcolor.z = factor * (floorcolor.z - 0.5) + 0.5;

	diffuse_color *= clarity*max(dot(N,L),0.2);
	specular_color *= clarity*((shininess+2.0)/(8.0*pi))*pow(max(dot(H,N),0.0),shininess);

	float kc = gl_LightSource[0].constantAttenuation;
	float kl = gl_LightSource[0].linearAttenuation;
	float kq = gl_LightSource[0].quadraticAttenuation;
	float d = length(gl_LightSource[0].position);
	float atten = 1.0 / (kc + kl*d + kq*d*d);

	gl_FragColor = atten*(diffuse_color + specular_color);

	// Hacky way of checking if this fragment is on the bottom face
	if (abs(colorAttribute.z - 1.0) < 0.1  && abs(ec_vnormal.y - 1.0) < 0.1) {
		gl_FragColor *= floorcolor;
	}
}
