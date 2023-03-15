#include "Resources/Shader/Object3d.hlsli"

cbuffer Param : register(b0)
{
	float cbEdgeFactor;//�O�p�`�̕ӂ̕����ʂ̎w��
	float cbInsideFactor;//�O�p�`�̓����̕����ʂ̎w��
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

//�n���V�F�[�_�̃G���g���|�C���g��`
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