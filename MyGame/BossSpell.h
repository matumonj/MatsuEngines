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
	Spells KnockAttack;
	//Spells UltimateAttack;
private:
	void SpellsInit(Spells& spell, int TexNum_G, int TexNum_F);
	void SpellAction(Spells& spell,float spellCount);
	void SpellsDraw(Spells spells);

public:
	void SetStartSpell_HL(bool f) { HalfAttack_Left.StartJudg = f; }
	void SetStartSpell_HR(bool f) { HalfAttack_Right.StartJudg = f; }
	void SetStartSpell_CA(bool f) { CircleAttack.StartJudg = f; }
	void SetStartSpell_KA(bool f) { KnockAttack.StartJudg = f; }
	//void SetStartSpell_UA(bool f) { UltimateAttack.StartJudg = f; }

	bool GetEndSpell_HL() { return HalfAttack_Left.EndJudg; }
	void SetEndSpell_HL(bool f) { HalfAttack_Left.EndJudg=f; }
	bool GetEndSpell_HR() { return HalfAttack_Right.EndJudg; }
	void SetEndSpell_HR(bool f) { HalfAttack_Right.EndJudg = f; }
	
	void SetEndSpell_CA(bool f) { CircleAttack.EndJudg = f; }
	bool GetEndSpell_CA() { return CircleAttack.EndJudg; }

	bool GetEndSpell_KA() { return KnockAttack.EndJudg; }
	void SetEndSpell_KA(bool f) { KnockAttack.EndJudg = f; }


	//bool GetEndSpell_UA() { return UltimateAttack.EndJudg; }
	//void SetEndSpell_UA(bool f) { UltimateAttack.EndJudg = f; }

};

