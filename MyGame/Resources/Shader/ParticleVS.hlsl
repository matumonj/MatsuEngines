#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION, float scale : TEXCOORD)//,float4 colors:COLOR)/*, float3 normal : NORMAL, float2 uv : TEXCOORD*/
{
	VSOutput output; // ピクセルシェーダーに渡す値
	output.pos = pos;
	output.scale = scale;
	//output.colors = colors;
	//output.svpos = mul(mat, pos);
	//output.normal = normal;
	//
	//output.color = color;
	return output;
}