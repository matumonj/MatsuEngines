#pragma once
#include"Sprite.h"
#include"Input.h"
#include"Texture.h"
class TutorialSprite
{
public:
	~TutorialSprite();
	static TutorialSprite* GetInstance();
private:
	static const int TaskNum = 5;
	Sprite* Task[TaskNum];
	Sprite* notClearTask[5];
	Texture* TargetPoint_Ground;
	Texture* TargetPoint_Icon;
	float TargetIconPosY;
	float TargetPoint_GroundRotY;
	float targetAlpha;
	DirectX::XMFLOAT3 TargetTexSize;
	Sprite* movecn;;
	Sprite* Attackcon;
	DirectX::XMFLOAT2 conpos;
	DirectX::XMFLOAT2 spos;
	float movea;
	float atacka;
	enum CLEARTASK
	{
		MOVE_CHARA,
		MOVE_CAMERA,
		ENEMYDESTROY,
		CONFIG,
		FRAME
	};

	float notTaskXpos[4];

	enum TaskMenu
	{
		//‰º‚Ì”z—ñ—p—ñ‹“Œ^‚Æ‚©‚Ô‚ç‚È‚¢‚æ‚¤‚É

		THELLO,
		TMOVE,
		TATTACK,
		TGETKEY,
		TEND
	};

	enum
	{
		HELLO,
		WALK,
		ATTACK,
		GETKEY,
		CLEAR,
	};

	float SpriteSizeX[TaskNum] = {0};
	float SpriteAlpha[TaskNum];
	float t[TaskNum];
	TaskMenu task = THELLO;
	float Movement = 0;
	int Movement_Camera = 0;
	bool Jump;

	bool AllTaskClear;

	bool ClearTaskJudg[TaskNum];
	bool MassageCheck[TaskNum];
	Input* input;

	void CheckMove_Camera_Player();
	void CheckAttack();

	int MoveTime;
	void TargetPosTexMove();
public:
	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void DrawTargetPos();
	void Ease_SpriteSize_Up(float& x, float& t, int index);
	void NextTask(float t, TaskMenu nexttask, bool nextjudg);

	bool GetClearMove() { return ClearTaskJudg[WALK]; }
	bool GetClearSetting() { return ClearTaskJudg[GETKEY]; }
};
