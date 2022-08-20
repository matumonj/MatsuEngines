#pragma once
#include"Sprite.h"
#include"Input.h"
class TutorialSprite
{
public:
	static TutorialSprite* GetInstance();
private:
	static const int TaskNum = 6;
	Sprite* Task[TaskNum];
	enum TaskMenu {//下の配列用列挙型とかぶらないように
		THELLO,
		TMOVE,
		TSETTING,
		TATTACK,
		TGETKEY,
		TEND
	};
	enum {
		HELLO,
		WALK,
		ATTAK,
		SETTING,
		GETKEY,
		CLEAR,
	};
	float SpriteSizeX[TaskNum] = { 0 };
	float SpriteAlpha[TaskNum];
	float t[TaskNum];
	TaskMenu task = THELLO;
	float Movement = 0;
	bool Jump;
	bool ClearWalk;
	bool ClearAttack;
	bool ClearSetting;
	bool GetChest;
	bool AllTaskClear;

	bool MassageCheck[TaskNum];
	Input* input;
public:
	void Initialize();

	void Update();

	void Draw();

	void Ease_SpriteSize_Up(float& x,float& t,int index);
	void NextTask(float t,TaskMenu nexttask,bool nextjudg);
};

