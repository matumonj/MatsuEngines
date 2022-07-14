cbuffer cb : register(b0)
{
    float4x4 mvp;       // MVP�s��
    float4 mulColor;    // ��Z�J���[
};

struct VSInput
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
    // step-7 �J������Ԃł�Z�l���L�^����ϐ���ǉ�
    float3 depthInView : TEXCOORD2; // �J������Ԃł�Z�l

};

struct PSOutput
{
    float4 color : SV_Target0;  // �����_�����O�^�[�Q�b�g0�ɕ`������
    float depth : SV_Target1;   // �����_�����O�^�[�Q�b�g1�ɕ`������

};