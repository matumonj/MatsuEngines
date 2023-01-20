#include "FBX.hlsli"

//0番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);
Texture2D<float4> stex : register(t1);
//0番スロットに設定されたサンプラー
SamplerState smp : register(s0);
//エントリーポイント


#define bloomR (10.f)

// これより大きい値の色がグローする
#define bloomThreshold (0.5f)

float3 getBloomPixel(SamplerState smp, float2 uv, float2 texPixelSize)
{
	float2 uv2 = floor(uv / texPixelSize) * texPixelSize;
	uv2 += texPixelSize * 0.001f;
	float3 tl = max(tex.Sample(smp, uv2).rgb - bloomThreshold, 0.f);
	float3 tr = max(tex.Sample(smp, uv2 + float2(texPixelSize.x, 0.f)).rgb - bloomThreshold, 0.f);
	float3 bl = max(tex.Sample(smp, uv2 + float2(0.f, texPixelSize.y)).rgb - bloomThreshold, 0.f);
	float3 br = max(tex.Sample(smp, uv2 + float2(texPixelSize.x, texPixelSize.y)).rgb - bloomThreshold, 0.f);
	float2 f = frac(uv / texPixelSize);

	float3 tA = lerp(tl, tr, f.x);
	float3 tB = lerp(bl, br, f.x);

	return lerp(tA, tB, f.y);
}

float3 getBloom(SamplerState smp, float2 uv, float2 texPixelSize)
{
	float3 bloom = float3(0.f, 0.f, 0.f);
	float2 off = float2(1.f, 1.f) * texPixelSize * bloomR;
	bloom += getBloomPixel(smp, uv + off * float2(-1.f, -1.f), texPixelSize * bloomR) * 0.292893f;
	bloom += getBloomPixel(smp, uv + off * float2(-1.f, 0.f), texPixelSize * bloomR) * 0.5f;
	bloom += getBloomPixel(smp, uv + off * float2(-1.f, 1.f), texPixelSize * bloomR) * 0.292893f;
	bloom += getBloomPixel(smp, uv + off * float2(0.f, -1.f), texPixelSize * bloomR) * 0.5f;
	bloom += getBloomPixel(smp, uv + off * float2(0.f, 0.f), texPixelSize * bloomR) * 1.f;
	bloom += getBloomPixel(smp, uv + off * float2(0.f, 1.f), texPixelSize * bloomR) * 0.5f;
	bloom += getBloomPixel(smp, uv + off * float2(1.f, -1.f), texPixelSize * bloomR) * 0.292893f;
	bloom += getBloomPixel(smp, uv + off * float2(1.f, 0.f), texPixelSize * bloomR) * 0.5f;
	bloom += getBloomPixel(smp, uv + off * float2(1.f, 1.f), texPixelSize * bloomR) * 0.292893f;
	bloom /= 4.171573f;
	return bloom;
}

float4 bloom(SamplerState smp, float2 uv, float intensity = 1.f)
{
	return float4(getBloom(smp, uv, 1.f / 1920) * intensity, 1.f);
}
PSOutPut main(VSOutput input)
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
	float dist = length(cameraPos - input.worldpos.xyz);
	float4 addcol = float4(lerp(nc.rgb, c.rgb, dist / 100), 0.0);
	float3 fc = {0.1, 0.1, 0.1};


	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	//陰影とテクスチャの色を合成
	//変更後
	output.target0 = texcolor * color + addcol;

	output.target1 = texcolor * color + addcol;
	
	return output;
}
