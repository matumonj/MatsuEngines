#include"Texture.hlsli"
//#include"Header.hlsli"
Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1); // 0番スロットに設定されたテクスチャ

SamplerState smp:register(s0);

PSOutPut main(Output input)
{
	PSOutPut output;

	float3 light = normalize(float3(1, -1, 1));
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 1.0f;
	float4 texcolor = float4(tex.Sample(smp, input.uv));

	float v = pow(input.worldpos.x - DisplayCenterpos.x, 2) + pow(input.worldpos.y - DisplayCenterpos.y, 2) + pow(
		          input.worldpos.z - DisplayCenterpos.z, 2) <= pow(radius, 2)
		          ? 1
		          : -1;

	clip(v);
	//変更後
	output.target0 = tex.Sample(smp, input.uv) * color;
	output.target1 = tex.Sample(smp, input.uv) * color;
	return output;
}
