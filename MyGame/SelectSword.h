#pragma once
#include"SwordBase.h"
#include"Sprite.h"
#include<memory>
#include"Input.h"
#include<array>
#include"Particle.h"

class SelectSword
{
public:
	~SelectSword();
private:
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	enum SelectWeapon
	{
		WAND,
		SWORD,
		AXE,
	};

private:
	Input* input;
	Sprite* SwordSample[3];

	Sprite* WeaponFrame[3];
	Sprite* Frame;

	Sprite* Button_LB;
	Sprite* Button_RB;

	XMFLOAT2 RBPos;
	XMFLOAT2 LBPos;
	XMFLOAT2 RBScl;
	XMFLOAT2 LBScl;
	float FrameScalingTime;
	float Alpha;
	XMFLOAT2 Position[3] = {{1300, 30}, {1500, 30}, {1700, 30}};
	XMFLOAT2 Scale[3] = {{200, 200}, {200, 200}, {200, 200}};

	XMFLOAT2 oldpos[3] = {{1300, 30}, {1500, 30}, {1700, 30}};
	SelectWeapon NowSelectSword = SWORD;
	int index;
	int oldindex;
	bool weaponscalingf;
	float WeaponScalingETime;
	XMFLOAT3 WeapomScl;
	SwordBase* sample_sword[3];
	std::unique_ptr<SwordBase> Sword;
	bool SelectJudg_Right;
	bool SelectJudg_Left;
	bool TurnOffDrawF;
	std::unique_ptr<Particle> WeaponChangeEffect;
private:
	float sampleSwordAlpha[3];
	float EaseTime = 0.0f;

public:
	static SelectSword* GetInstance();
	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void SwordDraw();
private:
	void SetSword(SelectWeapon nowsword);


	void SpriteSet();
	void SpriteDraw();


public:
	SwordBase* GetSword() { return Sword.get(); }
};
