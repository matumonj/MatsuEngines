#include"Texture.hlsli"




Output main(float4 pos:POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	Output output;
	//output.svpos = pos;
	output.normal = normal;
	//���W�ɍs�����Z���ďo��
	output.svpos = mul(mat, pos);
	if (uvf) {
		uv.y += time;
	}
	output.uv = uv;
	return output;
}