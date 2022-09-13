#include "Object3d.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);  // 0番スロットに設定されたテクスチャ

SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

PSOutPut main(GSOutput input) : SV_TARGET
{
	// テクスチャマッピング
	float4 texcolor = tex.Sample(smp, input.uv);
	PSOutPut output;
	//環境光
	float3 light = normalize(float3(1, -1, 1));
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 1.0f;
	//フォグ部分
	float4 nc = { 0.1,0.1,0.1,1 };//cameraPos付近の色
	float4 c = { 0.4,0.4,0.4,1 };//
	//cameraPosとあるが今は固定座標
	float dist = length(cameraPos - input.worldpos);
	float4 addcol = float4(lerp(nc.rgb, c.rgb, dist / 100),0.0);
	float3 fc = { 0.1,0.1,0.1 };

	if (flag) {
		output.target0 = float4(texcolor.rgb * fc, texcolor.a) + addcol;
		output.target1 = float4(0, 0, 0, 1);
	} else {
		output.target0 = float4(texcolor.rgb * brightness, texcolor.a);
		output.target1 = float4(texcolor.rgb * brightness, texcolor.a);
	}
	return output;
}