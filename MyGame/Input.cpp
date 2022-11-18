#include "Input.h"
#include"WinApp.h"
#include <cmath>

#define STICK_MAX 32768.0f

//#pragma comment(lib,"dxguid.lib")
Input* Input::GetInstance()
{
	static Input instance;

	return &instance;
}

bool Input::TriggerButton(XBOX Button)
{
	//�g���K�[
	if (Button == LT)
	{
		return oldXinputState.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD
			&& CheckTrigger(Button);
	}
	if (Button == RT)
	{
		return oldXinputState.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD
			&& CheckTrigger(Button);
	}
	return !(oldXinputState.Gamepad.wButtons & Button)
		&& CheckTrigger(Button);
	//assert(0);
	return false;
}

bool Input::CheckTrigger(XBOX Button)
{
	if (Button == LT)
	{
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState.Gamepad.bLeftTrigger;
	}
	if (Button == RT)
	{
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState.Gamepad.bRightTrigger;
	}
	return xinputState.Gamepad.wButtons & Button;
	//assert(0);
	return false;
}

Input::~Input()
{
	//delete 	Input::GetInstance();
	//	dinput.Reset();
	devkeyboard.Reset();
	devMouse.Reset();
}

void Input::Initialize(WinApp* winapp)
{
	HRESULT result;
	//winapp�̃C���X�^���X���L�^
	this->winapp = winapp;
	//DirectInput�̃C���X�^���X����
	ComPtr<IDirectInput8> dinput = nullptr;
	result = DirectInput8Create(winapp->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, &dinput, nullptr);
	//�L�[�{�[�h�f�o�C������
	//ComPtr<IDirectInputDevice8>devkeyboard;
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, nullptr);

	// �}�E�X�f�o�C�X�̐���	
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, nullptr);

	//���̓f�[�^�`���̃Z�b�g
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
	//�r�����䃌�x���̃Z�b�g
	result = devkeyboard->SetCooperativeLevel(winapp->GetHwnd(),
	                                          DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	//mouse
	// ���̓f�[�^�`���̃Z�b�g
	result = devMouse->SetDataFormat(&c_dfDIMouse2); // �W���`��

	// �r�����䃌�x���̃Z�b�g
	result = devMouse->SetCooperativeLevel(winapp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Input::update()
{
	HRESULT result;
	{
		//�O��̃L�[����ۑ�
		memcpy(keypre, key, sizeof(key));
		//�L�[�{�[�h���̎擾�J�n
		result = devkeyboard->Acquire();
		//�S�L�[�̓��͏��擾
		//BYTE key[256] = {};
		result = devkeyboard->GetDeviceState(sizeof(key), key);
	}
	{
		// �}�E�X
		result = devMouse->Acquire(); // �}�E�X����J�n

		// �O��̓��͂�ۑ�
		mouseStatePre = mouseState;

		// �}�E�X�̓���
		result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	}

	//�R���g���[���[
	oldXinputState = xinputState;
	//	ZeroMemory(&xinputState, sizeof(XINPUT_STATE));

	DWORD dwResult = XInputGetState(0, &xinputState);
	if (dwResult == ERROR_SUCCESS)
	{
		LeftControllerX = static_cast<float>(xinputState.Gamepad.sThumbLX);
		LeftControllerY = static_cast<float>(xinputState.Gamepad.sThumbLY);

		//�R���g���[���[���ڑ�����Ă���
	}
}

bool Input::Pushkey(BYTE keyNumber)
{
	//�w��L�[�������Ă����true��Ԃ�
	if (key[keyNumber])
	{
		return true;
	}
	return false;
}

//�g���K�[
bool Input::TriggerKey(BYTE keyNumber)
{
	//�O��̌��ʂ��O�ō���̉�b���P�̂Ƃ�
	if (!keypre[keyNumber] && key[keyNumber])
	{
		return true;
	}
	return false;
}

bool Input::PushMouseLeft()
{
	// 0�łȂ���Ή����Ă���
	if (mouseState.rgbButtons[0])
	{
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::PushMouseMiddle()
{
	// 0�łȂ���Ή����Ă���
	if (mouseState.rgbButtons[2])
	{
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool Input::TriggerMouseLeft()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre.rgbButtons[0] && mouseState.rgbButtons[0])
	{
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool Input::TriggerMouseMiddle()
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!mouseStatePre.rgbButtons[2] && mouseState.rgbButtons[2])
	{
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

Input::MouseMove Input::GetMouseMove()
{
	MouseMove tmp;
	tmp.lX = mouseState.lX;
	tmp.lY = mouseState.lY;
	tmp.lZ = mouseState.lZ;
	return tmp;
}


bool Input::TiltStick(STICK Stick)
{
	StickPos oldVec;
	StickPos vec;
	//�E������
	bool isLeftStick = Stick <= L_RIGHT;
	if (isLeftStick)
	{
		oldVec.x = oldXinputState.Gamepad.sThumbLX;
		oldVec.y = oldXinputState.Gamepad.sThumbLY;
		vec.x = xinputState.Gamepad.sThumbLX;
		vec.y = xinputState.Gamepad.sThumbLY;
	}
	else
	{
		oldVec.x = oldXinputState.Gamepad.sThumbRX;
		oldVec.y = oldXinputState.Gamepad.sThumbRY;
		vec.x = xinputState.Gamepad.sThumbRX;
		vec.y = xinputState.Gamepad.sThumbRY;
	}
	if (!StickInDeadZone(oldVec))
	{
		return false;
	}
	if (StickInDeadZone(vec))
	{
		return false;
	}

	bool result = false;

	if (Stick % 4 == L_UP)
	{
		result = 0.3f < (vec.y / STICK_MAX);
	}
	else if (Stick % 4 == L_DOWN)
	{
		result = vec.y / STICK_MAX < -0.3f;
	}
	else if (Stick % 4 == L_RIGHT)
	{
		result = 0.3f < (vec.x / STICK_MAX);
	}
	else if (Stick % 4 == L_LEFT)
	{
		result = vec.x / STICK_MAX < -0.3f;
	}

	return result;
}

bool Input::LeftTriggerStick(int stick)
{
	//��
	if (gamePadState.lX < -unresponsive_range && !(oldGamePadState.lX < -unresponsive_range) && stick == Left)
	{
		return true;
	}
	//�E
	if (gamePadState.lX > unresponsive_range && !(oldGamePadState.lX > unresponsive_range) && stick == Right)
	{
		return true;
	}
	//���
	if (gamePadState.lY > unresponsive_range && !(oldGamePadState.lY > unresponsive_range) && stick == Down)
	{
		return true;
	}
	//�O
	if (gamePadState.lY < -unresponsive_range && !(oldGamePadState.lY < -unresponsive_range) && stick == Up)
	{
		return true;
	}

	return false;
}

bool Input::RightTiltStick(int stick)
{
	unresponsive_range = 200;

	//��
	if (gamePadState.lRx < -unresponsive_range && stick == Left)
	{
		return true;
	}
	//�E
	if (gamePadState.lRx > unresponsive_range && stick == Right)
	{
		return true;
	}
	//���
	if (gamePadState.lRy > unresponsive_range && stick == Down)
	{
		return true;
	}
	//�O
	if (gamePadState.lRy < -unresponsive_range && stick == Up)
	{
		return true;
	}

	rightAngle = static_cast<float>(atan2(0 - gamePadState.lRx, 0 - gamePadState.lRy));

	posX = static_cast<float>(gamePadState.lRx);
	posY = static_cast<float>(gamePadState.lRy);
	return false;
}

bool Input::RightTriggerStick(int stick)
{
	//��
	if (gamePadState.lRx < -unresponsive_range && !(oldGamePadState.lRx < -unresponsive_range) && stick == Left)
	{
		return true;
	}
	//�E
	if (gamePadState.lRx > unresponsive_range && !(oldGamePadState.lRx > unresponsive_range) && stick == Right)
	{
		return true;
	}
	//���
	if (gamePadState.lRy > unresponsive_range && !(oldGamePadState.lRy > unresponsive_range) && stick == Down)
	{
		return true;
	}
	//�O
	if (gamePadState.lRy < -unresponsive_range && !(oldGamePadState.lRy < -unresponsive_range) && stick == Up)
	{
		return true;
	}

	return false;
}


bool Input::PushButton(int Button)
{
	for (int i = 0; i < 32; i++)
	{
		if (!(gamePadState.rgbButtons[i] & 0x80))
		{
			continue;
		}

		switch (i)
		{
		case 0:
			is_push[Button_A] = true;
			break;
		case 1:
			is_push[Button_B] = true;
			break;
		case 2:
			is_push[Button_X] = true;
			break;
		case 3:
			is_push[Button_Y] = true;
			break;
		case 4:
			is_push[Button_LB] = true;
			break;
		case 5:
			is_push[Button_RB] = true;
			break;
		case 6:
			is_push[Select] = true;
			break;
		case 7:
			is_push[Start] = true;
			break;
		case 8:
			is_push[Button_LeftStick] = true;
			break;
		case 9:
			is_push[Button_RightStick] = true;
			break;

		default:
			break;
		}
	}

	for (int i = 0; i < Cross_Up; i++)
	{
		if (is_push[i] == true)
		{
			if (is_push[i] == is_push[Button])
			{
				return true;
			}
		}
	}

	return false;
}

bool Input::PushCrossKey(int CrossKey)
{
	if (gamePadState.rgdwPOV[0] != 0xFFFFFFFF)
	{
		switch (gamePadState.rgdwPOV[0])
		{
		case 0:
			is_push[Cross_Up] = true;
			break;
		case 4500:
			is_push[Cross_Up] = true;
			is_push[Cross_Right] = true;
			break;
		case 9000:
			is_push[Cross_Right] = true;
			break;
		case 13500:
			is_push[Cross_Right] = true;
			is_push[Cross_Down] = true;
			break;
		case 18000:
			is_push[Cross_Down] = true;
			break;
		case 22500:
			is_push[Cross_Down] = true;
			is_push[Cross_Left] = true;
			break;
		case 27000:
			is_push[Cross_Left] = true;
			break;
		case 31500:
			is_push[Cross_Left] = true;
			is_push[Cross_Up] = true;
			break;
		}

		for (int i = Cross_Up; i < ButtonMax; i++)
		{
			if (is_push[i] == true)
			{
				if (is_push[i] == is_push[CrossKey])
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool Input::TriggerCrossKey(int CrossKey)
{
	if (gamePadState.rgdwPOV[0] != 0xFFFFFFFF && oldGamePadState.rgdwPOV[0] == 0xFFFFFFFF)
	{
		switch (gamePadState.rgdwPOV[0])
		{
		case 0:
			is_push[Cross_Up] = true;
			break;
		case 4500:
			is_push[Cross_Up] = true;
			is_push[Cross_Right] = true;
			break;
		case 9000:
			is_push[Cross_Right] = true;
			break;
		case 13500:
			is_push[Cross_Right] = true;
			is_push[Cross_Down] = true;
			break;
		case 18000:
			is_push[Cross_Down] = true;
			break;
		case 22500:
			is_push[Cross_Down] = true;
			is_push[Cross_Left] = true;
			break;
		case 27000:
			is_push[Cross_Left] = true;
			break;
		case 31500:
			is_push[Cross_Left] = true;
			is_push[Cross_Up] = true;
			break;
		}

		for (int i = Cross_Up; i < ButtonMax; i++)
		{
			if (is_push[i] == true)
			{
				if (is_push[i] == is_push[CrossKey])
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool Input::TiltPushStick(STICK Stick, float DeadZone)
{
	StickPos vec;
	StickPos oldVec;

	//�E������
	bool isLeftStick = Stick <= L_RIGHT;
	if (isLeftStick)
	{
		oldVec.x = oldXinputState.Gamepad.sThumbLX;
		oldVec.y = oldXinputState.Gamepad.sThumbLY;

		vec.x = xinputState.Gamepad.sThumbLX;
		vec.y = xinputState.Gamepad.sThumbLY;
	}
	else
	{
		oldVec.x = oldXinputState.Gamepad.sThumbRX;
		oldVec.y = oldXinputState.Gamepad.sThumbRY;

		vec.x = xinputState.Gamepad.sThumbRX;
		vec.y = xinputState.Gamepad.sThumbRY;
	}

	//if (!StickInDeadZone(oldVec))return false;
	if (StickInDeadZone(vec))
	{
		return false;
	}

	bool result = false;
	if (Stick % 4 == L_UP)
	{
		result = DeadZone < (vec.y / STICK_MAX);
	}
	else if (Stick % 4 == L_DOWN)
	{
		result = vec.y / STICK_MAX < -DeadZone;
	}
	else if (Stick % 4 == L_RIGHT)
	{
		result = DeadZone < (vec.x / STICK_MAX);
	}
	else if (Stick % 4 == L_LEFT)
	{
		result = vec.x / STICK_MAX < -DeadZone;
	}

	return result;
}

bool Input::StickInDeadZone(StickPos& Thumb)
{
	bool x = false, y = false;
	if ((Thumb.x < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 1.0f
		&& Thumb.x > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 1.0f))
	{
		Thumb.x = 0.0f;
		x = true;
	}
	if ((Thumb.y < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 1.0f
		&& Thumb.y > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * 1.0f))
	{
		Thumb.y = 0.0f;
		y = true;
	}
	if (x && y)
	{
		return true;
	}
	return false;
}
