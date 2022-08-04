#pragma once
#include"Input.h"
#include"Sprite.h"
class CustomButton
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	static CustomButton* GetInstance();
private:
	Input* input;
	bool CustomButtonJudg;
	Sprite* Controller;
	Sprite* ButtonSprite[4];
	Sprite* SelectSprite;
	
	static const int menuNum = 2;
	Sprite* MenuSprite[menuNum];
	XMFLOAT2 MenuSpriteSize = { 300,150 };
	XMFLOAT2 JumpSpritePosition = { 300,200 };
	XMFLOAT2 AttackSpritePosition = { 300,400 };

	XMFLOAT2 SelectSpritePosition[menuNum] = {
		{JumpSpritePosition.x - 100,JumpSpritePosition.y},
		{AttackSpritePosition.x - 100, AttackSpritePosition.y}
	};
private:
	XMFLOAT3 ButtonPosition[4] = {};
	int SelectNum = JUMP;
public:
	void Initialize();

	void Update();

	void Draw();

	void SetCustomButtonJudg(bool flag) { CustomButtonJudg = flag; }
	bool GetCustomButtonJudg() { return CustomButtonJudg; }

	void Custom_JumpButton();
	void Custom_AttackButton();
	void Custom_Draw();
private:

public:
	enum  {
		BUTTON_X=0,
		BUTTON_Y=1,
		BUTTON_A=2,
		BUTTON_B=3
	};

	enum {
		JUMP=0,
		ATTACK=1,
	};
private:
	bool JumpCustomFlag;
	enum SelectButton {
		SBUTTON_Y,
		SBUTTON_X,
		SBUTTON_A,
		SBUTTON_B
	};
	struct ActionButton
	{
		SelectButton selectButton=SBUTTON_A;
		bool judg = false;
	};
	ActionButton actionButton[4];

public:
	bool GetJumpAction() { return actionButton[JUMP].judg; }
	void SetJumpAction(bool flag) { actionButton[JUMP].judg = flag; }
};

