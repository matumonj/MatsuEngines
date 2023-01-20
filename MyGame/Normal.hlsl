#include"Normal.hlsli"
//Texture2D<float4> bokeTexture : register(t1);  // ボケ画像
//Texture2D<float4> depthTexture : register(t0); // 深度テクスチャ
Texture2D<float4> tex : register(t0); // ボケ画像
Texture2D<float4> tex1 : register(t1); // 深度テクスチャ

SamplerState smp:register(s0);

#define bloomR (10.f)

// これより大きい値の色がグローする
#define bloomThreshold (0.7f)

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
float4 main(Output input) : SV_TARGET
{
	// step-12 ボケ画像書き込み用のピクセルシェーダーを実装
	// カメラ空間での深度値をサンプリング
	// float depth = bokeTexture.Sample(Sampler, input.uv);

	// カメラ空間での深度値が200以下ならピクセルキル
	//      -> ボケ画像を描きこまない
	//clip(depth+(0));

	// ボケ画像をサンプリング
	//float4 boke =depthTexture.Sample(Sampler, input.uv);
	//boke.a =  min(1.0f, (depth + 1.0f) / 2.0f);

	// 深度値から不透明度を計算する
	// 深度値800からボケが始まり、深度値2000で最大のボケ具合になる
	//  -> つまり、深度値2000で不透明度が1になる
	float4 coltex0 = tex.Sample(smp, input.uv);
	float4 coltex1 = tex1.Sample(smp, input.uv);

	float4 col = coltex1;

	float4 feed = {0.5, 0.5, 0.5, 1};
	//	col = feed;

	// ブルーム
	col.rgb += bloom(smp, input.uv);

	return float4(col.rgb, 1);

	// ボケ画像を出力
	//return boke;
}
