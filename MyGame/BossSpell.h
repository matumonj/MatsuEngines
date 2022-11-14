#pragma once
#include"Sprite.h"

class BossSpell
{
public:
	~BossSpell();
public:
	static BossSpell* GetInstance();

	void Initialize();
	void Update();
	void Draw();
	void Finalize();
private:
	struct Spells
	{
		Sprite* SpellFrame;
		Sprite* SpellGauge;
		float Time; //�C�[�W���O�p
		bool StartJudg; //�r���J�n
		bool EndJudg; //�r���I��
	};

public:
	enum AttackType
	{
		HALF_LEFT,
		HALF_RIGHT,
		CIRCLE,
		KNOCK,
		ULTIMATE,
		SLAM
	};

private:
	static const int max_attack = 6;
	Spells Attack[max_attack];

private:
	void SpellsInit(Spells& spell, int TexNum_G, int TexNum_F);
	void SpellAction(Spells& spell, float spellCount);
	void SpellsDraw(const Spells& spells);

public:
	void SetStartSpell(const AttackType& index, bool f) { Attack[index].StartJudg = f; }

	bool GetEndSpell(const AttackType& index) { return Attack[index].EndJudg; }
	void SetEndSpell(const AttackType& index, bool f) { Attack[index].EndJudg = f; }
};
