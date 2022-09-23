#include "BossSpell.h"
#include"mHelper.h"
#include"Input.h"
BossSpell::~BossSpell()
{
	delete HalfAttack_Left.SpellFrame, HalfAttack_Left.SpellGauge;
	delete HalfAttack_Right.SpellFrame, HalfAttack_Right.SpellGauge;
	delete CircleAttack.SpellFrame, CircleAttack.SpellGauge;
}

void BossSpell::Finalize()
{
	delete HalfAttack_Left.SpellFrame, HalfAttack_Left.SpellGauge;
	delete HalfAttack_Right.SpellFrame, HalfAttack_Right.SpellGauge;
	delete CircleAttack.SpellFrame, CircleAttack.SpellGauge;
}

BossSpell* BossSpell::GetInstance()
{
	static BossSpell instance;
	return &instance;
}

void BossSpell::Initialize()
{
	Sprite::LoadTexture(50, L"Resources/Spail.png");
	Sprite::LoadTexture(51, L"Resources/halfAttackFrame.png");
	Sprite::LoadTexture(52, L"Resources/CircleAttackFarame.png");
	Sprite::LoadTexture(53, L"Resources/KnockAttackFrame.png");
	//¶UŒ‚
	SpellsInit(HalfAttack_Left, 50, 51);
	SpellsInit(HalfAttack_Right, 50, 51);
	SpellsInit(CircleAttack, 50, 52);
	SpellsInit(KnockAttack, 50, 53);
	}

void BossSpell::Update()
{
	/*1ˆø”:UŒ‚Ží—Þ 2ˆø”:UŒ‚‘Ò‹@ŽžŠÔ*/
	SpellAction(HalfAttack_Left, 120.0f);
	SpellAction(HalfAttack_Right, 120.0f);
	SpellAction(CircleAttack, 180.0f);
	SpellAction(KnockAttack, 240.0f);
}
void BossSpell::Draw()
{
	Sprite::PreDraw();
	SpellsDraw(HalfAttack_Left);
	SpellsDraw(HalfAttack_Right);
	SpellsDraw(CircleAttack);
	SpellsDraw(KnockAttack);
	Sprite::PostDraw();
}

//Še\‘¢‘Ì‚Ì‰Šú‰»AXV‚È‚Ç‚Ü‚Æ‚ß‚Äˆ—‚·‚éŠÖ”
void BossSpell::SpellsInit(Spells& spells, int TexNum_G,int TexNum_F)
{
	//¶UŒ‚
	spells.SpellFrame = Sprite::Create(TexNum_F, { 30.0f ,200.0f });
	spells.SpellGauge = Sprite::Create(TexNum_G, { 40.0f ,248.0f });
	spells.SpellFrame->SetAnchorPoint({ 0.0f ,0.0f });
	spells.SpellGauge->SetAnchorPoint({ 0.0f ,0.5f });
	spells.SpellFrame->SetSize({ 300.0f ,300.0f });
	spells.SpellGauge->SetSize({ 0.0f ,0.0f });
}


void BossSpell::SpellAction(Spells& spell, float spellCount)
{
	if (spell.StartJudg) {
		spell.SpellFrame->setcolor({ 1.0f ,1.0f ,1.0f ,1.0f });
		spell.SpellGauge->setcolor({ 1.0f ,1.0f ,1.0f ,1.0f });
		spell.Time += 1.0f / spellCount;
		spell.SpellGauge->SetSize({ Easing::EaseOut(spell.Time,0.0f ,286.0f),200.0f });
		if (spell.Time >= 1.0f) {
			spell.EndJudg = true;
		}
	}
	if (spell.EndJudg) {
		spell.Time = 0.0f;
		spell.SpellGauge->SetSize({ 0.0f ,0.0f });
		spell.StartJudg = false;
	}
}
void BossSpell::SpellsDraw(Spells spells)
{
	if (!spells.EndJudg&&spells.StartJudg) {
		spells.SpellFrame->Draw();
		spells.SpellGauge->Draw();
	}
}