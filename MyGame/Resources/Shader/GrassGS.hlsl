#include"Grass.hlsli"

// �l�p�`�̒��_��
static const uint vnum = 7;

float rand(float2 seed)
{
	return frac(sin(dot(seed.xy, float2(12.9808, 78.233))) * 43758.5453);
}

// �_�̓��͂���A�l�p�`���o��
[maxvertexcount(vnum)]
void main(

	triangle VSOutput input[3]:SV_POSITION,
	//point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<g2f> output
)
{
	// _WindDistortionMap("Wind Distortion Map", 2D) = "white" {}
	//���_�̐F
	float4 _TopColor = {0.0, 1.0, 0.0, 1.0};
	//���{�̐F
	float4 _BottomColor = {0.0, 0.0, 0.0, 1.0};

	//�����̊�l
	float _Height = 1;
	//���̊�l
	float _Width = 0.5f;
	//�����̔䗦@bottom, middle, high
	float4 _HeightRate = {0.3, 0.4, 0.5, 0};
	//���̔䗦@bottom, middle, high
	float4 _WidthRate = {0.5, 0.4, 0.25, 0};
	//���̗h�ꗦ@bottom, middle, high
	float4 _WindPowerRate = {0.9, 1.5, 2.5, 0};
	//���̋���
	float _WindPower = 2.0;
	//������������
	float _WindFrequency = 0.05;
	float4 pos0 = input[0].svpos;
	float4 pos1 = input[1].svpos;
	float4 pos2 = input[2].svpos;


	float3 nor0 = input[0].normal;
	float3 nor1 = input[1].normal;
	float3 nor2 = input[2].normal;

	//���͂��ꂽ�O�p���b�V���̒��_���W�̕��ϒl
	float4 centerPos = (pos0 + pos1 + pos2) / 3;
	//�@���x�N�g���̕��ϒl
	float4 centerNor = float4((nor0 + nor1 + nor2).xyz / 3, 1.0f);

	// VFX�p�̍����A���̒���
	float height = (1 + 1 + 1) / 3.0f;
	float width = (3) / 3.0f;

	float4 randpos2 = float4(rand(pos2.xy), rand(pos2.xy), rand(pos2.xy), rand(pos2.xy));
	float4 randpos1 = float4(rand(pos1.xy), rand(pos1.xy), rand(pos1.xy), rand(pos1.xy));
	//���̌���
	float4 dir = float4(normalize(pos2 * randpos2 - pos0 * randpos1) * float4(width, width, width, width));

	//���\�����ǂ�GrassObj�̕���uvtime�������Ē���

	float2 uv = pos0.xz * float2(0.5, 0.5) + float2(0.5, 0.5) + _WindFrequency * time;
	//float2 windDir_xy = (tex2Dlod(tex, float4(uv, 0, 0)).xy * 2 - 1) * _WindPower;
	float4 wind = float4(uv, 0, 0);

	g2f o[7];

	//bottom
	o[0].pos = centerPos - dir * _Width * _WidthRate.x;
	o[0].col = _BottomColor;

	o[1].pos = centerPos + dir * _Width * _WidthRate.x;
	o[1].col = _BottomColor;

	//bottom2middle
	o[2].pos = centerPos - dir * _Width * _WidthRate.y + centerNor * height * _Height * _HeightRate.x;
	o[2].col = lerp(_BottomColor, _TopColor, 0.33333f);

	o[3].pos = centerPos + dir * _Width * _WidthRate.y + centerNor * height * _Height * _HeightRate.x;
	o[3].col = lerp(_BottomColor, _TopColor, 0.33333f);

	//middley2high
	o[4].pos = o[3].pos - dir * _Width * _WidthRate.z + centerNor * height * _Height * _HeightRate.y;
	o[4].col = lerp(_BottomColor, _TopColor, 0.6666f);

	o[5].pos = o[3].pos + dir * _Width * _WidthRate.z + centerNor * height * _Height * _HeightRate.y;
	o[5].col = lerp(_BottomColor, _TopColor, 0.6666f);

	//top
	o[6].pos = o[5].pos + centerNor * height * _Height * _HeightRate.z;
	o[6].col = _TopColor;

	o[2].pos += wind * _WindPowerRate.x;
	o[3].pos += wind * _WindPowerRate.x;
	o[4].pos += wind * _WindPowerRate.y;
	o[5].pos += wind * _WindPowerRate.y;
	o[6].pos += wind * _WindPowerRate.z;
	[unroll]
	for (int i = 0; i < 7; i++)
	{
		o[i].pos = mul(viewproj, mul(world, float4(o[i].pos.xyz, 1.0f)));
		output.Append(o[i]);
	}
	output.RestartStrip();
}
