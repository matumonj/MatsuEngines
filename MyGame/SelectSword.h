#pragma once
#include"SwordBase.h"
#include"Sprite.h"
#include<memory>
#include"Input.h"
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
	enum SwordScale{
		SMALL,
		NORMAL,
		BIG
	};
private:
	Input* input;
	Sprite*SwordSample[3];
	Sprite* WeponParamSprite[3];
	Sprite* Frame;
	float Alpha;
	XMFLOAT2 Position[3] = { {200,400}, {600,400}, {1000,400} };
	XMFLOAT2 WeponParamSpritePos[3];
	float ypos_Up = 635.0f;
	float ypos_Center = 730.0f;
	float ypos_Bottom = 835.0f;
	float WeponParamSpriteAlpha[3];
	float WeponParamSpriteEaseT[3];
	SwordScale NowSelectSword=NORMAL;
	int index;
	std::unique_ptr<SwordBase>Sword;
	bool SelectJudg;
	bool SaveJudg;

private:
	std::unique_ptr<Object3d>PedestalObj;

	std::unique_ptr<Object3d>SampleSword[3];

	float RotAngle = 0;
	XMFLOAT3 sampleSwordRot[3];
	float RotAngle_Old[3] = {0,120,240};
	float EaseTime = 0.0f;
	bool RotationF;
	enum RotDir {
		NON,
		RIGHT,
		LEFT
	};
	RotDir rotDir;
public:
	static SelectSword* GetInstance();
	void Initialize();

	void Update();

	void Draw();
	
	void Finalize();

	void SwordDraw();
	void PedestalDraw();
private:
	void SetSword(SwordScale nowsword);

	void pedestalSet();
	void PedestalUpdate();

	void SpriteSet();
	void SpriteDraw();

	void SwordRot();

	void WeponParamInit();
	void WeponParamUpdate();
public:
	void SetSelectJudg(bool f) { SelectJudg = f; }
	bool GetSelectJudg() { return SelectJudg; }

	SwordBase* GetSword() { return Sword.get(); }
};

