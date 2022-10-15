#include"Sprite.hlsli"
Output main( float4 pos : POSITION,float2 uv:TEXCOORD )
{
	Output output;
	output.svpos = mul(mat, pos);
	if (uvscroll) {
		uv.x += uv_time;
	}
	output.uv = uv;
	return output;
}