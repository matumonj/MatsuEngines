#include "Resources/Shader/Object3d.hlsli"
// DSTriangle.hlsl
uint2 random(uint stream, uint sequence) {
	//実装は省略
}
float4 calColor(float3 domain) {
	//実装は省略
}

#define NUM_CONTROL_POINTS 3
//エントリポイントの定義
[domain("tri")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	VSOutput voutput;
	DS_OUTPUT output;
	//output.normal=
	output.svpos = float4(patch[0].svpos.xyz * domain.x + patch[1].svpos.xyz * domain.y + patch[2].svpos.xyz * domain.z, 1);
	for(uint i=0;i<patch.Length;i++)
	{
		output.svpos = patch[i].svpos;
		output.normal = patch[i].normal;
		output.uv = patch[i].uv;
		output.worldpos = patch[i].worldpos;
		
	}
	//output.
	//output.color = calColor(domain);
	return output;
}