#include"Particle.hlsli"


//四角形の頂点数
static const uint vnum = 4;

//センターからのオフセット
static const float4 offset_array[vnum] =
{
	float4(-0.5f,-0.5f,0,0),
	float4(-0.5f,+0.5f,0,0),
	float4(+0.5f,-0.5f,0,0),
	float4(+0.5f,+0.5f,0,0)
};

//左上が０，０　右下が1，1
static const float2 uv_array[vnum] =
{
	float2(0,1),
	float2(0,0),
	float2(1,1),
	float2(1,0)
};


[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	//点ストリーム
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	//一つ目の三角形
	for (uint i = 0; i < vnum; i++)
	{
		//中心からのオフセットをビルボード回転
		//float4 offset = mul(matBillboard, offset_array[i]);
		float4 offset = offset_array[i] * input[0].scale;
		offset = mul(matBillboard, offset);

		//オフセット分ずらす
		element.svpos = input[0].pos + offset;
		//ワールド座標ベースでずらす
		//element.svpos = input[0].pos + offset_array[i];

		//ビュー・射影変換
		element.svpos = mul(mat, element.svpos);
		//element.uv=float2(0.5f,0.5f);
		element.uv = uv_array[i];
		element.color = input[0].colors;
		output.Append(element);//頂点を一つ出力
	}
	
}

