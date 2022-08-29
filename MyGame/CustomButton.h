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
	~CustomButton();
	static CustomButton* GetInstance();
private:
	Input* input;
	bool CustomButtonJudg;
	Sprite* Controller;
	//�R���g���[���[�{�^���摜[�A�N�V������=2][�{�^����=4]
	Sprite* ButtonSprite[4][4];
	Sprite* SelectSprite;
	Sprite* ErrorSprite;
	static const int menuNum = 4;
	Sprite* MenuSprite[menuNum];
	Sprite* ButtonFrame[menuNum];
	Sprite* Custommenu;
	XMFLOAT2 MenuSpriteSize = { 400,300 };
	XMFLOAT2 JumpSpritePosition = { 100,100 };
	XMFLOAT2 AttackSpritePosition = { 100,300 };
	XMFLOAT2 SAttackSpritePosition = { 100,500 };
	XMFLOAT2 TAttackSpritePosition = { 100,700 };
	XMFLOAT2 SelectSpritePosition[menuNum] = {
		{JumpSpritePosition.x - 100,JumpSpritePosition.y},
		{AttackSpritePosition.x - 100, AttackSpritePosition.y},
		{SAttackSpritePosition.x - 100, SAttackSpritePosition.y},
		{TAttackSpritePosition.x - 100, TAttackSpritePosition.y}
	};
private:
	XMFLOAT3 ButtonPosition[4] = {};
	int SelectNum = JUMP;
	int i = 0;
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
	//�ݒ蔽�f����
	void ConsiderationButton(int index);
	//�R���g���[���[�{�^���`��
	void Custom_Draw();

	void ErrorJudg();
	//�{�^���d���`�F�b�N�p
	bool Check();

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
		SATTACK=2,
		TATTACK=3,
		NONE,
	};
private:
	//�A�N�V��������
	static const int ActionMax = 4;
	//�W�����v�{�^���ݒ�t���O
	bool JumpCustomFlag;
	bool AttackCustomFlag;
	bool SAttackCustomFlag;
	bool TAttackCustomFlag;
	bool ErrorFlag;
	//�G���[�X�v���C�g�̃A���t�@�l
	float ErrorSpriteAlpha=0;
	enum SelectButton {//�ǂ̃A�N�V�����ɂǂ̃{�^�����蓖�Ă�ꂽ���ۑ�������
		SBUTTON_Y,
		SBUTTON_X,
		SBUTTON_A,
		SBUTTON_B
	};
	//�e�A�N�V�����̍\����(�Ή��{�^���ƁA�����t���O)
	struct ActionButton
	{
		//���I������Ă���{�^��
		SelectButton selectButton_Before;
		SelectButton OldselectButton;
		bool judg = false;
	};
	ActionButton actionButton[ActionMax];

public:
	SelectButton GetActionButton_JUMP() {
		return actionButton[JUMP].selectButton_Before;
	}
	SelectButton GetActionButton_ATTACK() {
		return actionButton[ATTACK].selectButton_Before;
	}
	SelectButton GetActionButton_ATTACK2() {
		return actionButton[SATTACK].selectButton_Before;
	}
	SelectButton GetActionButton_ATTACK3() {
		return actionButton[TATTACK].selectButton_Before;
	}
	bool GetJumpAction() { return actionButton[JUMP].judg; }
	bool GetAttackAction() { return actionButton[ATTACK].judg; }
	void SetJumpAction(bool flag) { actionButton[JUMP].judg = flag; }
};

