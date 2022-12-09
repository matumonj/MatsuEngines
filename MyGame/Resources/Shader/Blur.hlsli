cbuffer cb : register(b0)
{
float4x4 mvp; // MVP�s��
float4 mulColor; // ��Z�J���[
};

struct VSInput
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

struct PSOutput
{
	float4 color : SV_Target0; // �����_�����O�^�[�Q�b�g0�ɕ`������
	float depth : SV_Target1; // �����_�����O�^�[�Q�b�g1�ɕ`������
};
