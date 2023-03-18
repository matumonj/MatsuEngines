cbuffer cbuff0 : register(b0)
{
float4 color;
matrix viewproj; // �r���[�v���W�F�N�V�����s��
matrix world; // ���[���h�s��
float3 cameraPos; // �J�������W�i���[���h���W�j
bool gsflag;
bool flag;
float time;
bool destF;
	float desttime;

//float3 destcenter;
float dislen;
float3 playerpos;
bool bloomf;
	bool shadowf;
	float2 pading;
};

cbuffer cbuff1 : register(b1)
{
float3 m_ambient : packoffset(c0); // �A���r�G���g�W��
float3 m_diffuse : packoffset(c1); // �f�B�t���[�Y�W��
float3 m_specular : packoffset(c2); // �X�y�L�����[�W��
float m_alpha : packoffset(c2.w); // �A���t�@
}


struct Point
{
	float x;
	float y;
};

struct Line2D
{
	float2 start;
	float2 end;
};

// ���s�����̐�
static const int DIRLIGHT_NUM = 3;

struct DirLight
{
	float3 lightv; // ���C�g�ւ̕����̒P�ʃx�N�g��
	float3 lightcolor; // ���C�g�̐F(RGB)
	uint active;
};

// �_�����̐�
static const int POINTLIGHT_NUM = 13;

struct PointLight
{
	float3 lightpos; // ���C�g���W
	float3 lightcolor; // ���C�g�̐F(RGB)
	float3 lightatten; // ���C�g���������W��
	uint active;
};

// �X�|�b�g���C�g�̐�
static const int SPOTLIGHT_NUM = 13;

struct SpotLight
{
	float3 lightvs; // ���C�g�̌��������̋t�x�N�g���i�P�ʃx�N�g���j
	float3 lightpos; // ���C�g���W
	float3 lightcolor; // ���C�g�̐F(RGB)
	float3 lightatten; // ���C�g���������W��
	float2 lightfactoranglecos; // ���C�g�����p�x�̃R�T�C��
	uint active;
};

// �ۉe�̐�
static const int CIRCLESHADOW_NUM = 30;

struct CircleShadow
{
	float3 dir; // ���e�����̋t�x�N�g���i�P�ʃx�N�g���j
	float3 casterPos; // �L���X�^�[���W
	float distanceCasterLight; // �L���X�^�[�ƃ��C�g�̋���
	float3 atten; // ���������W��
	float2 factorAngleCos; // �����p�x�̃R�T�C��
	uint active;
};

cbuffer cbuff2 : register(b2)
{
float3 ambientColor;
DirLight dirLights[DIRLIGHT_NUM];
PointLight pointLights[POINTLIGHT_NUM];
SpotLight spotLights[SPOTLIGHT_NUM];
CircleShadow circleShadows[CIRCLESHADOW_NUM];
}

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	//float4 color:COLOR;
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	float4 worldpos : POSITION; // ���[���h���W
	float3 normal :NORMAL; // �@��
	float2 uv :TEXCOORD; // uv�l
};


struct PSOutPut
{
	float4 target0:SV_TARGET0;
	float4 target1:SV_TARGET1;
};

struct g2f
{
	float4 pos : SV_POSITION;
	//float4 col : COLOR;
	float4 wo : POSITION; // ���[���h���W
	float3 normal:NORMAL;
	float2 uv:TEXCOORD;
};

struct GSOutput
{
	//float color_Alpha:COLOR;
	float4 svpos:SV_POSITION;
	float4 worldpos : POSITION; // ���[���h���W
	float3 normal:NORMAL;
	float2 uv:TEXCOORD;
};

//���_�V�F�[�_�[����n���V�F�[�_�[�ɓn���\����
struct HsInput
{
	float4 svpos:SV_POSITION;
	float4 worldpos : POSITION; // ���[���h���W
	float3 normal:NORMAL;
	float2 uv:TEXCOORD;
};

//�n���V�F�[�_�[����e�b�Z���[�^�[�o�R�Ńh���C���V�F�[�_�[�ɓn���\����
struct HsControlPointOutput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

//Patch-Constant-Function����e�b�Z���[�^�[�o�R�Ńh���C���V�F�[�_�[�ɓn���\����
struct HsConstantOutput
{
	float tessFactor[3] : SV_TessFactor;
	float insideTessFactor : SV_InsideTessFactor;
};

//�o�̓f�[�^
struct DS_OUTPUT
{
	float4 svpos:SV_POSITION;
	float4 worldpos : POSITION; // ���[���h���W
	float3 normal:NORMAL;
	float2 uv:TEXCOORD;
};
// �e�b�Z���[�^���o�͂������_
struct HS_CONTROL_POINT_OUTPUT
{
		float4 svpos:SV_POSITION;
		float4 worldpos : POSITION; // ���[���h���W
		float3 normal:NORMAL;
		float2 uv:TEXCOORD;
};
// �o�̓p�b�`�萔�f�[�^�B
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3] : SV_TessFactor;
	float InsideTessFactor : SV_InsideTessFactor;
};
//�h���C���V�F�[�_�[����t���O�����g�V�F�[�_�[�ɓn���\����
struct DsOutput
{
	float4 svpos:SV_POSITION;
	
};
