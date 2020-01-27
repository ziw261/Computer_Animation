#version 330 core
//#version 100 core
// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

//in vec3 fragPosition;
//in vec3 fragNormal;

in TData
{
    vec3 fragPosition;
    vec3 fragNormal;
} inData;

// uniforms used for lighting
uniform vec3 AmbientColor = vec3(0.2);
uniform vec3 LightDirection = normalize(vec3(1, 5, 2));
uniform vec3 LightColor = vec3(1);
uniform vec3 DiffuseColor;	// passed in from c++ side NOTE: you can also set the value here and then remove 
							// color from the c++ side

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{

	// Compute irradiance (sum of ambient & direct lighting)
	vec3 irradiance = AmbientColor + LightColor * max(0, dot(LightDirection, inData.fragNormal));

	// Diffuse reflectance
	vec3 reflectance = irradiance * DiffuseColor;

	// Gamma correction
	//gl_FragColor = vec4(sqrt(reflectance), 1);
    fragColor = vec4(sqrt(reflectance), 1);
}
