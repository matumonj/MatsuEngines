#include"Normal.hlsli"
//Texture2D<float4> bokeTexture : register(t1);  // �{�P�摜
//Texture2D<float4> depthTexture : register(t0); // �[�x�e�N�X�`��
Texture2D<float4> tex : register(t0); // �{�P�摜
Texture2D<float4> tex1 : register(t1); // �[�x�e�N�X�`��

SamplerState smp:register(s0);

#define bloomR (10.f)

// ������傫���l�̐F���O���[����
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
	// step-12 �{�P�摜�������ݗp�̃s�N�Z���V�F�[�_�[������
	// �J������Ԃł̐[�x�l���T���v�����O
	// float depth = bokeTexture.Sample(Sampler, input.uv);

	// �J������Ԃł̐[�x�l��200�ȉ��Ȃ�s�N�Z���L��
	//      -> �{�P�摜��`�����܂Ȃ�
	//clip(depth+(0));

	// �{�P�摜���T���v�����O
	//float4 boke =depthTexture.Sample(Sampler, input.uv);
	//boke.a =  min(1.0f, (depth + 1.0f) / 2.0f);

	// �[�x�l����s�����x���v�Z����
	// �[�x�l800����{�P���n�܂�A�[�x�l2000�ōő�̃{�P��ɂȂ�
	//  -> �܂�A�[�x�l2000�ŕs�����x��1�ɂȂ�
	float4 coltex0 = tex.Sample(smp, input.uv);
	float4 coltex1 = tex1.Sample(smp, input.uv);

	float4 col = coltex1;

	float4 feed = {0.5, 0.5, 0.5, 1};
	//	col = feed;

	// �u���[��
	col.rgb += bloom(smp, input.uv);

	return float4(col.rgb, 1);

	// �{�P�摜���o��
	//return boke;
}
