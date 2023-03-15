#include"Sprite.hlsli"
//input.uvture2D<float4> bokeinput.uvture : register(t1);  // ボケ画像
//input.uvture2D<float4> depthinput.uvture : register(t0); // 深度テクスチャ
Texture2D<float4> tex : register(t0); // ボケ画像
Texture2D<float4> tex1 : register(t1); // 深度テクスチャ

SamplerState smp:register(s0);

#define bloomR (10.f)

float3 BloomPixel(SamplerState smp, float2 uv, float2 texPixelSize)
{
	
	float2 uv2 = floor(uv / texPixelSize) * texPixelSize;
	uv2 += texPixelSize * 0.001f;
	float3 tl = max(tex.Sample(smp, uv2).rgb - 0.95f, 0.f);
	float3 tr = max(tex.Sample(smp, uv2 + float2(texPixelSize.x, 0.f)).rgb - 0.95f, 0.f);
	float3 bl = max(tex.Sample(smp, uv2 + float2(0.f, texPixelSize.y)).rgb - 0.95f, 0.f);
	float3 br = max(tex.Sample(smp, uv2 + float2(texPixelSize.x, texPixelSize.y)).rgb - 0.95f, 0.f);
	float2 f = frac(uv / texPixelSize);

	float3 tA = lerp(tl, tr, f.x);
	float3 tB = lerp(bl, br, f.x);

	return lerp(tA, tB, f.y);
}

float3 Vignette(float2 uv)
{
	float v = length(float2(0.5, 0.5) - uv);
	v = clamp(v - 0.f, 0.f, 1.f);
	return float3(1.f,vignettecol.g * v, vignettecol.b * v);
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
	
	//col.rgb += bloom(smp, input.uv).rgb

	float4 colors[10];
	float2 ViewportOffset = (float2(0.5, 0.5) / float2(1900,1000));
	float2 center = float2(smoothstep(0,1900, centerpos.x), smoothstep( 0, 1000, centerpos.y));
	float2 dir = center - input.uv;
	float len = length(dir);
	float2 offset = normalize(dir) * ViewportOffset;
	offset *= (bloomalpha * len);

	colors[0] = tex.Sample(smp, input.uv) * 0.19f;
	colors[1] = tex.Sample(smp, input.uv + offset) * 0.17f;
	colors[2] = tex.Sample(smp, input.uv + offset * 2.0f) * 0.15f;
	colors[3] = tex.Sample(smp, input.uv + offset * 3.0f) * 0.13f;
	colors[4] = tex.Sample(smp, input.uv + offset * 4.0f) * 0.11f;
	colors[5] = tex.Sample(smp, input.uv + offset * 5.0f) * 0.09f;
	colors[6] = tex.Sample(smp, input.uv + offset * 6.0f) * 0.07f;
	colors[7] = tex.Sample(smp, input.uv + offset * 7.0f) * 0.05f;
	colors[8] = tex.Sample(smp, input.uv + offset * 8.0f) * 0.03f;
	colors[9] = tex.Sample(smp, input.uv + offset * 9.0f) * 0.01f;
	//return float4(col.rgb, 1);
	float4 Color = float4(0.0f, 0.0f, 0.0f, 0.0f);
	Color = (colors[0] + colors[1] + colors[2] + colors[3] + colors[4]
		+ colors[5] + colors[6] + colors[7] + colors[8] + colors[9]);
	Color.gb -= Vignette(input.uv).gb;
	
	return float4(Color.xyz,1);
	
	// ボケ画像を出力
	//return tex.Sample(smp, input.uv);
}
