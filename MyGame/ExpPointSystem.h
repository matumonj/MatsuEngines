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
	std::unique_ptr<Sprite> m_BossHP;
	std::unique_ptr<Sprite> m_BossHPFrame;
private:
	int m_ExpPoint;
	int ol;
	int m_OldExpPoint;
	int m_Lvel;
	int m_LevelUp_needExp;
	bool m_LevelUpF;
	bool m_BarSet;

	//Ç±Ç±Ç©ÇÁÇÃïœêîå„Ç≈ëSè¡Çµ
	float BarSclX_AddValue;
	float fontEaseT[6];
	float fontSize[6];
	bool m_ReturnEase[6];
	bool TextDis;
	int nun;
	int nextl = 10;
	int old;
	int oldGetExp;
	XMFLOAT2 BarPos;
	XMFLOAT2 OldBarScl;
	XMFLOAT2 BarScl;
	XMFLOAT2 LevelUpTexPos;
	XMFLOAT2 BarFramePos;
	XMFLOAT2 BarFrameScl;
	float m_EaseTime;
	int OldHP;
	int NowHP;
public:
	static ExpPointSystem* GetInstance();
	void Init();
	void Upda();
	void Draw();
private:
	void LvelUpFont_Display();
	void LvelUpFont_FeedOut();
	void expBarUpda();
public:
	void ExpPoint_Get(int point);
	void ExpPoint_Los(int point);
};
