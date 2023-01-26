#include"Sprite.hlsli"
//Texture2D<float4> bokeTexture : register(t1);  // ボケ画像
//Texture2D<float4> depthTexture : register(t0); // 深度テクスチャ
Texture2D<float4> tex : register(t0); // ボケ画像
Texture2D<float4> tex1 : register(t1); // 深度テクスチャ

SamplerState smp:register(s0);

#define bloomR (10.f)

float3 BloomPixel(SamplerState smp, float2 uv, float2 texPixelSize)
{
	float2 uv2 = floor(uv / texPixelSize) * texPixelSize;
	uv2 += texPixelSize * 0.001f;
	float3 tl = max(tex.Sample(smp, uv2).rgb - bloomalpha, 0.f);
	float3 tr = max(tex.Sample(smp, uv2 + float2(texPixelSize.x, 0.f)).rgb - bloomalpha, 0.f);
	float3 bl = max(tex.Sample(smp, uv2 + float2(0.f, texPixelSize.y)).rgb - bloomalpha, 0.f);
	float3 br = max(tex.Sample(smp, uv2 + float2(texPixelSize.x, texPixelSize.y)).rgb - bloomalpha, 0.f);
	float2 f = frac(uv / texPixelSize);

	float3 tA = lerp(tl, tr, f.x);
	float3 tB = lerp(bl, br, f.x);

	return lerp(tA, tB, f.y);
}

float2 Vignette(float2 uv)
{
	float v = length(float2 (0.5, 0.5) - uv);
	v = clamp(v - 0.f, 0.f, 1.f);
	return float2( vignettecol.g*v, vignettecol.b*v);
}

float3 Fog()
{
	//float f=length()
}

float3 getBloom(SamplerState smp, float2 uv, float2 texPixelSize)
{
	float3 bloom = float3(0.f, 0.f, 0.f);
	float2 offset = float2(1.f, 1.f) * texPixelSize * bloomR;
	bloom += BloomPixel(smp, uv + offset * float2(-1.f, -1.f), texPixelSize * bloomR) * 0.292893f;
	bloom += BloomPixel(smp, uv + offset * float2(-1.f, 0.f), texPixelSize * bloomR) * 0.5f;
	bloom += BloomPixel(smp, uv + offset * float2(-1.f, 1.f), texPixelSize * bloomR) * 0.292893f;
	bloom += BloomPixel(smp, uv + offset * float2(0.f, -1.f), texPixelSize * bloomR) * 0.5f;
	bloom += BloomPixel(smp, uv + offset * float2(0.f, 0.f), texPixelSize * bloomR) * 1.f;
	bloom += BloomPixel(smp, uv + offset * float2(0.f, 1.f), texPixelSize * bloomR) * 0.5f;
	bloom += BloomPixel(smp, uv + offset * float2(1.f, -1.f), texPixelSize * bloomR) * 0.292893f;
	bloom += BloomPixel(smp, uv + offset * float2(1.f, 0.f), texPixelSize * bloomR) * 0.5f;
	bloom += BloomPixel(smp, uv + offset * float2(1.f, 1.f), texPixelSize * bloomR) * 0.292893f;
	bloom /= 4.171573f;
	return bloom;
}

float4 bloom(SamplerState smp, float2 uv, float intensity = 1.f)
{
	return float4(getBloom(smp, uv, 1.f / 1920) * intensity, 1.f);
}
float4 main(Output input) : SV_TARGET
{
	
	float4 coltex0 = tex.Sample(smp, input.uv);
	float4 coltex1 = tex1.Sample(smp, input.uv);

	float4 col = coltex0;

	float4 feed = {0.5, 0.5, 0.5, 1};
	//	col = feed;
	
	// ブルーム
	if (bloomf) {
		col.rgb += bloom(smp, input.uv);
	}
	col.gb -= Vignette(input.uv);

	return float4(col.rgb, 1);

	// ボケ画像を出力
	//return boke;
}
