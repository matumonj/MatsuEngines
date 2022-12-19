#include "BossSpell.h"
#include"mHelper.h"
#include"Input.h"

BossSpell::~BossSpell()
{
	for (int i = 0; i < 6; i++)
	{
		delete Attack[i].SpellFrame, Attack[i].SpellGauge;
	}
}

void BossSpell::Finalize()
{
	for (int i = 0; i < 6; i++)
	{
		delete Attack[i].SpellFrame, Attack[i].SpellGauge;
	}
}

BossSpell* BossSpell::GetInstance()
{
	static BossSpell instance;
	return &instance;
}

void BossSpell::Initialize()
{
	//���U��
	SpellsInit(Attack[HALF_LEFT], 50, 51);
	SpellsInit(Attack[HALF_RIGHT], 50, 51);
	SpellsInit(Attack[CIRCLE], 50, 52);
	SpellsInit(Attack[KNOCK], 50, 53);
	SpellsInit(Attack[ULTIMATE], 50, 51);
	SpellsInit(Attack[SLAM], 50, 51);
}

void BossSpell::Update()
{
	/*1����:�U����� 2����:�U���ҋ@����*/
	SpellAction(Attack[HALF_LEFT], 120.0f);
	SpellAction(Attack[HALF_RIGHT], 120.0f);
	SpellAction(Attack[CIRCLE], 180.0f);
	SpellAction(Attack[KNOCK], 240.0f);
	SpellAction(Attack[ULTIMATE], 120.0f);
	SpellAction(Attack[SLAM], 120.0f);
}

void BossSpell::Draw()
{
}

//�e�\���̂̏������A�X�V�Ȃǂ܂Ƃ߂ď�������֐�
void BossSpell::SpellsInit(Spells& spells, int TexNum_G, int TexNum_F)
{
	//���U��
	spells.SpellFrame = Sprite::Create(TexNum_F, {30.0f, 200.0f});
	spells.SpellGauge = Sprite::Create(TexNum_G, {40.0f, 248.0f});
	spells.SpellFrame->SetAnchorPoint({0.0f, 0.0f});
	spells.SpellGauge->SetAnchorPoint({0.0f, 0.5f});
	spells.SpellFrame->SetSize({300.0f, 300.0f});
	spells.SpellGauge->SetSize({0.0f, 0.0f});
}


void BossSpell::SpellAction(Spells& spell, float spellCount)
{
	if (spell.StartJudg)
	{
		spell.SpellFrame->setcolor({1.0f, 1.0f, 1.0f, 1.0f});
		spell.SpellGauge->setcolor({1.0f, 1.0f, 1.0f, 1.0f});
		spell.Time += 1.0f / spellCount;
		spell.SpellGauge->SetSize({Easing::EaseOut(spell.Time, 0.0f, 286.0f), 200.0f});
		if (spell.Time >= 1.0f)
		{
			spell.EndJudg = true;
		}
	}
	if (spell.EndJudg)
	{
		spell.Time = 0.0f;
		spell.SpellGauge->SetSize({0.0f, 0.0f});
		spell.StartJudg = false;
	}
}

void BossSpell::SpellsDraw(const Spells& spells)
{
	if (!spells.EndJudg && spells.StartJudg)
	{
		//	spells.SpellFrame->Draw();
		//spells.SpellGauge->Draw();
	}
}
