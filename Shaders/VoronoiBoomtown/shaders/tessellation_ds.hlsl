// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices
// S:\git\uniShaders\Shaders\E03_SpecularLighting\shaders\light_vs.hlsl(11,33-35): error X3530: invalid register specification, expected 'b' binding when cb used
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer TessellationWarpBuffer : register(b2)
{
	int powers;
	float repeats;
	float severity;
	float lerpAmount;
	float3 baseColour;
	bool targetSin;
}

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
    OutputType output;
    float3 vertexPosition;
     
    // Determine the position of the new vertex.
	// Invert the y and Z components of uvwCoord as these coords are generated in UV space and therefore y is positive downward.
	// Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).
	vertexPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;
    
	float4 sinColourModifier = float4(baseColour.x, baseColour.y, baseColour.z, 1.0f);
	float4 cosColourModifier = sinColourModifier;
	float4 colourModifier = sinColourModifier;
	float3 radialVector = vertexPosition;
	float3 sinVertexPosition = vertexPosition;
	float3 cosVertexPosition = vertexPosition;
    float radius = length(radialVector);

	float vxr = vertexPosition.x * repeats;
	float vyr = vertexPosition.y * repeats;
	float vzr = vertexPosition.z * repeats;
    
	//// Calculate sin wave surface modifier according to the parametric equations:
    // x1 = x0 + severity*x0*(sin(repeats*x0)*sin(repeats*y0)*sin(repeats*z0))^powers
    // y1 = y0 + severity*y0*(sin(repeats*x0)*sin(repeats*y0)*sin(repeats*z0))^powers
    // z1 = z0 + severity*z0*(sin(repeats*x0)*sin(repeats*y0)*sin(repeats*z0))^powers

	float sinWarp = 1.0f;
	float sinvx = sin(vxr);
	float sinvy = sin(vyr);
	float sinvz = sin(vzr);
	float sinvxyz = sinvx * sinvy * sinvz;

	for (int i = 0; i < powers; i++)
	{
		sinWarp *= sinvxyz;
	}

	float sinSev = sinWarp * severity;

	sinVertexPosition.x += radialVector.x * sinSev;
	sinVertexPosition.y += radialVector.y * sinSev;
	sinVertexPosition.z += radialVector.z * sinSev;
   
    //// Calculate cos wave surface modifier according to the parametric equations:
    // x1 = x0 + severity*x0*(cos(repeats*x0)*cos(repeats*y0)*cos(repeats*z0))^powers
    // y1 = y0 + severity*y0*(cos(repeats*x0)*cos(repeats*y0)*cos(repeats*z0))^powers
    // z1 = z0 + severity*z0*(cos(repeats*x0)*cos(repeats*y0)*cos(repeats*z0))^powers

	float cosWarp = 1.0f;
	float cosvx = cos(vxr);
	float cosvy = cos(vyr);
	float cosvz = cos(vzr);
	float cosvxyz = cosvx * cosvy * cosvz;
    
	for (int j = 0; j < powers; j++)
	{
		cosWarp *= cosvxyz;
	}

	float cosSev = cosWarp * severity;

	cosVertexPosition.x += radialVector.x * cosSev;
	cosVertexPosition.y += radialVector.y * cosSev;
	cosVertexPosition.z += radialVector.z * cosSev;

	// Lerp between cos function and sin function
	if (targetSin)
	{
		vertexPosition.x = lerp(cosVertexPosition.x, sinVertexPosition.x, lerpAmount);
		vertexPosition.y = lerp(cosVertexPosition.y, sinVertexPosition.y, lerpAmount);
		vertexPosition.z = lerp(cosVertexPosition.z, sinVertexPosition.z, lerpAmount);
	}
	else
	{
		vertexPosition.x = lerp(sinVertexPosition.x, cosVertexPosition.x, 1 - lerpAmount);
		vertexPosition.y = lerp(sinVertexPosition.y, cosVertexPosition.y, 1 - lerpAmount);
		vertexPosition.z = lerp(sinVertexPosition.z, cosVertexPosition.z, 1 - lerpAmount);
	}

    // Calculate the position of the new vertex (in 3D space)
    output.position = float4(vertexPosition, 1.0f);
  
    // Get tone of inverse base colour based on distance from centre of the sphere
    float tone = radius / (length(vertexPosition) * 2.0f);

    // Send the input color into the pixel shader.
    colourModifier.x = 1 - (baseColour.x + saturate(tone));
    colourModifier.y = 1 - (baseColour.y + saturate(tone));
    colourModifier.z = 1 - (baseColour.z + saturate(tone));
    colourModifier.w = 1.0f;
    
    colourModifier = saturate(colourModifier);

    output.colour = colourModifier;

    return output;
}

