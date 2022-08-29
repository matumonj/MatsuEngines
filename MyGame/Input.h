#pragma once
#include<Windows.h>
#include<wrl.h>
#include<dinput.h>
#include"WinApp.h"
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
using namespace Microsoft::WRL;


class Input
{
private:

	ComPtr<IDirectInputDevice8> devkeyboard;
	BYTE key[256] = {};
	BYTE keypre[256] = {};


	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};

	WinApp* winapp = nullptr;
public:
	static Input* GetInstance();
	~Input();
	//namespace�ȗ�
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};
	
	//�Q�[���p�b�h�i�{�^���j
	enum ButtonKind
	{
		Button_A,
		Button_B,
		Button_X,
		Button_Y,
		Button_LB,
		Button_RB,
		Select,
		Start,
		Button_LeftStick,
		Button_RightStick,
		Cross_Up,
		Cross_Down,
		Cross_Right,
		Cross_Left,
		ButtonMax
	};

	//�X�e�B�b�N
	enum StickKind
	{
		Up, Down, Right, Left
	};

	//������
	void Initialize(WinApp* winapp);
	//�X�V
	void update();
	//�L�[�̉������`�F�b�N
	bool Pushkey(BYTE keyNumber);
	//�L�[�̃g���K�[����
	bool TriggerKey(BYTE keyNumber);


	/// <summary>
	/// �L�[�̍��{�^���������`�F�b�N
	/// </summary>
	/// <returns>������Ă��邩</returns>
	bool PushMouseLeft();

	/// <summary>
	/// �L�[�̒��{�^���������`�F�b�N
	/// </summary>
	/// <returns>������Ă��邩</returns>
	bool PushMouseMiddle();

	/// <summary>
	/// �L�[�̍��{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns>�g���K�[��</returns>
	bool TriggerMouseLeft();

	/// <summary>
	/// �L�[�̒��{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns>�g���K�[��</returns>
	bool TriggerMouseMiddle();

	/// <summary>
	/// �}�E�X�ړ��ʂ��擾
	/// </summary>
	/// <returns>�}�E�X�ړ���</returns>
	MouseMove GetMouseMove();

public:
	//�Q�[���p�b�h
	bool LeftTiltStick(int stick);
	bool LeftTriggerStick(int stick);
	bool RightTiltStick(int stick);
	bool RightTriggerStick(int stick);
	bool PushButton(int Button);
	bool TriggerButton(int Button);
	bool PushCrossKey(int CrossKey);
	bool TriggerCrossKey(int CrossKey);
public:
	const LONG& GetPositionX() { return  gamePadState2.lX; }
	const LONG& GetPositionY() { return  gamePadState2.lY; }
	const float& GetPosX() { return posX; }
	const float& GetPosY() { return posY; }

	private:
		//�Q�[���p�b�h�f�o�C�X
		ComPtr<IDirectInputDevice8> devGamePad;
		DIJOYSTATE2 gamePadState = {};
		DIJOYSTATE2 oldGamePadState = {};
		DIJOYSTATE2 gamePadState2 = {};
		DIJOYSTATE2 oldGamePadState2 = {};
		bool is_push[32] = {};
		//�X�e�B�b�N�̖������͈�
		LONG unresponsive_range = 640;
		LONG unresponsive_range2 = 200;
		float posX = 0;
		float posY = 0;

};

