#include "FBX.hlsli"

//0番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);
Texture2D<float4> stex : register(t1);
//0番スロットに設定されたサンプラー
SamplerState smp : register(s0);
//エントリーポイント
PSOutPut main(VSOutput input) : SV_TARGET
{
	PSOutPut output;
	float4 texcolor1 = stex.Sample(smp, input.uv);

	PSInPut inp;
	//テクスチャマッピング
	float4 texcolor = tex.Sample(smp, input.uv);
	//Lambert反射
	float3 light = normalize(float3(1, -1, 1)); //右下奥向きライト
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	//フォグ部分
	float4 nc = {0.1, 0.1, 0.1, 1}; //cameraPos付近の色
	float4 c = {0.3, 0.3, 0.4, 1}; //
	//cameraPosとあるが今は固定座標
	float dist = length(cameraPos - input.worldpos);
	float4 addcol = float4(lerp(nc.rgb, c.rgb, dist / 100), 0.0);
	float3 fc = {0.1, 0.1, 0.1};


	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	//陰影とテクスチャの色を合成
	//変更後
	output.target0 = texcolor * color + addcol;

	output.target1 = texcolor * color + addcol;

	return output;
}
