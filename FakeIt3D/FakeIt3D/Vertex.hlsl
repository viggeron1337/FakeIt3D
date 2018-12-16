cbuffer CONSTANT_BUFFER : register(b0)
{
	float wvpMatrix; 
}

struct VS_INPUT
{
	float4 pos : POSITION; 
	float2 uv : UV; 
	float4 color : COLOR; 
	float4 normal : NORMAL; 
};

float4 main(VS_INPUT input) : SV_POSITION
{
	return mul(input.pos, wvpMatrix); 
}