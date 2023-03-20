cbuffer cbuff0:register(b0)
{
float bloomalpha;
float2 centerpos;
float uzulen;
float uzurad;
};

struct Output
{
	float4 svpos:SV_POSITION;
	float2 uv:TEXCOORD;
};
