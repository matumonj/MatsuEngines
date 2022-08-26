#pragma once
#include"SwordBase.h"
#include<memory>
class SelectSword
{
public:
	enum SwordScale{
		SMALL,
		NORMAL,
		BIG
	};
private:
	SwordScale NowSelectSword=NORMAL;
	int index;
	std::unique_ptr<SwordBase>Sword;

public:
	void SetSmallSword(SwordScale nowsword);
	void SetNormalSword();
	void SetBigSword();
};

