struct DS_OUTPUT
{
	float4 vPosition  : SV_POSITION;
};

struct HS_CONTROL_POINT_OUTPUT
{
	float3 vPosition : WORLDPOS; 
};

struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor;
	float InsideTessFactor			: SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 3

[domain("tri")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	DS_OUTPUT Output;

	Output.vPosition = float4(
		patch[0].vPosition*domain.x+patch[1].vPosition*domain.y+patch[2].vPosition*domain.z,1);

	return Output;
}
//
#include "Resources/Shader/Object3d.hlsli"

#define INPUT_PATCH_SIZE 3
[domain("tri")] //�����ɗ��p����`����w��@"tri" "quad" "isoline"����I��
DsOutput main(
    HsConstantOutput hsConst,
    const OutputPatch<HsControlPointOutput, INPUT_PATCH_SIZE> i,
    float3 bary : SV_DomainLocation)
{
    DsOutput o ;

    //�V�����o�͂���e���_�̍��W���v�Z

	float3 f3Position =
        bary.x * i[0].position +
        bary.y * i[1].position +
        bary.z * i[2].position;

    //�V�����o�͂���e���_�̖@�����v�Z
    o.normal = normalize(
        bary.x * i[0].normal +
        bary.y * i[1].normal +
        bary.z * i[2].normal);

    //�V�����o�͂���e���_��UV���W���v�Z
    o.uv =
        bary.x * i[0].texCoord +
        bary.y * i[1].texCoord +
        bary.z * i[2].texCoord;

    o.svpos= float4(f3Position.x, f3Position.y, f3Position.z, 0);
    //o.worldpos = 

    return o;
}
