// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 ambientColor;
	float4 diffuseColor;
    float3 lightDirection;
	float padding;
	float3 lightPosition;
    float specularPower;
	float4 specularColor;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
	float4 position3D : TEXCOORD2;
};

float4 main(InputType input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
	float3 direction;
    float lightIntensity;
    float4 color;
	float3 reflection;
	float4 specular;
	float4 finalSpec;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);
	
	// Set the default output colour to the ambient light value for all pixels
	color = ambientColor;

	//// For Specular Diretional Light
	/*
	direction = normalize(lightDirection);

	// Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));
	*/

	//// For Point Light
	// Invert the light direction for calculations.
	lightDir = normalize(input.position3D.xyz - lightPosition.xyz);

	// Calculate the amount of light on this pixel.
	lightIntensity = saturate(dot(input.normal, -lightDir));

	if (lightIntensity > 0.0f)
	{
		// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
		color += (diffuseColor * lightIntensity);
		color = saturate(color);


		//// For Specular Lighting
		/*
		// Calculate reflection vector based on the light intensity, normal vector and light direction
		reflection = reflect(direction, input.normal);

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
		specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);

		// Sum up specular light
		finalSpec = specularColor * specular;
		*/
	}

	// Turn this on to see the texture
	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
	color = color * textureColor;

	// Add the specular component last to the output colour.
	//color = saturate(color + finalSpec);

    return color;
}

