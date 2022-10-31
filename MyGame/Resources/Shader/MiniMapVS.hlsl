#include"MiniMap.hlsli"
Output main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	Output output;

	const float window_x = centerpos.x;
	const float window_y = 1080.0f;

	float addx=0.0f;
	float addy=0.0f;

	const float correction_Value = 0.25f;

	addx=smoothstep(0.0f,window_x,centerpos.x);
	addy=smoothstep(0.0f,window_y, centerpos.y);

	output.svpos =  float4(pos.x + addx + correction_Value, pos.y + addy, pos.z, pos.w);
	
	output.uv = uv;
	return output;
}