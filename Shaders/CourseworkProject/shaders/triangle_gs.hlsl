cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix lightView;
    matrix lightProjection;
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
    float3 viewDirection : TEXCOORD1;
    float3 normal : NORMAL;
    float3 position3D : TEXCOORD2;
    float3 lightPos : TEXCOORD3;
    float4 lightViewPosition : TEXCOORD4;
};

// pixel input type
struct OutputType
{
    float4 position : SV_POSITION;
    float4 colour : COLOR;
    float3 viewDirection : TEXCOORD1;
    float3 normal : NORMAL;
    float3 position3D : TEXCOORD2;
    float3 lightPos : TEXCOORD3;
    float4 lightViewPosition : TEXCOORD4;
};

// gs function
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
    output.viewDirection = input[0].viewDirection;
    output.normal = input[0].normal;
    output.position3D = input[0].position3D;
    output.lightPos = input[0].lightPos;
    output.lightViewPosition = input[0].lightViewPosition;
	//output.tex = input[0].tex;

   // output.normal = mul(input[0].normal, (float3x3)worldMatrix);
    //output.normal = normalize(output.normal);

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
    output.viewDirection = input[1].viewDirection;
    output.normal = input[1].normal;
    output.position3D = input[1].position3D;
    output.lightPos = input[1].lightPos;
    output.lightViewPosition = input[1].lightViewPosition;

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
    output.viewDirection = input[2].viewDirection;
    output.normal = input[2].normal;
    output.position3D = input[2].position3D;
    output.lightPos = input[2].lightPos;
    output.lightViewPosition = input[2].lightViewPosition;

    triStream.Append(output);

	triStream.RestartStrip();


}