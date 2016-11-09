// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry


Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};


float4 main(InputType input) : SV_TARGET
{
    float4 textureColor;
	
	float4 baseColour = float4(0.0f, 1.0f, 1.0f, 1.0f);

	float4 scale = float4(0.5f, 0.5f, 0.5f, 1.0f);

	float4 inverse = float4(1.0f, 1.0f, 1.0f, 0.0f);
	
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = texture0.Sample(Sampler0, input.tex);
	
	// result = x + s(y-x) where lerp(x, y, s)
	//textureColor = lerp(baseColour, textureColor, scale);
	//textureColor = inverse - textureColor;

    return textureColor;
}