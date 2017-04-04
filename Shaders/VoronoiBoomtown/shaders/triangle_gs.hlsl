cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer GeometryBuffer : register(b1)
{
    float explode;
    float3 padding;
}

struct InputType
{
    float4 position : SV_POSITION;
    float4 colour : COLOR;
};

// pixel input type
struct OutputType
{
    float4 position : SV_POSITION;
    float4 colour : COLOR;
};

// gs function, receives triangles which it displaces before outputting again
[maxvertexcount(3)]
void main(triangle InputType input[3], inout TriangleStream<OutputType> triStream)
{
	OutputType output;
    float4 direction = normalize(input[0].position);
		
	// Change the position vector to be 4 units for proper matrix calculations.
    input[0].position.w = 1.0f;

	// Move the vertex away from the point position
	output.position = input[0].position + (direction * explode);
    output.position.w = 1.0f;

    //output.position = input[0].position;
	output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);


    output.colour = input[0].colour;

	triStream.Append(output);

    // Change the position vector to be 4 units for proper matrix calculations.
    input[1].position.w = 1.0f;

	// Move the vertex away from the point position
    output.position = input[1].position + (direction * explode);
    output.position.w = 1.0f;

    //output.position = input[1].position;
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.colour = input[1].colour;

    triStream.Append(output);

     // Change the position vector to be 4 units for proper matrix calculations.
    input[2].position.w = 1.0f;

	// Move the vertex away from the point position
    output.position = input[2].position + (direction * explode);
    output.position.w = 1.0f;

    //output.position = input[2].position;
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.colour = input[2].colour;

    triStream.Append(output);

	triStream.RestartStrip();


}