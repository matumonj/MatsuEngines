#pragma once
#include"SwordBase.h"
#include"Sprite.h"
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
	Sprite*SwordSample[3];
	//XMFLOAT2 Position;
	SwordScale NowSelectSword=NORMAL;
	int index;
	std::unique_ptr<SwordBase>Sword;

public:
	void Initialize();

	void Update();

	void Draw();
	void SetSmallSword(SwordScale nowsword);
	void SetNormalSword();
	void SetBigSword();
};

