#include"PostEffectTest.hlsli"
//Texture2D<float4> bokeTexture : register(t1);  // �{�P�摜
//Texture2D<float4> depthTexture : register(t0); // �[�x�e�N�X�`��
Texture2D<float4> tex : register(t0);  // �{�P�摜
Texture2D<float4> tex1 : register(t1); // �[�x�e�N�X�`��

SamplerState smp:register(s0);
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

float4 col = coltex0;
float4 feed = { 0.5,0.5,0.5,1 };
	col = coltex1*feed;

return float4(col.rgb, 1);
// �{�P�摜���o��
//return boke;
}