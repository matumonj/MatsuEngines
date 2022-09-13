#include "Object3d.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��

SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

PSOutPut main(GSOutput input) : SV_TARGET
{
	// �e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp, input.uv);
	PSOutPut output;
	//����
	float3 light = normalize(float3(1, -1, 1));
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 1.0f;
	//�t�H�O����
	float4 nc = { 0.1,0.1,0.1,1 };//cameraPos�t�߂̐F
	float4 c = { 0.4,0.4,0.4,1 };//
	//cameraPos�Ƃ��邪���͌Œ���W
	float dist = length(cameraPos - input.worldpos);
	float4 addcol = float4(lerp(nc.rgb, c.rgb, dist / 100),0.0);
	float3 fc = { 0.1,0.1,0.1 };

	if (flag) {
		output.target0 = float4(texcolor.rgb * fc, texcolor.a) + addcol;
		output.target1 = float4(0, 0, 0, 1);
	} else {
		output.target0 = float4(texcolor.rgb * brightness, texcolor.a);
		output.target1 = float4(texcolor.rgb * brightness, texcolor.a);
	}
	return output;
}