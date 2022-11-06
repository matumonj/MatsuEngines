#include"Texture.hlsli"
//#include"Header.hlsli"

//#include"header.hlsli"
Texture2D<float4>tex:register(t0);

SamplerState smp:register(s0);

float4 main(Output input) : SV_TARGET
{
	float3 light = normalize(float3(1,-1,1));
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 1.0f;
	float4 texcolor = float4(tex.Sample(smp, input.uv));

	float v = pow(input.worldpos.x - DisplayCenterpos.x, 2) + pow(input.worldpos.y - DisplayCenterpos.y, 2) + pow(input.worldpos.z -  DisplayCenterpos.z,2) <= pow(radius, 2) ? 1 : -1;

	clip(v);
	//•ÏXŒã
	return tex.Sample(smp, input.uv)* color;
	}

