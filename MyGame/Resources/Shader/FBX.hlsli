cbuffer cbuff0 : register(b0)
{
float4 color;
matrix viewproj; //�r���[�v���W�F�N�V�����s��
matrix world; //���[���h�s��

float3 cameraPos; //�J�������W�i���[���h���W�j
};

//�o�[�e�b�N�X�o�b�t�@�[�̓���
struct VSInput
{
	float4 pos : POSITION; //�ʒu
	float3 normal : NORMAL; //���_�@��
	float2 uv : TEXCOORD; //�e�N�X�`���[���W
	uint4 boneIndices : BONEINDICES; //�{�[���̔ԍ�
	float4 boneWeights : BONEWEIGHTS; //�{�[���̃X�L���E�F�C�g
};

//���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION; //�V�X�e���p���_���W
	float4 worldpos : POSITION; // ���[���h���W
	float3 normal : NORMAL; //�@��
	float2 uv : TEXCOORD; //uv�l
	//float depth : TEXCOORD2; // �J������Ԃł�Z�l
};

//�{�[���̍ő吔
static const int MAX_BONES = 188;

cbuffer skinning : register(b3) //�{�[���̃X�L�j���O�s�񂪓���
{
matrix matSkinning[MAX_BONES];
}


struct PSOutPut
{
	float4 target0:SV_TARGET0;
	float4 target1:SV_TARGET1;
};
cbuffer cbuff1 : register(b1)
{
float3 m_ambient : packoffset(c0); // �A���r�G���g�W��
float3 m_diffuse : packoffset(c1); // �f�B�t���[�Y�W��
float3 m_specular : packoffset(c2); // �X�y�L�����[�W��
float m_alpha : packoffset(c2.w); // �A���t�@
}


struct PSInPut
{
	float4 pos : POSITION; //�ʒu
	float3 normal : NORMAL; //���_�@��
	float2 uv : TEXCOORD; //�e�N�X�`���[���W
	// step-7 �J������Ԃł�Z�l���L�^����ϐ���ǉ�
	float3 depthInView : TEXCOORD2; // �J������Ԃł�Z�l
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
	float3 lightv; // ���C�g�̌��������̋t�x�N�g���i�P�ʃx�N�g���j
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
