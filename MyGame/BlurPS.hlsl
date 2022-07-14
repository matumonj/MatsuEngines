#include"Blur.hlsli"
Texture2D<float4> bokeTexture : register(t0);  // �{�P�摜
Texture2D<float4> depthTexture : register(t1); // �[�x�e�N�X�`��

//Texture2D<float4> sceneTexture : register(t0); // �V�[���e�N�X�`��
sampler Sampler : register(s0);

float4 main(PSInput In) : SV_Target0
{
    // step-12 �{�P�摜�������ݗp�̃s�N�Z���V�F�[�_�[������
     // �J������Ԃł̐[�x�l���T���v�����O
     float depth = depthTexture.Sample(Sampler, In.uv);

// �J������Ԃł̐[�x�l��200�ȉ��Ȃ�s�N�Z���L��
//      -> �{�P�摜��`�����܂Ȃ�
clip(depth - 200.0f);

// �{�P�摜���T���v�����O
float4 boke = bokeTexture.Sample(Sampler, In.uv);

// �[�x�l����s�����x���v�Z����
// �[�x�l800����{�P���n�܂�A�[�x�l2000�ōő�̃{�P��ɂȂ�
//  -> �܂�A�[�x�l2000�ŕs�����x��1�ɂȂ�
boke.a = min(1.0f, (depth - 800.0f) / 2000.0f);

// �{�P�摜���o��
return boke;
}