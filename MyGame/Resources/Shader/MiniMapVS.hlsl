#include"MiniMap.hlsli"
Output main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	Output output;
	output.svpos = float4(pos.x+1.5, pos.y, pos.z, pos.w );
	output.uv = uv;
	return output;
}