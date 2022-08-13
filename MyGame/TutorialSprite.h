#pragma once
#include"Sprite.h"
class TutorialSprite
{
private:
	static const int TaskNum = 5;
	Sprite* Task[TaskNum];
	enum Tasks{
		HELLO,
		WALK,
		ATTAK,
		SETTING,
		CLEAR
	};
	Tasks task = HELLO;

public:
	void Initialize();

	void Update();

	void Draw();
};

