#include"MiniMap.hlsli"
Output main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	Output output;

	float window_x = 1600;
	float window_y = 1080;

	float addx=0;
	float addy=0;

	addx=smoothstep(0,window_x,centerpos.x);
	addy=smoothstep(0,window_y, centerpos.y);

	output.svpos = float4(pos.x+addx+0.25f, pos.y+addy, pos.z, pos.w);
	
	output.uv = uv;
	return output;
}