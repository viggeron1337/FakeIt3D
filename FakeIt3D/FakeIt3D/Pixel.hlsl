
struct VS_OUTPUT
{
	float4 wvpPos : Sv_POSITION; 
	float2 uv : UV; 
	float4 color : COLOR; 
	float4 normal : NORMAL; 
};


float4 main(VS_OUTPUT input) : SV_TARGET
{
	return float4(color); 
}