
struct VS_Input
{
    float2 pos : POS;
    float4 color : COL;
    float2 uv : TEX;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : COL;
    float2 uv : TEXCOORD;
};

Texture2D mytexture;
SamplerState mysampler;

//Texture2D mytexture : register(t0);
//SamplerState mysampler : register(s0);

VS_Output vs_main(VS_Input input)
{
    VS_Output output;
    output.position = float4(input.pos, 0.0f, 1.0f);
    output.color = input.color;    
    output.uv = input.uv;

    return output;
}

float4 ps_main(VS_Output input) : SV_TARGET
{
    return mytexture.Sample(mysampler, input.uv);// *input.color;
}
