

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

	//For correct calculations
	input.position.w = 1; 
	
	output.position = input.position; 
	output.color = input.color; 

	return output; 
}