#include"Grass.hlsli"
[maxvertexcount(7)]
void main(
    triangle VSOutput input[3] : SV_POSITION,
    inout TriangleStream<g2f > output
)
{
    //頂点の色
    float4 _TopColor = {1.0, 1.0, 1.0, 1.0};
        //根本の色
float4 _BottomColor = {0.0, 0.0, 0.0, 1.0};

        //高さの基準値
        float _Height = 10;
        //幅の基準値
        float _Width = 1;
        //高さの比率@bottom, middle, high
        float4 _HeightRate = {0.3, 0.4, 0.5, 0};
        //幅の比率@bottom, middle, high
        float4 _WidthRate = {0.5, 0.4, 0.25, 0};
        //風の揺れ率@bottom, middle, high
        float4 _WindPowerRate = {0.3, 1.0, 2.0, 0};
        //風の強さ
        float _WindPower = 2.0;
        //風が吹く周期
        float _WindFrequency = 0.05;
	

        float4 pos0 = input[0].svpos;
        float4 pos1 = input[1].svpos;
        float4 pos2 = input[2].svpos;


        float3 nor0 = input[0].normal;
        float3 nor1 = input[1].normal;
        float3 nor2 = input[2].normal;

        //入力された三角メッシュの頂点座標の平均値
        float4 centerPos = (pos0 + pos1 + pos2) / 3;
        //法線ベクトルの平均値
        float4 centerNor = float4((nor0 + nor1 + nor2).xyz / 3, 1.0f);

        // VFX用の高さ、幅の調整
        float height = (0.4 + 0.4 +0.4) / 3.0f;
        float width = (0.2 + 0.2 +0.2) / 3.0f;

        //草の向き
        float4 dir = float4(normalize(pos2 * rand(pos2) - pos0 * rand(pos1)).xyz * width, 1.0f);


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

        [unroll]
        for (int i = 0; i < 7; i++) {
            o[i].uv = input[i].uv;
            o[i].normal = input[i].normal;
            o[i].worldpos = input[i].worldpos;
           output.Append(o[i]);
        }
}