#include "Object3d.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1); // 0番スロットに設定されたテクスチャ

SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

PSOutPut main(GSOutput input) : SV_TARGET
{
	// テクスチャマッピング
	float4 texcolor = tex.Sample(smp, input.uv);
	PSOutPut output;
	//環境光
	float3 light = normalize(float3(1, -1, 1));
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 1.0f;
	//フォグ部分
	float4 nc = {0.1, 0.1, 0.1, 1}; //cameraPos付近の色
	float4 c = {0.3, 0.3, 0.4, 1}; //
	//cameraPosとあるが今は固定座標
	float dist = length(cameraPos - input.worldpos.xyz);
	float4 addcol = float4(lerp(nc.rgb, c.rgb, dist / 100), 0.0);
	float3 fc = {0.1, 0.1, 0.1};

	// 光沢度
	const float shininess = 4.0f;
	// 頂点から視点への方向ベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	// 環境反射光
	float3 ambient = m_ambient;

	// シェーディングによる色
	float4 shadecolor = float4(ambientColor * ambient, m_alpha);
	// 丸影
	for (int i = 0; i < CIRCLESHADOW_NUM; i++)
	{
		if (circleShadows[i].active)
		{
			// オブジェクト表面からキャスターへのベクトル
			float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
			// 光線方向での距離
			float d = dot(casterv, circleShadows[i].dir);

			// 距離減衰係数
			float atten = saturate(
				1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
			// 距離がマイナスなら0にする
			atten *= step(0, d);

			// ライトの座標
			float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			//  オブジェクト表面からライトへのベクトル（単位ベクトル）
			float3 lightv = normalize(lightpos - input.worldpos.xyz);
			// 角度減衰
			float cos = dot(lightv, circleShadows[i].dir);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
			float angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
			// 角度減衰を乗算
			atten *= angleatten;

			// 全て減算する
			shadecolor.rgb -= atten;
		}
	}
	// 平行光源
	for (int i = 0; i < int(DIRLIGHT_NUM); i++)
	{
		if (dirLights[i].active)
		{
			// ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
			// 反射光ベクトル
			float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			// 拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;
			// 鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			// 全て加算する
			shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
		}
	}

	// 点光源
	for (i = 0; i < POINTLIGHT_NUM; i++)
	{
		if (pointLights[i].active)
		{
			// ライトへの方向ベクトル
			float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);

			// 距離減衰係数
			float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].
				lightatten.z * d * d);

			// ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(lightv, input.normal);
			// 反射光ベクトル
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			// 拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;
			// 鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			// 全て加算する
			shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;
		}
	}

	// スポットライト
	for (i = 0; i < SPOTLIGHT_NUM; i++)
	{
		if (spotLights[i].active)
		{
			// ライトへの方向ベクトル
			float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);

			// 距離減衰係数
			float atten = saturate(
				1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d *
					d));

			// 角度減衰
			float cos = dot(lightv, spotLights[i].lightv);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
			float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x,
			                              cos);
			// 角度減衰を乗算
			atten *= angleatten;

			// ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(lightv, input.normal);
			// 反射光ベクトル
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			// 拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;
			// 鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			// 全て加算する
			shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
		}
	}

	// 丸影
	for (int i = 0; i < int(CIRCLESHADOW_NUM); i++)
	{
		if (circleShadows[i].active)
		{
			// オブジェクト表面からキャスターへのベクトル
			float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
			// 光線方向での距離
			float d = dot(casterv, circleShadows[i].dir);

			// 距離減衰係数
			float atten = saturate(
				1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
			// 距離がマイナスなら0にする
			atten *= step(0, d);

			// ライトの座標
			float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			//  オブジェクト表面からライトへのベクトル（単位ベクトル）
			float3 lightv = normalize(lightpos - input.worldpos.xyz);
			// 角度減衰
			float cos = dot(lightv, circleShadows[i].dir);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
			float angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
			// 角度減衰を乗算
			atten *= angleatten;

			// 全て減算する
			shadecolor.rgb -= atten;
		}
	}

	// float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	//陰影とテクスチャの色を合成
	//変更後

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
		output.target0 = shadecolor / 2 * float4(texcolor.rgb * fc, texcolor.a) + addcol;
		output.target1 = shadecolor / 2 * float4(texcolor.rgb * fc, texcolor.a) + addcol;
	}
	return output;
}
