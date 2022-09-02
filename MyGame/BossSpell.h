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
private:
	struct Spells {
		Sprite* SpellFrame;
		Sprite* SpellGauge;
		float Time;//�C�[�W���O�p
		bool StartJudg;//�r���J�n
		bool EndJudg;//�r���I��
	};
	Spells HalfAttack_Left;
	Spells HalfAttack_Right;
	Spells CircleAttack;

private:
	void SpellsInit(Spells& spell, int TexNum_G, int TexNum_F);
	void SpellAction(Spells& spell,float spellCount);
	void SpellsDraw(Spells spells);

public:
	void SetStartSpell_HL(bool f) { HalfAttack_Left.StartJudg = f; }
	void SetStartSpell_HR(bool f) { HalfAttack_Right.StartJudg = f; }
	void SetStartSpell_CA(bool f) { CircleAttack.StartJudg = f; }

	bool GetEndSpell_HL() { return HalfAttack_Left.EndJudg; }
	bool GetEndSpell_HR() { return HalfAttack_Right.EndJudg; }
	bool GetEndSpell_CA() { return CircleAttack.EndJudg; }

};

