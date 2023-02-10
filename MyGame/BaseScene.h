#pragma once
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")

#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include "LightGroup.h"
#include <ControlBase.h>

#include "PostEffect.h"
class SceneManager;

class BaseScene
{
public:
	BaseScene(SceneManager* sceneManager);
	//~BaseScene();
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize() = 0;

protected:
	virtual void MyGameDraw()=0;
	virtual void SpriteDraw() = 0;
	virtual void LightUpdate()=0;
	void LightSetParam(int index,bool actflag,XMFLOAT3 pos,XMVECTOR dir,XMFLOAT3 atten,XMFLOAT2 angle);
protected:
	//光源情報
	LightGroup* lightGroup = nullptr;
	Input* input;
	//エフェクト用(ただプログラムでつくれるものはプログラムで作る方がいい　多用はいくない)
	enum
	{
		Blur,
		Default,
	};
	int c_postEffect = Default;
	PostEffect* postEffect = nullptr;
	std::vector<ControlBase*> AllObjectControl;
protected:
	SceneManager* sceneManager_ = nullptr;
};
