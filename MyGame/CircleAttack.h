#pragma once
#include"Texture.h"
#include"Object3d.h"
#include<memory>
#include"Sprite.h"
#include"BossAttackActionManager.h"

class CircleAttack : public BossAttackActionManager
{
public:
	~CircleAttack();

public:
	

private:
	float rotY;
	static const int DirectionNum = 4;
	
	float TexAlpha = 1;
	float CircleAreaTime = 0;
	XMFLOAT2 CircleSize = {0, 0};
	
	Texture* ImpactAreaTex;
	std::unique_ptr<Texture>AllAreaTex;
	std::unique_ptr<Object3d>RingObj;
public:
	static CircleAttack* GetIns();
	void Init() override;
	void Upda() override;
	void Draw() override;

private:
	void DamageAreaTexSet();
	void ImpactAttack();
	void EndAttackAction();
public:

	
private:
	void CollisonNailPlayer();
};
