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
	float4 nc = {0.1, 0.1, 0.1, 1}; //cameraPos�t�߂̐F
	float4 c = {0.3, 0.3, 0.4, 1}; //
	//cameraPos�Ƃ��邪���͌Œ���W
	float dist = length(cameraPos - input.worldpos.xyz);
	float4 addcol = float4(lerp(nc.rgb, c.rgb, dist / 500), 0.0);

	float3 fc = {0.1, 0.1, 0.1};

	// ����x
	const float shininess = 1.0f;
	// ���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	// �����ˌ�
	float3 ambient = m_ambient;

	// �V�F�[�f�B���O�ɂ��F
	float4 shadecolor = float4(ambientColor * ambient, m_alpha);

	// ���s����
	if (shadowf)
	{
		for (int i = 0; i < DIRLIGHT_NUM; i++)
		{
			if (dirLights[i].active)
			{
				// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
				float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
				// ���ˌ��x�N�g��
				float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
				// �g�U���ˌ�
				float3 diffuse = dotlightnormal * m_diffuse;
				// ���ʔ��ˌ�
				float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

				// �S�ĉ��Z����
				shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
			}
		}
	}
	// �_����
	for (int nump = 0; nump < POINTLIGHT_NUM; nump++)
	{
		if (pointLights[nump].active)
		{
			// ���C�g�ւ̕����x�N�g��
			float3 lightv = pointLights[nump].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);

			// ���������W��
			float atten = 1.0f / (pointLights[nump].lightatten.x + pointLights[nump].lightatten.y * d + pointLights[
				nump].lightatten.z * d * d);

			// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
			float3 dotlightnormal = dot(lightv, input.normal);
			// ���ˌ��x�N�g��
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			// �g�U���ˌ�
			float3 diffuse = dotlightnormal * m_diffuse;
			// ���ʔ��ˌ�
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			// �S�ĉ��Z����
			shadecolor.rgb += atten * (diffuse + specular) * pointLights[nump].lightcolor;
		}
	}
	//if (shadowf) {
	// �X�|�b�g���C�g
	for (int nums = 0; nums < SPOTLIGHT_NUM; nums++)
	{
		if (spotLights[nums].active)
		{
			// ���C�g�ւ̕����x�N�g��
			float3 lightv = spotLights[nums].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);

			// ���������W��
			float atten = saturate(
				1.0f / (spotLights[nums].lightatten.x + spotLights[nums].lightatten.y * d + spotLights[nums].lightatten.
					z * d * d));

			// �p�x����
			float cos = dot(lightv, spotLights[nums].lightv);
			// �����J�n�p�x����A�����I���p�x�ɂ����Č���
			// �����J�n�p�x�̓�����1�{ �����I���p�x�̊O����0�{�̋P�x
			float angleatten = smoothstep(spotLights[nums].lightfactoranglecos.y,
			                              spotLights[nums].lightfactoranglecos.x, cos);
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
			shadecolor.rgb += atten * (diffuse + specular) * spotLights[nums].lightcolor;
		}
	}
	//	}
	// �ۉe
	for (int numc = 0; numc < CIRCLESHADOW_NUM; numc++)
	{
		if (circleShadows[numc].active)
		{
			// �I�u�W�F�N�g�\�ʂ���L���X�^�[�ւ̃x�N�g��
			float3 casterv = circleShadows[numc].casterPos - input.worldpos.xyz;
			// ���������ł̋���
			float d = dot(casterv, circleShadows[numc].dir);

			// ���������W��
			float atten = saturate(
				1.0f / (circleShadows[numc].atten.x + circleShadows[numc].atten.y * d + circleShadows[numc].atten.z * d
					* d));
			// �������}�C�i�X�Ȃ�0�ɂ���
			atten *= step(0, d);

			// ���C�g�̍��W
			float3 lightpos = circleShadows[numc].casterPos + circleShadows[numc].dir * circleShadows[numc].
				distanceCasterLight;
			//  �I�u�W�F�N�g�\�ʂ��烉�C�g�ւ̃x�N�g���i�P�ʃx�N�g���j
			float3 lightv = normalize(lightpos - input.worldpos.xyz);
			// �p�x����
			float cos = dot(lightv, circleShadows[numc].dir);
			// �����J�n�p�x����A�����I���p�x�ɂ����Č���
			// �����J�n�p�x�̓�����1�{ �����I���p�x�̊O����0�{�̋P�x
			float angleatten = smoothstep(circleShadows[numc].factorAngleCos.y, circleShadows[numc].factorAngleCos.x,
			                              cos);
			// �p�x��������Z
			atten *= angleatten;

			// �S�Č��Z����
			shadecolor.rgb -= atten;
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
			output.target0 = shadecolor * float4(texcolor.rgb * fc, texcolor.a) + addcol;
			output.target1 = shadecolor * float4(texcolor.rgb * fc, texcolor.a) + addcol;
		}
	}
	else
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
