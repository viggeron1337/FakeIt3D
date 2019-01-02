
struct VS_OUTPUT
{
	float4 wvpPos : SV_POSITION; 
	float4 color : COLOR; 
};


float4 main(VS_OUTPUT input) : SV_TARGET
{
	return float4(color); 
}