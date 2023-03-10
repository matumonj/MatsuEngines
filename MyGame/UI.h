#pragma once
#include"DebugCamera.h"
#include"HUD.h"

class UI
{
private:
	bool TurnOffDrawUI;
	std::vector<Enemy*> Subenemys;
public:
	static UI* GetIns();

	void Initialize();

	void HUDUpdate(bool& hudload, DebugCamera* camera);

	void Update(bool& hudload, DebugCamera* camera);

	void Finalize();
	void HUDDraw();
	void Draw();
	void AreaNameDraw();
public:
	void SetTurnoffUIDraw(bool f) { TurnOffDrawUI = f; }
	bool GetTurnoffUIDraw() { return TurnOffDrawUI; }

	void SetRadBlur(bool active) { BlurF = active; }
	float GetBlurPower() { return bloomval; }
private:
	bool BlurF;
	float EaseT;
	float bloomval;
};
