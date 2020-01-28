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
uniform vec3 LightDirection[2] = vec3[2](vec3(0,0,10), vec3(0,0,-10));
uniform vec3 LightColor[2] = vec3[2](vec3(0,0,1), vec3(1,0,0));
uniform vec3 DiffuseColor;	// passed in from c++ side NOTE: you can also set the value here and then remove 
							// color from the c++ side
// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;
//out vec4 fragColor1;

void main()
{
    
    vec3 reflectance;
    vec3 reflectance2;
    //vec3 reflectance2;
    //vec3 reflected;
    vec3 result;
    
    reflectance = (AmbientColor + LightColor[0] * max(0, dot(LightDirection[0], inData.fragNormal))) * DiffuseColor;
        
    reflectance2 = (AmbientColor + LightColor[1] * max(0, dot(LightDirection[1], inData.fragNormal))) * DiffuseColor;
        
    result = reflectance + reflectance2;
   
	// Gamma correction
	fragColor = vec4(sqrt(result), 1);

}
