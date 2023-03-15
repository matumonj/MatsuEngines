#include "Resources/Shader/Object3d.hlsli"

cbuffer Param : register(b0)
{
	float cbEdgeFactor;//三角形の辺の分割量の指定
	float cbInsideFactor;//三角形の内部の分割量の指定
};

#define NUM_CONTROL_POINTS 3
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VSOutput, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

	Output.EdgeTessFactor[0] = Output.EdgeTessFactor[1] = Output.EdgeTessFactor[2] = cbEdgeFactor;
	Output.InsideTessFactor = cbInsideFactor;

	return Output;
}

//ハルシェーダのエントリポイント定義
[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main(
	InputPatch<VSOutput, NUM_CONTROL_POINTS> inputPatch,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONTROL_POINT_OUTPUT Output;
	Output.svpos =inputPatch[i].svpos;
	Output.normal = inputPatch[i].normal;
	Output.uv = inputPatch[i].uv;
	Output.worldpos = inputPatch[i].worldpos;
	return Output;
}