Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float2 texCoord1 : TEXCOORD1;
    float2 texCoord2 : TEXCOORD2;
    float2 texCoord3 : TEXCOORD3;
    float2 texCoord4 : TEXCOORD4;
    float2 texCoord5 : TEXCOORD5;
    float3 blurWeights : TEXCOORD6;
};

float4 main(InputType input) : SV_TARGET
{
    float4 colour;


    // Initialize the colour to black.
    colour = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // Add the five vertical pixels to the colour by the specific weight of each.
    colour += shaderTexture.Sample(SampleType, input.texCoord1) * input.blurWeights.z;
    colour += shaderTexture.Sample(SampleType, input.texCoord2) * input.blurWeights.y;
    colour += shaderTexture.Sample(SampleType, input.texCoord3) * input.blurWeights.x;
    colour += shaderTexture.Sample(SampleType, input.texCoord4) * input.blurWeights.y;
    colour += shaderTexture.Sample(SampleType, input.texCoord5) * input.blurWeights.z;
 
    // Set the alpha channel to one.
    colour.a = 1.0f;

    return colour;
}

