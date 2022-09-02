#include "BossSpell.h"
#include"mHelper.h"
#include"Input.h"
BossSpell::~BossSpell()
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
	Sprite::LoadTexture(50, L"Resources/SpailFrame.png");
	Sprite::LoadTexture(51, L"Resources/Spail.png");
	//¶UŒ‚
	HalfAttack_Left.SpellFrame = Sprite::Create(50, { 30,200 });
	HalfAttack_Left.SpellGauge = Sprite::Create(51, { 30,210});
	HalfAttack_Left.SpellFrame->SetAnchorPoint({ 0,0 });
	HalfAttack_Left.SpellGauge->SetAnchorPoint({ 0,0.5 });
	HalfAttack_Left.SpellFrame->SetSize({ 150,150 });
	HalfAttack_Left.SpellGauge->SetSize({ 0,0 });
	//¶UŒ‚
	HalfAttack_Right.SpellFrame = Sprite::Create(50, { 30,200 });
	HalfAttack_Right.SpellGauge = Sprite::Create(51, { 30,210 });
	HalfAttack_Right.SpellFrame->SetAnchorPoint({ 0,0 });
	HalfAttack_Right.SpellGauge->SetAnchorPoint({ 0,0.5 });
	HalfAttack_Right.SpellFrame->SetSize({ 150,150 });
	HalfAttack_Right.SpellGauge->SetSize({ 0,0 });
	//¶UŒ‚
	CircleAttack.SpellFrame = Sprite::Create(50, { 30,200 });
	CircleAttack.SpellGauge = Sprite::Create(51, { 30,210 });
	CircleAttack.SpellFrame->SetSize({ 150,150 });
	CircleAttack.SpellGauge->SetSize({ 0,0 });
}

void BossSpell::Update()
{
	if (Input::GetInstance()->TriggerButton(Input::Button_A)) {
	//	HalfAttack_Left.StartJudg = true;
	}
	SpellAction(HalfAttack_Left, 120.0f);
	SpellAction(HalfAttack_Right, 120.0f);
	SpellAction(CircleAttack, 180.0f);
}

void BossSpell::SpellAction(Spells& spell,float spellCount)
{
	if (spell.StartJudg) {
		spell.SpellFrame->setcolor({ 1,1,1,1 });
		spell.SpellGauge->setcolor({ 1,1,1,1 });
		spell.Time += 1.0f/spellCount;
		spell.SpellGauge->SetSize({ Easing::EaseOut(spell.Time,0,143),130 });
		if (spell.Time >= 1.0f) {
			spell.EndJudg = true;
			spell.StartJudg = false;

		}
	}
	if (spell.EndJudg) {

		spell.Time = 0.0f;
	}
}
void BossSpell::Draw()
{
	Sprite::PreDraw();
	SpellsDraw(HalfAttack_Left);
	SpellsDraw(HalfAttack_Right);
	SpellsDraw(CircleAttack);
	Sprite::PostDraw();
}

void BossSpell::SpellsDraw(Spells spells)
{
	if (!spells.EndJudg&&spells.StartJudg) {
		spells.SpellFrame->Draw();
		spells.SpellGauge->Draw();
	}
}