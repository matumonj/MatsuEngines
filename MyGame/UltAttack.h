#pragma once
#include "Texture.h"
#include<memory>
#include"Object3d.h"
#include"BossAttackActionManager.h"

class UltAttack : public BossAttackActionManager
{
public:
	static UltAttack* GetIns();
	void Init() override;
	void Upda() override;
	void Draw() override;
	void Finalize();


	bool GetisEndAttack() { return isEndAttack; }
	void SetisEndAttack(bool f) { isEndAttack = f; }

private:
	void Phase_AreaSet();
	void Phase_Bom();
	void Phase_MakeSmall();
	void Phase_TexFade();
	void Phase_End();

public:
	void SceneBloom();
	bool GetFieldDestG() { return fielddestf; }
private:
	std::unique_ptr<Texture> DamageTex;
	std::unique_ptr<Object3d> normalAttackObj[8];
	XMFLOAT3 beampos[8];
	XMFLOAT3 beamscl[8];
	float scalingETime[8];
	bool fielddestf;
	XMFLOAT2 TexScl;
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

	float BloomAlpha;

	bool isEndAttack;

public:
	float GetBloomAlpha() { return BloomAlpha; }
};
