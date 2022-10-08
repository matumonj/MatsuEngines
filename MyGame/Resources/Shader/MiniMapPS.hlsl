#include"MiniMap.hlsli"
Texture2D<float4> tex : register(t0);  // ボケ画像
Texture2D<float4> tex1 : register(t1); // 深度テクスチャ

SamplerState smp:register(s0);
float4 main(Output input) : SV_TARGET
{
float4 coltex0 = tex.Sample(smp, input.uv);

float4 col = coltex0;
float f = input.svpos.x <600 ? -1 : 1;
float f2 = input.svpos.y > 400 ? -1 : 1;

clip(f);
clip(f2);

return float4(col.rgb, 1);
}