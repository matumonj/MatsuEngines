#include"Grass.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);  // 0番スロットに設定されたテクスチャ

SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

PSOutPut main(g2f input) : SV_TARGET
{
	//float4 texcolor = tex.Sample(smp, input.uv);
	PSOutPut output;

	
		output.target0 = input.col;

		output.target1 = input.col;
	
	return output;

}