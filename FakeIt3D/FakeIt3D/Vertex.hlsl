cbuffer CONSTANT_BUFFER : register(b0)
{
	float wvpMatrix; 
}

struct VS_INPUT
{
	float4 pos : POSITION; 
	float4 color : COLOR; 
};

struct PS_INPUT
{
	float4 pos : SV_POSITION; 
	float4 color : COLOR; 
};


PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output; 
	
	input.pos.w = 1.0f;

	//Store needed values

	//Transfer to wvp space 
	output.pos = mul(input.pos, wvpMatrix); 
	output.color = input.color; 

	return output; 
}