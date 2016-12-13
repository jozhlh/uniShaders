// Tessellation pixel shader
// Output colour passed to stage.
Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture : register(t1);

SamplerState SampleTypeWrap : register(s0);
SamplerState SampleTypeClamp : register(s1);


cbuffer LightBuffer : register(b4)
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float specularPower;
	float4 specularColor;
    float3 position;
    float padding;
};

struct InputType
{
    float4 position : SV_POSITION;
    float4 colour : COLOR;
	float3 viewDirection : TEXCOORD1;
	float3 normal : NORMAL;
    float3 position3D : TEXCOORD2;
    float3 lightPos : TEXCOORD3;
    float4 lightViewPosition : TEXCOORD4;
};

float4 main(InputType input) : SV_TARGET
{
    float bias;
	float4 textureColor;
	float3 lightDir;
	float3 direction;
	float lightIntensity;
	float4 color;
	float3 reflection;
	float4 specular;
	float4 finalSpec;
    float lightDepthValue;
    float depthValue;
    float2 projectTexCoord;

    // Set the bias value for fixing the floating point precision issues.
    bias = 0.0001f;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = input.colour;
	
	// Set the default output colour to the ambient light value for all pixels
	color = ambientColor;

    // Calculate the projected texture coordinates.
    projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;
    // Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
    if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
    {
	    // Sample the shadow map depth value from the depth texture using the sampler at the 	projected texture coordinate location.
        depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;

	    // Calculate the depth of the light.
        lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;

	    // Subtract the bias from the lightDepthValue.
        lightDepthValue = lightDepthValue - bias;

	    // Compare the depth of the shadow map value and the depth of the light to determine 	whether to shadow or to light this pixel.
	    // If the light is in front of the object then light the pixel, if not then shadow this 	pixel since an object (occluder) is casting a shadow on it.
        if (lightDepthValue < depthValue)
        {

            direction = normalize(lightDirection);
            // Invert the light direction for calculations.
            lightDir = direction;

            // Calculate the amount of light on this pixel.
            lightIntensity = saturate(dot(input.normal, -lightDir));

            if (lightIntensity > 0.0f)
            {
		        // Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
                color += (diffuseColor * lightIntensity);
                color = saturate(color);

		        // Calculate reflection vector based on the light intensity, normal vector and light direction
                reflection = reflect(lightDir, input.normal);

		        // Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
                specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);

		        // Sum up specular light
                finalSpec = specularColor * specular;
            }
        }
    }


	//direction = normalize(lightDirection);
    //Point Light
    //lightDir = normalize(input.position3D - position);

	//// Invert the light direction for calculations.
 //   lightDir = direction;

 //   // Calculate the amount of light on this pixel.
	//lightIntensity = saturate(dot(input.normal, -lightDir));

	//if (lightIntensity > 0.0f)
	//{
	//	// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
	//	color += (diffuseColor * lightIntensity);
	//	color = saturate(color);

	//	// Calculate reflection vector based on the light intensity, normal vector and light direction
 //       reflection = reflect(lightDir, input.normal);

	//	// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
	//	specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);

	//	// Sum up specular light
	//	finalSpec = specularColor * specular;
	//}

	// Turn this on to see the texture
	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
	//color = color * textureColor;

	// Add the specular component last to the output colour.
	color = saturate(color + finalSpec);

   // color = input.colour;
    return color;
}