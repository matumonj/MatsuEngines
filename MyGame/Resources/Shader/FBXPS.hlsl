#include "FBX.hlsli"

//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex : register(t0);
Texture2D<float4> stex : register(t1);
//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smp : register(s0);
//�G���g���[�|�C���g
PSOutPut main(VSOutput input) : SV_TARGET
{
	PSOutPut output;
	float4 texcolor1 = stex.Sample(smp, input.uv);

	PSInPut inp;
	//�e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp, input.uv);
	//Lambert����
	float3 light = normalize(float3(1, -1, 1)); //�E�����������C�g
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	//�t�H�O����
	float4 nc = {0.1, 0.1, 0.1, 1}; //cameraPos�t�߂̐F
	float4 c = {0.3, 0.3, 0.4, 1}; //
	//cameraPos�Ƃ��邪���͌Œ���W
	float dist = length(cameraPos - input.worldpos);
	float4 addcol = float4(lerp(nc.rgb, c.rgb, dist / 100), 0.0);
	float3 fc = {0.1, 0.1, 0.1};


	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	//�A�e�ƃe�N�X�`���̐F������
	//�ύX��
	output.target0 = texcolor * color + addcol;

	output.target1 = texcolor * color + addcol;

	return output;
}
