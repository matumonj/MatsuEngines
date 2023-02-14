#pragma once
#include"CollisionPrimitive.h"
#include "ObjectManager.h"

#include<fstream>
#include<string>
#include<sstream>

class SwordBase :
	public ObjectManager
{
public:
	SwordBase();
	~SwordBase() override;
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	XMMATRIX GetMatrot() { return m_Object->GetMatWorld(); }
	XMMATRIX GetMatWorld() { return m_Object->GetMatWorld(); }
	double GetAnimationTime() { return AnimationTime; }
	float GetKnockPower() { return KnockPower; }

	XMFLOAT3 GetSwordObbScl() { return SwordObbScl; }
protected:
	int Damage;
	int CoolTime;
	double AnimationTime;
	float KnockPower;
	XMFLOAT3 SwordObbScl;

private:
protected:
	void LoadCsv(std::string damval, std::string knoval, std::string speval, std::string modname);
	std::string line;
	std::stringstream popcom;
	std::ifstream file;

	std::unique_ptr<Model> swordModel;
	bool LoadF;
	std::string modelname;
public:
	int GetDamage() { return Damage; }
	int GetCoolTime() { return CoolTime; }
};
