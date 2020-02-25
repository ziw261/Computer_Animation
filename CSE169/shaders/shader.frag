#version 330 core

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

in TData{
    vec3 fragPosition;
    vec3 fragNormal;
} inData;

// uniforms used for lighting
uniform vec3 AmbientColor = vec3(0.2);
uniform vec3 LightDirection1 = normalize(vec3(0, 0, 0.5));
uniform vec3 LightDirection2 = normalize(vec3(0, 0, -10));

uniform vec3 LightColor1 = vec3(0.7,0.2,0.4);
uniform vec3 LightColor2 = vec3(0.3,0.3,0.3);
uniform vec3 DiffuseColor;	// passed in from c++ side NOTE: you can also set the value here and then remove 
							// color from the c++ side

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{

	// Compute irradiance (sum of ambient & direct lighting)
	vec3 irradiance = AmbientColor + LightColor1 * max(0, dot(LightDirection1, inData.fragNormal)) + LightColor2 * max(0, dot(LightDirection2, inData.fragNormal));

	// Diffuse reflectance
	vec3 reflectance = irradiance * DiffuseColor;

	// Gamma correction
	//gl_FragColor = vec4(sqrt(reflectance), 1);
    fragColor = vec4(sqrt(reflectance), 1);
}
