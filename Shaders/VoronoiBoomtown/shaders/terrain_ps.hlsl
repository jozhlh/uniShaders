// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightBuffer : register(b0)
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
    int regionNum;
    float3 terrainBaseCol;
    bool identify;
    int numOfRegions;
    float2 padding;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
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

    if (identify)
    {
        color.x = 0.7f;
        color.y = 0.7f;
        color.z = 0.7f;
        color.w = 0.7f;
    }

    direction = normalize(lightDirection);

	// Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));

    if (lightIntensity > 0.0f)
    {
		// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
        if (identify)
        {
            color += (color * lightIntensity);
        }
        else
        {
            color += (diffuseColor * lightIntensity);
        }
        
        color = saturate(color);

		// Calculate reflection vector based on the light intensity, normal vector and light direction
        reflection = reflect(direction, input.normal);

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);

		// Sum up specular light
        finalSpec = specularColor * specular;

    }

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    float4 c;
    c.x = terrainBaseCol.x;
    c.y = terrainBaseCol.y;
    c.z = terrainBaseCol.z;
    c.w = 1.0f;

    if (identify)
    {
        float modifier = 1.0f -((float) regionNum / (float) numOfRegions);
         //find the highest rgb value
        int highest = 0;
        float high = terrainBaseCol.x;
        if (terrainBaseCol.y > high)
        {
            high = terrainBaseCol.y;
            highest = 1;
        }
        if (terrainBaseCol.z > high)
        {
            high = terrainBaseCol.z;
            highest = 2;
        }

        switch(highest)
        {
            case 0:
                c.x -= (terrainBaseCol.x * modifier);
                c.y += ((1.0f - terrainBaseCol.y) * modifier);
                //c.z += ((1.0f - terrainBaseCol.z) * modifier);
                break;
            case 1:
                c.y -= (terrainBaseCol.y * modifier);
                //c.x += ((1.0f - terrainBaseCol.x) * modifier);
                c.z += ((1.0f - terrainBaseCol.z) * modifier);
                break;
            case 2:
                c.z -= (terrainBaseCol.z * modifier);
                c.x += ((1.0f - terrainBaseCol.x) * modifier);
                //c.y += ((1.0f - terrainBaseCol.y) * modifier);
                break;
        }

        //c = { modifier, modifier, modifier, 1.0f};
        c.x = modifier;
        c.y = modifier;
        c.z = modifier;
        c.w = 1.0f;
        //c.z = ((float)regionNum / (float)numOfRegions);
        
        //if (regionNum == 0)
        //{
        //    c.x = 1.0f;
        //    c.y = 0.0f;
        //    c.z = 0.0f;
        //    c.w = 1.0f;
        //}
        //else if (regionNum == 1)
        //{
        //    c.x = 0.0f;
        //    c.y = 1.0f;
        //    c.z = 0.0f;
        //    c.w = 1.0f;
        //}

        color = color * c;
        color = saturate(color + finalSpec);
        // Add the specular component last to the output colour.
        //color = saturate(color + finalSpec);
    }
    else
    {
        color = color * c;
        
        // Add the specular component last to the output colour.
        color = saturate(color + finalSpec);
    }




    return color;
}

