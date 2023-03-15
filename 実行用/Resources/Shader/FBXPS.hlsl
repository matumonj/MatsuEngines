#include "FBX.hlsli"

//0番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);
Texture2D<float4> stex : register(t1);
//0番スロットに設定されたサンプラー
SamplerState smp : register(s0);
//エントリーポイント


PSOutPut main(VSOutput input)
{
	PSOutPut output;
	float4 texcolor1 = stex.Sample(smp, input.uv);

	PSInPut inp;
	//テクスチャマッピング
	float4 texcolor = tex.Sample(smp, input.uv);
	//Lambert反射
	float3 light = normalize(float3(1, -1, 1)); //右下奥向きライト
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse;
	//フォグ部分
	float4 nc = { 0.1, 0.1, 0.1, 1 }; //cameraPos付近の色
	float4 c = { 0.3, 0.3, 0.4, 1 }; //
	//cameraPosとあるが今は固定座標
	float dist = length(cameraPos - input.worldpos.xyz);
	float4 addcol = float4(lerp(nc.rgb, c.rgb, dist / 100), 0.0);
	float3 fc = { 0.1, 0.1, 0.1 };


	//変更後


	// 光沢度
	const float shininess = 1.0f;
	// 頂点から視点への方向ベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	// 環境反射光
	float3 ambient =float3(1,1,1) ;

	// シェーディングによる色
	float4 shadecolor = float4(ambientColor * ambient, 1.f);

	// 平行光源

	for (int i = 0; i < DIRLIGHT_NUM; i++)
	{
		if (dirLights[i].active)
		{
			// ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
			// 反射光ベクトル
			float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			// 拡散反射光
			float3 diffuse = dotlightnormal * float3(0.f,0.f,0.f);
			// 鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * float3(0,0,0);

			// 全て加算する
			shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
		}
	}

	// 点光源
	for (int nump = 0; nump < POINTLIGHT_NUM; nump++)
	{
		if (pointLights[nump].active)
		{
			// ライトへの方向ベクトル
			float3 lightv = pointLights[nump].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);

			// 距離減衰係数
			float atten = 1.0f / (pointLights[nump].lightatten.x + pointLights[nump].lightatten.y * d + pointLights[
				nump].lightatten.z * d * d);

			// ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(lightv, input.normal);
			// 反射光ベクトル
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			// 拡散反射光
			float3 diffuse = dotlightnormal *float3(0,0,0);
			// 鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * float3(0,0,0);

			// 全て加算する
			shadecolor.rgb += atten * (diffuse + specular) * pointLights[nump].lightcolor;
		}
	}
	//if (shadowf) {
	// スポットライト
	for (int nums = 0; nums < SPOTLIGHT_NUM; nums++)
	{
		if (spotLights[nums].active)
		{
			// ライトへの方向ベクトル
			float3 lightv = spotLights[nums].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);

			// 距離減衰係数
			float atten = saturate(
				1.0f / (spotLights[nums].lightatten.x + spotLights[nums].lightatten.y * d + spotLights[nums].lightatten.
					z * d * d));

			// 角度減衰
			float cos = dot(lightv, spotLights[nums].lightv);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
			float angleatten = smoothstep(spotLights[nums].lightfactoranglecos.y,
				spotLights[nums].lightfactoranglecos.x, cos);
			// 角度減衰を乗算
			atten *= angleatten;

			// ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(lightv, input.normal);
			// 反射光ベクトル
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			// 拡散反射光
			float3 diffuse = dotlightnormal * float3(0,0,0);
			// 鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * float3(0,0,0);

			// 全て加算する
			shadecolor.rgb += atten * (diffuse + specular) * spotLights[nums].lightcolor;
		}
	}
	//	}
	// 丸影
	for (int numc = 0; numc < CIRCLESHADOW_NUM; numc++)
	{
		if (circleShadows[numc].active)
		{
			// オブジェクト表面からキャスターへのベクトル
			float3 casterv = circleShadows[numc].casterPos - input.worldpos.xyz;
			// 光線方向での距離
			float d = dot(casterv, circleShadows[numc].dir);

			// 距離減衰係数
			float atten = saturate(
				1.0f / (circleShadows[numc].atten.x + circleShadows[numc].atten.y * d + circleShadows[numc].atten.z * d
					* d));
			// 距離がマイナスなら0にする
			atten *= step(0, d);

			// ライトの座標
			float3 lightpos = circleShadows[numc].casterPos + circleShadows[numc].dir * circleShadows[numc].
				distanceCasterLight;
			//  オブジェクト表面からライトへのベクトル（単位ベクトル）
			float3 lightv = normalize(lightpos - input.worldpos.xyz);
			// 角度減衰
			float cos = dot(lightv, circleShadows[numc].dir);
			// 減衰開始角度から、減衰終了角度にかけて減衰
			// 減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
			float angleatten = smoothstep(circleShadows[numc].factorAngleCos.y, circleShadows[numc].factorAngleCos.x,
				cos);
			// 角度減衰を乗算
			atten *= angleatten;

			// 全て減算する
			shadecolor.rgb -= atten;
		}
	}
	output.target0 = shadecolor * float4(texcolor.rgb, texcolor.a)*color;
	output.target1 = shadecolor * float4(texcolor.rgb, texcolor.a)*color;

	return output;
}
