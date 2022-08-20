#pragma once
#include"Sprite.h"
#include"Input.h"
class TutorialSprite
{
public:
	static TutorialSprite* GetInstance();
private:
	static const int TaskNum = 5;
	Sprite* Task[TaskNum];
	enum Tasks{
		HELLO,
		WALK,
		ATTAK,
		SETTING,
		GETKEY,
		CLEAR,
	};
	float SpriteAlpha[TaskNum];
	Tasks task = HELLO;
	float Movement = 0;
	bool Jump;
	bool ClearWalk;
	bool ClearAttack;
	bool ClearSetting;
	bool AllTaskClear;
	Input* input;
public:
	void Initialize();

	void Update();

	void Draw();
};

