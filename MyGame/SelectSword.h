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
	Sprite* Frame;
	float Alpha;
	XMFLOAT2 Position[3] = { {200,400}, {600,400}, {1000,400} };
	SwordScale NowSelectSword=NORMAL;
	int index;
	std::unique_ptr<SwordBase>Sword;
	bool SelectJudg;
	bool SaveJudg;
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
public:
	void SetSelectJudg(bool f) { SelectJudg = f; }
	bool GetSelectJudg() { return SelectJudg; }

	SwordBase* GetSword() { return Sword.get(); }
};

