#pragma once
#include"Sprite.h"
#include<memory>
class ExpPointSystem
{
private:
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	std::unique_ptr<Sprite>m_ExpBarSpite;
	std::unique_ptr<Sprite>m_ExpBarFrameSprite;
private:
	int m_ExpPoint;
	int m_OldExpPoint;
	int m_Lvel;
	int m_LevelUp_needExp;
	bool m_LevelUpF;
	bool m_BarSet;
	float BarSclX_AddValue;

	XMFLOAT2 BarPos;
	XMFLOAT2 OldBarScl;
	XMFLOAT2 BarScl;

	XMFLOAT2 BarFramePos;
	XMFLOAT2 BarFrameScl;
	float m_EaseTime;
public:
	static ExpPointSystem* GetInstance();
	void Init();
	void Upda();
	void Draw();
private:
	void expBarUpda();
public:
	void ExpPoint_Get(int point);
	void ExpPoint_Los(int point);
};

