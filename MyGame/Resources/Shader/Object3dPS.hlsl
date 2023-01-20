#include "Object3d.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1); // 0番スロットに設定されたテクスチャ

SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー


#define bloomR (10.f)

// これより大きい値の色がグローする
#define bloomThreshold (0.5f)

float3 getBloomPixel(SamplerState smp, float2 uv, float2 texPixelSize)
{
	float2 uv2 = floor(uv / texPixelSize) * texPixelSize;
	uv2 += texPixelSize * 0.001f;
	float3 tl = max(tex.Sample(smp, uv2).rgb - bloomThreshold, 0.f);
	float3 tr = max(tex.Sample(smp, uv2 + float2(texPixelSize.x, 0.f)).rgb - bloomThreshold, 0.f);
	float3 bl = max(tex.Sample(smp, uv2 + float2(0.f, texPixelSize.y)).rgb - bloomThreshold, 0.f);
	float3 br = max(tex.Sample(smp, uv2 + float2(texPixelSize.x, texPixelSize.y)).rgb - bloomThreshold, 0.f);
	float2 f = frac(uv / texPixelSize);

	float3 tA = lerp(tl, tr, f.x);
	float3 tB = lerp(bl, br, f.x);

	return lerp(tA, tB, f.y);
}

float3 getBloom(SamplerState smp, float2 uv, float2 texPixelSize)
{
	float3 bloom = float3(0.f, 0.f, 0.f);
	float2 off = float2(1.f, 1.f) * texPixelSize * bloomR;
	bloom += getBloomPixel(smp, uv + off * float2(-1.f, -1.f), texPixelSize * bloomR) * 0.292893f;
	bloom += getBloomPixel(smp, uv + off * float2(-1.f, 0.f), texPixelSize * bloomR) * 0.5f;
	bloom += getBloomPixel(smp, uv + off * float2(-1.f, 1.f), texPixelSize * bloomR) * 0.292893f;
	bloom += getBloomPixel(smp, uv + off * float2(0.f, -1.f), texPixelSize * bloomR) * 0.5f;
	bloom += getBloomPixel(smp, uv + off * float2(0.f, 0.f), texPixelSize * bloomR) * 1.f;
	bloom += getBloomPixel(smp, uv + off * float2(0.f, 1.f), texPixelSize * bloomR) * 0.5f;
	bloom += getBloomPixel(smp, uv + off * float2(1.f, -1.f), texPixelSize * bloomR) * 0.292893f;
	bloom += getBloomPixel(smp, uv + off * float2(1.f, 0.f), texPixelSize * bloomR) * 0.5f;
	bloom += getBloomPixel(smp, uv + off * float2(1.f, 1.f), texPixelSize * bloomR) * 0.292893f;
	bloom /= 4.171573f;
	return bloom;
}

float4 bloom(SamplerState smp, float2 uv, float intensity = 1.f)
{
	return float4(getBloom(smp, uv, 1.f / 1920) * intensity, 1.f);
}
PSOutPut main(GSOutput input)
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
	float4 addcol = float4(lerp(nc.rgb, c.rgb, dist / 500), 0.0);
	
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

	for (int cnum = 0; cnum < CIRCLESHADOW_NUM; cnum++)
	{
		if (circleShadows[cnum].active)
		{
			// オブジェクト表面からキャスターへのベクトル
			float3 casterv = circleShadows[cnum].casterPos - input.worldpos.xyz;
			// 光線方向での距離
			float d = dot(casterv, circleShadows[cnum].dir);

			// 距離減衰係数
			float atten = saturate(
				1.0f / (circleShadows[cnum].atten.x + circleShadows[cnum].atten.y * d + circleShadows[cnum].atten.z * d
					* d));
			// 距離がマイナスなら0にする
			atten *= step(0, d);

			// ライトの座標
			float3 lightpos = circleShadows[cnum].casterPos + circleShadows[cnum].dir * circleShadows[cnum].
				distanceCasterLight;
			//  オブジェクト表面からライトへのベクトル（単位ベクトル）
			float3 lightv = normalize(lightpos - input.worldpos.xyz);
			// 角度減衰
			float cos = dot(lightv, circleShadows[cnum].dir);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
			float angleatten = smoothstep(circleShadows[cnum].factorAngleCos.y, circleShadows[cnum].factorAngleCos.x,
			                              cos);
			// 角度減衰を乗算
			atten *= angleatten;

			// 全て減算する
			shadecolor.rgb -= atten;
		}
	}
	// 平行光源
	if (shadowf){
	for (int dnum = 0; dnum < 3; dnum++)
	{
		if (dirLights[dnum].active)
		{
			// ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(dirLights[dnum].lightv, input.normal);
			// 反射光ベクトル
			float3 reflect = normalize(-dirLights[dnum].lightv + 2 * dotlightnormal * input.normal);
			// 拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;
			// 鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			// 全て加算する
			shadecolor.rgb += (diffuse + specular) * dirLights[dnum].lightcolor;
		}
	}
		}

	// 点光源
	for (int pnum = 0; pnum < 3; pnum++)
	{
		if (pointLights[pnum].active)
		{
			// ライトへの方向ベクトル
			float3 lightv = pointLights[pnum].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);

			// 距離減衰係数
			float atten = 1.0f / (pointLights[pnum].lightatten.x + pointLights[pnum].lightatten.y * d + pointLights[
					pnum].
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
			shadecolor.rgb += atten * (diffuse + specular) * pointLights[pnum].lightcolor;
		}
	}

	// スポットライト
	for (int snum = 0; snum < 3; snum++)
	{
		if (spotLights[snum].active)
		{
			// ライトへの方向ベクトル
			float3 lightv = spotLights[snum].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);

			// 距離減衰係数
			float atten = saturate(
				1.0f / (spotLights[snum].lightatten.x + spotLights[snum].lightatten.y * d + spotLights[snum].lightatten.
					z * d *
					d));

			// 角度減衰
			float cos = dot(lightv, spotLights[snum].lightv);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
			float angleatten = smoothstep(spotLights[snum].lightfactoranglecos.y,
			                              spotLights[snum].lightfactoranglecos.x,
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
			shadecolor.rgb += atten * (diffuse + specular) * spotLights[snum].lightcolor;
		}
	}

	// 丸影
	for (int cnum2 = 0; cnum2 < 30; cnum2++)
	{
		if (circleShadows[cnum2].active)
		{
			// オブジェクト表面からキャスターへのベクトル
			float3 casterv = circleShadows[cnum2].casterPos - input.worldpos.xyz;
			// 光線方向での距離
			float d = dot(casterv, circleShadows[cnum2].dir);

			// 距離減衰係数
			float atten = saturate(
				1.0f / (circleShadows[cnum2].atten.x + circleShadows[cnum2].atten.y * d + circleShadows[cnum2].atten.z *
					d * d));
			// 距離がマイナスなら0にする
			atten *= step(0, d);

			// ライトの座標
			float3 lightpos = circleShadows[cnum2].casterPos + circleShadows[cnum2].dir * circleShadows[cnum2].
				distanceCasterLight;
			//  オブジェクト表面からライトへのベクトル（単位ベクトル）
			float3 lightv = normalize(lightpos - input.worldpos.xyz);
			// 角度減衰
			float cos = dot(lightv, circleShadows[cnum2].dir);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
			float angleatten = smoothstep(circleShadows[cnum2].factorAngleCos.y, circleShadows[cnum2].factorAngleCos.x,
			                              cos);
			// 角度減衰を乗算
			atten *= angleatten;

			// 全て減算する
			shadecolor.rgb -= atten;
		}
	}

	// float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	//陰影とテクスチャの色を合成
	//変更後

	output.target0 = float4(texcolor.rgb, texcolor.a) * color;
	output.target1 = float4(texcolor.rgb, texcolor.a) * color;

	if (flag)
	{
		if (shadowf)
		{
			output.target0 = shadecolor * float4(texcolor.rgb , texcolor.a) ;
			output.target1 = shadecolor * float4(texcolor.rgb, texcolor.a) ;
		}
	}
	else
	{
		output.target0 = shadecolor * float4(texcolor.rgb * brightness, texcolor.a) * color;
		output.target1 = shadecolor * float4(texcolor.rgb * brightness, texcolor.a) * color;
	}
	if (!shadowf)
	{
		output.target0 = float4(texcolor.rgb, texcolor.a) * color;
		output.target1 = float4(texcolor.rgb, texcolor.a) * color;
	}

	if(bloomf)
	{
		output.target0=float4(0,0,0,0);
		output.target1 = output.target0;
	}
	return output;
}
