cbuffer cbuff0:register(b0)
{
float4 color;
matrix mat;
bool uvscroll;
float3 sub;
float uv_time;
};

struct Output
{
	float4 svpos:SV_POSITION;
	float2 uv:TEXCOORD;
};
