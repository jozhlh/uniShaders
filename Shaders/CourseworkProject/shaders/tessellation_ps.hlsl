// Tessellation pixel shader
// Output colour passed to stage.
Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture : register(t1);

SamplerState SampleTypeWrap : register(s0);
SamplerState SampleTypeClamp : register(s1);

struct InputType
{
    float4 position : SV_POSITION;
    float4 colour : COLOR;
};

float4 main(InputType input) : SV_TARGET
{
    // The colour is calculated in the domain shader stage, based on values found there

    return input.colour;;
}