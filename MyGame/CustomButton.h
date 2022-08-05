#pragma once
#include"Input.h"
#include"Sprite.h"
class CustomButton
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
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
	//�R���g���[���[�{�^���摜[�A�N�V������=2][�{�^����=4]
	Sprite* ButtonSprite[2][4];
	Sprite* SelectSprite;
	
	static const int menuNum = 2;
	Sprite* MenuSprite[menuNum];
	Sprite* ButtonFrame[menuNum];
	XMFLOAT2 MenuSpriteSize = { 400,300 };
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
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw();
	//��� Sprite��uniqueptr�g����������**=Sprite::Create(...)�̕����ǂ����邩
	void Fainalize();
	//�{�^���ݒ�
	void Custom_Button(bool customflag,int index);
	//�R���g���[���[�{�^���`��
	void Custom_Draw();
public:
	//Getter
	bool GetCustomButtonJudg() { return CustomButtonJudg; }

	//Setter
	void SetCustomButtonJudg(bool flag) { CustomButtonJudg = flag; }

private:

public:
	enum  {//�e�{�^���ԍ�
		BUTTON_X=0,
		BUTTON_Y=1,
		BUTTON_A=2,
		BUTTON_B=3
	};

	enum {//�e�A�N�V�����ԍ�
		JUMP=0,
		ATTACK=1,
		NONE,
	};
private:
	//�A�N�V��������
	static const int ActionMax = 4;
	//�W�����v�{�^���ݒ�t���O
	bool JumpCustomFlag;
	bool AttackCustomFlag;

	enum SelectButton {//�ǂ̃A�N�V�����ɂǂ̃{�^�����蓖�Ă�ꂽ���ۑ�������
		SBUTTON_Y,
		SBUTTON_X,
		SBUTTON_A,
		SBUTTON_B
	};
	//�e�A�N�V�����̍\����(�Ή��{�^���ƁA�����t���O)
	struct ActionButton
	{
		SelectButton selectButton=SBUTTON_A;
		bool judg = false;
	};
	ActionButton actionButton[ActionMax];

public:
	bool GetJumpAction() { return actionButton[JUMP].judg; }
	bool GetAttackAction() { return actionButton[ATTACK].judg; }
	void SetJumpAction(bool flag) { actionButton[JUMP].judg = flag; }
};

