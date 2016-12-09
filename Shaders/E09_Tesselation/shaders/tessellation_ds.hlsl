// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct ConstantOutputType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

struct InputType
{
    float3 position : POSITION;
    float4 colour : COLOR;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float4 colour : COLOR;
};

[domain("tri")]
OutputType main(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
    float3 vertexPosition;
    OutputType output;
 
    // Determine the position of the new vertex.
	// Invert the y and Z components of uvwCoord as these coords are generated in UV space and therefore y is positive downward.
	// Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).
	vertexPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;
    
	// Making the changes here, looks at the position in world space

	if (vertexPosition.x < 0)
	{
		vertexPosition.y += 1;
	}


    // Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Making the changes here, looks at the position in screen space

	/*if (output.position.x < 0)
	{
		output.position.y += 1;
	}*/

    // Send the input color into the pixel shader.
    output.colour = patch[0].colour;

    return output;
}

float Turbulence(float x, float y, float z, float f)
{
	float t = -.5f;
	for (; f < 800 / 12; f += 2)
	{
		t+= 
	}
}

