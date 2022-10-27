#include"Grass.hlsli"

// �l�p�`�̒��_��
static const uint vnum = 7;
float rand(float2 seed) {
	return frac(sin(dot(seed.xy, float2(12.9808, 78.233))) * 43758.5453);
}
// �_�̓��͂���A�l�p�`���o��
[maxvertexcount(vnum)]
void main(
    
	triangle VSOutput input[3]:SV_POSITION,
	//point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< g2f > output
)
{
    //���_�̐F
    float4 _TopColor = { 0.0, 1.0, 0.0, 1.0 };
    //���{�̐F
    float4 _BottomColor = { 0.0, 0.0, 0.0, 1.0 };

    //�����̊�l
    float _Height = 10;
    //���̊�l
    float _Width = 1;
    //�����̔䗦@bottom, middle, high
    float4 _HeightRate = { 0.3, 0.4, 0.5, 0 };
    //���̔䗦@bottom, middle, high
    float4 _WidthRate = { 0.5, 0.4, 0.25, 0 };
    //���̗h�ꗦ@bottom, middle, high
    float4 _WindPowerRate = { 0.3, 1.0, 2.0, 0 };
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
    float height = (1 +1 +1) / 3.0f;
    float width = (1+ 1+ 1) / 3.0f;

    //���̌���
    float4 dir = float4(normalize(pos2 * rand(pos2) - pos0 * rand(pos1)).xyz * width, 1.0f);

    //�������}�b�s���O�p�̃e�N�X�`��
    //tilitn, offset����������uv���W�{uv�X�N���[��
    //xz���ʂ�uv���W�ɑΉ�
    float2 uv = pos0.xz *float2(0.5,0.5) + float2(0.5, 0.5) + _WindFrequency * 1;
    //��������RG��񁗃p�[�����m�C�Y
   // float2 windDir_xy = (tex2Dlod(_WindDistortionMap, float4(uv, 0, 0)).xy * 2 - 1) * _WindPower;
   // float4 wind = float4(windDir_xy, 0, 0);

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
      //  for (int j = 0; j < 3; j++) {
            o[i].pos =  mul(viewproj,mul(world, float4(o[i].pos.xyz,1.0f)));
           
           //output.svpos = mul(mul(viewproj, world), pos);

        //o[i].worldpos = mul(mul(viewproj, world),o[i].pos);;
      
        output.Append(o[i]);
    }
	[unroll]
	
	output.RestartStrip();
}
