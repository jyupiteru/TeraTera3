
#include	"../Utils/CommonPSVS.fx"

float4 main( VS_OUTPUT input ) : SV_Target
{
	float4 col = input.Color;
	col.x /= 256.0f;
	col.y /= 256.0f;
	col.z /= 256.0f;


	return col;
}