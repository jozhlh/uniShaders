// Tessellation vertex shader.
// Doesn't do much, could manipulate the control points
// Pass forward data, strip out some values not required for example.

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct InputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float3 position : POSITION;
    float4 colour : COLOR;
};

OutputType main(InputType input)
{
    OutputType output;

	// Calculate the position of the vertex in the world.
	float3 worldPosition = mul(input.position, (float3x3)worldMatrix);

	 // Pass the vertex position into the hull shader.
    output.position = input.position;
    
    // Pass the input color into the hull shader.
    output.colour = float4(1.0, 1.0, 1.0, 1.0);
    
    return output;
}
