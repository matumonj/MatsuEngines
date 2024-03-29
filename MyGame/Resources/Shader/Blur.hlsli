cbuffer cb : register(b0)
{
float4x4 mvp; // MVP行列
float4 mulColor; // 乗算カラー
};

struct VSInput
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

struct PSOutput
{
	float4 color : SV_Target0; // レンダリングターゲット0に描きこむ
	float depth : SV_Target1; // レンダリングターゲット1に描きこむ
};
