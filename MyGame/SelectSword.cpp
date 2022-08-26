#include "SelectSword.h"
#include"BigSword.h"
#include"SmallSword.h"
#include"CameraControl.h"

void SelectSword::SetSmallSword(SwordScale nowsword)
{
	switch (nowsword)
	{
	case SelectSword::SMALL:
		Sword = std::make_unique<SmallSword>();
		break;
	case SelectSword::NORMAL:
		Sword = std::make_unique<BigSword>();
		break;
	case SelectSword::BIG:
		Sword = std::make_unique<BigSword>();
		break;
	default:
		break;
	}
	//Sword = std::make_unique<SmallSword>();
	Sword->Initialize(CameraControl::GetInstance()->GetCamera());

	NowSelectSword = SMALL;
}