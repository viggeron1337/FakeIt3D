
cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	matrix wvpMatrix; 
}

cbuffer CONSTANT_BUFFER : register(b1)
{
	matrix transMatrix; 
}

struct Input
{
	float4 position : POSITION; 
	float4 color : COLOR; 
};

struct Output
{
	//When the POSITION tag is seen by the video card, it knows this is the final position,
	//It will know how to project this to the screen  and clip it inside of the screen boundaires. 
	float4 position : SV_POSITION; 
	float4 color : COLOR; 
};

Output main(Input input)
{
	Output output; 

	matrix test; 
	//For correct calculations
	input.position.w = 1; 
	
	test = mul(transMatrix, wvpMatrix); 

	output.position = mul(input.position, test); 
	//output.position = mul(input.position,wvpMatrix); 
	output.color = input.color; 

	return output; 
}