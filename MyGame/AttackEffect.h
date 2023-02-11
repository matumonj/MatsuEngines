#pragma once
#include"Texture.h"
#include"Enemy.h"

class AttackEffect
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static AttackEffect* GetIns();

public:
	void Init();
	void Upda();
	void Draw();

	void ParticleUpda();
	void SetParticle(XMFLOAT3 pos);

	void GuarEffect(XMFLOAT3 pos);
private:
	void LoadTex();
	void Effect_Non();
	void Effect_First();
	void Effect_Second();
	void Effect_Third();
	void Effect_Particle();
	void Effect_Clear();
public:
	enum EffectType
	{
		NON,
		SLASH_FIRST,
		SLASH_SECOND,
		SLASH_THIRD,
		PARTICLE,
		CLEAREFFECT
	};

	enum ParticleSize
	{
		//パティクル数設定
		FIRST = 10,
		SECOND = 10,
		THIRD=15
	};

	enum EffectUpda
	{
		//関数内処理順
		LOAD,
		UPDA,
		CLEAR
	};

	EffectUpda eupda = LOAD;
	EffectType etype = NON;

	void SetEffectType(EffectType type) { etype = type; }

	EffectType GetEffectType() { return etype; }


private:
	//斬撃用テクスチャ
	static constexpr int Gsize = 2;
	std::unique_ptr<Texture> AttackTex;
	std::unique_ptr<Texture> GuardTex[2];
	float GuardAlpha[2];
	XMFLOAT3 GuardSize[2];
	XMFLOAT3 GuardRot[2];
	bool GuarJudg;

	enum GuarPhase
	{
		NOGUARD,
		LARGE,
		FEED,
		END
	} gphase = END;

	//攻撃時に出るパーティクル
	std::vector<std::unique_ptr<Texture>> AttackParticle;
	//
	std::unique_ptr<Texture> InpactTex;
	//攻撃用テクスチャのパラメータ
	XMFLOAT3 TexPos;
	XMFLOAT3 TexRot;
	XMFLOAT3 TexScl;
	float texAlpha = 1.0f;
	//パーティクル発生位置
	XMFLOAT3 ParCenterPos;
	//各パーティクル座標、スケール
	std::vector<XMFLOAT3> ParScl;
	std::vector<XMFLOAT3> ParPos;
	std::vector<XMFLOAT3> ParRot;
	std::vector<float> ParAlpha;
	bool CreateParFlag;
	//
	float InpactAlpha = 0;
	XMFLOAT2 InpactScl = {0.0f, 0.0f};
	XMFLOAT3 defrot;
	XMMATRIX matwo;
	XMFLOAT3 addrot;
public:
	void SetMatW(XMMATRIX m) { matwo = m; }
	void SetGuardRot(XMFLOAT3 rot) { defrot = rot; }

	void SetGuadJudg(bool f)
	{
		gphase = NOGUARD;
		GuarJudg = f;
	}
};
