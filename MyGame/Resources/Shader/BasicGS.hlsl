#include"Object3d.hlsli"
// 四角形の頂点数
static const uint vnum = 3;

float rand(float2 seed)
{
	return frac(sin(dot(seed.xy, float2(12.9808, 78.233))) * 43758.5453);
}


float CalculationVectorLength(float2 vec01)
{
	return sqrt((vec01.x * vec01.x) + (vec01.y * vec01.y));
}
float2 ConvertToNomalizeVector(float2 outs, float2 ins)
{
	float distance = sqrt((ins.x * ins.x) + (ins.y * ins.y));
	if (distance > 0.0f)
	{
		outs.x = ins.x / distance;
		outs.y = ins.y / distance;
	} else
	{
		outs = float2(0.0f, 0.0f);
	}
	return outs;
}

bool IsCollidinglinesAndCircle(Line2D lines, Point circle)
{
	//return true;

	// ベクトルの作成
	float2 start_to_center = float2(circle.x - lines.start.x, circle.y - lines.start.y);
	float2 end_to_center = float2(circle.x - lines.end.x, circle.y - lines.end.y);
	float2 start_to_end = float2(lines.end.x - lines.start.x, lines.end.y - lines.start.y);
	float2 normal_start_to_end;

	normal_start_to_end = ConvertToNomalizeVector(normal_start_to_end, start_to_end);

	//射影した線分の長さ
	float distance_projection = start_to_center.x * normal_start_to_end.y - normal_start_to_end.x * start_to_center.y;

	// 射影の長さが半径よりも小さい
	if (abs(distance_projection) < dislen)
	{
		// 始点 => 終点と始点 => 円の中心の内積を計算する
		float dot01 = start_to_center.x * start_to_end.x + start_to_center.y * start_to_end.y;
		// 始点 => 終点と終点 => 円の中心の内積を計算する
		float dot02 = end_to_center.x * start_to_end.x + end_to_center.y * start_to_end.y;

		// 二つの内積の掛け算結果が0以下なら当たり
		if (dot01 * dot02 <= 0.0f)
		{
			return true;
		}
		/*
			上の条件から漏れた場合、円は線分上にはないので、
			始点 => 円の中心の長さか、終点 => 円の中心の長さが
			円の半径よりも短かったら当たり
		*/
		if (CalculationVectorLength(start_to_center) < dislen ||
			CalculationVectorLength(end_to_center) < dislen)
		{
			return true;
		}
	}

	return false;
}
// 点の入力から、四角形を出力
[maxvertexcount(vnum)]
void main(
	triangle VSOutput input[3]:SV_POSITION,
	//point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
	float3 center = { (input[0].svpos.xyz + input[1].svpos.xyz + input[2].svpos.xyz) / 3 };
	float4 posworld = mul(viewproj, mul(world, float4(center, 1.0f)));
	float3 dist = length(cameraPos - posworld.xyz);
	float destruction = clamp(float3(dislen, dislen, dislen) - dist, 0.f, 1.f).x;
	float4 offset = float4(0, 0, 0, 0);

	float3 vec1 = input[1].svpos.xyz - input[0].svpos.xyz;
	float3 vec2 = input[2].svpos.xyz - input[0].svpos.xyz;
	float3 gnormal = normalize(cross(vec1, vec2));
	float random = rand(center.xy);
	float randms = random.x;

	Line2D lines;
	Point points;


	[unroll]
	for (uint i = 0; i < vnum; i++)
	{
		GSOutput element;
		element.worldpos = input[i].worldpos;
		element.svpos = input[i].svpos;

		if (gsflag)
		{

			lines.end = float2(cameraPos.x, cameraPos.z);
			lines.start = float2(playerpos.x, playerpos.z);

			points.x = element.svpos.x;
			points.y = element.svpos.z;

			if (IsCollidinglinesAndCircle(lines, points)) {
				element.svpos.xyz = center + (element.svpos.xyz - center) * (1 - destruction * 1.0);
			}
		}
		if (destF)
		{
			input[i].svpos.xy += gnormal.xy * (desttime / 2);
			element.svpos = input[i].svpos;
		} else if (!destF && !gsflag)
		{
			element.svpos = input[i].svpos;
		}

		element.normal = input[i].normal;
		element.uv = input[i].uv;

		output.Append(element);
	}
	output.RestartStrip();
}
