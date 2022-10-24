#include"Object3d.hlsli"

// 四角形の頂点数
static const uint vnum = 3;
float rand(float2 seed) {
	return frac(sin(dot(seed.xy, float2(12.9808, 78.233))) * 43758.5453);
}
// 点の入力から、四角形を出力
[maxvertexcount(vnum)]
void main(
	triangle VSOutput input[3]:SV_POSITION,
	//point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	float3 center = { (input[0].svpos.xyz + input[1].svpos.xyz + input[2].svpos.xyz) / 3 };
	float3 posworld = mul(world, float3(center));
	float3 dist = length(float3(0,0,0) - posworld);
	float destruction = clamp(50 - dist, 0, 1);
	float4 offset = float4(0, 0, 0, 0);

	float3 vec1 = input[1].svpos.xyz - input[0].svpos.xyz;
	float3 vec2 = input[2].svpos.xyz - input[0].svpos.xyz;
	float3 gnormal = normalize(cross(vec1, vec2));
	float random = rand(center.xy);
	float randms = random.xxx;
	[unroll]
	for (uint i = 0; i < vnum; i++) {

		GSOutput element;
		element.worldpos = input[i].worldpos;
		element.svpos = input[i].svpos;

		if (gsflag) {
			element.svpos.xyz = center + (element.svpos.xyz - center) * (1 - destruction * 0.9);//mul(viewproj, element.svpos);
		}
		if (destF) {
			element.svpos.xyz+=input[i].normal* rand(center.xy) * ((0.5f +desttime)* 0.9);
		}
		else if(!destF&&!gsflag) {
			element.svpos = input[i].svpos;
		}
		element.normal = input[i].normal;
		element.uv = input[i].uv;

		output.Append(element);
	}
	output.RestartStrip();
}
