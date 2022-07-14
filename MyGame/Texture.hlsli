cbuffer cbuff0:register(b0)
{
	float4 color;
	matrix mat;
	matrix matBillboard; // ビルボード行列
};

struct Output
{
	float4 svpos:SV_POSITION;
	float3 normal:NORMAL;
	float2 uv:TEXCOORD;
};
