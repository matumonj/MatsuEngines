cbuffer cbuff0:register(b0)
{
float4 color;
matrix mat;
matrix world;
matrix matBillboard; // �r���{�[�h�s��
bool uvf;
float3 sub;
float time;
float3 DisplayCenterpos;
float radius;
};

struct Output
{
	float4 svpos:SV_POSITION;
	float4 worldpos : POSITION; // ���[���h���W
	float3 normal:NORMAL;
	float2 uv:TEXCOORD;
};

struct PSOutPut
{
	float4 target0:SV_TARGET0;
	float4 target1:SV_TARGET1;
};
