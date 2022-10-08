#include"MiniMap.hlsli"
Texture2D<float4> tex : register(t0);  // ボケ画像
Texture2D<float4> tex1 : register(t1); // 深度テクスチャ

SamplerState smp:register(s0);
float4 main(Output input) : SV_TARGET
{
float4 coltex0 = tex.Sample(smp, input.uv);

float4 col = coltex0;

float radius = 200;

float v = pow(input.svpos.x -centerpos.x, 2) + pow(input.svpos.y - centerpos.y, 2) <= pow(radius, 2) ? 1 : -1;

clip(v);

return float4(col.rgb, 1);
}