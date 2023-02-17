#include"Object3d.hlsli"
// �l�p�`�̒��_��
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

	// �x�N�g���̍쐬
	float2 start_to_center = float2(circle.x - lines.start.x, circle.y - lines.start.y);
	float2 end_to_center = float2(circle.x - lines.end.x, circle.y - lines.end.y);
	float2 start_to_end = float2(lines.end.x - lines.start.x, lines.end.y - lines.start.y);
	float2 normal_start_to_end;

	normal_start_to_end = ConvertToNomalizeVector(normal_start_to_end, start_to_end);

	//�ˉe���������̒���
	float distance_projection = start_to_center.x * normal_start_to_end.y - normal_start_to_end.x * start_to_center.y;

	// �ˉe�̒��������a����������
	if (abs(distance_projection) < dislen)
	{
		// �n�_ => �I�_�Ǝn�_ => �~�̒��S�̓��ς��v�Z����
		float dot01 = start_to_center.x * start_to_end.x + start_to_center.y * start_to_end.y;
		// �n�_ => �I�_�ƏI�_ => �~�̒��S�̓��ς��v�Z����
		float dot02 = end_to_center.x * start_to_end.x + end_to_center.y * start_to_end.y;

		// ��̓��ς̊|���Z���ʂ�0�ȉ��Ȃ瓖����
		if (dot01 * dot02 <= 0.0f)
		{
			return true;
		}
		/*
			��̏�������R�ꂽ�ꍇ�A�~�͐�����ɂ͂Ȃ��̂ŁA
			�n�_ => �~�̒��S�̒������A�I�_ => �~�̒��S�̒�����
			�~�̔��a�����Z�������瓖����
		*/
		if (CalculationVectorLength(start_to_center) < dislen ||
			CalculationVectorLength(end_to_center) < dislen)
		{
			return true;
		}
	}

	return false;
}
// �_�̓��͂���A�l�p�`���o��
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
