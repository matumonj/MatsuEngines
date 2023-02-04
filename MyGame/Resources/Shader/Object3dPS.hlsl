#include "Object3d.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��

SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[


PSOutPut main(GSOutput input)
{
	// �e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp, input.uv);
	PSOutPut output;
	//����
	float3 light = normalize(float3(1, -1, 1));
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 1.0f;
	//�t�H�O����
	float4 nc = { 0.1, 0.1, 0.1, 1 }; //cameraPos�t�߂̐F
	float4 c = { 0.3, 0.3, 0.4, 1 }; //
	//cameraPos�Ƃ��邪���͌Œ���W
	float dist = length(cameraPos - input.worldpos.xyz);
	float4 addcol = float4(lerp(nc.rgb, c.rgb, dist / 500), 0.0);

	float3 fc = { 0.1, 0.1, 0.1 };

	// ����x
	const float shininess = 1.0f;
	// ���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	// �����ˌ�
	float3 ambient = m_ambient;

	// �V�F�[�f�B���O�ɂ��F
	float4 shadecolor = float4(ambientColor * ambient, m_alpha);
	// �ۉe
	if (!bloomf) {
		for (int cnum = 0; cnum < CIRCLESHADOW_NUM; cnum++)
		{
			if (circleShadows[cnum].active)
			{
				// �I�u�W�F�N�g�\�ʂ���L���X�^�[�ւ̃x�N�g��
				float3 casterv = circleShadows[cnum].casterPos - input.worldpos.xyz;
				// ���������ł̋���
				float d = dot(casterv, circleShadows[cnum].dir);

				// ���������W��
				float atten = saturate(
					1.0f / (circleShadows[cnum].atten.x + circleShadows[cnum].atten.y * d + circleShadows[cnum].atten.z * d
						* d));
				// �������}�C�i�X�Ȃ�0�ɂ���
				atten *= step(0, d);

				// ���C�g�̍��W
				float3 lightpos = circleShadows[cnum].casterPos + circleShadows[cnum].dir * circleShadows[cnum].
					distanceCasterLight;
				//  �I�u�W�F�N�g�\�ʂ��烉�C�g�ւ̃x�N�g���i�P�ʃx�N�g���j
				float3 lightv = normalize(lightpos - input.worldpos.xyz);
				// �p�x����
				float cos = dot(lightv, circleShadows[cnum].dir);
				// �����J�n�p�x����A�����I���p�x�ɂ����Č���
				// �����J�n�p�x�̓�����1�{ �����I���p�x�̊O����0�{�̋P�x
				float angleatten = smoothstep(circleShadows[cnum].factorAngleCos.y, circleShadows[cnum].factorAngleCos.x,
					cos);
				// �p�x��������Z
				atten *= angleatten;

				// �S�Č��Z����
				shadecolor.rgb -= atten;
			}
		}
	}

	// �_����
	for (int pnum = 0; pnum < 3; pnum++)
	{
		if (pointLights[pnum].active)
		{
			// ���C�g�ւ̕����x�N�g��
			float3 lightv = pointLights[pnum].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);

			// ���������W��
			float atten = 1.0f / (pointLights[pnum].lightatten.x + pointLights[pnum].lightatten.y * d + pointLights[
				pnum].
				lightatten.z * d * d);

			// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
			float3 dotlightnormal = dot(lightv, input.normal);
			// ���ˌ��x�N�g��
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			// �g�U���ˌ�
			float3 diffuse = dotlightnormal * m_diffuse;
			// ���ʔ��ˌ�
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			// �S�ĉ��Z����
			shadecolor.rgb += atten * (diffuse + specular) * pointLights[pnum].lightcolor;
		}
	}

	// �X�|�b�g���C�g
	for (int snum = 0; snum < 3; snum++)
	{
		if (spotLights[snum].active)
		{
			// ���C�g�ւ̕����x�N�g��
			float3 lightv = spotLights[snum].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);

			// ���������W��
			float atten = saturate(
				1.0f / (spotLights[snum].lightatten.x + spotLights[snum].lightatten.y * d + spotLights[snum].lightatten.
					z * d *
					d));

			// �p�x����
			float cos = dot(lightv, spotLights[snum].lightv);
			// �����J�n�p�x����A�����I���p�x�ɂ����Č���
			// �����J�n�p�x�̓�����1�{ �����I���p�x�̊O����0�{�̋P�x
			float angleatten = smoothstep(spotLights[snum].lightfactoranglecos.y,
				spotLights[snum].lightfactoranglecos.x,
				cos);
			// �p�x��������Z
			atten *= angleatten;

			// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
			float3 dotlightnormal = dot(lightv, input.normal);
			// ���ˌ��x�N�g��
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			// �g�U���ˌ�
			float3 diffuse = dotlightnormal * m_diffuse;
			// ���ʔ��ˌ�
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			// �S�ĉ��Z����
			shadecolor.rgb += atten * (diffuse + specular) * spotLights[snum].lightcolor;
		}
	}
	// ���s����
	if (shadowf) {
		if (dirLights.active)
		{
			// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
			float3 dotlightnormal = dot(dirLights.lightv, input.normal);
			// ���ˌ��x�N�g��
			float3 reflect = normalize(-dirLights.lightv + 2 * dotlightnormal * input.normal);
			// �g�U���ˌ�
			float3 diffuse = dotlightnormal * m_diffuse;
			// ���ʔ��ˌ�
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			// �S�ĉ��Z����
			shadecolor.rgb += (diffuse + specular) * dirLights.lightcolor;
		}
	}
	// float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	//�A�e�ƃe�N�X�`���̐F������
	//�ύX��

	output.target0 = float4(texcolor.rgb, texcolor.a) * color;
	output.target1 = float4(texcolor.rgb, texcolor.a) * color;

	if (flag)
	{
		if (shadowf)
		{
			output.target0 = shadecolor * float4(texcolor.rgb*fc, texcolor.a) + addcol;
			output.target1 = shadecolor * float4(texcolor.rgb*fc, texcolor.a) + addcol;
		}
	} else
	{
		output.target0 = shadecolor * float4(texcolor.rgb * brightness, texcolor.a) * color;
		output.target1 = shadecolor * float4(texcolor.rgb * brightness, texcolor.a) * color;
	}
	if (!shadowf)
	{
		output.target0 = shadecolor * float4(texcolor.rgb, texcolor.a) * color;
		output.target1 = shadecolor * float4(texcolor.rgb, texcolor.a) * color;
	}

	return output;
}
