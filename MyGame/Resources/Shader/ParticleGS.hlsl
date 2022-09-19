#include"Particle.hlsli"


//�l�p�`�̒��_��
static const uint vnum = 4;

//�Z���^�[����̃I�t�Z�b�g
static const float4 offset_array[vnum] =
{
	float4(-0.5f,-0.5f,0,0),
	float4(-0.5f,+0.5f,0,0),
	float4(+0.5f,-0.5f,0,0),
	float4(+0.5f,+0.5f,0,0)
};

//���オ�O�C�O�@�E����1�C1
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
	//�_�X�g���[��
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	//��ڂ̎O�p�`
	for (uint i = 0; i < vnum; i++)
	{
		//���S����̃I�t�Z�b�g���r���{�[�h��]
		//float4 offset = mul(matBillboard, offset_array[i]);
		float4 offset = offset_array[i] * input[0].scale;
		offset = mul(matBillboard, offset);

		//�I�t�Z�b�g�����炷
		element.svpos = input[0].pos + offset;
		//���[���h���W�x�[�X�ł��炷
		//element.svpos = input[0].pos + offset_array[i];

		//�r���[�E�ˉe�ϊ�
		element.svpos = mul(mat, element.svpos);
		//element.uv=float2(0.5f,0.5f);
		element.uv = uv_array[i];
		element.color = input[0].colors;
		output.Append(element);//���_����o��
	}
	
}

