cbuffer cbuff0 : register(b0)
{
float4 color;
matrix viewproj; //ビュープロジェクション行列
matrix world; //ワールド行列

float3 cameraPos; //カメラ座標（ワールド座標）
};

//バーテックスバッファーの入力
struct VSInput
{
	float4 pos : POSITION; //位置
	float3 normal : NORMAL; //頂点法線
	float2 uv : TEXCOORD; //テクスチャー座標
	uint4 boneIndices : BONEINDICES; //ボーンの番号
	float4 boneWeights : BONEWEIGHTS; //ボーンのスキンウェイト
};

//頂点シェーダーからピクセルシェーダーへのやり方に使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; //システム用頂点座標
	float4 worldpos : POSITION; // ワールド座標
	float3 normal : NORMAL; //法線
	float2 uv : TEXCOORD; //uv値
	//float depth : TEXCOORD2; // カメラ空間でのZ値
};

//ボーンの最大数
static const int MAX_BONES = 188;

cbuffer skinning : register(b3) //ボーンのスキニング行列が入る
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
	float3 m_ambient : packoffset(c0); // アンビエント係数
	float3 m_diffuse : packoffset(c1); // ディフューズ係数
	float3 m_specular : packoffset(c2); // スペキュラー係数
	float m_alpha : packoffset(c2.w); // アルファ
}


struct PSInPut
{
	float4 pos : POSITION; //位置
	float3 normal : NORMAL; //頂点法線
	float2 uv : TEXCOORD; //テクスチャー座標
	// step-7 カメラ空間でのZ値を記録する変数を追加
	float3 depthInView : TEXCOORD2; // カメラ空間でのZ値
};



// 平行光源の数
static const int DIRLIGHT_NUM = 3;

struct DirLight
{
	float3 lightv; // ライトへの方向の単位ベクトル
	float3 lightcolor; // ライトの色(RGB)
	uint active;
};

// 点光源の数
static const int POINTLIGHT_NUM = 13;

struct PointLight
{
	float3 lightpos; // ライト座標
	float3 lightcolor; // ライトの色(RGB)
	float3 lightatten; // ライト距離減衰係数
	uint active;
};

// スポットライトの数
static const int SPOTLIGHT_NUM = 13;

struct SpotLight
{
	float3 lightv; // ライトの光線方向の逆ベクトル（単位ベクトル）
	float3 lightpos; // ライト座標
	float3 lightcolor; // ライトの色(RGB)
	float3 lightatten; // ライト距離減衰係数
	float2 lightfactoranglecos; // ライト減衰角度のコサイン
	uint active;
};

// 丸影の数
static const int CIRCLESHADOW_NUM = 30;

struct CircleShadow
{
	float3 dir; // 投影方向の逆ベクトル（単位ベクトル）
	float3 casterPos; // キャスター座標
	float distanceCasterLight; // キャスターとライトの距離
	float3 atten; // 距離減衰係数
	float2 factorAngleCos; // 減衰角度のコサイン
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
