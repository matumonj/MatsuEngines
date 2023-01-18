#pragma once
#include "Texture.h"
#include<memory>
#include"Particle.h"
#include"Object3d.h"
#include "ObjectManager.h"

class BronzeAttack
{
private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	static BronzeAttack* GetIns();
	void Init();
	void Upda();
	void Draw();
	void Finalize();

	void SetAction(bool f) { if (phase == NON) { phase = AREASET; } }

	bool GetPhaseEnd()
	{
		if (phase == END) { return true; }
		return false;
	}
	void SetAttackPhase(bool f) { if (f && phase != AREASET) { phase = AREASET; } }

	void SetActionNon() { phase = NON; }

	bool GetisEndAttack() { return isEndAttack; }
	void SetisEndAttack(bool f) { isEndAttack = f; }

public:
	enum Phase
	{
		NON,
		AREASET,
		BOM,
		BEAMSMALL,
		TEXFADE,
		END
	} phase;

	Phase GetPhase() { return phase; }
private:
	

	void Phase_AreaSet();
	void Phase_Bom();
	void Phase_MakeSmall();
	void Phase_TexFade();
	void Phase_End();
private:
	std::unique_ptr<Texture> MagicTex;
	std::unique_ptr<Object3d> BeamObj;

	float scalingETime;
	XMFLOAT2 TexScl;
	XMFLOAT2 TexPos;
	float TexAlpha;
	float TexRotZ;
	//bossの座標
	XMFLOAT3 BossRot;
	XMFLOAT3 BossPos;
	XMFLOAT3 BossColor;

	//びーむの各パラメータ
	XMFLOAT3 BeamObjPos;
	XMFLOAT3 BeamObjScl;
	XMFLOAT3 BeamMaxScl;
	//アルファ値
	float ObjAlpha;
	float ColorT;

	bool isEndAttack;
};

