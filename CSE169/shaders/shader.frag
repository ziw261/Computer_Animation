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
//uniform vec3 LightDirection = normalize(vec3(1, 5, 2));

//uniform vec3 LightColor = vec3(0,1,0);
//uniform vec3 LightColor1 = vec3(0,0,1);
uniform vec3 LightDirection[2] = vec3[2](vec3(0,0,10), vec3(0,0,-10));
uniform vec3 LightColor[2] = vec3[2](vec3(0,1,0), vec3(1,0,0));
uniform vec3 DiffuseColor;	// passed in from c++ side NOTE: you can also set the value here and then remove 
							// color from the c++ side

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;
//out vec4 fragColor1;

void main()
{
    
    vec3 reflectance;
    //vec3 reflectance2;
    //vec3 reflected;
    vec3 result;
    
    
    for(int i=0; i<2; i++){
        //reflectance = (AmbientColor + LightColor[i] * max(0, dot(LightDirection[i], inData.fragNormal))) * DiffuseColor;
        
        //reflected += max(dot(LightDirection[i],inData.fragNormal),0) * LightColor[i];
        
        //reflectance += AmbientColor*LightColor[i] + DiffuseColor * LightColor[i];
        
        //reflectance += AmbientColor + max(0, dot(LightDirection[i], inData.fragNormal)) * LightColor[i];
        vec3 ambient = (0.2f * LightColor[i]);
        result += ambient * LightColor[i];
        vec3 lightDir = normalize(inData.fragPosition - LightDirection[i] );
        float diff = max(dot(inData.fragNormal, lightDir), 0.0);
        vec3 diffuse = (diff * LightColor[i]);
        result += diffuse;
        


        

    }
     
    
	// Compute irradiance (sum of ambient & direct lighting)
	//vec3 irradiance = AmbientColor + LightColor * max(0, dot(LightDirection, inData.fragNormal));
    //vec3 irradiance1 = AmbientColor + LightColor1 * max(0, dot(LightDirection, inData.fragNormal));
	// Diffuse reflectance
	//vec3 reflectance = irradiance * DiffuseColor;
    //vec3 reflectance1 = irradiance1 * DiffuseColor;

	// Gamma correction
	//fragColor = vec4(sqrt(reflectance), 1);
    fragColor = vec4(result, 1);
    //vec3 gamma = vec3(1/2.5);
    //fragColor = vec4(pow(reflectance,gamma), 0.5);
    //fragColor = vec4(reflectance, 1);
    //fragColor = vec4((reflected+AmbientColor)*DiffuseColor , 1);
    //fragColor1 = vec4(sqrt(reflectance1),1);
}
